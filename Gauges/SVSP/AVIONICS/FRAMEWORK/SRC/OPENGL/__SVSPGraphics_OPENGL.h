#ifndef __AAV_GRAPHICS_OPENGL_H__
#define __AAV_GRAPHICS_OPENGL_H__

#include <__AAVOPENGL.h>
#include "__AAVGraphics.h"

class __AAVVertex
{
public:
  FLOAT       x, y, z;
  DWORD       color;
  FLOAT       tu, tv;

  __AAVVertex()
  {
  }

  inline __AAVVertex(FLOAT x, FLOAT y, FLOAT z, FLOAT tu = 0, FLOAT tv = 0, DWORD color = __AAVColor::dwCOLOR)
  {
    this->x     = x;
    this->y     = y;
    this->z     = z;
    this->color = color;
    this->tu    = tu;
    this->tv    = tv;
  }

  inline __AAVVertex(D3DXVECTOR3 &rv, FLOAT tu = 0, FLOAT tv = 0, DWORD color = __AAVColor::dwCOLOR)
  {
    this->x     = rv.x;
    this->y     = rv.y;
    this->z     = rv.z;
    this->color = color;
    this->tu    = tu;
    this->tv    = tv;
  }
};
typedef __AAVVertex *LPCUSTOMVERTEX;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//class __AAVVertex
//{
//public:
//    FLOAT       x, y, z;
//    DWORD       color;
//    FLOAT       tu, tv;
//
//  __AAVVertex()
//  {
//  }
//
//  __AAVVertex(D3DXVECTOR3 &rv, DWORD color, FLOAT tu, FLOAT tv)
//  {
//    this->x     = rv.x;
//    this->y     = rv.y;
//    this->z     = rv.z;
//    this->color = color;
//    this->tu    = tu;
//    this->tv    = tv;
//  }
//
//  __AAVVertex(FLOAT x, FLOAT y, FLOAT z, DWORD color, FLOAT tu, FLOAT tv)
//  {
//    this->x     = x;
//    this->y     = y;
//    this->z     = z;
//    this->color = color;
//    this->tu    = tu;
//    this->tv    = tv;
//  }
//
//};
//#define D3DFVF_CUSTOMTEXVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class __AAVGraphics_OPENGL : public __AAVGraphics
{
public:
  enum STATE_BLOCK_LIST
  {
    STATE_BLOCK_TRANSPARENT_TEXTURE,
    STATE_BLOCK_NULL_TEXTURE,
    STATE_BLOCK_NUM_STATE_BLOCKS
  };


                               __AAVGraphics_OPENGL(class __AAVAvionics *pkAvionics); 
  virtual                     ~__AAVGraphics_OPENGL();
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
          HRESULT              SetHUDStencilSurface();
          HRESULT              DisableHUDStencilSurface();

  LPDIRECT3DSTATEBLOCK9        m_pkStateBlock[STATE_BLOCK_NUM_STATE_BLOCKS]; 

private:
  static ULONG     FASTCALL    Release_Callback              (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice)                                                                                                                                   ;
  static HRESULT   FASTCALL    Reset_Callback                (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)                                                                                   ;
  static HRESULT   FASTCALL    Present_Callback              (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)                               ;
  static HRESULT   FASTCALL    EndScene_Callback             (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)                                                                                   ;
  static HRESULT   FASTCALL    CreateTexture_Callback        (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) ;
  static HRESULT   FASTCALL    SetTexture_Callback           (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)                                                                                      ;
  static HRESULT   FASTCALL    DrawIndexedPrimitive_Callback (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)           ;

  HRESULT                      m_hResult;
  IDirect3D9                  *m_pkOPENGL;
  BOOL                         m_bReleased;
  BOOL                         m_bRender;
  BOOL                         m_bUpdateHUDStencilBuffer;
  D3DPRESENT_PARAMETERS        m_kPresentationParameters;
  IDirect3DTexture9           *m_pkHDDTexture;
  IDirect3DTexture9           *m_pkHUDTexture;
  class HUD                   *m_pkHUD;
  class HDD                   *m_pkHDD;
  BOOL                         m_bStencilVisible;
  LPD3DXSURFACE                m_pkHUDDepthStencilSurface;
  LPD3DXSURFACE                m_pkDefaultDepthStencilSurface;
  DWORD                        m_dwDepthStencilTestEnable;
  DWORD                        m_dwZEnable;
  DWORD                        m_dwZWriteEnable;
  DWORD                        m_dwZFunction;

};

#endif //__AAV_GRAPHICS_OPENGL_H__
