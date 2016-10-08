#ifndef __AAV_FONT_OPENGL_H__
#define __AAV_FONT_OPENGL_H__

#include "__AAVFont.h"
#include "__AAVGraphics_OPENGL.h"
#include "__AAVGeometryObject_OPENGL.h"

class __AAVFont_OPENGL : public __AAVFont
{
public:
                                              __AAVFont_OPENGL(__AAVGraphics_OPENGL* pkGraphics, const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L);
  virtual                                    ~__AAVFont_OPENGL();
  VOID                                        SetTransform(D3DXMATRIX *pkTransformMatrix);
  VOID                                        Begin();
  VOID                                        End();
  VOID                                        Release();
  HRESULT                                     Reset();
  VOID                                        RenderTextExt( const TCHAR* strText, D3DXVECTOR3 *v, DWORD dwFlags=0L, FLOAT fScaling = DEFAULT_FONT_SCALING, DWORD dwColor = __AAVColor::dwACTIVEGUICOLOR);
  VOID                                        RenderTextExt( const TCHAR* strText,                 DWORD dwFlags=0L, FLOAT fScaling = DEFAULT_FONT_SCALING, DWORD dwColor = __AAVColor::dwACTIVEGUICOLOR);
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
  __AAVGraphics_OPENGL*                         m_pkGraphics;
  LPDIRECT3DDEVICE9                           m_pkDevice;
  LPDIRECT3DTEXTURE9                          m_pkFontBitmapTexture;
  FLOAT                                       m_fTexCoords[128-32][4];
  //__AAVGeometryObject_OPENGL<__AAVVertex>  *m_pkTextRect;
  //__AAVGeometryObject_OPENGL<__AAVVertex>     *m_pkTextRect;
  LPDIRECT3DVERTEXBUFFER9                     m_pkVertexBuffer;
  LPDIRECT3DSTATEBLOCK9                       m_pStateBlockSaved; 
  LPDIRECT3DSTATEBLOCK9                       m_pStateBlockDrawText; 
};


#endif //__AAV_FONT_OPENGL_H__
