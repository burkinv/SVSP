#ifndef __AAV_GEOMETRY_OBJECT_OPENGL_H__
#define __AAV_GEOMETRY_OBJECT_OPENGL_H__

#include "__AAVGraphics_OPENGL.h"
#include "__AAVGeometryObject.h"
#include <__AAVTextureObject_OPENGL.h>
#include "__AAVDebug.h"

template <class T> class __AAVGeometryObject_OPENGL : public __AAVGeometryObject<T>
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

                               __AAVGeometryObject_OPENGL(__AAVGraphics_OPENGL *pkGraphics, DWORD eTopology, UINT uiNumVerts, DWORD dwUsage = __AAVGeometryObject::USAGE_STATIC);
  virtual                     ~__AAVGeometryObject_OPENGL();
  VOID                         SetTransform(D3DXMATRIX *pkTransformMatrix);
  VOID                         SetMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
  HRESULT                      CreateVertexBuffer();
  VOID                         Release();
  HRESULT                      Reset();
  HRESULT                      SetTextureFromFile(LPCSTR swFilename);
  VOID                         Render(UINT uiNumVertices, UINT StartVertex, bool bRenderWithTrueColor = false);
  VOID                         Render(bool bRenderWithTrueColor = false);
  VOID                         RenderPrimitives(UINT StartVertex, UINT uiNumPrimitives, bool bRenderWithTrueColor = false);
  HRESULT                      SetTexture(__AAVTextureObject* pkTextureObject);


private:
  HRESULT                      Update();

  __AAVGraphics_OPENGL*          m_pkGraphics;
  LPDIRECT3DDEVICE9            m_pkDevice;
  UINT                         m_uiStride;
  UINT                         m_uiOffset;
  D3DMATERIAL9                 m_Material;
  UINT                         m_uiNumPrimitives;

};


template <class T> __AAVGeometryObject_OPENGL<T>::__AAVGeometryObject_OPENGL(__AAVGraphics_OPENGL *pkGraphics, DWORD eTopology, UINT uiNumVerts, DWORD dwUsage) :
__AAVGeometryObject<T>(pkGraphics, eTopology, uiNumVerts, dwUsage)
{
  m_pkGraphics               = pkGraphics;
  m_pkDevice                 = NULL;
  m_pVerts                   = new T[m_uiNumVerts];
  m_uiStride                 = sizeof(T);
  m_uiOffset                 = 0;
  //m_dwFVF                    = (sizeof(T) == sizeof(__AAVVertex))? D3DFVF_CUSTOMVERTEX : D3DFVF_CUSTOMTEXVERTEX;
  m_dwFVF                    = D3DFVF_CUSTOMVERTEX;

  switch(eTopology)
  {
  case __AAVGeometryObject_OPENGL::LINELIST:
    m_uiNumPrimitives = m_uiNumVerts/2;
    break;
  case __AAVGeometryObject_OPENGL::LINESTRIP:
    m_uiNumPrimitives = m_uiNumVerts - 1;
    break;
  case __AAVGeometryObject_OPENGL::TRIANGLELIST:
    m_uiNumPrimitives = m_uiNumVerts/3;
    break;
  case __AAVGeometryObject_OPENGL::TRIANGLESTRIP:
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

template <class T> __AAVGeometryObject_OPENGL<T>::~__AAVGeometryObject_OPENGL()
{
  Release();

  if(m_pVerts != NULL) 
    delete[] m_pVerts;
}

template <class T> VOID __AAVGeometryObject_OPENGL<T>::SetTransform(D3DXMATRIX *pkTransformMatrix)
{
  AAV_ASSERT_RETURN_VOID(pkTransformMatrix != NULL);
  m_pkDevice->SetTransform(D3DTS_WORLD, pkTransformMatrix);
}

template <class T> VOID __AAVGeometryObject_OPENGL<T>::SetMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
  m_Material.Diffuse.r = r;
  m_Material.Diffuse.g = g;
  m_Material.Diffuse.b = b;
  m_Material.Diffuse.a = a;

  m_Material.Ambient = m_Material.Specular = m_Material.Emissive = m_Material.Diffuse;
}

template <class T> HRESULT __AAVGeometryObject_OPENGL<T>::CreateVertexBuffer()
{
  AAV_ASSERT_RETURN(m_uiNumVerts != 0, E_FAIL);
  return m_pkDevice->CreateVertexBuffer(m_uiNumVerts*sizeof(T), 
                                        m_dwUsage, 
                                        m_dwFVF, 
                                        D3DPOOL_DEFAULT, 
                                        &m_pVertexBuffer, 
                                        NULL);
}

template <class T> VOID __AAVGeometryObject_OPENGL<T>::Release()
{
  if(m_pVertexBuffer != NULL) 
  {
    m_pVertexBuffer->Release();
    m_pVertexBuffer = NULL;
  }
}

template <class T> HRESULT __AAVGeometryObject_OPENGL<T>::Reset()
{
  m_pkDevice = pkDevice;

  if(m_pVertexBuffer == NULL)
  {
    m_hResult = CreateVertexBuffer();
    AAV_ASSERT_RETURN((SUCCEEDED(m_hResult) && (m_pVertexBuffer != NULL)), E_FAIL);
  }

  m_hResult = Update();
  AAV_ASSERT_RETURN(SUCCEEDED(m_hResult), E_FAIL);

  return S_OK;
}

template <class T> HRESULT __AAVGeometryObject_OPENGL<T>::Update()
{
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



template <class T> VOID __AAVGeometryObject_OPENGL<T>::Render(UINT uiNumVertices, UINT StartVertex, bool bRenderWithTrueColor)
{
  UINT uiNumPrimitives = 0;

  switch(m_eTopology)
  {
  case __AAVGeometryObject_OPENGL::LINELIST:
    uiNumPrimitives = uiNumVertices/2;
    break;
  case __AAVGeometryObject_OPENGL::LINESTRIP:
    uiNumPrimitives = uiNumVertices - 1;
    break;
  case __AAVGeometryObject_OPENGL::TRIANGLELIST:
    uiNumPrimitives = uiNumVertices/3;
    break;
  case __AAVGeometryObject_OPENGL::TRIANGLESTRIP:
    uiNumPrimitives = uiNumVertices - 2;
    break;
  default:
    break;
  }

  RenderPrimitives(StartVertex, uiNumPrimitives, bRenderWithTrueColor);
}

template <class T> VOID  __AAVGeometryObject_OPENGL<T>::Render(bool bRenderWithTrueColor)
{
  //static const D3DMATERIAL9 kDefaultMaterial = 
  //{
  //  {0.4f, 0.8f, 0.9f, 1.0f},
  //  {0.4f, 0.8f, 0.9f, 1.0f},
  //  {0.4f, 0.8f, 0.9f, 1.0f},
  //  {0.4f, 0.8f, 0.9f, 1.0f},
  //  1.0f
  //};

  AAV_ASSERT_RETURN_VOID(m_pVertexBuffer != NULL);
  AAV_ASSERT_RETURN_VOID(m_pkDevice != NULL);

  //m_pkDevice->SetMaterial(bRenderWithTrueColor ? &m_Material : &kDefaultMaterial);
  //m_pkDevice->SetFVF( m_dwFVF );
  m_pkDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(T) );
  m_hResult = m_pkDevice->DrawPrimitive( (D3DPRIMITIVETYPE)m_eTopology, 0, m_uiNumPrimitives);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

template <class T> VOID __AAVGeometryObject_OPENGL<T>::RenderPrimitives(UINT StartVertex, UINT uiNumPrimitives, bool bRenderWithTrueColor = false)
{
  //static const D3DMATERIAL9 kDefaultMaterial = 
  //{
  //  {0.4f, 0.8f, 0.9f, 1.0f},
  //  {0.4f, 0.8f, 0.9f, 1.0f},
  //  {0.4f, 0.8f, 0.9f, 1.0f},
  //  {0.4f, 0.8f, 0.9f, 1.0f},
  //  1.0f
  //};

  AAV_ASSERT_RETURN_VOID(m_pVertexBuffer != NULL);
  AAV_ASSERT_RETURN_VOID(m_pkDevice != NULL);

  //m_pkDevice->SetMaterial(bRenderWithTrueColor ? &m_Material : &kDefaultMaterial);
  //m_pkDevice->SetFVF( m_dwFVF );
  m_pkDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(T) );
  m_hResult = m_pkDevice->DrawPrimitive( (D3DPRIMITIVETYPE)m_eTopology, StartVertex, uiNumPrimitives);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

template <class T> HRESULT __AAVGeometryObject_OPENGL<T>::SetTexture(__AAVTextureObject* pkTextureObject)
{
  if(pkTextureObject != NULL)
  {
    m_pkDevice->SetTexture(0, pkTextureObject->GetTexture());
    m_pkGraphics->m_pkStateBlock[__AAVGraphics_OPENGL::STATE_BLOCK_TRANSPARENT_TEXTURE]->Apply();
  }
  else
  {
    m_pkDevice->SetTexture(0, NULL);
    m_pkGraphics->m_pkStateBlock[__AAVGraphics_OPENGL::STATE_BLOCK_TRANSPARENT_TEXTURE]->Apply();
    //m_pkGraphics->m_pkStateBlock[__AAVGraphics_OPENGL::STATE_BLOCK_NULL_TEXTURE]->Apply();

  }
  return S_OK;
}

#endif //__AAV_GEOMETRY_OBJECT_OPENGL_H__
