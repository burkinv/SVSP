#ifndef __SVSP_COLOR_H__
#define __SVSP_COLOR_H__

#include <WINDOWS.h>

namespace __SVSPColor
{
#if   defined(LIGHT_GRAY_ALPHA)
  /////////////////////////////////////////////////////
  //Light Gray | Alpha
  /////////////////////////////////////////////////////
  static const DWORD  dwCOLOR           = 0x55f7f7f7;
  static const DWORD  dwHUDCOLOR        = 0xCC03feef;
  static const DWORD  dwWARNINGCOLOR    = 0x44f7f7f7;
  static const DWORD  dwGUICOLOR        = 0x55f7f7f7;
  static const DWORD  dwACTIVEGUICOLOR  = 0xCCf7f7f7;
  static const DWORD  dwBACKGROUNDCOLOR = 0x55f7f7f7;
  static const DWORD  dwACTIVEWINDOWCOLOR = 0xCCd8e000;

#elif defined(LIGHT_GREEN1_ALPHA)
  /////////////////////////////////////////////////////
  // Light Green | Alpha
  /////////////////////////////////////////////////////
  static const DWORD  dwCOLOR           = 0x7714cc06;
  static const DWORD  dwHUDCOLOR        = 0xCC03feef;
  static const DWORD  dwWARNINGCOLOR    = 0x4414cc06;
  static const DWORD  dwGUICOLOR        = 0x7714cc06;
  static const DWORD  dwACTIVEGUICOLOR  = 0xCC14cc06;
  static const DWORD  dwBACKGROUNDCOLOR = 0x7714cc06;
  static const DWORD  dwACTIVEWINDOWCOLOR = 0xCCd8e000;

#elif defined(LIGHT_GREEN2_ALPHA)
  /////////////////////////////////////////////////////
  // Light Green | Alpha
  /////////////////////////////////////////////////////
  static const DWORD  dwCOLOR           = 0xCC4ab503;
  static const DWORD  dwHUDCOLOR        = 0xCC03feef;
  static const DWORD  dwWARNINGCOLOR    = 0x444ab503;
  static const DWORD  dwGUICOLOR        = 0x774ab503;
  static const DWORD  dwACTIVEGUICOLOR  = 0xCC4ab503;
  static const DWORD  dwBACKGROUNDCOLOR = 0x774ab503;
  static const DWORD  dwACTIVEWINDOWCOLOR = 0xCCd8e000;

#elif defined(LIGHT_GREEN3_ALPHA)
  /////////////////////////////////////////////////////
  // Light Green | Alpha
  /////////////////////////////////////////////////////
  static const DWORD  dwCOLOR             = 0xCC2cfe03;
  static const DWORD  dwHUDCOLOR          = 0xCC03feef;
  static const DWORD  dwWARNINGCOLOR      = 0x442cfe03;
  static const DWORD  dwGUICOLOR          = 0x772cfe03;
  static const DWORD  dwACTIVEGUICOLOR    = 0xCC2cfe03;
  static const DWORD  dwBACKGROUNDCOLOR   = 0x772cfe03;
  static const DWORD  dwACTIVEWINDOWCOLOR = 0xCCd8e000;

#elif defined(LIGHT_BLUE_ALPHA)
  /////////////////////////////////////////////////////
  // Light Blue | Alpha
  /////////////////////////////////////////////////////
  static const DWORD  dwCOLOR           = 0xFF394b5d;
  static const DWORD  dwCOLOR2          = 0xFF39885d;
  static const DWORD  dwHUDCOLOR        = 0xFF03ffcf;//0xCC03feef;
  static const DWORD  dwWARNINGCOLOR    = 0x44a2d2ff;
  static const DWORD  dwGUICOLOR        = 0xFF506171;
  static const DWORD  dwACTIVEGUICOLOR  = 0xFFa2d2ff;
  static const DWORD  dwBACKGROUNDCOLOR = 0x55a2d2ff;
  static const DWORD  dwACTIVEWINDOWCOLOR = 0xCCd8e000;

#elif defined(LIGHT_ORANGE_ALPHA)
  /////////////////////////////////////////////////////
  // Light Orange | Alpha
  /////////////////////////////////////////////////////
  static const DWORD  dwCOLOR           = 0x77fb9c06;
  static const DWORD  dwHUDCOLOR        = 0xCC03feef;
  static const DWORD  dwWARNINGCOLOR    = 0x44fb9c06;
  static const DWORD  dwGUICOLOR        = 0x77fb9c06;
  static const DWORD  dwACTIVEGUICOLOR  = 0xCCfb9c06;
  static const DWORD  dwBACKGROUNDCOLOR = 0x77fb9c06;
  static const DWORD  dwACTIVEWINDOWCOLOR = 0xCCd8e000;

#else
  /////////////////////////////////////////////////////
  // Light Green | Alpha
  /////////////////////////////////////////////////////
  //static const DWORD  dwCOLOR           = 0x7714cc06;
  //static const DWORD  dwHUDCOLOR        = 0xCC03feef;
  //static const DWORD  dwWARNINGCOLOR    = 0x4414cc06;
  //static const DWORD  dwGUICOLOR        = 0x7714cc06;
  //static const DWORD  dwACTIVEGUICOLOR  = 0xCC14cc06;
  //static const DWORD  dwBACKGROUNDCOLOR = 0x7714cc06;
  //static const DWORD  dwACTIVEWINDOWCOLOR = 0xCCd8e000;

  static const DWORD  dwCOLOR           = 0x99acfbdc;
  static const DWORD  dwHUDCOLOR        = 0xCC03feef;
  static const DWORD  dwWARNINGCOLOR    = 0x44acfbdc;
  static const DWORD  dwGUICOLOR        = 0x99acfbdc;
  static const DWORD  dwACTIVEGUICOLOR  = 0xCCacfbdc;
  static const DWORD  dwBACKGROUNDCOLOR = 0x99acfbdc;
  static const DWORD  dwACTIVEWINDOWCOLOR = 0xCCd8e000;

#endif

};

#endif	/* __SVSP_COLOR_H__ */


