#include "__AAVTextureObject_OPENGL.h"
#include "__AAVDebug.h"

__AAVTextureObject_OPENGL::__AAVTextureObject_OPENGL(__AAVGraphics_OPENGL *pkGraphics, LPCTSTR pSrcFile) : __AAVTextureObject(pkGraphics, pSrcFile)
{
  m_pkDevice = NULL;
}

__AAVTextureObject_OPENGL::~__AAVTextureObject_OPENGL()
{
  Release();
}

VOID __AAVTextureObject_OPENGL::Release()
{
  if(m_pkTexture != NULL)
  {
    m_pkTexture->Release();
    m_pkTexture = NULL;
  }
}

HRESULT __AAVTextureObject_OPENGL::Reset()
{
  m_pkDevice = pkDevice;
  Update();

  return ((m_pkTexture != NULL) ? S_OK : E_FAIL);
}

HRESULT __AAVTextureObject_OPENGL::Update()
{
  AAV_ASSERT_RETURN(strlen(m_pSrcFile) != 0, E_FAIL);
  AAV_ASSERT_RETURN(m_pkDevice != NULL, E_FAIL);

  return D3DXCreateTextureFromFile(m_pkDevice, m_pSrcFile, &m_pkTexture);
}

