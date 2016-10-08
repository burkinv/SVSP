#include <__SVSPGraphics.h>

        __SVSPGraphics::__SVSPGraphics() : 
  m_pkAvionics   (NULL),
  m_pkHUD        (NULL),
  m_pkHDD        (NULL),
  m_pkDevice     (NULL),
  m_pkFont       (NULL)
{
}
        __SVSPGraphics::~__SVSPGraphics() {}
VOID    __SVSPGraphics::SetShaders() {}
VOID    __SVSPGraphics::ResetShaders() {}
VOID    __SVSPGraphics::SetScissorRect(const RECT* pRect) {}
VOID    __SVSPGraphics::GetViewMatrix(D3DXMATRIX *pkViewMatrix) {}
VOID    __SVSPGraphics::SetViewMatrix(D3DXMATRIX *pkViewMatrix) {}
VOID    __SVSPGraphics::SetViewMatrix(D3DXVECTOR3 *pkEyeVector, D3DXVECTOR3 *pkLookAtVector, D3DXVECTOR3 *pkUpVector) {}
VOID    __SVSPGraphics::SetTransform(D3DXMATRIX *pkTransformMatrix) {}

//VOID    __SVSPGraphics::SetGlobalMaterial() {}

