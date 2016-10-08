#include <fcntl.h>
#include <io.h>
#include <__SVSPDebug.h>

const CHAR* __SVSPDEBUG::CONSOLE_BUILD_VERSION = "0.77";
const SHORT __SVSPDEBUG::CONSOLE_BUFFER_WIDTH  = 150; //70;
const SHORT __SVSPDEBUG::CONSOLE_BUFFER_HEIGHT = 700;
const SHORT __SVSPDEBUG::CONSOLE_WINDOW_WIDTH  = CONSOLE_BUFFER_WIDTH - 1;
const SHORT __SVSPDEBUG::CONSOLE_WINDOW_HEIGHT = 25;

bool __SVSPDEBUG::m_bConsoleAllocated = false;

VOID __SVSPDEBUG::CreateConsole()
{
  BOOL bResult = 0;
  if (!m_bConsoleAllocated)
  {
    AllocConsole();

    CHAR acStr[512];
  #ifdef _DEBUG
    sprintf_s(&acStr[0], sizeof(acStr), "__SVSPDEBUG: __SVSPAvionics(c) Build: %s %s : WIN32 | %s | D3D9", __SVSPDEBUG::CONSOLE_BUILD_VERSION, __TIMESTAMP__, "DEBUG");
  #else
    sprintf_s(&acStr[0], sizeof(acStr), "__SVSPDEBUG: __SVSPAvionics(c) Build: %s %s : WIN32 | %s | D3D9", __SVSPDEBUG::CONSOLE_BUILD_VERSION, __TIMESTAMP__, "RELEASE");
  #endif //_DEBUG

    bResult = SetConsoleTitle(&acStr[0]);
    AAV_ASSERT( bResult == 1 );

    INT hSTDConsoleHandle = reinterpret_cast<INT>(GetStdHandle( STD_OUTPUT_HANDLE ));
    INT  hAAVConsoleHandle = _open_osfhandle( hSTDConsoleHandle, _O_TEXT );
    FILE *fp = _fdopen( hAAVConsoleHandle, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    CONSOLE_SCREEN_BUFFER_INFO kAAVConsoleBufferInfo;
    bResult = GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &kAAVConsoleBufferInfo );
    AAV_ASSERT( bResult == 1 );

    SMALL_RECT r;
    COORD      c;

    c.X = 2000;
    c.Y = 2000;
    if(SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), c ) == TRUE)
    {
      r.Left   = 0;
      r.Top    = 0;
      r.Right  = CONSOLE_WINDOW_WIDTH;
      r.Bottom = CONSOLE_WINDOW_HEIGHT;
      if(SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), TRUE, &r ) == TRUE)
      {
        c.X = CONSOLE_BUFFER_WIDTH;
        c.Y = CONSOLE_BUFFER_HEIGHT;
        bResult = SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), c );
        AAV_ASSERT( bResult == TRUE );

      }
      else
      {
        AAV_ASSERT( FALSE );
      }
    }
    else
    {
      AAV_ASSERT( FALSE );
    }

    m_bConsoleAllocated = true;
  }
}

VOID __SVSPDEBUG::DeleteConsole()
{
  BOOL bResult = 0;

  bResult = FreeConsole();
  AAV_ASSERT( bResult == 1 );

  m_bConsoleAllocated = false;
}
