#ifndef __SVSP_TEXTURE_OBJECT_H__
#define __SVSP_TEXTURE_OBJECT_H__

#include <WINDOWS.h>
#include <__SVSPGraphics.h>

class __SVSPTextureObject
{
public:
                               __SVSPTextureObject(LPCTSTR pSrcFile);
  virtual                     ~__SVSPTextureObject()                                                    = 0;
  virtual VOID                 Release            ()                                                    = 0;
  virtual HRESULT              Reset              (LPD3DXDEVICE pkDevice)                               = 0;
  inline  LPD3DXTEXTURE        GetTexture() const { return m_pkTexture; }

protected:
  LPCTSTR                      m_pSrcFile;
  LPD3DXTEXTURE                m_pkTexture;
};


#endif	/* __SVSP_TEXTURE_OBJECT_H__ */


