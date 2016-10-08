#ifndef __AAV_FONT_D3D10_H__
#define __AAV_FONT_D3D10_H__

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include "__AAVDebug.h"
#include "__AAVFont.h"
#include "__AAVGraphics_D3D10.h"
#include "__AAVGeometryObject_D3D10.h"

class __AAVFont_D3D10 : public __AAVFont
{
public:
                                              __AAVFont_D3D10(__AAVGraphics_D3D10* pkGraphics, const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L);
  virtual                                    ~__AAVFont_D3D10();
  VOID                                        SetTransform(D3DXMATRIX *pkTransformMatrix);
  VOID                                        Begin();
  VOID                                        End();
  VOID                                        Release();
  HRESULT                                     Reset();
  VOID                                        RenderText( const TCHAR* strText, D3DXVECTOR3 *v, DWORD dwFlags=0L, FLOAT fScaling = 130.0f);
  VOID                                        RenderText( const TCHAR* strText, DWORD dwFlags=0L, FLOAT fScaling = 130.0f);

protected:
  HRESULT                                     PaintAlphabet(HDC hDC, BOOL bMeasureOnly);
  HRESULT                                     GetTextExtent( const TCHAR* strText, DWORD dwFlags, SIZE* pSize );

  static const UINT                           NUM_TEXT_RECT_VERTICES = 50*6;

private:
  HRESULT                                     m_hResult;
  DWORD                                       m_dwFontBitmapTextureWidth;
  DWORD                                       m_dwFontBitmapTextureHeight;
  ID3D10Texture2D                            *m_pkFontBitmapTexture;
  FLOAT                                       m_fTexCoords[128-32][4];
  __AAVGeometryObject_D3D10<__AAVVertex> *m_pkTextRect;
};
#endif //__AAV_FONT_D3D10_H__
