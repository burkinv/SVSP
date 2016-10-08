#include "__AAVAvionics.h"
#include "AC.h"
#include "EICAS.h"
#include "ECS.h"
#include "SIM.h"
#include "WPN.h"

FLOAT  EICAS::FUEL_1500  = 1500;
FLOAT  EICAS::FUEL_800   = 800;
FLOAT  EICAS::FUEL_500   = 500;
FLOAT  EICAS::BINGO_FUEL = 300;

__AAVTouchRect EICAS::g_akTouchMapTable[EICAS::DISPLAY_STATE_MAX][__AAVModule::MAX_NUM_TOUCH_RECTS_PER_STATE] = 
{
  /* DISPLAY_STATE_ROOT_MENU */
  {
    __AAVTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3(-2*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __AAVTouchRect("ECS",   EVENT_ID_SET_MODULE, __AAVAvionics::AVIONICS_MODULE_ECS,       D3DXVECTOR3(-1*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_UP_SIDE_BUTTON),
    __AAVTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3(-0*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __AAVTouchRect("FUEL",  EVENT_ID_SET_MODULE, __AAVAvionics::AVIONICS_MODULE_EICAS,     D3DXVECTOR3( 1*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_UP_SIDE_BUTTON),
    __AAVTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3( 2*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),

    __AAVTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  2*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect("FLCS",  EVENT_ID_SET_MODULE, __AAVAvionics::AVIONICS_MODULE_FLCS,      D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  1*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_RT_SIDE_BUTTON),
    __AAVTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  0*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect("SMS",   EVENT_ID_SET_MODULE, __AAVAvionics::AVIONICS_MODULE_SMS,       D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -1*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_RT_SIDE_BUTTON),
    __AAVTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -2*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),

    __AAVTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3( 2*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __AAVTouchRect("SMS",   EVENT_ID_SET_MODULE, __AAVAvionics::AVIONICS_MODULE_FCR,       D3DXVECTOR3( 1*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __AAVTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(-0*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __AAVTouchRect("COP",   EVENT_ID_SET_MODULE, __AAVAvionics::AVIONICS_MODULE_CARRIER,   D3DXVECTOR3(-1*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __AAVTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(-2*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),

    __AAVTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -2*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect("AIP",   EVENT_ID_SET_MODULE, __AAVAvionics::AVIONICS_MODULE_AIP,       D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -1*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_LT_SIDE_BUTTON),
    __AAVTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  0*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect("SIM",   EVENT_ID_SET_MODULE, __AAVAvionics::AVIONICS_MODULE_SIM,       D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  1*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_LT_SIDE_BUTTON),
    __AAVTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  2*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
  },

  /* DISPLAY_STATE_MAIN */
  {
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-2*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-1*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-0*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3( 1*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3( 2*__AAVModule::DISPLAY_WINDOW_WIDTH/7, __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),

    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  2*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  1*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  0*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -1*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __AAVModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -2*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),

    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3( 2*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3( 1*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __AAVTouchRect("MENU",  EVENT_ID_SET_ROOT_MENU,     NULL,                         D3DXVECTOR3(-0*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-1*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-2*__AAVModule::DISPLAY_WINDOW_WIDTH/7, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),

    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -2*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -1*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  0*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  1*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __AAVTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__AAVModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  2*__AAVModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
  },
};

EICAS::EICAS(__AAVAvionics *pkAvionics, DWORD dwThreadSleepTime) : __AAVModule("EICAS", pkAvionics, dwThreadSleepTime, true, EICAS__DispatchProcess, NULL, EICAS::GEOM_OBJECT_NUM_OBJECTS, NULL, EICAS::TEXTURE_OBJECT_NUM_OBJECTS, EICAS::g_akTouchMapTable, EICAS::DISPLAY_STATE_MAX)
{
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, EICAS::DATA_ID_READ_FUEL_DATA, "FUEL TANK CENTER LEVEL",     "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, EICAS::DATA_ID_READ_FUEL_DATA, "FUEL TANK CENTER2 LEVEL",    "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, EICAS::DATA_ID_READ_FUEL_DATA, "FUEL TANK CENTER3 LEVEL",    "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, EICAS::DATA_ID_READ_FUEL_DATA, "FUEL TANK LEFT MAIN LEVEL",  "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, EICAS::DATA_ID_READ_FUEL_DATA, "FUEL TANK RIGHT MAIN LEVEL", "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, EICAS::DATA_ID_READ_FUEL_DATA, "FUEL TOTAL QUANTITY WEIGHT", "Pounds",           SIMCONNECT_DATATYPE_FLOAT32);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __AAVVertex*        pVerts       = NULL;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_BACKGROUND
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_BACKGROUND_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  pVerts[0] = __AAVVertex(-DISPLAY_WINDOW_WIDTH/2,  DISPLAY_WINDOW_WIDTH/2, 0.0f, 0.0f, 0.0f, __AAVColor::dwBACKGROUNDCOLOR);
  pVerts[1] = __AAVVertex( DISPLAY_WINDOW_WIDTH/2,  DISPLAY_WINDOW_WIDTH/2, 0.0f, 1.0f, 0.0f, __AAVColor::dwBACKGROUNDCOLOR);
  pVerts[2] = __AAVVertex(-DISPLAY_WINDOW_WIDTH/2, -DISPLAY_WINDOW_WIDTH/2, 0.0f, 0.0f, 1.0f, __AAVColor::dwBACKGROUNDCOLOR);
  pVerts[3] = __AAVVertex( DISPLAY_WINDOW_WIDTH/2, -DISPLAY_WINDOW_WIDTH/2, 0.0f, 1.0f, 1.0f, __AAVColor::dwBACKGROUNDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_BACKGROUND] = new D3DXTEXTUREOBJECT("Gauges/__AAVAvionics/__AAVAvionics/SRC/COMPS/CONTROL_SYSTEMS/EICAS/RES/TEXTURES/EICAS.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_TOUCH_RECT
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, 4);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __AAVVertex(-kTouchRectArea.tWidth/2,  kTouchRectArea.tHeight/2, 0, 0.0f, 0.0f, __AAVColor::dwGUICOLOR);
  pVerts[1] = __AAVVertex( kTouchRectArea.tWidth/2,  kTouchRectArea.tHeight/2, 0, 1.0f, 0.0f, __AAVColor::dwGUICOLOR);
  pVerts[2] = __AAVVertex(-kTouchRectArea.tWidth/2, -kTouchRectArea.tHeight/2, 0, 0.0f, 1.0f, __AAVColor::dwGUICOLOR);
  pVerts[3] = __AAVVertex( kTouchRectArea.tWidth/2, -kTouchRectArea.tHeight/2, 0, 1.0f, 1.0f, __AAVColor::dwGUICOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_TOUCH_RECT] = pkGeomObject;

  m_ppkTextureObjects[TEXTURE_OBJECT_UP_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/__AAVAvionics/__AAVAvionics/SRC/COMPS/CONTROL_SYSTEMS/COM/RES/TEXTURES/UP_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_LT_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/__AAVAvionics/__AAVAvionics/SRC/COMPS/CONTROL_SYSTEMS/COM/RES/TEXTURES/LT_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_RT_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/__AAVAvionics/__AAVAvionics/SRC/COMPS/CONTROL_SYSTEMS/COM/RES/TEXTURES/RT_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_BM_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/__AAVAvionics/__AAVAvionics/SRC/COMPS/CONTROL_SYSTEMS/COM/RES/TEXTURES/BM_SIDE_BUTTON.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_FRAME_BORDER_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fHalfLineWidth,  __AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[1] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fHalfLineWidth,  __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[2] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fHalfLineWidth,  __AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[3] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fHalfLineWidth,  __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[4] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fHalfLineWidth, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[5] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fHalfLineWidth, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[6] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fHalfLineWidth, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[7] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fHalfLineWidth, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[8] = pVerts[0];
  pVerts[9] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_FRAME_BORDER] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_CORNERS
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_FRAME_CORNERS_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2,                               __AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[ 1] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fCornerWidth,   __AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[ 2] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2,                               __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fCornerWidth, 0, 0, 0, __AAVColor::dwCOLOR2);

  pVerts[ 3] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2,                               __AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[ 4] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2,                               __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fCornerWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[ 5] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fCornerWidth,                __AAVModule::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __AAVColor::dwCOLOR2);

  pVerts[ 6] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2,                              -__AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[ 7] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fCornerWidth,               -__AAVModule::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[ 8] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2,                              -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fCornerWidth, 0, 0, 0, __AAVColor::dwCOLOR2);

  pVerts[ 9] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2,                              -__AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[10] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2,                              -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fCornerWidth, 0, 0, 0, __AAVColor::dwCOLOR2);
  pVerts[11] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fCornerWidth,  -__AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwCOLOR2);
  m_ppkGeomObjects[GEOM_OBJECT_FRAME_CORNERS] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_FRAME_BORDER_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fHalfLineWidth,  __AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[1] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fHalfLineWidth,  __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[2] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fHalfLineWidth,  __AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[3] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fHalfLineWidth,  __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[4] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fHalfLineWidth, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[5] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fHalfLineWidth, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[6] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fHalfLineWidth, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[7] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fHalfLineWidth, -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fHalfLineWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[8] = pVerts[0];
  pVerts[9] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_ACTIVE_FRAME_BORDER] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_CORNERS
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_FRAME_CORNERS_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2,                               __AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 1] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fCornerWidth,   __AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 2] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2,                               __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fCornerWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
                                                                                                                                                         
  pVerts[ 3] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2,                               __AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 4] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2,                               __AAVModule::DISPLAY_WINDOW_HEIGHT/2 - __AAVModule::fCornerWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 5] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fCornerWidth,                __AAVModule::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
                                                                                                                                                         
  pVerts[ 6] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2,                              -__AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 7] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2 - __AAVModule::fCornerWidth,               -__AAVModule::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 8] = __AAVVertex( __AAVModule::DISPLAY_WINDOW_WIDTH/2,                              -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fCornerWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
                                                                                                                                                         
  pVerts[ 9] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2,                              -__AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[10] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2,                              -__AAVModule::DISPLAY_WINDOW_HEIGHT/2 + __AAVModule::fCornerWidth, 0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  pVerts[11] = __AAVVertex(-__AAVModule::DISPLAY_WINDOW_WIDTH/2 + __AAVModule::fCornerWidth,  -__AAVModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __AAVColor::dwACTIVEWINDOWCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_ACTIVE_FRAME_CORNERS] = pkGeomObject;


  m_kPitchTrimValueVector          = D3DXVECTOR3(220, -20, 0);
  m_kFuelTotalQuantityValueVector  = m_kPitchTrimValueVector + D3DXVECTOR3(0, -20, 0);
  m_kEFTValueVector                = m_kFuelTotalQuantityValueVector + D3DXVECTOR3(0, -20, 0);

  m_kC1FuelPercentLeftValueVector = D3DXVECTOR3(   0,   70,  0);
  m_kC2FuelPercentLeftValueVector = D3DXVECTOR3(   0,  -20,  0);
  m_kC3FuelPercentLeftValueVector = D3DXVECTOR3(   0, -100,  0);
  m_kLMFuelPercentLeftValueVector = D3DXVECTOR3(-140,  -50,  0);
  m_kRMFuelPercentLeftValueVector = D3DXVECTOR3( 140,  -50,  0);
}

EICAS::~EICAS()
{
  __AAVModule::Stop();
}

VOID CALLBACK  EICAS__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  EICAS *__THIS = static_cast<EICAS *>(pContext);
  if(!__THIS->m_bAbort)
  {
    __THIS->DispatchProcess(pData, cbData);
  }
}

VOID EICAS::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
{
  if(pData->dwID == SIMCONNECT_RECV_ID_SIMOBJECT_DATA)
  {
    SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

    switch(pObjData->dwRequestID)
    {
    case EICAS::REQUEST_ID_READ_FUEL_DATA:
      {
        EICAS::FUEL_DATA *pkData = (EICAS::FUEL_DATA*)&pObjData->dwData;
        memcpy(&m_kFuelData, pkData, sizeof(EICAS::FUEL_DATA));
      }
      break;
    }
  }
}

VOID EICAS::RenderOnHDD(D3DXMATRIX &rkOPMatrix)
{
  switch(m_eDisplayState)
  {
  case EICAS::DISPLAY_STATE_MAIN:
    {
      AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_AC));
      AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
      ECS *pkECS = static_cast<ECS *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_ECS));
      AAV_ASSERT_RETURN_VOID(pkECS != NULL);
      SIM *pkSIM = static_cast<SIM *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_SIM));

      __AAVFont*      pkFont       = m_pkAvionics->GetFont();
      AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

      CHAR acStr[32];

      LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
      LPD3DXTEXTUREOBJECT  pkTextureObject = NULL;

      //SetTransform(&rkOPMatrix);

      ///////////////////////////////////////////////////////////////////
      // GEOM_OBJECT_BACKGROUND
      ///////////////////////////////////////////////////////////////////
      pkGeomObject    = m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND];
      pkTextureObject = m_ppkTextureObjects[TEXTURE_OBJECT_BACKGROUND];
      if(pkGeomObject != NULL)
      {
        pkGeomObject->SetTransform(&rkOPMatrix);
        pkGeomObject->SetTexture(pkTextureObject);
        pkGeomObject->Render();
      }

      //////////////////////////////////////////////////////////////
      // FUEL
      //////////////////////////////////////////////////////////////
      sprintf_s(&acStr[0], sizeof(acStr), "%u", (UINT)(m_kFuelData.fC1PercentLeft*100));
      pkFont->RenderTextExt(&acStr[0], &m_kC1FuelPercentLeftValueVector, __AAVFont::CENTERED_X|__AAVFont::CENTERED_Y, 1, __AAVColor::dwGUICOLOR);
      sprintf_s(&acStr[0], sizeof(acStr), "%u", (UINT)(m_kFuelData.fC2PercentLeft*100));
      pkFont->RenderTextExt(&acStr[0], &m_kC2FuelPercentLeftValueVector, __AAVFont::CENTERED_X|__AAVFont::CENTERED_Y, 1, __AAVColor::dwGUICOLOR);
      sprintf_s(&acStr[0], sizeof(acStr), "%u", (UINT)(m_kFuelData.fC3PercentLeft*100));
      pkFont->RenderTextExt(&acStr[0], &m_kC3FuelPercentLeftValueVector, __AAVFont::CENTERED_X|__AAVFont::CENTERED_Y, 1, __AAVColor::dwGUICOLOR);
      sprintf_s(&acStr[0], sizeof(acStr), "%u", (UINT)(m_kFuelData.fLMPercentLeft*100));
      pkFont->RenderTextExt(&acStr[0], &m_kLMFuelPercentLeftValueVector, __AAVFont::CENTERED_X|__AAVFont::CENTERED_Y, 1, __AAVColor::dwGUICOLOR);
      sprintf_s(&acStr[0], sizeof(acStr), "%u", (UINT)(m_kFuelData.fRMPercentLeft*100));
      pkFont->RenderTextExt(&acStr[0], &m_kRMFuelPercentLeftValueVector, __AAVFont::CENTERED_X|__AAVFont::CENTERED_Y, 1, __AAVColor::dwGUICOLOR);

      //////////////////////////////////////////////////////////////
      // FUEL TOTAL QUANTITY WEIGHT (KG)
      //////////////////////////////////////////////////////////////
      //RenderElementInt(pkFont, rkOPMatrix, &m_kFuelTotalQuantityValueVector, (INT)(pkAC->m_dFuelTotalQuantityWeightKg), 30000, "TFQ");


#if 0
      //////////////////////////////////////////////////////////////
      // FLAPS
      //////////////////////////////////////////////////////////////
      kGaugeOffsetVector = D3DXVECTOR3(-90.0f, -90.0f, 0);
      RenderElement(rkOPMatrix, &kGaugeOffsetVector, (INT)pkAC->m_uiFlapsLeft,  100, "L.Fp");
      kGaugeOffsetVector = D3DXVECTOR3(-35.0f, -90.0f, 0);
      RenderElement(rkOPMatrix, &kGaugeOffsetVector, (INT)pkAC->m_uiFlapsRight, 100, "R.Fp");
      //////////////////////////////////////////////////////////////
      // ELEVATOR TRIM
      //////////////////////////////////////////////////////////////
      kGaugeOffsetVector = D3DXVECTOR3(-90.0f, -125.0, 0);
      RenderElement(rkOPMatrix, &kGaugeOffsetVector, (INT)pkAC->m_fElevatorTrim, 270, "L.Tr");
      kGaugeOffsetVector = D3DXVECTOR3(-35.0f, -125.0, 0);
      RenderElement(rkOPMatrix, &kGaugeOffsetVector, (INT)pkAC->m_fElevatorTrim, 270, "R.Tr");
#endif 0
    }
    break;

  default:
    break;
  }
    __AAVModule::RenderOnHDD(rkOPMatrix);
}

BOOL EICAS::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  switch(dwEvent)
  {
  case 0:
    break;
  default:
    __AAVModule::EventHandler(dwEvent, dwEventData);
    break;
  }
  return TRUE;
}

VOID EICAS::Start()
{
  m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, EICAS::REQUEST_ID_READ_FUEL_DATA, EICAS::DATA_ID_READ_FUEL_DATA, SIMCONNECT_SIMOBJECT_TYPE_USER, SIMCONNECT_PERIOD_SECOND, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  __AAVModule::Start();
}

VOID EICAS::Process()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  ECS *pkECS = static_cast<ECS *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_ECS));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  WPN *pkWPN = static_cast<WPN *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_WPN));


  // Estimated Bingo Distance
  if(pkWPN != NULL)
  {
    UINT uiNumWaypoints = pkWPN->GetNumWaypoints();
    if(uiNumWaypoints != 0)
    {
      const AAVWAYPOINT<FLOAT>* pkLastWaypoint = pkWPN->GetWaypoint(uiNumWaypoints -1);
    }
  }
}