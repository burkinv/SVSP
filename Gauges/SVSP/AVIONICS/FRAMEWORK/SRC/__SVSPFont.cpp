#include <__SVSPFont.h>
  
const FLOAT __SVSPFont::DEFAULT_FONT_SCALING = 0.0139f;

__SVSPFont::__SVSPFont(UINT         uiHeight,
                     UINT         uiWidth, 
                     UINT         uiWeight, 
                     UINT         uiMipLevels, 
                     BOOL         bItalic,  
                     UINT         uiCharSet, 
                     UINT         uiOutputPrecision, 
                     UINT         uiQuality,
                     UINT         uiPitchAndFamily,
                     LPCTSTR      pFaceName){}
__SVSPFont::__SVSPFont(const TCHAR* sFontName, DWORD dwHeight, DWORD dwFlags)
{
  _tcsncpy( m_sFontName, sFontName, sizeof(m_sFontName) / sizeof(TCHAR) );
  m_sFontName[sizeof(m_sFontName) / sizeof(TCHAR) - 1] = _T('\0');
  m_dwFontHeight         = dwHeight;
  m_dwFontFlags          = dwFlags;
  m_dwSpacing            = 0;
  m_fTextScale           = 1;
}
        __SVSPFont::~__SVSPFont  () {}
VOID    __SVSPFont::Release      (){}
HRESULT __SVSPFont::Reset        (LPD3DXDEVICE pkDevice) { return S_OK; }
VOID    __SVSPFont::RenderTextExt(LPD3DXDEVICE pkDevice,  const TCHAR* strText, D3DXVECTOR3 *kVector, DWORD dwFlags, FLOAT fScaling, DWORD dwColor){}
VOID    __SVSPFont::RenderTextExt(LPD3DXDEVICE pkDevice,  const TCHAR* strText,                       DWORD dwFlags, FLOAT fScaling, DWORD dwColor){}




