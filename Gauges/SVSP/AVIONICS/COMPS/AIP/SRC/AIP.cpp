#include "AC.h"
#include "AIP.h"
#include "ECS.h"
#include "__SVSPAvionics.h"

AIP::AIP(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, CHAR *pcName, __SVSPGeoCoordinates3<FLOAT> *pkLocation, AAVVector3<FLOAT> *pkAttitude) :
__SVSPModule(pcName, pkAvionics, dwThreadSleepTime, true, AIP__DispatchProcess, NULL, 0)
{
  m_kLocation = *pkLocation;
  m_kAttitude = *pkAttitude;
  m_kVector = m_kLocation.GetVector3<FLOAT>();

  // Subscribe to system events
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, AIP::EVENT_ID_OBJECT_ADDED, "ObjectAdded");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, AIP::EVENT_ID_OBJECT_REMOVED, "ObjectRemoved");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  ///////////////////////////////////////////////////////////////////
  // AIP::DATA_ID_READ_OBJECT_STATE_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "SIM ON GROUND",                          "Bool",               SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE PITCH DEGREES",                    "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE HEADING DEGREES TRUE",             "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE BANK DEGREES",                     "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY X",               "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Y",               "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Z",               "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "VELOCITY BODY X",                        "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "VELOCITY BODY Y",                        "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "VELOCITY BODY Z",                        "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "STRUCT LATLONALT",                     "radians",            SIMCONNECT_DATATYPE_LATLONALT);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE LATITUDE",                         "degrees",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE LONGITUDE",                        "degrees",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE ALTITUDE",                         "feet",               SIMCONNECT_DATATYPE_FLOAT32);

  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "AIRSPEED INDICATED",                     "Knots",              SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "TOTAL WEIGHT",                           "Pounds",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "GEAR CENTER POSITION",                   "Percent Over 100",   SIMCONNECT_DATATYPE_FLOAT32);

  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "GENERAL ENG PROPELLER LEVER POSITION:1", "Percent",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "GENERAL ENG THROTTLE LEVER POSITION:1",  "Percent",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_READ_OBJECT_STATE_DATA, "TURB ENG JET THRUST:1",                  "Pounds",             SIMCONNECT_DATATYPE_FLOAT32);


  ///////////////////////////////////////////////////////////////////
  // AIP::DATA_ID_SET_OBJECT_VELOCITY_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_VELOCITY_DATA, "ROTATION VELOCITY BODY X",             "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_VELOCITY_DATA, "ROTATION VELOCITY BODY Y",             "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_VELOCITY_DATA, "ROTATION VELOCITY BODY Z",             "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_VELOCITY_DATA, "VELOCITY BODY X",                      "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_VELOCITY_DATA, "VELOCITY BODY Y",                      "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_VELOCITY_DATA, "VELOCITY BODY Z",                      "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);

  ///////////////////////////////////////////////////////////////////
  // AIP::DATA_ID_SET_OBJECT_GEAR_HANDLE
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_GEAR_HANDLE, "GEAR HANDLE POSITION",                    "Bool",          SIMCONNECT_DATATYPE_INT32);

  // AIP::DATA_ID_SET_OBJECT_PL
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_PL,          "GENERAL ENG PROPELLER LEVER POSITION:1",  "Percent",       SIMCONNECT_DATATYPE_FLOAT32);

  // AIP::DATA_ID_SET_OBJECT_ENGINE_THROTTLE
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_ET,          "GENERAL ENG THROTTLE LEVER POSITION:1",   "Percent",       SIMCONNECT_DATATYPE_FLOAT32);

  ///////////////////////////////////////////////////////////////////
  // STATE_CALLBACK_TABLE
  ///////////////////////////////////////////////////////////////////
  m_apkProcessStateCallback[AIP::STATE_ID_NONE]      = &AIP::ProcessStateNone;
  m_apkProcessStateCallback[AIP::STATE_ID_IDLE]      = &AIP::ProcessStateIdle;
  m_apkProcessStateCallback[AIP::STATE_ID_TAXING]    = &AIP::ProcessStateTaxing;
  m_apkProcessStateCallback[AIP::STATE_ID_TAKEOFF]   = &AIP::ProcessStateTakeoff;
  m_apkProcessStateCallback[AIP::STATE_ID_ENROUTE]   = &AIP::ProcessStateEnroute;
  m_apkProcessStateCallback[AIP::STATE_ID_APPROACH]  = &AIP::ProcessStateApproach;
  m_apkProcessStateCallback[AIP::STATE_ID_LANDING]   = &AIP::ProcessStateLanding;

  m_eState = AIP::STATE_ID_NONE;
}


AIP::~AIP()
{
  __SVSPModule::Stop();

  // Unsubscribe from system events
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, AIP::EVENT_ID_OBJECT_ADDED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, AIP::EVENT_ID_OBJECT_REMOVED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

HRESULT AIP::Create()
{
  return PostThreadMessage(m_dwThreadId, AIP::MESSAGE_CREATE, 0, 0);
}
  
HRESULT AIP::Destroy()
{
  return PostThreadMessage(m_dwThreadId, AIP::MESSAGE_DESTROY, 0, 0);
}

HRESULT AIP::SetAltitude(FLOAT fAltitude)
{
  AAV_ASSERT_RETURN(fAltitude >= 0, S_FALSE);
  m_fReferenceAltitude = fAltitude;
  return S_OK;
}



HRESULT AIP::CreateObject()
{
  ////////////////////////////////////////////////////////////////////////////
  // CREATE FSX MODEL
  ////////////////////////////////////////////////////////////////////////////
  SIMCONNECT_DATA_INITPOSITION          kInitPosition;
  kInitPosition.Latitude              = m_kLocation.Latitude;                                                   ///Degrees
  kInitPosition.Longitude             = m_kLocation.Longitude;                                                  ///Degrees
  kInitPosition.Altitude              = m_kLocation.Altitude * 3.28083989501;                                   ///Feet
  kInitPosition.Pitch                 = m_kAttitude.x;                                                          ///Degrees
  kInitPosition.Heading               = m_kAttitude.y;                                                          ///Degrees
  kInitPosition.Bank                  = m_kAttitude.z;                                                          ///Degrees
  kInitPosition.OnGround              = 1;
  kInitPosition.Airspeed              = 0;

  //m_hResult = SimConnect_AICreateSimulatedObject (m_hSimConnect, m_acName.getCStr(), kInitPosition, AIP::REQUEST_ID_CREATE_OBJECT);
  m_hResult = SimConnect_AICreateNonATCAircraft (m_hSimConnect, m_acName.getCStr(), "NK01", kInitPosition, AIP::REQUEST_ID_CREATE_OBJECT);
  AAV_ASSERT(SUCCEEDED(m_hResult));

  return m_hResult;
}

HRESULT AIP::RemoveObject()
{
  AAV_ASSERT_RETURN(m_dwObjectID != 0, S_FALSE);

  m_hResult = SimConnect_AIRemoveObject(m_hSimConnect, m_dwObjectID, AIP::REQUEST_ID_REMOVE_OBJECT);
  AAV_ASSERT(SUCCEEDED(m_hResult));

  return m_hResult;
}

VOID AIP::SetObjectVelocityData()
{
  //m_kBodyVelocity += m_kBodyAcceleration*m_dDeltaTime;   m_kData.fThrust/32*m_kData.

  //m_hResult = SimConnect_AIReleaseControl(m_hSimConnect, m_dwObjectID, AIP::REQUEST_ID_RELEASE_CONTROL);
  //AAV_ASSERT(SUCCEEDED(m_hResult));
  m_kSetObjectVelocityData.kBodyAngularVelocity = m_kBodyAngularVelocity;
  //m_kSetObjectVelocityData.kBodyVelocity        = m_kBodyVelocity;
  m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_VELOCITY_DATA, m_dwObjectID, 0, 1, sizeof(AIP::SET_OBJECT_VELOCITY_DATA), &m_kSetObjectVelocityData);
  AAV_ASSERT(SUCCEEDED(m_hResult));

}

VOID AIP::SetObjectGearData(INT bGearHandle)
{
  m_kSetObjectGearHandle.bGearHandle = bGearHandle;
  m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_GEAR_HANDLE, m_dwObjectID, 0, 1, sizeof(AIP::SET_OBJECT_GEAR_HANDLE), &m_kSetObjectGearHandle);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

VOID AIP::SetObjectPL(FLOAT dPL)
{
  m_kSetObjectPL.dPL = dPL;
  m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_PL, m_dwObjectID, 0, 1, sizeof(AIP::SET_OBJECT_PL), &m_kSetObjectPL);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

VOID AIP::SetObjectET(FLOAT dET)
{
  m_kSetObjectET.dET = dET;
  m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, AIP::DATA_ID_SET_OBJECT_ET, m_dwObjectID, 0, 1, sizeof(AIP::SET_OBJECT_ET), &m_kSetObjectET);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

VOID AIP::AdjustEnginePower(FLOAT fReferenceSpeed)
{
  FLOAT dET = m_kData.dET + 0.2f*(fReferenceSpeed - m_kData.fIndicatedAirspeed);
  dET = (dET > 100) ? 100 : ( (dET < 0) ? 0 : dET );
  SetObjectET(dET);
}

VOID CALLBACK  AIP__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  AIP *__THIS = static_cast<AIP *>(pContext);
  if(!__THIS->m_bAbort)
  {
    __THIS->DispatchProcess(pData, cbData);
  }
}

VOID AIP::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
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
      if(pObjData->dwRequestID == AIP::REQUEST_ID_CREATE_OBJECT)
      {
        m_dwObjectID = pObjData->dwObjectID;
        sprintf_s(&acStr[0], sizeof(acStr), "CREATED OBJECT_ID = %d", m_dwObjectID);
        __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);

        hResult = SimConnect_AIReleaseControl(m_hSimConnect, m_dwObjectID, AIP::REQUEST_ID_RELEASE_CONTROL);
        AAV_ASSERT(SUCCEEDED(hResult));

        m_eState = AIP::STATE_ID_IDLE;

        memset(&m_kData, 0, sizeof(AIP::READ_OBJECT_STATE_DATA));
        hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, AIP::REQUEST_ID_READ_OBJECT_STATE_DATA, AIP::DATA_ID_READ_OBJECT_STATE_DATA, m_dwObjectID, SIMCONNECT_PERIOD_SIM_FRAME);
        AAV_ASSERT(SUCCEEDED(hResult));
      }
    }
    break;

  case SIMCONNECT_RECV_ID_EVENT:
    break;

  case SIMCONNECT_RECV_ID_EVENT_OBJECT_ADDREMOVE:
    {
      SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE *pEvt = (SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE*)pData;
      if((pEvt->uEventID == AIP::EVENT_ID_OBJECT_REMOVED) && (pEvt->dwData == m_dwObjectID))
      {
        sprintf_s(&acStr[0], sizeof(acStr), "REMOVED OBJECT_ID = %d", m_dwObjectID);
        __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);

        m_eState = AIP::STATE_ID_NONE;
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
      if(pObjData->dwRequestID == AIP::REQUEST_ID_READ_OBJECT_STATE_DATA)
      {
        AIP::READ_OBJECT_STATE_DATA *pkData                = (AIP::READ_OBJECT_STATE_DATA *)&pObjData->dwData;
        AIP::READ_OBJECT_STATE_DATA *pkReadObjectStateData = &m_kData;
        memcpy_s(pkReadObjectStateData, sizeof(AIP::READ_OBJECT_STATE_DATA), pkData, sizeof(AIP::READ_OBJECT_STATE_DATA));

        //Global Weapon Vector
        m_kVector = pkReadObjectStateData->kLocation.GetVector3<FLOAT>();

        m_dMass   = pkReadObjectStateData->dTotalWeight/32.2f;


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
    //    // START PROCESSING IF THE AIP IS ON THE LAUNCH STATE
    //    ////////////////////////////////////////////////////////////
    //    if(GetState() == AIP::WEAPON_STATE_LAUNCH)
    //    {
    //      m_kSetAuxObjectStateData.bSmokeEnable = true;
    //      hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, AIP::DATA_ID_SET_AUX_OBJECT_STATE_DATA, m_dwObjectID, 0, 1, sizeof(AIP::SET_AUX_OBJECT_STATE_DATA), &m_kSetAuxObjectStateData);
    //      AAV_ASSERT(SUCCEEDED(hResult));

    //      //Confirm state change
    //      SetEvent(m_hStateChangeConfirmed);

    //      //Hide static weapon model
    //      set_named_variable_value(GetStaticModelVisibilityVar(), (BOOL)0 );


    //      m_dReleaseTime       = m_pkAvionics->m_dCurrentTime;
    //      m_dTotalAirborneTime = 0;
    //      m_bValid             = true;
    //      SetState(AIP::WEAPON_STATE_PURSUIT);
    //    }

    //    ////////////////////////////////////////////////////////////
    //    // DISABLE AIP IN CASE OF GROUND CRASH
    //    ////////////////////////////////////////////////////////////
    //    //if((pkReadObjectStateData->dAGLAltitude < 3.0) || (pkReadObjectStateData->eCrashFlag != 0))
    //    //{
    //    //  m_bValid = false;
    //    //  SetState(AIP::WEAPON_STATE_NONE);
    //    //  hResult = SimConnect_AIRemoveObject(m_hSimConnect, m_dwObjectID, AIP::REQUEST_ID_REMOVE_OBJECT);
    //    //  AAV_ASSERT(SUCCEEDED(hResult));
    //    //}
      }
    }
    break;

  default:
    break;
  }
}

HRESULT AIP::SetState(AIP::STATE eState)
{
  AAV_ASSERT_RETURN(eState < AIP::STATE_ID_MAX, S_FALSE);
  m_eState = eState;
  return S_OK;
}

VOID AIP::ProcessStateNone()
{
  MSG kMsg;

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_CREATE:
      CreateObject();
      break;
    }
  }

}

VOID AIP::ProcessStateIdle()
{
  MSG kMsg;

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_DESTROY:
      RemoveObject();
      break;
    }
  }

  m_kBodyVelocity        = AAVVector3<FLOAT>(0, 0, 0);
  m_kBodyAngularVelocity = AAVVector3<FLOAT>(0, 0, 0);

  SetObjectVelocityData();

  if(m_kData.dPL != ECS::THRUST_VECTOR_VALUE_LEVEL_OFF)
  {
    SetObjectPL(ECS::THRUST_VECTOR_VALUE_LEVEL_OFF);
  }

}

VOID AIP::ProcessStateTaxing()
{
  MSG kMsg;

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_DESTROY:
      RemoveObject();
      break;
    }
  }

  m_kBodyVelocity        = AAVVector3<FLOAT>(0, 0, 1.0f);
  m_kBodyAngularVelocity = AAVVector3<FLOAT>(0, 0, 0);


}

VOID AIP::ProcessStateTakeoff()
{
  MSG kMsg;

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_DESTROY:
      RemoveObject();
      break;
    }
  }

  // Adjust engine power
  AdjustEnginePower(550);


  // Rotate when ready
  if(m_kData.fIndicatedAirspeed > 145.0f)
  {
    m_kBodyAngularVelocity = AAVVector3<FLOAT>(-0.7f*(m_kData.kBodyAttitude.x + 0.1745f), 0, 0);
    SetObjectVelocityData();

    if(!m_kData.bOnGround && (m_kData.dGearPosition == 1))
    {
      SetObjectGearData(0);
    }
  }

  m_fReferenceAltitude = 1000;

  // Set enroute when ready
  if(m_kData.kLocation.Altitude > m_fReferenceAltitude)
  {
    m_eState = AIP::STATE_ID_ENROUTE;
  }

}

VOID AIP::ProcessStateEnroute()
{
  MSG kMsg;

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_DESTROY:
      RemoveObject();
      break;
    }
  }


  // Adjust engine power
  AdjustEnginePower(550);

  // Level off to the reference altitude.
  m_kBodyAngularVelocity = AAVVector3<FLOAT>(0.07f*__SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(m_kData.kLocation.Altitude, m_fReferenceAltitude, 0.05f) - 0.5f*m_kBodyAngularVelocity.x - 0.5f*m_kData.kBodyAttitude.x , 0, 0);
  SetObjectVelocityData();
}

VOID AIP::ProcessStateApproach()
{
  MSG kMsg;

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_DESTROY:
      RemoveObject();
      break;
    }
  }

}

VOID AIP::ProcessStateLanding()
{
  MSG kMsg;

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_DESTROY:
      RemoveObject();
      break;
    }
  }

}

VOID AIP::Start()
{
  __SVSPModule::Start();
}

VOID AIP::Process()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT(pkAC  != NULL);

  AAV_ASSERT(m_eState < AIP::STATE_ID_MAX);
  VOID (__thiscall AIP::*__ProcessStateCallback)(VOID) = m_apkProcessStateCallback[m_eState];
  AAV_ASSERT(__ProcessStateCallback != NULL);
  (this->*__ProcessStateCallback)();
}
