#ifndef __SVSP_GRAPHICS_H__
#define __SVSP_GRAPHICS_H__

#include <__SVSPTypes.h>

class __SVSPGraphics
{
public:


                        __SVSPGraphics();
  virtual              ~__SVSPGraphics()                                                         = 0;
  inline  LPD3DXDEVICE  GetDevice() const { return m_pkDevice; }
  inline  VOID          SetDevice(LPD3DXDEVICE pkDevice) { m_pkDevice = pkDevice; }
  virtual VOID          SetShaders()                                                            = 0;
  virtual VOID          ResetShaders()                                                          = 0;
  virtual VOID          SetScissorRect(const RECT* pRect)                                       = 0;
  virtual VOID          GetViewMatrix(D3DXMATRIX *pkViewMatrix)                                                       = 0;
  virtual VOID          SetViewMatrix(D3DXMATRIX *pkViewMatrix)                                                       = 0;
  virtual VOID          SetViewMatrix(D3DXVECTOR3 *pkEyeVector, D3DXVECTOR3 *pkLookAtVector, D3DXVECTOR3 *pkUpVector) = 0;
  virtual VOID          SetTransform(D3DXMATRIX *pkTransformMatrix)                                                   = 0;
//  virtual VOID          SetGlobalMaterial()                                                     = 0;
  inline class __SVSPFont* GetFont() const { return m_pkFont; }



protected:
  class __SVSPModule    *m_pkAvionics;
  class __SVSPModule    *m_pkHUD;
  class __SVSPModule    *m_pkHDD;
  LPD3DXDEVICE          m_pkDevice;
  class __SVSPFont      *m_pkFont;
};

#endif	/* __SVSP_GRAPHICS_H__ */


