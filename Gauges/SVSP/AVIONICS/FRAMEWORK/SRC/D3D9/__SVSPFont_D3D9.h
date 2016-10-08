#ifndef __SVSP_FONT_D3D9_H__
#define __SVSP_FONT_D3D9_H__

#include "__SVSPFont.h"
#include "__SVSPGeometryObject_D3D9.h"

class __SVSPFont_D3D9 : public __SVSPFont
{
public:
                                              __SVSPFont_D3D9(const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L);
  virtual                                    ~__SVSPFont_D3D9();
  VOID                                        Release        ();
  HRESULT                                     Reset          (LPD3DXDEVICE pkDevice);
  VOID                                        RenderTextExt  (LPD3DXDEVICE pkDevice, const TCHAR* strText, D3DXVECTOR3 *v, DWORD dwFlags=0L, FLOAT fScaling = DEFAULT_FONT_SCALING, DWORD dwColor = __SVSPColor::dwACTIVEGUICOLOR);
  VOID                                        RenderTextExt  (LPD3DXDEVICE pkDevice, const TCHAR* strText,                 DWORD dwFlags=0L, FLOAT fScaling = DEFAULT_FONT_SCALING, DWORD dwColor = __SVSPColor::dwACTIVEGUICOLOR);
  VOID                                        SetTransform   (LPD3DXDEVICE pkDevice, D3DXMATRIX *pkTransformMatrix);
  inline LPDIRECT3DTEXTURE9                   GetFontBitmapTexture() const { return m_pkFontBitmapTexture; }


protected:
  HRESULT                                     PaintAlphabet(HDC hDC, BOOL bMeasureOnly);
  HRESULT                                     GetTextExtent( const TCHAR* strText, DWORD dwFlags, SIZE* pSize );

  static const UINT                           NUM_TEXT_RECT_VERTICES = 50*6;

private:
  HRESULT                                     m_hResult;
  HBITMAP                                     m_hBitmap;
  DWORD*                                      m_pBitmapBits;
  DWORD                                       m_dwFontBitmapTextureWidth;
  DWORD                                       m_dwFontBitmapTextureHeight;
  LPDIRECT3DTEXTURE9                          m_pkFontBitmapTexture;
  FLOAT                                       m_fTexCoords[128-32][4];
  LPDIRECT3DVERTEXBUFFER9                     m_pkVertexBuffer;
  LPDIRECT3DSTATEBLOCK9                       m_pStateBlockSaved; 
  LPDIRECT3DSTATEBLOCK9                       m_pStateBlockDrawText; 
};


#endif //__SVSP_FONT_D3D9_H__
