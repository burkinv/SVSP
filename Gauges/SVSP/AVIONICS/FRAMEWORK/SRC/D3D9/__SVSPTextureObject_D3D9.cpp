#include "__SVSPTextureObject_D3D9.h"
#include "__SVSPGeometryObject_D3D9.h"
#include "__SVSPDebug.h"

__SVSPTextureObject_D3D9::__SVSPTextureObject_D3D9(LPCTSTR pSrcFile) : __SVSPTextureObject(pSrcFile)
{
}

__SVSPTextureObject_D3D9::~__SVSPTextureObject_D3D9()
{
  Release();
}

VOID __SVSPTextureObject_D3D9::Release()
{
  if(m_pkTexture != NULL)
  {
    m_pkTexture->Release();
    m_pkTexture = NULL;
  }
}

HRESULT __SVSPTextureObject_D3D9::Reset(LPD3DXDEVICE pkDevice)
{
  AAV_ASSERT_RETURN(strlen(m_pSrcFile) != 0, E_FAIL);

  return D3DXCreateTextureFromFile(pkDevice, m_pSrcFile, &m_pkTexture);
}

