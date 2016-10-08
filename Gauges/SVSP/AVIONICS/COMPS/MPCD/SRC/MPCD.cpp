#include "__SVSPAvionics.h"
#include "MPCD.h"

MPCD::MPCD(__SVSPAvionics *__pkAvionics, DWORD dwThreadSleepTime) : 
HDD("MPCD", __pkAvionics, dwThreadSleepTime, NULL, MPCD::GEOM_OBJECT_NUM_OBJECTS, NULL, MPCD::TEXTURE_OBJECT_NUM_OBJECTS)
{
  ///////////////////////////////////////////////////////////////////
  // SETUP DISPLAY ELEMENTS
  ///////////////////////////////////////////////////////////////////
  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;

  ///////////////////////////////////////////////////////////////////
  // SETUP DISPLAY MODULES
  ///////////////////////////////////////////////////////////////////
  SetDisplayModule(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_COM),   AAVVector3<FLOAT>(0, 870, 1), 0);
  SetDisplayModule(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FCR),   AAVVector3<FLOAT>(0, 290, 1), 1);
  SetDisplayModule(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS),   AAVVector3<FLOAT>(0, 290, 1), 2);
  SetDisplayModule(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS),   AAVVector3<FLOAT>(0, 290, 1), 3);
}

MPCD::~MPCD()
{
  __SVSPModule::Stop();
}

VOID MPCD::Render(LPD3DXDEVICE pkDevice)
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
      // Render Base Menu.




      //pkDisplayModule->RenderOnHDD(pkDevice, pkDisplayModule->m_kOnHddOffsetMatrix);
    }
  }

  m_pkTextureRenderer->End(pkDevice);
}

BOOL MPCD::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  AAV_ASSERT_RETURN(dwEventData < __SVSPAvionics::AVIONICS_MODULE_COUNT, false);

  return true;
}
