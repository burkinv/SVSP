#ifndef __SVSP_GEOMETRY_OBJECT_D3D9_H__
#define __SVSP_GEOMETRY_OBJECT_D3D9_H__

#include "__SVSPGeometryObject.h"
#include <__SVSPTextureObject_D3D9.h>
#include "__SVSPDebug.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

template <class T> class __SVSPGeometryObject_D3D9 : public __SVSPGeometryObject<T>
{
public:
  typedef enum 
  {
    UNDEFINED     = 0,
    POINTLIST     = D3DPT_POINTLIST,
    LINELIST      = D3DPT_LINELIST,
    LINESTRIP     = D3DPT_LINESTRIP,
    TRIANGLELIST  = D3DPT_TRIANGLELIST,
    TRIANGLESTRIP = D3DPT_TRIANGLESTRIP,
  } TOPOLOGY;

  typedef enum 
  {
    STATE_BLOCK_TRANSPARENT_TEXTURE,
    STATE_BLOCK_NULL_TEXTURE,
    STATE_BLOCK_NUM_STATE_BLOCKS
  } STATE_BLOCK;

                               __SVSPGeometryObject_D3D9(DWORD eTopology, UINT uiNumVerts, DWORD dwUsage = __SVSPGeometryObject::USAGE_STATIC);
  virtual                     ~__SVSPGeometryObject_D3D9();
  VOID                         Release              ();
  HRESULT                      Reset                (LPD3DXDEVICE pkDevice);
  HRESULT                      SetTextureFromFile   (LPCSTR swFilename);
  VOID                         Render               (LPD3DXDEVICE pkDevice, UINT uiNumVertices, UINT StartVertex, bool bRenderWithTrueColor = false);
  VOID                         Render               (LPD3DXDEVICE pkDevice, bool bRenderWithTrueColor = false);
  VOID                         RenderPrimitives     (LPD3DXDEVICE pkDevice, UINT StartVertex, UINT uiNumPrimitives, bool bRenderWithTrueColor = false);
  HRESULT                      SetTexture           (LPD3DXDEVICE pkDevice, __SVSPTextureObject* pkTextureObject);
  HRESULT                      CreateVertexBuffer   (LPD3DXDEVICE pkDevice);
  VOID                         SetTransform         (LPD3DXDEVICE pkDevice, D3DXMATRIX *pkTransformMatrix);
  VOID                         SetMaterial          (FLOAT r, FLOAT g, FLOAT b, FLOAT a);

  static VOID                  GlobalRelease        ();
  static VOID                  GlobalReset          (LPD3DXDEVICE pkDevice, DWORD *pParameters = NULL);
  static VOID                  GlobalApplyStateBlock(STATE_BLOCK eStateBlock);
  static VOID                  GlobalSetScissorRect (LPD3DXDEVICE pkDevice, const RECT* pRect);
  static VOID                  GlobalSetShaders     (LPD3DXDEVICE pkDevice);
  static VOID                  GlobalResetShaders   (LPD3DXDEVICE pkDevice);
  static VOID                  GlobalSetMaterial    (LPD3DXDEVICE pkDevice, D3DMATERIAL9 *pkMaterial);
  static VOID                  GlobalGetViewMatrix  (LPD3DXDEVICE pkDevice, D3DXMATRIX *pkViewMatrix);
  static VOID                  GlobalSetViewMatrix  (LPD3DXDEVICE pkDevice, D3DXMATRIX *pkViewMatrix);
  static VOID                  GlobalSetViewMatrix  (LPD3DXDEVICE pkDevice, D3DXVECTOR3 *pkEyeVector, D3DXVECTOR3 *pkLookAtVector, D3DXVECTOR3 *pkUpVector);
  static VOID                  GlobalSetTransform   (LPD3DXDEVICE pkDevice, D3DXMATRIX *pkTransformMatrix);

private:
  UINT                           m_uiStride;
  UINT                           m_uiOffset;
  D3DMATERIAL9                   m_Material;
  UINT                           m_uiNumPrimitives;
  static IDirect3DPixelShader9*  g_pkDefaultPixelShader;
  static IDirect3DVertexShader9* g_pkDefaultVertexShader;
  static DWORD                   g_dwDefaultFVF;
  static LPDIRECT3DSTATEBLOCK9   g_pkStateBlock[__SVSPGeometryObject_D3D9<T>::STATE_BLOCK_NUM_STATE_BLOCKS];
};


template <class T> __SVSPGeometryObject_D3D9<T>::__SVSPGeometryObject_D3D9(DWORD eTopology, UINT uiNumVerts, DWORD dwUsage) :
__SVSPGeometryObject<T>(eTopology, uiNumVerts, dwUsage)
{
  m_pVerts                   = new T[m_uiNumVerts];
  m_uiStride                 = sizeof(T);
  m_uiOffset                 = 0;
  //m_dwFVF                    = (sizeof(T) == sizeof(__SVSPVertex))? D3DFVF_CUSTOMVERTEX : D3DFVF_CUSTOMTEXVERTEX;
  m_dwFVF                    = D3DFVF_CUSTOMVERTEX;

  switch(eTopology)
  {
  case __SVSPGeometryObject_D3D9::LINELIST:
    m_uiNumPrimitives = m_uiNumVerts/2;
    break;
  case __SVSPGeometryObject_D3D9::LINESTRIP:
    m_uiNumPrimitives = m_uiNumVerts - 1;
    break;
  case __SVSPGeometryObject_D3D9::TRIANGLELIST:
    m_uiNumPrimitives = m_uiNumVerts/3;
    break;
  case __SVSPGeometryObject_D3D9::TRIANGLESTRIP:
    m_uiNumPrimitives = m_uiNumVerts - 2;
    break;
  default:
    break;
  }

  switch(dwUsage)
  {
  case USAGE_STATIC:
    m_dwUsage = D3DUSAGE_WRITEONLY;
    break;
  case USAGE_DYNAMIC:
    m_dwUsage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
    break;
  default:
    m_dwUsage = USAGE_UNDEFINED;
    break;
  }
}

template <class T> __SVSPGeometryObject_D3D9<T>::~__SVSPGeometryObject_D3D9()
{
  Release();

  if(m_pVerts != NULL) 
    delete[] m_pVerts;
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::SetTransform(LPD3DXDEVICE pkDevice, D3DXMATRIX *pkTransformMatrix)
{
  AAV_ASSERT_RETURN_VOID(pkTransformMatrix != NULL);
  pkDevice->SetTransform(D3DTS_WORLD, pkTransformMatrix);
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::SetMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
  m_Material.Diffuse.r = r;
  m_Material.Diffuse.g = g;
  m_Material.Diffuse.b = b;
  m_Material.Diffuse.a = a;

  m_Material.Ambient = m_Material.Specular = m_Material.Emissive = m_Material.Diffuse;
}

template <class T> HRESULT __SVSPGeometryObject_D3D9<T>::CreateVertexBuffer(LPD3DXDEVICE pkDevice)
{
  AAV_ASSERT_RETURN(m_uiNumVerts != 0, E_FAIL);
  return pkDevice->CreateVertexBuffer(m_uiNumVerts*sizeof(T), 
                                        m_dwUsage, 
                                        m_dwFVF, 
                                        D3DPOOL_DEFAULT, 
                                        &m_pVertexBuffer, 
                                        NULL);
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::Release()
{
  if(m_pVertexBuffer != NULL) 
  {
    m_pVertexBuffer->Release();
    m_pVertexBuffer = NULL;
  }
}

template <class T> HRESULT __SVSPGeometryObject_D3D9<T>::Reset(LPD3DXDEVICE pkDevice)
{
  if(m_pVertexBuffer == NULL)
  {
    CreateVertexBuffer(pkDevice);
  }

  AAV_ASSERT_RETURN(m_pVertexBuffer != NULL, E_FAIL);

  VOID* pVertices;
  if( SUCCEEDED( m_pVertexBuffer->Lock( 0, m_uiNumVerts*sizeof(T), (void**)&pVertices, 0 ) ) )
  {
    memcpy( pVertices, m_pVerts, m_uiNumVerts*sizeof(T) );
    m_pVertexBuffer->Unlock();
    return S_OK;
  }
  else
    return E_FAIL;

  return S_OK;
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::Render(LPD3DXDEVICE pkDevice, UINT uiNumVertices, UINT StartVertex, bool bRenderWithTrueColor)
{
  UINT uiNumPrimitives = 0;

  switch(m_eTopology)
  {
  case __SVSPGeometryObject_D3D9::LINELIST:
    uiNumPrimitives = uiNumVertices/2;
    break;
  case __SVSPGeometryObject_D3D9::LINESTRIP:
    uiNumPrimitives = uiNumVertices - 1;
    break;
  case __SVSPGeometryObject_D3D9::TRIANGLELIST:
    uiNumPrimitives = uiNumVertices/3;
    break;
  case __SVSPGeometryObject_D3D9::TRIANGLESTRIP:
    uiNumPrimitives = uiNumVertices - 2;
    break;
  default:
    break;
  }

  RenderPrimitives(pkDevice, StartVertex, uiNumPrimitives, bRenderWithTrueColor);
}

template <class T> VOID  __SVSPGeometryObject_D3D9<T>::Render(LPD3DXDEVICE pkDevice, bool bRenderWithTrueColor)
{
  AAV_ASSERT_RETURN_VOID(m_pVertexBuffer != NULL);

  //pkDevice->SetMaterial(bRenderWithTrueColor ? &m_Material : &kDefaultMaterial);
  //pkDevice->SetFVF( m_dwFVF );
  pkDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(T) );
  m_hResult = pkDevice->DrawPrimitive( (D3DPRIMITIVETYPE)m_eTopology, 0, m_uiNumPrimitives);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::RenderPrimitives(LPD3DXDEVICE pkDevice, UINT StartVertex, UINT uiNumPrimitives, bool bRenderWithTrueColor = false)
{
  AAV_ASSERT_RETURN_VOID(m_pVertexBuffer != NULL);

  //pkDevice->SetMaterial(bRenderWithTrueColor ? &m_Material : &kDefaultMaterial);
  //pkDevice->SetFVF( m_dwFVF );
  pkDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(T) );
  m_hResult = pkDevice->DrawPrimitive( (D3DPRIMITIVETYPE)m_eTopology, StartVertex, uiNumPrimitives);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

template <class T> HRESULT __SVSPGeometryObject_D3D9<T>::SetTexture(LPD3DXDEVICE pkDevice, __SVSPTextureObject* pkTextureObject)
{
  if(pkTextureObject != NULL)
  {
    pkDevice->SetTexture(0, pkTextureObject->GetTexture());
    __SVSPGeometryObject_D3D9<T>::g_pkStateBlock[__SVSPGraphics_D3D9::STATE_BLOCK_TRANSPARENT_TEXTURE]->Apply();
  }
  else
  {
    pkDevice->SetTexture(0, NULL);
    __SVSPGeometryObject_D3D9<T>::g_pkStateBlock[__SVSPGraphics_D3D9::STATE_BLOCK_TRANSPARENT_TEXTURE]->Apply();
    //__SVSPGeometryObject_D3D9<T>::g_pkStateBlock[__SVSPGraphics_D3D9::STATE_BLOCK_NULL_TEXTURE]->Apply();
  }
  return S_OK;
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalReset(LPD3DXDEVICE pkDevice, DWORD *pParameters)
{
  AAV_ASSERT_RETURN_VOID(pkDevice != NULL);

  // STATE_BLOCK_TRANSPARENT_TEXTURE
  pkDevice->BeginStateBlock();

  pkDevice->SetPixelShader(NULL);
  pkDevice->SetVertexShader(NULL);

  pkDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
  //pkDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_MATERIAL);
  //pkDevice->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
  //pkDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE,  D3DMCS_MATERIAL);
  //pkDevice->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

  pkDevice->SetRenderState( D3DRS_COLORVERTEX,            TRUE);
  //pkDevice->SetRenderState( D3DRS_SPECULARENABLE,         FALSE);

  pkDevice->SetRenderState( D3DRS_LIGHTING,               FALSE);
  pkDevice->SetRenderState( D3DRS_CLIPPING,               TRUE);
  pkDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,        FALSE);
  pkDevice->SetRenderState( D3DRS_VERTEXBLEND,            D3DVBF_DISABLE );

  pkDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,       TRUE );
  pkDevice->SetRenderState( D3DRS_SRCBLEND,               D3DBLEND_SRCALPHA );
  pkDevice->SetRenderState( D3DRS_DESTBLEND,              D3DBLEND_INVSRCALPHA );
  //pkDevice->SetRenderState( D3DRS_SRCBLEND,               D3DBLEND_SRCCOLOR );
  //pkDevice->SetRenderState( D3DRS_DESTBLEND,              D3DBLEND_INVSRCCOLOR );

  //pkDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );
  pkDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
  pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
  pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
  pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
  for(UINT i = 1; i < 8; ++i)
  {
    pkDevice->SetTextureStageState( i, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    pkDevice->SetTextureStageState( i, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
  }
  //pkDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
  //pkDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

  pkDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
  pkDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
  pkDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

  //pkDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
  //pkDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
  //pkDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
  //pkDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
  //pkDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
  //pkDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
  //pkDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
  //pkDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
  //pkDevice->SetRenderState( D3DRS_VERTEXBLEND,      D3DVBF_DISABLE );
  //pkDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
  //pkDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
  //pkDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED  | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
  //pkDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
  //pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  //pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
  //pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
  //pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  //pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
  //pkDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
  //pkDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
  //pkDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
  //pkDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
  pkDevice->EndStateBlock(&__SVSPGeometryObject_D3D9<T>::g_pkStateBlock[STATE_BLOCK_TRANSPARENT_TEXTURE]);


  // STATE_BLOCK_NULL_TEXTURE
  pkDevice->BeginStateBlock();
  pkDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,       TRUE );
  pkDevice->SetRenderState( D3DRS_SRCBLEND,               D3DBLEND_SRCALPHA );
  pkDevice->SetRenderState( D3DRS_DESTBLEND,              D3DBLEND_INVSRCALPHA );

  pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
  pkDevice->EndStateBlock(&__SVSPGeometryObject_D3D9<T>::g_pkStateBlock[STATE_BLOCK_NULL_TEXTURE]);

  __SVSPDEBUG::OUTPUT("__SVSPGeometryObject_D3D9", "RESET_STATE_BLOCKS");
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalRelease()
{
  for(UINT i = 0; i < STATE_BLOCK_NUM_STATE_BLOCKS; ++i)
  {
    if(g_pkStateBlock[i] != NULL)
    {
      g_pkStateBlock[i]->Release(); 
    }
  }
  memset(&g_pkStateBlock[0], 0, sizeof(g_pkStateBlock));

  __SVSPDEBUG::OUTPUT("__SVSPGeometryObject_D3D9", "RELEASE_STATE_BLOCKS");
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalApplyStateBlock(STATE_BLOCK eStateBlock)
{
  AAV_ASSERT_RETURN_VOID(eStateBlock < STATE_BLOCK_NUM_STATE_BLOCKS);
  __SVSPGeometryObject_D3D9<T>::g_pkStateBlock[eStateBlock]->Apply();
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalSetMaterial(LPD3DXDEVICE pkDevice, D3DMATERIAL9 *pkMaterial)
{
  pkDevice->SetMaterial(pkMaterial);
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalSetShaders(LPD3DXDEVICE pkDevice) 
{
  HRESULT hResult = S_FALSE;
  //pkDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,      TRUE );
  //pkDevice->SetRenderState( D3DRS_SRCBLEND,              D3DBLEND_SRCALPHA );
  //pkDevice->SetRenderState( D3DRS_DESTBLEND,             D3DBLEND_INVSRCALPHA );

  ////pkDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,       FALSE );
  //pkDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_MATERIAL);
  //pkDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
  //pkDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,  D3DMCS_MATERIAL);
  //pkDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
  //pkDevice->SetRenderState(D3DRS_COLORVERTEX,            TRUE);
  //pkDevice->SetRenderState(D3DRS_LIGHTING,               FALSE);

  //pkDevice->SetTextureStageState(0, D3DTSS_COLOROP,      D3DTOP_DISABLE );
  //pkDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,      D3DTOP_DISABLE );

  // Store current shader setup.
  hResult = pkDevice->GetPixelShader( &g_pkDefaultPixelShader );
  AAV_ASSERT(hResult == S_OK);
  hResult = pkDevice->GetVertexShader( &g_pkDefaultVertexShader );
  AAV_ASSERT(hResult == S_OK);
  hResult = pkDevice->GetFVF( &g_dwDefaultFVF);
  AAV_ASSERT(hResult == S_OK);

  //pkDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
  //AAV_ASSERT_RETURN_VOID(m_pkStateBlock[STATE_BLOCK_TRANSPARENT_TEXTURE] != NULL);
  //m_pkStateBlock[STATE_BLOCK_TRANSPARENT_TEXTURE]->Apply();

  pkDevice->SetPixelShader( NULL );
  pkDevice->SetVertexShader(NULL);
  pkDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalResetShaders(LPD3DXDEVICE pkDevice) 
{
  HRESULT hResult = S_FALSE;

  hResult = pkDevice->SetPixelShader( g_pkDefaultPixelShader );
  AAV_ASSERT(hResult == S_OK);
  if(g_pkDefaultPixelShader != 0)
  {
    g_pkDefaultPixelShader->Release();
  }

  hResult = pkDevice->SetVertexShader( g_pkDefaultVertexShader );
  AAV_ASSERT(hResult == S_OK);
  if(g_pkDefaultVertexShader != 0)
  {
    g_pkDefaultVertexShader->Release();
  }

  hResult = pkDevice->SetFVF( g_dwDefaultFVF );
  AAV_ASSERT(hResult == S_OK);

  pkDevice->SetTexture( 0, NULL);
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalSetScissorRect(LPD3DXDEVICE pkDevice, const RECT* pRect) 
{
  if(pRect != NULL)
  {
    HRESULT hResult = pkDevice->SetScissorRect(pRect);
    AAV_ASSERT_RETURN_VOID(hResult == S_OK);
    pkDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE);
  }
  else
  {
    pkDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE);
  }
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalGetViewMatrix(LPD3DXDEVICE pkDevice, D3DXMATRIX *pkViewMatrix)
{
  pkDevice->GetTransform(D3DTS_VIEW, pkViewMatrix);
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalSetViewMatrix(LPD3DXDEVICE pkDevice, D3DXMATRIX *pkViewMatrix)
{
  pkDevice->SetTransform(pkDevice, D3DTS_VIEW, pkViewMatrix);
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalSetViewMatrix(LPD3DXDEVICE pkDevice, D3DXVECTOR3 *pkEyeVector, D3DXVECTOR3 *pkLookAtVector, D3DXVECTOR3 *pkUpVector)
{
  D3DXMATRIXA16 kViewMatrix;
  D3DXMatrixLookAtLH( &kViewMatrix, pkEyeVector, pkLookAtVector, pkUpVector );
  pkDevice->SetTransform(pkDevice, D3DTS_VIEW, &kViewMatrix);
}

template <class T> VOID __SVSPGeometryObject_D3D9<T>::GlobalSetTransform(LPD3DXDEVICE pkDevice, D3DXMATRIX *pkTransformMatrix) 
{
  AAV_ASSERT_RETURN_VOID(pkTransformMatrix != NULL);
  pkDevice->SetTransform(D3DTS_WORLD, pkTransformMatrix);
}

#endif //__SVSP_GEOMETRY_OBJECT_D3D9_H__
