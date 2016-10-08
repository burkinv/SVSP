#include "__SVSPAvionics.h"
#include "AC.h"
#include "DDI.h"
#include "math.h"
#include "__SVSPGeoCoordinates3.h"
#include "__SVSPVector3.h"
#include "AIP.h"

const __SVSPTouchRect DDI::g_akTouchMapTable[DDI::DISPLAY_STATE_MAX][__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE] = 
{
  /* DISPLAY_STATE_ROOT_MENU */
  {
    __SVSPTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3(-2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect("ECS",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_ECS,       D3DXVECTOR3(-1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, NULL),
    __SVSPTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3(-0*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect("FUEL",  EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_EICAS,     D3DXVECTOR3( 1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, NULL),
    __SVSPTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3( 2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),

    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("FLCS",  EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_FLCS,      D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, NULL),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  0*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("SMS",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_SMS,       D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, NULL),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),

    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3( 2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect("SMS",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_FCR,       D3DXVECTOR3( 1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, NULL),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(-0*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect("COP",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_CARRIER,   D3DXVECTOR3(-1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, NULL),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(-2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),

    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("AIP",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_AIP,       D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, NULL),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  0*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("SIM",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_SIM,       D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, NULL),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
  },
};


DDI::DDI(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("DDI", pkAvionics, dwThreadSleepTime, true, DDI__DispatchProcess, NULL, DDI::GEOM_OBJECT_NUM_OBJECTS, NULL, DDI::TEXTURE_OBJECT_NUM_OBJECTS, DDI::g_akTouchMapTable, DDI::DISPLAY_STATE_MAX)
m_pkModule(NULL),
m_eDisplayState(DDI::DISPLAY_STATE_ROOT_MENU)
{
  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;

}

DDI::~DDI()
{
  __SVSPModule::Stop();
}

VOID DDI::RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  LPD3DXGEOMETRYOBJECT pkGeomObjectActive = NULL;

  if(m_pkModule != NULL)
  {
    m_pkModule->RenderOnHDD(pkDevice, rkOPMatrix);
  }
  else
  {
    ///////////////////////////////////////////////////////////////////
    // GEOM_OBJECT_BACKGROUND
    ///////////////////////////////////////////////////////////////////
    pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND];
    if(pkGeomObject != NULL)
    {
      pkGeomObject->SetTransform(pkDevice, &rkOPMatrix);
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->Render(pkDevice);
    }

    ///////////////////////////////////////////////////////////////////
    // GEOM_OBJECT_BOUNDARY_FRAME
    ///////////////////////////////////////////////////////////////////
    pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_BOUNDARY_FRAME];
    if(pkGeomObject != NULL)
    {
      pkGeomObject->SetTransform(pkDevice, &rkOPMatrix);
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->Render(pkDevice);
    }

    __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
  }
}

BOOL DDI::MouseHandler(PPIXPOINT pMousePoint, FLAGS32 mouse_flags) 
{
  if(m_pkModule != NULL)
  {
    return m_pkModule->MouseHandler(pMousePoint, mouse_flags);
  }
  else
  {
    return __SVSPModule::MouseHandler(pMousePoint, mouse_flags);
  }
}

BOOL DDI::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  AAV_ASSERT_RETURN(dwEventData < __SVSPAvionics::AVIONICS_MODULE_COUNT, false);
  m_pkModule = m_pkAvionics->GetModule(dwEventData);

  return true;
}

VOID CALLBACK DDI__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  DDI *__THIS = static_cast<DDI *>(pContext);
  if(!__THIS->m_bAbort)
  {
    __THIS->DispatchProcess(pData, cbData);
  }
}

VOID DDI::Process()
{
}
