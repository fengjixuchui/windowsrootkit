// original credits : https://github.com/Zer0Mem0ry/APIHook
// edited by : @cristeigabriel on github - cristeigabriel at riseup dot net

#include <TlHelp32.h>
#include <Windows.h>
#include <fstream>
#include <string>

// <winuser.h> -- https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclipboarddata
typedef HANDLE( WINAPI * func_clipboard_data )( _In_ UINT u_format );

// original hook
func_clipboard_data orig_clipboard_data;

// hook in itself
HANDLE WINAPI hook_clipboard_data( _In_ UINT u_format ) {
  // stream is being open to 'C:\Dumps\clipboard_type.txt'.
  std::ofstream output( "C:\\Dumps\\clipboard_type.txt", std::ofstream::in | std::ofstream::out | std::ofstream::binary );

  // I declare a buffer to which I will append the argument from the hook to retrieve it.
  std::string buffer;
  buffer.append( std::to_string( u_format ) );

  // writing the buffer to the output file.
  output.write( buffer.c_str( ), buffer.size( ) );

  // debug message that displays the data we retrieved and that the hook/action was succesful.
  std::cout << "[ * ] hook_clipboard_data: hijack succesful -> CLIPBOARD DATA FORMAT [u_format]: " << buffer << std::endl;

  return orig_clipboard_data( u_format );
}