#include "__AAVGraphics_OPENGL.h"
#include "__AAVDebug.h"
#include "__AAVFont_OPENGL.h"
#include "__AAVOPENGL.h"
#include "__AAVAvionics.h"
#include "HUD.h"
#include "HDD.h"

#define CONNECT_TO_AAVOPENGL_THROUGH_STATIC_WRAPPER

/**
__AAVGraphics_OPENGL::Release_Callback
*/
ULONG   FASTCALL __AAVGraphics_OPENGL::Release_Callback              (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice)
{
  if(!pkThis->m_bReleased)
  {
    pkThis->Release();
  }

  ULONG ulRefCount = pkDevice->Release();

  //CHAR acStr[64];
  //sprintf_s(&acStr[0], sizeof(acStr), "RefCount = %d", ulRefCount);
  //__AAVDEBUG::OUTPUT(&acStr[0]);
  return ulRefCount;
}

/**
__AAVGraphics_OPENGL::Reset_Callback
*/
HRESULT FASTCALL __AAVGraphics_OPENGL::Reset_Callback                (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  HRESULT hResult = pkDevice->Reset(pPresentationParameters);
  if(hResult != S_OK)
  {
    switch(hResult)
    {
    case D3DERR_DEVICELOST:
      AAV_ASSERT_RETURN(FALSE, hResult);
      break;
    case D3DERR_DRIVERINTERNALERROR:
      AAV_ASSERT_RETURN(FALSE, hResult);
      break;
    case D3DERR_INVALIDCALL:
      AAV_ASSERT_RETURN(FALSE, hResult);
      break;
    case D3DERR_OUTOFVIDEOMEMORY:
      AAV_ASSERT_RETURN(FALSE, hResult);
      break;
    case E_OUTOFMEMORY:
      AAV_ASSERT_RETURN(FALSE, hResult);
      break;
    default:
      AAV_ASSERT_RETURN(FALSE, hResult);
      break;
    }
  }

  if(pkThis->m_bReleased)
  {
    pkThis->Reset(pkDevice, (DWORD *)pPresentationParameters);
  }
  return hResult;
}

/**
__AAVGraphics_OPENGL::Present_Callback
*/
HRESULT FASTCALL __AAVGraphics_OPENGL::Present_Callback              (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
  pkThis->m_bRender = true;
  return pkDevice->Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
}

HRESULT FASTCALL __AAVGraphics_OPENGL::EndScene_Callback             (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  if(pkThis->m_bRender)
  {
    if(pkThis->m_bReleased)
    {
      pkThis->Reset(pkDevice, (DWORD *)pPresentationParameters);
    }
    else
    {
      pkThis->m_pkAvionics->Render();
    }
    pkThis->m_bRender = false;
  }
  return pkDevice->EndScene();
}

/**
__AAVGraphics_OPENGL::CreateTexture_Callback
*/
HRESULT FASTCALL __AAVGraphics_OPENGL::CreateTexture_Callback        (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
  HRESULT hResult = S_OK;

  if((pkThis->m_pkHDD != NULL) && (Width == 1024) && (Height == 1024) && (Format == D3DFMT_A1R5G5B5))
  {
    if(pkThis->m_pkHDDTexture != NULL)
    {
      *ppTexture = pkThis->m_pkHDDTexture;
    }
    else
    {
      Width                  = HDD::FRAMEBUFFER_WIDTH;
      Height                 = HDD::FRAMEBUFFER_HEIGHT;
      Levels                 = 1;
      Format                 = D3DFMT_X8R8G8B8;
      hResult                = pkDevice->CreateTexture(Width,Height, Levels, Usage, Format, Pool,ppTexture, pSharedHandle);
      pkThis->m_pkHDDTexture = *ppTexture;
    }
  }
  else if((pkThis->m_pkHUD != NULL) && (Width == 4) && (Height == 512))
  {
    if(pkThis->m_pkHUDTexture != NULL)
    {
      *ppTexture = pkThis->m_pkHUDTexture;
    }
    else
    {
      hResult               = pkDevice->CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture, pSharedHandle);
      pkThis->m_pkHUDTexture = *ppTexture;
    }
  }
  else
  {
    hResult = pkDevice->CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle);
  }

  return hResult;
}

/**
__AAVGraphics_OPENGL::SetTexture_Callback
*/
HRESULT FASTCALL __AAVGraphics_OPENGL::SetTexture_Callback          (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
  if((!pkThis->m_bReleased) && (pTexture != NULL))
  {
    if(pTexture == pkThis->m_pkHUDTexture)
    {
      AAV_ASSERT_RETURN(pkThis->m_pkHUD != NULL, S_FALSE);
      //Register__AAVOPENGL_CALLBACK_DRAW_INDEXED_PRIMITIVE for the upcomming HUD stencil rendering (Only in HUD::DISPLAY_MODE_HUD).
      if(pkThis->m_pkHUD->GetMode() == HUD::DISPLAY_MODE_HUD)
      {
        __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_DRAW_INDEXED_PRIMITIVE, (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::DrawIndexedPrimitive_Callback, pkThis);
      }
    }
    else if(pTexture == pkThis->m_pkHDDTexture)
    {
      AAV_ASSERT_RETURN(pkThis->m_pkHDD != NULL, S_FALSE);
      {
        //Replace the default 16Bit gauge texture with 32Bit HDD display texture.
        pTexture = pkThis->m_pkHDD->GetDisplayTexture();
      }
    }
  }
  return pkDevice->SetTexture(Stage,pTexture);
}

/**
__AAVGraphics_OPENGL::DrawIndexedPrimitive_Callback
  Must be called ones per frame whenever HUD stencil is set as an active texture, then unregistered.
*/
HRESULT FASTCALL __AAVGraphics_OPENGL::DrawIndexedPrimitive_Callback (__AAVGraphics_OPENGL *pkThis, LPDIRECT3DDEVICE9 pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
  HRESULT                hResult  = S_OK;
  IDirect3DBaseTexture9* pTexture = NULL;

  //Get active texture, this call increases internal reference count.
  pkDevice->GetTexture(0,&pTexture);
  //Decrease reference count to active texture.
  if(pTexture != NULL)
  {
    pTexture->Release();
  }

  if((!pkThis->m_bReleased) && (pTexture == pkThis->m_pkHUDTexture))
  {
    //Unregister __AAVOPENGL_CALLBACK_DRAW_INDEXED_PRIMITIVE
    __AAVOPENGL_UnregisterCallback(__AAVOPENGL_CALLBACK_DRAW_INDEXED_PRIMITIVE);

    AAV_ASSERT_RETURN(pkThis->m_pkHUD != NULL,                 S_FALSE);

    // Set HUD Depth-Stencil surface.
    hResult = pkThis->SetHUDStencilSurface();
    AAV_ASSERT_RETURN(SUCCEEDED(hResult), S_FALSE);

    // Clear HUD Depth-Stencil surface.
    //hResult = pkDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0, 1.0f, 0);
    hResult = pkDevice->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);
    AAV_ASSERT_RETURN(SUCCEEDED(hResult), S_FALSE);

    // Set Depth-Stencil render state to always fill in stencil buffer.
    pkDevice->SetRenderState(D3DRS_STENCILREF,       1);
    pkDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_NEVER);
    pkDevice->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_REPLACE);
    pkDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_KEEP);

    // Fill in the HUD stencil surface.
    hResult = pkDevice->DrawIndexedPrimitive(PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);

    // Disable HUD Depth-Stencil surface.
    hResult = pkThis->DisableHUDStencilSurface();
    AAV_ASSERT_RETURN(SUCCEEDED(hResult), S_FALSE);
  }
  else
  {
    hResult = pkDevice->DrawIndexedPrimitive(PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
  }

  return hResult;
}

__AAVGraphics_OPENGL::__AAVGraphics_OPENGL(class __AAVAvionics *pkAvionics) : __AAVGraphics(pkAvionics),
  m_bReleased                    (TRUE),
  m_bRender                      (FALSE),
  m_bUpdateHUDStencilBuffer      (FALSE),
  m_pkHUD                        (NULL),
  m_pkHDD                        (NULL),
  m_bStencilVisible              (FALSE),
  m_pkHDDTexture                 (NULL),
  m_pkHUDTexture                 (NULL),
  m_pkHUDDepthStencilSurface     (NULL),
  m_pkDefaultDepthStencilSurface (NULL),
  m_dwDepthStencilTestEnable     (0)

{
  //Connect to __AAVOPENGL layer. (Will couse "Entry Point Not Found" exception if no __AAVOPENGL.DLL code is loaded.
#if defined(CONNECT_TO_AAVOPENGL_THROUGH_STATIC_WRAPPER)
  //Connect to __AAVOPENGL layer through the static member wrapers.
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_RELEASE,                (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::Release_Callback,              this);
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_RESET,                  (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::Reset_Callback,                this);
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_PRESENT,                (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::Present_Callback,              this);
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_END_SCENE,              (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::EndScene_Callback,             this);
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_CREATE_TEXTURE,         (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::CreateTexture_Callback,        this);
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_SET_TEXTURE,            (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::SetTexture_Callback,           this);

  //__AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_DRAW_INDEXED_PRIMITIVE, (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::DrawIndexedPrimitive_Callback, this);
#else
  //Connect to __AAVOPENGL layer through non-static members directly.
  VOID (FASTCALL __AAVGraphics_OPENGL::*__ReleaseMethodCallback)() = &__AAVGraphics_OPENGL::Release;
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_RELEASE,                *(__AAVOPENGL_Callback*)&__ReleaseMethodCallback,                         this);

  VOID (FASTCALL __AAVGraphics_OPENGL::*__ResetMethodCallback)(LPD3DXDEVICE, DWORD *)  = &__AAVGraphics_OPENGL::__Reset;
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_RESET,                  *(__AAVOPENGL_Callback*)&__ResetMethodCallback,                           this);

  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_PRESENT,                (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::Present_Callback,              this);
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_END_SCENE,              (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::EndScene_Callback,             this);
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_CREATE_TEXTURE,         (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::CreateTexture_Callback,        this);
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_SET_TEXTURE,            (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::SetTexture_Callback,           this);
  __AAVOPENGL_RegisterCallback(__AAVOPENGL_CALLBACK_DRAW_INDEXED_PRIMITIVE, (__AAVOPENGL_Callback)&__AAVGraphics_OPENGL::DrawIndexedPrimitive_Callback, this);
#endif


  //Instanciate fonts.
  //m_pkFont = new D3DXFONTCUSTOM(this, "Arial", 84/*72*/, D3DXFONTCUSTOM::BOLD);
  //m_pkFont = new D3DXFONTCUSTOM(this, "Arial", 16/*72*/);
  //m_pkFont = new D3DXFONTCUSTOM(this, "Courier New", 20, __AAVFont_OPENGL::BOLD);
  //m_pkFont = new D3DXFONTCUSTOM(this, "Makisupa", 20, __AAVFont_OPENGL::BOLD);
  //m_pkFont = new D3DXFONTCUSTOM(this, "Novamono", 30, __AAVFont_OPENGL::BOLD);
  //m_pkFont = new D3DXFONTCUSTOM(this, "NORMT___", 18, __AAVFont_OPENGL::BOLD);

  m_pkFont = new D3DXFONTCUSTOM(this, "ProFontWindows", 16, __AAVFont_OPENGL::BOLD);

  //Clear state blocks.
  memset(&m_pkStateBlock[0], 0, sizeof(m_pkStateBlock));
}

__AAVGraphics_OPENGL::~__AAVGraphics_OPENGL()
{
  //Release graphics.
  Release();

  //Delete system fonts.
  if(m_pkFont != NULL) 
    delete m_pkFont;

  //Disconnect from __AAVOPENGL layer. (Will couse "Entry Point Not Found" exception if no __AAVOPENGL.DLL code is loaded.
  __AAVOPENGL_UnregisterCallback(__AAVOPENGL_CALLBACK_RELEASE);
  __AAVOPENGL_UnregisterCallback(__AAVOPENGL_CALLBACK_RESET);
  __AAVOPENGL_UnregisterCallback(__AAVOPENGL_CALLBACK_PRESENT);
  __AAVOPENGL_UnregisterCallback(__AAVOPENGL_CALLBACK_END_SCENE);
  __AAVOPENGL_UnregisterCallback(__AAVOPENGL_CALLBACK_CREATE_TEXTURE);
  __AAVOPENGL_UnregisterCallback(__AAVOPENGL_CALLBACK_SET_TEXTURE);

  //__AAVOPENGL_UnregisterCallback(__AAVOPENGL_CALLBACK_DRAW_INDEXED_PRIMITIVE);
}



// Called from __AAVOPENGL layer as a __AAVD3DDevice9_Release_Callback.
VOID __AAVGraphics_OPENGL::Release()
{
  /////////////////////////////////////////////////////////////////////
  //Release fonts.
  /////////////////////////////////////////////////////////////////////
  if(m_pkFont != NULL) 
    m_pkFont->Release();


  /////////////////////////////////////////////////////////////////////
  //Release __AAVAvionics modules.
  /////////////////////////////////////////////////////////////////////
  m_pkAvionics->Release();

  /////////////////////////////////////////////////////////////////////
  //Release state blocks.
  /////////////////////////////////////////////////////////////////////
  for(UINT i = 0; i < STATE_BLOCK_NUM_STATE_BLOCKS; ++i)
  {
    if(m_pkStateBlock[i] != NULL) m_pkStateBlock[i]->Release(); 
  }
  memset(&m_pkStateBlock[0], 0, sizeof(m_pkStateBlock));

  /////////////////////////////////////////////////////////////////////
  //Release HUD stencil surface.
  /////////////////////////////////////////////////////////////////////
  if(m_pkHUDDepthStencilSurface != NULL)
  {
    m_pkHUDDepthStencilSurface->Release();
    m_pkHUDDepthStencilSurface = NULL;
  }

  /////////////////////////////////////////////////////////////////////
  //Release Default stencil surface.
  /////////////////////////////////////////////////////////////////////
  if(m_pkDefaultDepthStencilSurface != NULL)
  {
    m_pkDefaultDepthStencilSurface->Release();
    m_pkDefaultDepthStencilSurface = NULL;
  }

  m_bReleased = true;
  __AAVDEBUG::OUTPUT("__AAVGraphics_OPENGL::RELEASED");
  return;
}

// Called from __AAVOPENGL layer as a __AAVD3DDevice9_Reset_Callback.
VOID __AAVGraphics_OPENGL::Reset()
{
  m_pkDevice = pkDevice;
  memcpy_s(&m_kPresentationParameters, sizeof(m_kPresentationParameters), pParameters, sizeof(D3DPRESENT_PARAMETERS));

  //m_pkHDDTexture should be released by FSX itself.
  m_pkHDDTexture = NULL;
  //m_pkHUDTexture should be released by FSX itself.
  m_pkHUDTexture = NULL;

  if(m_pkHUD == NULL)
  {
    m_pkHUD = static_cast<HUD *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_HUD));
  }

  if(m_pkHDD == NULL)
  {
    m_pkHDD = static_cast<HDD *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_HDD));
  }

  /////////////////////////////////////////////////////////////////////
  //Reset fonts.
  /////////////////////////////////////////////////////////////////////
  if(m_pkFont != NULL) 
    m_pkFont->Reset(pkDevice, pParameters);


  /////////////////////////////////////////////////////////////////////
  //Reset __AAVAvionics modules.
  /////////////////////////////////////////////////////////////////////
  m_pkAvionics->Reset(pkDevice, pParameters);

  /////////////////////////////////////////////////////////////////////
  // STATE_BLOCK_TRANSPARENT_TEXTURE
  /////////////////////////////////////////////////////////////////////
  m_pkDevice->BeginStateBlock();

  m_pkDevice->SetPixelShader(NULL);
  m_pkDevice->SetVertexShader(NULL);

  m_pkDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );


  //m_pkDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_MATERIAL);
  //m_pkDevice->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
  //m_pkDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE,  D3DMCS_MATERIAL);
  //m_pkDevice->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

  m_pkDevice->SetRenderState( D3DRS_COLORVERTEX,            TRUE);
  //m_pkDevice->SetRenderState( D3DRS_SPECULARENABLE,         FALSE);

  m_pkDevice->SetRenderState( D3DRS_LIGHTING,               FALSE);
  m_pkDevice->SetRenderState( D3DRS_CLIPPING,               TRUE);
  m_pkDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,        FALSE);
  m_pkDevice->SetRenderState( D3DRS_VERTEXBLEND,            D3DVBF_DISABLE );

  m_pkDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,       TRUE );
  m_pkDevice->SetRenderState( D3DRS_SRCBLEND,               D3DBLEND_SRCALPHA );
  m_pkDevice->SetRenderState( D3DRS_DESTBLEND,              D3DBLEND_INVSRCALPHA );
  //m_pkDevice->SetRenderState( D3DRS_SRCBLEND,               D3DBLEND_SRCCOLOR );
  //m_pkDevice->SetRenderState( D3DRS_DESTBLEND,              D3DBLEND_INVSRCCOLOR );

  //m_pkDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );
  m_pkDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
  m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
  m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
  m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

  m_pkDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
  m_pkDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
  m_pkDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

  //m_pkDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
  //m_pkDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
  //m_pkDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
  //m_pkDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
  //m_pkDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
  //m_pkDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
  //m_pkDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
  //m_pkDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
  //m_pkDevice->SetRenderState( D3DRS_VERTEXBLEND,      D3DVBF_DISABLE );
  //m_pkDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
  //m_pkDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
  //m_pkDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED  | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
  //m_pkDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
  //m_pkDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
  //m_pkDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
  m_pkDevice->EndStateBlock(&m_pkStateBlock[STATE_BLOCK_TRANSPARENT_TEXTURE]);

  /////////////////////////////////////////////////////////////////////
  // STATE_BLOCK_NULL_TEXTURE
  /////////////////////////////////////////////////////////////////////
  m_pkDevice->BeginStateBlock();
  m_pkDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,       TRUE );
  m_pkDevice->SetRenderState( D3DRS_SRCBLEND,               D3DBLEND_SRCALPHA );
  m_pkDevice->SetRenderState( D3DRS_DESTBLEND,              D3DBLEND_INVSRCALPHA );

  m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
  m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
  m_pkDevice->EndStateBlock(&m_pkStateBlock[STATE_BLOCK_NULL_TEXTURE]);



  /////////////////////////////////////////////////////////////////////
  // HUD Stencil surface
  /////////////////////////////////////////////////////////////////////
  D3DPRESENT_PARAMETERS* pPresentationParameters = (D3DPRESENT_PARAMETERS *)pParameters;

  m_hResult = pkDevice->CreateDepthStencilSurface(pPresentationParameters->BackBufferWidth, 
                                                  pPresentationParameters->BackBufferHeight, 
                                                  D3DFMT_D24S8,                                    //pPresentationParameters->AutoDepthStencilFormat,
                                                  pPresentationParameters->MultiSampleType,        //D3DMULTISAMPLE_NONE,
                                                  pPresentationParameters->MultiSampleQuality,     //0, 
                                                  TRUE, 
                                                  &m_pkHUDDepthStencilSurface, 
                                                  NULL);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_pkHUDDepthStencilSurface != NULL));

  m_bReleased = false;
  __AAVDEBUG::OUTPUT("__AAVGraphics_OPENGL::RESET");
  return;
}

HRESULT  __AAVGraphics_OPENGL::SetHUDStencilSurface()
{
  HRESULT hResult = S_OK;

  AAV_ASSERT_RETURN(m_pkHUDDepthStencilSurface != NULL, S_FALSE);

  // Save Default Depth-Stencil surface.
  //hResult = m_pkDevice->GetDepthStencilSurface(&m_pkDefaultDepthStencilSurface);
  //AAV_ASSERT_RETURN(SUCCEEDED(hResult), hResult);

  // Save Default Render state.
  m_pkDevice->GetRenderState(D3DRS_STENCILENABLE, &m_dwDepthStencilTestEnable);
  m_pkDevice->GetRenderState(D3DRS_ZENABLE,       &m_dwZEnable);
  m_pkDevice->GetRenderState(D3DRS_ZWRITEENABLE,  &m_dwZWriteEnable);
  m_pkDevice->GetRenderState(D3DRS_ZFUNC,         &m_dwZFunction);

  // Set HUD Stencil surface.
  //hResult = m_pkDevice->SetDepthStencilSurface(m_pkHUDDepthStencilSurface);
  //AAV_ASSERT_RETURN(SUCCEEDED(hResult), hResult);

  m_pkDevice->SetRenderState(D3DRS_STENCILENABLE,    TRUE);
  m_pkDevice->SetRenderState(D3DRS_STENCILMASK,      0xFFFFFFFF);
  m_pkDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);

  return hResult;
}

HRESULT  __AAVGraphics_OPENGL::DisableHUDStencilSurface()
{
  HRESULT hResult = S_OK;

  // Restore Default Depth-Stencil surface.
  //m_pkDevice->SetDepthStencilSurface(m_pkDefaultDepthStencilSurface);
  //AAV_ASSERT_RETURN(SUCCEEDED(hResult), S_FALSE);

  // Decrease reference counter to the default depth stencil surface.
  //if(m_pkDefaultDepthStencilSurface != NULL)
  //{
  //  m_pkDefaultDepthStencilSurface->Release();
  //  m_pkDefaultDepthStencilSurface = NULL;
  //}

  // Restore Default Render state.
  m_pkDevice->SetRenderState(D3DRS_STENCILENABLE, m_dwDepthStencilTestEnable);
  m_pkDevice->SetRenderState(D3DRS_ZENABLE,       m_dwZEnable);
  m_pkDevice->SetRenderState(D3DRS_ZWRITEENABLE,  m_dwZWriteEnable);
  m_pkDevice->SetRenderState(D3DRS_ZFUNC,         m_dwZFunction);

  return hResult;
}

VOID __AAVGraphics_OPENGL::SetGlobalMaterial(D3DMATERIAL9 *pkMaterial)
{
  m_pkDevice->SetMaterial(pkMaterial);
}


VOID __AAVGraphics_OPENGL::SetShaders() 
{
  //m_pkDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,      TRUE );
  //m_pkDevice->SetRenderState( D3DRS_SRCBLEND,              D3DBLEND_SRCALPHA );
  //m_pkDevice->SetRenderState( D3DRS_DESTBLEND,             D3DBLEND_INVSRCALPHA );

  ////m_pkDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,       FALSE );
  //m_pkDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_MATERIAL);
  //m_pkDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
  //m_pkDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,  D3DMCS_MATERIAL);
  //m_pkDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
  //m_pkDevice->SetRenderState(D3DRS_COLORVERTEX,            TRUE);
  //m_pkDevice->SetRenderState(D3DRS_LIGHTING,               FALSE);

  //m_pkDevice->SetTextureStageState(0, D3DTSS_COLOROP,      D3DTOP_DISABLE );
  //m_pkDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,      D3DTOP_DISABLE );

  //m_pkDevice->SetPixelShader( NULL );
  //m_pkDevice->SetVertexShader(NULL);
  //m_pkDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

  //AAV_ASSERT_RETURN_VOID(m_pkStateBlock[STATE_BLOCK_TRANSPARENT_TEXTURE] != NULL);
  //m_pkStateBlock[STATE_BLOCK_TRANSPARENT_TEXTURE]->Apply();

  m_pkDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
}

VOID __AAVGraphics_OPENGL::ResetShaders() 
{
}

VOID __AAVGraphics_OPENGL::SetScissorRect(const RECT* pRect) 
{
  if(pRect != NULL)
  {
    m_hResult = m_pkDevice->SetScissorRect(pRect);
    AAV_ASSERT_RETURN_VOID(m_hResult == S_OK);
    m_pkDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE);
  }
  else
  {
    m_pkDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE);
  }
}

VOID  __AAVGraphics_OPENGL::GetViewMatrix(D3DXMATRIX *pkViewMatrix)
{
  m_pkDevice->GetTransform(D3DTS_VIEW, pkViewMatrix);
}

VOID  __AAVGraphics_OPENGL::SetViewMatrix(D3DXMATRIX *pkViewMatrix)
{
  m_pkDevice->SetTransform(D3DTS_VIEW, pkViewMatrix);
}

VOID  __AAVGraphics_OPENGL::SetViewMatrix(D3DXVECTOR3 *pkEyeVector, D3DXVECTOR3 *pkLookAtVector, D3DXVECTOR3 *pkUpVector)
{
  D3DXMATRIXA16 kViewMatrix;
  D3DXMatrixLookAtLH( &kViewMatrix, pkEyeVector, pkLookAtVector, pkUpVector );
  m_pkDevice->SetTransform(D3DTS_VIEW, &kViewMatrix);
}

VOID __AAVGraphics_OPENGL::SetTransform(D3DXMATRIX *pkTransformMatrix) 
{
  AAV_ASSERT_RETURN_VOID(pkTransformMatrix != NULL);
  m_pkDevice->SetTransform(D3DTS_WORLD, pkTransformMatrix);
}
