#include <Windows.h>
#include <iostream>

namespace nt::console {
  // initialize console with the window name and first message.
  void initialize_console( const char * name, const char * message ) {
    AllocConsole( );
    const HWND console = GetConsoleWindow( );
    freopen_s( reinterpret_cast<FILE **>( stdin ), "CONIN$", "r", stdin );
    freopen_s( reinterpret_cast<FILE **>( stdout ), "CONOUT$", "w", stdout );

    // window [ x pos, y pos, width, height ]
    RECT rect = { 100, 100, 600, 600 };
    MoveWindow( console, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE );
     
    // check WINAPI for more styles
    LONG_PTR style = GetWindowLongPtr( console, GWL_STYLE );
    SetWindowLong( console, GWL_STYLE, style & ~WS_SIZEBOX & ~WS_SYSMENU );

    SetConsoleTitle( name );
    Beep( 10, 1000 );
    std::cout << message << std::endl;
  }

  // destroys console, by free-ing what was allocated within its buffer.
  void destroy_console( ) {
    fclose( reinterpret_cast<FILE *>( stdin ) );
    fclose( reinterpret_cast<FILE *>( stdout ) );

    const HWND console = GetConsoleWindow( );

    FreeConsole( );
    PostMessageW( console, WM_CLOSE, 0, 0 );
    Beep( 10, 1000 );
  }

} // namespace nt::console