#ifndef __AAV_GEOMETRY_OBJECT_D3D10_H__
#define __AAV_GEOMETRY_OBJECT_D3D10_H__

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <GAUGES.h>
#include "__AAVDebug.h"
#include "__AAVGeometryObject.h"
#include "__AAVGraphics_D3D10.h"

typedef D3D10_PRIMITIVE_TOPOLOGY D3DXPRIMITIVETYPE;

class __AAVVertex
{
public:
  D3DXVECTOR3 v;
  D3DXVECTOR4 color;

  __AAVVertex()
  {
  }

  __AAVVertex(D3DXVECTOR3 &rv, DWORD color)
  {
    v           = rv;
    this->color = D3DXVECTOR4((color & 0x000000ff)/255.0f, ((color & 0x0000ff00) >> 8)/255.0f, ((color & 0x00ff0000) >> 16)/255.0f, ((color & 0xff000000) >> 24)/255.0f);
  }

  __AAVVertex(FLOAT x, FLOAT y, FLOAT z, DWORD color)
  {
    v           = D3DXVECTOR3(x, y, z);
    this->color = D3DXVECTOR4((color & 0x000000ff)/255.0f, ((color & 0x0000ff00) >> 8)/255.0f, ((color & 0x00ff0000) >> 16)/255.0f, ((color & 0xff000000) >> 24)/255.0f);
  }
};
typedef __AAVVertex *LPCUSTOMVERTEX;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)


class __AAVVertex
{
public:
    D3DXVECTOR3 v;
    D3DXVECTOR4 color;
    FLOAT tu, tv;

  __AAVVertex()
  {
  }

  __AAVVertex(D3DXVECTOR3 &rv, DWORD color, FLOAT tu, FLOAT tv)
  {
    this->v     = rv;
    this->color = D3DXVECTOR4((color & 0x000000ff)/255.0f, ((color & 0x0000ff00) >> 8)/255.0f, ((color & 0x00ff0000) >> 16)/255.0f, ((color & 0xff000000) >> 24)/255.0f);
    this->tu    = tu;
    this->tv    = tv;
  }

  __AAVVertex(FLOAT x, FLOAT y, FLOAT z, DWORD color, FLOAT tu, FLOAT tv)
  {
    this->v     = D3DXVECTOR3(x, y, z);
    this->color = D3DXVECTOR4((color & 0x000000ff)/255.0f, ((color & 0x0000ff00) >> 8)/255.0f, ((color & 0x00ff0000) >> 16)/255.0f, ((color & 0xff000000) >> 24)/255.0f);
    this->tu    = tu;
    this->tv    = tv;
  }

};
#define D3DFVF_CUSTOMTEXVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)


template <class T> class __AAVGeometryObject_D3D10 : public __AAVGeometryObject<T>
{
public:
  typedef enum 
  {
    UNDEFINED     = D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED,
    POINTLIST     = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST,
    LINELIST      = D3D10_PRIMITIVE_TOPOLOGY_LINELIST,
    LINESTRIP     = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,
    TRIANGLELIST  = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    TRIANGLESTRIP = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
  } TOPOLOGY;

              __AAVGeometryObject_D3D10(__AAVGraphics_D3D10 *pkGraphics, DWORD eTopology, UINT uiNumVerts);
  virtual    ~__AAVGeometryObject_D3D10();
  VOID        SetTransform(D3DXMATRIX *pkTransformMatrix);
  VOID        SetMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
  VOID        Release();
  HRESULT     CreateVertexBuffer();
  VOID        Update();
  HRESULT     Reset();
  HRESULT     SetTexture(LPD3DXTEXTURE pkTexture);
  HRESULT     SetTextureFromFile(LPCSTR swFilename);
  VOID        Render(UINT uiNumVertices, UINT StartVertex, bool bRenderWithTrueColor = false);
  inline VOID Render(bool bRenderWithTrueColor = false)
  {
    Render( m_uiNumVerts, 0, bRenderWithTrueColor);
  }


private:
  ID3D10Buffer                *m_pVertexBuffer;
  ID3D10ShaderResourceView    *m_pkPixelShaderResourceView;
  UINT                         m_uiStride;
  UINT                         m_uiOffset;
};


template <class T> __AAVGeometryObject_D3D10<T>::__AAVGeometryObject_D3D10(__AAVGraphics_D3D10 *pkGraphics, DWORD eTopology, UINT uiNumVerts) :
__AAVGeometryObject<T>(pkGraphics, eTopology, uiNumVerts),
m_pkPixelShaderResourceView(NULL)
{
  m_pVerts                   = new T[m_uiNumVerts];
  m_uiStride                 = sizeof(T);
  m_uiOffset                 = 0;

  m_hResult = CreateVertexBuffer();
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));
}

template <class T> __AAVGeometryObject_D3D10<T>::~__AAVGeometryObject_D3D10()
{
  if(m_pVertexBuffer != NULL) 
  {
    m_pVertexBuffer->Release();
    m_pVertexBuffer = NULL;
  }

  if(m_pVerts != NULL) 
    delete[] m_pVerts;
}

template <class T> VOID __AAVGeometryObject_D3D10<T>::SetTransform(D3DXMATRIX *pkTransformMatrix)
{
  if((m_pkGraphics != NULL) && (pkTransformMatrix != NULL))
  {
    ((__AAVGraphics_D3D10 *)m_pkGraphics)->m_pkTransformVariable->SetMatrix(( float* )pkTransformMatrix);
  }
}

template <class T> VOID __AAVGeometryObject_D3D10<T>::SetMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
}

template <class T> VOID __AAVGeometryObject_D3D10<T>::Release()
{
}

template <class T> HRESULT __AAVGeometryObject_D3D10<T>::Reset()
{
  return S_OK;
}

template <class T> HRESULT __AAVGeometryObject_D3D10<T>::CreateVertexBuffer()
{
  if((m_pkGraphics->GetDevice() != NULL) && (m_uiNumVerts != 0)) 
  {
    D3D10_BUFFER_DESC bd;
    bd.Usage          = D3D10_USAGE_DEFAULT;
    bd.ByteWidth      = m_uiNumVerts * sizeof( T );
    bd.BindFlags      = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags      = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem  = m_pVerts;

    if( SUCCEEDED( m_pkGraphics->GetDevice()->CreateBuffer( &bd, &InitData, &m_pVertexBuffer  ) ) )
    {
      return S_OK;
    }
    else
    {
      return E_FAIL;
    }
  }
  return E_FAIL;
}
  
template <class T>  VOID  __AAVGeometryObject_D3D10<T>::Update()
{
  if(m_pVertexBuffer != NULL)
  {
    D3D10_BOX kDstRegion;
    kDstRegion.left   = 0;
    kDstRegion.right  = m_uiNumVerts*sizeof(T);
    kDstRegion.top    = 0;
    kDstRegion.bottom = 1;
    kDstRegion.front  = 0;
    kDstRegion.back   = 1;

    m_pkGraphics->GetDevice()->UpdateSubresource(m_pVertexBuffer, 0, &kDstRegion, m_pVerts, sizeof(T), 0);
  }
  else
  {
    m_hResult = CreateVertexBuffer();
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));
  }
}

template <class T> VOID __AAVGeometryObject_D3D10<T>::Render(UINT uiNumVertices, UINT StartVertex, bool bRenderWithTrueColor)
{
  if(m_pVertexBuffer != NULL)
  {
    if(m_pkPixelShaderResourceView == NULL)
    {
      m_pkGraphics->GetDevice()->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_uiStride, &m_uiOffset );
      m_pkGraphics->GetDevice()->IASetPrimitiveTopology((D3D10_PRIMITIVE_TOPOLOGY)m_eTopology);
      m_pkGraphics->GetDevice()->IASetInputLayout(m_pkGraphics->m_pkVertexLayout);
      m_pkGraphics->m_pkTechnique->GetPassByIndex(0)->Apply(0);
    }
    else
    {
      m_pkGraphics->m_pkDiffuseVariable->SetResource(m_pkPixelShaderResourceView);
      m_pkGraphics->GetDevice()->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_uiStride, &m_uiOffset );
      m_pkGraphics->GetDevice()->IASetPrimitiveTopology((D3D10_PRIMITIVE_TOPOLOGY)m_eTopology);
      m_pkGraphics->GetDevice()->IASetInputLayout(m_pkGraphics->m_pkTexVertexLayout);
      m_pkGraphics->m_pkTechnique->GetPassByIndex(1)->Apply(0);
    }
    m_pkGraphics->GetDevice()->Draw(uiNumVertices, StartVertex);
  }
  else
  {
    m_hResult = CreateVertexBuffer();
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));
  }
}

template <class T> HRESULT __AAVGeometryObject_D3D10<T>::SetTexture(LPD3DXTEXTURE pkTexture)
{
  m_pkTexture = pkTexture;
  m_hResult = m_pkGraphics->GetDevice()->CreateShaderResourceView((ID3D10Texture2D*)pkTexture, NULL, &m_pkPixelShaderResourceView);

  return m_hResult;
}

template <class T> HRESULT __AAVGeometryObject_D3D10<T>::SetTextureFromFile(LPCSTR swFilename)
{
  //m_pkTexture = pkTexture;
  //m_hResult = m_pkGraphics->GetDevice()->CreateShaderResourceView((ID3D10Texture2D*)pkTexture, NULL, &m_pkPixelShaderResourceView);

  return m_hResult;
}


#endif //__AAV_GEOMETRY_OBJECT_D3D10_H__
