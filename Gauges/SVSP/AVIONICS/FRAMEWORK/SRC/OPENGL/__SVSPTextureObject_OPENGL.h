#ifndef __AAV_TEXTURE_OBJECT_OPENGL_H__
#define __AAV_TEXTURE_OBJECT_OPENGL_H__

#include "__AAVGraphics_OPENGL.h"
#include "__AAVTextureObject.h"

class __AAVTextureObject_OPENGL : public __AAVTextureObject
{
public:
                               __AAVTextureObject_OPENGL(__AAVGraphics_OPENGL *pkGraphics, LPCTSTR pSrcFile);
  virtual                     ~__AAVTextureObject_OPENGL();
  virtual VOID                 Release();
  virtual HRESULT              Reset();
  virtual HRESULT              Update();

protected:
  LPDIRECT3DDEVICE9            m_pkDevice;
};

#endif //__AAV_TEXTURE_OBJECT_OPENGL_H__
