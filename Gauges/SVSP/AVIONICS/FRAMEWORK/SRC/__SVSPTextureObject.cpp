
#include <__SVSPTextureObject.h>

__SVSPTextureObject::__SVSPTextureObject(LPCTSTR pSrcFile):
m_pSrcFile(pSrcFile),
m_pkTexture(NULL)
{
}

__SVSPTextureObject::~__SVSPTextureObject()
{
}

VOID __SVSPTextureObject::Release()
{
}

HRESULT __SVSPTextureObject::Reset(LPD3DXDEVICE pkDevice)
{
  return S_OK;
}


