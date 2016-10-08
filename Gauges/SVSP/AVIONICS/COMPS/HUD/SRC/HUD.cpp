#include "__SVSPAvionics.h"
#include "AC.h"
#include "HUD.h"
#include "HUD.h"
#include "FLCS.h"
#include "ECS.h"
#include "SIM.h"
#include "SMS.h"
#include "WPN.h"
#include "VWS.h"
#include "FCR.h"
#include "HOTAS.h"


const UINT  HUD::AIRSPEED_SCALE_MODULO                = 50;
const UINT  HUD::ALTITUDE_SCALE_MODULO                = 100;
const UINT  HUD::HEADING_SCALE_MODULO                 = 10;
const FLOAT HUD::OFFSET_X                             = 5.75f;
const FLOAT HUD::cMaxFlightPathMarkerDeviationAngle   = D3DX_PI*1.0f;//*1.95f;

static const FLOAT fBankTop                           = 0.15f;
static const FLOAT fBankMiddle                        = 0.10f;
static const FLOAT fHeadingBottom                     = 0;
static const FLOAT fHeadingMiddle                     = fHeadingBottom + 0.1f;
static const FLOAT fHeadingTop                        = fHeadingBottom + 0.2f;
static const FLOAT fHeadingDisp                       = 0.8f;
static const FLOAT c_fScaleDisp                       = 0.25f;
static const FLOAT c_fAGLScaleDisp                    = 0.30f;
static const FLOAT c_fAirspeedScaleDispMod            = ((FLOAT)HUD::AIRSPEED_SCALE_MODULO)/4.0f;
static const FLOAT c_fMSLAltitudeScaleDispMod         = ((FLOAT)HUD::ALTITUDE_SCALE_MODULO)/4.0f;
static const FLOAT c_fHeadingScaleDispMod             = ((FLOAT)HUD::HEADING_SCALE_MODULO)/2.0f;
static const FLOAT fHalfLineThickness                 = 0.025f;
static const FLOAT fHeadingVisLimit                   = 0.8f*fHeadingDisp;


const D3DXVECTOR2 HUD::g_kMaxFlightDirectionArea = D3DXVECTOR2(0.12f, 0.12f);

const D3DXVECTOR3             g_kAGLAltitudeScaleVector = D3DXVECTOR3(2.9f, 2.9f, 0);
const HUD::AGL_ALTITUDE_SCALE g_akAGLAltitudeScaleTable[HUD::NUM_AGL_ALTITUDE_SCALE_TABLE_ENTRIES] =
{
  {D3DXVECTOR3(g_kAGLAltitudeScaleVector.x, g_kAGLAltitudeScaleVector.y -  0*c_fAGLScaleDisp, 0), "1.5"},
  {D3DXVECTOR3(g_kAGLAltitudeScaleVector.x, g_kAGLAltitudeScaleVector.y -  1*c_fAGLScaleDisp, 0), "1.0"},
  {D3DXVECTOR3(g_kAGLAltitudeScaleVector.x, g_kAGLAltitudeScaleVector.y -  3*c_fAGLScaleDisp, 0), " .8",},
  {D3DXVECTOR3(g_kAGLAltitudeScaleVector.x, g_kAGLAltitudeScaleVector.y -  5*c_fAGLScaleDisp, 0), " .6",},
  {D3DXVECTOR3(g_kAGLAltitudeScaleVector.x, g_kAGLAltitudeScaleVector.y -  7*c_fAGLScaleDisp, 0), " .4",},
  {D3DXVECTOR3(g_kAGLAltitudeScaleVector.x, g_kAGLAltitudeScaleVector.y -  9*c_fAGLScaleDisp, 0), " .2",},
  {D3DXVECTOR3(g_kAGLAltitudeScaleVector.x, g_kAGLAltitudeScaleVector.y - 11*c_fAGLScaleDisp, 0), " .0", },
};

HUD::HUD(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("HUD", pkAvionics, dwThreadSleepTime, false, NULL, NULL, GEOM_OBJECT_NUM_OBJECTS, NULL, TEXTURE_OBJECT_NUM_OBJECTS),
  m_fAngle(0),
  m_uiNumStencilTextures(0)
{
  LPD3DXGEOMETRYOBJECT pkGeomObject    = NULL;
  __SVSPVertex*        pVerts          = NULL;

  const FLOAT fVerticalOffset = 0.4f;

  D3DXMatrixIdentity(&m_matIdentity);
  D3DXMatrixIdentity(&m_matHeading);
  D3DXMatrixIdentity(&m_matWayPoints);

  D3DXVECTOR3 v;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_HUD_STENCIL_CIRCLED
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_HUD_STENCIL_CIRCLED_VERTS);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  DWORD HUDStencilColor =  ((0x10 << 24) | __SVSPColor::dwHUDCOLOR & 0x00FFFFFF);
  FLOAT fR = 7.5f;
  FLOAT fGradScale = 360.0f/(NUM_HUD_STENCIL_CIRCLED_VERTS/3);
  for(UINT i = 0; i < NUM_HUD_STENCIL_CIRCLED_VERTS/3; ++i)
  {
    FLOAT fAngleStart = (D3DX_PI/180.0f)*fGradScale*i;
    FLOAT fAngleEnd   = (D3DX_PI/180.0f)*fGradScale*(i + 1);
    pVerts[3*i+0] = __SVSPVertex(0,                   0,                   0, 0, -1.7f, HUDStencilColor);
    pVerts[3*i+1] = __SVSPVertex(fR*cos(fAngleStart), fR*sin(fAngleStart), 0, 0, -1.7f, HUDStencilColor);
    pVerts[3*i+2] = __SVSPVertex(fR*cos(fAngleEnd),   fR*sin(fAngleEnd),   0, 0, -1.7f, HUDStencilColor);
  }
  m_ppkGeomObjects[GEOM_OBJECT_HUD_STENCIL_CIRCLED] = pkGeomObject;
  D3DXMatrixScaling    (&m_kStencilScalingMatrix,    1.0f, 1.0f, 1);
  D3DXMatrixScaling    (&m_kHUDStencilScalingMatrix, 1.0f, 1.0f, 1);
  D3DXMatrixScaling    (&m_kHMDStencilScalingMatrix, 1.5f, 1.5f, 1);

  /////////////////////////////////////////////////////////
  // NOSE DIRECTION MARKER
  /////////////////////////////////////////////////////////
  FLOAT a1 = 0.05f, a2 = 0.2f;
  m_kNoseDirectionMarkerVector = D3DXVECTOR3(0, 2.0f, 0);
  m_kHeadingValueVector = D3DXVECTOR3(0, -1.3f, 0);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_NOSE_DIRECTION_MARKER_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0 ] = __SVSPVertex(-a2                 + m_kNoseDirectionMarkerVector.x, -fHalfLineThickness + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1 ] = __SVSPVertex(-a1                 + m_kNoseDirectionMarkerVector.x,  fHalfLineThickness + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[2 ] = __SVSPVertex(-a2                 + m_kNoseDirectionMarkerVector.x,  fHalfLineThickness + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3 ] = pVerts[0 ];
  pVerts[4 ] = __SVSPVertex(-a1                 + m_kNoseDirectionMarkerVector.x, -fHalfLineThickness + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[5 ] = pVerts[1 ];

  pVerts[6 ] = __SVSPVertex(-fHalfLineThickness + m_kNoseDirectionMarkerVector.x,  a1                 + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[7 ] = __SVSPVertex( fHalfLineThickness + m_kNoseDirectionMarkerVector.x,  a1                 + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[8 ] = __SVSPVertex( fHalfLineThickness + m_kNoseDirectionMarkerVector.x,  a2                 + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[9 ] = pVerts[6 ];
  pVerts[10] = pVerts[8 ];
  pVerts[11] = __SVSPVertex(-fHalfLineThickness + m_kNoseDirectionMarkerVector.x,  a2                 + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[12] = __SVSPVertex( a2                 + m_kNoseDirectionMarkerVector.x, -fHalfLineThickness + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[13] = __SVSPVertex( a1                 + m_kNoseDirectionMarkerVector.x,  fHalfLineThickness + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[14] = __SVSPVertex( a2                 + m_kNoseDirectionMarkerVector.x,  fHalfLineThickness + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[15] = pVerts[12];
  pVerts[16] = __SVSPVertex( a1                 + m_kNoseDirectionMarkerVector.x, -fHalfLineThickness + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[17] = pVerts[13];

  pVerts[18] = __SVSPVertex(-fHalfLineThickness + m_kNoseDirectionMarkerVector.x, -a1                 + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[19] = __SVSPVertex( fHalfLineThickness + m_kNoseDirectionMarkerVector.x, -a1                 + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[20] = __SVSPVertex( fHalfLineThickness + m_kNoseDirectionMarkerVector.x, -a2                 + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[21] = pVerts[18];
  pVerts[22] = pVerts[20];
  pVerts[23] = __SVSPVertex(-fHalfLineThickness + m_kNoseDirectionMarkerVector.x, -a2                 + m_kNoseDirectionMarkerVector.y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_NOSE_DIRECTION_MARKER] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // HEADING BOX
  /////////////////////////////////////////////////////////
  m_kHeadingValueVector = D3DXVECTOR3(0, -1.3f, 0);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_HEADING_BOX_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-0.8f,       m_kHeadingValueVector.y + 0.4f, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 0.8f,       m_kHeadingValueVector.y + 0.4f, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-0.8f,       m_kHeadingValueVector.y - 0.4f, 0, 0.0f, 0.5f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( 0.8f,       m_kHeadingValueVector.y - 0.4f, 0, 1.0f, 0.5f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_HEADING_BOX] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_HDG] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/HUD/RES/TEXTURES/HDG.dds");

  /////////////////////////////////////////////////////////
  // AIRSPEED BOX
  /////////////////////////////////////////////////////////
  v = D3DXVECTOR3(-OFFSET_X, 0, 0);
  m_kAirspeedValueVector = v + D3DXVECTOR3(1.12f, 0, 0);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_AIRSPEED_BOX_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(v.x        - 0.2f,  0.4f, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex(v.x + 1.6f - 0.2f,  0.4f, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(v.x        - 0.2f, -0.4f, 0, 0.0f, 0.5f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex(v.x + 1.6f - 0.2f, -0.4f, 0, 1.0f, 0.5f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_AIRSPEED_BOX] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_ALT_KIAS] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/HUD/RES/TEXTURES/ALT_KIAS.dds");

  /////////////////////////////////////////////////////////
  // MSL ALTITUDE BOX
  /////////////////////////////////////////////////////////
  v = D3DXVECTOR3(OFFSET_X, 0, 0);
  m_kMSLAltitudeValueVector = v + D3DXVECTOR3(0.05f, 0, 0);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_MSL_ALTITUDE_BOX_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(v.x - 1.6f + 0.2f,  0.4f, 0, 0.0f, 0.5f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex(v.x        + 0.2f,  0.4f, 0, 1.0f, 0.5f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(v.x - 1.6f + 0.2f, -0.4f, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex(v.x        + 0.2f, -0.4f, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_MSL_ALTITUDE_BOX] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // AGL ALTITUDE BOX
  /////////////////////////////////////////////////////////
  v = D3DXVECTOR3(4.0f, -1.9f, 0);
  m_kAGLAltitudeValueVector = v + D3DXVECTOR3(0.6f, 0, 0);
  m_kAGLDesignatorVector    = m_kAGLAltitudeValueVector + D3DXVECTOR3(-1.55f, 0, 0);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_AGL_ALTITUDE_BOX_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-0.8f + v.x,  0.4f + v.y, 0, 0.0f, 0.5f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 0.8f + v.x,  0.4f + v.y, 0, 1.0f, 0.5f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-0.8f + v.x, -0.4f + v.y, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( 0.8f + v.x, -0.4f + v.y, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_AGL_ALTITUDE_BOX] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // LOW ALTITUDE
  /////////////////////////////////////////////////////////
  m_kALDesignatorVector = m_kAGLDesignatorVector + D3DXVECTOR3(0, -fVerticalOffset, 0);
  m_kALValueVector      = m_kAGLAltitudeValueVector + D3DXVECTOR3(0, -fVerticalOffset, 0);


  /////////////////////////////////////////////////////////
  // MASTER ARMAMENT INDICATOR
  /////////////////////////////////////////////////////////
  m_kMasterModeStateIndicationVector  = D3DXVECTOR3(-m_kAGLAltitudeValueVector.x + 0.5f, m_kAGLAltitudeValueVector.y, m_kAGLAltitudeValueVector.z);

  /////////////////////////////////////////////////////////
  // MACH
  /////////////////////////////////////////////////////////
  m_kMachValueVector   = m_kMasterModeStateIndicationVector + D3DXVECTOR3(0, -fVerticalOffset, 0);

  /////////////////////////////////////////////////////////
  // G
  /////////////////////////////////////////////////////////
  m_kGValueVector   = D3DXVECTOR3(m_kMachValueVector.x, -m_kMachValueVector.y, m_kMachValueVector.z);

  /////////////////////////////////////////////////////////
  // AoA
  /////////////////////////////////////////////////////////
  m_kAoAValueVector = m_kMachValueVector + D3DXVECTOR3(-0.2f, -fVerticalOffset, 0);

  /////////////////////////////////////////////////////////
  // MAX G
  /////////////////////////////////////////////////////////
  m_kMaxGValueVector = m_kAoAValueVector + D3DXVECTOR3(0, -fVerticalOffset, 0);

  /////////////////////////////////////////////////////////
  // FUEL WARNING
  /////////////////////////////////////////////////////////
  m_kFuelWarningVector = m_kMaxGValueVector + D3DXVECTOR3(0, -fVerticalOffset, 0);

  /////////////////////////////////////////////////////////
  // AIRBRAKE WARNING
  /////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINESTRIP, NUM_AIRBRAKE_BOX_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-0.35f, 0.4f,  0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 0.0f,  0.95f, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex( 0.35f, 0.4f,  0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = pVerts[0]; 
  m_ppkGeomObjects[GEOM_OBJECT_AIRBRAKE_BOX] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // HEADING SCALE
  /////////////////////////////////////////////////////////
  m_kHeadingScaleVector          = D3DXVECTOR3( 0, -0.8f, 0);
  m_akHeadingScaleValueVector[0] = D3DXVECTOR3( 0,              -0.6f, 0);
  m_akHeadingScaleValueVector[1] = D3DXVECTOR3(-2*fHeadingDisp, -0.6f, 0);
  m_akHeadingScaleValueVector[2] = D3DXVECTOR3( 2*fHeadingDisp, -0.6f, 0);

  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_HEADING_SCALE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0 ] = __SVSPVertex(-3*fHeadingDisp - fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1 ] = __SVSPVertex(-3*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[2 ] = __SVSPVertex(-3*fHeadingDisp + fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3 ] = __SVSPVertex(-3*fHeadingDisp + fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[4 ] = __SVSPVertex(-3*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[5 ] = __SVSPVertex(-3*fHeadingDisp + fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[6 ] = __SVSPVertex(-2*fHeadingDisp - fHalfLineThickness, -fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[7 ] = __SVSPVertex(-2*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[8 ] = __SVSPVertex(-2*fHeadingDisp + fHalfLineThickness, -fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[9 ] = __SVSPVertex(-2*fHeadingDisp + fHalfLineThickness, -fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);//----
  pVerts[10] = __SVSPVertex(-2*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[11] = __SVSPVertex(-2*fHeadingDisp + fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[12] = __SVSPVertex(-1*fHeadingDisp - fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[13] = __SVSPVertex(-1*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[14] = __SVSPVertex(-1*fHeadingDisp + fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[15] = __SVSPVertex(-1*fHeadingDisp + fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[16] = __SVSPVertex(-1*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[17] = __SVSPVertex(-1*fHeadingDisp + fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[18] = __SVSPVertex(                - fHalfLineThickness, -fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[19] = __SVSPVertex(                - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[20] = __SVSPVertex(                + fHalfLineThickness, -fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[21] = __SVSPVertex(                + fHalfLineThickness, -fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);//----
  pVerts[22] = __SVSPVertex(                - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[23] = __SVSPVertex(                + fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[24] = __SVSPVertex( 1*fHeadingDisp - fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[25] = __SVSPVertex( 1*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[26] = __SVSPVertex( 1*fHeadingDisp + fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[27] = __SVSPVertex( 1*fHeadingDisp + fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[28] = __SVSPVertex( 1*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[29] = __SVSPVertex( 1*fHeadingDisp + fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[30] = __SVSPVertex( 2*fHeadingDisp - fHalfLineThickness, -fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[31] = __SVSPVertex( 2*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[32] = __SVSPVertex( 2*fHeadingDisp + fHalfLineThickness, -fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[33] = __SVSPVertex( 2*fHeadingDisp + fHalfLineThickness, -fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);//----
  pVerts[34] = __SVSPVertex( 2*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[35] = __SVSPVertex( 2*fHeadingDisp + fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[36] = __SVSPVertex( 3*fHeadingDisp - fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[37] = __SVSPVertex( 3*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[38] = __SVSPVertex( 3*fHeadingDisp + fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[39] = __SVSPVertex( 3*fHeadingDisp + fHalfLineThickness, -fHeadingMiddle, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[40] = __SVSPVertex( 3*fHeadingDisp - fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[41] = __SVSPVertex( 3*fHeadingDisp + fHalfLineThickness, -fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  //stright line
  pVerts[42] = __SVSPVertex(                - fHalfLineThickness, -0.5f -1.5f*fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[43] = __SVSPVertex(                - fHalfLineThickness, -0.5f -1.5f*fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[44] = __SVSPVertex(                + fHalfLineThickness, -0.5f -1.5f*fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[45] = __SVSPVertex(                + fHalfLineThickness, -0.5f -1.5f*fHeadingTop,    0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[46] = __SVSPVertex(                - fHalfLineThickness, -0.5f -1.5f*fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[47] = __SVSPVertex(                + fHalfLineThickness, -0.5f -1.5f*fHeadingBottom, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  m_ppkGeomObjects[GEOM_OBJECT_HEADING_SCALE] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // AIRSPEED SCALE
  /////////////////////////////////////////////////////////
  m_kAirspeedScaleVector          = m_kAirspeedValueVector + D3DXVECTOR3(0.45f, 0, 0);
  m_akAirspeedScaleValueVector[0] = D3DXVECTOR3(-0.3f,  0,              0);
  m_akAirspeedScaleValueVector[1] = D3DXVECTOR3(-0.3f,  4*c_fScaleDisp, 0);
  m_akAirspeedScaleValueVector[2] = D3DXVECTOR3(-0.3f, -4*c_fScaleDisp, 0);

  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_AIRSPEED_SCALE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0 ] = __SVSPVertex(-fHeadingMiddle, 7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1 ] = __SVSPVertex(-fHeadingBottom, 7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[2 ] = __SVSPVertex(-fHeadingMiddle, 7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3 ] = __SVSPVertex(-fHeadingMiddle, 7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[4 ] = __SVSPVertex(-fHeadingBottom, 7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[5 ] = __SVSPVertex(-fHeadingBottom, 7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[6 ] = __SVSPVertex(-fHeadingMiddle, 6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[7 ] = __SVSPVertex(-fHeadingBottom, 6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[8 ] = __SVSPVertex(-fHeadingMiddle, 6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[9 ] = __SVSPVertex(-fHeadingMiddle, 6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[10] = __SVSPVertex(-fHeadingBottom, 6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[11] = __SVSPVertex(-fHeadingBottom, 6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[12] = __SVSPVertex(-fHeadingMiddle, 5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[13] = __SVSPVertex(-fHeadingBottom, 5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[14] = __SVSPVertex(-fHeadingMiddle, 5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[15] = __SVSPVertex(-fHeadingMiddle, 5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[16] = __SVSPVertex(-fHeadingBottom, 5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[17] = __SVSPVertex(-fHeadingBottom, 5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[18] = __SVSPVertex(-fHeadingTop,    4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[19] = __SVSPVertex(-fHeadingBottom, 4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[20] = __SVSPVertex(-fHeadingTop,    4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//----
  pVerts[21] = __SVSPVertex(-fHeadingTop,    4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[22] = __SVSPVertex(-fHeadingBottom, 4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[23] = __SVSPVertex(-fHeadingBottom, 4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[24] = __SVSPVertex(-fHeadingMiddle, 3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[25] = __SVSPVertex(-fHeadingBottom, 3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[26] = __SVSPVertex(-fHeadingMiddle, 3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[27] = __SVSPVertex(-fHeadingMiddle, 3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[28] = __SVSPVertex(-fHeadingBottom, 3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[29] = __SVSPVertex(-fHeadingBottom, 3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[30] = __SVSPVertex(-fHeadingMiddle, 2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[31] = __SVSPVertex(-fHeadingBottom, 2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[32] = __SVSPVertex(-fHeadingMiddle, 2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[33] = __SVSPVertex(-fHeadingMiddle, 2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[34] = __SVSPVertex(-fHeadingBottom, 2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[35] = __SVSPVertex(-fHeadingBottom, 2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[36] = __SVSPVertex(-fHeadingMiddle, 1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[37] = __SVSPVertex(-fHeadingBottom, 1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[38] = __SVSPVertex(-fHeadingMiddle, 1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[39] = __SVSPVertex(-fHeadingMiddle, 1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[40] = __SVSPVertex(-fHeadingBottom, 1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[41] = __SVSPVertex(-fHeadingBottom, 1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[42] = __SVSPVertex(-fHeadingTop,                  - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[43] = __SVSPVertex(-fHeadingBottom,               - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[44] = __SVSPVertex(-fHeadingTop,                  + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[45] = __SVSPVertex(-fHeadingTop,                  + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//----
  pVerts[46] = __SVSPVertex(-fHeadingBottom,               - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[47] = __SVSPVertex(-fHeadingBottom,               + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[48] = __SVSPVertex(-fHeadingMiddle, -1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[49] = __SVSPVertex(-fHeadingBottom, -1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[50] = __SVSPVertex(-fHeadingMiddle, -1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[51] = __SVSPVertex(-fHeadingMiddle, -1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[52] = __SVSPVertex(-fHeadingBottom, -1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[53] = __SVSPVertex(-fHeadingBottom, -1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[54] = __SVSPVertex(-fHeadingMiddle, -2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[55] = __SVSPVertex(-fHeadingBottom, -2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[56] = __SVSPVertex(-fHeadingMiddle, -2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[57] = __SVSPVertex(-fHeadingMiddle, -2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[58] = __SVSPVertex(-fHeadingBottom, -2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[59] = __SVSPVertex(-fHeadingBottom, -2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[60] = __SVSPVertex(-fHeadingMiddle, -3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[61] = __SVSPVertex(-fHeadingBottom, -3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[62] = __SVSPVertex(-fHeadingMiddle, -3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[63] = __SVSPVertex(-fHeadingMiddle, -3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[64] = __SVSPVertex(-fHeadingBottom, -3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[65] = __SVSPVertex(-fHeadingBottom, -3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[66] = __SVSPVertex(-fHeadingTop,    -4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[67] = __SVSPVertex(-fHeadingBottom, -4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[68] = __SVSPVertex(-fHeadingTop,    -4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[69] = __SVSPVertex(-fHeadingTop,    -4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//----
  pVerts[70] = __SVSPVertex(-fHeadingBottom, -4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[71] = __SVSPVertex(-fHeadingBottom, -4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[72] = __SVSPVertex(-fHeadingMiddle, -5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[73] = __SVSPVertex(-fHeadingBottom, -5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[74] = __SVSPVertex(-fHeadingMiddle, -5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[75] = __SVSPVertex(-fHeadingMiddle, -5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[76] = __SVSPVertex(-fHeadingBottom, -5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[77] = __SVSPVertex(-fHeadingBottom, -5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[78] = __SVSPVertex(-fHeadingMiddle, -6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[79] = __SVSPVertex(-fHeadingBottom, -6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[80] = __SVSPVertex(-fHeadingMiddle, -6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[81] = __SVSPVertex(-fHeadingMiddle, -6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[82] = __SVSPVertex(-fHeadingBottom, -6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[83] = __SVSPVertex(-fHeadingBottom, -6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[84] = __SVSPVertex(-fHeadingMiddle, -7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[85] = __SVSPVertex(-fHeadingBottom, -7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[86] = __SVSPVertex(-fHeadingMiddle, -7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[87] = __SVSPVertex(-fHeadingMiddle, -7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[88] = __SVSPVertex(-fHeadingBottom, -7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[89] = __SVSPVertex(-fHeadingBottom, -7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  m_ppkGeomObjects[GEOM_OBJECT_SPEED_SCALE] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // ALTITUDE SCALE
  /////////////////////////////////////////////////////////
  m_kMSLAltitudeScaleVector          = m_kMSLAltitudeValueVector + D3DXVECTOR3(-1.65f, 0, 0);
  m_akMSLAltitudeScaleValueVector[0] = D3DXVECTOR3(0.3f,  0,            0);
  m_akMSLAltitudeScaleValueVector[1] = D3DXVECTOR3(0.3f,  4*c_fScaleDisp, 0);
  m_akMSLAltitudeScaleValueVector[2] = D3DXVECTOR3(0.3f, -4*c_fScaleDisp, 0);

  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_ALTITUDE_SCALE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0 ] = __SVSPVertex(fHeadingMiddle, 7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1 ] = __SVSPVertex(fHeadingBottom, 7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[2 ] = __SVSPVertex(fHeadingMiddle, 7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3 ] = __SVSPVertex(fHeadingMiddle, 7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[4 ] = __SVSPVertex(fHeadingBottom, 7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[5 ] = __SVSPVertex(fHeadingBottom, 7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[6 ] = __SVSPVertex(fHeadingMiddle, 6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[7 ] = __SVSPVertex(fHeadingBottom, 6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[8 ] = __SVSPVertex(fHeadingMiddle, 6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[9 ] = __SVSPVertex(fHeadingMiddle, 6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[10] = __SVSPVertex(fHeadingBottom, 6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[11] = __SVSPVertex(fHeadingBottom, 6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[12] = __SVSPVertex(fHeadingMiddle, 5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[13] = __SVSPVertex(fHeadingBottom, 5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[14] = __SVSPVertex(fHeadingMiddle, 5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[15] = __SVSPVertex(fHeadingMiddle, 5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[16] = __SVSPVertex(fHeadingBottom, 5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[17] = __SVSPVertex(fHeadingBottom, 5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[18] = __SVSPVertex(fHeadingTop,    4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[19] = __SVSPVertex(fHeadingBottom, 4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[20] = __SVSPVertex(fHeadingTop,    4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//----
  pVerts[21] = __SVSPVertex(fHeadingTop,    4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[22] = __SVSPVertex(fHeadingBottom, 4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[23] = __SVSPVertex(fHeadingBottom, 4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[24] = __SVSPVertex(fHeadingMiddle, 3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[25] = __SVSPVertex(fHeadingBottom, 3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[26] = __SVSPVertex(fHeadingMiddle, 3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[27] = __SVSPVertex(fHeadingMiddle, 3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[28] = __SVSPVertex(fHeadingBottom, 3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[29] = __SVSPVertex(fHeadingBottom, 3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[30] = __SVSPVertex(fHeadingMiddle, 2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[31] = __SVSPVertex(fHeadingBottom, 2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[32] = __SVSPVertex(fHeadingMiddle, 2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[33] = __SVSPVertex(fHeadingMiddle, 2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[34] = __SVSPVertex(fHeadingBottom, 2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[35] = __SVSPVertex(fHeadingBottom, 2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[36] = __SVSPVertex(fHeadingMiddle, 1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[37] = __SVSPVertex(fHeadingBottom, 1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[38] = __SVSPVertex(fHeadingMiddle, 1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[39] = __SVSPVertex(fHeadingMiddle, 1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[40] = __SVSPVertex(fHeadingBottom, 1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[41] = __SVSPVertex(fHeadingBottom, 1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[42] = __SVSPVertex(fHeadingTop,                  - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[43] = __SVSPVertex(fHeadingBottom,               - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[44] = __SVSPVertex(fHeadingTop,                  + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[45] = __SVSPVertex(fHeadingTop,                  + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//----
  pVerts[46] = __SVSPVertex(fHeadingBottom,               - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[47] = __SVSPVertex(fHeadingBottom,               + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[48] = __SVSPVertex(fHeadingMiddle, -1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[49] = __SVSPVertex(fHeadingBottom, -1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[50] = __SVSPVertex(fHeadingMiddle, -1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[51] = __SVSPVertex(fHeadingMiddle, -1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[52] = __SVSPVertex(fHeadingBottom, -1*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[53] = __SVSPVertex(fHeadingBottom, -1*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[54] = __SVSPVertex(fHeadingMiddle, -2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[55] = __SVSPVertex(fHeadingBottom, -2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[56] = __SVSPVertex(fHeadingMiddle, -2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[57] = __SVSPVertex(fHeadingMiddle, -2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[58] = __SVSPVertex(fHeadingBottom, -2*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[59] = __SVSPVertex(fHeadingBottom, -2*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[60] = __SVSPVertex(fHeadingMiddle, -3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[61] = __SVSPVertex(fHeadingBottom, -3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[62] = __SVSPVertex(fHeadingMiddle, -3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[63] = __SVSPVertex(fHeadingMiddle, -3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[64] = __SVSPVertex(fHeadingBottom, -3*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[65] = __SVSPVertex(fHeadingBottom, -3*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[66] = __SVSPVertex(fHeadingTop,    -4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[67] = __SVSPVertex(fHeadingBottom, -4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[68] = __SVSPVertex(fHeadingTop,    -4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[69] = __SVSPVertex(fHeadingTop,    -4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//----
  pVerts[70] = __SVSPVertex(fHeadingBottom, -4*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[71] = __SVSPVertex(fHeadingBottom, -4*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[72] = __SVSPVertex(fHeadingMiddle, -5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[73] = __SVSPVertex(fHeadingBottom, -5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[74] = __SVSPVertex(fHeadingMiddle, -5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[75] = __SVSPVertex(fHeadingMiddle, -5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[76] = __SVSPVertex(fHeadingBottom, -5*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[77] = __SVSPVertex(fHeadingBottom, -5*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[78] = __SVSPVertex(fHeadingMiddle, -6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[79] = __SVSPVertex(fHeadingBottom, -6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[80] = __SVSPVertex(fHeadingMiddle, -6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[81] = __SVSPVertex(fHeadingMiddle, -6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[82] = __SVSPVertex(fHeadingBottom, -6*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[83] = __SVSPVertex(fHeadingBottom, -6*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[84] = __SVSPVertex(fHeadingMiddle, -7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[85] = __SVSPVertex(fHeadingBottom, -7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[86] = __SVSPVertex(fHeadingMiddle, -7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[87] = __SVSPVertex(fHeadingMiddle, -7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[88] = __SVSPVertex(fHeadingBottom, -7*c_fScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[89] = __SVSPVertex(fHeadingBottom, -7*c_fScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_ALTITUDE_SCALE] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // GEOM_OBJECT_AGL_ALTITUDE_SCALE
  /////////////////////////////////////////////////////////
  D3DXMatrixTranslation(&m_kAGLAltitudeScaleMatrix, g_kAGLAltitudeScaleVector.x - 0.35f, g_kAGLAltitudeScaleVector.y, 0);

  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_AGL_ALTITUDE_SCALE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  pVerts[0 ] = __SVSPVertex(fHeadingTop,     0*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1 ] = __SVSPVertex(fHeadingBottom,  0*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[2 ] = __SVSPVertex(fHeadingTop,     0*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3 ] = __SVSPVertex(fHeadingTop,     0*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//---- 1500
  pVerts[4 ] = __SVSPVertex(fHeadingBottom,  0*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[5 ] = __SVSPVertex(fHeadingBottom,  0*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[6 ] = __SVSPVertex(fHeadingTop,    -1*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[7 ] = __SVSPVertex(fHeadingBottom, -1*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[8 ] = __SVSPVertex(fHeadingTop,    -1*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[9 ] = __SVSPVertex(fHeadingTop,    -1*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//---- 1000
  pVerts[10] = __SVSPVertex(fHeadingBottom, -1*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[11] = __SVSPVertex(fHeadingBottom, -1*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[12] = __SVSPVertex(fHeadingMiddle, -2*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[13] = __SVSPVertex(fHeadingBottom, -2*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[14] = __SVSPVertex(fHeadingMiddle, -2*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[15] = __SVSPVertex(fHeadingMiddle, -2*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[16] = __SVSPVertex(fHeadingBottom, -2*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[17] = __SVSPVertex(fHeadingBottom, -2*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[18] = __SVSPVertex(fHeadingTop,    -3*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[19] = __SVSPVertex(fHeadingBottom, -3*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[20] = __SVSPVertex(fHeadingTop,    -3*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[21] = __SVSPVertex(fHeadingTop,    -3*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//---- 80
  pVerts[22] = __SVSPVertex(fHeadingBottom, -3*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[23] = __SVSPVertex(fHeadingBottom, -3*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[24] = __SVSPVertex(fHeadingMiddle, -4*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[25] = __SVSPVertex(fHeadingBottom, -4*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[26] = __SVSPVertex(fHeadingMiddle, -4*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[27] = __SVSPVertex(fHeadingMiddle, -4*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[28] = __SVSPVertex(fHeadingBottom, -4*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[29] = __SVSPVertex(fHeadingBottom, -4*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[30] = __SVSPVertex(fHeadingTop,    -5*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[31] = __SVSPVertex(fHeadingBottom, -5*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[32] = __SVSPVertex(fHeadingTop,    -5*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[33] = __SVSPVertex(fHeadingTop,    -5*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//---- 60
  pVerts[34] = __SVSPVertex(fHeadingBottom, -5*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[35] = __SVSPVertex(fHeadingBottom, -5*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[36] = __SVSPVertex(fHeadingMiddle, -6*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[37] = __SVSPVertex(fHeadingBottom, -6*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[38] = __SVSPVertex(fHeadingMiddle, -6*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[39] = __SVSPVertex(fHeadingMiddle, -6*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[40] = __SVSPVertex(fHeadingBottom, -6*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[41] = __SVSPVertex(fHeadingBottom, -6*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[42] = __SVSPVertex(fHeadingTop,    -7*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[43] = __SVSPVertex(fHeadingBottom, -7*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[44] = __SVSPVertex(fHeadingTop,    -7*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[45] = __SVSPVertex(fHeadingTop,    -7*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//---- 40
  pVerts[46] = __SVSPVertex(fHeadingBottom, -7*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[47] = __SVSPVertex(fHeadingBottom, -7*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[48] = __SVSPVertex(fHeadingMiddle, -8*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[49] = __SVSPVertex(fHeadingBottom, -8*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[50] = __SVSPVertex(fHeadingMiddle, -8*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[51] = __SVSPVertex(fHeadingMiddle, -8*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[52] = __SVSPVertex(fHeadingBottom, -8*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[53] = __SVSPVertex(fHeadingBottom, -8*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[54] = __SVSPVertex(fHeadingTop,    -9*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[55] = __SVSPVertex(fHeadingBottom, -9*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[56] = __SVSPVertex(fHeadingTop,    -9*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[57] = __SVSPVertex(fHeadingTop,    -9*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//---- 20
  pVerts[58] = __SVSPVertex(fHeadingBottom, -9*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[59] = __SVSPVertex(fHeadingBottom, -9*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[60] = __SVSPVertex(fHeadingMiddle, -10*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[61] = __SVSPVertex(fHeadingBottom, -10*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[62] = __SVSPVertex(fHeadingMiddle, -10*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[63] = __SVSPVertex(fHeadingMiddle, -10*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//-
  pVerts[64] = __SVSPVertex(fHeadingBottom, -10*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[65] = __SVSPVertex(fHeadingBottom, -10*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  pVerts[66] = __SVSPVertex(fHeadingTop,    -11*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[67] = __SVSPVertex(fHeadingBottom, -11*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[68] = __SVSPVertex(fHeadingTop,    -11*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[69] = __SVSPVertex(fHeadingTop,    -11*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);//---- 0
  pVerts[70] = __SVSPVertex(fHeadingBottom, -11*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[71] = __SVSPVertex(fHeadingBottom, -11*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_AGL_ALTITUDE_SCALE] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // GEOM_OBJECT_AGL_ALTITUDE_SCALE_BAR
  /////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_AGL_ALTITUDE_SCALE_BAR_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0 ] = __SVSPVertex(g_kAGLAltitudeScaleVector.x - 0.5f,                         g_kAGLAltitudeScaleVector.y - 11*c_fAGLScaleDisp, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1 ] = __SVSPVertex(g_kAGLAltitudeScaleVector.x - 0.5f + 2*fHalfLineThickness,  g_kAGLAltitudeScaleVector.y - 11*c_fAGLScaleDisp, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[2 ] = __SVSPVertex(g_kAGLAltitudeScaleVector.x - 0.5f,                         g_kAGLAltitudeScaleVector.y - 10*c_fAGLScaleDisp, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3 ] = __SVSPVertex(g_kAGLAltitudeScaleVector.x - 0.5f + 2*fHalfLineThickness,  g_kAGLAltitudeScaleVector.y - 10*c_fAGLScaleDisp, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_AGL_ALTITUDE_SCALE_BAR] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // GEOM_OBJECT_AGL_ALTITUDE_SCALE_BAR2
  /////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_AGL_ALTITUDE_SCALE_BAR2_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0 ] = __SVSPVertex(g_kAGLAltitudeScaleVector.x - 0.37f,                         g_kAGLAltitudeScaleVector.y - 11*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[1 ] = __SVSPVertex(g_kAGLAltitudeScaleVector.x - 0.45f,                         g_kAGLAltitudeScaleVector.y - 11*c_fAGLScaleDisp + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[2 ] = __SVSPVertex(g_kAGLAltitudeScaleVector.x - 0.37f,                         g_kAGLAltitudeScaleVector.y - 11*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[3 ] = __SVSPVertex(g_kAGLAltitudeScaleVector.x - 0.45f,                         g_kAGLAltitudeScaleVector.y - 11*c_fAGLScaleDisp - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_AGL_ALTITUDE_SCALE_BAR2] = pkGeomObject;

  /////////////////////////////////////////////////////////
  // PITCH SCALE
  /////////////////////////////////////////////////////////
  const FLOAT w      = 0.95f;
  const FLOAT d      = 0.7f;
  const FLOAT r      = 0.2f;

  //GEOM_OBJECT_DIVE_SCALE
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_DIVE_SCALE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  for(UINT iDialNum = 0; iDialNum < 20; iDialNum++)
  {
    const UINT  n      = 60;
    UINT  iDialNum_n   = iDialNum*n;
    FLOAT h            = fHalfLineThickness*2;
    INT   iDegrees     = -100 + iDialNum*5;
    FLOAT alpha        = -D3DX_PI*((FLOAT)(iDegrees - 10)/180)/2;
    FLOAT beta         = alpha/2 + D3DX_PI/4;
    FLOAT theta        = D3DX_PI/4 - alpha/2;
    FLOAT cos_alpha    = cos(alpha);
    FLOAT sin_alpha    = sin(alpha);
    FLOAT tan_alpha    = sin_alpha/cos_alpha;
    FLOAT len          = w/cos_alpha;
    FLOAT a            = len/7;
    FLOAT w_tan_alpha  = w*tan_alpha;
    FLOAT h2_tan_theta = fHalfLineThickness*tan(theta);
    FLOAT a_cos_alpha  = a*cos_alpha;
    FLOAT a_sin_alpha  = a*sin_alpha;
    FLOAT h_sin_alpha  = h*sin_alpha;
    FLOAT h_cos_alpha  = h*cos_alpha;
    FLOAT h2_sin_alpha = fHalfLineThickness*sin_alpha;
    FLOAT h2_cos_alpha = fHalfLineThickness*cos_alpha;

    //PITCH DEGREE
    FLOAT t            = 1.1f*w + d;
    m_PitchDegrees[iDialNum].kLeft  = D3DXVECTOR3(-t,  0, 0);
    m_PitchDegrees[iDialNum].kRight = D3DXVECTOR3( t,  0, 0);
    sprintf_s(&m_PitchDegrees[iDialNum].s[0], sizeof(m_PitchDegrees[iDialNum].s), "%d", iDegrees);

    pVerts[iDialNum_n +  0] = __SVSPVertex(-d + fHalfLineThickness,                   w_tan_alpha + r,                                 0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  1] = __SVSPVertex(-d - fHalfLineThickness,                   pVerts[iDialNum_n +  0].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  2] = __SVSPVertex(pVerts[iDialNum_n +  1].x,                 w_tan_alpha + h2_tan_theta,                      0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n +  3] = __SVSPVertex(pVerts[iDialNum_n +  0].x,                 pVerts[iDialNum_n +  0].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  4] = __SVSPVertex(pVerts[iDialNum_n +  2].x,                 pVerts[iDialNum_n +  2].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  5] = __SVSPVertex(pVerts[iDialNum_n +  0].x,                 w_tan_alpha - h2_tan_theta,                      0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n +  6] = __SVSPVertex(pVerts[iDialNum_n +  2].x,                 pVerts[iDialNum_n +  2].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  7] = __SVSPVertex(-d - a_cos_alpha - h2_sin_alpha,           w_tan_alpha - a_sin_alpha + h2_cos_alpha,        0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  8] = __SVSPVertex(-d - a_cos_alpha + h2_sin_alpha,           w_tan_alpha - a_sin_alpha - h2_cos_alpha,        0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n +  9] = __SVSPVertex(pVerts[iDialNum_n +  2].x,                 pVerts[iDialNum_n +  2].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 10] = __SVSPVertex(pVerts[iDialNum_n +  8].x,                 pVerts[iDialNum_n +  8].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 11] = __SVSPVertex(pVerts[iDialNum_n +  5].x,                 pVerts[iDialNum_n +  5].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n + 12] = __SVSPVertex(pVerts[iDialNum_n +  7].x - a_cos_alpha,   pVerts[iDialNum_n +  7].y - a_sin_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 13] = __SVSPVertex(pVerts[iDialNum_n + 12].x - a_cos_alpha,   pVerts[iDialNum_n + 12].y - a_sin_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 14] = __SVSPVertex(pVerts[iDialNum_n + 13].x + h_sin_alpha,   pVerts[iDialNum_n + 13].y - h_cos_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n + 15] = __SVSPVertex(pVerts[iDialNum_n + 12].x,                 pVerts[iDialNum_n + 12].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 16] = __SVSPVertex(pVerts[iDialNum_n + 14].x,                 pVerts[iDialNum_n + 14].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 17] = __SVSPVertex(pVerts[iDialNum_n + 12].x + h_sin_alpha,   pVerts[iDialNum_n + 12].y - h_cos_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n + 18] = __SVSPVertex(pVerts[iDialNum_n + 13].x - a_cos_alpha,   pVerts[iDialNum_n + 13].y - a_sin_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 19] = __SVSPVertex(pVerts[iDialNum_n + 18].x - a_cos_alpha,   pVerts[iDialNum_n + 18].y - a_sin_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 20] = __SVSPVertex(pVerts[iDialNum_n + 19].x + h_sin_alpha,   pVerts[iDialNum_n + 19].y - h_cos_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n + 21] = __SVSPVertex(pVerts[iDialNum_n + 18].x,                 pVerts[iDialNum_n + 18].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 22] = __SVSPVertex(pVerts[iDialNum_n + 20].x,                 pVerts[iDialNum_n + 20].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 23] = __SVSPVertex(pVerts[iDialNum_n + 18].x + h_sin_alpha,   pVerts[iDialNum_n + 18].y - h_cos_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n + 24] = __SVSPVertex(pVerts[iDialNum_n + 19].x - a_cos_alpha,   pVerts[iDialNum_n + 19].y - a_sin_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 25] = __SVSPVertex(pVerts[iDialNum_n + 24].x - a_cos_alpha,   pVerts[iDialNum_n + 24].y - a_sin_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 26] = __SVSPVertex(pVerts[iDialNum_n + 25].x + h_sin_alpha,   pVerts[iDialNum_n + 25].y - h_cos_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n + 27] = __SVSPVertex(pVerts[iDialNum_n + 24].x,                 pVerts[iDialNum_n + 24].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 28] = __SVSPVertex(pVerts[iDialNum_n + 26].x,                 pVerts[iDialNum_n + 26].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 29] = __SVSPVertex(pVerts[iDialNum_n + 24].x + h_sin_alpha,   pVerts[iDialNum_n + 24].y - h_cos_alpha,         0, 0, 0, __SVSPColor::dwHUDCOLOR);

    for(UINT i = 0; i < 30; ++i)
    {
      UINT idx = iDialNum_n + i;
      pVerts[idx + 30] = __SVSPVertex(-pVerts[idx].x, pVerts[idx].y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
    }
  }
  m_ppkGeomObjects[GEOM_OBJECT_DIVE_SCALE] = pkGeomObject;

  //GEOM_OBJECT_HORIZON
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_HORIZON_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  {
    pVerts[0]               = __SVSPVertex(-d,                                         fHalfLineThickness,                             0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[1]               = __SVSPVertex(-d - 20*d,                                  fHalfLineThickness,                             0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[2]               = __SVSPVertex(-d - 20*d,                                 -fHalfLineThickness,                             0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[3]               = __SVSPVertex(pVerts[0].x,                                pVerts[0].y,                                    0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[4]               = __SVSPVertex(pVerts[2].x,                                pVerts[2].y,                                    0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[5]               = __SVSPVertex(pVerts[0].x,                               -fHalfLineThickness,                             0, 0, 0, __SVSPColor::dwHUDCOLOR);

    for(UINT i = 0; i < 6; ++i)
    {
      pVerts[6 + i] = __SVSPVertex(-pVerts[i].x, pVerts[i].y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
    }
  }
  m_ppkGeomObjects[GEOM_OBJECT_HORIZON] = pkGeomObject;

  //GEOM_OBJECT_HORIZON_WARNING
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_HORIZON_WARNING_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  {
    pVerts[ 0]               = __SVSPVertex(-d,                                         fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[ 1]               = __SVSPVertex(-d - 2*d,                                   fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[ 2]               = __SVSPVertex(-d - 2*d,                                  -fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);

    pVerts[ 3]               = __SVSPVertex(pVerts[0].x,                                pVerts[0].y,                  0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[ 4]               = __SVSPVertex(pVerts[2].x,                                pVerts[2].y,                  0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[ 5]               = __SVSPVertex(pVerts[0].x,                               -fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);

    pVerts[ 6]               = __SVSPVertex(-5*d,                                       fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[ 7]               = __SVSPVertex(-5*d - 2*d,                                 fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[ 8]               = __SVSPVertex(-5*d - 2*d,                                -fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);

    pVerts[ 9]               = __SVSPVertex(pVerts[6].x,                                pVerts[6].y,                  0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[10]               = __SVSPVertex(pVerts[8].x,                                pVerts[8].y,                  0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[11]               = __SVSPVertex(pVerts[6].x,                               -fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);

    pVerts[12]               = __SVSPVertex(-9*d,                                       fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[13]               = __SVSPVertex(-9*d - 2*d,                                 fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[14]               = __SVSPVertex(-9*d - 2*d,                                -fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);

    pVerts[15]               = __SVSPVertex(pVerts[12].x,                               pVerts[12].y,                 0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[16]               = __SVSPVertex(pVerts[14].x,                               pVerts[14].y,                 0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    pVerts[17]               = __SVSPVertex(pVerts[12].x,                              -fHalfLineThickness,           0, 0, 0, __SVSPColor::dwWARNINGCOLOR);

    for(UINT i = 0; i < 18; ++i)
    {
      pVerts[18 + i]         = __SVSPVertex(-pVerts[i].x,                               pVerts[i].y,                  0, 0, 0, __SVSPColor::dwWARNINGCOLOR);
    }
  }
  m_ppkGeomObjects[GEOM_OBJECT_HORIZON_WARNING] = pkGeomObject;

  //GEOM_OBJECT_CLIMB_SCALE
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_CLIMB_SCALE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  for(UINT iDialNum = 0; iDialNum < 20; iDialNum++)
  {
    const UINT  n      = 24;
    UINT  iDialNum_n   = iDialNum*n;
    FLOAT h            = fHalfLineThickness*2;
    INT   iDegrees     = 5 + iDialNum*5;
    FLOAT alpha        = D3DX_PI*((FLOAT)iDegrees/180)/2;
    FLOAT beta         = alpha/2 + D3DX_PI/4;
    FLOAT theta        = D3DX_PI/4 - alpha/2;
    FLOAT cos_alpha    = cos(alpha);
    FLOAT sin_alpha    = sin(alpha);
    FLOAT tan_alpha    = sin_alpha/cos_alpha;
    FLOAT len          = w/cos_alpha;
    FLOAT a            = len/7;
    FLOAT w_tan_alpha  = w*tan_alpha;
    FLOAT h2_tan_theta = fHalfLineThickness*tan(theta);
    FLOAT w_cos_alpha  = w*cos_alpha;
    FLOAT w_sin_alpha  = w*sin_alpha;
    //FLOAT h_sin_alpha  = h*sin_alpha;
    //FLOAT h_cos_alpha  = h*cos_alpha;
    FLOAT h2_sin_alpha = fHalfLineThickness*sin_alpha;
    FLOAT h2_cos_alpha = fHalfLineThickness*cos_alpha;

    //PITCH DEGREE
    FLOAT t            = 1.1f*w + d;
    m_PitchDegrees[iDialNum + 20].kLeft  = D3DXVECTOR3(-t,  0, 0);
    m_PitchDegrees[iDialNum + 20].kRight = D3DXVECTOR3( t,  0, 0);
    sprintf_s(&m_PitchDegrees[iDialNum + 20].s[0], sizeof(m_PitchDegrees[iDialNum + 20].s), "%d", iDegrees);

    pVerts[iDialNum_n +  0] = __SVSPVertex(-d + fHalfLineThickness,                  -w_tan_alpha - r,                                 0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  1] = __SVSPVertex(-d - fHalfLineThickness,                   pVerts[iDialNum_n +  0].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  2] = __SVSPVertex(pVerts[iDialNum_n +  1].x,                -w_tan_alpha - h2_tan_theta,                      0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n +  3] = __SVSPVertex(pVerts[iDialNum_n +  0].x,                 pVerts[iDialNum_n +  0].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  4] = __SVSPVertex(pVerts[iDialNum_n +  2].x,                 pVerts[iDialNum_n +  2].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  5] = __SVSPVertex(pVerts[iDialNum_n +  0].x,                -w_tan_alpha + h2_tan_theta,                      0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n +  6] = __SVSPVertex(pVerts[iDialNum_n +  2].x,                 pVerts[iDialNum_n +  2].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  7] = __SVSPVertex(-d - w_cos_alpha - h2_sin_alpha,          -w_tan_alpha + w_sin_alpha - h2_cos_alpha,        0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n +  8] = __SVSPVertex(-d - w_cos_alpha + h2_sin_alpha,          -w_tan_alpha + w_sin_alpha + h2_cos_alpha,        0, 0, 0, __SVSPColor::dwHUDCOLOR);

    pVerts[iDialNum_n +  9] = __SVSPVertex(pVerts[iDialNum_n +  2].x,                 pVerts[iDialNum_n +  2].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 10] = __SVSPVertex(pVerts[iDialNum_n +  8].x,                 pVerts[iDialNum_n +  8].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[iDialNum_n + 11] = __SVSPVertex(pVerts[iDialNum_n +  5].x,                 pVerts[iDialNum_n +  5].y,                       0, 0, 0, __SVSPColor::dwHUDCOLOR);

    for(UINT i = 0; i < 12; ++i)
    {
      UINT idx = iDialNum_n + i;
      pVerts[idx + 12] = __SVSPVertex(-pVerts[idx].x, pVerts[idx].y, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
    }
  }
  m_ppkGeomObjects[GEOM_OBJECT_CLIMB_SCALE] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // FLIGHT PATH MARKER (VELOCITY VECTOR)
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_FPM_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-0.4f,  0.4f, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 0.4f,  0.4f, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-0.4f, -0.4f, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( 0.4f, -0.4f, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_FLIGHT_PATH_MARKER] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_FPM] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/HUD/RES/TEXTURES/FPM.dds");


  ///////////////////////////////////////////////////////////////////
  // FLIGHT DIRECTION WARNING
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_FDW_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-0.4f,  0.4f, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 0.4f,  0.4f, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-0.4f, -0.4f, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( 0.4f, -0.4f, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_FLIGHT_DIRECTION_WARNING] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_FDW] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/HUD/RES/TEXTURES/FDW.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_WARN_BAR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_WARN_BAR_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-1.6f,  1.6f, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 1.6f,  1.6f, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-1.6f, -1.6f, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( 1.6f, -1.6f, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_WARN_BAR] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_WARN] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/HUD/RES/TEXTURES/WARN.dds");


  ///////////////////////////////////////////////////////////////////
  // STEERING CUE MARKER
  ///////////////////////////////////////////////////////////////////
  m_kSteeringCueMarkerVector = D3DXVECTOR3(0, 1.5f, 0);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_STEERING_CUE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-0.4f,  0.4f, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 0.4f,  0.4f, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-0.4f, -0.4f, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( 0.4f, -0.4f, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_STEERING_CUE_MARKER] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_SCM] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/HUD/RES/TEXTURES/SCM.dds");

  ///////////////////////////////////////////////////////////////////
  // AAVWAYPOINT INFORMATION
  ///////////////////////////////////////////////////////////////////
  m_kCurrentWaypointNameVector = m_kAGLDesignatorVector + D3DXVECTOR3(0, -2*fVerticalOffset, 0);
  m_kTTIValueVector            = m_kAGLDesignatorVector + D3DXVECTOR3(0, -3*fVerticalOffset, 0);
  m_kCurrentWaypointNumVector  = m_kAGLDesignatorVector + D3DXVECTOR3(0, -4*fVerticalOffset, 0);


  ///////////////////////////////////////////////////////////////////
  // BANK SCALE
  ///////////////////////////////////////////////////////////////////
  D3DXMatrixTranslation(&m_kBankScaleTranslateMatrix, 0, -0.7f, 0);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, NUM_BANK_SCALE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  FLOAT R = 3.05f, Rp = 3.15f;
  const FLOAT afAlpha[NUM_BANK_SCALE_VERTS/6] = {45.0f, 30.0f, 20.0f, 10.0f, 0, -10.0f, -20.0f, -30.0f, -45.0f};
  const FLOAT afA[NUM_BANK_SCALE_VERTS/6]     = {fBankTop, fBankTop, fBankMiddle, fBankMiddle, fBankTop, fBankMiddle, fBankMiddle, fBankTop, fBankTop};
  for(UINT i = 0; i < NUM_BANK_SCALE_VERTS/6; ++i)
  {
    FLOAT fAlpha                       = afAlpha[i]*(D3DX_PI/180.0f);
    FLOAT fA                           = afA[i];
    FLOAT cos_alpha                    = cos(fAlpha);
    FLOAT sin_alpha                    = sin(fAlpha);
    FLOAT r_cos_alpha                  = R*cos_alpha;
    FLOAT r_sin_alpha                  = R*sin_alpha;
    FLOAT fHalfLineThickness_cos_alpha = fHalfLineThickness*cos_alpha;
    FLOAT fHalfLineThickness_sin_alpha = fHalfLineThickness*sin_alpha;

    pVerts[6*i + 0] = __SVSPVertex(-(r_sin_alpha - fHalfLineThickness_cos_alpha),      -(r_cos_alpha + fHalfLineThickness_sin_alpha),      0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[6*i + 1] = __SVSPVertex(pVerts[6*i + 0].x + fA*sin_alpha,                   pVerts[6*i + 0].y + fA*cos_alpha,                   0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[6*i + 2] = __SVSPVertex(pVerts[6*i + 1].x - 2*fHalfLineThickness_cos_alpha, pVerts[6*i + 1].y + 2*fHalfLineThickness_sin_alpha, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[6*i + 3] = pVerts[6*i + 0];
    pVerts[6*i + 4] = pVerts[6*i + 2];
    pVerts[6*i + 5] = __SVSPVertex(pVerts[6*i + 0].x - 2*fHalfLineThickness_cos_alpha, pVerts[6*i + 0].y + 2*fHalfLineThickness_sin_alpha, 0, 0, 0, __SVSPColor::dwHUDCOLOR);

  }
  m_ppkGeomObjects[GEOM_OBJECT_BANK_SCALE] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // BANK POINTER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_BANK_POINTER_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-0.15f, -Rp + 0.15f, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 0.15f, -Rp + 0.15f, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-0.15f, -Rp - 0.15f, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( 0.15f, -Rp - 0.15f, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_BANK_POINTER] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_BKP] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/HUD/RES/TEXTURES/BKP.dds");

  ///////////////////////////////////////////////////////
  // LOCALISER/GLIDESLOPE
  ///////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINELIST, HUD::NUM_LOCALIZER_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  for(UINT i = 0; i < HUD::NUM_LOCALIZER_VERTS; ++i)
  {
    pVerts[i] = __SVSPVertex( 0.0f, 1.2f - i*(2.4f/(HUD::NUM_LOCALIZER_VERTS - 1)), 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  }
  m_ppkGeomObjects[HUD::GEOM_OBJECT_LOCALIZER] = pkGeomObject;

  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINELIST, HUD::NUM_GLIDESLOPE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  for(UINT i = 0; i < HUD::NUM_GLIDESLOPE_VERTS; ++i)
  {
    pVerts[i] = __SVSPVertex( -1.2f + i*(2.4f/(HUD::NUM_GLIDESLOPE_VERTS - 1)), 0, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  }
  m_ppkGeomObjects[HUD::GEOM_OBJECT_GLIDESLOPE] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_AOA_BRACKET
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_AOA_BRACKET_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex( -0.7f,                       0.8f + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[ 1] = __SVSPVertex( -0.7f,                       0.8f - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[ 2] = __SVSPVertex( -0.9f - fHalfLineThickness,  0.8f + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[ 3] = __SVSPVertex( -0.9f + fHalfLineThickness,  0.8f - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[ 4] = __SVSPVertex( -0.9f - fHalfLineThickness, -0.8f - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[ 5] = __SVSPVertex( -0.9f + fHalfLineThickness, -0.8f + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[ 6] = __SVSPVertex( -0.7f,                      -0.8f - fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  pVerts[ 7] = __SVSPVertex( -0.7f,                      -0.8f + fHalfLineThickness, 0, 0, 0, __SVSPColor::dwHUDCOLOR);
  for(UINT i = 8; i < NUM_AOA_BRACKET_VERTS; ++i)
  {
    pVerts[i]   = pVerts[i - 8];
    pVerts[i].x = -pVerts[i].x;
  }
  m_ppkGeomObjects[HUD::GEOM_OBJECT_AOA_BRACKET] = pkGeomObject;

#if 0
  ///////////////////////////////////////////////////////////////////
  // WIND DIRECTION
  ///////////////////////////////////////////////////////////////////
  m_ppkGeomObjects[GEOM_OBJECT_WIND_DIRECTION] = new D3DXGEOMETRYOBJECT(NUM_WIND_DIRECTION_VERTS);
  m_ppkGeomObjects[GEOM_OBJECT_WIND_DIRECTION]->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = m_ppkGeomObjects[GEOM_OBJECT_WIND_DIRECTION]->GetVerts();
  pVerts[0] = __SVSPVertex( 0.0f, 0.0f, 0);
  pVerts[1] = __SVSPVertex(-0.1f, 0.1f, 0);
  pVerts[2] = __SVSPVertex( 0.0f, 0.0f, 0);
  pVerts[3] = __SVSPVertex( 0.1f, 0.1f, 0);
  pVerts[4] = __SVSPVertex( 0.0f, 0.0f, 0);
  pVerts[5] = __SVSPVertex( 0.0f, 0.2f, 0);
  pkGeomObject->Reset(m_ppkGeomObjects[GEOM_OBJECT_WIND_DIRECTION]);

  ///////////////////////////////////////////////////////////////////
  // HSI SCALE
  ///////////////////////////////////////////////////////////////////
  r1 = 1.89f; r2 = 1.7f; a = 0.07f;
  FLOAT fDegrees;
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINELIST, NUM_HSI_SCALE_VERTS);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  FLOAT fGradScale = 10.0f;
  for(i = 0; i < NUM_HSI_SCALE_VERTS/4; ++i)
  {
    fDegrees = fGradScale + 3*i*fGradScale;
    pVerts[4*i    ] = __SVSPVertex(r1*sin((D3DX_PI/180.0f)*fDegrees), r1*cos((D3DX_PI/180.0f)*fDegrees), 0, COLOR);
    pVerts[4*i + 1] = __SVSPVertex(r2*sin((D3DX_PI/180.0f)*fDegrees), r2*cos((D3DX_PI/180.0f)*fDegrees), 0, COLOR);
    pVerts[4*i + 2] = __SVSPVertex(r1*sin((D3DX_PI/180.0f)*(fDegrees + 10.0f)), r1*cos((D3DX_PI/180.0f)*(fDegrees + 10.0f)), 0, COLOR);
    pVerts[4*i + 3] = __SVSPVertex(r2*sin((D3DX_PI/180.0f)*(fDegrees + 10.0f)), r2*cos((D3DX_PI/180.0f)*(fDegrees + 10.0f)), 0, COLOR);
  }
  m_ppkGeomObjects[GEOM_OBJECT_HSI_SCALE] = pkGeomObject;

  r3 = (r1 + r2)/2;
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

  ///////////////////////////////////////////////////////////////////
  //HEADING BUG
  ///////////////////////////////////////////////////////////////////
  r4 = 2.49f;
  m_ppkGeomObjects[GEOM_OBJECT_HSI_HEADING_BUG] = new D3DXGEOMETRYOBJECT(NUM_HSI_HEADING_BUG_VERTS);
  m_ppkGeomObjects[GEOM_OBJECT_HSI_HEADING_BUG]->SetMaterial(0.3f, 1.0f, 0.3f, 1.0f);
  pVerts = m_ppkGeomObjects[GEOM_OBJECT_HSI_HEADING_BUG]->GetVerts();
  pVerts[0] = __SVSPVertex(-0.03f,  r4 + 0.1f,  0);
  pVerts[1] = __SVSPVertex(-0.03f,  r4 - 0.2f,  0);
  pVerts[2] = __SVSPVertex( 0.03f,  r4 + 0.1f,  0);
  pVerts[3] = __SVSPVertex( 0.03f,  r4 - 0.2f,  0);
  pkGeomObject->Reset(m_ppkGeomObjects[GEOM_OBJECT_HSI_HEADING_BUG]);

  //ADF BEARING
  m_ppkGeomObjects[GEOM_OBJECT_ADF_BEARING] = new D3DXGEOMETRYOBJECT(NUM_ADF_BEARING_VERTS);
  m_ppkGeomObjects[GEOM_OBJECT_ADF_BEARING]->SetMaterial(0.3f, 1.0f, 0.3f, 1.0f);
  pVerts = m_ppkGeomObjects[GEOM_OBJECT_ADF_BEARING]->GetVerts();
  pVerts[0] = __SVSPVertex(-0.09f,  0.05f,  0);
  pVerts[1] = __SVSPVertex( 0.00f,  0.25f,  0);
  pVerts[2] = __SVSPVertex( 0.09f,  0.05f,  0);
  pkGeomObject->Reset(m_ppkGeomObjects[GEOM_OBJECT_ADF_BEARING]);

  /////////////////////////////////////////////////////////
  // SIM THRUST TO WEIGHT RATIO SCALE
  /////////////////////////////////////////////////////////
  m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_SCALE] = new D3DXGEOMETRYOBJECT(NUM_THRUST_TO_WEIGHT_RATIO_SCALE_VERTS);
  m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_SCALE]->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_SCALE]->GetVerts();
  pVerts[0] = __SVSPVertex(0.85f, 1.5f, 0);
  pVerts[1] = __SVSPVertex(1.5f, 1.5f, 0);
  pVerts[2] = __SVSPVertex(1.5f,-1.5f, 0);
  pVerts[3] = __SVSPVertex(0.85f,-1.5f, 0);
  pVerts[4] = pVerts[0];
  pkGeomObject->Reset(m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_SCALE]);

  /////////////////////////////////////////////////////////
  // SIM THRUST TO WEIGHT RATIO BAR
  /////////////////////////////////////////////////////////
  m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_BAR] = new D3DXGEOMETRYOBJECT(NUM_THRUST_TO_WEIGHT_RATIO_BAR_VERTS);
  m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_BAR]->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_BAR]->GetVerts();
  pVerts[0] = __SVSPVertex(0.85f, 1.5f, 0);
  pVerts[1] = __SVSPVertex(1.5f, 1.5f, 0);
  pVerts[2] = __SVSPVertex(0.85f,-1.5f, 0);
  pVerts[3] = __SVSPVertex(1.5f,-1.5f, 0);
  pkGeomObject->Reset(m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_BAR]);


  ////WAY POINT CONNECTION
  //// Create the vertex buffer.
  //if( FAILED( pD3DDevice->CreateVertexBuffer(4*WPN::MAX_NUM_WAYPOINTS*sizeof(__SVSPVertex),D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pWPConnectionVB, NULL ) ) )
  //  return E_FAIL;

  ////WPI CROSS
  //m_WPICrossVerts[0] = __SVSPVertex(-5.0f,    0, 0); 
  //m_WPICrossVerts[1] = __SVSPVertex( 5.0f,    0, 0); 
  //m_WPICrossVerts[2] = __SVSPVertex(   0, -5.0f, 0); 
  //m_WPICrossVerts[3] = __SVSPVertex(   0,  5.0f, 0); 
  //// Create the vertex buffer.
  //if( SUCCEEDED( pD3DDevice->CreateVertexBuffer(sizeof(m_WPICrossVerts),D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pWPICrossVB, NULL ) ) )
  //{
  //  // Fill the vertex buffer.
  //  VOID* pVertices;
  //  if( SUCCEEDED( m_pWPICrossVB->Lock(0, sizeof(m_WPICrossVerts), (void**)&pVertices, 0 ) ) )
  //  {
  //    memcpy(pVertices, m_WPICrossVerts, sizeof(m_WPICrossVerts));
  //    m_pWPICrossVB->Unlock();
  //  }
  //  else
  //    return E_FAIL;
  //}
  //else
  //  return E_FAIL;


  ////Tex Test
  //float fDim = 6.0f;
  //m_TexTestVerts[0].x = -fDim; m_TexTestVerts[0].y = fDim; m_TexTestVerts[0].z = 0; m_TexTestVerts[0].color = 0xffffffff; m_TexTestVerts[0].tu = 0.0f; m_TexTestVerts[0].tv = 0.0f; 
  //m_TexTestVerts[1].x =  fDim; m_TexTestVerts[1].y = fDim; m_TexTestVerts[1].z = 0; m_TexTestVerts[1].color = 0xffffffff; m_TexTestVerts[1].tu = 1.0f; m_TexTestVerts[1].tv = 0.0f; 
  //m_TexTestVerts[2].x = -fDim; m_TexTestVerts[2].y =-fDim; m_TexTestVerts[2].z = 0; m_TexTestVerts[2].color = 0xffffffff; m_TexTestVerts[2].tu = 0.0f; m_TexTestVerts[2].tv = 1.0f; 
  //m_TexTestVerts[3].x =  fDim; m_TexTestVerts[3].y =-fDim; m_TexTestVerts[3].z = 0; m_TexTestVerts[3].color = 0xffffffff; m_TexTestVerts[3].tu = 1.0f; m_TexTestVerts[3].tv = 1.0f; 

  //if( SUCCEEDED( pD3DDevice->CreateVertexBuffer(sizeof(m_TexTestVerts),D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMTEXVERTEX, D3DPOOL_DEFAULT, &m_pTexTestVB, NULL ) ) )
  //{
  //  VOID* pVertices;
  //  if( SUCCEEDED( m_pTexTestVB->Lock(0, sizeof(m_TexTestVerts), (void**)&pVertices, 0 ) ) )
  //  {
  //    memcpy(pVertices, m_TexTestVerts, sizeof(m_TexTestVerts));
  //    m_pTexTestVB->Unlock();
  //  }
  //  else
  //    return E_FAIL;
  //}
  //else
  //  return E_FAIL;


  //if( SUCCEEDED( pD3DDevice->CreateTexture(2, 2, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL)))
  //{
  //  D3DLOCKED_RECT LockedRect;
  //  if(m_pTexture != NULL)
  //  {
  //    if(SUCCEEDED(m_pTexture->LockRect(0, &LockedRect, NULL, NULL)))
  //    {
  //      UINT *puiTexData = (UINT *)LockedRect.pBits;
  //      puiTexData[0] = 0x1fff00ff;
  //      puiTexData[1] = 0x1f0000ff;
  //      puiTexData[2] = 0x1f0000ff;
  //      puiTexData[3] = 0x1fff00ff;
  //      m_pTexture->UnlockRect(0);
  //    }
  //  }
  //}

#endif

  m_kLATValueVector = D3DXVECTOR3(-2.1f, -4.5f, 0);
  m_kLNGValueVector = D3DXVECTOR3(-2.1f, -4.9f, 0);
  m_kELVValueVector = D3DXVECTOR3(-2.1f, -5.3f, 0);
  m_kTOSValueVector = D3DXVECTOR3(-2.1f, -5.7f, 0);


  SetMode(HUD::DISPLAY_MODE_HMD);
}

HUD::~HUD()
{
}

VOID HUD::Release()
{
  m_uiNumStencilTextures = 0;

  __SVSPModule::Release();
}

HRESULT HUD::CreateTexture(LPD3DXDEVICE pkDevice, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPPD3DXTEXTURE ppTexture, HANDLE* pSharedHandle)
{
  HRESULT hResult = S_FALSE;

  if((Width == HUD::STENCIL_TEXTURE_WIDTH) && (Height == HUD::STENCIL_TEXTURE_HEIGHT))
  {
    AAV_ASSERT_RETURN((m_uiNumStencilTextures < HUD::MAX_NUM_STENCIL_TEXTURES), S_FALSE);
    hResult   = pkDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, &m_apkStencilTexture[m_uiNumStencilTextures], pSharedHandle);
    *ppTexture = m_apkStencilTexture[m_uiNumStencilTextures];
    m_uiNumStencilTextures++;
    AAV_ASSERT_RETURN(hResult == S_OK, S_FALSE);
    __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "Created new HUD Stencil texture");
  }

  return hResult;
}

HRESULT HUD::SetTexture(LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
  HRESULT hResult = S_FALSE;

  if((m_apkStencilTexture[0] != NULL) && (pTexture == m_apkStencilTexture[0]))
  {
    pkDevice->SetTexture(Stage, pTexture);
    //Register__SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE for the upcomming HUD stencil rendering (Only in HUD::DISPLAY_MODE_HUD).
    hResult = __SVSPD3D9_RegisterDrawIndexedPrimitiveCallback((__SVSPD3DDevice9_DrawIndexedPrimitive_Callback)HUD::DrawIndexedPrimitive_Callback, this);
    //hResult = S_OK;
  }

  return hResult;
}

/**
__SVSPAvionics::DrawIndexedPrimitive_Callback
  Must be called ones per frame whenever HUD stencil is set as an active texture, then unregistered.
*/
HRESULT FASTCALL HUD::DrawIndexedPrimitive_Callback(HUD *pkThis, LPD3DXDEVICE pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
  return pkThis->DrawIndexedPrimitive(pkDevice, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT HUD::DrawIndexedPrimitive(LPD3DXDEVICE pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
  HRESULT                hResult  = S_OK;
  IDirect3DBaseTexture9* pTexture = NULL;

  //hResult = pkDevice->DrawIndexedPrimitive(PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
 //Unregister __SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE
 //__SVSPD3D9_UnregisterDrawIndexedPrimitiveCallback();

  if(m_pkAvionics->IsReset())
  {
    //Get active texture, this call increases internal reference count.
    pkDevice->GetTexture(0,&pTexture);
    //Decrease reference count to active texture.
    if(pTexture != NULL)
    {
      pTexture->Release();
    }

    if(pTexture == m_apkStencilTexture[0])
    {
      //Unregister __SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE
      __SVSPD3D9_UnregisterDrawIndexedPrimitiveCallback();

      // Save Default Render state.
      pkDevice->GetRenderState(D3DRS_STENCILENABLE,              &m_dwDepthStencilTestEnable);
      pkDevice->GetRenderState(D3DRS_ZENABLE,                    &m_dwZEnable);
      pkDevice->GetRenderState(D3DRS_ZWRITEENABLE,               &m_dwZWriteEnable);
      pkDevice->GetRenderState(D3DRS_ZFUNC,                      &m_dwZFunction);
      pkDevice->GetRenderState(D3DRS_ALPHABLENDENABLE,           &m_dwAphaBlendEnable);
      pkDevice->GetRenderState(D3DRS_BLENDOP,                    &m_dwBlendOperation);
      pkDevice->GetRenderState(D3DRS_SRCBLEND,                   &m_dwSourceBlend);
      pkDevice->GetRenderState(D3DRS_SRCBLENDALPHA,              &m_dwSourceBlendAlpha);
      pkDevice->GetRenderState(D3DRS_DESTBLEND,                  &m_dwDestBlend);
      pkDevice->GetRenderState(D3DRS_DESTBLENDALPHA,             &m_dwDestBlendAlpha);
      pkDevice->GetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,   &m_dwSeparateAlphaBlendEnable);
      pkDevice->GetRenderState(D3DRS_ALPHATESTENABLE,            &m_dwAphaTestEnable);

      pkDevice->SetRenderState(D3DRS_STENCILENABLE,              TRUE);
      pkDevice->SetRenderState(D3DRS_STENCILMASK,                0xFFFFFFFF);
      pkDevice->SetRenderState(D3DRS_STENCILWRITEMASK,           0xFFFFFFFF);

      //pkDevice->SetRenderState(D3DRS_SRCBLENDALPHA,              D3DBLEND_ZERO);
      //pkDevice->SetRenderState(D3DRS_DESTBLENDALPHA,             D3DBLEND_ONE);


      // Clear HUD Depth-Stencil surface.
      //hResult = pkDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0, 1.0f, 0);
      //hResult = pkDevice->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);
      //AAV_ASSERT_RETURN(SUCCEEDED(hResult), S_FALSE);

      if(GetMode() == HUD::DISPLAY_MODE_HUD)
      {
        // Set Depth-Stencil render state to always fill in stencil buffer.
        pkDevice->SetRenderState(D3DRS_STENCILREF,       1);
        pkDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_NEVER);
        pkDevice->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_REPLACE);
        pkDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_KEEP);

        // Render in the current stencil surface.
        hResult = pkDevice->DrawIndexedPrimitive(PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
      }

      // Render
      __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalSetShaders(pkDevice);
      Render(pkDevice);
      __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalResetShaders(pkDevice);

      // Restore Default Render state.
      pkDevice->SetRenderState(D3DRS_STENCILENABLE,              m_dwDepthStencilTestEnable);
      pkDevice->SetRenderState(D3DRS_ZENABLE,                    m_dwZEnable);
      pkDevice->SetRenderState(D3DRS_ZWRITEENABLE,               m_dwZWriteEnable);
      pkDevice->SetRenderState(D3DRS_ZFUNC,                      m_dwZFunction);
      pkDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,           m_dwAphaBlendEnable);
      pkDevice->SetRenderState(D3DRS_BLENDOP,                    m_dwBlendOperation);
      pkDevice->SetRenderState(D3DRS_SRCBLEND,                   m_dwSourceBlend);
      pkDevice->SetRenderState(D3DRS_SRCBLENDALPHA,              m_dwSourceBlendAlpha);
      pkDevice->SetRenderState(D3DRS_DESTBLEND,                  m_dwDestBlend);
      pkDevice->SetRenderState(D3DRS_DESTBLENDALPHA,             m_dwDestBlendAlpha);
      pkDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,   m_dwSeparateAlphaBlendEnable);
      pkDevice->SetRenderState(D3DRS_ALPHATESTENABLE,            m_dwAphaTestEnable);
    }
  }

  return S_OK;
}



VOID HUD::RenderHSIData(LPD3DXDEVICE pkDevice)
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;

#if 0
  /////////////////////////////////////////////////
  // HSI
  /////////////////////////////////////////////////
  D3DXMATRIXA16 kRotationMatrix;
  D3DXMATRIXA16 kTranslationMatrix;

  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_HSI_SCALE];
  D3DXMatrixRotationZ(&m_matHeading, pkACData->m_dIndicatedMagneticHeading);

  m_matHeading = m_matHeading*m_kHUDTransformMatrix;
  pkGeomObject->SetTransform(pkDevice, &m_matHeading);
  //hr = pD3DDevice->GetTransform(D3DTS_WORLD, &kTranslationMatrix);
  pkGeomObject->Render(pkDevice);


  //HSI HEADING MARKS
  D3DXVECTOR3 v = D3DXVECTOR3(0.0f, 0.0f, 0);
  D3DXMatrixRotationZ(&matRot, -pkACData->m_dIndicatedMagneticHeading);
  for(int i = 0; i < (sizeof(m_akHSIHeading)/sizeof(HEADING)); ++i)
  {
    D3DXMatrixTranslation(&kTranslationMatrix, m_akHSIHeading[i].v.x, m_akHSIHeading[i].v.y, 0.0f );
    D3DXMatrixMultiply(&m_matHeadingNames, &matRot, &kTranslationMatrix );
    D3DXMatrixMultiply(&m_matHeadingNames, &m_matHeadingNames, &m_matHeading );
    pkGeomObject->SetTransform(pkDevice, &m_matHeadingNames);
    pkFont->RenderTextExt(pkDevice, (m_akHSIHeading[i].s), &v, __SVSPFont::FILTERED|__SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 110.0f);
  }

  //WIND DIRECTION
  D3DXMatrixTranslation(&kTranslationMatrix, 0.0f, 1.5f, 0.0f);
  D3DXMatrixRotationZ(&matRot, -(pkAC->m_usWindDirection*D3DX_PI)/180.0f);
  D3DXMatrixMultiply(&matRot, &kTranslationMatrix, &matRot);
  D3DXMatrixMultiply(&matRot, &matRot, &m_matHeading );
  pkGeomObject->SetTransform(pkDevice, &matRot);
  pkGeomObject->Render(m_ppkGeomObjects[GEOM_OBJECT_WIND_DIRECTION], D3DXGEOMETRYOBJECT::LINELIST, 0, 3);
  //AP HDG
  D3DXMatrixRotationZ(&matRot, -(pkAC->m_fAP_HDG*D3DX_PI)/180.0f);
  D3DXMatrixMultiply(&matRot, &matRot, &m_matHeading );
  pkGeomObject->SetTransform(pkDevice, &matRot);
  pkGeomObject->Render(m_ppkGeomObjects[GEOM_OBJECT_HSI_HEADING_BUG], D3DXGEOMETRYOBJECT::TRIANGLESTRIP, 0, 2);
  //ADF1 BEARING
  if(pkAC->m_usNAV1ID & 0x8)
  {
    D3DXMatrixTranslation(&kTranslationMatrix, 0.0f, 2.4f, 0.0f);
    D3DXMatrixRotationZ(&matRot, -(pkAC->m_fADF1Bearing*D3DX_PI)/180.0f);
    D3DXMatrixMultiply(&matRot, &kTranslationMatrix, &matRot);
    D3DXMatrixMultiply(&matRot, &matRot, &m_matHeading );
    pkGeomObject->SetTransform(pkDevice, &matRot);
    pkGeomObject->Render(m_ppkGeomObjects[GEOM_OBJECT_ADF_BEARING], D3DXGEOMETRYOBJECT::TRIANGLESTRIP, 0, 1);
  }

  switch(pkAC->m_dwNAVGPS)
  {
  case NAV_MODE:
    //VOR1 BEARING
    D3DXMatrixTranslation(&kTranslationMatrix, 0.0f, 2.4f, 0.0f);
    D3DXMatrixRotationZ(&matRot, -(FLOAT)(pkACData->m_dNavBearing*D3DX_PI)/180.0 + D3DX_PI);
    D3DXMatrixMultiply(&matRes, &kTranslationMatrix, &matRot);
    D3DXMatrixMultiply(&matRes, &matRes, &m_matHeading );
    pkGeomObject->SetTransform(pkDevice, &matRes);
    pkGeomObject->Render(m_ppkGeomObjects[GEOM_OBJECT_ADF_BEARING], D3DXGEOMETRYOBJECT::LINESTRIP, 0, 2);

    //VOR1 DME
    if(pkACData->m_bNavHasDME)
    {
      D3DXMatrixTranslation(&kTranslationMatrix, 0.0f, 1.7f, 0.0f);
      //FLOAT fTrueBearing = (FLOAT)((pkACData->m_dNavBearing - pkACData->m_dHeadingMagnetic)*D3DX_PI/180.0 + D3DX_PI);
      FLOAT fTrueBearing = (FLOAT)((pkACData->m_dNavBearing)*D3DX_PI/180.0 + D3DX_PI);
      D3DXMatrixRotationZ(&matRot,     fTrueBearing);
      D3DXMatrixRotationZ(&matRotAux, -fTrueBearing);
      D3DXMatrixMultiply(&matRes, &matRot, &kTranslationMatrix);
      D3DXMatrixMultiply(&matRes, &matRes, &matRotAux);
      matRes = matRes*m_kHUDTransformMatrix;
      pkGeomObject->SetTransform(pkDevice, &matRes);
      sprintf_s(acStr, sizeof(acStr), "%0.2f", pkACData->m_dNavDMEDistance);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &v, __SVSPFont::FILTERED|__SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y);
    }

    //NAV1 OBS
    D3DXMatrixTranslation(&kTranslationMatrix, 0.0f, 1.9f, 0.0f);
    D3DXMatrixRotationZ(&matRot, -(pkAC->m_fNAV1OBS*D3DX_PI)/180.0f);
    D3DXMatrixMultiply(&matRot, &kTranslationMatrix, &matRot);
    D3DXMatrixMultiply(&matRot, &matRot, &m_matHeading );
    matRot = matRot*m_kHUDTransformMatrix;
    pkGeomObject->SetTransform(pkDevice, &matRot);
    //pkGeomObject->Render(m_ppkGeomObjects[GEOM_OBJECT_ADF_BEARING], D3DXGEOMETRYOBJECT::TRIANGLESTRIP, 0, 1);
    break;

  case GPS_MODE:
    if(pkAC->m_kGPS.dwFlags & 0x4)
    {
      //NEXT WPT BEARING
      D3DXMatrixTranslation(&kTranslationMatrix, 0.0f, 2.4f, 0.0f);
      D3DXMatrixRotationZ(&matRot, -(FLOAT)pkAC->m_kGPS.dBearingToNextWPT);
      D3DXMatrixMultiply(&matRes, &kTranslationMatrix, &matRot);
      D3DXMatrixMultiply(&matRes, &matRes, &m_matHeading );
      pkGeomObject->SetTransform(pkDevice, &matRes);
      pkGeomObject->Render(m_ppkGeomObjects[GEOM_OBJECT_ADF_BEARING], D3DXGEOMETRYOBJECT::LINESTRIP, 0, 2);

      //NEXT WPT DISTANCE
      D3DXMatrixTranslation(&kTranslationMatrix, 0.0f, 1.7f, 0.0f);
      FLOAT fTrueBearing = (FLOAT)pkAC->m_kGPS.dBearingToNextWPT - pkACData->m_dIndicatedMagneticHeading*D3DX_PI/180.0f;
      D3DXMatrixRotationZ(&matRot,     fTrueBearing);
      D3DXMatrixRotationZ(&matRotAux, -fTrueBearing);
      D3DXMatrixMultiply(&matRes, &matRot, &kTranslationMatrix);
      D3DXMatrixMultiply(&matRes, &matRes, &matRotAux);
      matRes = matRes*m_kHUDTransformMatrix;
      pkGeomObject->SetTransform(pkDevice, &matRes);
      sprintf_s(acStr, sizeof(acStr), "%0.1f", ((FLOAT)pkAC->m_kGPS.dDistToNextWPT)/1852.0f);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &v, __SVSPFont::FILTERED|__SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y);
    }
    break;
  }
#endif
}


VOID HUD::RenderBankPointer(LPD3DXDEVICE pkDevice)
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  /////////////////////////////////////////////////
  // BANK POINTER
  /////////////////////////////////////////////////
  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_BANK_POINTER];
  //pkGeomObject->SetTransform(pkDevice, &(pkAC->m_kLocalToBodyZMatrix*m_kBankScaleTranslateMatrix*m_kHUDTransformMatrix));
  pkGeomObject->SetTransform(pkDevice, &(pkAC->m_kLocalToBodyZMatrix*m_kBankScaleTranslateMatrix*m_kFPMTransformMatrix));
  pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_BKP]);
  pkGeomObject->Render(pkDevice);
  /////////////////////////////////////////////////
  // BANK SCALE
  /////////////////////////////////////////////////
  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_BANK_SCALE];
  //pkGeomObject->SetTransform(pkDevice, &(m_kBankScaleTranslateMatrix*m_kHUDTransformMatrix));
  pkGeomObject->SetTransform(pkDevice, &(m_kBankScaleTranslateMatrix*m_kFPMTransformMatrix));
  pkGeomObject->SetTexture(pkDevice, NULL);
  pkGeomObject->Render(pkDevice);
}

VOID HUD::RenderNoseDirectionMarker(LPD3DXDEVICE pkDevice)
{
  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  /////////////////////////////////////////////////
  // NOSE DIRECTION MARKER
  /////////////////////////////////////////////////
  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_NOSE_DIRECTION_MARKER];
  pkGeomObject->SetTransform(pkDevice, &m_kHUDTransformMatrix);
  pkGeomObject->SetTexture(pkDevice, NULL);
  pkGeomObject->Render(pkDevice);
}

VOID HUD::RenderFlightPathMarker(LPD3DXDEVICE pkDevice)
{
  AC  *pkAC  = static_cast<AC  *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  D3DXMATRIXA16        kRotationMatrix;
  D3DXMATRIXA16        kTranslationMatrix;

  ////////////////////////////////////////////////
  // FLIGHT PATH MARKER
  ////////////////////////////////////////////////
  GetTransformMatrixFromLocalNormalizedVector(&m_kVelocityVectorTransformMatrix, (D3DXVECTOR3 *)&pkAC->m_kLocalVelocityNormalized);
  m_kFPMTransformMatrix = m_kOPMatrix*pkAC->m_kBankRotationMatrix*m_kVelocityVectorTransformMatrix;


  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_FLIGHT_PATH_MARKER];
  pkGeomObject->SetTransform(pkDevice, &m_kFPMTransformMatrix);
  pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_FPM]);
  pkGeomObject->Render(pkDevice);

  /////////////////////////////////////////////////////////
  // FLIGHT DIRECTION WARNING
  /////////////////////////////////////////////////////////
  FLOAT fMaxFlightPathMarkerDeviationAngle = m_kStencilScalingMatrix._11*m_kOPMatrix._11*HUD::cMaxFlightPathMarkerDeviationAngle;
  if(pkAC->m_kBodyVelocityNormalized.z < cos(fMaxFlightPathMarkerDeviationAngle))
  {
    D3DXVECTOR3 kNormalVector = *D3DXVec3Cross(&kNormalVector, (D3DXVECTOR3 *)&pkAC->m_kBodyVelocityNormalized, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
    D3DXVECTOR3 kUnitRotationVector = *D3DXVec3Normalize(&kUnitRotationVector, &kNormalVector);
    D3DXMATRIXA16 kAuxRotationMatrix;
    GetRotationAroundUnitVector(&kAuxRotationMatrix, &kUnitRotationVector, -fMaxFlightPathMarkerDeviationAngle);
    // m_kInboundBodyVelocityNormalized
    D3DXVec3TransformCoord(&m_kInboundBodyVelocityNormalized, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &kAuxRotationMatrix);

    D3DXVec3TransformCoord(&m_kInboundLocalVelocityNormalized, &m_kInboundBodyVelocityNormalized, &pkAC->m_kBodyToLocalTransformMatrix);
    m_kInboundLocalVelocityNormalized.x = -m_kInboundLocalVelocityNormalized.x;

    GetTransformMatrixFromLocalNormalizedVector(&m_kVelocityVectorTransformMatrix, &m_kInboundLocalVelocityNormalized);

    // (!) m_kFPMTransformMatrix gets redefined here. (!)
    m_kFPMTransformMatrix = m_kOPMatrix*pkAC->m_kBankRotationMatrix*m_kVelocityVectorTransformMatrix;

    // (!) m_kHUDTransformMatrix gets redefined here. (!)
    D3DXMatrixTranslation(&kTranslationMatrix, 0, 15.0f*(FLOAT)m_kInboundBodyVelocityNormalized.y, 0);
    m_kHUDShiftMatrix = kTranslationMatrix*m_kHUDTransformMatrix;
    m_kHUDTransformMatrix = m_kHUDShiftMatrix;

    if(__SVSPAvionics::g_bWarnClockEdgeUP)
    {
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_FLIGHT_DIRECTION_WARNING];
      pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_FDW]);
      pkGeomObject->SetTransform(pkDevice, &m_kFPMTransformMatrix);
      pkGeomObject->Render(pkDevice);
    }

    D3DXVec2Normalize(&m_kXZLocalVelocityVectorNormalized, &D3DXVECTOR2(m_kInboundLocalVelocityNormalized.x, m_kInboundLocalVelocityNormalized.z));
  }
  else
  {
    // (!) m_kHUDTransformMatrix gets redefined here. (!)
    D3DXMatrixTranslation(&kTranslationMatrix, 0, 15.0f*(FLOAT)pkAC->m_kBodyVelocityNormalized.y, 0);
    m_kHUDShiftMatrix = kTranslationMatrix*m_kHUDTransformMatrix;
    m_kHUDTransformMatrix = m_kHUDShiftMatrix;

    D3DXVec2Normalize(&m_kXZLocalVelocityVectorNormalized, &D3DXVECTOR2(pkAC->m_kLocalVelocityNormalized.x, pkAC->m_kLocalVelocityNormalized.z));
  }
}

VOID HUD::RenderAoABracket(LPD3DXDEVICE pkDevice)
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkACData  != NULL);

  ///////////////////////////////////////////////////////
  // GEOM_OBJECT_AOA_BRACKET
  ///////////////////////////////////////////////////////
  LPD3DXGEOMETRYOBJECT pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_AOA_BRACKET];
  AAV_ASSERT_RETURN_VOID(pkGeomObject  != NULL);

  if(!pkACData->m_bOnGround && (pkACData->m_dGearPosition == 1))
  {
    D3DXMatrixTranslation(&m_AoABracketTranslationMatrix, 0.0f, 1.6f*(pkAC->m_fAoA/FLCS::MAXIMUM_ANGLE_OF_ATTACK) - 0.8f, 0.0f);
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->SetTransform(pkDevice, &(m_AoABracketTranslationMatrix*m_kFPMTransformMatrix));
    pkGeomObject->RenderPrimitives(pkDevice, 0, 6);
    pkGeomObject->RenderPrimitives(pkDevice, 8, 6);
  }
}

VOID HUD::RenderSteeringData(LPD3DXDEVICE pkDevice)
{
  CHAR acStr[32];
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  WPN *pkWPN = static_cast<WPN *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_WPN));
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);


  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;

  if((pkWPN != NULL) && (pkWPN->GetNumWaypoints() != 0))
  {
    UINT uiCurrWaypointNum = pkWPN->GetCurrentWaypointNum();
    const AAVWAYPOINT<FLOAT> *pkSTP = pkWPN->GetWaypoint(uiCurrWaypointNum);
    if(pkSTP != NULL)
    {
      if(pkSTP->bHasRangeData)
      {
        pkFont->SetTransform(pkDevice, &m_kHUDTransformMatrix);

        /////////////////////////////////////////////////
        // SLANT RANGE TO NEXT STP
        /////////////////////////////////////////////////
        sprintf_s(acStr, sizeof(acStr), "B%05.1f", pkSTP->fSlantRangeMiles);
        //sprintf_s(acStr, sizeof(acStr), "B%05.1f %.2f %.2f %.2f", pkCurrentSTP->fSlantRangeMiles, pkAC->m_dVelocity/3.28, pkCurrentSTP->fClosingInVelocity, pkCurrentSTP->fTTI);
        pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kCurrentWaypointNameVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);

        /////////////////////////////////////////////////
        // TIME-TO-INTERCEPT VALUE
        /////////////////////////////////////////////////
        if(pkSTP->fClosingInVelocity > 10.0f) /* m/s*/
        {
          sprintf_s(acStr, sizeof(acStr), "%03u:%02u", pkSTP->usTTIMinutes, pkSTP->usTTISeconds);
          pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kTTIValueVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
        }

        if(!(pkWPN->m_bRouteDeviationCheck && !m_pkAvionics->g_bWarnClockEdgeUP))
        {
          /////////////////////////////////////////////////
          // CURRENT AAVWAYPOINT VALUE
          /////////////////////////////////////////////////
          sprintf_s(acStr, sizeof(acStr), "%03u>%02u", pkSTP->uiDTI, uiCurrWaypointNum + 1);
          pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kCurrentWaypointNumVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);

          /////////////////////////////////////////////////////////
          // STEERING CUE MARKER
          /////////////////////////////////////////////////////////
          //D3DXMatrixTranslation(&kTranslationMatrix, m_kSteeringCueMarkerVector.x, m_kSteeringCueMarkerVector.y, 0);
          pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_STEERING_CUE_MARKER];
          pkGeomObject->SetTransform(pkDevice, &(pkSTP->kSteercueTransformMatrix*pkAC->m_kLocalToBodyZMatrix*m_kFPMTransformMatrix));
          pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_SCM]);
          pkGeomObject->Render(pkDevice);
        }

      }
      else
      {
        /////////////////////////////////////////////////
        // SLANT RANGE TO NEXT STP
        /////////////////////////////////////////////////
        pkFont->SetTransform(pkDevice, &m_kHUDTransformMatrix);
        pkFont->RenderTextExt(pkDevice, "-----", &m_kCurrentWaypointNameVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
      }
    }
  }
  else
  {
      /////////////////////////////////////////////////
      // SLANT RANGE TO NEXT STP
      /////////////////////////////////////////////////
      pkFont->SetTransform(pkDevice, &m_kHUDTransformMatrix);
      pkFont->RenderTextExt(pkDevice, "-----", &m_kCurrentWaypointNameVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
}

VOID HUD::RenderLockedTargetData(LPD3DXDEVICE pkDevice)
{
  CHAR acStr[32];
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  FCR *pkFCR = static_cast<FCR *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FCR));
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;

  if(pkFCR != NULL)
  {
    OBJECT *pkTarget = pkFCR->GetTarget();
    if(pkTarget != NULL)
    {
      /////////////////////////////////////////////////////////
      // STEERING CUE MARKER
      /////////////////////////////////////////////////////////
      //D3DXMatrixTranslation(&kTranslationMatrix, m_kSteeringCueMarkerVector.x, m_kSteeringCueMarkerVector.y, 0);
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_STEERING_CUE_MARKER];
      //pkGeomObject->SetTransform(pkDevice, &(pkSTP->kSteercueTransformMatrix*pkAC->m_kLocalToBodyZMatrix*m_kFPMTransformMatrix));
      //pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_SCM]);
      //pkGeomObject->Render(pkDevice);

      pkGeomObject->SetTransform(pkDevice, &m_kHUDTransformMatrix);
      /////////////////////////////////////////////////
      // SLANT RANGE TO THE TARGET
      /////////////////////////////////////////////////
      sprintf_s(acStr, sizeof(acStr), "F%05.1f", pkTarget->fSlantRangeMiles);
      //sprintf_s(acStr, sizeof(acStr), "B%05.1f %.2f %.2f %.2f", pkCurrentSTP->fSlantRangeMiles, pkAC->m_dVelocity/3.28, pkCurrentSTP->fClosingInVelocity, pkCurrentSTP->fTTI);
      pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kCurrentWaypointNameVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);

      /////////////////////////////////////////////////
      // TIME-TO-INTERCEPT VALUE
      /////////////////////////////////////////////////
      if(pkTarget->fClosingInVelocity > 10.0f) /* m/s*/
      {
        sprintf_s(acStr, sizeof(acStr), "%03u:%02u", pkTarget->usTTIMinutes, pkTarget->usTTISeconds);
        pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kTTIValueVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
      }
    }
  }
  else
  {
      /////////////////////////////////////////////////
      // SLANT RANGE TO THE TARGET
      /////////////////////////////////////////////////
      pkFont->RenderTextExt(pkDevice, "-----", &m_kCurrentWaypointNameVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
}

VOID HUD::RenderILSData(LPD3DXDEVICE pkDevice)
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkACData  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;

  ///////////////////////////////////////////////////////
  // ILS LOCALISER/GLIDESLOPE 
  ///////////////////////////////////////////////////////
  if(!pkACData->m_bOnGround)
  {
    if(pkAC->m_bNavHasLocalizer)
    {
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_LOCALIZER];
      if(pkGeomObject != NULL)
      {
        D3DXMatrixTranslation(&m_matLocaliserTransform, 1.6f*pkAC->m_kILSErrorVector.x/127.0f, 0.0f, 0.0f);
        pkGeomObject->SetTransform(pkDevice, &(m_matLocaliserTransform*m_kFPMTransformMatrix));
        pkGeomObject->SetTexture(pkDevice, NULL);
        pkGeomObject->Render(pkDevice);
      }
    }

    if(pkAC->m_bNavHasGlideslope)
    {
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_GLIDESLOPE];
      if(pkGeomObject != NULL)
      {
        D3DXMatrixTranslation(&m_matGlideslopeTransform, 0.0f, -1.6f*pkAC->m_kILSErrorVector.y/119.0f, 0.0f);
        pkGeomObject->SetTransform(pkDevice, &(m_matGlideslopeTransform*m_kFPMTransformMatrix));
        pkGeomObject->SetTexture(pkDevice, NULL);
        pkGeomObject->Render(pkDevice);
      }
    }
  }
}

VOID HUD::RenderPitchScale(LPD3DXDEVICE pkDevice)
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkACData  != NULL);
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;

  /////////////////////////////////////////////////
  // ATTITUDE (PITCH) SCALE
  /////////////////////////////////////////////////
  D3DXMATRIXA16 kPitchTransformMatrix;
  FLOAT         fPitchAngle;
  FLOAT         fCosPitchAngle;
  FLOAT         fSinPitchAngle;
  FLOAT         fVirtualHorizonPitchDiff = 5;

  FLOAT fDegrees = 180.0f*((FLOAT)pkAC->m_kBodyAttitude.x/D3DX_PI);

  if((abs(fDegrees) > fVirtualHorizonPitchDiff) && m_pkAvionics->g_bWarnClockEdgeUP)
  {
    //Render virtual horizon.
    FLOAT fPichAngleDegrees = sign<FLOAT>(fDegrees)*fVirtualHorizonPitchDiff - fDegrees;
    fPitchAngle = D3DX_PI*fPichAngleDegrees/180.0f;
    fCosPitchAngle = cos(fPitchAngle);
    fSinPitchAngle = sin(fPitchAngle);
    GetTransformMatrixFromLocalNormalizedVector( &kPitchTransformMatrix, &D3DXVECTOR3(fCosPitchAngle*m_kXZLocalVelocityVectorNormalized.x, fSinPitchAngle, fCosPitchAngle*m_kXZLocalVelocityVectorNormalized.y));
    pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_HORIZON_WARNING];
    pkGeomObject->SetTransform(pkDevice, &(m_kOPMatrix*kPitchTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->Render(pkDevice);
  }

  UINT uiCurDial = (UINT)(-fDegrees/5.0f) + 20;
  uiCurDial = (uiCurDial < 0) ? 0 : uiCurDial;
  uiCurDial = (uiCurDial > 40) ? 40 : uiCurDial;

  INT iFirstDial = ((uiCurDial - 1) < 0) ? 0   : uiCurDial - 1;
  INT iLastDial  = ((uiCurDial + 1) > 40) ? 40 : uiCurDial + 1;

  for(INT iDialNum = iFirstDial; iDialNum <= iLastDial; iDialNum++)
  {
    INT iDegrees   = 5*(iDialNum - 20);
    fPitchAngle    = D3DX_PI*(iDegrees/180.0f);
    fCosPitchAngle = cos(fPitchAngle);
    fSinPitchAngle = sin(fPitchAngle);

    GetTransformMatrixFromLocalNormalizedVector( &kPitchTransformMatrix, &D3DXVECTOR3(fCosPitchAngle*m_kXZLocalVelocityVectorNormalized.x, fSinPitchAngle, fCosPitchAngle*m_kXZLocalVelocityVectorNormalized.y));

    if(iDialNum < 20)
    {
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_DIVE_SCALE];
      pkGeomObject->SetTransform(pkDevice, &(m_kOPMatrix*kPitchTransformMatrix));
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->RenderPrimitives(pkDevice, iDialNum*60, 20);
      PITCH_DEGREE *pPitchDegree = &m_PitchDegrees[iDialNum];
      pkFont->RenderTextExt(pkDevice, &pPitchDegree->s[0], &pPitchDegree->kLeft,  __SVSPFont::ALIGN_RIGHT|__SVSPFont::CENTERED_Y|__SVSPFont::FILTERED );
      pkFont->RenderTextExt(pkDevice, &pPitchDegree->s[0], &pPitchDegree->kRight,                        __SVSPFont::CENTERED_Y|__SVSPFont::FILTERED );
    }
    else if(iDialNum == 20)
    {
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_HORIZON];
      pkGeomObject->SetTransform(pkDevice, &(m_kOPMatrix*kPitchTransformMatrix));
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->Render(pkDevice);
    }
    else
    {
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_CLIMB_SCALE];
      pkGeomObject->SetTransform(pkDevice, &(m_kOPMatrix*kPitchTransformMatrix));
      pkGeomObject->SetTexture(pkDevice, NULL);
      pkGeomObject->RenderPrimitives(pkDevice, (iDialNum - 21)*24, 8 );
      PITCH_DEGREE *pPitchDegree = &m_PitchDegrees[iDialNum - 1];
      pkFont->RenderTextExt(pkDevice, &pPitchDegree->s[0], &pPitchDegree->kLeft,  __SVSPFont::ALIGN_RIGHT|__SVSPFont::CENTERED_Y|__SVSPFont::FILTERED );
      pkFont->RenderTextExt(pkDevice, &pPitchDegree->s[0], &pPitchDegree->kRight,                        __SVSPFont::CENTERED_Y|__SVSPFont::FILTERED );
    }
  }
}

VOID HUD::RenderHeadingScale(LPD3DXDEVICE pkDevice)
{
  CHAR acStr[32];
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkACData  != NULL);
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  D3DXMATRIXA16        kRotationMatrix;
  D3DXMATRIXA16        kTranslationMatrix;

  /////////////////////////////////////////////////
  // HEADING SCALE
  /////////////////////////////////////////////////
  FLOAT fHeading        = (FLOAT)(pkACData->m_dIndicatedMagneticHeading*180.0)/D3DX_PI;
  UINT uiHeading        = (UINT)fHeading;
  UINT uiHeadingMod     = uiHeading%HEADING_SCALE_MODULO;
  pkGeomObject          = m_ppkGeomObjects[GEOM_OBJECT_HEADING_SCALE];

  if(uiHeadingMod <= 2)
  {
    m_uiHeading[0]        = uiHeading - uiHeadingMod;
    m_uiHeading[0]        = (m_uiHeading[0] == 360) ?   0 : m_uiHeading[0];
    m_uiHeading[1]        = (m_uiHeading[0] ==   0) ? 350 : m_uiHeading[0] - 10;
    m_uiHeading[2]        = (m_uiHeading[0] == 350) ?   0 : m_uiHeading[0] + 10;
    m_kHeadingScaleVector.x = -fmod(fHeading, (FLOAT)HEADING_SCALE_MODULO)*fHeadingDisp/c_fHeadingScaleDispMod; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kHeadingScaleVector.x, m_kHeadingScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kFPMTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 6, 10);
    sprintf_s(acStr, sizeof(acStr), "%0.2d", m_uiHeading[1]/10);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akHeadingScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_X);
    sprintf_s(acStr, sizeof(acStr), "%0.2d", m_uiHeading[2]/10);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akHeadingScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_X);
  }
  else if(uiHeadingMod <= 4)
  {
    m_uiHeading[0]        = uiHeading - uiHeadingMod;
    m_uiHeading[0]        = (m_uiHeading[0] == 360) ?   0 : m_uiHeading[0];
    //m_uiHeading[1]        = (m_uiHeading[0] ==   0) ? 350 : m_uiHeading[0] - 10;
    m_uiHeading[2]        = (m_uiHeading[0] == 350) ?   0 : m_uiHeading[0] + 10;
    m_kHeadingScaleVector.x = -fmod(fHeading, (FLOAT)HEADING_SCALE_MODULO)*fHeadingDisp/c_fHeadingScaleDispMod; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kHeadingScaleVector.x, m_kHeadingScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kFPMTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 12, 8);
    sprintf_s(acStr, sizeof(acStr), "%0.2d", m_uiHeading[2]/10);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akHeadingScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_X);
  }
  else if(uiHeadingMod <= 6)
  {
    m_uiHeading[0]        = uiHeading - uiHeadingMod + 10;
    m_uiHeading[0]        = (m_uiHeading[0] == 360) ?   0 : m_uiHeading[0];
    m_uiHeading[1]        = (m_uiHeading[0] ==   0) ? 350 : m_uiHeading[0] - 10;
    //m_uiHeading[2]        = (m_uiHeading[0] == 350) ?   0 : m_uiHeading[0] + 10;
    m_kHeadingScaleVector.x = -fmod(fHeading, (FLOAT)HEADING_SCALE_MODULO)*fHeadingDisp/c_fHeadingScaleDispMod + 2*fHeadingDisp; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kHeadingScaleVector.x, m_kHeadingScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kFPMTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 10);
    sprintf_s(acStr, sizeof(acStr), "%0.2d", m_uiHeading[0]/10);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akHeadingScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_X);
    sprintf_s(acStr, sizeof(acStr), "%0.2d", m_uiHeading[1]/10);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akHeadingScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_X);
  }
  else if(uiHeadingMod < 8)
  {
    m_uiHeading[0]        = uiHeading - uiHeadingMod + 10;
    m_uiHeading[0]        = (m_uiHeading[0] == 360) ?   0 : m_uiHeading[0];
    m_uiHeading[1]        = (m_uiHeading[0] ==   0) ? 350 : m_uiHeading[0] - 10;
    //m_uiHeading[2]        = (m_uiHeading[0] == 350) ?   0 : m_uiHeading[0] + 10;
    m_kHeadingScaleVector.x = -fmod(fHeading, (FLOAT)HEADING_SCALE_MODULO)*fHeadingDisp/c_fHeadingScaleDispMod + 2*fHeadingDisp; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kHeadingScaleVector.x, m_kHeadingScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kFPMTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 10);
    sprintf_s(acStr, sizeof(acStr), "%0.2d", m_uiHeading[1]/10);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akHeadingScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_X);
  }
  else
  {
    m_uiHeading[0]        = uiHeading - uiHeadingMod + 10;
    m_uiHeading[0]        = (m_uiHeading[0] == 360) ?   0 : m_uiHeading[0];
    m_uiHeading[1]        = (m_uiHeading[0] ==   0) ? 350 : m_uiHeading[0] - 10;
    m_uiHeading[2]        = (m_uiHeading[0] == 350) ?   0 : m_uiHeading[0] + 10;
    m_kHeadingScaleVector.x = -fmod(fHeading, (FLOAT)HEADING_SCALE_MODULO)*fHeadingDisp/c_fHeadingScaleDispMod + 2*fHeadingDisp; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kHeadingScaleVector.x, m_kHeadingScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kFPMTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 6, 10);
    sprintf_s(acStr, sizeof(acStr), "%0.2d", m_uiHeading[1]/10);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akHeadingScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_X);
    sprintf_s(acStr, sizeof(acStr), "%0.2d", m_uiHeading[2]/10);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akHeadingScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_X);
  }

  /////////////////////////////////////////////////
  // HEADING VALUE
  /////////////////////////////////////////////////
  pkGeomObject->SetTransform(pkDevice, &m_kFPMTransformMatrix);
  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_HEADING_BOX];
  pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_HDG]);
  pkGeomObject->Render(pkDevice);

  sprintf_s(acStr, sizeof(acStr), "%03d", uiHeading);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kHeadingValueVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y);

  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_HEADING_SCALE];
  pkGeomObject->SetTexture(pkDevice, NULL);
  pkGeomObject->RenderPrimitives(pkDevice, 42, 2);
}

VOID HUD::RenderAirspeedScale(LPD3DXDEVICE pkDevice)
{
  CHAR acStr[32];
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkACData  != NULL);
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  D3DXMATRIXA16        kRotationMatrix;
  D3DXMATRIXA16        kTranslationMatrix;

  /////////////////////////////////////////////////
  // AIRSPEED SCALE
  /////////////////////////////////////////////////
  INT iAirSpeed          = (INT)pkACData->m_dIndicatedAirspeed;
  INT iAirSpeedMod       = iAirSpeed%AIRSPEED_SCALE_MODULO;
  pkGeomObject           = m_ppkGeomObjects[GEOM_OBJECT_SPEED_SCALE];

  if(iAirSpeedMod <= 2)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 6, 26);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else if(iAirSpeedMod <= 7)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 6, 24);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else if(iAirSpeedMod <= 12)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 26);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else if(iAirSpeedMod <= 17)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 24);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else if(iAirSpeedMod <= 22)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 24);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else if(iAirSpeedMod <= 27)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    //m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 22);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else if(iAirSpeedMod <= 32)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 4*c_fScaleDisp; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 18, 24);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else if(iAirSpeedMod <= 37)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 4*c_fScaleDisp; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 18, 24);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else if(iAirSpeedMod <= 42)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 4*c_fScaleDisp; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 18, 22);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else if(iAirSpeedMod <= 47)
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 4*c_fScaleDisp; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 12, 24);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
  else
  {
    m_iAirSpeed[0]           = iAirSpeed - iAirSpeedMod + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[1]           = m_iAirSpeed[0] + AIRSPEED_SCALE_MODULO;
    m_iAirSpeed[2]           = m_iAirSpeed[0] - AIRSPEED_SCALE_MODULO;
    m_kAirspeedScaleVector.y = -fmod((FLOAT)pkACData->m_dIndicatedAirspeed, (FLOAT)AIRSPEED_SCALE_MODULO)*c_fScaleDisp/c_fAirspeedScaleDispMod + 4*c_fScaleDisp; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kAirspeedScaleVector.x, m_kAirspeedScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 6, 26);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[0]/100.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[1]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAirSpeed[2]/100.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akAirspeedScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }

  /////////////////////////////////////////////////
  // AIRSPEED VALUE
  /////////////////////////////////////////////////
  pkGeomObject->SetTransform(pkDevice, &m_kHUDTransformMatrix);
  sprintf_s(acStr, sizeof(acStr), "%d", iAirSpeed);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kAirspeedValueVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
}

VOID HUD::RenderAltitudeScale(LPD3DXDEVICE pkDevice)
{
  CHAR acStr[32];
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkACData  != NULL);
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  D3DXMATRIXA16        kRotationMatrix;
  D3DXMATRIXA16        kTranslationMatrix;

  /////////////////////////////////////////////////
  // ALTITUDE SCALE
  /////////////////////////////////////////////////
  INT iAltitude               = (UINT)pkACData->m_dIndicatedAltitude;
  INT iAltitudeMod            = iAltitude%ALTITUDE_SCALE_MODULO;
  pkGeomObject                = m_ppkGeomObjects[GEOM_OBJECT_ALTITUDE_SCALE];

  if(iAltitudeMod < 5)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 6, 26);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else if(iAltitudeMod < 15)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 6, 24);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else if(iAltitudeMod < 25)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 26);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else if(iAltitudeMod < 35)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 24);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else if(iAltitudeMod < 45)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 24);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else if(iAltitudeMod < 55)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    //m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 0.0f; 
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 0, 22);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else if(iAltitudeMod < 65)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod + ALTITUDE_SCALE_MODULO;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 4*c_fScaleDisp;
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 18, 24);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else if(iAltitudeMod < 75)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod + ALTITUDE_SCALE_MODULO;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 4*c_fScaleDisp;
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 18, 24);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else if(iAltitudeMod < 85)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod + ALTITUDE_SCALE_MODULO;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 4*c_fScaleDisp;
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 18, 22);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else if(iAltitudeMod < 95)
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod + ALTITUDE_SCALE_MODULO;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 4*c_fScaleDisp;
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 12, 24);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }
  else
  {
    m_iAltitude[0]              = iAltitude - iAltitudeMod + ALTITUDE_SCALE_MODULO;
    m_iAltitude[1]              = m_iAltitude[0] + ALTITUDE_SCALE_MODULO;
    m_iAltitude[2]              = m_iAltitude[0] - ALTITUDE_SCALE_MODULO;
    m_kMSLAltitudeScaleVector.y = -(FLOAT)fmod(pkACData->m_dIndicatedAltitude, (FLOAT)ALTITUDE_SCALE_MODULO)*c_fScaleDisp/c_fMSLAltitudeScaleDispMod + 4*c_fScaleDisp;
    D3DXMatrixTranslation(&kTranslationMatrix, m_kMSLAltitudeScaleVector.x, m_kMSLAltitudeScaleVector.y, 0);
    pkGeomObject->SetTransform(pkDevice, &(kTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->RenderPrimitives(pkDevice, 6, 26);
    //sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[0]/1000.0f));
    //pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[0], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[1]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[1], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    sprintf_s(acStr, sizeof(acStr), "%#04.1f", (FLOAT)(m_iAltitude[2]/1000.0f));
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_akMSLAltitudeScaleValueVector[2], __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
  }

  /////////////////////////////////////////////////
  // AGL ALTITUDE SCALE
  /////////////////////////////////////////////////
  if((pkAC->m_dRadarAltitude < 1000) && (!pkAC->m_kData.m_bOnGround))
  {
    pkGeomObject                = m_ppkGeomObjects[GEOM_OBJECT_AGL_ALTITUDE_SCALE];
    pkGeomObject->SetTransform(pkDevice, &(m_kAGLAltitudeScaleMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->Render(pkDevice);
    pkGeomObject->SetTransform(pkDevice, &m_kHUDTransformMatrix);

    for(UINT i = 0; i < NUM_AGL_ALTITUDE_SCALE_TABLE_ENTRIES; ++i)
    {
      const AGL_ALTITUDE_SCALE *pkAGLAltitudeScaleEntry = &g_akAGLAltitudeScaleTable[i];
      pkFont->RenderTextExt(pkDevice, &pkAGLAltitudeScaleEntry->s[0], (D3DXVECTOR3 *)&pkAGLAltitudeScaleEntry->kVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_LEFT);
    }

    FLOAT fBarScale = pkAC->m_dRadarAltitude/100;
    pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_AGL_ALTITUDE_SCALE_BAR2];
    D3DXMATRIXA16 kAGLAltitudeScaleBarTranslationMatrix;
    D3DXMatrixTranslation(&kAGLAltitudeScaleBarTranslationMatrix, 0, fBarScale*c_fAGLScaleDisp, 0);
    pkGeomObject->SetTransform(pkDevice, &(kAGLAltitudeScaleBarTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->Render(pkDevice);
    pkGeomObject->SetTransform(pkDevice, &(m_kHUDTransformMatrix));
    pkGeomObject->Render(pkDevice);

    //Adjust fBarScale to cover the line thickness (2*fHalfLineThickness)
    fBarScale += (2*fHalfLineThickness)/c_fAGLScaleDisp;
    D3DXMatrixTranslation(&kAGLAltitudeScaleBarTranslationMatrix, 0, -(g_kAGLAltitudeScaleVector.y - 11*c_fAGLScaleDisp)*(fBarScale - 1) - fHalfLineThickness, 0);
    D3DXMATRIXA16 kAGLAltitudeScaleBarScalingMatrix;
    D3DXMatrixScaling(&kAGLAltitudeScaleBarScalingMatrix, 1, fBarScale, 1);
    pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_AGL_ALTITUDE_SCALE_BAR];
    pkGeomObject->SetTransform(pkDevice, &(kAGLAltitudeScaleBarScalingMatrix*kAGLAltitudeScaleBarTranslationMatrix*m_kHUDTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, NULL);
    pkGeomObject->Render(pkDevice);


  }

  /////////////////////////////////////////////////
  // ALTITUDE VALUE (MSL)
  /////////////////////////////////////////////////
  pkGeomObject->SetTransform(pkDevice, &m_kHUDTransformMatrix);
  sprintf_s(acStr, sizeof(acStr), "%#5.3f", (FLOAT)(iAltitude/1000.0f), __SVSPFont::ALIGN_RIGHT);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kMSLAltitudeValueVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);

  /////////////////////////////////////////////////
  // ALTITUDE VALUE (AGL)
  /////////////////////////////////////////////////
  pkFont->RenderTextExt(pkDevice, "R", &m_kAGLDesignatorVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y);
  sprintf_s(acStr, sizeof(acStr), "%#5.3f", (FLOAT)(pkAC->m_dRadarAltitude/1000.0), __SVSPFont::ALIGN_RIGHT);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kAGLAltitudeValueVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_AGL_ALTITUDE_BOX];
  pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_HDG]);
  pkGeomObject->Render(pkDevice);

  /////////////////////////////////////////////////
  // LOW ALTITUDE VALUE
  /////////////////////////////////////////////////
  if(!(((pkAC->m_dRadarAltitude) < FLCS::BINGO_ALTITUDE) && (pkACData->m_dGearPosition == 0) && !__SVSPAvionics::g_bWarnClockEdgeUP))
  {
    pkFont->RenderTextExt(pkDevice, "AL", &m_kALDesignatorVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y);
    sprintf_s(acStr, sizeof(acStr), "%d", (INT)FLCS::BINGO_ALTITUDE, __SVSPFont::ALIGN_RIGHT);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kALValueVector, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y|__SVSPFont::ALIGN_RIGHT);
  }
}

VOID HUD::RenderAttentionAreas(LPD3DXDEVICE pkDevice)
{
  CHAR acStr[32];
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  //ECS *pkECS = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  //AAV_ASSERT_RETURN_VOID(pkECS  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkACData  != NULL);
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);
  VWS *pkVWS = static_cast<VWS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_VWS));

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;

  /////////////////////////////////////////////////
  // HEADING, AIRSPEED AND ALTITIDE BOXES
  /////////////////////////////////////////////////
  //if(!(((pkACData->m_dIndicatedAirspeed) < 130) && (pkACData->m_bOnGround == 0) && !__SVSPAvionics::g_bWarnClockEdgeUP))
  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_AIRSPEED_BOX];
  pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_ALT_KIAS]);
  pkGeomObject->Render(pkDevice);
  //if(!(((pkACData->m_dIndicatedAltitude) < 300) && (pkACData->m_bOnGround == 0) && (pkACData->m_dGearPosition[0] == 0) && !__SVSPAvionics::g_bWarnClockEdgeUP))
  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_MSL_ALTITUDE_BOX];
  pkGeomObject->Render(pkDevice);

  /////////////////////////////////////////////////
  // AoA
  /////////////////////////////////////////////////
  //sprintf_s(acStr, sizeof(acStr), "%03.1f %03.1f", pkAC->m_fAoA, pkAC->m_fIncidenceAlpha);
  sprintf_s(acStr, sizeof(acStr), "%03.1f", pkAC->m_fAoA);
  //sprintf_s(acStr, sizeof(acStr), "%03.1f %d", pkAC->m_fAoA, pkECS->m_kData.dAfterburnerStageNum, pkECS->m_kData.dAfterburnerStageNum);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kAoAValueVector, __SVSPFont::FILTERED);

  /////////////////////////////////////////////////
  // MACH
  /////////////////////////////////////////////////
  sprintf_s(acStr, sizeof(acStr), "%3.2f", pkACData->m_dMach);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kMachValueVector, __SVSPFont::FILTERED);

  /////////////////////////////////////////////////
  // G
  /////////////////////////////////////////////////
  sprintf_s(acStr, sizeof(acStr), "%3.1f", pkACData->m_dG);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kGValueVector, __SVSPFont::FILTERED);

  /////////////////////////////////////////////////
  // MAX G
  /////////////////////////////////////////////////
  sprintf_s(acStr, sizeof(acStr), "%3.1f", pkACData->m_dMaxG);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kMaxGValueVector, __SVSPFont::FILTERED);

  if(pkVWS != NULL) 
  {
    /////////////////////////////////////////////////
    // FUEL WARNING
    /////////////////////////////////////////////////
    if((pkVWS->IsAssertionFailed(VWS::ASSERT_ID_BINGO_FUEL) || pkVWS->IsAssertionFailed(VWS::ASSERT_ID_FUEL_LOW)) && m_pkAvionics->g_bCautionClockEdgeUP)
    {
      pkFont->RenderTextExt(pkDevice, "FUEL", &m_kFuelWarningVector, __SVSPFont::FILTERED);
    }

    /////////////////////////////////////////////////
    // MASTER WARNING
    /////////////////////////////////////////////////
    if(pkVWS->IsAssertionFailed(VWS::ASSERT_ID_MASTER_WARNING) && m_pkAvionics->g_bWarnClockEdgeUP)
    {
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_WARN_BAR];
      pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_WARN]);
      pkGeomObject->SetTransform(pkDevice, &m_kHUDTransformMatrix);
      pkGeomObject->Render(pkDevice);
    }
  }
}

VOID HUD::RenderNAVMode(LPD3DXDEVICE pkDevice)
{
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);
  /////////////////////////////////////////////////
  // MASTER MODE STATE INDICATION
  /////////////////////////////////////////////////
  pkFont->RenderTextExt(pkDevice, "NAV", &m_kMasterModeStateIndicationVector, __SVSPFont::FILTERED);

}

VOID HUD::RenderFCRMode(LPD3DXDEVICE pkDevice)
{
  CHAR acStr[32];
  SMS *pkSMS = static_cast<SMS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS));
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);
  /////////////////////////////////////////////////
  // MASTER MODE STATE INDICATION
  /////////////////////////////////////////////////
  if((pkSMS != NULL) && (pkSMS->GetSelectedWeapon() != NULL))
  {
    WEAPON::WEAPON_GROUP *pkSelectedWeaponGroup = (pkSMS->GetSelectedWeapon())->GetWeaponGroup();
    sprintf_s(acStr, sizeof(acStr), "%s %d", pkSelectedWeaponGroup->GetName(), pkSelectedWeaponGroup->GetAmount());
    pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kMasterModeStateIndicationVector, __SVSPFont::FILTERED);
  }
  else
  {
    pkFont->RenderTextExt(pkDevice, "FCR", &m_kMasterModeStateIndicationVector, __SVSPFont::FILTERED);
  }
}

VOID HUD::RenderT2WScale(LPD3DXDEVICE pkDevice)
{
#if 0
  /////////////////////////////////////////////////
  // SIM THRUST TO WEIGHT RATIO SCALE
  /////////////////////////////////////////////////
  //if(pkAC->m_kInternalSimData.m_bHoverMode)
  if(pkAC->m_bInternalSimOn)
  {
    sprintf_s(acStr, sizeof(acStr), "%1.2f", (FLOAT)pkAC->m_kInternalSimData.m_dLiftToWeightRatio);
    pkFont->RenderTextExt(pkDevice, &acStr[0], &D3DXVECTOR3(0.75f, 0, 0), __SVSPFont::FILTERED|__SVSPFont::CENTERED_X, 120.0f);
    pkGeomObject->Render(m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_SCALE], D3DXGEOMETRYOBJECT::LINESTRIP, 0, NUM_THRUST_TO_WEIGHT_RATIO_SCALE_VERTS - 1);

    LPCUSTOMVERTEX pVerts = D3DXGEOMETRYOBJECT::GET_VERTS(m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_BAR]);
    pVerts[0].v.y = pVerts[1].v.y = (FLOAT)pkAC->m_kInternalSimData.m_dLiftToWeightRatio*3.0f/2.0f - 1.5f;
    D3DXGEOMETRYOBJECT::UPDATE(m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_BAR]);
    pkGeomObject->Render(m_ppkGeomObjects[GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_BAR], D3DXGEOMETRYOBJECT::TRIANGLESTRIP, 0, NUM_THRUST_TO_WEIGHT_RATIO_BAR_VERTS - 2);
  }
#endif //0
}

VOID HUD::RenderMissionData(LPD3DXDEVICE pkDevice)
{
  CHAR acStr[32];
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkACData  != NULL);
  __SVSPFont           *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);
  SMS *pkSMS = static_cast<SMS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS));

  // LATITUDE
  FLOAT fLatitude   = pkAC->m_kLocation.Latitude;
  CHAR  cLat        = (fLatitude > 0) ? 'N' : 'S';
        fLatitude   = abs(fLatitude);
  INT   iLatDegrees = (INT)fLatitude;
  FLOAT fLatMinutes = 60*(fLatitude - iLatDegrees);
  sprintf_s(acStr, sizeof(acStr), "LAT:  %c%03d.%02.02f'", cLat, iLatDegrees, fLatMinutes);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kLATValueVector, __SVSPFont::FILTERED);

  // LONGITUDE
  FLOAT fLongitude  = pkAC->m_kLocation.Longitude;
  CHAR  cLon        = (fLongitude > 0) ? 'E' : 'W';
        fLongitude  = abs(fLongitude);
  INT   iLonDegrees = (INT)fLongitude;
  FLOAT fLonMinutes = 60*(fLongitude - iLonDegrees);
  sprintf_s(acStr, sizeof(acStr), "LON:  %c%03d.%02.02f'", cLon, iLonDegrees, fLonMinutes);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kLNGValueVector, __SVSPFont::FILTERED);

  // ELEVATION
  FLOAT fElevation  = pkACData->m_dIndicatedAltitude - pkAC->m_dRadarAltitude;
  sprintf_s(acStr, sizeof(acStr), "ELV:  %d FT", (INT)fElevation);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kELVValueVector, __SVSPFont::FILTERED);

  // TOS
  FLOAT fSeconds    = pkACData->m_dLocalTimeSeconds;
  INT   iHours      = (INT)(fSeconds/3600);
  INT   iMinutes    = ((INT)(fSeconds/60))%60;
  INT   iSeconds    = ((INT)fSeconds)%60;
  sprintf_s(acStr, sizeof(acStr), "TOS:  %02d:%02d:%02d", iHours, iMinutes, iSeconds);
  pkFont->RenderTextExt(pkDevice, &acStr[0], &m_kTOSValueVector, __SVSPFont::FILTERED);
}

VOID HUD::Render(LPD3DXDEVICE pkDevice)
{
  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  FCR *pkFCR = static_cast<FCR *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FCR));
  SMS *pkSMS = static_cast<SMS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS));
  WPN *pkWPN = static_cast<WPN *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_WPN));

  //Read Frame Critical data.
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  pkAC->ReadCriticalData();

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_HUD_STENCIL_CIRCLED
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_HUD_STENCIL_CIRCLED];
  AAV_ASSERT_RETURN_VOID(pkGeomObject  != NULL);

  if(m_eMode == HUD::DISPLAY_MODE_HMD)
  {
    // Set Transform to View Matrix.
    pkDevice->GetTransform(D3DTS_VIEW, &m_kViewMatrix);

    m_kViewMatrix._41 = m_kViewMatrix._14 = 0;
    m_kViewMatrix._42 = m_kViewMatrix._24 = 0;
    m_kViewMatrix._43 = m_kViewMatrix._34 = 0;
    D3DXMatrixTranspose(&m_kTransposedViewMatrix, &m_kViewMatrix);

    m_kStencilTransformMatrix = m_kOPMatrix*m_kStencilScalingMatrix*m_kTransposedViewMatrix;

    pkDevice->SetRenderState(D3DRS_ZENABLE,          D3DZB_FALSE);
    pkDevice->SetRenderState(D3DRS_ZWRITEENABLE,     TRUE);
    pkDevice->SetRenderState(D3DRS_ZFUNC,            D3DCMP_LESSEQUAL);

    pkDevice->SetRenderState(D3DRS_STENCILREF,       2);
    pkDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_ALWAYS);
    pkDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE);
    pkDevice->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
  }
  else //m_eMode == HUD::DISPLAY_MODE_HUD
  {
    m_kStencilTransformMatrix = m_kOPMatrix*m_kStencilScalingMatrix*pkAC->m_kBodyToLocalTransformMatrix;

    pkDevice->SetRenderState(D3DRS_ZENABLE,          D3DZB_TRUE);
    pkDevice->SetRenderState(D3DRS_ZWRITEENABLE,     FALSE);
    pkDevice->SetRenderState(D3DRS_ZFUNC,            D3DCMP_LESSEQUAL);

    pkDevice->SetRenderState(D3DRS_STENCILREF,       1);
    pkDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_EQUAL);
    pkDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_INCRSAT);
    pkDevice->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
  }

  pkGeomObject->SetTransform(pkDevice, &m_kStencilTransformMatrix);
  pkGeomObject->SetTexture(pkDevice, NULL);
  pkGeomObject->Render(pkDevice);


  //pkDevice->SetRenderState(D3DRS_ZWRITEENABLE,     TRUE);


  pkDevice->SetRenderState(D3DRS_STENCILREF,       2);
  pkDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_EQUAL);
  pkDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_KEEP);
  pkDevice->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);

  ///////////////////////////////////////////////////////////////////
  // RENDER HUD ELEMENTS
  ///////////////////////////////////////////////////////////////////
  m_kHUDTransformMatrix = m_kOPMatrix*pkAC->m_kBodyToLocalTransformMatrix;

  FCR::STATE eFCRState = (pkFCR != NULL)? pkFCR->GetState() : FCR::FCR_STATE_OFF;
  switch(eFCRState)
  {
  case FCR::FCR_STATE_OFF:
    RenderNoseDirectionMarker(pkDevice);
    RenderFlightPathMarker   (pkDevice); // (!) REDEFINES m_kHUDTransformMatrix (!)
    RenderBankPointer        (pkDevice);
    RenderSteeringData       (pkDevice);
    RenderILSData            (pkDevice);
    RenderPitchScale         (pkDevice);
    RenderHeadingScale       (pkDevice);
    RenderAoABracket         (pkDevice);
    RenderAirspeedScale      (pkDevice);
    RenderAltitudeScale      (pkDevice);
    RenderAttentionAreas     (pkDevice);
    RenderNAVMode            (pkDevice);
    RenderMissionData        (pkDevice);
    break;

  case FCR::FCR_STATE_ON:
    RenderNoseDirectionMarker(pkDevice);
    RenderFlightPathMarker   (pkDevice); // (!) REDEFINES m_kHUDTransformMatrix (!)
    RenderBankPointer        (pkDevice);
    RenderSteeringData       (pkDevice);
    RenderPitchScale         (pkDevice);
    RenderHeadingScale       (pkDevice);
    RenderAoABracket         (pkDevice);
    RenderAirspeedScale      (pkDevice);
    RenderAltitudeScale      (pkDevice);
    RenderAttentionAreas     (pkDevice);
    RenderFCRMode            (pkDevice);
    RenderMissionData        (pkDevice);
    break;

  case FCR::FCR_STATE_LOCKED:
    RenderNoseDirectionMarker(pkDevice);
    RenderFlightPathMarker   (pkDevice); // (!) REDEFINES m_kHUDTransformMatrix (!)
    RenderBankPointer        (pkDevice);
    RenderLockedTargetData   (pkDevice);
    RenderPitchScale         (pkDevice);
    RenderHeadingScale       (pkDevice);
    RenderAoABracket         (pkDevice);
    RenderAirspeedScale      (pkDevice);
    RenderAltitudeScale      (pkDevice);
    RenderAttentionAreas     (pkDevice);
    RenderFCRMode            (pkDevice);
    RenderMissionData        (pkDevice);
    break;
  }

  // FCR
  if(pkFCR != NULL)
  {
    pkFCR->RenderOnHUD(pkDevice, m_kOPMatrix);
  }

  // SMS
  if(pkSMS != NULL)
  {
    pkSMS->RenderOnHUD(pkDevice, m_kOPMatrix);
  }

  // WPN
  if(pkWPN != NULL)
  {
    pkWPN->RenderOnHUD(pkDevice, m_kOPMatrix);
  }

}


VOID HUD::SetScale(FLOAT fScale)
{
  m_kOPMatrix._11 = m_kOPMatrix._22 = fScale;
}

VOID HUD::SetProjectorScale(FLOAT fScale)
{
  m_kStencilScalingMatrix._11 = m_kStencilScalingMatrix._22 = fScale;
}

VOID HUD::IncreaseScale()
{
  FLOAT fScale    = m_kOPMatrix._11;
  fScale          = (fScale < 1.0f) ? fScale + 0.0005f : fScale;
  m_kOPMatrix._11 = m_kOPMatrix._22 = fScale;
}

VOID HUD::DecreaseScale()
{
  FLOAT fScale    = m_kOPMatrix._11;
  fScale          = (fScale > 0.01f) ? fScale - 0.0005f : fScale;
  m_kOPMatrix._11 = m_kOPMatrix._22  = fScale;
}

VOID HUD::IncreaseProjectorScale()
{
  FLOAT fScale    = m_kStencilScalingMatrix._11;
  fScale          = (fScale < 2.0f) ? fScale + 0.0005f : fScale;
  m_kStencilScalingMatrix._11 = m_kStencilScalingMatrix._22 = fScale;
}

VOID HUD::DecreaseProjectorScale()
{
  FLOAT fScale    = m_kStencilScalingMatrix._11;
  fScale          = (fScale > 0.01f) ? fScale - 0.0005f : fScale;
  m_kStencilScalingMatrix._11 = m_kStencilScalingMatrix._22  = fScale;
}

VOID HUD::SetMode(HUD::DISPLAY_MODE eMode)
{
  AAV_ASSERT_RETURN_VOID(m_eMode < MAX_NUM_DISPLAY_MODES);

  ID kID = 0;
  switch(eMode)
  {
  case HUD::DISPLAY_MODE_HUD:
    kID = check_named_variable("HUD_VISIBILITY_ID");
    set_named_variable_value( kID, (BOOL)1 );
    kID = check_named_variable("HMD_VISIBILITY_ID");
    set_named_variable_value( kID, (BOOL)0 );
    m_kHMDStencilScalingMatrix = m_kStencilScalingMatrix;
    m_kStencilScalingMatrix    = m_kHUDStencilScalingMatrix;
    break;
  case HUD::DISPLAY_MODE_HMD:
    kID = check_named_variable("HUD_VISIBILITY_ID");
    set_named_variable_value( kID, (BOOL)0 );
    kID = check_named_variable("HMD_VISIBILITY_ID");
    set_named_variable_value( kID, (BOOL)1 );
    m_kHUDStencilScalingMatrix = m_kStencilScalingMatrix;
    m_kStencilScalingMatrix    = m_kHMDStencilScalingMatrix;
    break;
  }
  m_eMode = eMode;
}




VOID HUD::Process()
{
  MSG       kMsg;
  HOTAS *pkHOTAS = static_cast<HOTAS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HOTAS));

  //TODO:MOVE TO GLOBAL HOTAS CONFIG
  if(pkHOTAS != NULL)
  {
    if((pkHOTAS->m_kJS.rgbButtons[16] & 0x80) == 0x80)
    {
      if((pkHOTAS->m_kJS.rgbButtons[2] & 0x80) == 0x80)
      {
        IncreaseProjectorScale();
      }
      else
      {
        IncreaseScale();
      }
    }
    else
    if((pkHOTAS->m_kJS.rgbButtons[14] & 0x80) == 0x80)
    {
      if((pkHOTAS->m_kJS.rgbButtons[2] & 0x80) == 0x80)
      {
        DecreaseProjectorScale();
      }
      else
      {
        DecreaseScale();
      }
    }
  }

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case HUD::MESSAGE_SCALE_INCREASE:
      IncreaseScale();
      break;

    case HUD::MESSAGE_SCALE_DECREASE:
      DecreaseScale();
      break;

    case HUD::MESSAGE_STENCIL_SCALE_INCREASE:
      IncreaseProjectorScale();
      break;

    case HUD::MESSAGE_STENCIL_SCALE_DECREASE:
      DecreaseProjectorScale();
      break;
    }
  }
}
