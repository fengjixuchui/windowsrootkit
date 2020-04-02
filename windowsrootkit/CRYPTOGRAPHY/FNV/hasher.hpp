// made by: @cristeigabriel, @T0b1-iOS

#include <cmath>
#include <iostream>
#include <string>
#include <string_view>

namespace crypt::hash {
  std::uint32_t offset_basis = 0x411c7723;

  // hashing function for null-terminated char/wchar strings
  template <typename str_type,
            typename = std::enable_if_t<std::is_same_v<str_type, const char *> || std::is_same_v<str_type, const wchar_t *>, int>>
  constexpr std::uint32_t hash( const str_type str ) noexcept {
    std::uint32_t value = offset_basis;
    while ( *str ) {

      value ^= *str++;
      value *= prime;
    }

    return value;
  }

  // hashing function for char/wchar arrays that may be null-terminated
  template <typename char_type,
            std::size_t arr_len,
            typename = std::enable_if<std::is_same_v<char_type, char> || std::is_same_v<char_type, wchar_t>, int>>
  constexpr std::uint32_t hash( const char_type ( &str )[ arr_len ] ) noexcept {
    std::uint32_t value = offset_basis;
    for ( auto i = 0u; i < arr_len && str[ i ]; ++i ) {
      value ^= str[ i ];
      value *= prime;
    }

    return value;
  }

  // hashing function for string/wstring_views
  template <
      typename view_type,
      typename =
          std::enable_if_t<std::is_same_v<view_type, std::string_view> || std::is_same_v<view_type, std::wstring_view>, int>>
  constexpr std::uint32_t hash_view( const view_type str ) noexcept {

    std::uint32_t value = offset_basis;
    for ( const auto c : str ) {
      value ^= c;
      value *= prime;
    }

    return value;
  }
} // namespace crypt::hash

#define FNV( type, STR_ ) crypt::hash::fnv<type>( STR_ )