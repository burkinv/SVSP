#include "AC.h"
#include "FCR.h"
#include "WPN.h"
#include "MDA.h"
#include "FCR.h"
#include "__SVSPAvionics.h"

const FLOAT                    FCR::DISPLAY_WINDOW_WIDTH                            = 2*__SVSPModule::DISPLAY_WINDOW_WIDTH; //1.55f*__SVSPModule::DISPLAY_WINDOW_WIDTH;
const FLOAT                    FCR::DISPLAY_WINDOW_HEIGHT                           =   __SVSPModule::DISPLAY_WINDOW_HEIGHT;//1.10f*__SVSPModule::DISPLAY_WINDOW_HEIGHT;
const FLOAT                    FCR::INITIAL_NUM_PIXELS_IN_NAUTICAL_MILE             = 30;
const FLOAT                    FCR::INITIAL_FCR_RANGE_MILES                         = 10;
const FLOAT                    FCR::TEN_MILES_CIRCLE_RANGE                          = 10*INITIAL_NUM_PIXELS_IN_NAUTICAL_MILE;
const FLOAT                    FCR::MAX_LOCK_DISTANCE                               =  80000.0f;
const FLOAT                    FCR::MIN_LOCK_DISTANCE                               =  10.0f;
const FLOAT                    FCR::AQUISITION_ASPECT_CAGED                         = 0.9962f; //Cos(5.0);
const FLOAT                    FCR::AQUISITION_ASPECT_UNCAGED                       = 0.9659f; //Cos(15.0);
const FLOAT                    FCR::g_afRadarRangeTables[FCR::MAX_NUM_RADAR_RANGES] = {3.0000, 6.0000, 9.0000, 15.0000, 30.0000, 60.0000, 90.0000, 120.0000};

__SVSPTouchRect FCR::g_akTouchMapTable[FCR::DISPLAY_STATE_MAX][__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE] = 
{
  /* DISPLAY_STATE_ROOT_MENU */
  {
    __SVSPTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3(-2*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect("ECS",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_ECS,       D3DXVECTOR3(-1*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3(-0*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect("FUEL",  EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_EICAS,     D3DXVECTOR3( 1*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                NULL,                                D3DXVECTOR3( 2*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),

    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  2*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("FLCS",  EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_FLCS,      D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  1*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  0*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("FCR",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_FCR,       D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -1*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -2*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),

    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3( 2*FCR::DISPLAY_WINDOW_WIDTH/7, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect("FCR",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_FCR,       D3DXVECTOR3( 1*FCR::DISPLAY_WINDOW_WIDTH/7, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(-0*FCR::DISPLAY_WINDOW_WIDTH/7, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect("COP",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_CARRIER,   D3DXVECTOR3(-1*FCR::DISPLAY_WINDOW_WIDTH/7, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(-2*FCR::DISPLAY_WINDOW_WIDTH/7, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),

    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -2*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("AIP",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_AIP,       D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -1*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  0*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect("SIM",   EVENT_ID_SET_MODULE, __SVSPAvionics::AVIONICS_MODULE_SIM,       D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  1*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect(NULL,    NULL,                NULL,                                D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  2*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
  },

  /* DISPLAY_STATE_NAV */
  {
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(-2*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(-1*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(-0*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3( 1*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3( 2*FCR::DISPLAY_WINDOW_WIDTH/7, FCR::DISPLAY_WINDOW_HEIGHT/2 - kExtraOffsetVector.y,   0), kTouchRectArea),

    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  2*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  1*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x,  0*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -1*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(   FCR::DISPLAY_WINDOW_WIDTH/2 - kExtraOffsetVector.x, -2*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),

    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3( 2*FCR::DISPLAY_WINDOW_WIDTH/6, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),
    __SVSPTouchRect("IN",    FCR::EVENT_ID_ZOOM_IN,      NULL, D3DXVECTOR3( 1*FCR::DISPLAY_WINDOW_WIDTH/6, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, FCR::GEOM_OBJECT_TOUCH_RECT, FCR::TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __SVSPTouchRect("NOP",   NULL,                       NULL, D3DXVECTOR3(-0*FCR::DISPLAY_WINDOW_WIDTH/6, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, FCR::GEOM_OBJECT_TOUCH_RECT, FCR::TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __SVSPTouchRect("OUT",   FCR::EVENT_ID_ZOOM_OUT,     NULL, D3DXVECTOR3(-1*FCR::DISPLAY_WINDOW_WIDTH/6, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea, FCR::GEOM_OBJECT_TOUCH_RECT, FCR::TEXTURE_OBJECT_BM_SIDE_BUTTON),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(-2*FCR::DISPLAY_WINDOW_WIDTH/6, -FCR::DISPLAY_WINDOW_HEIGHT/2 + kExtraOffsetVector.y,  0), kTouchRectArea),

    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -2*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x, -1*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect(" - ",   FCR::EVENT_ID_RANGE_DEC,    NULL, D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  0*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, FCR::GEOM_OBJECT_TOUCH_RECT, FCR::TEXTURE_OBJECT_LT_SIDE_BUTTON),
    __SVSPTouchRect( NULL,   NULL,                       NULL, D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  1*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea),
    __SVSPTouchRect(" + ",   FCR::EVENT_ID_RANGE_INC,    NULL, D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  2*FCR::DISPLAY_WINDOW_HEIGHT/7, 0), kTouchRectArea, FCR::GEOM_OBJECT_TOUCH_RECT, FCR::TEXTURE_OBJECT_LT_SIDE_BUTTON),
  },
};

FCR::FCR(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("FCR", pkAvionics, dwThreadSleepTime, true, FCR__DispatchProcess, NULL, FCR::GEOM_OBJECT_NUM_OBJECTS, NULL, FCR::TEXTURE_OBJECT_NUM_OBJECTS, FCR::g_akTouchMapTable, FCR::DISPLAY_STATE_MAX),
m_pkTarget(NULL)
{
  SetRadarRange(4);
  m_fCurrentZoomFactor = 1;

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;

  UINT i;
  FLOAT r;
  FLOAT a;
  FLOAT fGradScale;
  FLOAT fBackgroundWidth = 5.0f;

  //Defines FCR Area Window for drawing on Upper HDD display.
  D3DXMATRIXA16 kWindowMatrix = *D3DXMatrixIdentity(&kWindowMatrix);
  m_kWindowRect = __SVSPTouchRect(NULL, NULL, NULL, &kWindowMatrix, AAVArea<FLOAT>(FCR::DISPLAY_WINDOW_WIDTH, FCR::DISPLAY_WINDOW_HEIGHT));

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
  m_ppkGeomObjects[FCR::GEOM_OBJECT_TOUCH_RECT] = pkGeomObject;

  m_ppkTextureObjects[TEXTURE_OBJECT_UP_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/UP_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_LT_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/LT_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_RT_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/RT_SIDE_BUTTON.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_BM_SIDE_BUTTON] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/BM_SIDE_BUTTON.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, FCR::NUM_FRAME_BORDER_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,  FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[1] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,  FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[2] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,  FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[3] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,  FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[4] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[5] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[6] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[7] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[8] = pVerts[0];
  pVerts[9] = pVerts[1];
  m_ppkGeomObjects[FCR::GEOM_OBJECT_FRAME_BORDER] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_CORNERS
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, FCR::NUM_FRAME_CORNERS_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2,                               FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 1] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,   FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 2] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2,                               FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);

  pVerts[ 3] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2,                               FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 4] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2,                               FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 5] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,                FCR::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __SVSPColor::dwCOLOR2);

  pVerts[ 6] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2,                              -FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 7] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,               -FCR::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[ 8] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2,                              -FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);

  pVerts[ 9] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2,                              -FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[10] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2,                              -FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR2);
  pVerts[11] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,  -FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwCOLOR2);
  m_ppkGeomObjects[FCR::GEOM_OBJECT_FRAME_CORNERS] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, FCR::NUM_FRAME_BORDER_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,  FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[1] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,  FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[2] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,  FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[3] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,  FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[4] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[5] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[6] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[7] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[8] = pVerts[0];
  pVerts[9] = pVerts[1];
  m_ppkGeomObjects[FCR::GEOM_OBJECT_ACTIVE_FRAME_BORDER] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_CORNERS
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, FCR::NUM_FRAME_CORNERS_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2,                               FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 1] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,   FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 2] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2,                               FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
                                                                                                                                                         
  pVerts[ 3] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2,                               FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 4] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2,                               FCR::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 5] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,                FCR::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
                                                                                                                                                         
  pVerts[ 6] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2,                              -FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 7] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,               -FCR::DISPLAY_WINDOW_HEIGHT/2,                0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 8] = __SVSPVertex( FCR::DISPLAY_WINDOW_WIDTH/2,                              -FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
                                                                                                                                                         
  pVerts[ 9] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2,                              -FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[10] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2,                              -FCR::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[11] = __SVSPVertex(-FCR::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,  -FCR::DISPLAY_WINDOW_HEIGHT/2,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  m_ppkGeomObjects[FCR::GEOM_OBJECT_ACTIVE_FRAME_CORNERS] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_CENTER_POINT_DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINELIST, NUM_CENTER_POINT_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  FLOAT a1 = 10;
  FLOAT a2 = 1;
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-a1,  0, 0, 0, 0);
  pVerts[1] = __SVSPVertex(-a2,  0, 0, 0, 0);
  pVerts[2] = __SVSPVertex(  0, a1, 0, 0, 0);
  pVerts[3] = __SVSPVertex(  0, a2, 0, 0, 0);
  pVerts[4] = __SVSPVertex( a1,  0, 0, 0, 0);
  pVerts[5] = __SVSPVertex( a2,  0, 0, 0, 0);
  pVerts[6] = __SVSPVertex(  0,-a1, 0, 0, 0);
  pVerts[7] = __SVSPVertex(  0,-a2, 0, 0, 0);
  m_ppkGeomObjects[GEOM_OBJECT_CENTER_POINT_DESIGNATOR] = pkGeomObject;

  // HSI Translation matrix
  D3DXMATRIX kTranslationMatrix = *D3DXMatrixTranslation(&kTranslationMatrix, 0, -50, 0);
  D3DXMATRIX kScalingMatrix     = *D3DXMatrixScaling    (&kScalingMatrix, 1, 1, 1);
  m_kOPMatrix = kTranslationMatrix*kScalingMatrix;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_RANGE
  ///////////////////////////////////////////////////////////////////
  //pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_RANGE_VERTICES);
  //pkGeomObject->SetMaterial(0.3f, 1.0f, 0.3f, 1.0f);
  //pVerts = pkGeomObject->GetVerts();

  //r = 130.0f;
  //a = 1.0f;

  //fGradScale = 360.0f/(NUM_RANGE_VERTICES/2 - 1);
  //for(i = 0; i < (NUM_RANGE_VERTICES/2 - 1); ++i)
  //{
  //  pVerts[2*i  ] = __SVSPVertex(      r*cos((D3DX_PI/180.0f)*i*fGradScale),       r*sin((D3DX_PI/180.0f)*i*fGradScale), 0.0f);
  //  pVerts[2*i+1] = __SVSPVertex((r - a)*cos((D3DX_PI/180.0f)*i*fGradScale), (r - a)*sin((D3DX_PI/180.0f)*i*fGradScale), 0.0f);
  //}
  //pVerts[2*i  ] = pVerts[0];
  //pVerts[2*i+1] = pVerts[1];
  //m_ppkGeomObjects[GEOM_OBJECT_RANGE] = pkGeomObject;

  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINESTRIP, NUM_RANGE_VERTICES);
  pkGeomObject->SetMaterial(0.3f, 1.0f, 0.3f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  r = (FLOAT)FCR::TEN_MILES_CIRCLE_RANGE;

  fGradScale = 360.0f/(NUM_RANGE_VERTICES - 1);
  for(i = 0; i < (NUM_RANGE_VERTICES - 1); ++i)
  {
    pVerts[i] = __SVSPVertex(r*cos((D3DX_PI/180.0f)*i*fGradScale), r*sin((D3DX_PI/180.0f)*i*fGradScale), 0.0f, 0, 0, __SVSPColor::dwACTIVEGUICOLOR);
  }
  pVerts[NUM_RANGE_VERTICES - 1] = pVerts[0];
  m_ppkGeomObjects[GEOM_OBJECT_RANGE] = pkGeomObject;


  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_HSI_SCALE
  ///////////////////////////////////////////////////////////////////
  FLOAT r1 = (FLOAT)FCR::TEN_MILES_CIRCLE_RANGE;
  FLOAT r2 = (FLOAT)FCR::TEN_MILES_CIRCLE_RANGE - 10; 
        a  = 1.0f;
  FLOAT fDegrees;
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINELIST, NUM_HSI_SCALE_VERTS);
  pkGeomObject->SetMaterial(0.3f, 1.0f, 0.3f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  fGradScale = 10.0f;
  for(i = 0; i < NUM_HSI_SCALE_VERTS/4; ++i)
  {
    fDegrees = fGradScale + 3*i*fGradScale;
    pVerts[4*i    ] = __SVSPVertex(r1*sin((D3DX_PI/180.0f)*fDegrees),           r1*cos((D3DX_PI/180.0f)*fDegrees),           0, 0, 0, __SVSPColor::dwACTIVEGUICOLOR);
    pVerts[4*i + 1] = __SVSPVertex(r2*sin((D3DX_PI/180.0f)*fDegrees),           r2*cos((D3DX_PI/180.0f)*fDegrees),           0, 0, 0, __SVSPColor::dwACTIVEGUICOLOR);
    pVerts[4*i + 2] = __SVSPVertex(r1*sin((D3DX_PI/180.0f)*(fDegrees + 10.0f)), r1*cos((D3DX_PI/180.0f)*(fDegrees + 10.0f)), 0, 0, 0, __SVSPColor::dwACTIVEGUICOLOR);
    pVerts[4*i + 3] = __SVSPVertex(r2*sin((D3DX_PI/180.0f)*(fDegrees + 10.0f)), r2*cos((D3DX_PI/180.0f)*(fDegrees + 10.0f)), 0, 0, 0, __SVSPColor::dwACTIVEGUICOLOR);
  }

  FLOAT r3 = r2 - 5;
  for(i = 0; i < (sizeof(m_akHSIHeading)/sizeof(HEADING)); ++i)
  {
    fDegrees = i*360.0f/(sizeof(m_akHSIHeading)/sizeof(HEADING));
    m_akHSIHeading[i].v = D3DXVECTOR3(r3*sin((D3DX_PI/180.0f)*fDegrees), r3*cos((D3DX_PI/180.0f)*fDegrees), 0); 
    switch((UINT)fDegrees)
    {
    case 0:
      sprintf_s(&m_akHSIHeading[i].s[0], sizeof(m_akHSIHeading[i].s), "%c", 'N');
      break;
    case 90:
      sprintf_s(&m_akHSIHeading[i].s[0], sizeof(m_akHSIHeading[i].s), "%c", 'E');
      break;
    case 180:
      sprintf_s(&m_akHSIHeading[i].s[0], sizeof(m_akHSIHeading[i].s), "%c", 'S');
      break;
    case 270:
      sprintf_s(&m_akHSIHeading[i].s[0], sizeof(m_akHSIHeading[i].s), "%c", 'W');
      break;
    default:
      sprintf_s(&m_akHSIHeading[i].s[0], sizeof(m_akHSIHeading[i].s), "%0.2d", ((UINT)fDegrees)/10);
      break;
    }
  }
  m_ppkGeomObjects[GEOM_OBJECT_HSI_SCALE] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_TARGET_DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_TARGET_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  FLOAT fTargetScale = 32;
  pVerts[0] = __SVSPVertex(-fTargetScale/2,  fTargetScale/2, 0, 0, 0, __SVSPColor::dwGUICOLOR);
  pVerts[1] = __SVSPVertex( fTargetScale/2,  fTargetScale/2, 0, 1, 0, __SVSPColor::dwGUICOLOR);
  pVerts[2] = __SVSPVertex(-fTargetScale/2, -fTargetScale/2, 0, 0, 1, __SVSPColor::dwGUICOLOR);
  pVerts[3] = __SVSPVertex( fTargetScale/2, -fTargetScale/2, 0, 1, 1, __SVSPColor::dwGUICOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_TARGET_DESIGNATOR] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_TARGET_DESIGNATOR] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/FCR/RES/TEXTURES/TARGET.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_SELECTED_TARGET_DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_SELECTED_TARGET_DESIGNATOR_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-fTargetScale/2,  fTargetScale/2, 0, 0, 0, __SVSPColor::dwACTIVEGUICOLOR);
  pVerts[1] = __SVSPVertex( fTargetScale/2,  fTargetScale/2, 0, 1, 0, __SVSPColor::dwACTIVEGUICOLOR);
  pVerts[2] = __SVSPVertex(-fTargetScale/2, -fTargetScale/2, 0, 0, 1, __SVSPColor::dwACTIVEGUICOLOR);
  pVerts[3] = __SVSPVertex( fTargetScale/2, -fTargetScale/2, 0, 1, 1, __SVSPColor::dwACTIVEGUICOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_SELECTED_TARGET_DESIGNATOR] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_BACKGROUND
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_BACKGROUND_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  AAVArea<FLOAT> kBackgroundArea = AAVArea<FLOAT>(1012, 1012);
  pVerts[0] = __SVSPVertex(-kBackgroundArea.tWidth/2,  kBackgroundArea.tHeight/2, 0.01f, 0, 0, __SVSPColor::dwBACKGROUNDCOLOR);
  pVerts[1] = __SVSPVertex( kBackgroundArea.tWidth/2,  kBackgroundArea.tHeight/2, 0.01f, 1, 0, __SVSPColor::dwBACKGROUNDCOLOR);
  pVerts[2] = __SVSPVertex(-kBackgroundArea.tWidth/2, -kBackgroundArea.tHeight/2, 0.01f, 0, 1, __SVSPColor::dwBACKGROUNDCOLOR);
  pVerts[3] = __SVSPVertex( kBackgroundArea.tWidth/2, -kBackgroundArea.tHeight/2, 0.01f, 1, 1, __SVSPColor::dwBACKGROUNDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_BACKGROUND] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/FCR/RES/TEXTURES/BACKGROUND.dds");


  FLOAT fHalfWidth = 0.025f;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FCR_CONE_DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_FCR_CONE_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(0.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  r = 4.8f;
  a = 2*fHalfWidth;

  fGradScale = 360.0f/(NUM_FCR_CONE_DESIGNATOR_VERTICES/2 - 1);
  for(i = 0; i < (NUM_FCR_CONE_DESIGNATOR_VERTICES/2 - 1); ++i)
  {
    pVerts[2*i  ] = __SVSPVertex(      r*cos((D3DX_PI/180.0f)*i*fGradScale),       r*sin((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[2*i+1] = __SVSPVertex((r - a)*cos((D3DX_PI/180.0f)*i*fGradScale), (r - a)*sin((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  }
  pVerts[2*i  ] = pVerts[0];
  pVerts[2*i+1] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_FCR_CONE_DESIGNATOR] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FCR_HEADING_DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_FCR_HEADING_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-0.4f,  0.4f, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 0.4f,  0.4f, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-0.4f, -0.4f, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( 0.4f, -0.4f, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_FCR_HEADING_DESIGNATOR] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_FCR_HEADING_DESIGNATOR] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/HUD/RES/TEXTURES/RHDG.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_TIRIS_DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_TIRIS_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(0.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  r = 2.2f;

  fGradScale = 360.0f/(NUM_TIRIS_DESIGNATOR_VERTICES/2 - 1);
  for(i = 0; i < (NUM_TIRIS_DESIGNATOR_VERTICES/2 - 1); ++i)
  {
    pVerts[2*i  ] = __SVSPVertex(      r*cos((D3DX_PI/180.0f)*i*fGradScale),       r*sin((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[2*i+1] = __SVSPVertex((r - a)*cos((D3DX_PI/180.0f)*i*fGradScale), (r - a)*sin((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  }
  pVerts[2*i  ] = pVerts[0];
  pVerts[2*i+1] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_TIRIS_DESIGNATOR] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // LOCKED TARGET DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_LOCKED_TARGET_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(0.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  pVerts[0] = __SVSPVertex(-0.6f - fHalfWidth, -0.6f - fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex(-0.6f + fHalfWidth, -0.6f + fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[2] = __SVSPVertex(-0.6f - fHalfWidth,  0.6f + fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex(-0.6f + fHalfWidth,  0.6f - fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[4] = __SVSPVertex( 0.6f + fHalfWidth,  0.6f + fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[5] = __SVSPVertex( 0.6f - fHalfWidth,  0.6f - fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[6] = __SVSPVertex( 0.6f + fHalfWidth, -0.6f - fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[7] = __SVSPVertex( 0.6f - fHalfWidth, -0.6f + fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[8] = pVerts[0];
  pVerts[9] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_LOCKED_TARGET_DESIGNATOR] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // LOCKED TARGET DESIGNATOR 2
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_LOCKED_TARGET_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(0.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  pVerts[0] = __SVSPVertex(-0.6f - fHalfWidth, -0.6f - fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex(-0.6f + fHalfWidth, -0.6f + fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[2] = __SVSPVertex(-0.6f - fHalfWidth,  0.6f + fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex(-0.6f + fHalfWidth,  0.6f - fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[4] = __SVSPVertex( 0.6f + fHalfWidth,  0.6f + fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[5] = __SVSPVertex( 0.6f - fHalfWidth,  0.6f - fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[6] = __SVSPVertex( 0.6f + fHalfWidth, -0.6f - fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[7] = __SVSPVertex( 0.6f - fHalfWidth, -0.6f + fHalfWidth, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[8] = pVerts[0];
  pVerts[9] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_LOCKED_TARGET_DESIGNATOR_2] = pkGeomObject;


  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_LOCKED_TARGET_ASPECT
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_LOCKED_TARGET_ASPECT_VERTICES);
  pkGeomObject->SetMaterial(0.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  r = 1.1f;

  fGradScale = 360.0f/((NUM_LOCKED_TARGET_ASPECT_VERTICES - 4)/2 - 1);
  for(i = 0; i < ((NUM_LOCKED_TARGET_ASPECT_VERTICES - 4)/2 - 1); ++i)
  {
    pVerts[2*i  ] = __SVSPVertex(      r*cos((D3DX_PI/180.0f)*i*fGradScale),       r*sin((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[2*i+1] = __SVSPVertex((r - a)*cos((D3DX_PI/180.0f)*i*fGradScale), (r - a)*sin((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  }
  pVerts[2*i  ] = pVerts[0];
  pVerts[2*i+1] = pVerts[1];

  pVerts[NUM_LOCKED_TARGET_ASPECT_VERTICES - 4] = __SVSPVertex( -fHalfWidth,       r,        0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[NUM_LOCKED_TARGET_ASPECT_VERTICES - 3] = __SVSPVertex(  fHalfWidth,       r,        0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[NUM_LOCKED_TARGET_ASPECT_VERTICES - 2] = __SVSPVertex( -fHalfWidth,       r + 0.4f, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[NUM_LOCKED_TARGET_ASPECT_VERTICES - 1] = __SVSPVertex(  fHalfWidth,       r + 0.4f, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  m_ppkGeomObjects[GEOM_OBJECT_LOCKED_TARGET_ASPECT] = pkGeomObject;

  m_kTargetAirspeedValueVector = D3DXVECTOR3(0, 0.8f, 0);
  m_kTargetAltitudeValueVector = D3DXVECTOR3(0, -0.8f, 0);




  ///////////////////////////////////////////////////////////////////
  // CLIPPING RECT
  ///////////////////////////////////////////////////////////////////
  m_kClippRect.left   = (LONG)(m_kWindowRect.kMatrix._41 - (m_kWindowRect.kArea.tWidth )/2 + 1024);
  m_kClippRect.right  = (LONG)(m_kWindowRect.kMatrix._41 + (m_kWindowRect.kArea.tWidth )/2 + 1024);
  m_kClippRect.top    = (LONG)(m_kWindowRect.kMatrix._42 + (m_kWindowRect.kArea.tHeight)/2 + 1024);
  m_kClippRect.bottom = (LONG)(m_kWindowRect.kMatrix._42 - (m_kWindowRect.kArea.tHeight)/2 + 1024);

  m_kGTSIndicatorVector    = D3DXVECTOR3(-2*m_kWindowRect.kArea.tWidth/7,    m_kWindowRect.kArea.tHeight/2 - kTouchRectArea.tHeight/2 - 3.0f,   0);
  m_kSPDIndicatorVector    = D3DXVECTOR3(-1*m_kWindowRect.kArea.tWidth/7,    m_kWindowRect.kArea.tHeight/2 - kTouchRectArea.tHeight/2 - 3.0f,   0);
  m_kHDGIndicatorVector    = D3DXVECTOR3( 0*m_kWindowRect.kArea.tWidth/7,    m_kWindowRect.kArea.tHeight/2 - kTouchRectArea.tHeight/2 - 3.0f,   0);
  m_kALTIndicatorVector    = D3DXVECTOR3( 1*m_kWindowRect.kArea.tWidth/7,    m_kWindowRect.kArea.tHeight/2 - kTouchRectArea.tHeight/2 - 3.0f,   0);
  m_kRNGIndicatorVector    = D3DXVECTOR3(  -FCR::DISPLAY_WINDOW_WIDTH/2 + kExtraOffsetVector.x,  1*FCR::DISPLAY_WINDOW_HEIGHT/7, 0);
  m_kRSTATIndicatorVector  = D3DXVECTOR3( 0,                                 m_kWindowRect.kArea.tHeight/2 - 2*kTouchRectArea.tHeight/2 - 3.0f, 0);
  m_kLATLONIndicatorVector = D3DXVECTOR3( 2*m_kWindowRect.kArea.tWidth/7,    m_kWindowRect.kArea.tHeight/2 - 2*kTouchRectArea.tHeight/2 - 3.0f, 0);


  //Subscribe to system events
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, FCR::EVENT_ID_OBJECT_ADDED,   "ObjectAdded");
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, FCR::EVENT_ID_OBJECT_REMOVED, "ObjectRemoved");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  //Define requested data
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "SIM ON GROUND",                       "Bool",            SIMCONNECT_DATATYPE_INT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "STRUCT LATLONALT",                    "radians",         SIMCONNECT_DATATYPE_LATLONALT);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "PLANE LATITUDE",                      "degrees",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "PLANE LONGITUDE",                     "degrees",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "PLANE ALTITUDE",                      "feet",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "VELOCITY BODY X",                     "Feet per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "VELOCITY BODY Y",                     "Feet per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "VELOCITY BODY Z",                     "Feet per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "PLANE HEADING DEGREES MAGNETIC",      "radians",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "AIRSPEED INDICATED",                  "knots",           SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FCR::AI_OBJECT_DATA_ID, "INDICATED ALTITUDE",                  "Feet",            SIMCONNECT_DATATYPE_INT32);
  
  ///////////////////////////////////////////////////////////////////
  // INIT FCR LISTS
  ///////////////////////////////////////////////////////////////////
  InitObjectsList();

  m_pkProcessedObject = 0;

  m_apkProcessStateCallback[FCR::FCR_STATE_OFF]     = &FCR::ProcessStateOff;
  m_apkProcessStateCallback[FCR::FCR_STATE_ON]      = &FCR::ProcessStateOn;
  m_apkProcessStateCallback[FCR::FCR_STATE_LOCKED]  = &FCR::ProcessStateLocked;

  m_eState = FCR::FCR_STATE_OFF;
}

FCR::~FCR()
{
  __SVSPModule::Stop();

  //Unsubscribe from system events
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, FCR::EVENT_ID_OBJECT_ADDED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, FCR::EVENT_ID_OBJECT_REMOVED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

VOID CALLBACK  FCR__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  FCR *__THIS = static_cast<FCR *>(pContext);
  if(!__THIS->m_bAbort)
  {
    __THIS->DispatchProcess(pData, cbData);
  }
}

VOID FCR::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
{
  switch(pData->dwID)
  {
  case SIMCONNECT_RECV_ID_EVENT_OBJECT_ADDREMOVE:
    {
      SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE *pEvt = (SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE*)pData;
      switch(pEvt->uEventID)
      {
      case FCR::EVENT_ID_OBJECT_ADDED:
        if((pEvt->eObjType == SIMCONNECT_SIMOBJECT_TYPE_AIRCRAFT) || (pEvt->eObjType == SIMCONNECT_SIMOBJECT_TYPE_BOAT) || (pEvt->eObjType == SIMCONNECT_SIMOBJECT_TYPE_GROUND))
        {
          AddObject(pEvt->dwData);
        }
        break;

      case FCR::EVENT_ID_OBJECT_REMOVED:
        if((m_pkTarget != NULL) && (m_pkTarget->dwObjectID == pEvt->dwData))
        {
          m_pkTarget = NULL;
        }
        RemoveObject(pEvt->dwData);
        break;

      default:
        break;
      }
      break;
    }
    break;

  case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
    {
      SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
      if(pObjData->dwRequestID == FCR::AI_OBJECT_DATA_REQUEST_ID)
      {
        __SVSPModule::READ_OBJECT_DATA *pData = (__SVSPModule::READ_OBJECT_DATA*)&pObjData->dwData;

        //TODO: Switch algorithm to splay tree with linked objects list.
        if(pObjData->dwObjectID < FCR::MAX_NUM_OBJECTS)
        {
          __SVSPModule::OBJECT *pkObject = &m_akObjects[pObjData->dwObjectID];
          memcpy(&(pkObject->kData), pData, sizeof(__SVSPModule::READ_OBJECT_DATA));

          //SetEvent(m_hUpdateObjectDataEvent);
        }
      }
    }
    break;

  case SIMCONNECT_RECV_ID_EXCEPTION:
    {
      CHAR acStr[64];
      SIMCONNECT_RECV_EXCEPTION *pObjData = (SIMCONNECT_RECV_EXCEPTION *)pData;
      sprintf_s(&acStr[0], sizeof(acStr), "EXCEPTION_ID = %d", pObjData->dwException);
      __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
    }
    break;

  default:
    break;
  }
}

VOID FCR::UpdateTargetBearingData()
{
  AAV_ASSERT_RETURN_VOID(m_pkTarget != NULL);

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  // Slant range (Meters)
  FLOAT fSlantRangeMeters                   = D3DXVec3Length(&m_pkTarget->kLocalOffsetVector);
  // fSlantRange (Miles)
  m_pkTarget->fSlantRangeMiles              = 0.62137E-3f*fSlantRangeMeters;
  FLOAT fVelocity                           = (FLOAT)pkAC->m_dVelocity/3.28f; // m/s
  // Cosinus of a Target aspect anlge.
  D3DXVECTOR3 kBodyOffsetXZVector = *D3DXVec3TransformCoord(&kBodyOffsetXZVector, &m_pkTarget->kLocalOffsetVector, &pkAC->m_kLocalToBodyYMatrix);
  D3DXVECTOR3 kBodyOffsetXZVectorNormalized = *D3DXVec3Normalize(&kBodyOffsetXZVectorNormalized, &kBodyOffsetXZVector);
  FLOAT fCosOfSTPAspectAngle                = kBodyOffsetXZVectorNormalized.z;
  FLOAT fSinOfSTPAspectAngle                = kBodyOffsetXZVectorNormalized.x;
  m_pkTarget->fClosingInVelocity            = fVelocity*fCosOfSTPAspectAngle;
  // TIME-TO-INTERCEPT (TTI)
  m_pkTarget->usTTI                         = (m_pkTarget->fClosingInVelocity > 10.0f) ? (USHORT)(fSlantRangeMeters/m_pkTarget->fClosingInVelocity) : m_pkTarget->usTTI;
  m_pkTarget->usTTIMinutes                  = m_pkTarget->usTTI/60;
  m_pkTarget->usTTISeconds                  = m_pkTarget->usTTI%60;
  // DISTANCE-TO-INTERCEPT (DTI)
  m_pkTarget->uiDTI                         = (fmod(m_pkTarget->fSlantRangeMiles, 1.0f) < 0.5f) ? (UINT)m_pkTarget->fSlantRangeMiles : (UINT)m_pkTarget->fSlantRangeMiles + 1;
}


VOID FCR::ToggleUncageRadar()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC != NULL);

  if(m_pkLocalRadarDirectionMatrix == &m_kLocalAntenaMatrix)
  {
    m_pkLocalRadarDirectionMatrix = &pkAC->m_kBodyToLocalTransformMatrix;
    m_fMinObjectAquisitionAspect  = FCR::AQUISITION_ASPECT_UNCAGED;
  }
  else
  {
    m_pkLocalRadarDirectionMatrix = &m_kLocalAntenaMatrix;
    m_fMinObjectAquisitionAspect  = FCR::AQUISITION_ASPECT_CAGED;
  }
}

__SVSPModule::OBJECT* FCR::SelectNearestTarget() 
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN(pkAC  != NULL, NULL);

  AAV_ASSERT_RETURN(m_iNumObjects > 1, NULL);

  FLOAT fDistanceToPreviousTarget = FCR::MAX_LOCK_DISTANCE;

  D3DXVECTOR3  kLocalTargetDirectionVector;
  D3DXVECTOR3  kLocalTargetDirectionNormalizedVector;

  OBJECT *pkTarget = NULL;
  //First object in the linked FCR list is the user.
  OBJECT *pkObject = &m_akObjects[0];
  for(INT i = 0; i < m_iNumObjects - 1; ++i)
  {
    //Get next object from the linked FCR list.
    pkObject = pkObject->pkNextObject;
    D3DXVECTOR3  kGlobalTargetDirectionVector = (D3DXVECTOR3)(pkObject->kGlobalVector) - pkAC->m_kGlobalVector;
    FLOAT        fDistanceToCurrentTarget = D3DXVec3Length(&kGlobalTargetDirectionVector);

    if((fDistanceToCurrentTarget > FCR::MIN_LOCK_DISTANCE) && (fDistanceToCurrentTarget < FCR::MAX_LOCK_DISTANCE) && (fDistanceToCurrentTarget < fDistanceToPreviousTarget))
    {
      D3DXVec3TransformCoord(&kLocalTargetDirectionVector, &kGlobalTargetDirectionVector, &pkAC->m_kGlobalToLocalTransformMatrix);
      D3DXVec3Normalize(&kLocalTargetDirectionNormalizedVector, &kLocalTargetDirectionVector);

      // check whether the target is within the FCR cone.
      D3DXVECTOR3 m_kLocalRadarDirectionNormalizedVector;
      D3DXVec3TransformCoord(&m_kLocalRadarDirectionNormalizedVector, &UNIT_VECTOR, m_pkLocalRadarDirectionMatrix);
      m_kLocalRadarDirectionNormalizedVector.x = -m_kLocalRadarDirectionNormalizedVector.x;
      FLOAT fAngularAspectOfCurrentTarget = D3DXVec3Dot(&m_kLocalRadarDirectionNormalizedVector, &kLocalTargetDirectionNormalizedVector);
      if(fAngularAspectOfCurrentTarget > m_fMinObjectAquisitionAspect)
      {
        fDistanceToPreviousTarget      = fDistanceToCurrentTarget;
        pkTarget                       = pkObject;
      }
    }
  }

  return pkTarget;
}

VOID FCR::RotateAntenaTowardViewDirection(LPD3DXDEVICE pkDevice)
{
  //Get current view matrix
  D3DXMATRIXA16 kViewMatrix;
  D3DXMATRIXA16 kTransposedViewMatrix;
  __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalGetViewMatrix(pkDevice, &kViewMatrix);
  kViewMatrix._41 = kViewMatrix._14 = 0;
  kViewMatrix._42 = kViewMatrix._24 = 0;
  kViewMatrix._43 = kViewMatrix._34 = 0;
  D3DXMatrixTranspose(&kTransposedViewMatrix, &kViewMatrix);

  ///////////////////////////////////////////////////////////
  //All rotations are performed in the local reference system.
  ///////////////////////////////////////////////////////////

#ifdef REALISTIC_ANTENA_ROTATION
  D3DXVECTOR3   kLocalViewDirectionNormalizedVector;
  D3DXVECTOR3   kLocalAntenaRotationVector;
  D3DXVECTOR3   kLocalAntenaRotationNormalizedVector;
  D3DXVECTOR3   kLocalNewAntenaDirectionVector;
  D3DXMATRIX    kLocalAntenaRotationMatrix;

  //Get local view direction vector.
  D3DXVec3TransformCoord(&kLocalViewDirectionNormalizedVector, &UNIT_VECTOR, &kTransposedViewMatrix);

  kLocalViewDirectionNormalizedVector.x = -kLocalViewDirectionNormalizedVector.x;

  //Get the normal vector of the plane defined by view direction and that of the Antena itself.
  D3DXVec3Cross(&kLocalAntenaRotationVector, &kLocalViewDirectionNormalizedVector, &m_kLocalAntenaDirectionNormalizedVector);
  D3DXVec3Normalize(&kLocalAntenaRotationNormalizedVector, &kLocalAntenaRotationVector);

  FLOAT fCosOfAntenaAspectAngle = D3DXVec3Dot(&kLocalViewDirectionNormalizedVector, &m_kLocalAntenaDirectionNormalizedVector);
  FLOAT fAntenaRotationAngle = -0.02f*(1 - 1/(1 + 15250.0f*(1 - fCosOfAntenaAspectAngle)));

  //Get Antena rotation matrix.
  __SVSPModule::GetRotationAroundUnitVector(&kLocalAntenaRotationMatrix, &kLocalAntenaRotationNormalizedVector, fAntenaRotationAngle);
  //Rotate Antena toward view vector.
  D3DXVec3TransformCoord(&kLocalNewAntenaDirectionVector, &m_kLocalAntenaDirectionNormalizedVector, &kLocalAntenaRotationMatrix);
  //Normalize Antena direction vector again to asure its unit length after each rotation.
  D3DXVec3Normalize(&m_kLocalAntenaDirectionNormalizedVector, &kLocalNewAntenaDirectionVector);

  //Get Antena direction matrix for local reference system.
  __SVSPModule::GetTransformMatrixFromLocalNormalizedVector(&m_kLocalAntenaMatrix, &m_kLocalAntenaDirectionNormalizedVector);
#else
  m_kLocalAntenaMatrix = kTransposedViewMatrix;
#endif //REALISTIC_ANTENA_ROTATION
}

VOID FCR::RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  CHAR acStr[32];
  AAVVector3<FLOAT> kInterpolatedGlobalTargetVector;
  D3DXVECTOR3        kGlobalTargetOffsetVector;
  DOUBLE             dDeltaTime;
  D3DXMATRIXA16      mTrans;
  D3DXMATRIXA16      kLockedStateTransformMatrix;

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  __SVSPFont*      pkFont       = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;

  //FLOAT fScaling;
  D3DXMATRIX mDistScalingMatrix;
  switch(m_eState)
  {
  case FCR::FCR_STATE_OFF:
    break;

  case FCR::FCR_STATE_ON:
    AAV_ASSERT_RETURN_VOID(m_pkTarget == NULL);
    if(m_pkLocalRadarDirectionMatrix == &m_kLocalAntenaMatrix)
    {
      // Uncaged FCR mode
      RotateAntenaTowardViewDirection(pkDevice);
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_TIRIS_DESIGNATOR];
      AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
      pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*(*m_pkLocalRadarDirectionMatrix)));
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->Render(pkDevice);
    }
    else
    {
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_FCR_CONE_DESIGNATOR];
      AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
      pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*(*m_pkLocalRadarDirectionMatrix)));
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->Render(pkDevice);

      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_FCR_HEADING_DESIGNATOR];
      AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
      pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_FCR_HEADING_DESIGNATOR]);
      pkGeomObject->Render(pkDevice);
    }
    break;

  case FCR::FCR_STATE_LOCKED:
    AAV_ASSERT_RETURN_VOID(m_pkTarget != NULL);

    //Interpolated direction
    dDeltaTime = GetSystemTime() - m_pkTarget->dLastUpdateTime;
    kInterpolatedGlobalTargetVector = m_pkTarget->kGlobalVector + m_pkTarget->kGlobalVelocity*dDeltaTime;
    kGlobalTargetOffsetVector = (D3DXVECTOR3)kInterpolatedGlobalTargetVector - pkAC->m_kGlobalVector;
    D3DXVec3TransformCoord(&m_kLocalTargetDirectionVector, &kGlobalTargetOffsetVector, &pkAC->m_kGlobalToLocalTransformMatrix);
    D3DXVec3Normalize(&m_kLocalTargetDirectionNormalizedVector, &m_kLocalTargetDirectionVector);
    GetTransformMatrixFromLocalNormalizedVector(&mTrans, &m_kLocalTargetDirectionNormalizedVector);

    // Differentiate Airborne, Naval or Ground Targets
    if(!m_pkTarget->kData.bOnGround)
    {
      kLockedStateTransformMatrix = pkAC->m_kBankRotationMatrix*rkOPMatrix*mTrans;

      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_LOCKED_TARGET_DESIGNATOR_2];
      AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
      pkGeomObject->SetTransform(pkDevice, &kLockedStateTransformMatrix);
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->Render(pkDevice);

      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_LOCKED_TARGET_ASPECT];
      AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
      //pkGeomObject->SetTransform(pkDevice, &(pkAC->m_kBankRotationMatrix*rkOPMatrix*mTrans));
      pkGeomObject->SetTransform(pkDevice, &kLockedStateTransformMatrix);
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->RenderPrimitives(pkDevice, 0, (FCR::NUM_LOCKED_TARGET_ASPECT_VERTICES - 4) - 2, true);
      pkGeomObject->RenderPrimitives(pkDevice, (FCR::NUM_LOCKED_TARGET_ASPECT_VERTICES - 4), 2, true);

      //LOCKED TARGET INDICATED AIRSPEED
      sprintf_s(&acStr[0], sizeof(acStr), "%d", (INT)(m_pkTarget->kData.dAirSpeed));
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kTargetAirspeedValueVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y);
      //LOCKED TARGET INDICATED ALTITUDE
      sprintf_s(&acStr[0], sizeof(acStr), "%#5.3f", (FLOAT)(m_pkTarget->kData.dAltitude/1000));
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kTargetAltitudeValueVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y);
    }
    else
    {
      kLockedStateTransformMatrix = pkAC->m_kBankRotationMatrix*rkOPMatrix*mTrans;

      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_LOCKED_TARGET_DESIGNATOR_2];
      AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
      pkGeomObject->SetTransform(pkDevice, &kLockedStateTransformMatrix);
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->Render(pkDevice);
    }
    break;

  default:
    break;
  }
}

VOID FCR::RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  __SVSPFont* pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  switch(m_eDisplayState)
  {
  case FCR::DISPLAY_STATE_MAIN:
    {
      AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
      AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
      WPN *pkWPN = static_cast<WPN *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_WPN));
      MDA *pkMDA = static_cast<MDA *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_MDA));


      LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
      LPD3DXTEXTUREOBJECT  pkTextureObject = NULL;

      D3DXMATRIXA16 matHeading;
      D3DXMATRIXA16 matHeadingNames;
      D3DXMATRIXA16 kObjectOffsetMatrix;
      //D3DXMatrixRotationZ(&m_kHSIRotationMatrix, pkAC->m_kBodyAttitude.y);
      D3DXMatrixRotationZ(&m_kHSIRotationMatrix, pkAC->m_kData.m_dIndicatedMagneticHeading);
      D3DXMatrixTranspose(&m_kHSIRotationTransposedMatrix, &m_kHSIRotationMatrix);

      // Set Scissor Rect
      m_kClippRect.left   = (LONG)(rkOPMatrix._41 - (m_kWindowRect.kArea.tWidth )/2 + 1024);
      m_kClippRect.right  = (LONG)(rkOPMatrix._41 + (m_kWindowRect.kArea.tWidth )/2 + 1024);
      m_kClippRect.top    = (LONG)(rkOPMatrix._42 - (m_kWindowRect.kArea.tHeight)/2 + 1024);
      m_kClippRect.bottom = (LONG)(rkOPMatrix._42 + (m_kWindowRect.kArea.tHeight)/2 + 1024);

      __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalSetScissorRect(pkDevice, &m_kClippRect);

      D3DXMATRIXA16 kCenterPointMatrix = m_kOPMatrix*rkOPMatrix;

      ///////////////////////////////////////////////////////////////////
      // GEOM_OBJECT_BACKGROUND
      ///////////////////////////////////////////////////////////////////
      //pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND];
      //AAV_ASSERT_RETURN_VOID(pkGeomObject  != NULL);
      //pkGeomObject->SetTransform(pkDevice, &kCenterPointMatrix);
      //pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_BACKGROUND]);
      //pkGeomObject->Render(pkDevice);

      ///////////////////////////////////////////////////////////////////
      // GEOM_OBJECT_CENTER_POINT_DESIGNATOR
      ///////////////////////////////////////////////////////////////////
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_CENTER_POINT_DESIGNATOR];
      AAV_ASSERT_RETURN_VOID(pkGeomObject  != NULL);
      pkGeomObject->SetTransform(pkDevice, &kCenterPointMatrix);
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->Render(pkDevice);

      ///////////////////////////////////////////////////////////////////
      // GEOM_OBJECT_RANGE
      ///////////////////////////////////////////////////////////////////
      //pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_RANGE];
      //AAV_ASSERT_RETURN_VOID(pkGeomObject  != NULL);
      //pkGeomObject->SetTexture(pkDevice, NULL);
      //for(UINT i = 0; i < FCR::MAX_NUM_RANGES; ++i)
      //{
      //  pkGeomObject->SetTransform(pkDevice, &(m_kHSIScalingMatrix*m_akRangeScalingMatrix[i]*kCenterPointMatrix));
      //  pkGeomObject->Render(pkDevice);
      //}

      ///////////////////////////////////////////////////////////////////
      // GEOM_OBJECT_HSI_SCALE
      ///////////////////////////////////////////////////////////////////
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND];
      AAV_ASSERT_RETURN_VOID(pkGeomObject  != NULL);
      pkGeomObject->SetTransform(pkDevice, &(m_kHSIRotationMatrix*kCenterPointMatrix));
      pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_BACKGROUND]);
      pkGeomObject->Render(pkDevice);

      // HSI HEADING MARKS
      D3DXVECTOR3 v = D3DXVECTOR3(0.0f, 0.0f, 0);
      for(UINT i = 0; i < (sizeof(m_akHSIHeading)/sizeof(HEADING)); ++i)
      {
        HEADING *pkHeading = &m_akHSIHeading[i];
        D3DXMatrixTranslation(&kObjectOffsetMatrix, pkHeading->v.x*m_kOPMatrix._11, pkHeading->v.y*m_kOPMatrix._22, 0.0f );
        pkGeomObject->SetTransform(pkDevice, &(m_kHSIRotationTransposedMatrix*kObjectOffsetMatrix*m_kHSIRotationMatrix*kCenterPointMatrix));
        pkFont->RenderTextExt(pkDevice, &pkHeading->s[0], &v, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 1, __SVSPColor::dwACTIVEGUICOLOR);
      }

      ///////////////////////////////////////////////////////////////////
      // GEOM_OBJECT_TARGET_DESIGNATOR
      ///////////////////////////////////////////////////////////////////
      LPD3DXGEOMETRYOBJECT pkGeomObjectRadarObjectDesignator = m_ppkGeomObjects[GEOM_OBJECT_TARGET_DESIGNATOR];
      AAV_ASSERT_RETURN_VOID(pkGeomObjectRadarObjectDesignator  != NULL);
      LPD3DXGEOMETRYOBJECT pkGeomObjectSelectedObjectDesignator = m_ppkGeomObjects[GEOM_OBJECT_SELECTED_TARGET_DESIGNATOR];
      AAV_ASSERT_RETURN_VOID(pkGeomObjectSelectedObjectDesignator  != NULL);

      //Get first object from the radar list.
      OBJECT *pkObject = m_akObjects[0].pkNextObject;
      AAV_ASSERT_RETURN_VOID(pkObject != NULL);
      D3DXVECTOR3 kBodyOffsetVector;
      D3DXVECTOR3 kBodyOffsetVectorNauticalMiles;

      //Set radar obejct texture.
      pkGeomObjectRadarObjectDesignator->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_TARGET_DESIGNATOR]);
      pkGeomObjectSelectedObjectDesignator->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_TARGET_DESIGNATOR]);

      for(INT i = 0; i < m_iNumObjects; ++i)
      {
        D3DXVec3TransformCoord(&kBodyOffsetVector, &pkObject->kLocalOffsetVector, &pkAC->m_kYawRotationMatrix);
        D3DXVECTOR3 kBodyOffsetVectorNauticalMiles = kBodyOffsetVector/1852;
        D3DXVECTOR3 kBodyOffsetVectorPixels = m_fCurrentNumPixelsInNauticalMile*kBodyOffsetVectorNauticalMiles;
        D3DXMatrixTranslation(&kObjectOffsetMatrix, -kBodyOffsetVectorPixels.x, kBodyOffsetVectorPixels.z, 0);

        pkGeomObject = (pkObject == m_pkTarget) ? pkGeomObjectSelectedObjectDesignator : pkGeomObjectRadarObjectDesignator;
        pkGeomObject->SetTransform(pkDevice, &(pkObject->kHeadingMatrix*kObjectOffsetMatrix*kCenterPointMatrix));
        pkGeomObject->Render(pkDevice);

        //sprintf_s(&m_acStr[0], sizeof(m_acStr), "%d", i);
        //pkFont->RenderText((&m_acStr[0]), (D3DXVECTOR3 *)&UNIT_VECTOR, __SVSPFont::FILTERED|__SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f);
        //sprintf_s(&m_acStr[0], sizeof(m_acStr), "  %3.1f", D3DXVec3Length(&kGlobalVec)/1852.0f);
        //pkFont->RenderText((&m_acStr[0]), (D3DXVECTOR3 *)&UNIT_VECTOR, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y, 0.667f);
        pkObject = pkObject->pkNextObject;
      }

      ///////////////////////////////////////////////////////////////////
      // WPN
      ///////////////////////////////////////////////////////////////////
      if(pkWPN != NULL)
      {
        pkWPN->RenderOnHDD(pkDevice, kCenterPointMatrix, m_fCurrentNumPixelsInNauticalMile);
      }

      ///////////////////////////////////////////////////////////////////
      // MDA
      ///////////////////////////////////////////////////////////////////
      //if(pkMDA != NULL)
      //{
      //  pkMDA->Render( kCenterPointMatrix, m_fCurrentScaleFactor);
      //}

      ///////////////////////////////////////////////////////////////////
      // FCR
      ///////////////////////////////////////////////////////////////////
      //if(pkFCR != NULL)
      //{
      //  pkFCR->Render( kCenterPointMatrix, m_fCurrentScaleFactor);
      //}

      // Reset Scissor Rect
      __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalSetScissorRect(pkDevice, NULL);

      CHAR acStr[64];
      pkFont->SetTransform(pkDevice, &rkOPMatrix);

      sprintf_s(&acStr[0], sizeof(acStr), "GTS %03d", (UINT)pkAC->m_kData.m_dGroundTrackSpeed);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kGTSIndicatorVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);

      sprintf_s(&acStr[0], sizeof(acStr), "SPD %03d", (UINT)pkAC->m_kData.m_dIndicatedAirspeed);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kSPDIndicatorVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);

      sprintf_s(&acStr[0], sizeof(acStr), "HDG %03d", (UINT)(180.0*pkAC->m_kData.m_dIndicatedMagneticHeading/D3DX_PI));
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kHDGIndicatorVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);

      sprintf_s(&acStr[0], sizeof(acStr), "ALT %#5.3f", pkAC->m_dRadarAltitude/1000.0f);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kALTIndicatorVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);

      sprintf_s(&acStr[0], sizeof(acStr), "%d", (INT)m_fCurrentRadarRangeMiles);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kRNGIndicatorVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 1.0f, __SVSPColor::dwGUICOLOR);

      if(pkWPN != NULL)
      {
        sprintf_s(&acStr[0], sizeof(acStr), "RSTAT %s", pkWPN->m_bEnroute ? "ENROUTE" : "OFFROUTE");
        pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kRSTATIndicatorVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);
      }

      sprintf_s(&acStr[0], sizeof(acStr), "LAT %02.4f  LON %02.4f", pkAC->m_kLocation.Latitude, pkAC->m_kLocation.Longitude);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kLATLONIndicatorVector, __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);
    }
    break;
  }



  __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}

BOOL FCR::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  switch(dwEvent)
  {
  case FCR::EVENT_ID_HDG:
    break;

  case FCR::EVENT_ID_RANGE_DEC:
    if(m_uiCurrentRadarRangeIndex > 0)
    {
      SetRadarRange(--m_uiCurrentRadarRangeIndex);
    }
    break;

  case FCR::EVENT_ID_ZOOM_IN:
    {
      m_fCurrentZoomFactor          = (m_fCurrentZoomFactor < 5) ? m_fCurrentZoomFactor + 1 : m_fCurrentZoomFactor;
      D3DXMATRIX kTranslationMatrix = *D3DXMatrixTranslation(&kTranslationMatrix, 0, -50*m_fCurrentZoomFactor*m_fCurrentZoomFactor, 0);
      D3DXMATRIX kScalingMatrix     = *D3DXMatrixScaling    (&kScalingMatrix, m_fCurrentZoomFactor, m_fCurrentZoomFactor, 1);
      m_kOPMatrix                   = kTranslationMatrix*kScalingMatrix;
    }
    break;

  case FCR::EVENT_ID_ZOOM_OUT:
    {
      m_fCurrentZoomFactor          = (m_fCurrentZoomFactor > 1) ? m_fCurrentZoomFactor - 1 : m_fCurrentZoomFactor;
      D3DXMATRIX kTranslationMatrix = *D3DXMatrixTranslation(&kTranslationMatrix, 0, -50*m_fCurrentZoomFactor*m_fCurrentZoomFactor, 0);
      D3DXMATRIX kScalingMatrix     = *D3DXMatrixScaling    (&kScalingMatrix, m_fCurrentZoomFactor, m_fCurrentZoomFactor, 1);
      m_kOPMatrix                   = kTranslationMatrix*kScalingMatrix;
    }
    break;

  case FCR::EVENT_ID_RANGE_INC:
    if(m_uiCurrentRadarRangeIndex < (FCR::MAX_NUM_RADAR_RANGES - 1))
    {
      SetRadarRange(++m_uiCurrentRadarRangeIndex);
    }
    break;

  default:
    __SVSPModule::EventHandler(dwEvent, dwEventData);
    break;
  }
  return false;
}

VOID FCR::InitObjectsList()
{
  memset(&m_akObjects[0], 0, sizeof(m_akObjects));

  //Set first object to be user aircraft and link it to itself.
  m_akObjects[0].pkPrevObject = &m_akObjects[0];
  m_akObjects[0].pkNextObject = &m_akObjects[0];
  m_iNumObjects   = 1;
}

VOID FCR::AddObject(DWORD dwObjectID)
{
  AAV_ASSERT_RETURN_VOID((dwObjectID != 0) && (dwObjectID < FCR::MAX_NUM_OBJECTS));
  if(m_iNumObjects < FCR::MAX_NUM_OBJECTS)
  {
    //Get object from the pull.
    OBJECT *pkObject = &m_akObjects[dwObjectID];

    //Get first object from the radar list.
    OBJECT *pkUserAcObject = &m_akObjects[0];

    //Check link consistency.
    AAV_ASSERT_RETURN_VOID(pkUserAcObject->pkPrevObject != NULL);
    AAV_ASSERT_RETURN_VOID(pkUserAcObject->pkPrevObject->pkNextObject == pkUserAcObject);

    //Link new object at the end of the radar list.
    pkUserAcObject->pkPrevObject->pkNextObject = pkObject;
    pkObject->pkPrevObject = pkUserAcObject->pkPrevObject;
    pkObject->pkNextObject = pkUserAcObject;
    pkUserAcObject->pkPrevObject = pkObject;

    //Set object ID.
    pkObject->dwObjectID = dwObjectID;

    m_iNumObjects++;
  }
}

VOID FCR::RemoveObject(DWORD dwObjectID)
{
  AAV_ASSERT_RETURN_VOID((dwObjectID != 0) && (dwObjectID < FCR::MAX_NUM_OBJECTS));
  //TODO: Switch algorithm to splay tree with linked objects list.
  AAV_ASSERT_RETURN_VOID(m_iNumObjects > 0);

  //Get object from the pull.
  OBJECT *pkObject = &m_akObjects[dwObjectID];
  if(pkObject->dwObjectID == dwObjectID)
  {
    //Get previous and next objects in the radar list.
    OBJECT *pkPrevObject = pkObject->pkPrevObject;
    OBJECT *pkNextObject = pkObject->pkNextObject;

    //Check link consistency.
    AAV_ASSERT_RETURN_VOID(pkPrevObject != NULL);
    AAV_ASSERT_RETURN_VOID(pkPrevObject->pkNextObject == pkObject);
    AAV_ASSERT_RETURN_VOID(pkNextObject != NULL);
    AAV_ASSERT_RETURN_VOID(pkNextObject->pkPrevObject == pkObject);

    // Unlink object from the radar list.
    pkPrevObject->pkNextObject = pkNextObject;
    pkNextObject->pkPrevObject = pkPrevObject;
    pkObject->pkPrevObject = NULL;
    pkObject->pkNextObject = NULL;

    //Reset object ID.
    pkObject->dwObjectID = 0;

    m_iNumObjects--;
  }
}

VOID FCR::SetRadarRange(UINT uiRadarRangeIndex)
{
  AAV_ASSERT_RETURN_VOID(uiRadarRangeIndex < FCR::MAX_NUM_RADAR_RANGES);
  m_uiCurrentRadarRangeIndex        = uiRadarRangeIndex;
  m_fCurrentRadarRangeMiles         = g_afRadarRangeTables[m_uiCurrentRadarRangeIndex];
  m_fCurrentScaleFactor             = INITIAL_FCR_RANGE_MILES/m_fCurrentRadarRangeMiles;
  m_fCurrentNumPixelsInNauticalMile = FCR::INITIAL_NUM_PIXELS_IN_NAUTICAL_MILE*m_fCurrentScaleFactor;
}

VOID FCR::Start()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  // Set local FCR direction to aircraft head-on.
  m_pkLocalRadarDirectionMatrix = &pkAC->m_kBodyToLocalTransformMatrix;
  m_fMinObjectAquisitionAspect  = FCR::AQUISITION_ASPECT_UNCAGED;

  m_pkProcessedObject = &m_akObjects[0];

  __SVSPModule::Start();
}

HRESULT FCR::ProcessStateOff()
{
  MSG kMsg;
  AAV_ASSERT_RETURN(m_pkTarget == NULL, S_FALSE);

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case FCR::MESSAGE_ON_VISUAL_RANGE:
      m_fCurrentRadarRangeMiles = 30; //Miles
      m_eState = FCR::FCR_STATE_ON;
      break;

    case FCR::MESSAGE_ON_BEYOND_VISUAL_RANGE:
      m_fCurrentRadarRangeMiles = 150; //Miles
      m_eState = FCR::FCR_STATE_ON;
      break;

    case FCR::MESSAGE_SET_RADAR_RANGE_INDEX:
      SetRadarRange(kMsg.wParam);
      break;
    }
  }
  return S_OK;
}

HRESULT FCR::ProcessStateOn()
{
  MSG kMsg;
  AAV_ASSERT_RETURN(m_pkTarget == NULL, S_FALSE);

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case FCR::MESSAGE_OFF:
      m_pkTarget = NULL;
      m_eState = FCR::FCR_STATE_OFF;
      break;

    case FCR::MESSAGE_TOGGLE_UNCAGE_FCR:
      ToggleUncageRadar();
      break;

    case FCR::MESSAGE_TOGGLE_LOCK_ON_NEAREST_TARGET:
      m_pkTarget = SelectNearestTarget();
      if(m_pkTarget != NULL)
      {
        m_eState = FCR::FCR_STATE_LOCKED;
      }
      break;

    case FCR::MESSAGE_SET_RADAR_RANGE_INDEX:
      SetRadarRange(kMsg.wParam);
      break;
    }
  }
  return S_OK;
}

HRESULT FCR::ProcessStateLocked()
{
  MSG kMsg;
  AAV_ASSERT_RETURN(m_pkTarget != NULL, S_FALSE);

  // Update Target Bearing Data.
  UpdateTargetBearingData();

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case FCR::MESSAGE_OFF:
      m_pkTarget = NULL;
      m_eState   = FCR::FCR_STATE_OFF;
      break;

    case FCR::MESSAGE_TOGGLE_LOCK_ON_NEAREST_TARGET:
      m_pkTarget = NULL;
      m_eState   = FCR::FCR_STATE_ON;
      break;

    case FCR::MESSAGE_SET_RADAR_RANGE_INDEX:
      SetRadarRange(kMsg.wParam);
      break;
    }
  }
  return S_OK;
}

VOID FCR::Process()
{

  AC *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC != NULL);

  if(m_iNumObjects > 0)
  {
    if((m_pkProcessedObject == NULL) || (m_pkProcessedObject->pkNextObject == NULL))
    {
      //Object has been invalidated. Reset update chain to the first element.
      m_pkProcessedObject = &m_akObjects[0];
    }

    //Track locked object every frame.
    if((m_pkTarget != NULL) && (m_pkTarget != m_pkProcessedObject))
    {
      m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, FCR::AI_OBJECT_DATA_REQUEST_ID, FCR::AI_OBJECT_DATA_ID, m_pkTarget->dwObjectID, SIMCONNECT_PERIOD_ONCE);
    }

    //Do not track the user aircraft.
    if(m_pkProcessedObject->dwObjectID != 0)
    {
      ///////////////////////////////////////////////////////////////////
      //Interpolation objects data.
      ///////////////////////////////////////////////////////////////////

      //Update Global Vector
      AAVVector3<FLOAT> kPreviousGlobalVector               = m_pkProcessedObject->kGlobalVector;
      m_pkProcessedObject->kGlobalVector                     = m_pkProcessedObject->kData.kLocation.GetVector3<FLOAT>();

      //Update Global Velocity
      AAVVector3<FLOAT> kGlobalDisplacementVector           = m_pkProcessedObject->kGlobalVector - kPreviousGlobalVector;
      AAVVector3<FLOAT> kGlobalDisplacementVectorNormalized = kGlobalDisplacementVector.Normalize();
      FLOAT              dTrueSpeed                          = (m_pkProcessedObject->kData.kBodyVelocity.Length())/3.2808f; // m/s
      m_pkProcessedObject->kGlobalVelocity                   = kGlobalDisplacementVectorNormalized*dTrueSpeed;   // m/s

      //Update Local Offset Vector
      D3DXVECTOR3 kGlobalOffsetVec                           = (D3DXVECTOR3)m_pkProcessedObject->kGlobalVector - pkAC->m_kGlobalVector;
      D3DXVec3TransformCoord(&m_pkProcessedObject->kLocalOffsetVector, &kGlobalOffsetVec, &pkAC->m_kGlobalToLocalTransformMatrix);
      //m_pkProcessedObject->kLocalOffsetVector.x            = -m_pkProcessedObject->kLocalOffsetVector.x;
      D3DXMatrixRotationZ(&m_pkProcessedObject->kHeadingMatrix, (pkAC->m_kData.m_dIndicatedMagneticHeading - m_pkProcessedObject->kData.dHeading));

      //Update Timings
      m_pkProcessedObject->dLastUpdateTime                   = GetSystemTime();


      //TODO:Check wether the current object location is within the radar conus

      m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, FCR::AI_OBJECT_DATA_REQUEST_ID, FCR::AI_OBJECT_DATA_ID, m_pkProcessedObject->dwObjectID, SIMCONNECT_PERIOD_ONCE);
    }

    ///////////////////////////////////////////////////////////////////
    //Query next object data.
    ///////////////////////////////////////////////////////////////////
    m_pkProcessedObject = m_pkProcessedObject->pkNextObject;
  }

  AAV_ASSERT(m_eState < FCR::FCR_STATE_MAX);
  HRESULT (__thiscall FCR::*__ProcessStateCallback)(VOID) = m_apkProcessStateCallback[m_eState];
  AAV_ASSERT(__ProcessStateCallback != NULL);
  (this->*__ProcessStateCallback)();

}

