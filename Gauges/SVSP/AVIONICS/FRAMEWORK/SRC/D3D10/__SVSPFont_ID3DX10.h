#ifndef __AAV_FONT_ID3DX10_H__
#define __AAV_FONT_ID3DX10_H__

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include "__AAVDebug.h"
#include "__AAVFont.h"
#include "__AAVGraphics_D3D10.h"

class __AAVFont_D3D10 : public __AAVFont
{
public:
             __AAVFont_D3D10(__AAVGraphics_D3D10* pkGraphics, 
                              UINT             uiHeight,
                              UINT             uiWidth, 
                              UINT             uiWeight, 
                              UINT             uiMipLevels, 
                              BOOL             bItalic,  
                              UINT             uiCharSet, 
                              UINT             uiOutputPrecision, 
                              UINT             uiQuality,
                              UINT             uiPitchAndFamily,
                              LPCTSTR          pFaceName);

  virtual   ~__AAVFont_D3D10();

        VOID Begin();

        VOID End();

        VOID Release();

        VOID RenderText( const TCHAR* strText, D3DXVECTOR3 *v, DWORD dwFlags=0L, FLOAT fScaling = 130.0f);

private:
  ID3DX10Font   *m_pkFont;
  ID3DX10Sprite *m_pkFontSprite;

};
#endif //__AAV_FONT_ID3DX10_H__
