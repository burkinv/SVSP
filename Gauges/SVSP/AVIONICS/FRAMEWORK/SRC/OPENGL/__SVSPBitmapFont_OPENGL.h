#ifndef __AAV_FONT_OPENGL_H__
#define __AAV_FONT_OPENGL_H__

#include "__AAVFont.h"
#include "__AAVGraphics_OPENGL.h"
#include "__AAVGeometryObject_OPENGL.h"

class __AAVBitmapFont_OPENGL : public __AAVFont
{
public:
                                              __AAVBitmapFont_OPENGL(__AAVGraphics_OPENGL* pkGraphics, const LPSTR pcFontName, DWORD dwHeight, DWORD dwFlags=0L);
  virtual                                    ~__AAVBitmapFont_OPENGL();
  VOID                                        SetTransform(D3DXMATRIX *pkTransformMatrix);
  VOID                                        Begin();
  VOID                                        End();
  VOID                                        Release();
  HRESULT                                     Reset();
  VOID                                        RenderText( const TCHAR* strText, D3DXVECTOR3 *v, DWORD dwFlags=0L, FLOAT fScaling = 130.0f);
  VOID                                        RenderText( const TCHAR* strText, DWORD dwFlags=0L, FLOAT fScaling = 130.0f);

protected:
  LPSTR                                       m_pcFontFilename;
  LPSTR                                       m_pkCoordFilename;
  HRESULT                                     GetTextExtent( const TCHAR* strText, DWORD dwFlags, SIZE* pSize );

  static const UINT                           NUM_TEXT_RECT_VERTICES = 50*6;

private:
  HRESULT                                     m_hResult;
  LPDIRECT3DTEXTURE9                          m_pkFontBitmapTexture;
  DWORD                                       m_dwFontBitmapTextureWidth;
  DWORD                                       m_dwFontBitmapTextureHeight;
  LPDIRECT3DDEVICE9                           m_pkDevice;
  FLOAT                                       m_fTexCoords[128-32][4];
  LPDIRECT3DVERTEXBUFFER9                     m_pkVertexBuffer;
  LPDIRECT3DSTATEBLOCK9                       m_pStateBlockSaved; 
  LPDIRECT3DSTATEBLOCK9                       m_pStateBlockDrawText; 
};


#endif //__AAV_FONT_OPENGL_H__
