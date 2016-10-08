#include "__SVSPAvionics.h"
#include "AC.h"
#include "FLCS.h"
#include "SIM.h"
#include "CARRIER.h"


AC::AC(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("AC", pkAvionics, dwThreadSleepTime, true, AC__DispatchProcess, NULL, NULL)
{
  D3DXMatrixIdentity(&m_matACRot);
  D3DXMatrixIdentity(&m_matVPRot);

  memset(&m_kData,            0, sizeof(AC::AIRCRAFT_STATE_DATA));

  m_fAoA              = 0;

  // DATA_ID_READ_OBJECT_CRITICAL_DATA
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_CRITICAL_DATA,  "PLANE PITCH DEGREES",              "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_CRITICAL_DATA,  "PLANE HEADING DEGREES TRUE",       "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_CRITICAL_DATA,  "PLANE BANK DEGREES",               "radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_CRITICAL_DATA,  "VELOCITY BODY X",                  "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_CRITICAL_DATA,  "VELOCITY BODY Y",                  "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_CRITICAL_DATA,  "VELOCITY BODY Z",                  "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_CRITICAL_DATA,  "PLANE ALT ABOVE GROUND",           "Feet",               SIMCONNECT_DATATYPE_FLOAT32);
  // DATA_ID_READ_LATLONALT_DATA
  //m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_LATLONALT_DATA,      "STRUCT LATLONALT",                 "radians",            SIMCONNECT_DATATYPE_LATLONALT);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_LATLONALT_DATA,        "PLANE LATITUDE",                   "degrees",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_LATLONALT_DATA,        "PLANE LONGITUDE",                  "degrees",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_LATLONALT_DATA,        "PLANE ALTITUDE",                   "feet",               SIMCONNECT_DATATYPE_FLOAT32);
  // DATA_ID_READ_OBJECT_STATE_DATA
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "SIM ON GROUND",                    "Bool",               SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "TOTAL WEIGHT",                     "Pounds",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "INCIDENCE ALPHA",                  "Radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "ANGLE OF ATTACK INDICATOR",        "Radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "INDICATED ALTITUDE",               "Feet",               SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "AIRSPEED INDICATED",               "Knots",              SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "GROUND VELOCITY",                  "Knots",              SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "VERTICAL SPEED",                   "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "PLANE HEADING DEGREES MAGNETIC",   "Radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "AIRSPEED MACH",                    "Mach",               SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "G FORCE",                          "Gforce",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "MAX G FORCE",                      "Gforce",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "GEAR CENTER POSITION",             "Percent Over 100",   SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "ACCELERATION BODY X",              "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "ACCELERATION BODY Y",              "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "ACCELERATION BODY Z",              "Feet per second",    SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "ROTATION VELOCITY BODY X",         "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "ROTATION VELOCITY BODY Y",         "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "ROTATION VELOCITY BODY Z",         "Radians per second", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "FLAPS HANDLE INDEX",               "Number",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "ELEVATOR POSITION",                "position",           SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "RUDDER POSITION",                  "position",           SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "AILERON POSITION",                 "position",           SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "ELEVATOR TRIM POSITION",           "Radians",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "FUEL TOTAL QUANTITY WEIGHT",       "Pounds",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "SPOILERS LEFT POSITION",           "Percent Over 100",   SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "LOCAL TIME",                       "Seconds",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "BRAKE LEFT POSITION",              "Position",           SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA,     "BRAKE PARKING POSITION",           "Position",           SIMCONNECT_DATATYPE_FLOAT32);
  // DATA_ID_READ_OBJECT_NAV_DATA
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_NAV_RADIO_DATA, "NAV ACTIVE FREQUENCY:1",           "MHz",                SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_NAV_RADIO_DATA, "NAV HAS LOCALIZER:1",              "Bool",               SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_NAV_RADIO_DATA, "NAV HAS GLIDE SLOPE:1",            "Bool",               SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_NAV_RADIO_DATA, "NAV HAS DME:1",                    "Bool",               SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_NAV_RADIO_DATA, "NAV CDI:1",                        "Number",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_NAV_RADIO_DATA, "NAV GSI:1",                        "Number",             SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_NAV_RADIO_DATA, "NAV DME:1",                        "Nautical miles",     SIMCONNECT_DATATYPE_FLOAT32);
  // DATA_ID_READ_OBJECT_RADIO_DATA
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_COM_RADIO_DATA, "COM ACTIVE FREQUENCY:1",           "Frequency BCD16",    SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_COM_RADIO_DATA, "COM ACTIVE FREQUENCY:2",           "Frequency BCD16",    SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_COM_RADIO_DATA, "COM STANDBY FREQUENCY:1",          "Frequency BCD16",    SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_COM_RADIO_DATA, "COM STANDBY FREQUENCY:2",          "Frequency BCD16",    SIMCONNECT_DATATYPE_INT32);


  m_idRadiansEnum       = get_units_enum("Radians");
  m_idFeetPerSecondEnum = get_units_enum("Feet per second");
}

AC::~AC()
{
  __SVSPModule::Stop();

  // Clear all data definitions
  m_hResult = SimConnect_ClearDataDefinition(m_hSimConnect, AC::DATA_ID_READ_LATLONALT_DATA);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_ClearDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_CRITICAL_DATA);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_ClearDataDefinition(m_hSimConnect, AC::DATA_ID_READ_OBJECT_STATE_DATA);
  AAV_ASSERT(SUCCEEDED(m_hResult));
}

VOID CALLBACK  AC__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  AC *__THIS = static_cast<AC *>(pContext);
  if(!__THIS->m_bAbort)
  {
    __THIS->DispatchProcess(pData, cbData);
  }
}


VOID AC::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
{
  switch(pData->dwID)
  {
  case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
    {
      SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

      switch(pObjData->dwRequestID)
      {
      case AC::REQUEST_ID_READ_LATLONALT_DATA:
        {
          m_kLocation  = *((__SVSPGeoCoordinates3<FLOAT> *)&pObjData->dwData);
          m_dwProcessUpdateMask |= AC::PROCESS_UPDATE_AIRCRAFT_LOCATION_DATA;
        }
        break;

      case AC::REQUEST_ID_READ_OBJECT_STATE_DATA:
        {
          AC::AIRCRAFT_STATE_DATA *pkData = (AC::AIRCRAFT_STATE_DATA*)&pObjData->dwData;
          memcpy(&m_kData, pkData, sizeof(AC::AIRCRAFT_STATE_DATA));
          m_dwProcessUpdateMask |= AC::PROCESS_UPDATE_AIRCRAFT_STATE_DATA;
        }
        break;

      case AC::REQUEST_ID_READ_OBJECT_NAV_RADIO_DATA:
        {
          AC::AIRCRAFT_NAV_RADIO_DATA *pkNavRadioData = (AC::AIRCRAFT_NAV_RADIO_DATA*)&pObjData->dwData;
          memcpy(&m_kNavRadioData, pkNavRadioData, sizeof(AC::AIRCRAFT_NAV_RADIO_DATA));
          m_dwProcessUpdateMask |= AC::PROCESS_UPDATE_AIRCRAFT_NAV_RADIO_DATA;
        }
        break;

      case AC::REQUEST_ID_READ_OBJECT_COM_RADIO_DATA:
        {
          AC::AIRCRAFT_COM_RADIO_DATA *pkComRadioData = (AC::AIRCRAFT_COM_RADIO_DATA*)&pObjData->dwData;
          memcpy(&m_kComRadioData, pkComRadioData, sizeof(AC::AIRCRAFT_COM_RADIO_DATA));
          m_dwProcessUpdateMask |= AC::PROCESS_UPDATE_AIRCRAFT_COM_RADIO_DATA;
        }
        break;
      }
    }
    break;

  }
}


VOID AC::GetDataEnums()
{
  //m_kDataEnums.m_dAbsoluteTimeEnum                    = get_aircraft_var_enum("ABSOLUTE TIME");             //AAV_ASSERT(m_kDataEnums.m_dAbsoluteTimeEnum        != SIMVAR_NONE);
  m_kDataEnums.m_kBodyAttitudeEnum.x                  = get_aircraft_var_enum("PLANE PITCH DEGREES");         //AAV_ASSERT(m_kDataEnums.m_kBodyAttitudeEnum.x      != SIMVAR_NONE);
  m_kDataEnums.m_kBodyAttitudeEnum.y                  = get_aircraft_var_enum("PLANE HEADING DEGREES TRUE");  //AAV_ASSERT(m_kDataEnums.m_kBodyAttitudeEnum.y      != SIMVAR_NONE);
  m_kDataEnums.m_kBodyAttitudeEnum.z                  = get_aircraft_var_enum("PLANE BANK DEGREES");          //AAV_ASSERT(m_kDataEnums.m_kBodyAttitudeEnum.z      != SIMVAR_NONE);
  m_kDataEnums.m_kBodyVelocityEnum.x                  = get_aircraft_var_enum("VELOCITY BODY X");             //AAV_ASSERT(m_kDataEnums.m_kBodyVelocityEnum.x      != SIMVAR_NONE);
  m_kDataEnums.m_kBodyVelocityEnum.y                  = get_aircraft_var_enum("VELOCITY BODY Y");             //AAV_ASSERT(m_kDataEnums.m_kBodyVelocityEnum.y      != SIMVAR_NONE);
  m_kDataEnums.m_kBodyVelocityEnum.z                  = get_aircraft_var_enum("VELOCITY BODY Z");             //AAV_ASSERT(m_kDataEnums.m_kBodyVelocityEnum.z      != SIMVAR_NONE);
  m_kDataEnums.m_dRadarAltitudeEnum                   = get_aircraft_var_enum("PLANE ALT ABOVE GROUND");      //AAV_ASSERT(m_kDataEnums.m_dRadarAltitudeEnum       != SIMVAR_NONE);
}

VOID  AC::ReadCriticalData()
{
  SIM *pkSIM    = static_cast<SIM *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SIM));
  FLCS *pkFLCS  = static_cast<FLCS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FLCS));

  // m_kBodyAttitude
  m_kBodyAttitude      = AAVVector3<FLOAT>((FLOAT)aircraft_varget( m_kDataEnums.m_kBodyAttitudeEnum.x, m_idRadiansEnum, 0),
                                            (FLOAT)aircraft_varget( m_kDataEnums.m_kBodyAttitudeEnum.y, m_idRadiansEnum, 0),
                                            (FLOAT)aircraft_varget( m_kDataEnums.m_kBodyAttitudeEnum.z, m_idRadiansEnum, 0));
  // Rotation matrices
  D3DXMatrixRotationX(&m_kLocalToBodyXMatrix, -(FLOAT)m_kBodyAttitude.x);
  D3DXMatrixRotationY(&m_kLocalToBodyYMatrix, -(FLOAT)m_kBodyAttitude.y);
  D3DXMatrixRotationZ(&m_kLocalToBodyZMatrix, -(FLOAT)m_kBodyAttitude.z);
  D3DXMatrixTranspose(&m_kBankRotationMatrix, &m_kLocalToBodyZMatrix);
  D3DXMatrixTranspose(&m_kYawRotationMatrix,  &m_kLocalToBodyYMatrix);

  D3DXMATRIXA16 kLocalToBodyXZTransformMatrix = m_kLocalToBodyXMatrix*m_kLocalToBodyZMatrix;
  // m_kLocalToBodyTransformMatrix
  m_kLocalToBodyTransformMatrix   = m_kLocalToBodyYMatrix*kLocalToBodyXZTransformMatrix;
  // m_kBodyToLocalTransformMatrix
  D3DXMatrixTranspose(&m_kBodyToLocalTransformMatrix, &m_kLocalToBodyTransformMatrix);
  // m_kBodyToTangentialTransformMatrix
  D3DXMatrixTranspose(&m_kBodyToTangentialTransformMatrix, &kLocalToBodyXZTransformMatrix);

  // m_kBodyVelocity
  if((pkSIM != NULL) && pkSIM->IsValid())
  {
    m_kBodyVelocity  = pkSIM->m_kBodyVelocity;
  }
  else
  {
    m_kBodyVelocity      = AAVVector3<FLOAT>((FLOAT)aircraft_varget( m_kDataEnums.m_kBodyVelocityEnum.x, m_idFeetPerSecondEnum, 0),
                                              (FLOAT)aircraft_varget( m_kDataEnums.m_kBodyVelocityEnum.y, m_idFeetPerSecondEnum, 0),
                                              (FLOAT)aircraft_varget( m_kDataEnums.m_kBodyVelocityEnum.z, m_idFeetPerSecondEnum, 0));
  }

  // m_dVelocity (feet per second)
  m_dVelocity = m_kBodyVelocity.Length();

  // Constrain body velocity to UNIT vector when small.
  if(m_dVelocity < 0.2f)
  {
    m_kBodyVelocity = 0.2f*UNIT_VECTOR;
    m_dVelocity     = 0.2f;
  }

  // m_kLocalVelocity
  D3DXVec3TransformCoord((D3DXVECTOR3 *)&m_kLocalVelocity, (D3DXVECTOR3 *)&m_kBodyVelocity, &m_kBodyToLocalTransformMatrix);
  m_kLocalVelocity.x = -m_kLocalVelocity.x;

  // m_kBodyVelocityNormalized
  m_kBodyVelocityNormalized = m_kBodyVelocity/m_dVelocity;
  // m_kLocalVelocityNormalized
  m_kLocalVelocityNormalized = m_kLocalVelocity/m_dVelocity;
}


VOID AC::Start()
{
  GetDataEnums();

  m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, AC::REQUEST_ID_READ_LATLONALT_DATA,        AC::DATA_ID_READ_LATLONALT_DATA,        SIMCONNECT_SIMOBJECT_TYPE_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
  m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, AC::REQUEST_ID_READ_OBJECT_STATE_DATA,     AC::DATA_ID_READ_OBJECT_STATE_DATA,     SIMCONNECT_SIMOBJECT_TYPE_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
  //m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, AC::REQUEST_ID_READ_OBJECT_STATE_DATA,   AC::DATA_ID_READ_OBJECT_STATE_DATA,     SIMCONNECT_SIMOBJECT_TYPE_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
  m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, AC::REQUEST_ID_READ_OBJECT_NAV_RADIO_DATA, AC::DATA_ID_READ_OBJECT_NAV_RADIO_DATA, SIMCONNECT_SIMOBJECT_TYPE_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
  m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, AC::REQUEST_ID_READ_OBJECT_COM_RADIO_DATA, AC::DATA_ID_READ_OBJECT_COM_RADIO_DATA, SIMCONNECT_SIMOBJECT_TYPE_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
  AAV_ASSERT(SUCCEEDED(m_hResult));

  __SVSPModule::Start();
}

VOID AC::Process()
{
  ///////////////////////////////////////////////////////
  // PROCESS_UPDATE_AIRCRAFT_LOCATION_DATA
  ///////////////////////////////////////////////////////
  if((m_dwProcessUpdateMask & AC::PROCESS_UPDATE_AIRCRAFT_LOCATION_DATA) == AC::PROCESS_UPDATE_AIRCRAFT_LOCATION_DATA)
  {
    //m_kGlobalToLocalTransformMatrix
    FLOAT fACLat      = (FLOAT)(m_kLocation.GetLatitudeAngle());
    FLOAT fACLon      = (FLOAT)(m_kLocation.GetLongitudeAngle() - D3DX_PI/2.0);
    FLOAT fCos_fACLat = cos(fACLat);
    FLOAT fSin_fACLat = sin(fACLat);
    FLOAT fCos_fACLon = cos(fACLon);
    FLOAT fSin_fACLon = sin(fACLon);
    m_kGlobalToLocalTransformMatrix._11 =  fCos_fACLon;             m_kGlobalToLocalTransformMatrix._21 =  fSin_fACLon;             m_kGlobalToLocalTransformMatrix._31 =  0;           m_kGlobalToLocalTransformMatrix._41 =  0;
    m_kGlobalToLocalTransformMatrix._12 = -fCos_fACLat*fSin_fACLon; m_kGlobalToLocalTransformMatrix._22 =  fCos_fACLat*fCos_fACLon; m_kGlobalToLocalTransformMatrix._32 =  fSin_fACLat; m_kGlobalToLocalTransformMatrix._42 =  0;
    m_kGlobalToLocalTransformMatrix._13 =  fSin_fACLat*fSin_fACLon; m_kGlobalToLocalTransformMatrix._23 = -fSin_fACLat*fCos_fACLon; m_kGlobalToLocalTransformMatrix._33 =  fCos_fACLat; m_kGlobalToLocalTransformMatrix._43 =  0;
    m_kGlobalToLocalTransformMatrix._14 =  0;                       m_kGlobalToLocalTransformMatrix._24 =  0;                       m_kGlobalToLocalTransformMatrix._34 =  0;           m_kGlobalToLocalTransformMatrix._44 =  1;

    //m_kLocalToGlobalTransformMatrix
    D3DXMatrixTranspose(&m_kLocalToGlobalTransformMatrix, &m_kGlobalToLocalTransformMatrix);

    m_kPreviousVector                 = m_kVector;
    m_kVector                         = m_kLocation.GetVector3<FLOAT>();
    m_kGlobalVector                   = (D3DXVECTOR3)m_kVector;
    m_kGlobalVelocityVectorNormalized = (m_kVector - m_kPreviousVector).Normalize();

    m_dwProcessUpdateMask &= ~AC::PROCESS_UPDATE_AIRCRAFT_LOCATION_DATA;
  }

  ///////////////////////////////////////////////////////
  // PROCESS_UPDATE_AIRCRAFT_STATE_DATA
  ///////////////////////////////////////////////////////
  if((m_dwProcessUpdateMask & AC::PROCESS_UPDATE_AIRCRAFT_STATE_DATA) == AC::PROCESS_UPDATE_AIRCRAFT_STATE_DATA)
  {
    m_fIncidenceAlpha                 = 180.0f*(m_kData.m_dIncidenceAlpha/D3DX_PI);
    m_fAoA                            = 180.0f*(1.0f - m_kData.m_dIndicatedAoA/D3DX_PI);
    m_dFuelTotalQuantityWeightKg      = m_kData.m_dFuelTotalQuantityWeightPounds*0.4536f;
    m_dMass                           = m_kData.m_dTotalWeight/32.2f;
    // m_dRadarAltitude
    FLOAT dPreviousRadarAltitude = m_dRadarAltitude;
    m_dRadarAltitude        = (FLOAT)aircraft_varget( m_kDataEnums.m_dRadarAltitudeEnum, get_units_enum("Feet"), 0);
    // m_dRadarVerticalVelocity
    AAV_ASSERT_RETURN_VOID(m_dDeltaTime > 0);
    m_dRadarVerticalVelocity = (m_dRadarAltitude - dPreviousRadarAltitude)/(FLOAT)m_dDeltaTime;

    m_dwProcessUpdateMask &= ~AC::PROCESS_UPDATE_AIRCRAFT_STATE_DATA;
  }

  ///////////////////////////////////////////////////////
  // PROCESS_UPDATE_AIRCRAFT_NAV_RADIO_DATA
  ///////////////////////////////////////////////////////
  if((m_dwProcessUpdateMask & AC::PROCESS_UPDATE_AIRCRAFT_NAV_RADIO_DATA) == AC::PROCESS_UPDATE_AIRCRAFT_NAV_RADIO_DATA)
  {
    //CARRIER ACLS Directors
    if(!m_bACLSOn)
    {
      m_bNavHasLocalizer  = m_kNavRadioData.m_bNavHasLocalizer;
      m_bNavHasGlideslope = m_kNavRadioData.m_bNavHasGlideslope;
      m_kILSErrorVector   = m_kNavRadioData.m_kILSErrorVector;
    }

    m_dwProcessUpdateMask &= ~AC::PROCESS_UPDATE_AIRCRAFT_NAV_RADIO_DATA;
  }

  ///////////////////////////////////////////////////////
  // PROCESS_UPDATE_AIRCRAFT_COM_RADIO_DATA
  ///////////////////////////////////////////////////////
  if((m_dwProcessUpdateMask & AC::PROCESS_UPDATE_AIRCRAFT_COM_RADIO_DATA) == AC::PROCESS_UPDATE_AIRCRAFT_COM_RADIO_DATA)
  {
    DWORD dFrequency;

    dFrequency = m_kComRadioData.m_dComActiveFrequency[0];
    sprintf_s(&m_acComActiveFrequency[0][0],  sizeof(m_acComActiveFrequency[0]),  "1%02X.%02X", (dFrequency >> 8) & 0xff, dFrequency & 0xff);

    dFrequency = m_kComRadioData.m_dComStandbyFrequency[0];
    sprintf_s(&m_acComStandbyFrequency[0][0], sizeof(m_acComStandbyFrequency[0]), "1%02X.%02X", (dFrequency >> 8) & 0xff, dFrequency & 0xff);

    dFrequency = m_kComRadioData.m_dComActiveFrequency[1];
    sprintf_s(&m_acComActiveFrequency[1][0],  sizeof(m_acComActiveFrequency[1]),  "1%02X.%02X", (dFrequency >> 8) & 0xff, dFrequency & 0xff);

    dFrequency = m_kComRadioData.m_dComStandbyFrequency[1];
    sprintf_s(&m_acComStandbyFrequency[1][0], sizeof(m_acComStandbyFrequency[1]), "1%02X.%02X", (dFrequency >> 8) & 0xff, dFrequency & 0xff);

    m_dwProcessUpdateMask &= ~AC::PROCESS_UPDATE_AIRCRAFT_COM_RADIO_DATA;
  }
}

