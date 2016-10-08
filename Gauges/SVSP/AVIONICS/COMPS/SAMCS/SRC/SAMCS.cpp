#include "AC.h"
#include "SAMCS.h"
#include "__SVSPAvionics.h"

SAMCS::SAMCS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, CHAR *pcName, __SVSPGeoCoordinates3<FLOAT> *pkLocation, AAVVector3<FLOAT> *pkAttitude) :
__SVSPModule(pcName, pkAvionics,dwThreadSleepTime, true, NULL, NULL, 0)
{
  m_kLocation = *pkLocation;
  m_kAttitude = *pkAttitude;
  m_kVector = m_kLocation.GetVector3<FLOAT>();

  // Subscribe to system events
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, SAMCS::EVENT_ID_OBJECT_ADDED, "ObjectAdded");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, SAMCS::EVENT_ID_OBJECT_REMOVED, "ObjectRemoved");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  ///////////////////////////////////////////////////////////////////
  // SAMCS::DATA_ID_READ_OBJECT_STATE_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE PITCH DEGREES",                "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE HEADING DEGREES TRUE",         "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE BANK DEGREES",                 "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY X",           "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Y",           "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Z",           "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "VELOCITY BODY X",                    "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "VELOCITY BODY Y",                    "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "VELOCITY BODY Z",                    "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "STRUCT LATLONALT",                   "radians",            SIMCONNECT_DATATYPE_LATLONALT);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE LATITUDE",                     "degrees",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE LONGITUDE",                    "degrees",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE ALTITUDE",                     "feet",               SIMCONNECT_DATATYPE_FLOAT32);


  ///////////////////////////////////////////////////////////////////
  // SAMCS::DATA_ID_SET_OBJECT_STATE_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY X",            "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Y",            "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Z",            "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY X",                     "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY Y",                     "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SAMCS::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY Z",                     "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);


  ///////////////////////////////////////////////////////////////////
  // STATE_CALLBACK_TABLE
  ///////////////////////////////////////////////////////////////////
  m_apkProcessStateCallback[SAMCS::STATE_ID_NONE]     = &SAMCS::ProcessStateNone;
  m_apkProcessStateCallback[SAMCS::STATE_ID_IDLE]     = &SAMCS::ProcessStateIdle;
  m_apkProcessStateCallback[SAMCS::STATE_ID_ARMED]    = &SAMCS::ProcessStateArmed;
  m_apkProcessStateCallback[SAMCS::STATE_ID_LOCKED]   = &SAMCS::ProcessStateLocked;

  m_eState = SAMCS::STATE_ID_NONE;
}


SAMCS::~SAMCS()
{
  __SVSPModule::Stop();

  // Unsubscribe from system events
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, SAMCS::EVENT_ID_OBJECT_ADDED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, SAMCS::EVENT_ID_OBJECT_REMOVED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

VOID SAMCS::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
{
  CHAR acStr[128];
  HRESULT hResult;

  switch(pData->dwID)
  {
  case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
    break;

  case SIMCONNECT_RECV_ID_ASSIGNED_OBJECT_ID:
    {
      SIMCONNECT_RECV_ASSIGNED_OBJECT_ID *pObjData = (SIMCONNECT_RECV_ASSIGNED_OBJECT_ID *)pData;
      if(pObjData->dwRequestID == SAMCS::REQUEST_ID_CREATE_OBJECT)
      {
        m_dwObjectID = pObjData->dwObjectID;
        sprintf_s(&acStr[0], sizeof(acStr), "CREATED OBJECT_ID = %d", m_dwObjectID);
        __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);

        hResult = SimConnect_AIReleaseControl(m_hSimConnect, m_dwObjectID, SAMCS::REQUEST_ID_RELEASE_CONTROL);
        AAV_ASSERT(SUCCEEDED(hResult));

        memset(&m_kReadObjectStateData, 0, sizeof(SAMCS::READ_OBJECT_STATE_DATA));
        hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, SAMCS::REQUEST_ID_READ_OBJECT_STATE_DATA, SAMCS::DATA_ID_READ_OBJECT_STATE_DATA, m_dwObjectID, SIMCONNECT_PERIOD_SIM_FRAME);
        AAV_ASSERT(SUCCEEDED(hResult));

        m_eState = SAMCS::STATE_ID_IDLE;
      }
    }
    break;

  case SIMCONNECT_RECV_ID_EVENT:
    break;

  case SIMCONNECT_RECV_ID_EVENT_OBJECT_ADDREMOVE:
    {
      SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE *pEvt = (SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE*)pData;
      if((pEvt->uEventID == SAMCS::EVENT_ID_OBJECT_REMOVED) && (pEvt->dwData == m_dwObjectID))
      {
        sprintf_s(&acStr[0], sizeof(acStr), "REMOVED OBJECT_ID = %d", m_dwObjectID);
        __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);

        m_eState = SAMCS::STATE_ID_NONE;
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
      if(pObjData->dwRequestID == SAMCS::REQUEST_ID_READ_OBJECT_STATE_DATA)
      {
        SAMCS::READ_OBJECT_STATE_DATA *pkData                = (SAMCS::READ_OBJECT_STATE_DATA *)&pObjData->dwData;
        SAMCS::READ_OBJECT_STATE_DATA *pkReadObjectStateData = &m_kReadObjectStateData;
        memcpy_s(pkReadObjectStateData, sizeof(SAMCS::READ_OBJECT_STATE_DATA), pkData, sizeof(SAMCS::READ_OBJECT_STATE_DATA));

        //Global Weapon Vector
        m_kVector = pkReadObjectStateData->kLocation.GetVector3<FLOAT>();

    //    //m_kGlobalToLocalTransformMatrix
    //    FLOAT fACLat      = (FLOAT)(pkReadObjectStateData->kLocation.GetLatitudeAngle());
    //    FLOAT fACLon      = (FLOAT)(pkReadObjectStateData->kLocation.GetLongitudeAngle() - D3DX_PI/2.0);
    //    FLOAT fCos_fACLat = cos(fACLat);
    //    FLOAT fSin_fACLat = sin(fACLat);
    //    FLOAT fCos_fACLon = cos(fACLon);
    //    FLOAT fSin_fACLon = sin(fACLon);
    //    LPD3DXMATRIXA16 pkGlobalToLocalTransformMatrix = &m_kGlobalToLocalTransformMatrix;
    //    pkGlobalToLocalTransformMatrix->_11 =  fCos_fACLon;             pkGlobalToLocalTransformMatrix->_21 =  fSin_fACLon;             pkGlobalToLocalTransformMatrix->_31 =  0;           pkGlobalToLocalTransformMatrix->_41 =  0;
    //    pkGlobalToLocalTransformMatrix->_12 = -fCos_fACLat*fSin_fACLon; pkGlobalToLocalTransformMatrix->_22 =  fCos_fACLat*fCos_fACLon; pkGlobalToLocalTransformMatrix->_32 =  fSin_fACLat; pkGlobalToLocalTransformMatrix->_42 =  0;
    //    pkGlobalToLocalTransformMatrix->_13 =  fSin_fACLat*fSin_fACLon; pkGlobalToLocalTransformMatrix->_23 = -fSin_fACLat*fCos_fACLon; pkGlobalToLocalTransformMatrix->_33 =  fCos_fACLat; pkGlobalToLocalTransformMatrix->_43 =  0;
    //    pkGlobalToLocalTransformMatrix->_14 =  0;                       pkGlobalToLocalTransformMatrix->_24 =  0;                       pkGlobalToLocalTransformMatrix->_34 =  0;           pkGlobalToLocalTransformMatrix->_44 =  1;

    //    //m_kLocalToBodyTransformMatrix
    //    D3DXMATRIXA16     kLocToBodyXMatrix;
    //    D3DXMATRIXA16     kLocToBodyYMatrix;
    //    D3DXMATRIXA16     kLocToBodyZMatrix;
    //    D3DXMatrixRotationX(&kLocToBodyXMatrix, -(FLOAT)pkReadObjectStateData->kBodyAttitude.x);
    //    D3DXMatrixRotationY(&kLocToBodyYMatrix, -(FLOAT)pkReadObjectStateData->kBodyAttitude.y);
    //    D3DXMatrixRotationZ(&kLocToBodyZMatrix, -(FLOAT)pkReadObjectStateData->kBodyAttitude.z);
    //    m_kLocalToBodyTransformMatrix = kLocToBodyYMatrix*kLocToBodyXMatrix*kLocToBodyZMatrix;
    //    //D3DXMatrixRotationYawPitchRoll(&pkAC->m_kBodyToLocalTransformMatrix, -(FLOAT)pkAC->m_kBodyAttitude.y, 
    //    //                                                                     -(FLOAT)pkAC->m_kBodyAttitude.x, 
    //    //                                                                     -(FLOAT)pkAC->m_kBodyAttitude.z);
    //    //m_kBodyToLocalTransformMatrix
    //    D3DXMatrixTranspose(&m_kBodyToLocalTransformMatrix, &m_kLocalToBodyTransformMatrix);



    //    ////////////////////////////////////////////////////////////
    //    // START PROCESSING IF THE SAMCS IS ON THE LAUNCH STATE
    //    ////////////////////////////////////////////////////////////
    //    if(GetState() == SAMCS::WEAPON_STATE_LAUNCH)
    //    {
    //      m_kSetAuxObjectStateData.bSmokeEnable = true;
    //      hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, SAMCS::DATA_ID_SET_AUX_OBJECT_STATE_DATA, m_dwObjectID, 0, 1, sizeof(SAMCS::SET_AUX_OBJECT_STATE_DATA), &m_kSetAuxObjectStateData);
    //      AAV_ASSERT(SUCCEEDED(hResult));

    //      //Confirm state change
    //      SetEvent(m_hStateChangeConfirmed);

    //      //Hide static weapon model
    //      set_named_variable_value(GetStaticModelVisibilityVar(), (BOOL)0 );


    //      m_dReleaseTime       = m_pkAvionics->m_dCurrentTime;
    //      m_dTotalAirborneTime = 0;
    //      m_bValid             = true;
    //      SetState(SAMCS::WEAPON_STATE_PURSUIT);
    //    }

    //    ////////////////////////////////////////////////////////////
    //    // DISABLE SAMCS IN CASE OF GROUND CRASH
    //    ////////////////////////////////////////////////////////////
    //    //if((pkReadObjectStateData->dAGLAltitude < 3.0) || (pkReadObjectStateData->eCrashFlag != 0))
    //    //{
    //    //  m_bValid = false;
    //    //  SetState(SAMCS::WEAPON_STATE_NONE);
    //    //  hResult = SimConnect_AIRemoveObject(m_hSimConnect, m_dwObjectID, SAMCS::REQUEST_ID_REMOVE_OBJECT);
    //    //  AAV_ASSERT(SUCCEEDED(hResult));
    //    //}
      }
    }
    break;

  default:
    break;
  }
}


VOID SAMCS::ProcessStateNone()
{
}

VOID SAMCS::ProcessStateIdle()
{
  MSG kMsg;

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_MASTER_ARM:
      break;

    case MESSAGE_RESET:
      break;

    default:
      break;
    }
  }
}

VOID SAMCS::ProcessStateArmed()
{
}

VOID SAMCS::ProcessStateLocked()
{
}

VOID SAMCS::Start()
{
  // CREATE FSX MODEL
  SIMCONNECT_DATA_INITPOSITION          kInitPosition;
  kInitPosition.Latitude              = m_kLocation.Latitude;                                                   ///Degrees
  kInitPosition.Longitude             = m_kLocation.Longitude;                                                  ///Degrees
  kInitPosition.Altitude              = m_kLocation.Altitude * 3.28083989501;                                   ///Feet
  kInitPosition.Pitch                 = m_kAttitude.x;                                                          ///Degrees
  kInitPosition.Heading               = m_kAttitude.y;                                                          ///Degrees
  kInitPosition.Bank                  = m_kAttitude.z;                                                          ///Degrees
  kInitPosition.OnGround              = 1;
  kInitPosition.Airspeed              = 0;

  m_hResult = SimConnect_AICreateSimulatedObject (m_hSimConnect, m_acName.getCStr(), kInitPosition, SAMCS::REQUEST_ID_CREATE_OBJECT);
  AAV_ASSERT(SUCCEEDED(m_hResult));

  __SVSPModule::Start();
}

VOID SAMCS::Process()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT(pkAC  != NULL);

  if(m_eState >= SAMCS::STATE_ID_IDLE)
  {
    m_kSetObjectStateData.kBodyAngularVelocity = AAVVector3<FLOAT>(0, 0.0, 0);
    m_kSetObjectStateData.kBodyVelocity         = AAVVector3<FLOAT>(0, 0, 40.0);
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, SAMCS::DATA_ID_SET_OBJECT_STATE_DATA, m_dwObjectID, 0, 1, sizeof(SAMCS::SET_OBJECT_STATE_DATA), &m_kSetObjectStateData);
    AAV_ASSERT(SUCCEEDED(m_hResult));
  }

  AAV_ASSERT(m_eState < SAMCS::STATE_ID_MAX);
  VOID (__thiscall SAMCS::*__ProcessStateCallback)(VOID) = m_apkProcessStateCallback[m_eState];
  AAV_ASSERT(__ProcessStateCallback != NULL);
  (this->*__ProcessStateCallback)();
}
