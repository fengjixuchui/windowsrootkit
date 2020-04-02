// original credits : https://github.com/Zer0Mem0ry/APIHook
// edited by : @cristeigabriel on github - cristeigabriel at riseup dot net

#include <TlHelp32.h>
#include <Windows.h>

// TODO: Fix usage of insecure methods
// TODO: Consider std::any instead of void*

class c_hook {
public:
  struct internal_t {
    bool is_hooked;
    void * func_addr;
    void * hook;
    char pad[ 6 ];
    char api_bytes[ 6 ];
    void * api_function;
  } internal;

public:
  bool init_hook( c_hook * hook, std::string mod_name, std::string func_name, void * hook_func ) {
    HMODULE module;
    ULONG orig_func, func_addr;
    char opcodes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0xe9, 0x00, 0x00, 0x00, 0x00 };

    if ( hook->internal.is_hooked ) {
      return false;
    }

    module = GetModuleHandle( mod_name.c_str( ) );
    if ( module == NULL ) {
      hook->internal.is_hooked = false;
      return false;
    }

    hook->internal.func_addr = GetProcAddress( module, func_name.c_str( ) );
    if ( hook->internal.func_addr == NULL ) {
      hook->internal.is_hooked = false;
      return false;
    }

    hook->internal.pad[ 0 ] = 0xe9;
    *reinterpret_cast<PULONG>( &hook->internal.pad[ 1 ] ) =
        reinterpret_cast<ULONG>( hook_func ) - reinterpret_cast<ULONG>( hook->internal.func_addr ) - 5;
    memcpy( hook->internal.api_bytes, hook->internal.func_addr, 5 );
    hook->internal.api_function = VirtualAlloc( NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
    if ( hook->internal.api_function == NULL ) {
      return false;
    }
    memcpy( hook->internal.api_function, hook->internal.api_bytes, 5 );
    orig_func = reinterpret_cast<ULONG>( hook->internal.api_function ) + 5;
    func_addr = reinterpret_cast<ULONG>( hook->internal.func_addr ) + 5;
    *reinterpret_cast<LPBYTE>( reinterpret_cast<LPBYTE>( hook->internal.api_function ) + 5 ) = 0xe9;
    *reinterpret_cast<PULONG>( reinterpret_cast<LPBYTE>( hook->internal.api_function ) + 6 ) =
        (ULONG)func_addr - (ULONG)orig_func - 5;
    hook->internal.is_hooked = true;
    return true;
  }

  bool initialize_by_address( c_hook * hook, void * addr, void * hook_func ) {
    ULONG orig_func, func_addr;
    char opcodes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0xe9, 0x00, 0x00, 0x00, 0x00 };
    if ( hook->internal.is_hooked ) {
      return false;
    }
    hook->internal.func_addr = addr;
    hook->internal.pad[ 0 ] = 0xe9;

    *reinterpret_cast<PULONG>( &hook->internal.pad[ 1 ] ) =
        reinterpret_cast<ULONG>( hook_func ) - reinterpret_cast<ULONG>( hook->internal.func_addr ) - 5;
    memcpy( hook->internal.api_bytes, hook->internal.func_addr, 5 );

    hook->internal.api_function = VirtualAlloc( NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
    memcpy( hook->internal.api_function, hook->internal.api_bytes, 5 );

    orig_func = reinterpret_cast<ULONG>( hook->internal.api_function ) + 5;
    func_addr = reinterpret_cast<ULONG>( hook->internal.func_addr ) + 5;

    *reinterpret_cast<LPBYTE>( reinterpret_cast<LPBYTE>( hook->internal.api_function ) + 5 ) = 0xe9;
    *reinterpret_cast<PULONG>( reinterpret_cast<LPBYTE>( hook->internal.api_function ) + 6 ) =
        (ULONG)func_addr - (ULONG)orig_func - 5;
    hook->internal.is_hooked = true;
    return true;
  }

  bool insert_hook( c_hook * hook ) {
    DWORD op;
    if ( !hook->internal.is_hooked ) {
      return false;
    }
    VirtualProtect( hook->internal.func_addr, 5, PAGE_EXECUTE_READWRITE, &op );
    memcpy( hook->internal.func_addr, hook->internal.pad, 5 );
    VirtualProtect( hook->internal.func_addr, 5, op, &op );
    return true;
  }

  bool unhook( c_hook * hook ) {
    DWORD op;
    if ( !hook->internal.is_hooked ) {
      return false;
    }
    VirtualProtect( hook->internal.func_addr, 5, PAGE_EXECUTE_READWRITE, &op );
    memcpy( hook->internal.func_addr, hook->internal.api_bytes, 5 );
    VirtualProtect( hook->internal.func_addr, 5, op, &op );

    hook->internal.is_hooked = false;
    return true;
  }

  bool free_hook( c_hook * hook ) {
    if ( hook->internal.is_hooked ) {
      return false;
    }
    VirtualFree( hook->internal.api_function, 0, MEM_RELEASE );
    memset( hook, 0, sizeof( c_hook * ) );
    return true;
  }

public:
  // for hooking stuff that requires OpenProcess and functions alike
  unsigned long attach( char * name ) {
    HANDLE handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof( entry );
    do
      if ( !strcmp( entry.szExeFile, name ) ) {
        CloseHandle( handle );
        return entry.th32ProcessID;
      }
    while ( Process32Next( handle, &entry ) );
    return false;
  }
};