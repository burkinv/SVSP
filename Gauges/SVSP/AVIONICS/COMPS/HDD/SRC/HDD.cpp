#include "__SVSPAvionics.h"
#include "HDD.h"
#include "AC.h"
#include "__SVSPD3D9.h"

HDD::HDD(CHAR *pcName, __SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, LPPD3DXGEOMETRYOBJECT ppkGeomObjectsList, UINT uiNumGeomObjects, LPPD3DXTEXTUREOBJECT ppkTextureObjectsList, UINT uiNumTextureObjects) : 
__SVSPModule(pcName, pkAvionics, dwThreadSleepTime, FALSE, NULL, ppkGeomObjectsList, uiNumGeomObjects, ppkTextureObjectsList, uiNumTextureObjects),
  m_uiFrameCount(HDD::RENDERING_FRAME_FREQ),
  m_pkTextureRenderer(0),
  m_uiNumGaugeTextures(0)
{
  ///////////////////////////////////////////////////////////////////
  // SETUP TEXTURE RENDERER
  ///////////////////////////////////////////////////////////////////
  m_pkTextureRenderer = new D3DXTEXTURERENDERER(HDD::FRAMEBUFFER_WIDTH, HDD::FRAMEBUFFER_HEIGHT);

  m_apkDisplayModule.clear();
  m_apkDisplayModule.reserve(HDD::MAX_NUM_DISPLAY_MODULES);
}

HDD::~HDD()
{
  //for(UINT i = 0; i < MAX_NUM_DISPLAY_MODULES; ++i)
  //{
  //  if(m_apkDisplayModule[i] != NULL)
  //  {
  //    delete m_apkDisplayModule[i]; 
  //    m_apkDisplayModule[i] = NULL;
  //  }
  //}
}

VOID HDD::Release()
{
  m_uiNumGaugeTextures = 0;

  __SVSPModule::Release();

  AAV_ASSERT_RETURN_VOID(m_pkTextureRenderer != NULL);
  m_pkTextureRenderer->Release();

  //for(UINT i = 0; i < m_apkDisplayModule.size(); ++i)
  //{
  //  if(m_apkDisplayModule[i] != NULL)
  //  {
  //    m_apkDisplayModule[i]->Release();
  //  }
  //}
}

HRESULT HDD::Reset(LPD3DXDEVICE pkDevice)
{
  HRESULT hResult = S_OK;
  hResult = __SVSPModule::Reset(pkDevice);

  AAV_ASSERT_RETURN(m_pkTextureRenderer != NULL, S_FALSE);
  hResult = m_pkTextureRenderer->Reset(pkDevice);
  //for(UINT i = 0; i < m_apkDisplayModule.size(); ++i)
  //{
  //  if(m_apkDisplayModule[i] != NULL)
  //  {
  //    m_apkDisplayModule[i]->Reset();
  //  }
  //}
  return hResult;
}

HRESULT HDD::CreateTexture(LPD3DXDEVICE pkDevice, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPPD3DXTEXTURE ppTexture, HANDLE* pSharedHandle)
{
  HRESULT hResult = S_FALSE;

  if((Width == HDD::HDD_GAUGE_TEXTURE_WIDTH) && (Height == HDD::HDD_GAUGE_TEXTURE_HEIGHT)  && (Format == HDD::HDD_GAUGE_TEXTURE_FORMAT))
  {
    AAV_ASSERT_RETURN((m_uiNumGaugeTextures < HDD::MAX_NUM_GAUGE_TEXTURES), S_FALSE);
    Width                  = HDD::FRAMEBUFFER_WIDTH;
    Height                 = HDD::FRAMEBUFFER_HEIGHT;
    Levels                 = 1;
    Format                 = D3DFMT_X8R8G8B8;

    hResult   = pkDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, &m_apkGaugeTexture[m_uiNumGaugeTextures], pSharedHandle);
    *ppTexture = m_apkGaugeTexture[m_uiNumGaugeTextures];
    m_uiNumGaugeTextures++;
    AAV_ASSERT_RETURN(hResult == S_OK, S_FALSE);
    __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "Created new HDD gauge texture");
  }

  return hResult;
}

HRESULT HDD::SetTexture(LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
  HRESULT hResult = S_FALSE;

  AAV_ASSERT_RETURN(m_pkTextureRenderer != NULL, S_FALSE);

  if((m_apkGaugeTexture[0] != NULL) && (pTexture == m_apkGaugeTexture[0]))
  {
    hResult = pkDevice->SetTexture(Stage, m_pkTextureRenderer->GetDisplayTexture());
  }

  return hResult;
}

HRESULT HDD::EndScene(LPD3DXDEVICE pkDevice)
{
  __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalSetShaders(pkDevice);
  Render(pkDevice);
  __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalResetShaders(pkDevice);

  return S_OK;
}

VOID HDD::Render(LPD3DXDEVICE pkDevice)
{
  if(m_uiFrameCount == 0)
  {
    m_uiFrameCount = HDD::RENDERING_FRAME_FREQ;
  }
  else
  {
    --m_uiFrameCount;
    return;
  }

  __SVSPFont* pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  LPD3DXTEXTUREOBJECT  pkTextureObject = NULL;

  AAV_ASSERT_RETURN_VOID(m_pkTextureRenderer != NULL);

  m_pkTextureRenderer->Begin(pkDevice);

  for(UINT i = 0; i < m_apkDisplayModule.size(); ++i)
  {
    __SVSPModule *pkDisplayModule = m_apkDisplayModule[i];
    if(pkDisplayModule != NULL)
    {
      pkDisplayModule->RenderOnHDD(pkDevice, pkDisplayModule->m_kOnHddOffsetMatrix);
    }
  }


  m_pkTextureRenderer->End(pkDevice);
}

VOID HDD::SwapDisplayModules(__SVSPModule *pkModule1, __SVSPModule *pkModule2)
{
  AAV_ASSERT_RETURN_VOID(pkModule1 != NULL);
  AAV_ASSERT_RETURN_VOID(pkModule1->GetNumDisplayStates() > (__SVSPModule::DISPLAY_STATE_ROOT_MENU + 1));
  AAV_ASSERT_RETURN_VOID(pkModule2 != NULL);
  AAV_ASSERT_RETURN_VOID(pkModule2->GetNumDisplayStates() > (__SVSPModule::DISPLAY_STATE_ROOT_MENU + 1));

  for(UINT uiModule1DisplayIndex = 0; uiModule1DisplayIndex < m_apkDisplayModule.size(); uiModule1DisplayIndex++)
  {
    if(pkModule1 == m_apkDisplayModule[uiModule1DisplayIndex])
    {
      for(UINT uiModule2DisplayIndex = 0; uiModule2DisplayIndex < m_apkDisplayModule.size(); uiModule2DisplayIndex++)
      {
        if(pkModule2 == m_apkDisplayModule[uiModule2DisplayIndex])
        {
          //Swap display modules
          m_apkDisplayModule[uiModule1DisplayIndex] = pkModule2;
          pkModule2->m_eDisplayState = __SVSPModule::DISPLAY_STATE_ROOT_MENU + 1;

          m_apkDisplayModule[uiModule2DisplayIndex] = pkModule1;
          pkModule1->m_eDisplayState = __SVSPModule::DISPLAY_STATE_ROOT_MENU + 1;

          return;
        }
      }

     //pkModule2 is not on the display.
      m_apkDisplayModule[uiModule1DisplayIndex] = pkModule2;
      pkModule2->m_eDisplayState = __SVSPModule::DISPLAY_STATE_ROOT_MENU + 1;
    }
  }
}

VOID HDD::SetDisplayModule(__SVSPModule *pkModule, AAVVector3<FLOAT> &rkDisplayOffsetVector, UINT uiDisplayModuleIndex)
{
  if(pkModule != NULL)
  {
    AAV_ASSERT_RETURN_VOID(m_apkDisplayModule.size() < m_apkDisplayModule.size());
    AAV_ASSERT_RETURN_VOID(pkModule->GetNumDisplayStates() > (__SVSPModule::DISPLAY_STATE_ROOT_MENU + 1));

    pkModule->m_eDisplayState = __SVSPModule::DISPLAY_STATE_ROOT_MENU + 1;
    D3DXMatrixTranslation(&pkModule->m_kOnHddOffsetMatrix, rkDisplayOffsetVector.x, rkDisplayOffsetVector.y, rkDisplayOffsetVector.z);

    m_apkDisplayModule.push_back(pkModule);
  }
}

VOID HDD::GetBackbufferData(struct IMAGE* pImage)
{
  AAV_ASSERT_RETURN_VOID(m_pkTextureRenderer != NULL);

  m_pkTextureRenderer->GetBackbufferData(pImage);
}

BOOL HDD::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  //switch(dwEvent)
  //{
  //case __SVSPModule::EVENT_ID_SET_ROOT_MENU:
  //  m_eDisplayState = DISPLAY_STATE_ROOT_MENU;
  //  break;

  ////case __SVSPModule::EVENT_ID_SET_MODULE:
  ////  {
  ////    __SVSPModule *pkModule = m_pkAvionics->GetModule(dwEventData);
  ////    if((pkHDD != NULL) && (pkModule != NULL))
  ////    {
  ////      pkHDD->SwapDisplayModules(this, pkModule);
  ////    }
  ////  }
  //}
  return false;
}

