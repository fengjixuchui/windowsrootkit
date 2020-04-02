// inspired from stackoverflow

#include <TlHelp32.h>
#include <Windows.h>
#include <atlstr.h>
#include <atlimage.h>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <string_view>
#include <vector>
#include <thread>

// TODO: use this_thread::sleep_for

// enough documentation should be here
// https://docs.microsoft.com/en-us/cpp/atl-mfc-shared/reference/cimage-class?view=vs-2019

namespace rkit::screenshot {
  void take_screenshot( const std::string & path ) {
    // press prntscrn, release prntscrn
    keybd_event( VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY, 0 );
    keybd_event( VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );

    // handler of the bitmap that saves the screen shot
    HBITMAP bitmap;

    // wait until the screenshot is taken
    Sleep( 100 );

    // take the screenshot from the clipboard
    OpenClipboard( NULL );

    // save the screenshot in the bitmap handler
    bitmap = reinterpret_cast<HBITMAP>( GetClipboardData( CF_BITMAP ) );

    // release the screenshot from clipboard data memory
    CloseClipboard( );

    // initializations
    std::vector<BYTE> buffer;
    IStream * stream = nullptr;
    HRESULT hr = CreateStreamOnHGlobal( 0, TRUE, &stream );
    CImage image;
    ULARGE_INTEGER liSize;

    // make the screenshot a jpg and pass it to the stream
    image.Attach( bitmap );
    image.Save( stream, Gdiplus::ImageFormatJPEG );
    IStream_Size( stream, &liSize );
    DWORD len = liSize.LowPart;
    IStream_Reset( stream );
    buffer.resize( len );
    IStream_Read( stream, &buffer[ 0 ], len );
    stream->Release( );

    // put the imapge in the file
    std::ofstream fi;
    fi.open( path, std::ofstream::in | std::fstream::out | std::ofstream::binary | std::ofstream::app );
    fi.write( reinterpret_cast<const char *>( &buffer[ 0 ] ), buffer.size( ) * sizeof( BYTE ) );
    std::cout << "[ * ] Screenshot taken at " << path << std::endl;
    fi.close( );
  }
} // namespace rkit::screenshot