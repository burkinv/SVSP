#include "AC.h"
#include "FCR.h"
#include "SMS.h"
#include "__SVSPAvionics.h"

const FLOAT                 SMS::INTERNAL_BAY_LAUNCHER_ANIMATION_VELOCITY = 5.25;
SMS::PART_ANIMATION_TAG     SMS::m_akInternalBayLauncher[]                = 
{
  { 3, "internal_bay_launcher_l1", 0, 0},
  { 4, "internal_bay_launcher_l2", 0, 0},
  {11, "internal_bay_launcher_r1", 0, 0},
  {10, "internal_bay_launcher_r2", 0, 0},
};


__SVSPTouchRect SMS::g_akTouchMapTable[SMS::DISPLAY_STATE_MAX][__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE] = 
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
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-0*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect("JET",   SMS::EVENT_ID_JETTISON_LOAD,NULL,                         D3DXVECTOR3( 1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_UP_SIDE_BUTTON),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3( 2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),

    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  0*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(   __SVSPModule::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),

    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3( 2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3( 1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect("MENU",  EVENT_ID_SET_ROOT_MENU,     NULL,                         D3DXVECTOR3(-0*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, GEOM_OBJECT_TOUCH_RECT, TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-1*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(-2*__SVSPModule::DISPLAY_WINDOW_WIDTH/7, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),

    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  0*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  1*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL,                         D3DXVECTOR3(  -__SVSPModule::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  2*__SVSPModule::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
  },
};

SMS::SMS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("SMS", pkAvionics, dwThreadSleepTime, true, NULL, NULL, SMS::GEOM_OBJECT_NUM_OBJECTS, NULL, SMS::TEXTURE_OBJECT_NUM_OBJECTS, SMS::g_akTouchMapTable, SMS::DISPLAY_STATE_MAX),
m_pkSelectedWeapon(NULL)
{
  memset(&m_akWeaponGroup[0], 0, SMS::MAX_NUM_WEAPON_GROUPS*sizeof(WEAPON::WEAPON_GROUP));

  for(UINT i = 0; i < SMS::MAX_NUM_STATIONS; ++i)
  {
    for(UINT k = 0; k < SMS::MAX_NUM_UNITS_PER_STATION; ++k)
    {
      CHAR acStr[128];

      sprintf_s(&acStr[0], sizeof(acStr), "GENERAL_VISIBILITY_ID_%d_%d", i, k);
      ID kID = check_named_variable(&acStr[0]);
      set_named_variable_value( kID, (BOOL)0 );
    }
  }

  memset(&m_kSetAircraftStationLoadData.uiStationLoadWeight[0], 0, sizeof(m_kSetAircraftStationLoadData.uiStationLoadWeight));

  ///////////////////////////////////////////////////////////////////
  // SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA (SMS::MAX_NUM_WEAPONS)
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:1",  "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:2",  "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:3",  "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:4",  "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:5",  "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:6",  "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:7",  "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:8",  "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:9",  "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:10", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:11", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:12", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:13", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:14", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:15", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:16", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:17", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:18", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:19", "Pounds", SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,  "PAYLOAD STATION WEIGHT:20", "Pounds", SIMCONNECT_DATATYPE_INT32);

  //Clear all FSX stations (Saved FSX flight files could occasionaly restore stations loads).
  memset(&m_kSetAircraftStationLoadData.uiStationLoadWeight[0], 0, sizeof(m_kSetAircraftStationLoadData.uiStationLoadWeight));
  m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA, SIMCONNECT_OBJECT_ID_USER, 0, 1, sizeof(SMS::SET_AIRCRAFT_STATION_LOAD_DATA), &m_kSetAircraftStationLoadData);
  AAV_ASSERT(SUCCEEDED(m_hResult));


  LPD3DXGEOMETRYOBJECT    pkGeomObject = NULL;
  __SVSPVertex*           pVerts       = NULL;

  UINT i;
  FLOAT r;
  FLOAT a;
  FLOAT fGradScale;
  FLOAT fBackgroundWidth = 5.0f;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_BACKGROUND
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_BACKGROUND_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  pVerts[0] = __SVSPVertex(-DISPLAY_WINDOW_WIDTH/2,  DISPLAY_WINDOW_WIDTH/2, 0.01f, 0.0f, 0.0f, __SVSPColor::dwBACKGROUNDCOLOR);
  pVerts[1] = __SVSPVertex( DISPLAY_WINDOW_WIDTH/2,  DISPLAY_WINDOW_WIDTH/2, 0.01f, 1.0f, 0.0f, __SVSPColor::dwBACKGROUNDCOLOR);
  pVerts[2] = __SVSPVertex(-DISPLAY_WINDOW_WIDTH/2, -DISPLAY_WINDOW_WIDTH/2, 0.01f, 0.0f, 1.0f, __SVSPColor::dwBACKGROUNDCOLOR);
  pVerts[3] = __SVSPVertex( DISPLAY_WINDOW_WIDTH/2, -DISPLAY_WINDOW_WIDTH/2, 0.01f, 1.0f, 1.0f, __SVSPColor::dwBACKGROUNDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_BACKGROUND] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/SMS/RES/TEXTURES/SMS.dds");

  AAVArea<FLOAT> kWeaponImageArea = AAVArea<FLOAT>(32, 128);
  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_WEAPON_IMAGE
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_WEAPON_IMAGE_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-kWeaponImageArea.tWidth/2,  kWeaponImageArea.tHeight/2, 0.0f, 0.0f, 0.0f, __SVSPColor::dwGUICOLOR);
  pVerts[1] = __SVSPVertex( kWeaponImageArea.tWidth/2,  kWeaponImageArea.tHeight/2, 0.0f, 1.0f, 0.0f, __SVSPColor::dwGUICOLOR);
  pVerts[2] = __SVSPVertex(-kWeaponImageArea.tWidth/2, -kWeaponImageArea.tHeight/2, 0.0f, 0.0f, 1.0f, __SVSPColor::dwGUICOLOR);
  pVerts[3] = __SVSPVertex( kWeaponImageArea.tWidth/2, -kWeaponImageArea.tHeight/2, 0.0f, 1.0f, 1.0f, __SVSPColor::dwGUICOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_WEAPON_IMAGE] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_SELECTED_WEAPON_IMAGE
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_SELECTED_WEAPON_IMAGE_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-kWeaponImageArea.tWidth/2,  kWeaponImageArea.tHeight/2, 0.0f, 0.0f, 0.0f, __SVSPColor::dwACTIVEGUICOLOR);
  pVerts[1] = __SVSPVertex( kWeaponImageArea.tWidth/2,  kWeaponImageArea.tHeight/2, 0.0f, 1.0f, 0.0f, __SVSPColor::dwACTIVEGUICOLOR);
  pVerts[2] = __SVSPVertex(-kWeaponImageArea.tWidth/2, -kWeaponImageArea.tHeight/2, 0.0f, 0.0f, 1.0f, __SVSPColor::dwACTIVEGUICOLOR);
  pVerts[3] = __SVSPVertex( kWeaponImageArea.tWidth/2, -kWeaponImageArea.tHeight/2, 0.0f, 1.0f, 1.0f, __SVSPColor::dwACTIVEGUICOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_SELECTED_WEAPON_IMAGE] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // WEAPON DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_WEAPON_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  r = 0.10f;
  a = 0.08f;

  fGradScale = 360.0f/(NUM_WEAPON_DESIGNATOR_VERTICES - 2);
  pVerts[0] = __SVSPVertex(0.0f, 0.0f, 0.0f);
  for(i = 1; i < (NUM_WEAPON_DESIGNATOR_VERTICES - 1); ++i)
  {
    pVerts[i] = __SVSPVertex(r*cos((D3DX_PI/180.0f)*i*fGradScale), r*sin((D3DX_PI/180.0f)*i*fGradScale), 0.0f);
  }
  pVerts[i] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_WEAPON_DESIGNATOR] = pkGeomObject;

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
  m_ppkGeomObjects[SMS::GEOM_OBJECT_TOUCH_RECT] = pkGeomObject;

  m_ppkTextureObjects[TEXTURE_OBJECT_UP_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/UP_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_LT_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/LT_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_RT_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/RT_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_BM_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/BM_SIDE_BUTTON.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, SMS::NUM_FRAME_BORDER_VERTICES);
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
  m_ppkGeomObjects[SMS::GEOM_OBJECT_FRAME_BORDER] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_CORNERS
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, SMS::NUM_FRAME_CORNERS_VERTICES);
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
  m_ppkGeomObjects[SMS::GEOM_OBJECT_FRAME_CORNERS] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, SMS::NUM_FRAME_BORDER_VERTICES);
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
  m_ppkGeomObjects[SMS::GEOM_OBJECT_ACTIVE_FRAME_BORDER] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_CORNERS
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, SMS::NUM_FRAME_CORNERS_VERTICES);
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
  m_ppkGeomObjects[SMS::GEOM_OBJECT_ACTIVE_FRAME_CORNERS] = pkGeomObject;


  m_apkProcessStateCallback[SMS::SMS_STATE_IDLE]      = &SMS::ProcessStateIdle;
  m_apkProcessStateCallback[SMS::SMS_STATE_ARMED]     = &SMS::ProcessStateArmed;

  SetState(SMS::SMS_STATE_IDLE);
}

SMS::~SMS()
{
  Stop();

  DeleteAllStores();
}

VOID SMS::SetState(STATE eState)
{
  WEAPON::WEAPON_GROUP *pkSelectedGroup = NULL;

  switch(eState)
  {
  case SMS::SMS_STATE_IDLE:
      m_pkSelectedWeapon = NULL;
      UnarmAllWeaponGroups();
      for(UINT i = 0; i < SMS::INTERNAL_BAY_LAUNCHER_MAX; ++i)
      {
        m_akInternalBayLauncher[i].dValueToGain = 0;
      }
      m_eState = eState;
      break;

    case SMS::SMS_STATE_ARMED:
      SelectNextWeaponGroup();
      m_eState = SMS_STATE_ARMED;
      break;

    default:
      break;
  }
}

VOID SMS::ProcessInternalBayLauncherState()
{
  for(UINT i = 0; i < SMS::INTERNAL_BAY_LAUNCHER_MAX; ++i)
  {
    SMS::PART_ANIMATION_TAG *pkInternalBayLauncher = &m_akInternalBayLauncher[i];
    FLOAT dInternalBayLauncherValueError = pkInternalBayLauncher->dValue - pkInternalBayLauncher->dValueToGain;
    if(abs(dInternalBayLauncherValueError) > 0.01)
    {
      dInternalBayLauncherValueError *= SMS::INTERNAL_BAY_LAUNCHER_ANIMATION_VELOCITY;
      pkInternalBayLauncher->dValue -= (dInternalBayLauncherValueError/sqrt(1 + dInternalBayLauncherValueError*dInternalBayLauncherValueError));
      ID kID = check_named_variable(pkInternalBayLauncher->pcName);
      set_named_variable_value( kID, (INT)pkInternalBayLauncher->dValue);
    }
  }
}

VOID SMS::SelectNextWeaponGroup()
{
  FCR *pkFCR = static_cast<FCR *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FCR));
  AAV_ASSERT_RETURN_VOID(pkFCR != NULL);

  //Store information about current selected weapon group.
  WEAPON::WEAPON_GROUP *pkPreviousWeaponGroup = NULL;
  if(m_pkSelectedWeapon != NULL)
  {
    pkPreviousWeaponGroup = m_pkSelectedWeapon->GetWeaponGroup();
    AAV_ASSERT_RETURN_VOID(pkPreviousWeaponGroup != NULL);
  }

  //Search next available weapon group
  WEAPON::WEAPON_GROUP *pkNextGroup = GetNextWeaponGroup();
  if(pkNextGroup != NULL)
  {
    //Close internal weapon bays from previous group if necessary.
    if((pkPreviousWeaponGroup != NULL) && (pkPreviousWeaponGroup != pkNextGroup))
    {
      for(UINT i = 0; (i < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS); ++i)
      {
        WEAPON *pkWeapon = pkPreviousWeaponGroup->apkWeapon[i];
        if(pkWeapon != NULL)
        {
          UINT uiWeaponStationID = pkWeapon->GetStationId();
          for(UINT i = 0; i < SMS::INTERNAL_BAY_LAUNCHER_MAX; ++i)
          {
            if(m_akInternalBayLauncher[i].uiStationId == uiWeaponStationID)
            {
              m_akInternalBayLauncher[i].dValueToGain = 0;
            }
          }
        }
      }
    }

    //Select next available weapon
    WEAPON *pkNextWeapon = pkNextGroup->GetNextIdleWeapon();
    if(pkNextWeapon != NULL)
    {
      if((m_pkSelectedWeapon != NULL) && (m_pkSelectedWeapon->GetState() < WEAPON::WEAPON_STATE_LAUNCH))
      {
        m_pkSelectedWeapon->SetState(WEAPON::WEAPON_STATE_IDLE);
        m_pkSelectedWeapon->SetTargetID(0xFFFFFFFF);
      }

      if(pkNextWeapon->GetState() == WEAPON::WEAPON_STATE_IDLE)
      {
        pkNextWeapon->SetState(WEAPON::WEAPON_STATE_ARMED);
      }
      pkNextWeapon->SetTargetID(pkFCR->GetTargetID());
      m_pkSelectedWeapon = pkNextWeapon;
    }
  }

  if(m_pkSelectedWeapon != NULL)
  {
    WEAPON::WEAPON_GROUP *pkSelectedWeaponGroup = m_pkSelectedWeapon->GetWeaponGroup();
    AAV_ASSERT_RETURN_VOID(pkSelectedWeaponGroup != NULL);

    //Open internal weapon bays if necessary (even if the weapon no longer available).
    for(UINT i = 0; (i < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS); ++i)
    {
      WEAPON *pkWeapon = pkSelectedWeaponGroup->apkWeapon[i];
      if(pkWeapon != NULL)
      {
        UINT uiWeaponStationID = pkWeapon->GetStationId();
        for(UINT i = 0; i < SMS::INTERNAL_BAY_LAUNCHER_MAX; ++i)
        {
          if(m_akInternalBayLauncher[i].uiStationId == uiWeaponStationID)
          {
            m_akInternalBayLauncher[i].dValueToGain = 40;
          }
        }
      }
    }
  }
}

VOID SMS::AddStore(WEAPON *pkWeapon)
{
  AAV_ASSERT_RETURN_VOID(pkWeapon != NULL);
  AAV_ASSERT_RETURN_VOID(pkWeapon->GetName()->getLength() != 0);

  WEAPON::WEAPON_GROUP *pkFoundGroup = NULL;

  for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
  {
    pkFoundGroup = &m_akWeaponGroup[i];

    //If empty group found add new group right away.
    if((pkFoundGroup->acName.getLength() == 0) && (pkFoundGroup->uiAmount == 0))
    {
      pkFoundGroup->acName = __SVSPString<CHAR>(pkWeapon->GetName()->getCStr());
      break;
    }
    else 
      //If non empty group found, check whether the weapon fits to it.
      if(pkFoundGroup->acName == *(pkWeapon->GetName()))
      {
        //Check if group is not full.
        if(pkFoundGroup->uiAmount < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS)
        {
          break;
        }
        else
        {
          return;
        }
      }
      //Proceed with search.
  }

  //Add weapon to the found group.
  if(pkFoundGroup != NULL)
  {
    //Assert StationID.
    UINT uiStationID = pkWeapon->GetStationId();
    UINT uiUnitID    = pkWeapon->GetUnitId();
    AAV_ASSERT_RETURN_VOID(uiStationID < SMS::MAX_NUM_STATIONS);

    // Unhide static weapon model
    CHAR acStr[128];
    sprintf_s(&acStr[0], sizeof(acStr), "GENERAL_VISIBILITY_ID_%d_%d", uiStationID, uiUnitID);
    ID kID = check_named_variable(&acStr[0]);
    set_named_variable_value(kID, (BOOL)1);

    //Acknowledge FS about changed stations weight.
    m_kSetAircraftStationLoadData.uiStationLoadWeight[uiStationID] += pkWeapon->GetWeight();
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA, SIMCONNECT_OBJECT_ID_USER, 0, 1, sizeof(SMS::SET_AIRCRAFT_STATION_LOAD_DATA), &m_kSetAircraftStationLoadData);
    AAV_ASSERT(SUCCEEDED(m_hResult));

    //Add weapon to the found group.
    pkWeapon->SetWeaponGroup(pkFoundGroup);
    pkWeapon->SetState(WEAPON::WEAPON_STATE_IDLE);
    pkFoundGroup->apkWeapon[pkFoundGroup->uiAmount] = pkWeapon;
    pkFoundGroup->uiAmount++;
  }
}

VOID SMS::RemoveStore(WEAPON *pkWeapon)
{
  AAV_ASSERT_RETURN_VOID(pkWeapon != NULL);
  //AAV_ASSERT_RETURN_VOID(pkWeapon->GetState() >= WEAPON::WEAPON_STATE_LAUNCH);

  WEAPON::WEAPON_GROUP *pkGroup = pkWeapon->GetWeaponGroup();
  AAV_ASSERT_RETURN_VOID(pkGroup != NULL);

  //Remove from the group.
  AAV_ASSERT_RETURN_VOID(pkGroup->uiAmount > 0);
  pkGroup->uiAmount--;

  //Assert StationID.
  UINT uiStationID = pkWeapon->GetStationId();
  UINT uiUnitID    = pkWeapon->GetUnitId();
  AAV_ASSERT_RETURN_VOID(uiStationID < SMS::MAX_NUM_STATIONS);

  //Hide static weapon model
  CHAR acStr[128];
  sprintf_s(&acStr[0], sizeof(acStr), "GENERAL_VISIBILITY_ID_%d_%d", uiStationID, uiUnitID);
  ID kID = check_named_variable(&acStr[0]);
  set_named_variable_value(kID, (BOOL)0);

  //Acknowledge FS about changed stations weight.
  m_kSetAircraftStationLoadData.uiStationLoadWeight[uiStationID] -= pkWeapon->GetWeight();
  AAV_ASSERT_RETURN_VOID(m_kSetAircraftStationLoadData.uiStationLoadWeight[uiStationID] >= 0);
  m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA, SIMCONNECT_OBJECT_ID_USER, 0, 1, sizeof(SMS::SET_AIRCRAFT_STATION_LOAD_DATA), &m_kSetAircraftStationLoadData);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

VOID SMS::RemoveAllStores()
{
  //Unselect active weapon.
  m_pkSelectedWeapon = NULL;

  for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
  {
    WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
    if(pkGroup->acName != 0)
    {
      for(UINT j = 0; j < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS; ++j)
      {
        WEAPON *pkWeapon = pkGroup->apkWeapon[j];
        if((pkWeapon != NULL) && (pkWeapon->GetState() < WEAPON::WEAPON_STATE_LAUNCH))
        {
          RemoveStore(pkWeapon);
        }
      }
      AAV_ASSERT_RETURN_VOID(pkGroup->uiAmount == 0);
    }
  }
  
  //Zero memory.
  memset(&m_akWeaponGroup[0], 0, sizeof(m_akWeaponGroup));
}

VOID  SMS::DeleteAllStores()
{
  Stop();

  //Unselect active weapon.
  m_pkSelectedWeapon = NULL;

  for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
  {
    WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
    if(pkGroup->acName != 0)
    {
      for(UINT j = 0; j < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS; ++j)
      {
        WEAPON *pkWeapon = pkGroup->apkWeapon[j];
        if(pkWeapon != NULL)
        {
          delete pkWeapon;
        }
      }
      pkGroup->acName = 0;
      pkGroup->uiAmount = 0;
    }
  }

  //Zero memory.
  memset(&m_akWeaponGroup[0], 0, sizeof(m_akWeaponGroup));

  //Clear all aircraft stations (Weapons, Pilons and Launchers)
  for(UINT i = 0; i < SMS::MAX_NUM_STATIONS; ++i)
  {
    for(UINT k = 0; k < SMS::MAX_NUM_UNITS_PER_STATION; ++k)
    {
      CHAR acStr[128];

      sprintf_s(&acStr[0], sizeof(acStr), "GENERAL_VISIBILITY_ID_%d_%d", i, k);
      ID kID = check_named_variable(&acStr[0]);
      set_named_variable_value( kID, (BOOL)0 );
    }
  }

  //Acknowledge FS about changed stations weight.
  memset(&m_kSetAircraftStationLoadData.uiStationLoadWeight[0], 0, sizeof(m_kSetAircraftStationLoadData.uiStationLoadWeight));
  m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, SMS::DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA, SIMCONNECT_OBJECT_ID_USER, 0, 1, sizeof(SMS::SET_AIRCRAFT_STATION_LOAD_DATA), &m_kSetAircraftStationLoadData);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

WEAPON::WEAPON_GROUP *SMS::GetNextWeaponGroup()
{
  if(m_pkSelectedWeapon != NULL)
  {
    if(m_pkSelectedWeapon->GetWeaponGroup() != NULL)
    {
      for(UINT uiSelectedGroupIndex = 0; uiSelectedGroupIndex < SMS::MAX_NUM_WEAPON_GROUPS; uiSelectedGroupIndex++)
      {
        if(&m_akWeaponGroup[uiSelectedGroupIndex] == m_pkSelectedWeapon->GetWeaponGroup())
        {
          //Search after selected group
          for(UINT i = uiSelectedGroupIndex + 1; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
          {
            WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
            if((pkGroup != NULL) && (pkGroup->acName != 0) && (pkGroup->uiAmount != 0))
            {
              return pkGroup;
            }
          }
          // Search before selected group
          for(UINT i = 0; i < uiSelectedGroupIndex; ++i)
          {
            WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
            if((pkGroup != NULL) && (pkGroup->acName != 0) && (pkGroup->uiAmount != 0))
            {
              return pkGroup;
            }
          }
        }
      }
    }
  }
  else
  {
    for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
    {
      WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
      if((pkGroup != NULL) && (pkGroup->acName != 0) && (pkGroup->uiAmount != 0))
      {
        return pkGroup;
      }
    }
  }
  return NULL;
}

WEAPON* SMS::GetNextWeapon()
{
  WEAPON::WEAPON_GROUP *pkGroup  = NULL;
  WEAPON               *pkNextWeapon = NULL;

  // Search for any non empty group.
  if(m_pkSelectedWeapon != NULL)
  {
    pkGroup = m_pkSelectedWeapon->GetWeaponGroup();
    if(pkGroup != NULL)
    {
      if(pkGroup->GetAmount() == 0)
      {
        pkGroup = GetNextWeaponGroup();
      }
    }
  }
  else
  {
    pkGroup = GetNextWeaponGroup();
  }

  // Get next idle weapon in the selected group.
  if(pkGroup != NULL)
  {
    pkNextWeapon = pkGroup->GetNextIdleWeapon();
  }

  return pkNextWeapon;
}

VOID SMS::UnarmAllWeaponGroups()
{
  for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
  {
    WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
    if(pkGroup->acName != 0)
    {
      for(UINT j = 0; j < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS; ++j)
      {
        WEAPON *pkWeapon = pkGroup->apkWeapon[j];
        if((pkWeapon != NULL) && (pkWeapon->GetState() < WEAPON::WEAPON_STATE_LAUNCH))
        {
          pkWeapon->SetTargetID(0xFFFFFFFF);
          pkWeapon->SetState(WEAPON::WEAPON_STATE_IDLE);
        }
      }
    }
  }
}

VOID SMS::Start()
{
  for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
  {
    WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
    if(pkGroup->acName != 0)
    {
      for(UINT j = 0; j < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS; ++j)
      {
        WEAPON *pkWeapon = pkGroup->apkWeapon[j];
        if((pkWeapon != NULL) && (!pkWeapon->IsStarted()))
        {
          pkWeapon->Start();
        }
      }
    }
  }

  __SVSPModule::Start();
}

VOID SMS::Stop()
{
  if(!m_bAbort)
  {
    __SVSPModule::Stop();

    for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
    {
      WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
      if(pkGroup->acName != 0)
      {
        for(UINT j = 0; j < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS; ++j)
        {
          WEAPON *pkWeapon = pkGroup->apkWeapon[j];
          if((pkWeapon != NULL) && (pkWeapon->IsStarted()))
          {
            pkGroup->apkWeapon[j]->Stop();
          }
        }
      }
    }
  }
}

VOID SMS::CheckWeaponState()
{
  FCR *pkFCR = static_cast<FCR *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FCR));
  AAV_ASSERT_RETURN_VOID(pkFCR != NULL);

  if(m_pkSelectedWeapon == NULL)
  {
    //Select and arm next weapon if any.
    m_pkSelectedWeapon = GetNextWeapon();
  }

  if(m_pkSelectedWeapon != NULL)
  {
    //Check weapon target.
    if(m_pkSelectedWeapon->GetTargetID() != pkFCR->GetTargetID())
    {
      m_pkSelectedWeapon->SetTargetID(pkFCR->GetTargetID());
    }

    WEAPON::WEAPON_GROUP *pkSelectedWeaponGroup = m_pkSelectedWeapon->GetWeaponGroup();
    AAV_ASSERT_RETURN_VOID(pkSelectedWeaponGroup != NULL);
  }

}

HRESULT SMS::ProcessStateIdle()
{
  MSG kMsg;

  FCR *pkFCR = static_cast<FCR *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FCR));
  AAV_ASSERT_RETURN(pkFCR != NULL, S_FALSE);
  // Process Internal Weapon Bays and Launchers
  ProcessInternalBayLauncherState();

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_RESET:
      for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
      {
        WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
        if(pkGroup->acName != 0)
        {
          pkGroup->uiAmount = 0;
          for(UINT j = 0; j < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS; ++j)
          {
            if(pkGroup->apkWeapon[j] != NULL)
            {
              pkGroup->apkWeapon[j]->SetState(WEAPON::WEAPON_STATE_IDLE);
              pkGroup->uiAmount++;
            }
          }
        }
      }
      m_pkSelectedWeapon = NULL;
      break;

    case SMS::MESSAGE_JETTISON_ALL_PAYLOAD:
      RemoveAllStores();
      break;

    default:
      break;
    }
  }

  //Check SMS state.
  if(pkFCR->GetState() == FCR::FCR_STATE_ON)
  {
    SetState(SMS_STATE_ARMED);
  }

  return S_OK;
}

HRESULT SMS::ProcessStateArmed()
{
  MSG kMsg;

  FCR *pkFCR = static_cast<FCR *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FCR));
  AAV_ASSERT_RETURN(pkFCR != NULL, S_FALSE);
  // Process Internal Weapon Bays and Launchers
  ProcessInternalBayLauncherState();

  // Check weapon state
  CheckWeaponState();

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case SMS::MESSAGE_SELECT_NEXT_WEAPON_GROUP:
      SelectNextWeaponGroup();
      break;

    case SMS::MESSAGE_SELECT_NEXT_WEAPON_IN_GROUP:
      {
        WEAPON *pkNextWeapon = GetNextWeapon();
        if(pkNextWeapon != NULL)
        {
          if((m_pkSelectedWeapon != NULL) && (m_pkSelectedWeapon->GetState() < WEAPON::WEAPON_STATE_LAUNCH))
          {
            m_pkSelectedWeapon->SetState(WEAPON::WEAPON_STATE_IDLE);
            m_pkSelectedWeapon->SetTargetID(0xFFFFFFFF);
          }

          if(pkNextWeapon->GetState() == WEAPON::WEAPON_STATE_IDLE)
          {
            pkNextWeapon->SetState(WEAPON::WEAPON_STATE_ARMED);
          }
          pkNextWeapon->SetTargetID(pkFCR->GetTargetID());
          m_pkSelectedWeapon = pkNextWeapon;
        }
      }
      break;

    case SMS::MESSAGE_RELEASE_WEAPON:
      if((m_pkSelectedWeapon != NULL) && (m_pkSelectedWeapon->GetState() == WEAPON::WEAPON_STATE_LOCKED))
      {
        //Get weapon group
        WEAPON::WEAPON_GROUP *pkSelectedWeaponGroup = m_pkSelectedWeapon->GetWeaponGroup();
        AAV_ASSERT_RETURN(pkSelectedWeaponGroup != NULL, S_FALSE);

        //Launch weapon
        m_pkSelectedWeapon->SetState(WEAPON::WEAPON_STATE_LAUNCH);
        //Remove launched store
        RemoveStore(m_pkSelectedWeapon);
        //Get next idle weapon in the selected group.
        m_pkSelectedWeapon = pkSelectedWeaponGroup->GetNextIdleWeapon();
      }
      break;

    case SMS::MESSAGE_JETTISON_ALL_PAYLOAD:
      RemoveAllStores();
      break;

    default:
      // Forward message to the selected weapon.
      if(m_pkSelectedWeapon != NULL)
      {
        PostThreadMessage(m_pkSelectedWeapon->GetThreadId(), kMsg.message, kMsg.wParam, kMsg.lParam);
      }
      break;
    }
  }

  //Check SMS state.
  if(pkFCR->GetState() == FCR::FCR_STATE_OFF)
  {
    SetState(SMS_STATE_IDLE);
  }

  return S_OK;
}

VOID SMS::Process()
{
  AAV_ASSERT(m_eState < SMS::SMS_STATE_MAX);
  HRESULT (__thiscall SMS::*__ProcessStateCallback)(VOID) = m_apkProcessStateCallback[m_eState];
  AAV_ASSERT(__ProcessStateCallback != NULL);
  (this->*__ProcessStateCallback)();
}

VOID SMS::Release()
{
  for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
  {
    WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
    if(pkGroup->acName != 0)
    {
      pkGroup->Release();
    }
  }

  __SVSPModule::Release();
}

HRESULT SMS::Reset(LPD3DXDEVICE pkDevice)
{
  HRESULT hResult = S_OK;
  for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
  {
    WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
    if(pkGroup->acName != 0)
    {
      hResult = pkGroup->Reset(pkDevice);
    }
  }

  return __SVSPModule::Reset(pkDevice);
}


VOID SMS::RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  if(m_pkSelectedWeapon != NULL)
  {
    m_pkSelectedWeapon->RenderOnHUD(pkDevice, rkOPMatrix);
  }
}

VOID SMS::RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  switch(m_eDisplayState)
  {
  case SMS::DISPLAY_STATE_MAIN:
    {
      LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
      LPD3DXTEXTUREOBJECT  pkTextureObject = NULL;

      __SVSPFont* pkFont = m_pkAvionics->GetFont();
      AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

      //rkOPMatrix = m_kOPMatrix*pkAC->m_kLocalToBodyTransformMatrix;

      ///////////////////////////////////////////////////////////////////
      // GEOM_OBJECT_BACKGROUND
      ///////////////////////////////////////////////////////////////////
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND];
      pkTextureObject = m_ppkTextureObjects[TEXTURE_OBJECT_BACKGROUND];
      if(pkGeomObject != NULL)
      {
        pkGeomObject->SetTransform(pkDevice, &rkOPMatrix);
        pkGeomObject->SetTexture(pkDevice, pkTextureObject);
        pkGeomObject->Render(pkDevice);
        pkGeomObject->SetTexture(pkDevice, NULL);
      }

      ///////////////////////////////////////////////////////////////////
      // GEOM_OBJECT_WEAPON_IMAGE
      ///////////////////////////////////////////////////////////////////
      D3DXMATRIXA16 kWeaponImageMatrix;
      LPD3DXGEOMETRYOBJECT pkGeomObjectWeaponImage              = m_ppkGeomObjects[GEOM_OBJECT_WEAPON_IMAGE];
      AAV_ASSERT_RETURN_VOID(pkGeomObjectWeaponImage != NULL);
      LPD3DXGEOMETRYOBJECT pkGeomObjectSelectedWeaponImage      = m_ppkGeomObjects[GEOM_OBJECT_SELECTED_WEAPON_IMAGE];
      AAV_ASSERT_RETURN_VOID(pkGeomObjectSelectedWeaponImage != NULL);

      // Render weapon type and unit quantity.
      if(m_pkSelectedWeapon != NULL)
      {
        WEAPON::WEAPON_GROUP *pkWeaponGroup = m_pkSelectedWeapon->GetWeaponGroup();
        AAV_ASSERT_RETURN_VOID(pkWeaponGroup != NULL);

        pkGeomObjectSelectedWeaponImage->SetTransform(pkDevice, &rkOPMatrix);
        pkFont->RenderTextExt(pkDevice, pkWeaponGroup->GetName(), &D3DXVECTOR3(-m_kWindowRect.kArea.tWidth/2 + 60.0f, m_kWindowRect.kArea.tHeight/2 - 30.0f, 0), __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);
      }

      //Get selected weapon group
      WEAPON::WEAPON_GROUP *pkSelectedWeaponGroup = (m_pkSelectedWeapon != NULL) ? m_pkSelectedWeapon->GetWeaponGroup() : NULL;

      const FLOAT fPilonOffsetScale = 38.5f;

      for(UINT i = 0; i < SMS::MAX_NUM_WEAPON_GROUPS; ++i)
      {
        WEAPON::WEAPON_GROUP *pkGroup = &m_akWeaponGroup[i];
        if(pkGroup->acName != 0)
        {
          for(UINT j = 0; j < WEAPON::WEAPON_GROUP::MAX_NUM_WEAPONS; ++j)
          {
            WEAPON *pkWeapon = pkGroup->apkWeapon[j];
            if((pkWeapon != NULL) && (pkWeapon->GetState() < WEAPON::WEAPON_STATE_LAUNCH))
            {
              const AAVVector3<FLOAT>* pkPilonVector = pkWeapon->GetPilonVector();
              D3DXMatrixTranslation(&kWeaponImageMatrix, pkPilonVector->x*fPilonOffsetScale - m_kWindowRect.kArea.tWidth/512, pkPilonVector->z*fPilonOffsetScale + 30*m_kWindowRect.kArea.tWidth/512.0f, 0);

              pkGeomObject = (pkWeapon->GetWeaponGroup() == pkSelectedWeaponGroup) ? pkGeomObjectSelectedWeaponImage : pkGeomObjectWeaponImage;
              pkGeomObject->SetTransform(pkDevice, &(kWeaponImageMatrix*rkOPMatrix));
              pkGeomObject->SetTexture(pkDevice, pkWeapon->GetWeaponTexture());
              //pkGeomObject->SetTexture(pkDevice, NULL);
              pkGeomObject->Render(pkDevice);
            }
          }
        }
      }
    }
    break;

  default:
    break;
  }
    __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}



BOOL SMS::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  switch(dwEvent)
  {
  case EVENT_ID_JETTISON_LOAD:
    PostThreadMessage(GetThreadId(), MESSAGE_JETTISON_ALL_PAYLOAD, 0, 0);
    break;

  default:
    __SVSPModule::EventHandler(dwEvent, dwEventData);
    break;
  }
  return TRUE;
}
