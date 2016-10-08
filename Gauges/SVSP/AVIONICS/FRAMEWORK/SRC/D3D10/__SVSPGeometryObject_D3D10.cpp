#include "__AAVGeometryObject_D3D10.h"

template <class T> __AAVGeometryObject_D3D10<T>::__AAVGeometryObject_D3D10(DWORD eTopology, UINT uiNumVerts) :
m_pkGraphics(NULL),
m_pVertexBuffer(NULL)
{
  m_pVerts                   = new T[m_uiNumVerts];
  m_uiStride                 = sizeof(T);
  m_uiOffset                 = 0;
}

//template <class T> __AAVGeometryObject_D3D10<T>::__AAVGeometryObject_D3D10<T>(TOPOLOGY eTopology, UINT uiNumVerts) :
//__AAVGeometryObject<T>((__AAVGeometryObject<T>::__AAVTopology)eTopology, uiNumVerts),
//m_pkGraphics(NULL),
//m_pVertexBuffer(NULL)
//{
//  m_pVerts                   = new T[m_uiNumVerts];
//  m_uiStride                 = sizeof(T);
//  m_uiOffset                 = 0;
//}

//template <class T> __AAVGeometryObject_D3D10<T>::~__AAVGeometryObject_D3D10()
//{
//  if(m_pVertexBuffer != NULL) 
//  {
//    m_pVertexBuffer->Release();
//    m_pVertexBuffer = NULL;
//  }
//
//  if(m_pVerts != NULL) 
//    delete[] m_pVerts;
//}

//template <class T> VOID __AAVGeometryObject_D3D10<T>::SetTransform(D3DXMATRIX *pkTransformMatrix)
//{
//  if((m_pkGraphics != NULL) && (pkTransformMatrix != NULL))
//  {
//    m_pkGraphics->m_pkTransformVariable->SetMatrix(( float* )pkTransformMatrix);
//  }
//}

//template <class T> VOID __AAVGeometryObject_D3D10<T>::SetMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
//{
//}
//
//template <class T> VOID __AAVGeometryObject_D3D10<T>::Release()
//{
//}
//
//template <class T> HRESULT __AAVGeometryObject_D3D10<T>::CreateVertexBuffer()
//{
//  if((m_pkGraphics->m_pkDevice != NULL) && (m_uiNumVerts != 0)) 
//  {
//    D3D10_BUFFER_DESC bd;
//    bd.Usage          = D3D10_USAGE_DEFAULT;
//    bd.ByteWidth      = m_uiNumVerts * sizeof( __AAVVertex );
//    bd.BindFlags      = D3D10_BIND_VERTEX_BUFFER;
//    bd.CPUAccessFlags = 0;
//    bd.MiscFlags      = 0;
//    D3D10_SUBRESOURCE_DATA InitData;
//    InitData.pSysMem  = m_pVerts;
//
//    if( SUCCEEDED( m_pkGraphics->m_pkDevice->CreateBuffer( &bd, &InitData, &m_pVertexBuffer  ) ) )
//    {
//      return S_OK;
//    }
//    else
//    {
//      return E_FAIL;
//    }
//  }
//  return E_FAIL;
//}
//  
//template <class T>  VOID  Update()
//{
//}
//
//template <class T> VOID __AAVGeometryObject_D3D10<T>::Render(__AAVGraphics *pkGraphics, UINT uiNumVertices, UINT StartVertex, bool bRenderWithTrueColor)
//{
//  if(m_pVertexBuffer != NULL)
//  {
//    m_pkGraphics->m_pkDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_uiStride, &m_uiOffset );
//    m_pkGraphics->m_pkDevice->IASetPrimitiveTopology((D3D10_PRIMITIVE_TOPOLOGY)m_eTopology);
//    m_pkGraphics->m_pkTechnique->GetPassByIndex(0)->Apply(0);
//    m_pkGraphics->m_pkDevice->Draw(uiNumVertices, StartVertex);
//  }
//  else
//  {
//    m_pkGraphics = (__AAVGraphics_D3D10 *)pkGraphics;
//    m_hResult = CreateVertexBuffer();
//    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));
//  }
//}
