#ifndef __SVSP_TEXTURE_RENDERER_H__
#define __SVSP_TEXTURE_RENDERER_H__

#include <__SVSPTypes.h>

class __SVSPTextureRenderer
{
public:
                        __SVSPTextureRenderer(UINT uiWidth, UINT uiHeight);
  virtual              ~__SVSPTextureRenderer()                                                 = 0;
  virtual VOID          Clear(DWORD dwColor)                                                    = 0;
  virtual VOID          Begin()                                                                 = 0;
  virtual VOID          End()                                                                   = 0;
  virtual VOID          GetBackbufferData(struct IMAGE* pImage)                                 = 0;
  virtual VOID          SetOnScreenRenderingModule(class __SVSPModule *pkModule);
  virtual VOID          SetDefaultShaders()                                                     = 0;
};

#endif	/* __SVSP_TEXTURE_RENDERER_H__ */


