#include "AC.h"
#include "CARRIER.h"
#include "__SVSPAvionics.h"
#include "ECS.h"
#include "SS.h"
#include "__SVSPGeoCoordinates3.h"

//const USHORT CONST_CAT_LENGTH                    = 300;
//const USHORT CONST_TAKEOFF_SPEED                 = 175;
const FLOAT               CARRIER::AIRCRAFT_EVALUATION_RANGE           = 402336.0f; //25 Miles
//const FLOAT               CARRIER::AIRCRAFT_APPROACH_COSINUS             = 0.985f;    //10 Degrees
//const FLOAT               CARRIER::AIRCRAFT_APPROACH_COSINUS             = 0.940f;    //20 Degrees
const FLOAT               CARRIER::AIRCRAFT_APPROACH_COSINUS             = 0.866f;    //30 Degrees
const FLOAT              CARRIER::AIRCRAFT_EVALUATION_TIME            = 5.0;
const FLOAT              CARRIER::TOUCH_AND_GO_EVALUATION_TIME        = 2.0;
const AAVVector3<FLOAT> CARRIER::kBodyOptimalApproachVector          = AAVVector3<FLOAT>();

CARRIER::CARRIER(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, CHAR *pcName, __SVSPGeoCoordinates3<FLOAT> *pkLocation, AAVVector3<FLOAT> *pkAttitude) : 
__SVSPModule(pcName, pkAvionics, dwThreadSleepTime, true, CARRIER__DispatchProcess, NULL, GEOM_OBJECT_NUM_OBJECTS)
{

  m_kLocation = *pkLocation;
  m_kAttitude = *pkAttitude;
  m_kVector = m_kLocation.GetVector3<FLOAT>();

  // Subscribe to system events
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, CARRIER::EVENT_ID_OBJECT_ADDED, "ObjectAdded");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, CARRIER::EVENT_ID_OBJECT_REMOVED, "ObjectRemoved");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  ///////////////////////////////////////////////////////////////////
  // CARRIER::DATA_ID_READ_OBJECT_STATE_DATA
  ///////////////////////////////////////////////////////////////////
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_READ_OBJECT_STATE_DATA, "STRUCT LATLONALT",              "radians",        SIMCONNECT_DATATYPE_LATLONALT);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE LATITUDE",                "degrees",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE LONGITUDE",               "degrees",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE ALTITUDE",                "feet",               SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE PITCH DEGREES",           "radians",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE HEADING DEGREES TRUE",    "radians",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE BANK DEGREES",            "radians",             SIMCONNECT_DATATYPE_FLOAT32);

  ///////////////////////////////////////////////////////////////////
  // CARRIER::DATA_ID_SET_OBJECT_STATE_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY X",        "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Y",        "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Z",        "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY X",                 "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY Y",                 "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY Z",                 "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);

  ///////////////////////////////////////////////////////////////////
  // CARRIER::DATA_ID_SET_USER_AC_SIM_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "ACCELERATION BODY X",               "Feet per second",   SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "ACCELERATION BODY Y",               "Feet per second",   SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "ACCELERATION BODY Z",               "Feet per second",   SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "VELOCITY BODY X",                   "Feet per second",   SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "VELOCITY BODY Y",                   "Feet per second",   SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "VELOCITY BODY Z",                   "Feet per second",   SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "ROTATION VELOCITY BODY X",          "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "ROTATION VELOCITY BODY Y",          "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "ROTATION VELOCITY BODY Z",          "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "PLANE PITCH DEGREES",               "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "PLANE BANK DEGREES",                "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, "PLANE HEADING DEGREES TRUE",        "radians",            SIMCONNECT_DATATYPE_FLOAT32);



  ///////////////////////////////////////////////////////////////////
  // APPROACH_MAP
  ///////////////////////////////////////////////////////////////////
  //0.016667 Degrees - 1 nm.
  //m_akApproachWaypoint[0] = __SVSPModule::AAVWAYPOINT<FLOAT>("CA0", __SVSPGeoCoordinates3<DOUBLE>(/*LAT*/0.0f,         /*LON*/0.0f, /*ALT*/24.3f), 0.0f);
  //m_akApproachWaypoint[1] = __SVSPModule::AAVWAYPOINT<FLOAT>("CA1", __SVSPGeoCoordinates3<DOUBLE>(/*LAT*/25*0.016667f, /*LON*/0.0f, /*ALT*/24.3f), 0.0f);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;
  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_APPROACH_MAP
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINESTRIP, CARRIER::NUM_APPROACH_MAP_VERTICES);
  pkGeomObject->SetMaterial(1.0f, 0.3f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(0, 0, 0, 0, 0, 0xFFFFFFFF);
  pVerts[1] = __SVSPVertex(1, 0, 0, 0, 0, 0xFFFFFFFF);
  m_ppkGeomObjects[CARRIER::GEOM_OBJECT_APPROACH_MAP] = pkGeomObject;

  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_NONE]                    = &CARRIER::ProcessStateNone;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_IDLE]                    = &CARRIER::ProcessStateIdle;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_ENROUTE]                 = &CARRIER::ProcessStateEnroute;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_APPROACH_EVALUATION]     = &CARRIER::ProcessStateApproachEvaluation;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_FLYBY_EVALUATION]        = &CARRIER::ProcessStateFlybyEvaluation;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_USER_AC_ON_APPROACH]     = &CARRIER::ProcessStateUserAcOnApproach;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_ARRESTING]               = &CARRIER::ProcessStateArresting;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_TOUCH_AND_GO_EVALUATION] = &CARRIER::ProcessStateTouchAndGoEvaluation;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_TOUCH_DOWN_EVALUATION]   = &CARRIER::ProcessStateTouchDownEvaluation;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_CAT_ARMED]               = &CARRIER::ProcessStateCatArmed;
  m_apkProcessStateCallback[CARRIER::CARRIER_STATE_CAT_LAUNCHED]            = &CARRIER::ProcessStateCatLaunched;

  m_uiThreadSleepPeriod = 35;
  m_dwObjectID          = 0;
  m_eState              = CARRIER::CARRIER_STATE_NONE;
}

CARRIER::~CARRIER()
{
  __SVSPModule::Stop();

  // Unsubscribe from system events
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, CARRIER::EVENT_ID_OBJECT_ADDED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, CARRIER::EVENT_ID_OBJECT_REMOVED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

VOID CALLBACK  CARRIER__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  CARRIER *__THIS = static_cast<CARRIER *>(pContext);
  if(!__THIS->m_bAbort)
  {
    __THIS->DispatchProcess(pData, cbData);
  }
}

VOID CARRIER::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
{
  CHAR acStr[128];
  HRESULT m_hResult;

  switch(pData->dwID)
  {
  case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
    break;

  case SIMCONNECT_RECV_ID_ASSIGNED_OBJECT_ID:
    {
      SIMCONNECT_RECV_ASSIGNED_OBJECT_ID *pObjData = (SIMCONNECT_RECV_ASSIGNED_OBJECT_ID *)pData;
      if(pObjData->dwRequestID == CARRIER::REQUEST_ID_CREATE_OBJECT)
      {
        m_dwObjectID = pObjData->dwObjectID;
        sprintf_s(&acStr[0], sizeof(acStr), "CREATED OBJECT_ID = %d", m_dwObjectID);
        __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);

        m_hResult = SimConnect_AIReleaseControl(m_hSimConnect, m_dwObjectID, CARRIER::REQUEST_ID_RELEASE_CONTROL);
        AAV_ASSERT(SUCCEEDED(m_hResult));

        memset(&m_kReadObjectStateData, 0, sizeof(CARRIER::READ_OBJECT_STATE_DATA));
        m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, CARRIER::REQUEST_ID_READ_OBJECT_STATE_DATA, CARRIER::DATA_ID_READ_OBJECT_STATE_DATA, m_dwObjectID, SIMCONNECT_PERIOD_SIM_FRAME);
        AAV_ASSERT(SUCCEEDED(m_hResult));

        m_eState = CARRIER::CARRIER_STATE_ENROUTE;
      }
    }
    break;

  case SIMCONNECT_RECV_ID_EVENT:
    break;

  case SIMCONNECT_RECV_ID_EVENT_OBJECT_ADDREMOVE:
    {
      SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE *pEvt = (SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE*)pData;
      if((pEvt->uEventID == CARRIER::EVENT_ID_OBJECT_REMOVED) && (pEvt->dwData == m_dwObjectID))
      {
        sprintf_s(&acStr[0], sizeof(acStr), "REMOVED OBJECT_ID = %d", m_dwObjectID);
        __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
        m_dwObjectID = 0;

        m_eState = CARRIER::CARRIER_STATE_NONE;
      }
    }
    break;

  case SIMCONNECT_RECV_ID_EXCEPTION:
    {
      SIMCONNECT_RECV_EXCEPTION *pObjData = (SIMCONNECT_RECV_EXCEPTION *)pData;
      sprintf_s(&acStr[0], sizeof(acStr), "EXCEPTION_ID = %d", pObjData->dwException);
      __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
    }
    break;

  case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
    {
      SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
      if(pObjData->dwRequestID == CARRIER::REQUEST_ID_READ_OBJECT_STATE_DATA)
      {
        CARRIER::READ_OBJECT_STATE_DATA *pkData                = (CARRIER::READ_OBJECT_STATE_DATA *)&pObjData->dwData;
        CARRIER::READ_OBJECT_STATE_DATA *pkReadObjectStateData = &m_kReadObjectStateData;
        memcpy_s(pkReadObjectStateData, sizeof(CARRIER::READ_OBJECT_STATE_DATA), pkData, sizeof(CARRIER::READ_OBJECT_STATE_DATA));

      }
    }
    break;

  default:
    break;
  }
}

D3DXVECTOR2 CARRIER::GetACLSErrorVector()
{
  //m_kGlobalToLocalTransformMatrix
  FLOAT fACLat      = (FLOAT)(m_kReadObjectStateData.kLocation.GetLatitudeAngle());
  FLOAT fACLon      = (FLOAT)(m_kReadObjectStateData.kLocation.GetLongitudeAngle() - D3DX_PI/2.0);
  FLOAT fCos_fACLat = cos(fACLat);
  FLOAT fSin_fACLat = sin(fACLat);
  FLOAT fCos_fACLon = cos(fACLon);
  FLOAT fSin_fACLon = sin(fACLon);
  m_kGlobalToLocalTransformMatrix._11 =  fCos_fACLon;             m_kGlobalToLocalTransformMatrix._21 =  fSin_fACLon;             m_kGlobalToLocalTransformMatrix._31 =  0;           m_kGlobalToLocalTransformMatrix._41 =  0;
  m_kGlobalToLocalTransformMatrix._12 = -fCos_fACLat*fSin_fACLon; m_kGlobalToLocalTransformMatrix._22 =  fCos_fACLat*fCos_fACLon; m_kGlobalToLocalTransformMatrix._32 =  fSin_fACLat; m_kGlobalToLocalTransformMatrix._42 =  0;
  m_kGlobalToLocalTransformMatrix._13 =  fSin_fACLat*fSin_fACLon; m_kGlobalToLocalTransformMatrix._23 = -fSin_fACLat*fCos_fACLon; m_kGlobalToLocalTransformMatrix._33 =  fCos_fACLat; m_kGlobalToLocalTransformMatrix._43 =  0;
  m_kGlobalToLocalTransformMatrix._14 =  0;                       m_kGlobalToLocalTransformMatrix._24 =  0;                       m_kGlobalToLocalTransformMatrix._34 =  0;           m_kGlobalToLocalTransformMatrix._44 =  1;

  //m_kLocalToOptimalApproachTransformMatrix
  //D3DXMatrixRotationX(&m_kLocalToBodyXMatrix, -(FLOAT)m_kReadObjectStateData.kBodyAttitude.x);
  //D3DXMatrixRotationY(&m_kLocalToBodyYMatrix, -(FLOAT)m_kReadObjectStateData.kBodyAttitude.y);
  D3DXMatrixRotationX(&m_kLocalToBodyXMatrix, -(FLOAT)m_kReadObjectStateData.kBodyAttitude.x + 0.07f); //4.0f*D3DX_PI/180.0f);
  D3DXMatrixRotationY(&m_kLocalToBodyYMatrix, -(FLOAT)m_kReadObjectStateData.kBodyAttitude.y - 0.16f); //9.5f*D3DX_PI/180.0f);
  D3DXMatrixRotationZ(&m_kLocalToBodyZMatrix, -(FLOAT)m_kReadObjectStateData.kBodyAttitude.z);
  m_kLocalToOptimalApproachTransformMatrix = m_kLocalToBodyYMatrix*m_kLocalToBodyXMatrix*m_kLocalToBodyZMatrix;

  //m_kACLSVector
  m_kLocalUserAcOffsetVectorNormalized = m_kGlobalToLocalTransformMatrix*m_kGlobalOffsetVectorNormalized;
  m_kACLSVector = m_kLocalToOptimalApproachTransformMatrix*m_kLocalUserAcOffsetVectorNormalized;
  FLOAT X = 2.5f*m_kACLSVector.x;
  FLOAT Y = 2.5f*m_kACLSVector.y;

  return D3DXVECTOR2( ((abs(X) > 2.5f) ? 127.0f*sign(X) : 127.0f*X), (-((abs(Y) > 2.5f) ? 119.0f*sign(Y) : 119.0f*Y)) );
}

VOID CARRIER::ProcessStateNone()
{

}

VOID CARRIER::ProcessStateIdle()
{

}

VOID CARRIER::ProcessStateEnroute()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  m_uiThreadSleepPeriod = 35;
  pkAC->m_bACLSOn = false;

  //Evaluate User Aircraft behavior within inbound zone.
  if((m_fDistanceToUserAircraft < CARRIER::AIRCRAFT_EVALUATION_RANGE) && !pkAC->m_kData.m_bOnGround)
  {
    if(m_dApproachCosinus > CARRIER::AIRCRAFT_APPROACH_COSINUS)
    {
      m_dStartEvaluationTime = GetSystemTime();
      m_eState = CARRIER::CARRIER_STATE_APPROACH_EVALUATION;
    }
  }
}

VOID CARRIER::ProcessStateApproachEvaluation()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  m_uiThreadSleepPeriod = 35;

  if(m_fDistanceToUserAircraft < CARRIER::AIRCRAFT_EVALUATION_RANGE)
  {
    if(pkAC->m_kData.m_bOnGround)
    {
      m_eState = CARRIER::CARRIER_STATE_TOUCH_DOWN_EVALUATION;
    }
    else
    if(m_dApproachCosinus > CARRIER::AIRCRAFT_APPROACH_COSINUS)
    {
      m_dEvaluationTime = GetSystemTime() - m_dStartEvaluationTime;
      if(m_dEvaluationTime > CARRIER::AIRCRAFT_EVALUATION_TIME)
      {
        m_eState = CARRIER::CARRIER_STATE_USER_AC_ON_APPROACH;
      }
    }
    else
    {
      m_eState = CARRIER::CARRIER_STATE_ENROUTE;
    }
  }
  else
  {
    m_eState = CARRIER::CARRIER_STATE_ENROUTE;
  }
}

VOID CARRIER::ProcessStateFlybyEvaluation()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  m_uiThreadSleepPeriod = 35;

  if(m_fDistanceToUserAircraft < CARRIER::AIRCRAFT_EVALUATION_RANGE)
  {
    if(pkAC->m_kData.m_bOnGround)
    {
      m_eState = CARRIER::CARRIER_STATE_TOUCH_DOWN_EVALUATION;
    }
    else
    {
      if(m_dApproachCosinus < CARRIER::AIRCRAFT_APPROACH_COSINUS)
      {
        m_dEvaluationTime = GetSystemTime() - m_dStartEvaluationTime;
        if(m_dEvaluationTime > CARRIER::AIRCRAFT_EVALUATION_TIME)
        {
          m_eState = CARRIER::CARRIER_STATE_ENROUTE;
        }
        else
        {
          pkAC->m_bACLSOn           = true;
          pkAC->m_bNavHasLocalizer  = true;
          pkAC->m_bNavHasGlideslope = true;
          pkAC->m_kILSErrorVector = GetACLSErrorVector();
        }
      }
      else
      {
        m_eState = CARRIER::CARRIER_STATE_USER_AC_ON_APPROACH;
      }
    }
  }
  else
  {
    m_eState = CARRIER::CARRIER_STATE_ENROUTE;
  }
}

VOID CARRIER::ProcessStateUserAcOnApproach()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  m_uiThreadSleepPeriod = 35;

  if(m_fDistanceToUserAircraft < CARRIER::AIRCRAFT_EVALUATION_RANGE)
  {
    if(pkAC->m_kData.m_bOnGround)
    {
      m_eState = CARRIER::CARRIER_STATE_TOUCH_DOWN_EVALUATION;
    }
    else
    {
      if(m_dApproachCosinus > CARRIER::AIRCRAFT_APPROACH_COSINUS)
      {
        pkAC->m_bACLSOn           = true;
        pkAC->m_bNavHasLocalizer  = true;
        pkAC->m_bNavHasGlideslope = true;
        pkAC->m_kILSErrorVector = GetACLSErrorVector();
      }
      else
      {
        m_dStartEvaluationTime = GetSystemTime();
        m_eState = CARRIER::CARRIER_STATE_FLYBY_EVALUATION;
      }
    }
  }
  else
  {
    m_eState = CARRIER::CARRIER_STATE_ENROUTE;
  }
}

VOID CARRIER::ProcessStateArresting()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;

  m_uiThreadSleepPeriod = 35;

  if(!pkAC->m_kData.m_bOnGround)
  {
    m_dStartEvaluationTime = GetSystemTime();
    m_eState = CARRIER::CARRIER_STATE_TOUCH_AND_GO_EVALUATION;
  }
  else if(abs(pkAC->m_kBodyVelocity.Length()) < (m_kBodyVelocity.z - 5.0))
  {
    m_eState = CARRIER::CARRIER_STATE_ENROUTE;
  }
  else
  {
    m_kBodyArrestingVectorNormalized = pkAC->m_kLocalToBodyTransformMatrix*m_kLocalArrestingVectorNormalized;

    m_kSetUserAcSimData.kBodyAcceleration     = m_kBodyArrestingVectorNormalized*50.0;
    m_kSetUserAcSimData.kBodyVelocity         = pkAC->m_kBodyVelocity + m_kSetUserAcSimData.kBodyAcceleration*m_dDeltaTime;
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, CARRIER::DATA_ID_SET_USER_AC_SIM_DATA, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(CARRIER::SET_USER_AC_SIM_DATA), &m_kSetUserAcSimData);
  }
}

VOID CARRIER::ProcessStateTouchAndGoEvaluation()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;

  m_uiThreadSleepPeriod = 35;

  m_dEvaluationTime = GetSystemTime() - m_dStartEvaluationTime;

  if(pkAC->m_kData.m_bOnGround)
  {
    m_eState = CARRIER::CARRIER_STATE_ARRESTING;
  }
  else if(m_dEvaluationTime > CARRIER::TOUCH_AND_GO_EVALUATION_TIME)
  {
    m_eState = CARRIER::CARRIER_STATE_ENROUTE;
  }
  else
  {
    //TODO:
  }
}

VOID CARRIER::ProcessStateTouchDownEvaluation()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;

  m_uiThreadSleepPeriod = 35;

  if((pkAC->m_kData.m_bOnGround) && (m_fDistanceToUserAircraft < 100.0f) && (pkAC->m_kData.m_dIndicatedAirspeed > 110.0) && (m_dApproachCosinus > 0))
  {
    //Store initial arresting vector.
    m_kLocalArrestingVector           = pkAC->m_kBodyToLocalTransformMatrix*pkAC->m_kBodyVelocity;
    m_kLocalArrestingVector           = AAVVector3<FLOAT>(-m_kLocalArrestingVector.x, 0, -m_kLocalArrestingVector.z);
    m_kLocalArrestingVectorNormalized = m_kLocalArrestingVector.Normalize();

    m_eState = CARRIER::CARRIER_STATE_ARRESTING;
  }
  else
  {
    m_eState = CARRIER::CARRIER_STATE_ENROUTE;
  }
}

VOID CARRIER::ProcessStateCatArmed()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  //SS *pkSS = static_cast<SS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ISS));
  //AAV_ASSERT_RETURN_VOID(pkSS != NULL);

  ECS *pkECS = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  AAV_ASSERT_RETURN_VOID(pkECS != NULL);
}

VOID CARRIER::ProcessStateCatLaunched()
{
}

VOID CARRIER::Start()
{
  if(m_dwObjectID == 0)
  {
    AAVVector3<FLOAT> kBodyVelocityNormalized;

    // INIT OBJECT POSITION AND VELOCITIES 
    m_kBodyAngularVelocity = AAVVector3<FLOAT>(0, 0, 0);
    m_kBodyVelocity         = AAVVector3<FLOAT>(0, 0, 50.0f);

    SIMCONNECT_DATA_INITPOSITION          kInitPosition;
    kInitPosition.Latitude              = m_kLocation.Latitude;                                                   ///Degrees
    kInitPosition.Longitude             = m_kLocation.Longitude;                                                  ///Degrees
    kInitPosition.Altitude              = m_kLocation.Altitude * 3.28083989501;                                   ///Feet
    kInitPosition.Pitch                 = m_kAttitude.x;                                                          ///Degrees
    kInitPosition.Heading               = m_kAttitude.y;                                                          ///Degrees
    kInitPosition.Bank                  = m_kAttitude.z;                                                          ///Degrees
    kInitPosition.OnGround              = 1;
    kInitPosition.Airspeed              = 0;

    // CREATE FSX MODEL
    m_hResult = SimConnect_AICreateSimulatedObject (m_hSimConnect, m_acName.getCStr(), kInitPosition, CARRIER::REQUEST_ID_CREATE_OBJECT);
    AAV_ASSERT(SUCCEEDED(m_hResult));
  }

  __SVSPModule::Start();
}

VOID CARRIER::Process()
{

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  if(m_eState >= CARRIER::CARRIER_STATE_ENROUTE)
  {
    ////////////////////////////////////////////////////////////////////////////
    // SET OBJECT DATA
    ////////////////////////////////////////////////////////////////////////////
    m_kSetObjectStateData.kBodyAngularVelocity = m_kBodyAngularVelocity;
    m_kSetObjectStateData.kBodyVelocity         = m_kBodyVelocity;
    m_hResult = SimConnect_AIReleaseControl(m_hSimConnect, m_dwObjectID, CARRIER::REQUEST_ID_RELEASE_CONTROL);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, CARRIER::DATA_ID_SET_OBJECT_STATE_DATA, m_dwObjectID, 0, 1, sizeof(CARRIER::SET_OBJECT_STATE_DATA), &m_kSetObjectStateData);
    AAV_ASSERT(SUCCEEDED(m_hResult));


    //Global Object Vector
    m_kReadObjectStateData.kLocation.Altitude += 24.3f; //Deck elevation
    m_kVector = m_kReadObjectStateData.kLocation.GetVector3<FLOAT>();

    m_kGlobalOffsetVector = m_kVector - pkAC->m_kVector;
    m_fDistanceToUserAircraft = m_kGlobalOffsetVector.Length();
    D3DXVec3TransformCoord(&m_kLocalOffsetVector, (D3DXVECTOR3 *)&m_kGlobalOffsetVector, &pkAC->m_kGlobalToLocalTransformMatrix);
    m_kLocalOffsetVector.x  = -m_kLocalOffsetVector.x;

    m_kGlobalOffsetVectorNormalized = m_kGlobalOffsetVector/m_fDistanceToUserAircraft;
    m_dApproachCosinus = m_kGlobalOffsetVectorNormalized.Dot(pkAC->m_kGlobalVelocityVectorNormalized);
  }

  AAV_ASSERT(m_eState < CARRIER::CARRIER_STATE_MAX);
  VOID (__thiscall CARRIER::*__ProcessStateCallback)(VOID) = m_apkProcessStateCallback[m_eState];
  AAV_ASSERT(__ProcessStateCallback != NULL);
  (this->*__ProcessStateCallback)();
}

VOID CARRIER::RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
}


VOID CARRIER::RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix, FLOAT fCurrentNumPixelsInNauticalMile)
{
  //LPD3DXGEOMETRYOBJECT pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_APPROACH_MAP];

  //  D3DXVec3TransformCoord(&kBodyOffsetVector, &pkObject->kLocalOffsetVector, &pkAC->m_kYawRotationMatrix);
  //D3DXVECTOR3 kBodyOffsetVectorNauticalMiles = kBodyOffsetVector/1852;
  //D3DXVECTOR3 kBodyOffsetVectorPixels = m_fCurrentNumPixelsInNauticalMile*kBodyOffsetVectorNauticalMiles;
  //D3DXMatrixTranslation(&kObjectOffsetMatrix, -kBodyOffsetVectorPixels.x, kBodyOffsetVectorPixels.z, 0);

  //pkGeomObject = (pkObject == m_pkTarget) ? pkGeomObjectSelectedObjectDesignator : pkGeomObjectRadarObjectDesignator;
  //pkGeomObject->SetTransform(pkDevice, &(pkObject->kHeadingMatrix*kObjectOffsetMatrix*kCenterPointMatrix));
  //pkGeomObject->Render(pkDevice);

}