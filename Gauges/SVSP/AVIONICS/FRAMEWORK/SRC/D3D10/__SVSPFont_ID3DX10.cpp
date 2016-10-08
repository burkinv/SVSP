#include <Windows.h>
#include "__AAVDebug.h"
#include "__AAVFont_D3D10.h"
#include "__AAVGraphics_D3D10.h"

__AAVFont_D3D10::__AAVFont_D3D10(__AAVGraphics_D3D10* pkGraphics, 
                                  UINT                 uiHeight,
                                  UINT                 uiWidth, 
                                  UINT                 uiWeight, 
                                  UINT                 uiMipLevels, 
                                  BOOL                 bItalic,  
                                  UINT                 uiCharSet, 
                                  UINT                 uiOutputPrecision, 
                                  UINT                 uiQuality,
                                  UINT                 uiPitchAndFamily,
                                  LPCTSTR              pFaceName) : __AAVFont(pkGraphics, uiHeight, uiWidth, uiWeight, uiMipLevels, bItalic, uiCharSet, uiOutputPrecision, uiQuality, uiPitchAndFamily, pFaceName)
{
  m_pkFont       = NULL;
  m_pkFontSprite = NULL;

  if(pkGraphics != NULL)
  {
    HRESULT hResult = S_FALSE;
    hResult = D3DX10CreateFont(pkGraphics->GetDevice(), uiHeight, uiWidth, uiWeight, uiMipLevels, bItalic, uiCharSet, uiOutputPrecision, uiQuality, uiPitchAndFamily, pFaceName, &m_pkFont);
    AAV_ASSERT(SUCCEEDED(hResult == S_OK));
    hResult = D3DX10CreateSprite(pkGraphics->GetDevice(), 512, &m_pkFontSprite);
    AAV_ASSERT(SUCCEEDED(hResult == S_OK));
  }
}

__AAVFont_D3D10::~__AAVFont_D3D10() 
{
  if(m_pkFontSprite != NULL)
  {
    m_pkFontSprite->Release();
    m_pkFontSprite = NULL;
  }

  if(m_pkFont != NULL)
  {
    m_pkFont->Release();
    m_pkFont = NULL;
  }
}

VOID __AAVFont_D3D10::Begin()
{
  AAV_ASSERT_RETURN_VOID(m_pkFontSprite != 0);
  m_pkFontSprite->Begin(D3DX10_SPRITE_SORT_TEXTURE);
}

VOID __AAVFont_D3D10::End()
{
  AAV_ASSERT_RETURN_VOID(m_pkFontSprite != 0);
  m_pkFontSprite->End();
}

VOID __AAVFont_D3D10::Release()
{
  m_pkFontSprite->Release();
  m_pkFontSprite = NULL;
  m_pkFont->Release();
  m_pkFont = 0;
}

VOID __AAVFont_D3D10::RenderText( const TCHAR* strText, struct D3DXVECTOR3 *v, DWORD dwFlags, FLOAT fScaling)
{
  AAV_ASSERT_RETURN_VOID(m_pkFont != 0);
  AAV_ASSERT_RETURN_VOID(m_pkFontSprite != 0);
  RECT rc;
  rc.left   = v->x;
  rc.right  = v->x + 100;
  rc.top    = v->y;
  rc.bottom = v->y + 100;
  D3DXCOLOR color = D3DXCOLOR(0xffffffff);
  m_pkFont->DrawText(m_pkFontSprite, strText, strlen(strText), &rc, dwFlags, color);
}
