// credits : @cristeigabriel on github - cristeigabriel at riseup dot net
// for: api-hook by @zer0mem0ry [wrapper]

#include <TlHelp32.h>
#include <Windows.h>

// access point to class
c_hook hook;

// hook declaration & insertion shortened
#define create_hook_api(                                                                                                         \
    HOOK_ACCESS_POINT, MODULE, FUNCTION, HOOKED_FUNCTION, HOOKED_ORIGINAL, HOOKED_DEFINITION, API_FUNCTION )                     \
  hook.init_hook( HOOK_ACCESS_POINT, const_cast<char *>( MODULE ), const_cast<char *>( FUNCTION ), HOOKED_FUNCTION );            \
  HOOKED_ORIGINAL = reinterpret_cast<HOOKED_DEFINITION>( API_FUNCTION );

namespace nt::apihook {
  // to be called on attach
  void initialize_hooks( ) {
    create_hook_api( &hook,
                     "user32.dll",
                     "GetClipboardData",
                     hook_clipboard_data,
                     orig_clipboard_data,
                     func_clipboard_data,
                     hook.internal.api_function );

    hook.insert_hook( &hook );
  }

  // to be called on detach
  void detach_hooks( ) {
    hook.unhook( &hook );
    hook.free_hook( &hook );
  }
} // namespace nt::apihook