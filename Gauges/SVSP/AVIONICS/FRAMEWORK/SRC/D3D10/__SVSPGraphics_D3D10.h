#ifndef __AAV_GRAPHICS_D3D10_H__
#define __AAV_GRAPHICS_D3D10_H__

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <GAUGES.h>
#include "__AAVDebug.h"
#include "__AAVGraphics.h"
#include "__AAVFont.h"

class __AAVGraphics_D3D10 : public __AAVGraphics
{
public:
  static const UINT                   MIN_SWAP_BUFFER_OFFSET = 3;
  static const UINT                   MAX_NUM_SWAP_BUFFERS = 6;

                                      __AAVGraphics_D3D10(UINT uiWidth, UINT uiHeight); 
  virtual                            ~__AAVGraphics_D3D10();
  VOID                                Clear(DWORD dwColor);
  VOID                                Begin();
  VOID                                End(PIMAGE  pImage);
  HRESULT                             LoadTextureFromFile(LPCSTR swFilename, class __AAVTexture **ppkTexture);

  HRESULT                             m_hResult;
  IDXGISwapChain*                     m_pSwapChain;
  ID3D10Texture2D                    *m_apkGPURenderTextureMS[__AAVGraphics_D3D10::MAX_NUM_SWAP_BUFFERS];
  ID3D10Texture2D                    *m_apkGPURenderTexture[__AAVGraphics_D3D10::MAX_NUM_SWAP_BUFFERS];
  ID3D10Texture2D                    *m_apkCPURenderTexture[__AAVGraphics_D3D10::MAX_NUM_SWAP_BUFFERS];
  ID3D10RenderTargetView*             m_apkRenderTargetView[__AAVGraphics_D3D10::MAX_NUM_SWAP_BUFFERS];
  ID3D10InputLayout*                  m_pkVertexLayout;
  ID3D10InputLayout*                  m_pkTexVertexLayout;
  ID3D10RasterizerState*              m_pkRasterState;
  ID3D10Effect*                       m_pkEffect;
  ID3D10EffectTechnique*              m_pkTechnique;
  UINT                                m_uiRenderBufferIndex;
  UINT                                m_uiDisplayBufferIndex;

  ID3D10EffectMatrixVariable*         m_pkViewVariable;
  D3DXMATRIXA16                       m_kViewMatrix;
  ID3D10EffectMatrixVariable*         m_pkProjectionVariable;
  D3DXMATRIXA16                       m_kProjectionMatrix;
  ID3D10EffectMatrixVariable*         m_pkTransformVariable;
  D3DXMATRIXA16                       m_kTransformMatrix;
  ID3D10EffectShaderResourceVariable* m_pkDiffuseVariable;

#ifdef USE_DEPTH_STENCIL
  ID3D10Texture2D*        m_pDepthStencilMS;
  ID3D10DepthStencilView* m_pDepthStencilView;
#endif //USE_DEPTH_STENCIL

};

#endif //__AAV_GRAPHICS_D3D10_H__
