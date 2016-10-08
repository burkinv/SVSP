#ifndef __SVSP_TEXTURE_RENDERER_H__
#define __SVSP_TEXTURE_RENDERER_H__

#include <__SVSPTypes.h>

class __SVSPTextureRenderer_D3D9
{
public:
  static const UINT  MIN_SWAP_BUFFER_OFFSET = 3;
  static const UINT  MAX_NUM_SWAP_BUFFERS   = 6;


                        __SVSPTextureRenderer_D3D9(UINT uiWidth, UINT uiHeight);
  virtual              ~__SVSPTextureRenderer_D3D9();
  virtual VOID          Release();
  virtual HRESULT       Reset(LPD3DXDEVICE pkDevice);
  virtual VOID          GetBackbufferData(struct IMAGE* pImage);
  virtual LPD3DXTEXTURE GetDisplayTexture();
  virtual VOID          Clear(LPD3DXDEVICE pkDevice, DWORD dwColor);
  virtual VOID          Begin(LPD3DXDEVICE pkDevice);
  virtual VOID          End(LPD3DXDEVICE pkDevice);

protected:
  HRESULT                             m_hResult;
  bool                                m_bValid;
  UINT                                m_uiWidth;
  UINT                                m_uiHeight;
  D3DFORMAT                           m_eFormat;
  D3DMULTISAMPLE_TYPE                 m_eMultisampleType;
  DWORD                               m_dwQualityLevels;
  IDirect3DSurface9                  *m_apkGPURenderTextureMS[__SVSPTextureRenderer_D3D9::MAX_NUM_SWAP_BUFFERS];
  //IDirect3DSurface9                  *m_apkGPURenderTexture[__SVSPTextureRenderer_D3D9::MAX_NUM_SWAP_BUFFERS];
  IDirect3DTexture9                  *m_apkGPURenderTextureDS[__SVSPTextureRenderer_D3D9::MAX_NUM_SWAP_BUFFERS];
  //IDirect3DSurface9                  *m_apkCPURenderTexture[__SVSPTextureRenderer_D3D9::MAX_NUM_SWAP_BUFFERS];
  IDirect3DSurface9                  *m_pkSavedRenderTarget;
  D3DXMATRIX                         *m_pkSavedTransformMatrix;
  DWORD                               m_dwSavedZEnable;
  bool                                m_bMultisamplingEnabled;

  //IDirect3DTexture9                  *m_apkCPURenderTexture[__SVSPTextureRenderer_D3D9::MAX_NUM_SWAP_BUFFERS];
  UINT                                m_uiRenderBufferIndex;
  UINT                                m_uiDisplayBufferIndex;
  D3DXMATRIXA16                       m_kViewMatrix;
  D3DXMATRIXA16                       m_kProjectionMatrix;
  D3DVIEWPORT9                        m_kViewport;
};

#endif	/* __SVSP_TEXTURE_RENDERER_H__ */


