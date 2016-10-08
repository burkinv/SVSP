#ifndef __SVSP_TEXTURE_OBJECT_D3D9_H__
#define __SVSP_TEXTURE_OBJECT_D3D9_H__

#include "__SVSPGraphics_D3D9.h"
#include "__SVSPTextureObject.h"

class __SVSPTextureObject_D3D9 : public __SVSPTextureObject
{
public:
                               __SVSPTextureObject_D3D9(LPCTSTR pSrcFile);
  virtual                     ~__SVSPTextureObject_D3D9();
  virtual VOID                 Release();
  virtual HRESULT              Reset(LPD3DXDEVICE pkDevice);
};

#endif //__SVSP_TEXTURE_OBJECT_D3D9_H__
