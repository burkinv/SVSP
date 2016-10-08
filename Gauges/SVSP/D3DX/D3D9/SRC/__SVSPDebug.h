#ifndef __SVSP_DEBUG_H__
#define __SVSP_DEBUG_H__

#include <stdio.h>
#include <assert.h>
#include <windows.h>

class __SVSPDEBUG
{
public:
  static const CHAR* CONSOLE_BUILD_VERSION;
  static const SHORT CONSOLE_BUFFER_WIDTH;
  static const SHORT CONSOLE_BUFFER_HEIGHT;
  static const SHORT CONSOLE_WINDOW_WIDTH;
  static const SHORT CONSOLE_WINDOW_HEIGHT;

  /** Outputs into __SVSPDEBUG Loging console */
  static inline VOID OUTPUT(const CHAR *pcModuleName, const CHAR *pcStr)
  {
    SYSTEMTIME kSysTime;
    GetSystemTime( &kSysTime );
    CHAR acStr[512];
    sprintf_s( &acStr[0], sizeof(acStr), "%02d:%02d:%02d [%*s]: %s\n", kSysTime.wHour, 
                                                                       kSysTime.wMinute, 
                                                                       kSysTime.wSecond, 
                                                                       sizeof("__SVSPTextureRenderer_D3D9"), 
                                                                       pcModuleName, 
                                                                       pcStr);
    printf( &acStr[0] );
  }

  static inline VOID ASSERTION_FAILURE(const CHAR *pcModuleName, const CHAR *pcFilename, const INT iLineNum)
  {
    SYSTEMTIME kSysTime;
    GetSystemTime( &kSysTime );
    CHAR acStr[512];
    sprintf_s( &acStr[0], sizeof(acStr), "%02d:%02d:%02d [%*s]: ASSERTION FAILURE IN %s:%d\n", kSysTime.wHour, 
                                                                                               kSysTime.wMinute, 
                                                                                               kSysTime.wSecond, 
                                                                                               sizeof("__SVSPTextureRenderer_D3D9"), 
                                                                                               pcModuleName, 
                                                                                               pcFilename, 
                                                                                               iLineNum);
    printf( &acStr[0] );
  }

  /** Creates __SVSPDEBUG Loging console */
  static       VOID  CreateConsole();

  /** Deletes __SVSPDEBUG Loging console */
  static       VOID  DeleteConsole();


#ifdef _DEBUG

#define AAV_ASSERT(b) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  /* assert(false); */\
  } \

#define AAV_CHECK_CONTINUE(b) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  continue; \
  } \

#define AAV_ASSERT_CONTINUE(b) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  Sleep(100); \
  continue; \
  } \

#define AAV_ASSERT_RETURN(b, r) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  return r; \
  } \

#define AAV_ASSERT_RETURN_VOID(b) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  return; \
  } \

#else

#define AAV_ASSERT(b) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  /* assert(false); */\
  } \

#define AAV_CHECK_CONTINUE(b) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  continue; \
  } \

#define AAV_ASSERT_CONTINUE(b) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  Sleep(100); \
  continue; \
  } \

#define AAV_ASSERT_RETURN(b, r) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  return r; \
  } \

#define AAV_ASSERT_RETURN_VOID(b) \
  if(!(b)) \
  { \
  __SVSPDEBUG::ASSERTION_FAILURE("__SVSPDEBUG", __FILE__, __LINE__ ); \
  return; \
  } \

#endif //_DEBUG

protected:
  static bool m_bConsoleAllocated;

};

#endif	/* __SVSP_DEBUG_H__ */


