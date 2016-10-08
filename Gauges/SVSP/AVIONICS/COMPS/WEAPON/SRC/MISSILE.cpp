#include "MISSILE.h"
#include "AC.h"
#include "SMS.h"
#include "HUD.h"

const FLOAT               MISSILE::MASS                                   = 190*0.45359237f;
const FLOAT               MISSILE::FUSE_DETONATION_DISTANCE               = 30;
const FLOAT               MISSILE::MIN_AIRSPEED                           = 740;
const FLOAT               MISSILE::MAX_AIRSPEED                           = 2200;
const FLOAT               MISSILE::MAX_G                                  = 22;
const FLOAT               MISSILE::MAX_THRUST                             = 2200;
const FLOAT               MISSILE::TIME_TO_SELFDISTRUCT                   = 25;
const FLOAT               MISSILE::MIN_LOCK_DISTANCE                      = 1000;
const FLOAT               MISSILE::MAX_LOCK_DISTANCE                      = 25000;
const FLOAT               MISSILE::EFFECTIVE_LOCK_DISTANCE                = 5000;
const FLOAT               MISSILE::PROPELANT_BURNOUT_TIME                 = 45;
const FLOAT               MISSILE::MIN_COS_OF_SEEKER_ASPECT_ANGLE         = cos((45.0f/180.0f)*D3DX_PI);  //45 Degrees.
const FLOAT               MISSILE::MIN_COS_OF_LOCKED_TARGET_ASPECT_ANGLE  = cos(( 1.5f/180.0f)*D3DX_PI);  //03 Degrees.
const AAVVector3<FLOAT>  MISSILE::AIR_RESISTANCE_FACTOR                  = AAVVector3<FLOAT>(0.15f, 0.15f, MISSILE::MAX_THRUST/4840000);


MISSILE::MISSILE(CHAR *pcName, CHAR *pcType, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude) :
WEAPON(pcName, pcType, uiStationID, uiUnitID, uiWeight, pkAvionics, rkPilonVector, rkPilonAttitude, NULL, MISSILE::GEOM_OBJECT_NUM_OBJECTS, NULL, MISSILE::TEXTURE_OBJECT_NUM_OBJECTS),
  c_fMass                            (MISSILE::MASS),
  c_fFuseDetonationDistance          (MISSILE::FUSE_DETONATION_DISTANCE),
  c_fMinAirspeed                     (MISSILE::MIN_AIRSPEED),
  c_fMaxAirspeed                     (MISSILE::MAX_AIRSPEED),
  c_fMaxG                            (MISSILE::MAX_G),
  c_fMaxTrust                        (MISSILE::MAX_THRUST),
  c_fTimeToSelfDestruct              (MISSILE::TIME_TO_SELFDISTRUCT),
  c_fMinLockDistance                 (MISSILE::MIN_LOCK_DISTANCE),
  c_fMaxLockDistance                 (MISSILE::MAX_LOCK_DISTANCE),
  c_fEffectiveLockDistance           (MISSILE::EFFECTIVE_LOCK_DISTANCE),
  c_fPropelantBurnoutTime            (MISSILE::PROPELANT_BURNOUT_TIME),
  c_fMinCosOfSeekerAspectAngle       (MISSILE::MIN_COS_OF_SEEKER_ASPECT_ANGLE),
  c_fMinCosOfLockedTargetAspectAngle (MISSILE::MIN_COS_OF_LOCKED_TARGET_ASPECT_ANGLE),
  c_kAirResistanceFactor             (MISSILE::AIR_RESISTANCE_FACTOR)
{
  m_dwTargetMissedTime           = 0;
  m_fCosOfSeekerAspectAngle      = 0;
  m_fLunchEfficiencyFactor       = 0;
  m_fStateUpdateDeltaTime        = 0.02f;
  m_fSpeedAspectRatio            = 0.0f;
  memset(&m_kTarget, 0, sizeof(OBJECT));

  ///////////////////////////////////////////////////////////////////
  // DATA_ID_READ_TARGET_OBJECT_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "SIM ON GROUND",                       "Bool",            SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "PLANE LATITUDE",                      "degrees",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "PLANE LONGITUDE",                     "degrees",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "PLANE ALTITUDE",                      "feet",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "VELOCITY BODY X",                     "Feet per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "VELOCITY BODY Y",                     "Feet per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "VELOCITY BODY Z",                     "Feet per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "PLANE HEADING DEGREES TRUE",          "radians",         SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "AIRSPEED INDICATED",                  "knots",           SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_TARGET_OBJECT_DATA, "INDICATED ALTITUDE",                  "Feet",            SIMCONNECT_DATATYPE_INT32);

  ///////////////////////////////////////////////////////////////////
  // DATA_ID_SET_AUX_OBJECT_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, DATA_ID_SET_AUX_OBJECT_DATA, "SMOKE ENABLE",              "Bool",               SIMCONNECT_DATATYPE_FLOAT32);


  //Reset seeker direction
  m_kBodySeekerDirectionVector = UNIT_VECTOR;


  LPD3DXGEOMETRYOBJECT    pkGeomObject = NULL;
  __SVSPVertex*           pVerts       = NULL;

  UINT i;
  FLOAT r;
  FLOAT a;
  FLOAT fGradScale;

  ///////////////////////////////////////////////////////////////////
  // BORESITE VIEW DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_BORESITE_VIEW_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(0.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  r = 2.2f;
  a = 0.1f;

  fGradScale = 360.0f/(NUM_BORESITE_VIEW_DESIGNATOR_VERTICES/2 - 1);
  for(i = 0; i < (NUM_BORESITE_VIEW_DESIGNATOR_VERTICES/2 - 1); ++i)
  {
    pVerts[2*i  ] = __SVSPVertex(r*cos((D3DX_PI/180.0f)*i*fGradScale),       r*sin((D3DX_PI/180.0f)*i*fGradScale),       0.0f, 0, 0, __SVSPColor::dwHUDCOLOR);
    pVerts[2*i+1] = __SVSPVertex((r - a)*cos((D3DX_PI/180.0f)*i*fGradScale), (r - a)*sin((D3DX_PI/180.0f)*i*fGradScale), 0.0f, 0, 0, __SVSPColor::dwHUDCOLOR);
  }
  pVerts[2*i  ] = pVerts[0];
  pVerts[2*i+1] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_BORESITE_VIEW_DESIGNATOR] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_SEEKER_DIRECTION_DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_SEEKER_DIRECTION_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(0.0f, 1.0f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  pVerts[0] = __SVSPVertex(-0.6f,  0.6f, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( 0.6f,  0.6f, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-0.6f, -0.6f, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( 0.6f, -0.6f, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_SEEKER_DIRECTION_DESIGNATOR] = pkGeomObject;

  m_ppkTextureObjects[TEXTURE_OBJECT_SEEKER_DIRECTION_DESIGNATOR] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/SIM/WEAPON/RES/TEXTURES/SDD.dds");
  m_ppkTextureObjects[TEXTURE_OBJECT_LOCKED_TARGET_DESIGNATOR]    = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/SIM/WEAPON/RES/TEXTURES/LTD.dds");

  FLOAT fDLZBarHalfWidth  = 0.1f;
  FLOAT fDLZBarHalfHeight = 1.2f;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_DLZ_BAR_R1
  ///////////////////////////////////////////////////////////////////
  DWORD R1Color =  ((0x33 << 24) | __SVSPColor::dwHUDCOLOR & 0x00FFFFFF);
  D3DXMatrixTranslation(&m_kDLZ_R1Matrix, 0.6f, 0, 0);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_DLZ_BAR_R1_VERTICES);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-fDLZBarHalfWidth,  fDLZBarHalfHeight, 0, 0, 0, R1Color);
  pVerts[1] = __SVSPVertex( fDLZBarHalfWidth,  fDLZBarHalfHeight, 0, 0, 0, R1Color);
  pVerts[2] = __SVSPVertex(-fDLZBarHalfWidth, -fDLZBarHalfHeight, 0, 0, 0, R1Color);
  pVerts[3] = __SVSPVertex( fDLZBarHalfWidth, -fDLZBarHalfHeight, 0, 0, 0, R1Color);
  m_ppkGeomObjects[GEOM_OBJECT_DLZ_BAR_R1] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_DLZ_BAR_R2
  ///////////////////////////////////////////////////////////////////
  DWORD R2Color =  ((0xDD << 24) | __SVSPColor::dwHUDCOLOR & 0x00FFFFFF);
  D3DXMatrixTranslation(&m_kDLZ_R2Matrix, 0.6f, 0, 0);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_DLZ_BAR_R2_VERTICES);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-fDLZBarHalfWidth,  fDLZBarHalfHeight, 0, 0, 0, R2Color);
  pVerts[1] = __SVSPVertex( fDLZBarHalfWidth,  fDLZBarHalfHeight, 0, 0, 0, R2Color);
  pVerts[2] = __SVSPVertex(-fDLZBarHalfWidth, -fDLZBarHalfHeight, 0, 0, 0, R2Color);
  pVerts[3] = __SVSPVertex( fDLZBarHalfWidth, -fDLZBarHalfHeight, 0, 0, 0, R2Color);
  m_ppkGeomObjects[GEOM_OBJECT_DLZ_BAR_R2] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_DLZ_DESIGNATOR_R1
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_DLZ_DESIGNATOR_R1_VERTICES);
  pVerts = pkGeomObject->GetVerts();

  r = 1.2f;
  a = 0.2f;

  fGradScale = 360.0f/(NUM_DLZ_DESIGNATOR_R1_VERTICES/2 - 1);
  for(i = 0; i < (NUM_DLZ_DESIGNATOR_R1_VERTICES/2 - 1); ++i)
  {
    pVerts[2*i  ] = __SVSPVertex(      r*sin((D3DX_PI/180.0f)*i*fGradScale),       r*cos((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, R1Color);
    pVerts[2*i+1] = __SVSPVertex((r - a)*sin((D3DX_PI/180.0f)*i*fGradScale), (r - a)*cos((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, R1Color);
  }
  pVerts[2*i  ] = pVerts[0];
  pVerts[2*i+1] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_DLZ_DESIGNATOR_R1] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_DLZ_DESIGNATOR_R2
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_DLZ_DESIGNATOR_R2_VERTICES);
  pVerts = pkGeomObject->GetVerts();

  fGradScale = 360.0f/(NUM_DLZ_DESIGNATOR_R2_VERTICES/2 - 1);
  for(i = 0; i < (NUM_DLZ_DESIGNATOR_R2_VERTICES/2 - 1); ++i)
  {
    pVerts[2*i  ] = __SVSPVertex(      r*sin((D3DX_PI/180.0f)*i*fGradScale),       r*cos((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, R2Color);
    pVerts[2*i+1] = __SVSPVertex((r - a)*sin((D3DX_PI/180.0f)*i*fGradScale), (r - a)*cos((D3DX_PI/180.0f)*i*fGradScale), 0, 0, 0, R2Color);
  }
  pVerts[2*i  ] = pVerts[0];
  pVerts[2*i+1] = pVerts[1];
  m_ppkGeomObjects[GEOM_OBJECT_DLZ_DESIGNATOR_R2] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // TEXTURE_OBJECT_WEAPON_IMAGE
  ///////////////////////////////////////////////////////////////////
  m_ppkTextureObjects[TEXTURE_OBJECT_WEAPON_IMAGE] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/SIM/WEAPON/RES/TEXTURES/MISSILE.dds");

}

MISSILE::~MISSILE()
{
  WEAPON::Stop();
}

HRESULT MISSILE::SetState(WEAPON::STATE eState)
{
  return WEAPON::SetState(eState);
}

VOID MISSILE::SetTargetID(DWORD dwObjectID)
{
  m_kTarget.dwObjectID = dwObjectID;

  if(m_kTarget.dwObjectID != 0xFFFFFFFF)
  {
    m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, MISSILE::REQUEST_ID_READ_TARGET_OBJECT_DATA, MISSILE::DATA_ID_READ_TARGET_OBJECT_DATA, m_kTarget.dwObjectID, SIMCONNECT_PERIOD_SIM_FRAME);
    AAV_ASSERT(SUCCEEDED(m_hResult));
  }
  else
  {
    m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, MISSILE::REQUEST_ID_READ_TARGET_OBJECT_DATA, MISSILE::DATA_ID_READ_TARGET_OBJECT_DATA, m_kTarget.dwObjectID, SIMCONNECT_PERIOD_NEVER);
    AAV_ASSERT(SUCCEEDED(m_hResult));
  }
}

LPD3DXTEXTUREOBJECT MISSILE::GetWeaponTexture() const
{
  return m_ppkTextureObjects[TEXTURE_OBJECT_WEAPON_IMAGE];
}

VOID MISSILE::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
{
  switch(pData->dwID)
  {
  case SIMCONNECT_RECV_ID_ASSIGNED_OBJECT_ID:
    {
      SIMCONNECT_RECV_ASSIGNED_OBJECT_ID *pObjData = (SIMCONNECT_RECV_ASSIGNED_OBJECT_ID *)pData;
      if((pObjData->dwRequestID == WEAPON::REQUEST_ID_CREATE_OBJECT) && (m_dwObjectID != 0))
      {
        m_kSetAuxObjectStateData.bSmokeEnable = true;
        m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, MISSILE::DATA_ID_SET_AUX_OBJECT_DATA, m_dwObjectID, 0, 1, sizeof(MISSILE::SET_AUX_OBJECT_STATE_DATA), &m_kSetAuxObjectStateData);
        AAV_ASSERT(SUCCEEDED(m_hResult));
      }
    }
    break;

  case SIMCONNECT_RECV_ID_EVENT_OBJECT_ADDREMOVE:
    {
      SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE *pEvt = (SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE*)pData;
      switch(pEvt->uEventID)
      {
      case WEAPON::EVENT_ID_OBJECT_ADDED:
        break;

      case WEAPON::EVENT_ID_OBJECT_REMOVED:
        if(m_kTarget.dwObjectID == pData->dwID)
        {
          m_kTarget.dwObjectID = 0xFFFFFFFF;

          if(m_eState < WEAPON::WEAPON_STATE_LAUNCH)
          {
            SetState(WEAPON::WEAPON_STATE_IDLE);
          }
          else if((m_eState >= WEAPON::WEAPON_STATE_LAUNCH) && (m_eState < WEAPON::WEAPON_STATE_MISSED))
          {
            SetState(WEAPON::WEAPON_STATE_MISSED);
          }
        }
        break;

      default:
        break;
      }
    }
    break;

  case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
    {
      SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
      if((pObjData->dwRequestID == WEAPON::REQUEST_ID_READ_TARGET_OBJECT_DATA))
      {
        __SVSPModule::READ_OBJECT_DATA *pData = (__SVSPModule::READ_OBJECT_DATA *)&pObjData->dwData;

        if(pObjData->dwObjectID == m_kTarget.dwObjectID)
        {
          memcpy(&(m_kTarget.kData), pData, sizeof(__SVSPModule::READ_OBJECT_DATA));
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

  WEAPON::DispatchProcess(pData, cbData);
}


VOID MISSILE::UpdateTargetData()
{

}

VOID MISSILE::RotateSeekerTowardTargetDirection()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  ///////////////////////////////////////////////////////////
  //All rotations are performed in the body reference system.
  ///////////////////////////////////////////////////////////

  if(m_kTarget.dwObjectID != 0xFFFFFFFF)
  {
    D3DXVECTOR3   kBodySeekerRotationVector;
    D3DXVECTOR3   kBodySeekerRotationNormalizedVector;
    D3DXVECTOR3   kBodyNewSeekerDirectionVector;
    D3DXMATRIX    kBodySeekerRotationMatrix;

    //D3DXVECTOR3   kLocalTargetDirectionVector = *(pkFCR->GetLocalTargetDirectionVector());
    D3DXVECTOR3 kGlobalTargetOffsetVector             = (D3DXVECTOR3)m_kTarget.kGlobalVector - pkAC->m_kGlobalVector;
    D3DXVECTOR3 kLocalTargetDirectionVector           = *D3DXVec3TransformCoord(&kLocalTargetDirectionVector, &kGlobalTargetOffsetVector, &pkAC->m_kGlobalToLocalTransformMatrix);
    D3DXVECTOR3 kLocalTargetDirectionNormalizedVector = *D3DXVec3Normalize(&kLocalTargetDirectionNormalizedVector, &kLocalTargetDirectionVector);

    //Flip X axis in local ref system.
    kLocalTargetDirectionNormalizedVector.x = -kLocalTargetDirectionNormalizedVector.x;

    //Get target direction in the body ref system.
    D3DXVec3TransformCoord(&m_kBodyTargetDirectionVector, &kLocalTargetDirectionNormalizedVector, &pkAC->m_kLocalToBodyTransformMatrix);

    //Get target aspect angle
    m_fCosOfSeekerAspectAngle = D3DXVec3Dot(&m_kBodyTargetDirectionVector, &m_kBodySeekerDirectionVector);

    //Asses target aspect (whether the target is within the seekers cone).
    D3DXVECTOR3 *pkFinalSeekerDirectionVector = ((m_kBodyTargetDirectionVector.z > c_fMinCosOfSeekerAspectAngle) && 
                                                 (m_fDistanceToIntercept > c_fMinLockDistance) && 
                                                 (m_fDistanceToIntercept < c_fMaxLockDistance)) ? &m_kBodyTargetDirectionVector : (D3DXVECTOR3 *)&UNIT_VECTOR;

    //Get the normal vector of the plane defined by target direction and that of the seeker itself.
    D3DXVec3Cross(&kBodySeekerRotationVector, pkFinalSeekerDirectionVector, &m_kBodySeekerDirectionVector);
    D3DXVec3Normalize(&kBodySeekerRotationNormalizedVector, &kBodySeekerRotationVector);

    FLOAT fCosOfSeekerAspectAngle = D3DXVec3Dot(pkFinalSeekerDirectionVector, &m_kBodySeekerDirectionVector);
    FLOAT fSeekerRotationAngle    = -0.005f*(1 - 1/(1 + 50250.0f*(1 - fCosOfSeekerAspectAngle)));
    
    //Get seeker rotation matrix.
    __SVSPModule::GetRotationAroundUnitVector(&kBodySeekerRotationMatrix, &kBodySeekerRotationNormalizedVector, fSeekerRotationAngle);
    //Rotate seeker toward target.
    D3DXVec3TransformCoord(&kBodyNewSeekerDirectionVector, &m_kBodySeekerDirectionVector, &kBodySeekerRotationMatrix);
    //Normalize seeker direction vector again to asure its unit length after each rotation.
    D3DXVec3Normalize(&m_kBodySeekerDirectionVector, &kBodyNewSeekerDirectionVector);

    //TODO: Missile seeker alignment test.
    //m_kBodySeekerDirectionVector = D3DXVECTOR3(0, 0, 1);

    //Transform to local reference system.
    D3DXVECTOR3   kLocalSeekerDirectionVector;
    D3DXVec3TransformCoord(&kLocalSeekerDirectionVector, &m_kBodySeekerDirectionVector, &pkAC->m_kBodyToLocalTransformMatrix);

    //Flip X axis in local ref system.
    kLocalSeekerDirectionVector.x = -kLocalSeekerDirectionVector.x;

    //Get seeker direction matrix for local reference system.
    __SVSPModule::GetTransformMatrixFromLocalNormalizedVector(&m_kLocalSeekerDirectionMatrix, &kLocalSeekerDirectionVector);
  }
}


/////////////////////////////////////////////////////////
// WEAPON_STATE_IDLE
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStateIdle()
{
  MSG kMsg;
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case WEAPON::MESSAGE_ARM:
      if(m_kTarget.dwObjectID != 0xFFFFFFFF)
      {
        SetState(WEAPON::WEAPON_STATE_LOCKED);
      }
      else
      {
        m_fSelectBoresiteTime  = 0;
        SetState(WEAPON::WEAPON_STATE_ARMED);
      }
      break;
    }
  }
  m_pkSeekerMatrix = &pkAC->m_kBodyToLocalTransformMatrix;
}

/////////////////////////////////////////////////////////
// WEAPON_STATE_ARMED
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStateArmed()
{
  MSG        kMsg;

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  //if(m_pkSeekerMatrix == &pkAC->m_kBodyToLocalTransformMatrix)
  //{
  //  if((m_fSelectBoresiteTime += m_dDeltaTime) > 3.0f)
  //  {
  //    m_kTarget.dwObjectID  = 0xFFFFFFFF;
  //    m_fMaxCosAngularRes   = 0;
  //    m_fSelectBoresiteTime = 0;
  //    SetState(WEAPON::WEAPON_STATE_ACQUISITION);
  //  }
  //}

  if(m_kTarget.dwObjectID != 0xFFFFFFFF)
  {
    SetState(WEAPON::WEAPON_STATE_ACQUISITION);
  }

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case WEAPON::MESSAGE_UNARM:
      SetState(WEAPON::WEAPON_STATE_IDLE); 
      break;

      //HACK
    case WEAPON::MESSAGE_LAUNCH:
      SetState(WEAPON::WEAPON_STATE_LAUNCH);
      break;
    }
  }
}

/////////////////////////////////////////////////////////
// WEAPON_STATE_ACQUISITION
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStateAcquisition()
{
  MSG kMsg;
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  SMS *pkSMS = static_cast<SMS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS));
  AAV_ASSERT_RETURN_VOID(pkSMS  != NULL);

  if(m_kTarget.dwObjectID != 0xFFFFFFFF)
  {
    m_kVector                               = pkAC->m_kVector;
    m_kGlobalTargetInterseptVector          = m_kTarget.kGlobalVector;
    m_kGlobalTargetInterseptDirectionVector = m_kGlobalTargetInterseptVector - m_kVector;
    m_fDistanceToIntercept                  = m_kGlobalTargetInterseptDirectionVector.Length();
    //Check whether the seeker points within the locked target cone.
    if((m_fCosOfSeekerAspectAngle > c_fMinCosOfLockedTargetAspectAngle) && (m_fDistanceToIntercept > c_fMinLockDistance) && (m_fDistanceToIntercept < c_fMaxLockDistance))
    {
      SetState(WEAPON::WEAPON_STATE_LOCKED);
    }
  }

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case WEAPON::MESSAGE_UNARM:
      SetState(WEAPON::WEAPON_STATE_IDLE); 
      break;

      //HACK
    case WEAPON::MESSAGE_LAUNCH:
      SetState(WEAPON::WEAPON_STATE_LAUNCH);
      break;
    }
  }
}

/////////////////////////////////////////////////////////
// WEAPON_STATE_LOCKED
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStateLocked()
{
  MSG kMsg;
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  SMS *pkSMS = static_cast<SMS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS));
  AAV_ASSERT_RETURN_VOID(pkSMS  != NULL);

  if(m_kTarget.dwObjectID != 0xFFFFFFFF)
  {
    m_kVector                               = pkAC->m_kVector;
    m_kGlobalTargetInterseptVector          = m_kTarget.kGlobalVector;
    m_kGlobalTargetInterseptDirectionVector = m_kGlobalTargetInterseptVector - m_kVector;
    m_fDistanceToIntercept                  = m_kGlobalTargetInterseptDirectionVector.Length();

    //Check whether the seeker points within the locked target cone.
    if((m_fCosOfSeekerAspectAngle < c_fMinCosOfLockedTargetAspectAngle) || (m_fDistanceToIntercept <= c_fMinLockDistance) || (m_fDistanceToIntercept >= c_fMaxLockDistance))
    {
      SetState(WEAPON::WEAPON_STATE_ACQUISITION);
    }


    AAV_ASSERT_RETURN_VOID( (m_fDistanceToIntercept > c_fMinLockDistance) && (m_fDistanceToIntercept < c_fMaxLockDistance) );
    FLOAT fDX = m_fDistanceToIntercept - c_fEffectiveLockDistance;
    FLOAT fDY = -0.00004f*fDX*fDX/((m_fDistanceToIntercept - c_fMinLockDistance));
    m_fLunchEfficiencyFactor = exp(fDY);

    if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
    {
      switch(kMsg.message)
      {
      case WEAPON::MESSAGE_UNARM:
        m_kTarget.dwObjectID = 0;
        SetState(WEAPON::WEAPON_STATE_IDLE); 
        break;

      case WEAPON::MESSAGE_LAUNCH:
        SetState(WEAPON::WEAPON_STATE_LAUNCH);
        break;
      }
    }
  }
  else
  {
    //target lost, reset to boresite acquision mode
    //m_pkSeekerMatrix = &pkAC->m_kLocalToBodyTransformMatrix;
    SetState(WEAPON::WEAPON_STATE_ARMED);
  }
}

/////////////////////////////////////////////////////////
// WEAPON_STATE_LAUNCH
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStateLaunch()
{
  if(m_kTarget.dwObjectID != 0xFFFFFFFF)
  {
    AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
    AAV_ASSERT(pkAC  != NULL);

    m_dReleaseTime       = GetSystemTime();
    m_dTotalAirborneTime = 0;
    m_bValid             = true;
    m_kTargetPreviousVector = m_kTarget.kGlobalVector;
    m_fDistanceToIntercept          = 0;

    ///////////////////////////////////////////////////////////
    //Set state WEAPON::WEAPON_STATE_WAIT_LAUNCH_ACKNOWLEDGE
    ///////////////////////////////////////////////////////////
    m_kTargetPreviousVector = m_kTarget.kGlobalVector;
    SetState(WEAPON::WEAPON_STATE_WAIT_LAUNCH_ACKNOWLEDGE);

    ///////////////////////////////////////////////////////////
    // Create FSX object
    ///////////////////////////////////////////////////////////
    //AAVVector3<FLOAT> kLocalPilonVector   = pkAC->m_kBodyToLocalTransformMatrix*m_kPilonVector;
    //AAVVector3<FLOAT> kGlobalPilonVector  = pkAC->m_kLocalToGlobalTransformMatrix*kLocalPilonVector;
    D3DXVECTOR3 kLocalPilonVector     = *D3DXVec3TransformCoord(&kLocalPilonVector, &(D3DXVECTOR3)m_kPilonVector, &pkAC->m_kBodyToLocalTransformMatrix);
    kLocalPilonVector.x               = -kLocalPilonVector.x;
    D3DXVECTOR3 kGlobalPilonVector    = *D3DXVec3TransformCoord(&kGlobalPilonVector, &kLocalPilonVector, &pkAC->m_kLocalToGlobalTransformMatrix);
    AAVVector3<FLOAT> kGlobalVector = pkAC->m_kVector + AAVVector3<FLOAT>(kGlobalPilonVector);
    __SVSPGeoCoordinates3<FLOAT> kLocation  = __SVSPGeoCoordinates3<FLOAT>(kGlobalVector);
    //__SVSPGeoCoordinates3<FLOAT> kLocation  = __SVSPGeoCoordinates3<FLOAT>(pkAC->m_kVector);

    SIMCONNECT_DATA_INITPOSITION          kInitPosition;
    kInitPosition.Latitude              = (DOUBLE)(kLocation.Latitude);                                             ///Degrees
    kInitPosition.Longitude             = (DOUBLE)(kLocation.Longitude);                                            ///Degrees
    kInitPosition.Altitude              = (DOUBLE)(kLocation.Altitude * 3.28083989501f);                            ///Feet
    kInitPosition.Pitch                 = (DOUBLE)(180*(pkAC->m_kBodyAttitude.x / D3DX_PI) + m_kPilonAttitude.x);   ///Degrees
    kInitPosition.Bank                  = (DOUBLE)(180*(pkAC->m_kBodyAttitude.z / D3DX_PI) + m_kPilonAttitude.z);   ///Degrees
    kInitPosition.Heading               = (DOUBLE)(180*(pkAC->m_kBodyAttitude.y / D3DX_PI) + m_kPilonAttitude.y);   ///Degrees
    kInitPosition.OnGround              = (DWORD)0;
    kInitPosition.Airspeed              = (DWORD)pkAC->m_kData.m_dIndicatedAirspeed;

    m_kBodyVelocity                     = pkAC->m_kBodyVelocity;
    m_fAirspeed                         = m_kBodyVelocity.z;

    //--HACK--
    m_kSetObjectStateData.kBodyVelocity = m_kBodyVelocity;

    m_hResult = SimConnect_AICreateNonATCAircraft(m_hSimConnect, m_acName.getCStr(), "NK001", kInitPosition, WEAPON::REQUEST_ID_CREATE_OBJECT);
    AAV_ASSERT(SUCCEEDED(m_hResult));
  }
}

/////////////////////////////////////////////////////////
// WEAPON_STATE_WAIT_LAUNCH_ACKNOWLEDGE
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStateWaitLaunchAcknowledge()
{
  //NOP
}

/////////////////////////////////////////////////////////
// WEAPON_STATE_PURSUIT
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStatePursuit()
{
  //FLOAT fCosQx;
  //FLOAT fSinQx;
  //FLOAT fCosQy;
  //FLOAT fSinQy;
  MSG   kMsg;
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT(pkAC  != NULL);

  if(m_bValid)
  {
    //////////////////////////////////////////////////////////////////////////////////
    //// BODY ANGULAR VELOCITY (ROLL)
    //////////////////////////////////////////////////////////////////////////////////
    //D3DXVECTOR3 kBodyUserAcBankVec           = D3DXVECTOR3(cos(pkAC->m_kBodyAttitude.z),        sin(pkAC->m_kBodyAttitude.z), 0);
    //D3DXVECTOR3 kBodyAcBankVec               = D3DXVECTOR3(cos(m_kReadObjectStateData.kBodyAttitude.z), sin(m_kReadObjectStateData.kBodyAttitude.z), 0);
    //D3DXVECTOR3 kBodyAcRollVec               = *D3DXVec3Cross(&kBodyAcRollVec, &kBodyAcBankVec, &kBodyUserAcBankVec);
    //D3DXVECTOR3 kBodyAcRollNormalizedVec     = *D3DXVec3Normalize(&kBodyAcRollNormalizedVec, &kBodyAcRollVec);
    //FLOAT       fRollVelocity                = 132.1f*(1.0f - D3DXVec3Dot(&kBodyUserAcBankVec, &kBodyAcBankVec));
    //kBodyAcRollVec                           = fRollVelocity*kBodyAcRollNormalizedVec;

    //m_kSetObjectStateData.kBodyAngularVelocity = AAVVector3<FLOAT>(kBodyAcRotationVec.x, kBodyAcRotationVec.y, kBodyAcRollVec.z);

    //m_kSetObjectStateData.kBodyVelocity.x = m_kReadObjectStateData.kBodyVelocity.x;
    //m_kSetObjectStateData.kBodyVelocity.y = m_kReadObjectStateData.kBodyVelocity.y;
    //m_kSetObjectStateData.kBodyVelocity.z += (m_kReadObjectStateData.kBodyVelocity.z < 4000.0) ? 20.0 : 0.0;
    //m_kSetObjectStateData.kBodyVelocity.z += (m_kSetObjectStateData.kBodyVelocity.z < 4000.0) ? m_dTotalForwardAcceleration*m_dDeltaTime : 0.0;

    //m_kSetObjectStateData.kBodyVelocity.z = 0.0;

    //m_kSetObjectStateData.kBodyAngularVelocity = AAVVector3<FLOAT>(-0.5*m_kReadObjectStateData.kBodyAttitude.x, 
    //                                                                  -0.5*m_kReadObjectStateData.kBodyAttitude.y,
    //                                                                  -0.5*m_kReadObjectStateData.kBodyAttitude.z);
    //m_kSetObjectStateData.kBodyVelocity         = AAVVector3<FLOAT>(0, 
    //                                                                  0,
    //                                                                  0);

    //m_kSetObjectStateData.kBodyAttitude         = pkAC->m_kBodyAttitude;

    //SimConnect_SetDataOnSimObject(m_hSimConnect, MISSILE::DATA_ID_SET_OBJECT_STATE_DATA, m_dwObjectID, 0, 1, sizeof(MISSILE::SET_OBJECT_STATE_DATA), &m_kSetObjectStateData);

    if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
    {
      switch(kMsg.message)
      {
      case MISSILE::MESSAGE_REARM:
        SetState(WEAPON::WEAPON_STATE_IDLE);

        m_hResult = SimConnect_AIRemoveObject(m_hSimConnect, m_dwObjectID, MISSILE::REQUEST_ID_REMOVE_OBJECT);
        AAV_ASSERT(SUCCEEDED(m_hResult));
        break;
      }
    }

    if(m_kTarget.dwObjectID != 0xFFFFFFFF)
    {
      ////////////////////////////////////////////////////////////////////////////////////
      // TARGET INTERCEPT DIRECTION VECTOR IN THE BODY REFERENCE SYSTEM (LEAD-PERSUIT MODE)
      ////////////////////////////////////////////////////////////////////////////////////
      m_kGlobalTargetVelocityVector     = m_kTarget.kGlobalVector - m_kTargetPreviousVector;
      m_kTargetPreviousVector           = m_kTarget.kGlobalVector;
      if(m_dDeltaTime != 0)
      {
        m_fTargetVelocity               = m_kGlobalTargetVelocityVector.Length()/(FLOAT)m_dDeltaTime;
      }
      if(m_fTargetVelocity != 0)
      {
        m_kGlobalTargetNormalizedVelocityVector = m_kGlobalTargetVelocityVector/m_fTargetVelocity;
        m_kGlobalTargetInterseptVector          = m_kTarget.kGlobalVector + m_kGlobalTargetNormalizedVelocityVector*(m_fTargetVelocity*m_fStateUpdateDeltaTime);
      }
      else
      {
        m_kGlobalTargetInterseptVector          = m_kTarget.kGlobalVector;
      }
      m_kGlobalTargetInterseptDirectionVector           = m_kGlobalTargetInterseptVector - m_kVector;
      m_fDistanceToIntercept                            = m_kGlobalTargetInterseptDirectionVector.Length();

      m_kGlobalTargetInterseptDirectionNormalizedVector = m_kGlobalTargetInterseptDirectionVector/m_fDistanceToIntercept;
      m_kLocalTargetInterceptDirectionVector            = m_kGlobalToLocalTransformMatrix*m_kGlobalTargetInterseptDirectionNormalizedVector;
      m_kLocalTargetInterceptDirectionVector.x          = -m_kLocalTargetInterceptDirectionVector.x;
      m_kBodyTargetInterceptDirectionVector             = m_kLocalToBodyTransformMatrix*m_kLocalTargetInterceptDirectionVector;

      ////////////////////////////////////////////////////////////////////////////////////
      // BODY ANGULAR VELOCITY (PITCH, YAW)
      ////////////////////////////////////////////////////////////////////////////////////
      //m_kBodyAngularVelocity = (m_kBodyTargetInterceptDirectionVector.Cross(AAVVector3<FLOAT>(0, 0, 1))).Normalize();
      m_kBodyAngularVelocity = (AAVVector3<FLOAT>(0, 0, 1).Cross(m_kBodyTargetInterceptDirectionVector)).Normalize();
      m_fCosInterceptAngle    = m_kBodyTargetInterceptDirectionVector.Dot(AAVVector3<FLOAT>(0, 0, 1));
      FLOAT fWmax             = 85.0f;
      FLOAT fWmin             = 0.1f;
      m_fAngularVelocity      = (3*m_fAirspeed/c_fMaxAirspeed)*((fWmax - fWmin)*m_fCosInterceptAngle + (fWmax + fWmin))*(1.0f - m_fCosInterceptAngle)/2;
      m_kBodyAngularVelocity = m_fAngularVelocity*m_kBodyAngularVelocity;


      //////////////////////////////////////////////////////////////////////////////////
      //// BODY ANGULAR VELOCITY (PITCH, YAW)
      //////////////////////////////////////////////////////////////////////////////////
      //fCosQx = cos(pkAC->m_kBodyAttitude.x);
      //fSinQx = sin(pkAC->m_kBodyAttitude.x);
      //fCosQy = cos(pkAC->m_kBodyAttitude.y);
      //fSinQy = sin(pkAC->m_kBodyAttitude.y);
      ////D3DXVECTOR3 kLocalUserAcAttitudeVec = D3DXVECTOR3(fCosQx*fSinQy, -fSinQx, fCosQx*fCosQy);
      //AAVVector3<FLOAT> kLocalUserAcAttitudeVec3 = AAVVector3<FLOAT>(fCosQx*fSinQy, -fSinQx, fCosQx*fCosQy);

      //fCosQx = cos(m_kReadObjectStateData.kBodyAttitude.x);
      //fSinQx = sin(m_kReadObjectStateData.kBodyAttitude.x);
      //fCosQy = cos(m_kReadObjectStateData.kBodyAttitude.y);
      //fSinQy = sin(m_kReadObjectStateData.kBodyAttitude.y);
      ////D3DXVECTOR3 kLocalAcAttitudeVec = D3DXVECTOR3(fCosQx*fSinQy, -fSinQx, fCosQx*fCosQy);
      //AAVVector3<FLOAT> kLocalAcAttitudeVec3 = AAVVector3<FLOAT>(fCosQx*fSinQy, -fSinQx, fCosQx*fCosQy);

      ////D3DXVECTOR3 kLocalAcRotationVec          = *D3DXVec3Cross(&kLocalAcRotationVec, &kLocalAcAttitudeVec, &kLocalUserAcAttitudeVec);
      ////D3DXVECTOR3 kLocalAcRotationNormalizedVec = *D3DXVec3Normalize(&kLocalAcRotationNormalizedVec, &kLocalAcRotationVec);
      ////D3DXVECTOR3 kBodyAcRotationVec           = *D3DXVec3TransformCoord(&kBodyAcRotationVec, &kLocalAcRotationNormalizedVec, &m_kLocalToBodyTransformMatrix);
      ////FLOAT       fAngularVelocity             = 125.1f*(1.0f - D3DXVec3Dot(&kLocalUserAcAttitudeVec, &kLocalAcAttitudeVec));
      ////kBodyAcRotationVec                       = fAngularVelocity*kBodyAcRotationVec;

      //AAVVector3<FLOAT> kLocalAcRotationVec3           = kLocalAcAttitudeVec3.Cross(kLocalUserAcAttitudeVec3);
      //AAVVector3<FLOAT> kLocalAcRotationNormalizedVec3 = kLocalAcRotationVec3.Normalize();
      //                    m_kBodyAngularVelocity        = m_kLocalToBodyTransformMatrix*kLocalAcRotationNormalizedVec3;
      //FLOAT               fAngularVelocity3              = 125.1f*(1.0f - kLocalUserAcAttitudeVec3.Dot(kLocalAcAttitudeVec3));
      //                    m_kBodyAngularVelocity        = fAngularVelocity3*m_kBodyAngularVelocity;

      ////////////////////////////////////////////////////////////////////////////////
      // BODY ANGULAR VELOCITY (ROLL)
      ////////////////////////////////////////////////////////////////////////////////
      //TODO: Roll angle has to be adjusted according to flight attitude.
      m_kBodyAngularVelocity.z = 0; //0.0015f*m_fAirspeed; 

      ////////////////////////////////////////////////////////////////////////////////
      // BODY GRAVITY VECTOR
      ////////////////////////////////////////////////////////////////////////////////
      m_kBodyGravityVector = m_kLocalToBodyTransformMatrix*AAVVector3<FLOAT>(0, (m_fAirspeed < c_fMinAirspeed) ? (m_fAirspeed/c_fMinAirspeed - 1)*32.66f*c_fMass : 0, 0);

      ////////////////////////////////////////////////////////////////////////////////
      // BODY THRUST VECTOR
      ////////////////////////////////////////////////////////////////////////////////
      m_kBodyThrust = AAVVector3<FLOAT>(0, 0, (m_dTotalAirborneTime < c_fPropelantBurnoutTime) ? c_fMaxTrust : 0);

      ////////////////////////////////////////////////////////////////////////////////
      // AIR RESISTANCE VECTOR
      ////////////////////////////////////////////////////////////////////////////////
      m_kBodyAirResistance = AAVVector3<FLOAT>(0, 0, -c_kAirResistanceFactor.z*pow(m_fAirspeed, 2));

      ////////////////////////////////////////////////////////////////////////////////////
      // LINEAR ACCELERATION IN THE BODY REFERENCE SYSTEM
      ////////////////////////////////////////////////////////////////////////////////////
      m_kBodyAcceleration = (m_kBodyThrust + m_kBodyGravityVector + m_kBodyAirResistance)/c_fMass;
      //m_kBodyAcceleration = (m_kBodyGravityVector)/c_fMass;

      ////////////////////////////////////////////////////////////////////////////////////
      // BODY VELOCITY
      ////////////////////////////////////////////////////////////////////////////////////
      m_kBodyVelocity = m_kBodyVelocity +  m_kBodyAcceleration*m_dDeltaTime;
      m_fAirspeed     = m_kBodyVelocity.z;

      //m_fSpeedAspectRatio = 0.84f*(m_fAirspeed/c_fMaxAirspeed);
      //m_kVelocityVector = (m_kVelocityVector + (m_kGlobalTargetInterseptDirectionVector - m_kVelocityVector)*(m_fSpeedAspectRatio*m_fSpeedAspectRatio*m_fSpeedAspectRatio)).Normalize();

      //char acStr[64];
      //sprintf_s(acStr, sizeof(acStr), "DIST INTERCEPT %f", m_fDistanceToIntercept);
      //__SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);

      m_kSetObjectStateData.kBodyAngularVelocity = m_kBodyAngularVelocity;
      m_kSetObjectStateData.kBodyVelocity         = m_kBodyVelocity;
      SimConnect_SetDataOnSimObject(m_hSimConnect, MISSILE::DATA_ID_SET_OBJECT_STATE_DATA, m_dwObjectID, 0, 1, sizeof(MISSILE::SET_OBJECT_STATE_DATA), &m_kSetObjectStateData);

      //if(m_fCosInterceptAngle < 0.0f)
      //{
      //  m_dwTargetMissedTime = GetTickCount();
      //  SetState(WEAPON::WEAPON_STATE_MISSED);
      //  return;
      //}
      //else if(m_fDistanceToIntercept < c_fFuseDetonationDistance)
      //{
      //  SetState(WEAPON::WEAPON_STATE_DETONATE);
      //  return;
      //}
    }
    else
    {
      m_kTarget.dwObjectID = 0;
      SetState(WEAPON::WEAPON_STATE_MISSED);
    }
  }
  else
  {
    m_kTarget.dwObjectID = 0;
    SetState(WEAPON::WEAPON_STATE_NONE);
  }
}

/////////////////////////////////////////////////////////
// WEAPON_STATE_MISSED
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStateMissed()
{
  SetState(WEAPON::WEAPON_STATE_NONE);

}

/////////////////////////////////////////////////////////
// WEAPON_STATE_DETONATE
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStateDetonate()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT(pkAC  != NULL);

  if(m_kTarget.dwObjectID != 0)
  {
    //m_kTarget.IsValid() = false;
  }
  //PostThreadMessage(pkSharedPool->kFCS.dwThreadId, FCS_MESSAGE_SYSTEM_FAILURE,  0,                    0);
  //PostThreadMessage(pkSharedPool->kFCS.dwThreadId, FSCS_MESSAGE_SYSTEM_FAILURE, 0,                    0);
  SetState(WEAPON::WEAPON_STATE_NONE);
  //PostThreadMessage(__pkSMS->GetThreadId(), SMS::MESSAGE_DELETE_WEAPON, m_uiId, NULL);
}

/////////////////////////////////////////////////////////
// WEAPON_STATE_NONE
/////////////////////////////////////////////////////////
VOID MISSILE::ProcessStateNone()
{
  MSG   kMsg;

  if(m_bValid)
  {
    m_hResult = SimConnect_AIRemoveObject(m_hSimConnect, m_dwObjectID, WEAPON::REQUEST_ID_REMOVE_OBJECT);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_kTarget.dwObjectID  = 0xFFFFFFFF;
    m_bValid   = false;
  }

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case WEAPON::MESSAGE_REARM:
      SetState(WEAPON::WEAPON_STATE_IDLE); 
      break;
    }
  }
}

VOID MISSILE::Render(D3DXMATRIX &rkOPMatrix)
{
}

VOID MISSILE::RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  D3DXVECTOR3 kGlobVector;
  D3DXVECTOR3 kLocVector;
  D3DXMATRIXA16 mTrans;
  //FLOAT fVXZ, fVXYZ, fSinYaw, fCosYaw, fSinPitch, fCosPitch;

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  HUD *pkHUD = static_cast<HUD *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HUD));
  AAV_ASSERT_RETURN_VOID(pkHUD  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject    = NULL;

  //FLOAT fScaling;
  D3DXMATRIX mDistScalingMatrix;
  switch(m_eState)
  {
  case WEAPON::WEAPON_STATE_ARMED:
    break;

  case WEAPON::WEAPON_STATE_ACQUISITION:
    if(m_kTarget.dwObjectID != 0xFFFFFFFF)
    {
      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_SEEKER_DIRECTION_DESIGNATOR];
      if(pkGeomObject != NULL)
      {
        //HACK
        RotateSeekerTowardTargetDirection();

        pkGeomObject->SetTransform(pkDevice, &(pkAC->m_kBankRotationMatrix*rkOPMatrix*m_kLocalSeekerDirectionMatrix));
        //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*m_kLocalSeekerDirectionMatrix));
        pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_SEEKER_DIRECTION_DESIGNATOR]);
        pkGeomObject->Render(pkDevice);
        pkGeomObject->SetTexture(pkDevice, NULL);
      }
    }
    //  if(m_pkSeekerMatrix == &(pkAC->m_kBodyToLocalTransformMatrix))
    //  {
    //    pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_BORESITE_VIEW_DESIGNATOR];
    //    if(pkGeomObject != NULL)
    //    {
    //      //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*(*m_pkSeekerMatrix)));
    //      pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*pkAC->m_kBodyToLocalTransformMatrix));
    //      //pkGeomObject->RenderPrimitives(pkDevice, 0, NUM_BORESITE_VIEW_DESIGNATOR_VERTICES - 2, true);
    //      pkGeomObject->Render(pkDevice);
    //    }
    //  }
    //  else
    //  {
    //    pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_TIRIS_DESIGNATOR];
    //    if(pkGeomObject != NULL)
    //    {
    //      //Get current view matrix from D3DX
    //      D3DXMATRIXA16 kViewMatrix;
    //      pkGraphics->GetViewMatrix(&kViewMatrix);
    //      //pD3DDevice->GetTransform(D3DTS_VIEW, &kViewMatrix);
    //      D3DXMatrixTranspose(&pkAC->m_kViewMatrix, &kViewMatrix);
    //      //pD3DDevice->SetTransform(pkDevice, D3DTS_VIEW, &pkAC->m_kViewMatrix);
    //      //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*(*pkAC->m_kViewMatrix)));
    //      pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*pkAC->m_kViewMatrix));
    //      //pkGeomObject->SetTransform(pkDevice, &pkAC->m_kViewMatrix);
    //      pkGeomObject->RenderPrimitives(pkDevice, 0, NUM_TIRIS_DESIGNATOR_VERTICES - 2, true);
    //    }
    //  }
    break;


  case WEAPON::WEAPON_STATE_LOCKED:
    if(m_kTarget.dwObjectID != 0xFFFFFFFF)
    {
      D3DXMATRIXA16 kSeekerTransformMatrix = pkAC->m_kBankRotationMatrix*rkOPMatrix*m_kLocalSeekerDirectionMatrix;

      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_SEEKER_DIRECTION_DESIGNATOR];
      AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);

      //TODO::Must not be in the rendering function.
      RotateSeekerTowardTargetDirection();

      pkGeomObject->SetTransform(pkDevice, &kSeekerTransformMatrix);
      //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*m_kLocalSeekerDirectionMatrix));
      //pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_LOCKED_TARGET_DESIGNATOR]);
      pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_SEEKER_DIRECTION_DESIGNATOR]);
      pkGeomObject->Render(pkDevice);
      pkGeomObject->SetTexture(pkDevice, NULL);

      ///////////////////////////////////////////////////////////////////
      // RENDER DLZ DESIGNATORS
      ///////////////////////////////////////////////////////////////////
      AAV_ASSERT_RETURN_VOID(m_fLunchEfficiencyFactor >= 0);
      AAV_ASSERT_RETURN_VOID(m_fLunchEfficiencyFactor <= 1);
      if((m_fLunchEfficiencyFactor < 0.7f) || (m_pkAvionics->g_bWarnClockEdgeUP))
      {
        pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_DLZ_DESIGNATOR_R1];
        AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
        pkGeomObject->Render(pkDevice);

        pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_DLZ_DESIGNATOR_R2];
        AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
        UINT uiNumVertsToRender = 2*static_cast<UINT>((NUM_DLZ_DESIGNATOR_R2_VERTICES/2)*m_fLunchEfficiencyFactor);
        pkGeomObject->Render(pkDevice, uiNumVertsToRender, 0);
      }
      ///////////////////////////////////////////////////////////////////
      // RENDER DLZ BARS
      ///////////////////////////////////////////////////////////////////
      //D3DXMATRIXA16 kHUDTransformMatrix = pkHUD->GetTransformMatrix();

      //  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_DLZ_BAR_R1];
      //  if(pkGeomObject != NULL)
      //  {
      //    pkGeomObject->SetTransform(pkDevice, &(m_kDLZ_R1Matrix*kSeekerTransformMatrix));
      //    pkGeomObject->SetTexture(pkDevice, NULL);
      //    pkGeomObject->Render(pkDevice);
      //  }

      //  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_DLZ_BAR_R2];
      //  if(pkGeomObject != NULL)
      //  {
      //    D3DXMATRIXA16 kDLZ_R2ScalingMatrix = *D3DXMatrixScaling(&kDLZ_R2ScalingMatrix, 1, 0.6f, 1);
      //    pkGeomObject->SetTransform(pkDevice, &(kDLZ_R2ScalingMatrix*m_kDLZ_R2Matrix*kSeekerTransformMatrix));
      //    pkGeomObject->SetTexture(pkDevice, NULL);
      //    pkGeomObject->Render(pkDevice);
      //  }
      //



      //if(__SVSPAvionics::g_bCautionClockEdgeUP)
      //{
      //  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_TIRIS_DESIGNATOR];
      //  if(pkGeomObject != NULL)
      //  {
      //    pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*mTrans));
      //    pkGeomObject->RenderPrimitives(pkDevice, 0, NUM_TIRIS_DESIGNATOR_VERTICES - 2, true);
      //  }
      //}

      //if(m_bUpdateLockDirection)
      //{
      //  //D3DXVECTOR3 kNormalizedLocVector;
      //  //D3DXVec3Normalize(&kNormalizedLocVector, &kLocVector);
      //  //D3DXVec3TransformCoord(&m_kLockDirectionVector, &kLocVector, m_pkACMatrix);
      //  //m_kLockDirectionVector.y = 0;
      //  //m_kLockedDirectionMatrix = mTrans;
      //  m_bUpdateLockDirection = false;

      //  //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*m_kLockedDirectionMatrix));
      //  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_LOCKED_TARGET_DESIGNATOR];
      //  if(pkGeomObject != NULL)
      //  {
      //    pkGeomObject->RenderPrimitives(pkDevice, 0, NUM_LOCKED_TARGET_DESIGNATOR_VERTICES - 2, true);
      //  }
      //}
      //else
      //{
      //  //fVXZ      =  sqrt(m_kLockDirectionVector.x*m_kLockDirectionVector.x + m_kLockDirectionVector.z*m_kLockDirectionVector.z);
      //  //fVXYZ     =  sqrt(m_kLockDirectionVector.x*m_kLockDirectionVector.x + m_kLockDirectionVector.y*m_kLockDirectionVector.y + m_kLockDirectionVector.z*m_kLockDirectionVector.z);
      //  //fSinYaw   =  m_kLockDirectionVector.x/fVXZ;
      //  //fCosYaw   =  m_kLockDirectionVector.z/fVXZ;
      //  //fSinPitch =  m_kLockDirectionVector.y/fVXYZ;
      //  //fCosPitch =  fVXZ/fVXYZ;

      //  //mTrans._11 =  fCosYaw;           mTrans._12 =  0;         mTrans._13 =  fSinYaw;           mTrans._14 =  0;
      //  //mTrans._21 =  fSinPitch*fSinYaw; mTrans._22 =  fCosPitch; mTrans._23 = -fSinPitch*fCosYaw; mTrans._24 =  0;
      //  //mTrans._31 = -fCosPitch*fSinYaw; mTrans._32 =  fSinPitch; mTrans._33 =  fCosPitch*fCosYaw; mTrans._34 =  0;
      //  //mTrans._41 =  0;                 mTrans._42 =  0;         mTrans._43 =  0;                 mTrans._44 =  1;

      //  //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*m_kLocDirectionMatrix*(*m_pkACMatrix)));
      //  //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*m_kLockedDirectionMatrix));
      //  //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*(*m_pkSeekerMatrix)));
      //  pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_LOCKED_TARGET_DESIGNATOR];
      //  if(pkGeomObject != NULL)
      //  {
      //    pkGeomObject->RenderPrimitives(pkDevice, 0, NUM_LOCKED_TARGET_DESIGNATOR_VERTICES - 2, true);
      //  }
      //}
    }
    break;

  default:
    break;
  }
}
