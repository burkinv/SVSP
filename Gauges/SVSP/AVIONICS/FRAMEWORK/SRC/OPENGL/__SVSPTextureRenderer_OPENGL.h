#ifndef __AAV_TEXTURE_RENDERER_H__
#define __AAV_TEXTURE_RENDERER_H__

#include <__AAVTypes.h>

class __AAVTextureRenderer_OPENGL
{
public:
  static const UINT  MIN_SWAP_BUFFER_OFFSET = 3;
  static const UINT  MAX_NUM_SWAP_BUFFERS   = 6;


                        __AAVTextureRenderer_OPENGL(UINT uiWidth, UINT uiHeight);
  virtual              ~__AAVTextureRenderer_OPENGL();
  virtual VOID          Clear(DWORD dwColor);
  virtual VOID          Release();
  virtual VOID          Reset();
  virtual VOID          Begin();
  virtual VOID          End();
  virtual VOID          GetBackbufferData(struct IMAGE* pImage);
  virtual LPD3DXTEXTURE GetDisplayTexture();
  inline  LPD3DXDEVICE  GetDevice() const { return m_pkDevice; }
  inline  VOID          SetDevice(LPD3DXDEVICE pkDevice) { m_pkDevice = pkDevice; }

protected:
  HRESULT                             m_hResult;
  bool                                m_bValid;
  LPD3DXDEVICE                        m_pkDevice;
  UINT                                m_uiWidth;
  UINT                                m_uiHeight;
  D3DFORMAT                           m_eFormat;
  D3DMULTISAMPLE_TYPE                 m_eMultisampleType;
  DWORD                               m_dwQualityLevels;
  IDirect3DSurface9                  *m_apkGPURenderTextureMS[__AAVTextureRenderer_OPENGL::MAX_NUM_SWAP_BUFFERS];
  //IDirect3DSurface9                  *m_apkGPURenderTexture[__AAVTextureRenderer_OPENGL::MAX_NUM_SWAP_BUFFERS];
  IDirect3DTexture9                  *m_apkGPURenderTextureDS[__AAVTextureRenderer_OPENGL::MAX_NUM_SWAP_BUFFERS];
  //IDirect3DSurface9                  *m_apkCPURenderTexture[__AAVTextureRenderer_OPENGL::MAX_NUM_SWAP_BUFFERS];
  IDirect3DSurface9                  *m_pkSavedRenderTarget;
  D3DXMATRIX                         *m_pkSavedTransformMatrix;
  DWORD                               m_dwSavedZEnable;
  bool                                m_bMultisamplingEnabled;

  //IDirect3DTexture9                  *m_apkCPURenderTexture[__AAVTextureRenderer_OPENGL::MAX_NUM_SWAP_BUFFERS];
  UINT                                m_uiRenderBufferIndex;
  UINT                                m_uiDisplayBufferIndex;
  D3DXMATRIXA16                       m_kViewMatrix;
  D3DXMATRIXA16                       m_kProjectionMatrix;
  D3DVIEWPORT9                        m_kViewport;
};

#endif	/* __AAV_TEXTURE_RENDERER_H__ */


