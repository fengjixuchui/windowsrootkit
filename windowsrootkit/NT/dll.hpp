#include <TlHelp32.h>
#include <Windows.h>
#include <thread>

namespace nt {
  void terminate_dll_on( int key ) {
    if ( !GetAsyncKeyState( key ) )
      std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
  }
} // namespace nt