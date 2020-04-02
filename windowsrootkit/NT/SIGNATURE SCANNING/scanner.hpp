// credits: paracord / @mooncaller

#include <TlHelp32.h>
#include <Windows.h>
#include <memory.h>
#include <memory>
#include <string>
#include <string_view>

namespace nt::memory {
  namespace masking {
    constexpr std::uint8_t char_to_hex( char c ) { return c <= '9' ? c - '0' : ( c | ' ' ) - 'a' + 10; }

    template <typename T, size_t N, size_t... Idx>
    constexpr std::array<T, N + 1> add_to_front( std::array<T, N> arr, T val, std::index_sequence<Idx...> ) {
      return std::array<T, N + 1>{ val, arr[ Idx ]... };
    }

    template <std::uint32_t C, std::uint32_t N, const char * Pattern, class Enable = void> struct ida_mask_helper_t {
      using next_mask_helper = ida_mask_helper_t<C + 2, N, Pattern>;
      constexpr static auto length = next_mask_helper::length + 1;

      constexpr static auto pattern =
          add_to_front<std::uint8_t>( next_mask_helper::pattern,
                                      char_to_hex( Pattern[ C ] ) * 16 + char_to_hex( Pattern[ C + 1 ] ),
                                      std::make_index_sequence<length - 1>( ) );
      constexpr static auto wildcard = add_to_front( next_mask_helper::wildcard, false, std::make_index_sequence<length - 1>( ) );
    };

    template <std::uint32_t C, std::uint32_t N, const char * Pattern>
    struct ida_mask_helper_t<C, N, Pattern, std::enable_if_t<C >= N>> {
      constexpr static auto length = 1;

      constexpr static std::array<std::uint8_t, 1> pattern{ 0 };
      constexpr static std::array<bool, 1> wildcard{ true };
    };

    template <std::uint32_t C, std::uint32_t N, const char * Pattern>
    struct ida_mask_helper_t<C, N, Pattern, std::enable_if_t<Pattern[ C ] == '?'>> {
      using next_mask_helper = ida_mask_helper_t<C + 1, N, Pattern>;
      constexpr static auto length = next_mask_helper::length + 1;

      constexpr static auto pattern =
          add_to_front( next_mask_helper::pattern, static_cast<std::uint8_t>( 0 ), std::make_index_sequence<length - 1>( ) );
      constexpr static auto wildcard = add_to_front( next_mask_helper::wildcard, true, std::make_index_sequence<length - 1>( ) );
    };

    template <std::uint32_t C, std::uint32_t N, const char * Pattern>
    struct ida_mask_helper_t<C, N, Pattern, std::enable_if_t<Pattern[ C ] == ' '>> {
      using next_mask_helper = ida_mask_helper_t<C + 1, N, Pattern>;
      constexpr static auto length = next_mask_helper::length;

      constexpr static auto pattern = next_mask_helper::pattern;
      constexpr static auto wildcard = next_mask_helper::wildcard;
    };

    template <std::uint32_t N, const char Pattern[ N ]> struct ida_mask_t { using value = ida_mask_helper_t<0, N - 1, Pattern>; };
  } // namespace masking

  template <typename mask> constexpr std::uint8_t * find_signature( const std::string_view module_name ) {
    const auto module_base = reinterpret_cast<std::uint8_t *>( GetModuleHandleA( module_name.data( ) ) );

    if ( !module_base )
      return nullptr;

    const auto module_size =
        reinterpret_cast<PIMAGE_NT_HEADERS>( module_base + reinterpret_cast<PIMAGE_DOS_HEADER>( module_base )->e_lfanew )
            ->OptionalHeader.SizeOfImage;

    auto signature = mask::pattern;
    auto signature_len = mask::length;
    auto wildcards = mask::wildcard;

    for ( auto i = 0ul; i < module_size - signature_len; i++ ) {
      auto found = true;

      for ( auto j = 0ul; j < signature_len; j++ ) {
        if ( !wildcards[ j ] && signature[ j ] != module_base[ i + j ] ) {
          found = false;
          break;
        }
      }

      if ( found )
        return module_base + i;
    }

    return nullptr;
  }
} // namespace nt::memory

// TODO: store all signatures in an unordered_map, access them accordingly with indexes that are sorted in an enum, example: enum
// { D3D_SIGNATURE = 0, POSTPROCESS_SIGNATURE = 1 } and then do map.at(index) = signature;

// at usage, a member with the name 'name' will be initialized, scanning for 'sig' in 'module', it will be used in your
// declaration of the member you want to use but have to sigscan for, adding offsets to it will work like: m_device = name +
// offset;
#define find_signature( name, mod, sig )                                                                                         \
  const auto n = util::memory::find_signature<util::memory::masking::ida_mask_t<sizeof( s ), s>::value>( x );