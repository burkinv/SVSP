#ifndef __SVSP_FONT_H__
#define __SVSP_FONT_H__

#include <tchar.h>
#include <windows.h>
#include <__SVSPTypes.h>
#include <__SVSPColor.h>

class __SVSPFont
{
public:
  static const FLOAT DEFAULT_FONT_SCALING;
  enum
  {
    BOLD    = 0x0001,
    ITALIC  = 0x0002,
    ZENABLE = 0x0004,
  } CREATE_FLAGS;
  enum
  {
    CENTERED_X   = 0x0001,
    CENTERED_Y   = 0x0002,
    TWOSIDED     = 0x0004,
    FILTERED     = 0x0008,
    ALIGN_LEFT   = 0x0010,
    ALIGN_RIGHT  = 0x0020,
    VERTICAL     = 0x0040,
  } FLAGS;

                                __SVSPFont(UINT           uiHeight,
                                           UINT           uiWidth, 
                                           UINT           uiWeight, 
                                           UINT           uiMipLevels, 
                                           BOOL           bItalic,  
                                           UINT           uiCharSet, 
                                           UINT           uiOutputPrecision, 
                                           UINT           uiQuality,
                                           UINT           uiPitchAndFamily,
                                           LPCTSTR        pFaceName);
                                __SVSPFont   (const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L);
  virtual                      ~__SVSPFont   ()                                                                                                                                                        = 0;
  virtual VOID                  Release      ()                                                                                                                                                           = 0;
  virtual HRESULT               Reset        (LPD3DXDEVICE pkDevice)                                                                                                                                        = 0;
  virtual VOID                  RenderTextExt(LPD3DXDEVICE pkDevice, const TCHAR* strText, struct D3DXVECTOR3 *kVector, DWORD dwFlags=0L, FLOAT fScaling = DEFAULT_FONT_SCALING, DWORD dwColor = __SVSPColor::dwHUDCOLOR) = 0;
  virtual VOID                  RenderTextExt(LPD3DXDEVICE pkDevice, const TCHAR* strText,                              DWORD dwFlags=0L, FLOAT fScaling = DEFAULT_FONT_SCALING, DWORD dwColor = __SVSPColor::dwHUDCOLOR) = 0;
  virtual VOID                  SetTransform (LPD3DXDEVICE pkDevice, struct D3DXMATRIX *pkTransformMatrix)                                                                                                                  = 0;

protected:
  TCHAR                         m_sFontName[80];
  DWORD                         m_dwFontHeight;
  DWORD                         m_dwFontFlags;
  DWORD                         m_dwSpacing;
  FLOAT                         m_fTextScale;
  FLOAT                         m_fTexCoords[128-32][4];
};

typedef __SVSPFont* LPAAVFONT;

#endif	/* __SVSP_FONT_H__ */


