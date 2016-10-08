#include "__SVSPAvionics.h"
#include "ECS.h"
#include "AC.h"
#include "SIM.h"

const FLOAT ECS::THRUST_VECTOR_VALUE_LEVEL_OFF    = 30;
const FLOAT ECS::THRUST_VECTOR_VALUE_PITCH_UP     = 0;
const FLOAT ECS::THRUST_VECTOR_VALUE_PITCH_DOWN   = 60;
const FLOAT ECS::THRUST_VECTOR_VALUE_FULL_DOWN    = 100;
const FLOAT ECS::FUEL_1500                        = 1500;
const FLOAT ECS::FUEL_800                         = 800;
const FLOAT ECS::FUEL_500                         = 500;
const FLOAT ECS::BINGO_FUEL                       = 300;

__SVSPTouchRect ECS::g_akTouchMapTable[ECS::DISPLAY_STATE_MAX][__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE] = 
{
  /* DISPLAY_STATE_ROOT_MENU */
  {
    __SVSPTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3(-2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect("ECS",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_ECS,       D3DXVECTOR3(-1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_UP_SIDE_BUTTON),
    __SVSPTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3(-0*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect("FUEL",  EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_EICAS,     D3DXVECTOR3( 1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_UP_SIDE_BUTTON),
    __SVSPTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3( 2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),

    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("FLCS",  EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_FLCS,      D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_RT_SIDE_BUTTON),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  0*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("SMS",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_SMS,       D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_RT_SIDE_BUTTON),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),

    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3( 2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect("SMS",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_FCR,       D3DXVECTOR3( 1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(-0*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect("COP",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_CARRIER,   D3DXVECTOR3(-1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(-2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),

    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("AIP",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_AIP,       D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_LT_SIDE_BUTTON),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  0*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("SIM",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_SIM,       D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_LT_SIDE_BUTTON),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
  },

  /* DISPLAY_STATE_MAIN */
  {
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(-2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(-1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(-0*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3( 1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3( 2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),

    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  0*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),

    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3( 2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3( 1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect("MENU",  EVENT_ID_SET_ROOT_MENU,     NULL, D3DXVECTOR3(-0*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(-1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(-2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),

    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  0*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
  },
};

///////////////////////////////////////////////////////
// ENGINE CONTROL SYSTEM (ECS)
///////////////////////////////////////////////////////
ECS::ECS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("ECS", pkAvionics, dwThreadSleepTime, true, ECS__DispatchProcess, NULL, ECS::GEOM_OBJECT_NUM_OBJECTS, NULL, ECS::TEXTURE_OBJECT_NUM_OBJECTS, ECS::g_akTouchMapTable, ECS::DISPLAY_STATE_MAX)
{
  memset(&m_akEngine[0], 0, sizeof(m_akEngine));
  memset(&m_kData,       0, sizeof(m_kData));

  m_dLiftFanValueToGain           = 0;

  ///////////////////////////////////////////////////////
  // READ DATA SECTION
  ///////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_FUEL_DATA, "FUEL TANK CENTER LEVEL",     "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_FUEL_DATA, "FUEL TANK CENTER2 LEVEL",    "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_FUEL_DATA, "FUEL TANK CENTER3 LEVEL",    "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_FUEL_DATA, "FUEL TANK LEFT MAIN LEVEL",  "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_FUEL_DATA, "FUEL TANK RIGHT MAIN LEVEL", "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_FUEL_DATA, "FUEL TOTAL QUANTITY WEIGHT", "Pounds",           SIMCONNECT_DATATYPE_FLOAT32);

  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENGINE TYPE",                            "Enum");
  //ENGINE[0]
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "GENERAL ENG THROTTLE LEVER POSITION:1",  "Percent",                 SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "RECIP ENG TURBINE INLET TEMPERATURE:1",  "Celsius",                 SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG N1:1",                          "Percent",                 SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG N2:1",                          "Percent",                 SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG ITT:1",                         "Rankine",                 SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG EXHAUST GAS TEMPERATURE:1",          "Rankine",                 SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG AFTERBURNER STAGE ACTIVE:1",    "Number",                  SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG JET THRUST:1",                  "Pounds",                  SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG PRIMARY NOZZLE PERCENT:1",      "Percent",                 SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG FUEL FLOW PPH:1",                    "Pounds per hour",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG OIL PRESSURE:1",                     "Pounds per square foot",  SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG OIL TEMPERATURE:1",                  "Rankine",                 SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG HYDRAULIC PRESSURE:1",               "Pounds per square foot",  SIMCONNECT_DATATYPE_FLOAT32);
  //ENGINE[1]                                                                                                                                  
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "GENERAL ENG THROTTLE LEVER POSITION:2",  "Percent",                 SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "RECIP ENG TURBINE INLET TEMPERATURE:2",  "Celsius",                 SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG N1:2",                          "Percent",                 SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG N2:2",                          "Percent",                 SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG ITT:2",                         "Rankine",                 SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG EXHAUST GAS TEMPERATURE:2",          "Rankine",                 SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG AFTERBURNER STAGE ACTIVE:2",    "Number",                  SIMCONNECT_DATATYPE_INT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG JET THRUST:2",                  "Pounds",                  SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "TURB ENG PRIMARY NOZZLE PERCENT:2",      "Percent",                 SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG FUEL FLOW PPH:2",                    "Pounds per hour",         SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG OIL PRESSURE:2",                     "Pounds per square foot",  SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG OIL TEMPERATURE:2",                  "Rankine",                 SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, ECS::DATA_ID_READ_ENGINE_DATA, "ENG HYDRAULIC PRESSURE:2",               "Pounds per square foot",  SIMCONNECT_DATATYPE_FLOAT32);


  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;
  AAVArea<FLOAT>   kGaugeCellArea  = AAVArea<FLOAT>(64, 64);

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_BACKGROUND
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_BACKGROUND_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  pVerts[0] = __SVSPVertex(-DISPLAY_WINDOW_WIDTH/2,  DISPLAY_WINDOW_WIDTH/2, 0.0f, 0.0f, 0.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[1] = __SVSPVertex( DISPLAY_WINDOW_WIDTH/2,  DISPLAY_WINDOW_WIDTH/2, 0.0f, 1.0f, 0.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[2] = __SVSPVertex(-DISPLAY_WINDOW_WIDTH/2, -DISPLAY_WINDOW_WIDTH/2, 0.0f, 0.0f, 1.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[3] = __SVSPVertex( DISPLAY_WINDOW_WIDTH/2, -DISPLAY_WINDOW_WIDTH/2, 0.0f, 1.0f, 1.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_BACKGROUND] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/ECS/RES/TEXTURES/ECS.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_GAUGE_CELL
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_GAUGE_CELL_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-kGaugeCellArea.tWidth/2,  kGaugeCellArea.tHeight/2, 0, 0.0f, 0.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[1] = __SVSPVertex( kGaugeCellArea.tWidth/2,  kGaugeCellArea.tHeight/2, 0, 1.0f, 0.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[2] = __SVSPVertex(-kGaugeCellArea.tWidth/2, -kGaugeCellArea.tHeight/2, 0, 0.0f, 1.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[3] = __SVSPVertex( kGaugeCellArea.tWidth/2, -kGaugeCellArea.tHeight/2, 0, 1.0f, 1.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_GAUGE_CELL] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_CELL_MAX]     = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/EICAS/RES/TEXTURES/GAUGE_MAX.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_CELL_MIN_MAX] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/EICAS/RES/TEXTURES/GAUGE_MIN_MAX.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_GAUGE_NEEDLE
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_GAUGE_NEEDLE_VERTICES);
  //pkGeomObject->SetMaterial(1.0f, 0.3f, 0.0f, 1.0f);
  pkGeomObject->SetMaterial(0.3f, 1.0f, 0.3f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-kGaugeCellArea.tWidth/2,  kGaugeCellArea.tHeight/2, 0, 0.0f, 0.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[1] = __SVSPVertex( kGaugeCellArea.tWidth/2,  kGaugeCellArea.tHeight/2, 0, 1.0f, 0.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[2] = __SVSPVertex(-kGaugeCellArea.tWidth/2, -kGaugeCellArea.tHeight/2, 0, 0.0f, 1.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[3] = __SVSPVertex( kGaugeCellArea.tWidth/2, -kGaugeCellArea.tHeight/2, 0, 1.0f, 1.0f, __SVSPColor::dwACTIVEWINDOWCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_GAUGE_NEEDLE] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_NEEDLE] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/EICAS/RES/TEXTURES/NEEDLE.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_TOUCH_RECT
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, 4);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-kTouchRectArea.tWidth/2,  kTouchRectArea.tHeight/2, 0, 0.0f, 0.0f, __SVSPColor::dwGUICOLOR);
  pVerts[1] = __SVSPVertex( kTouchRectArea.tWidth/2,  kTouchRectArea.tHeight/2, 0, 1.0f, 0.0f, __SVSPColor::dwGUICOLOR);
  pVerts[2] = __SVSPVertex(-kTouchRectArea.tWidth/2, -kTouchRectArea.tHeight/2, 0, 0.0f, 1.0f, __SVSPColor::dwGUICOLOR);
  pVerts[3] = __SVSPVertex( kTouchRectArea.tWidth/2, -kTouchRectArea.tHeight/2, 0, 1.0f, 1.0f, __SVSPColor::dwGUICOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_TOUCH_RECT] = pkGeomObject;

  m_ppkTextureObjects[TEXTURE_OBJECT_UP_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/UP_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_LT_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/LT_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_RT_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/RT_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_BM_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/BM_SIDE_BUTTON.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_FRAME_BORDER_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[1] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[2] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[3] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[4] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[5] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[6] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[7] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[8] = pVerts[0];
  pVerts[9] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_FRAME_BORDER] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_CORNERS
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_FRAME_CORNERS_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2,                               __SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 1] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,   __SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 2] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2,                               __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);

  pVerts[ 3] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2,                               __SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 4] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2,                               __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 5] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,                __SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __SVSPColor::dwCOLOR2);

  pVerts[ 6] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2,                              -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 7] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,               -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 8] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2,                              -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);

  pVerts[ 9] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2,                              -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[10] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2,                              -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[11] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,  -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  m_ppkGeomObjects[GEOM_OBJECT_FRAME_CORNERS] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_FRAME_BORDER_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[1] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[2] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[3] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[4] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[5] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[6] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[7] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[8] = pVerts[0];
  pVerts[9] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_ACTIVE_FRAME_BORDER] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_CORNERS
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_FRAME_CORNERS_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2,                               __SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 1] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,   __SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 2] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2,                               __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
                                                                                                                                                         
  pVerts[ 3] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2,                               __SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 4] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2,                               __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 5] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,                __SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
                                                                                                                                                         
  pVerts[ 6] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2,                              -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 7] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,               -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 8] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH/2,                              -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
                                                                                                                                                         
  pVerts[ 9] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2,                              -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[10] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2,                              -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[11] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,  -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_ACTIVE_FRAME_CORNERS] = pkGeomObject;


  ///////////////////////////////////////////////////////////////////
  // GAUGE TABLE VECTORS
  ///////////////////////////////////////////////////////////////////
  m_kInletTemperatureValueVector   = D3DXVECTOR3( 95, 205, 0);
  m_kThrustPercentValueVector      = D3DXVECTOR3(-115, 275, 0);
  m_kN1PercentValueVector          = m_kThrustPercentValueVector + D3DXVECTOR3(1.5f*kGaugeCellArea.tWidth,  0,                                 0);
  m_kN2PercentValueVector          = m_kN1PercentValueVector     + D3DXVECTOR3(1.5f*kGaugeCellArea.tWidth,  0,                                 0);
  m_kEGTValueVector                = m_kN2PercentValueVector     + D3DXVECTOR3(1.5f*kGaugeCellArea.tWidth,  0,                                 0);

  m_kNozzleValueVector             = m_kN2PercentValueVector     + D3DXVECTOR3(0,                          -1.5f*kGaugeCellArea.tHeight - 15, 0);
  m_kITTValueVector                = m_kEGTValueVector           + D3DXVECTOR3(0,                          -1.5f*kGaugeCellArea.tHeight - 15, 0);

  m_kEFFValueVector                = m_kThrustPercentValueVector + D3DXVECTOR3(0, -20, 0);

  m_kT2WRatioValueVector           = m_kEGTValueVector           + D3DXVECTOR3(0, -20, 0);

  m_kFFValueVector                 = D3DXVECTOR3(-80, 200, 0);

  m_kC1FuelPercentLeftValueVector  = D3DXVECTOR3(   0,   40,  0);
  m_kC2FuelPercentLeftValueVector  = D3DXVECTOR3(   0,  -60,  0);
  m_kC3FuelPercentLeftValueVector  = D3DXVECTOR3(   0, -180,  0);
  m_kLMFuelPercentLeftValueVector  = D3DXVECTOR3(-110,  -50,  0);
  m_kRMFuelPercentLeftValueVector  = D3DXVECTOR3( 110,  -50,  0);
  m_kEFTValueVector                = D3DXVECTOR3(-210, -260,  0);

}

ECS::~ECS()
{
  __SVSPModule::Stop();
}

VOID CALLBACK  ECS__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  ECS *__THIS = static_cast<ECS *>(pContext);
  if(!__THIS->m_bAbort)
  {
    __THIS->DispatchProcess(pData, cbData);
  }
}

VOID ECS::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
{
  if(pData->dwID == SIMCONNECT_RECV_ID_SIMOBJECT_DATA)
  {
    SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
    switch(pObjData->dwRequestID)
    {
    case ECS::REQUEST_ID_READ_ENGINE_DATA:
      {
        ECS::READ_OBJECT_STATE_DATA *pkData = (ECS::READ_OBJECT_STATE_DATA*)&pObjData->dwData;
        memcpy(&m_kData, pkData, sizeof(ECS::READ_OBJECT_STATE_DATA));
        m_dwProcessUpdateMask |= ECS::PROCESS_UPDATE_AIRCRAFT_ENGINE_DATA;
      }
      break;
    case ECS::REQUEST_ID_READ_FUEL_DATA:
      {
        ECS::FUEL_DATA *pkData = (ECS::FUEL_DATA*)&pObjData->dwData;
        memcpy(&m_kFuelData, pkData, sizeof(ECS::FUEL_DATA));
        m_dwProcessUpdateMask |= ECS::PROCESS_UPDATE_AIRCRAFT_FUEL_DATA;
      }
      break;
    }
  }
}


VOID ECS::RenderGaugeElement(LPD3DXDEVICE pkDevice, ECS::GAUGE_ELEMENT eGaugeElement, D3DXMATRIX &rkOPMatrix, D3DXVECTOR3 *pkGaugeTranslationVector, FLOAT dScaling)
{
  LPD3DXTEXTUREOBJECT pkGaugeCellTextureObject = NULL;
  LPD3DXTEXTUREOBJECT pkGaugeNeedleTextureObject     = m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_NEEDLE];

  __SVSPFont*      pkFont       = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  switch(eGaugeElement)
  {
  case ECS::GAUGE_ELEMENT_THRUST:
    pkGaugeCellTextureObject = m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_CELL_MAX];
    AAV_ASSERT_RETURN_VOID(pkFont  != NULL);
    RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellTextureObject, pkGaugeNeedleTextureObject, rkOPMatrix, pkGaugeTranslationVector, m_akEngine[0].dThrustPercent,    100,  "THRUST");
    break;
  case ECS::GAUGE_ELEMENT_N1_RPM:
    pkGaugeCellTextureObject = m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_CELL_MIN_MAX];
    AAV_ASSERT_RETURN_VOID(pkFont  != NULL);
    RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellTextureObject, pkGaugeNeedleTextureObject, rkOPMatrix, pkGaugeTranslationVector, m_akEngine[0].fN[0],             100,  "N1 RPM");
    break;
  case ECS::GAUGE_ELEMENT_N2_RPM:
    pkGaugeCellTextureObject = m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_CELL_MIN_MAX];
    AAV_ASSERT_RETURN_VOID(pkFont  != NULL);
    RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellTextureObject, pkGaugeNeedleTextureObject, rkOPMatrix, pkGaugeTranslationVector, m_akEngine[0].fN[1],             100,  "N2 RPM");
    break;
  case ECS::GAUGE_ELEMENT_EGT_RPM:
    pkGaugeCellTextureObject = m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_CELL_MAX];
    AAV_ASSERT_RETURN_VOID(pkFont  != NULL);
    RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellTextureObject, pkGaugeNeedleTextureObject, rkOPMatrix, pkGaugeTranslationVector, m_akEngine[0].dEGT,              1200, "EGT");
    break;
  }
}

VOID ECS::RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  switch(m_eDisplayState)
  {
  case ECS::DISPLAY_STATE_MAIN:
    {
      AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
      AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
      SIM *pkSIM = static_cast<SIM *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SIM));

      __SVSPFont*      pkFont       = m_pkAvionics->GetFont();
      AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

      CHAR acStr[32];

      LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
      LPD3DXTEXTUREOBJECT  pkTextureObject = NULL;

      ///////////////////////////////////////////////////////////////////
      // GEOM_OBJECT_BACKGROUND
      ///////////////////////////////////////////////////////////////////
      pkGeomObject    = m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND];
      AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
      pkTextureObject = m_ppkTextureObjects[TEXTURE_OBJECT_BACKGROUND];
      pkGeomObject->SetTransform(pkDevice, &rkOPMatrix);
      pkGeomObject->SetTexture(pkDevice, pkTextureObject);
      pkGeomObject->Render(pkDevice);

      if((m_ppkGeomObjects[GEOM_OBJECT_GAUGE_CELL] != NULL) && (m_ppkGeomObjects[GEOM_OBJECT_GAUGE_NEEDLE] != NULL))
      {
        D3DXVECTOR3   kGaugeOffsetVector;
        //rkOPMatrix = m_kOPMatrix*pkAC->m_kLocalToBodyTransformMatrix;

        LPD3DXTEXTUREOBJECT pkGaugeCellMinMaxTextureObject = m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_CELL_MIN_MAX];
        LPD3DXTEXTUREOBJECT pkGaugeCellMaxTextureObject    = m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_CELL_MAX];
        LPD3DXTEXTUREOBJECT pkGaugeNeedleTextureObject     = m_ppkTextureObjects[TEXTURE_OBJECT_GAUGE_NEEDLE];

        // ENGINE DATA
        RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellMaxTextureObject,    pkGaugeNeedleTextureObject, rkOPMatrix, &m_kThrustPercentValueVector, m_akEngine[0].dThrustPercent,    100,  "THRUST");
        RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellMinMaxTextureObject, pkGaugeNeedleTextureObject, rkOPMatrix, &m_kN1PercentValueVector,     m_akEngine[0].fN[0],             100,  "N1 RPM");
        RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellMinMaxTextureObject, pkGaugeNeedleTextureObject, rkOPMatrix, &m_kN2PercentValueVector,     m_akEngine[0].fN[1],             100,  "N2 RPM");
        RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellMaxTextureObject,    pkGaugeNeedleTextureObject, rkOPMatrix, &m_kEGTValueVector,           m_akEngine[0].dEGT,              1200, "EGT");

        RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellMaxTextureObject,    pkGaugeNeedleTextureObject, rkOPMatrix, &m_kNozzleValueVector,        m_akEngine[0].dNozzle,           100,  "NOZZLE");
        RenderGaugeElement<FLOAT>(pkDevice, pkFont, pkGaugeCellMaxTextureObject,    pkGaugeNeedleTextureObject, rkOPMatrix, &m_kITTValueVector,           m_akEngine[0].dITT,              1700, "ITT");

        pkGeomObject->SetTransform(pkDevice, &rkOPMatrix);
        //FF
        sprintf_s(&acStr[0], sizeof(acStr), "%u", (UINT)(m_akEngine[0].dFF));
        pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kFFValueVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwACTIVEWINDOWCOLOR);
      }

      //////////////////////////////////////////////////////////////
      // FUEL
      //////////////////////////////////////////////////////////////
      UINT uiC1PercentLeft = (UINT)(m_kFuelData.fC1PercentLeft*100);
      sprintf_s(&acStr[0], sizeof(acStr), "%u   %u", uiC1PercentLeft, uiC1PercentLeft);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kC1FuelPercentLeftValueVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 1, __SVSPColor::dwACTIVEWINDOWCOLOR);

      UINT uiC2PercentLeft = (UINT)(m_kFuelData.fC2PercentLeft*100);
      sprintf_s(&acStr[0], sizeof(acStr), "%u   %u", uiC2PercentLeft, uiC2PercentLeft);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kC2FuelPercentLeftValueVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 1, __SVSPColor::dwACTIVEWINDOWCOLOR);

      UINT uiC3PercentLeft = (UINT)(m_kFuelData.fC3PercentLeft*100);
      sprintf_s(&acStr[0], sizeof(acStr), "%u   %u", uiC3PercentLeft, uiC3PercentLeft);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kC3FuelPercentLeftValueVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 1, __SVSPColor::dwACTIVEWINDOWCOLOR);

      sprintf_s(&acStr[0], sizeof(acStr), "%u", (UINT)(m_kFuelData.fLMPercentLeft*100));
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kLMFuelPercentLeftValueVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 1, __SVSPColor::dwACTIVEWINDOWCOLOR);

      sprintf_s(&acStr[0], sizeof(acStr), "%u", (UINT)(m_kFuelData.fRMPercentLeft*100));
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kRMFuelPercentLeftValueVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 1, __SVSPColor::dwACTIVEWINDOWCOLOR);


      if(m_akEngine[0].dFF > 0)
      {
        if(m_dEstimatedFlightTime > 60)
        {
          FLOAT m_dEstimatedFlightTimeHours = m_dEstimatedFlightTime/60;
          sprintf_s(&acStr[0], sizeof(acStr), "EFT: %#.1f h", m_dEstimatedFlightTimeHours);
        }
        else
        {
          sprintf_s(&acStr[0], sizeof(acStr), "EFT: %#d min", (INT)m_dEstimatedFlightTime);
        }
        pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kEFTValueVector, __SVSPFont::ALIGN_LEFT|__SVSPFont::CENTERED_Y, 1, __SVSPColor::dwACTIVEWINDOWCOLOR);
      }
    }
    break;

  default:
    break;
  }
    __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}

BOOL ECS::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  __SVSPModule::EventHandler(dwEvent, dwEventData);
  return TRUE;
}

VOID ECS::SetGainThrustVectorValue(FLOAT dThrustVectorValueToGain)
{
  AAV_ASSERT_RETURN_VOID((dThrustVectorValueToGain >= ECS::THRUST_VECTOR_VALUE_PITCH_UP) && (dThrustVectorValueToGain <= ECS::THRUST_VECTOR_VALUE_FULL_DOWN));
  m_dThrustVectorValueToGain = dThrustVectorValueToGain;
}

VOID ECS::SetGainLiftFanValue(FLOAT dLiftFanValueToGain)
{
  AAV_ASSERT_RETURN_VOID((dLiftFanValueToGain >= 0) && (dLiftFanValueToGain <= 100));
  m_dLiftFanValueToGain = dLiftFanValueToGain;
}


VOID ECS::Start()
{
  m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, DATA_ID_READ_FUEL_DATA, REQUEST_ID_READ_FUEL_DATA, SIMCONNECT_SIMOBJECT_TYPE_USER, SIMCONNECT_PERIOD_SIM_FRAME);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, REQUEST_ID_READ_ENGINE_DATA, DATA_ID_READ_ENGINE_DATA, SIMCONNECT_SIMOBJECT_TYPE_USER, SIMCONNECT_PERIOD_SIM_FRAME);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  m_dThrustVectorValueToGain = ECS::THRUST_VECTOR_VALUE_LEVEL_OFF;
  m_dThrustVectorValue       = ECS::THRUST_VECTOR_VALUE_PITCH_DOWN;
  m_dLiftFanValueToGain      = 0;
  m_dLiftFanValue            = 10;
  m_kTorqueControlVector     = AAVVector3<FLOAT>(0, 0, 0);
  m_dAugmentedThrust         = 0;
  m_fFuelTotalQuantityWeightKg = 0;
  m_dEstimatedFlightTime     = 0;

  __SVSPModule::Start();
}

VOID ECS::Process()
{
  AC  *pkAC      = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  ///////////////////////////////////////////////////////
  // PROCESS_UPDATE_AIRCRAFT_ENGINE_DATA
  ///////////////////////////////////////////////////////
  if((m_dwProcessUpdateMask & ECS::PROCESS_UPDATE_AIRCRAFT_ENGINE_DATA) == ECS::PROCESS_UPDATE_AIRCRAFT_ENGINE_DATA)
  {
    for(int i = 0; i < ECS::NUM_ENGINES; ++i)
    {
      ENGINE            *pkEngine     = &m_akEngine[i];
      ECS::GEN_ENGINE_DATA *pEngineData  = &m_kData.akEngineData[i];

      pkEngine->dThrottle                = pEngineData->dThrottle;
      pkEngine->dTIT                     = pEngineData->dTIT;
      pkEngine->fN[0]                    = (FLOAT)pEngineData->dN[0];
      pkEngine->fN[1]                    = (FLOAT)pEngineData->dN[1];
      pkEngine->dITT                     = (pEngineData->dITT - 491.67f)/1.8f;    //Degrees Celsius
      pkEngine->dEGT                     = (pEngineData->dEGT - 491.67f)/1.8f;    //Degrees Celsius
      pkEngine->dwAB                     = pEngineData->dwAB;
      pkEngine->dNozzle                  = pEngineData->dNozzle;
      pkEngine->dFF                      = (pEngineData->dFuelFlowPPH*0.4536f)/60; //Kg/Min
      pkEngine->dOilPressure             = pEngineData->dOilPressure;
      pkEngine->dOilTemp                 = (pEngineData->dOilTemp - 491.67f)/1.8f; //Degrees Celsius
      pkEngine->dHydPressure             = pEngineData->dHydPressure;
      pkEngine->dCurrentThrustCapacity   = pEngineData->dThrust;
      pkEngine->dThrust                  = pkEngine->dCurrentThrustCapacity + m_dAugmentedThrust;
      pkEngine->dThrustPercent           = 100.0f*pkEngine->dThrust/ECS::MAX_THRUST;
      pkEngine->dThrustVectorAngle       = (D3DX_PI/2)*(m_dThrustVectorValue - ECS::THRUST_VECTOR_VALUE_LEVEL_OFF)/(ECS::THRUST_VECTOR_VALUE_FULL_DOWN - ECS::THRUST_VECTOR_VALUE_LEVEL_OFF);


      if(m_dLiftFanValue > 90.0f)
      {
        pkEngine->kBodyThrust              = AAVVector3<FLOAT>(/*Lateral     */0, 
                                                                /*Vertical    */pkEngine->dThrust*sin(pkEngine->dThrustVectorAngle), 
                                                                /*Longitudinal*/pkEngine->dThrust*cos(pkEngine->dThrustVectorAngle));

        pkEngine->kBodyTorque              = AAVVector3<FLOAT>(/*Pitch*/       pkEngine->dThrust*m_kTorqueControlVector.x, 
                                                                /*Yaw  */       pkEngine->dThrust*m_kTorqueControlVector.y, 
                                                                /*Roll */       pkEngine->dThrust*m_kTorqueControlVector.z);
      }
      else
      {
        pkEngine->kBodyThrust              = AAVVector3<FLOAT>(/*Lateral     */0, 
                                                                /*Vertical    */0, 
                                                                /*Longitudinal*/pkEngine->dThrust*cos(pkEngine->dThrustVectorAngle));

        pkEngine->kBodyTorque              = AAVVector3<FLOAT>(/*Pitch*/       pkEngine->dThrust*sin(pkEngine->dThrustVectorAngle)*5.0f, 
                                                                /*Yaw  */       0, 
                                                                /*Roll */       0);
      }
    }

    m_dwProcessUpdateMask &= ~ECS::PROCESS_UPDATE_AIRCRAFT_ENGINE_DATA;
  }

  ///////////////////////////////////////////////////////
  // PROCESS_UPDATE_AIRCRAFT_FUEL_DATA
  ///////////////////////////////////////////////////////
  if((m_dwProcessUpdateMask & ECS::PROCESS_UPDATE_AIRCRAFT_FUEL_DATA) == ECS::PROCESS_UPDATE_AIRCRAFT_FUEL_DATA)
  {
    // Total Fuel Quantity (Kg)
    m_fFuelTotalQuantityWeightKg = m_kFuelData.fFuelTotalQuantityWeight*0.453f;

    m_dwProcessUpdateMask &= ~ECS::PROCESS_UPDATE_AIRCRAFT_FUEL_DATA;
  }

  /////////////////////////////////////////////////////////
  //LIFT FAN CONTROL
  /////////////////////////////////////////////////////////
  FLOAT dLiftFanValueError = m_dLiftFanValue - m_dLiftFanValueToGain;
  if(abs(dLiftFanValueError) > 0.01)
  {
    m_dLiftFanValue -= 0.75f*(dLiftFanValueError/sqrt(1 + dLiftFanValueError*dLiftFanValueError));
    ID kID = check_named_variable("lift_fan_door");
    set_named_variable_value( kID, m_dLiftFanValue);
  }

  /////////////////////////////////////////////////////////
  //THRUST VECTOR CONTROL
  /////////////////////////////////////////////////////////
  m_dThrustVectorRotationVelocity = (m_dThrustVectorValue < THRUST_VECTOR_VALUE_PITCH_DOWN) ? 0.95f : 0.25f;

  FLOAT dThrustVectorValueError = m_dThrustVectorValue - m_dThrustVectorValueToGain;
  if(abs(dThrustVectorValueError) > 0.01)
  {
    m_dThrustVectorValue -= m_dThrustVectorRotationVelocity*(dThrustVectorValueError/sqrt(1 + dThrustVectorValueError*dThrustVectorValueError));
    ID kID = check_named_variable("swivel_nozzle");
    set_named_variable_value( kID, (INT)m_dThrustVectorValue);
  }

  /////////////////////////////////////////////////////////
  //ESTIMATED FLIGHT TIME
  /////////////////////////////////////////////////////////
  if(m_akEngine[0].dFF > 0)
  {
    // Estimated Flight Time
    m_dEstimatedFlightTime = m_fFuelTotalQuantityWeightKg/m_akEngine[0].dFF;
  }
}

