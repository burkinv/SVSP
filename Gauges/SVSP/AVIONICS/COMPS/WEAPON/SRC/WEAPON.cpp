#include "AC.h"
#include "SS.h"
#include "WEAPON.h"
#include "SMS.h"


typedef struct
{
  WORD wSecond;
  WORD wMillisecond;
} SysTime;

WEAPON::WEAPON(CHAR *pcName, CHAR *pcType, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude, 
              LPPD3DXGEOMETRYOBJECT ppkGeomObjectsList, UINT uiNumGeomObjects, LPPD3DXTEXTUREOBJECT ppkTextureObjectsList, UINT uiNumTextureObjects) :
__SVSPModule(pcName, pkAvionics, 50, true, WEAPON__DispatchProcess, ppkGeomObjectsList, uiNumGeomObjects, ppkTextureObjectsList, uiNumTextureObjects),
  m_acType(__SVSPString<CHAR>(pcType)),
  m_dwObjectID(0),
  m_uiStationID(uiStationID),
  m_uiUnitID(uiUnitID),
  m_uiWeight(uiWeight),
  m_eState(WEAPON::WEAPON_STATE_IDLE),
  m_pkWeaponGroup(NULL)
{
  m_kPilonVector   = rkPilonVector;
  m_kPilonAttitude = rkPilonAttitude;

  m_dReleaseTime       = 0;
  m_dTotalAirborneTime = 0;

  ///////////////////////////////////////////////////////////////////
  // WEAPON::DATA_ID_READ_OBJECT_STATE_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "SIM ON GROUND",                      "Bool",               SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "CRASH FLAG",                         "Enum",               SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE ALT ABOVE GROUND",             "Feet",               SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE PITCH DEGREES",                "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE HEADING DEGREES TRUE",         "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE BANK DEGREES",                 "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY X",           "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Y",           "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Z",           "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "VELOCITY BODY X",                    "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "VELOCITY BODY Y",                    "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "VELOCITY BODY Z",                    "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "STRUCT LATLONALT",                   "radians",            SIMCONNECT_DATATYPE_LATLONALT);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE LATITUDE",                     "degrees",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE LONGITUDE",                    "degrees",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, "PLANE ALTITUDE",                     "feet"   ,            SIMCONNECT_DATATYPE_FLOAT32);


  ///////////////////////////////////////////////////////////////////
  // WEAPON::DATA_ID_SET_OBJECT_STATE_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY X",            "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Y",            "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Z",            "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY X",                     "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY Y",                     "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY Z",                     "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_SET_OBJECT_STATE_DATA, "PLANE PITCH DEGREES",                 "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_SET_OBJECT_STATE_DATA, "PLANE HEADING DEGREES TRUE",          "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, WEAPON::DATA_ID_SET_OBJECT_STATE_DATA, "PLANE BANK DEGREES",                  "radians",            SIMCONNECT_DATATYPE_FLOAT32);

  //Initialize Process callback table.
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_IDLE]                     = &WEAPON::ProcessStateIdle;
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_ARMED]                    = &WEAPON::ProcessStateArmed;
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_ACQUISITION]              = &WEAPON::ProcessStateAcquisition;
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_LOCKED]                   = &WEAPON::ProcessStateLocked;
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_LAUNCH]                   = &WEAPON::ProcessStateLaunch;
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_WAIT_LAUNCH_ACKNOWLEDGE]  = &WEAPON::ProcessStateWaitLaunchAcknowledge;
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_PURSUIT]                  = &WEAPON::ProcessStatePursuit;
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_MISSED]                   = &WEAPON::ProcessStateMissed;
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_DETONATE]                 = &WEAPON::ProcessStateDetonate;
  m_apkProcessStateCallback[WEAPON::WEAPON_STATE_NONE]                     = &WEAPON::ProcessStateNone;
}

WEAPON::~WEAPON()
{
  Stop();
}

HRESULT WEAPON::SetState(WEAPON::STATE eState)
{
  HRESULT hResult = S_FALSE;

  m_eState = eState;
  hResult = S_OK;

  return hResult;
}

VOID WEAPON::SetTargetID(DWORD dwObjectID)
{
  m_kTarget.dwObjectID = dwObjectID;
}

LPD3DXTEXTUREOBJECT WEAPON::GetWeaponTexture() const
{
  return NULL;
}


VOID CALLBACK  WEAPON__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  WEAPON *__THIS = static_cast<WEAPON *>(pContext);
  if(!__THIS->m_bAbort)
  {
    __THIS->DispatchProcess(pData, cbData);
  }
}

VOID WEAPON::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
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
        if(pObjData->dwRequestID == WEAPON::REQUEST_ID_CREATE_OBJECT)
        {
          m_dwObjectID = pObjData->dwObjectID;
          sprintf_s(&acStr[0], sizeof(acStr), "CREATED OBJECT_ID = %d", m_dwObjectID);
          __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);

          hResult = SimConnect_AIReleaseControl(m_hSimConnect, m_dwObjectID, WEAPON::REQUEST_ID_RELEASE_CONTROL);
          AAV_ASSERT(SUCCEEDED(hResult));

          memset(&m_kData, 0, sizeof(WEAPON::READ_OBJECT_STATE_DATA));
          hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, WEAPON::REQUEST_ID_READ_OBJECT_STATE_DATA, WEAPON::DATA_ID_READ_OBJECT_STATE_DATA, m_dwObjectID, SIMCONNECT_PERIOD_VISUAL_FRAME);
          AAV_ASSERT(SUCCEEDED(hResult));

          m_bValid = true;
          SetState(WEAPON::WEAPON_STATE_PURSUIT);
        }
      }
      break;

    case SIMCONNECT_RECV_ID_EVENT:
      break;

    case SIMCONNECT_RECV_ID_EVENT_OBJECT_ADDREMOVE:
      {
        SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE *pEvt = (SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE*)pData;
        if((pEvt->uEventID == WEAPON::EVENT_ID_OBJECT_REMOVED) && (pEvt->dwData == m_dwObjectID))
        {
          sprintf_s(&acStr[0], sizeof(acStr), "REMOVED OBJECT_ID = %d", m_dwObjectID);
          __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
          m_bValid = false;
          SetState(WEAPON::WEAPON_STATE_NONE);
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
        if(pObjData->dwRequestID == WEAPON::REQUEST_ID_READ_OBJECT_STATE_DATA)
        {
          WEAPON::READ_OBJECT_STATE_DATA *pkData                = (WEAPON::READ_OBJECT_STATE_DATA *)&pObjData->dwData;
          WEAPON::READ_OBJECT_STATE_DATA *pkReadObjectStateData = &m_kData;
          memcpy_s(pkReadObjectStateData, sizeof(WEAPON::READ_OBJECT_STATE_DATA), pkData, sizeof(WEAPON::READ_OBJECT_STATE_DATA));
          m_dwProcessUpdateMask |= WEAPON::PROCESS_UPDATE_WEAPON_STATE_DATA;
        }
      }
      break;

    default:
      break;
    }
}

VOID WEAPON::Start()
{
  // Subscribe to system events
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, WEAPON::EVENT_ID_OBJECT_ADDED, "ObjectAdded");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, WEAPON::EVENT_ID_OBJECT_REMOVED, "ObjectRemoved");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  __SVSPModule::Start();

}

VOID WEAPON::Stop()
{
  // Unsubscribe from system events
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, WEAPON::EVENT_ID_OBJECT_ADDED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, WEAPON::EVENT_ID_OBJECT_REMOVED);
  AAV_ASSERT(SUCCEEDED(m_hResult));


  __SVSPModule::Stop();
}

VOID WEAPON::Process()
{
  if((m_dwProcessUpdateMask & WEAPON::PROCESS_UPDATE_WEAPON_STATE_DATA) == WEAPON::PROCESS_UPDATE_WEAPON_STATE_DATA)
  {
  //m_kGlobalToLocalTransformMatrix
  FLOAT fACLat      = (FLOAT)(m_kData.kLocation.GetLatitudeAngle());
  FLOAT fACLon      = (FLOAT)(m_kData.kLocation.GetLongitudeAngle() - D3DX_PI/2.0);
  FLOAT fCos_fACLat = cos(fACLat);
  FLOAT fSin_fACLat = sin(fACLat);
  FLOAT fCos_fACLon = cos(fACLon);
  FLOAT fSin_fACLon = sin(fACLon);
  m_kGlobalToLocalTransformMatrix._11 =  fCos_fACLon;             m_kGlobalToLocalTransformMatrix._21 =  fSin_fACLon;             m_kGlobalToLocalTransformMatrix._31 =  0;           m_kGlobalToLocalTransformMatrix._41 =  0;
  m_kGlobalToLocalTransformMatrix._12 = -fCos_fACLat*fSin_fACLon; m_kGlobalToLocalTransformMatrix._22 =  fCos_fACLat*fCos_fACLon; m_kGlobalToLocalTransformMatrix._32 =  fSin_fACLat; m_kGlobalToLocalTransformMatrix._42 =  0;
  m_kGlobalToLocalTransformMatrix._13 =  fSin_fACLat*fSin_fACLon; m_kGlobalToLocalTransformMatrix._23 = -fSin_fACLat*fCos_fACLon; m_kGlobalToLocalTransformMatrix._33 =  fCos_fACLat; m_kGlobalToLocalTransformMatrix._43 =  0;
  m_kGlobalToLocalTransformMatrix._14 =  0;                       m_kGlobalToLocalTransformMatrix._24 =  0;                       m_kGlobalToLocalTransformMatrix._34 =  0;           m_kGlobalToLocalTransformMatrix._44 =  1;

  //m_kLocalToBodyTransformMatrix
  D3DXMATRIXA16     kLocToBodyXMatrix;
  D3DXMATRIXA16     kLocToBodyYMatrix;
  D3DXMATRIXA16     kLocToBodyZMatrix;
  D3DXMatrixRotationX(&kLocToBodyXMatrix, -(FLOAT)m_kData.kBodyAttitude.x);
  D3DXMatrixRotationY(&kLocToBodyYMatrix, -(FLOAT)m_kData.kBodyAttitude.y);
  D3DXMatrixRotationZ(&kLocToBodyZMatrix, -(FLOAT)m_kData.kBodyAttitude.z);
  m_kLocalToBodyTransformMatrix = kLocToBodyYMatrix*kLocToBodyXMatrix*kLocToBodyZMatrix;
  //D3DXMatrixRotationYawPitchRoll(&pkAC->m_kBodyToLocalTransformMatrix, -(FLOAT)pkAC->m_kBodyAttitude.y, 
  //                                                                     -(FLOAT)pkAC->m_kBodyAttitude.x, 
  //                                                                     -(FLOAT)pkAC->m_kBodyAttitude.z);
  //m_kBodyToLocalTransformMatrix
  D3DXMatrixTranspose(&m_kBodyToLocalTransformMatrix, &m_kLocalToBodyTransformMatrix);

  //Global Weapon Vector
  m_kVector = m_kData.kLocation.GetVector3<FLOAT>();

  ////////////////////////////////////////////////////////////
  // DISABLE WEAPON IN CASE OF GROUND CRASH
  ////////////////////////////////////////////////////////////
  //if((m_kData.dAGLAltitude < 3.0) || (m_kData.eCrashFlag != 0))
  //{
  //  m_bValid = false;
  //  SetState(WEAPON::WEAPON_STATE_NONE);
  //  hResult = SimConnect_AIRemoveObject(m_hSimConnect, m_dwObjectID, WEAPON::REQUEST_ID_REMOVE_OBJECT);
  //  AAV_ASSERT(SUCCEEDED(hResult));
  //}

    m_dwProcessUpdateMask &= ~WEAPON::PROCESS_UPDATE_WEAPON_STATE_DATA;
  }

  AAV_ASSERT(m_eState < WEAPON::WEAPON_STATE_MAX);
  VOID (__thiscall WEAPON::*__ProcessStateCallback)(VOID) = m_apkProcessStateCallback[m_eState];
  AAV_ASSERT(__ProcessStateCallback != NULL);
  (this->*__ProcessStateCallback)();
}
