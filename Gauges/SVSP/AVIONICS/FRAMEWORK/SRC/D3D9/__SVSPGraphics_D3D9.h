#ifndef __SVSP_GRAPHICS_D3D9_H__
#define __SVSP_GRAPHICS_D3D9_H__

#include <__SVSPD3D9.h>
#include "__SVSPGraphics.h"


class __SVSPGraphics_D3D9 : public __SVSPGraphics
{
public:
  enum STATE_BLOCK_LIST
  {
    STATE_BLOCK_TRANSPARENT_TEXTURE,
    STATE_BLOCK_NULL_TEXTURE,
    STATE_BLOCK_NUM_STATE_BLOCKS
  };

                               __SVSPGraphics_D3D9(); 
  virtual                     ~__SVSPGraphics_D3D9();
          VOID     FASTCALL    Release();
          VOID     FASTCALL    Reset();
  virtual VOID                 SetShaders();
  virtual VOID                 ResetShaders();
  virtual VOID                 SetScissorRect(const RECT* pRect);
  virtual VOID                 SetGlobalMaterial(D3DMATERIAL9 *pkMaterial);
  virtual VOID                 GetViewMatrix(D3DXMATRIX *pkViewMatrix);
  virtual VOID                 SetViewMatrix(D3DXMATRIX *pkViewMatrix);
  virtual VOID                 SetViewMatrix(D3DXVECTOR3 *pkEyeVector, D3DXVECTOR3 *pkLookAtVector, D3DXVECTOR3 *pkUpVector);
  virtual VOID                 SetTransform(D3DXMATRIX *pkTransformMatrix);

  LPDIRECT3DSTATEBLOCK9        m_pkStateBlock[STATE_BLOCK_NUM_STATE_BLOCKS]; 

private:
  static ULONG     FASTCALL    Release_Callback              (__SVSPGraphics_D3D9 *pkThis, LPD3DXDEVICE pkDevice)                                                                                                                                   ;
  static HRESULT   FASTCALL    Reset_Callback                (__SVSPGraphics_D3D9 *pkThis, LPD3DXDEVICE pkDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)                                                                                   ;
  static HRESULT   FASTCALL    Present_Callback              (__SVSPGraphics_D3D9 *pkThis, LPD3DXDEVICE pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)                               ;
  static HRESULT   FASTCALL    EndScene_Callback             (__SVSPGraphics_D3D9 *pkThis, LPD3DXDEVICE pkDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)                                                                                   ;
  static HRESULT   FASTCALL    CreateTexture_Callback        (__SVSPGraphics_D3D9 *pkThis, LPD3DXDEVICE pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) ;
  static HRESULT   FASTCALL    SetTexture_Callback           (__SVSPGraphics_D3D9 *pkThis, LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)                                                                                      ;
  static HRESULT   FASTCALL    DrawIndexedPrimitive_Callback (__SVSPGraphics_D3D9 *pkThis, LPD3DXDEVICE pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)           ;

  HRESULT                      m_hResult;
  IDirect3D9                  *m_pkD3D9;
  BOOL                         m_bReleased;
  BOOL                         m_bRender;
  //D3DPRESENT_PARAMETERS        m_kPresentationParameters;
  LPD3DXSURFACE                m_pkDefaultDepthStencilSurface;
  DWORD                        m_dwDepthStencilTestEnable;
  DWORD                        m_dwZEnable;
  DWORD                        m_dwZWriteEnable;
  DWORD                        m_dwZFunction;

};

#endif //__SVSP_GRAPHICS_D3D9_H__
