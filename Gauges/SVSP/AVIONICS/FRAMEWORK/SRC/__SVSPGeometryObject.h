#ifndef __SVSP_GEOMETRY_OBJECT_H__
#define __SVSP_GEOMETRY_OBJECT_H__

#include <WINDOWS.h>
#include <__SVSPTextureObject.h>
#include <__SVSPColor.h>

class __SVSPVertex
{
public:
  FLOAT       x, y, z;
  DWORD       color;
  FLOAT       tu, tv;

  __SVSPVertex()
  {
  }

  inline __SVSPVertex(FLOAT x, FLOAT y, FLOAT z, FLOAT tu = 0, FLOAT tv = 0, DWORD color = __SVSPColor::dwCOLOR)
  {
    this->x     = x;
    this->y     = y;
    this->z     = z;
    this->color = color;
    this->tu    = tu;
    this->tv    = tv;
  }

  inline __SVSPVertex(D3DXVECTOR3 &rv, FLOAT tu = 0, FLOAT tv = 0, DWORD color = __SVSPColor::dwCOLOR)
  {
    this->x     = rv.x;
    this->y     = rv.y;
    this->z     = rv.z;
    this->color = color;
    this->tu    = tu;
    this->tv    = tv;
  }
};
typedef __SVSPVertex *LPCUSTOMVERTEX;

template <class T> class __SVSPGeometryObject
{
public:

  typedef enum 
  {
    UNDEFINED     = 0,
    POINTLIST     = 1,
    LINELIST      = 2,
    LINESTRIP     = 3,
    TRIANGLELIST  = 4,
    TRIANGLESTRIP = 5,
  } __SVSPTopology;

  typedef enum
  {
    USAGE_UNDEFINED = 0,
    USAGE_STATIC    = 1,
    USAGE_DYNAMIC   = 2,
  } __SVSPVertexBufferUsage;
                               __SVSPGeometryObject(DWORD eTopology, UINT uiNumVerts, DWORD dwUsage = __SVSPGeometryObject::USAGE_STATIC);
  virtual                     ~__SVSPGeometryObject()                                                            = 0;
  virtual VOID                 Release             ()                                                            = 0;
  virtual HRESULT              Reset               (LPD3DXDEVICE pkDevice)                                       = 0;
  virtual HRESULT              CreateVertexBuffer  (LPD3DXDEVICE pkDevice)                                       = 0;
  virtual VOID                 Render              (LPD3DXDEVICE pkDevice, UINT uiNumVertices, UINT StartVertex, bool bRenderWithTrueColor = false);
  virtual VOID                 RenderPrimitives    (LPD3DXDEVICE pkDevice, UINT StartVertex, UINT uiNumPrimitives, bool bRenderWithTrueColor = false);
  virtual VOID                 Render              (LPD3DXDEVICE pkDevice, bool bRenderWithTrueColor = false);
  virtual HRESULT              SetTexture          (LPD3DXDEVICE pkDevice, __SVSPTextureObject* pkTextureObject) = 0;
  virtual VOID                 SetTransform        (LPD3DXDEVICE pkDevice, struct D3DXMATRIX *pkTransformMatrix) = 0;
  virtual VOID                 SetMaterial         (FLOAT r, FLOAT g, FLOAT b, FLOAT a)   = 0;

  inline  T*                   GetVerts() const { return m_pVerts; }
  inline  UINT                 GetNumVerts() const { return m_uiNumVerts; }

protected:
  DWORD                        m_eTopology;
  UINT                         m_uiNumVerts;
  T*                           m_pVerts;
  LPD3DXVERTEXBUFFER           m_pVertexBuffer;
  HRESULT                      m_hResult;
  DWORD                        m_dwFVF;
  DWORD                        m_dwUsage;
};

template<typename T> __SVSPGeometryObject<T>::__SVSPGeometryObject(DWORD eTopology, UINT uiNumVerts, DWORD dwUsage) :
  m_eTopology     (eTopology),
  m_uiNumVerts    (uiNumVerts),
  m_pVerts        (NULL),
  m_pVertexBuffer (NULL)
{
}
template<typename T>          __SVSPGeometryObject<T>::~__SVSPGeometryObject(){}
template<typename T> VOID     __SVSPGeometryObject<T>::Release              () {}
template<typename T> HRESULT  __SVSPGeometryObject<T>::Reset                (LPD3DXDEVICE pkDevice) {}
template<typename T> HRESULT  __SVSPGeometryObject<T>::CreateVertexBuffer   (LPD3DXDEVICE pkDevice) { return false; }
template<typename T> VOID     __SVSPGeometryObject<T>::Render               (LPD3DXDEVICE pkDevice, UINT uiNumVertices, UINT StartVertex, bool bRenderWithTrueColor) {}
template<typename T> VOID     __SVSPGeometryObject<T>::Render               (LPD3DXDEVICE pkDevice, bool bRenderWithTrueColor) {}
template<typename T> VOID     __SVSPGeometryObject<T>::RenderPrimitives     (LPD3DXDEVICE pkDevice, UINT StartVertex, UINT uiNumPrimitives, bool bRenderWithTrueColor = false) {}
template<typename T> HRESULT  __SVSPGeometryObject<T>::SetTexture           (LPD3DXDEVICE pkDevice, __SVSPTextureObject* pkTextureObject) { return S_OK; }
template<typename T> VOID     __SVSPGeometryObject<T>::SetTransform         (LPD3DXDEVICE pkDevice, D3DXMATRIX *pkTransformMatrix) {}
template<typename T> VOID     __SVSPGeometryObject<T>::SetMaterial          (FLOAT r, FLOAT g, FLOAT b, FLOAT a) {}


#endif	/* __SVSP_GEOMETRY_OBJECT_H__ */


