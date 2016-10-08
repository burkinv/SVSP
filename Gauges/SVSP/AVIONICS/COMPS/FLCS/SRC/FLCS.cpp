#include "__SVSPAvionics.h"
#include "AC.h"
#include "FLCS.h"
#include "ECS.h"
#include "HOTAS.h"
#include "WPN.h"
#include "SIM.h"

const FLOAT FLCS::MINIMUM_SPEED                          = 110.0f;
const FLOAT FLCS::MAXIMUM_SPEED                          = 930.0f;
const FLOAT FLCS::MAXIMUM_MACH                           = 2.3f;
const FLOAT FLCS::BINGO_ALTITUDE                         = 300;
const FLOAT FLCS::MAXIMUM_G                              = 8.0f;
const FLOAT FLCS::MAXIMUM_ANGLE_OF_ATTACK                = 27.0f; //DEGREES
const FLOAT FLCS::CRITICAL_ANGLE_OF_ATTACK               = 55.0f; //DEGREES
const FLOAT FLCS::MAXIMUM_ANGLE_OF_ATTACK_RADIAN         = 0.47f; //RADIAN
const FLOAT FLCS::LEF_DEPLOYMENT_ANGLE_OF_ATTACK         = 10.0f; //DEGRESS
const FLOAT FLCS::GEAR_DOWN_WARNING_SPEED                = 270.0f;
const FLOAT FLCS::GEAR_UP_WARNING_SPEED                  = 200.0f;
const FLOAT FLCS::MINIMAL_LANDING_PITCH_ANGLE            = -0.0873f;//RADIAN (5  Degrees Up)
const FLOAT FLCS::MAXIMAL_LANDING_PITCH_ANGLE            = -0.3840f;//RADIAN (22 Degrees Up)
const FLOAT FLCS::MAXIMAL_ELEVATOR_TRIM_DEFLECTION_ANGLE =  0.1745f;//RADIAN (10 Degrees)
const ULONG FLCS::OVERRIDE_PITCH_INPUT                   = 0x1;
const ULONG FLCS::OVERRIDE_ROLL_INPUT                    = 0x2;
const ULONG FLCS::OVERRIDE_YAW_INPUT                     = 0x4;
const ULONG FLCS::OVERRIDE_PITCH_TRIM_INPUT              = 0x20;
const ULONG FLCS::OVERRIDE_FLAPS_OPERATION               = 0x1;
const ULONG FLCS::OVERRIDE_SPOILER_OPERATION             = 0x2;
const ULONG FLCS::VV_HOLD_Y_AREA                         = 1000;
const ULONG FLCS::MANUALL_PITCH_OVERRIDE_BUTTON          = 3;
const FLOAT FLCS::MANUALL_PITCH_OVERRIDE_MAXIMIUM_SPEED  = 350;
const FLOAT FLCS::EFFECTIVE_STICK_RANGE_X                = 32767;
const FLOAT FLCS::EFFECTIVE_STICK_RANGE_Y                = 32767;
const FLOAT FLCS::MAX_HOVER_ACTIVATION_AIRSPEED          = 220;
const FLOAT FLCS::MAX_HOVER_ACTIVATION_BANK_ANGLE        = 0.96f;
const FLOAT FLCS::MAX_HOVER_ACTIVATION_RPM_PERCENT_N1    = 55.0f;


const AAVVector3<FLOAT> FLCS::c_fA = AAVVector3<FLOAT>(0.8f, 0.9f, 0.8f);

const AAVVector3<FLOAT> FLCS::c_akLimiterCtrlPointVectorTable[FLCS::MAX_NUM_LIMITER_CTRL_POINTS] = 
{
   /*    0 -  100 */  AAVVector3<FLOAT>(0.00f, 0.00f, 0.00f),
   /*  100 -  200 */  AAVVector3<FLOAT>(0.00f, 0.00f, 0.00f),
   /*  200 -  300 */  AAVVector3<FLOAT>(0.00f, 0.06f, 0.01f),
   /*  300 -  400 */  AAVVector3<FLOAT>(0.00f, 0.13f, 0.01f),
   /*  400 -  500 */  AAVVector3<FLOAT>(0.00f, 0.18f, 0.02f),
   /*  500 -  600 */  AAVVector3<FLOAT>(0.00f, 0.23f, 0.02f),
   /*  600 -  700 */  AAVVector3<FLOAT>(0.00f, 0.24f, 0.03f),
   /*  700 -  800 */  AAVVector3<FLOAT>(0.00f, 0.25f, 0.03f),
   /*  800 -  900 */  AAVVector3<FLOAT>(0.00f, 0.28f, 0.04f),
   /*  900 - 1000 */  AAVVector3<FLOAT>(0.00f, 0.29f, 0.04f),
   /* 1000 - 1100 */  AAVVector3<FLOAT>(0.00f, 0.32f, 0.05f),
   /* 1100 - 1200 */  AAVVector3<FLOAT>(0.00f, 0.35f, 0.05f),
   /* 1200 - 1300 */  AAVVector3<FLOAT>(0.00f, 0.40f, 0.06f),
   /* 1300 - 1400 */  AAVVector3<FLOAT>(0.00f, 0.42f, 0.06f),
   /* 1400 - 1500 */  AAVVector3<FLOAT>(0.00f, 0.46f, 0.07f),
   /* 1500 - 1600 */  AAVVector3<FLOAT>(0.00f, 0.49f, 0.08f),
   /* 1700 - 1800 */  AAVVector3<FLOAT>(0.00f, 0.52f, 0.08f),
   /* 1800 - 1900 */  AAVVector3<FLOAT>(0.00f, 0.42f, 0.08f),
   /* 2000 - 2100 */  AAVVector3<FLOAT>(0.00f, 0.43f, 0.08f),
   /* 2100 - 2200 */  AAVVector3<FLOAT>(0.00f, 0.43f, 0.09f),
   /* 2200 - 2300 */  AAVVector3<FLOAT>(0.00f, 0.43f, 0.09f),
   /* 2400 - 2500 */  AAVVector3<FLOAT>(0.00f, 0.43f, 0.10f),
   /* 2500 - 2600 */  AAVVector3<FLOAT>(0.00f, 0.43f, 0.10f),
   /* 2600 - 2700 */  AAVVector3<FLOAT>(0.00f, 0.43f, 0.10f),
   /* 2700 - 2800 */  AAVVector3<FLOAT>(0.00f, 0.44f, 0.10f),
   /* 2800 - 2900 */  AAVVector3<FLOAT>(0.00f, 0.44f, 0.11f),
   /* 2900 - 3000 */  AAVVector3<FLOAT>(0.00f, 0.44f, 0.11f),
   /* 3000 - 3100 */  AAVVector3<FLOAT>(0.00f, 0.44f, 0.11f),
   /* 3100 - 3200 */  AAVVector3<FLOAT>(0.00f, 0.44f, 0.12f),
   /* 3200 - 3300 */  AAVVector3<FLOAT>(0.00f, 0.45f, 0.12f),
};

FLCS::FLCS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("FLCS", pkAvionics, dwThreadSleepTime, true, NULL, NULL, NULL)
{
  m_bAutomaticFlapsControl = true;

  memset(&m_kAileronControl,      0, sizeof(AILERON_CONTROL));
  memset(&m_kElevatorControl,     0, sizeof(ELEVATOR_CONTROL));
  memset(&m_kRudderControl,       0, sizeof(RUDDER_CONTROL));
  memset(&m_kSpoilerControl,      0, sizeof(SPOILER_CONTROL));
  memset(&m_kThrottleControl,     0, sizeof(THROTTLE_CONTROL));
  memset(&m_kLEFControl,          0, sizeof(LEAD_FLAPS_CONTROL));
  memset(&m_kElevatorTrimControl, 0, sizeof(ELEVATOR_TRIM_CONTROL));


  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_AILERON,       "YOKE X POSITION",                       "position",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_AILERON,       "AILERON POSITION",                      "position",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_ELEVATOR,      "YOKE Y POSITION",                       "position",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_ELEVATOR,      "ELEVATOR POSITION",                     "position",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_RUDDER,        "RUDDER PEDAL POSITION",                 "position",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_RUDDER,        "RUDDER POSITION",                       "position",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_SPOILER,       "SPOILERS HANDLE POSITION",              "position",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_THROTTLE,      "GENERAL ENG THROTTLE LEVER POSITION:1", "Percent",          SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_THROTTLE,      "GENERAL ENG THROTTLE LEVER POSITION:2", "Percent",          SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_FLAPS,         "FLAPS HANDLE INDEX",                    "Number",           SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_LEAD_FLAPS,    "LEADING EDGE FLAPS LEFT PERCENT",       "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_LEAD_FLAPS,    "LEADING EDGE FLAPS RIGHT PERCENT",      "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_TRAIL_FLAPS,   "TRAILING EDGE FLAPS LEFT PERCENT",      "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_TRAIL_FLAPS,   "TRAILING EDGE FLAPS RIGHT PERCENT",     "Percent Over 100", SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, FLCS::CONTROL_DATA_ID_ELEVATOR_TRIM, "ELEVATOR TRIM POSITION",                "Radians",          SIMCONNECT_DATATYPE_FLOAT32);

  m_apkProcessStateCallback[FLCS::FLCS_STATE_SLEW]                = &FLCS::ProcessStateSlew;
  m_apkProcessStateCallback[FLCS::FLCS_STATE_HOVER]               = &FLCS::ProcessStateHover;
  m_apkProcessStateCallback[FLCS::FLCS_STATE_ON_GROUND]           = &FLCS::ProcessStateOnGround;
  m_apkProcessStateCallback[FLCS::FLCS_STATE_AIRBORNE_GEAR_DOWN]  = &FLCS::ProcessStateAirborneGearDown;
  m_apkProcessStateCallback[FLCS::FLCS_STATE_AIRBORNE]            = &FLCS::ProcessStateAirborne;

  m_eState = FLCS::FLCS_STATE_ON_GROUND;
}

FLCS::~FLCS()
{
  __SVSPModule::Stop();
}


const SHORT  STICK_INPUT_AXIS_X_MAX             = 16300;
const FLOAT  G_LIM_V_MIN                        = 270.0f;
const FLOAT  G_LIM_V_MAX                        = 2270.0f;
const FLOAT  G_LIM_B                            = 0.0f;

const USHORT MPO_REJECT_SPEED                   = 470;

const FLOAT  FIRST_STAGE_LEF_DEPLOYMENT_ALPHA   = 12.0;
const FLOAT  SECOND_STAGE_LEF_DEPLOYMENT_ALPHA  = 19.0;
const FLOAT  THIRD_STAGE_LEF_DEPLOYMENT_ALPHA   = 26.0;
const UINT   FIRST_STAGE_FLAP_DEPLOYMENT_SPEED  = 250;
const UINT   SECOND_STAGE_FLAP_DEPLOYMENT_SPEED = 210;
const UINT   THIRD_STAGE_FLAP_DEPLOYMENT_SPEED  = 170;

const FLOAT  ELEVATOR_EFFECTIVENESS             = 2.0f;
const FLOAT  AILERON_EFFECTIVENESS              = 1.0f;

static FLOAT dMAX_N2                            = 80.0;
static UINT  RUDDER_DEFLECTION_INCREMENT        = 3000;

#define FCS_BYPASS_LIMITER

VOID FLCS::ProcessSteerpointNavigationControl()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  WPN *pkWPN  = static_cast<WPN *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_WPN));

  if(pkWPN != NULL)
  {
    //const D3DXVECTOR3 *pkBodySteepointOffsetVectorNormalized = pkWPN->GetBodySteerpointOffsetVectorNormalized();
    ////const D3DXVECTOR3 *pkBodyVelocityVectorNormalized        = static_cast<const D3DXVECTOR3 *>(&pkAC->m_kBodyVelocityNormalized);

    //D3DXVECTOR3 kCrossProductVector = *D3DXVec3Cross(&kCrossProductVector, kBodyVelocityVectorNormalized, pkBodySteepointOffsetVectorNormalized);
    //FLOAT fSin = D3DXVec3Length(&kCrossProductVector);

    //if(pkBodySteepointOffsetVector != NULL)
    //{
    //  m_kYokeIn = AAVVector3<FLOAT>(/*Elevator*/__SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(m_kYokeIn.x, 0, 1.4f),
    //                                 /*Rudder  */                                         m_kYokeIn.y,
    //                                 /*Aileron */__SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(m_kYokeIn.z, 0, 1.4f));
    //}

  }
}

VOID FLCS::ProcessAileronInput()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  //FLOAT fBVx = K*B.z*( (pkAC->m_kData.m_dIndicatedAirspeed - C.z)/sqrt(1 + pow(B.z*(pkAC->m_kData.m_dIndicatedAirspeed - C.z), 2)) + C.z/sqrt(1 + pow(B.z*C.z, 2)) );
  //FLOAT fBVx = pkAC->m_kData.m_dIndicatedAirspeed/(pkAC->m_kData.m_dIndicatedAirspeed + 7200.0);
  //FLOAT fBVx = ((2*atan(pkAC->m_kData.m_dIndicatedAirspeed/600 - 3))/D3DX_PI + 0.8)/1.85;
  FLOAT X    = (1.0f - m_kBV.z)*m_kYokeIn.z;
  m_kYokeOut.z  = (m_bBypassLimiter)? m_kYokeIn.z : c_fA.z*X*X*X + (1.0f - c_fA.z)*X;
  if(m_kAileronControl.dYokeZ != m_kYokeOut.z)
  {
    m_kAileronControl.dYokeZ  = m_kAileronControl.dOutput = m_kYokeOut.z;
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_AILERON, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(AILERON_CONTROL), &m_kAileronControl);

    //Save bank reference value.
    //m_kReferenceAttitudeVector.z = pkAC->m_kBodyAttitude.z;
  }
}

VOID FLCS::ProcessElevatorInput()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  ECS  *pkECS  = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));

  //FLOAT fBVy = K*B.x*( (pkAC->m_kData.m_dIndicatedAirspeed - C.x)/sqrt(1 + pow(B.x*(pkAC->m_kData.m_dIndicatedAirspeed - C.x), 2)) + C.x/sqrt(1 + pow(B.x*C.x, 2)) );
  //fBGy += (FLCS::MAXIMUM_G*m_kYokeIn.x - pkAC->m_kData.m_dG)*1000;
  //fBVy = pkAC->m_kData.m_dIndicatedAirspeed/(pkAC->m_kData.m_dIndicatedAirspeed + 1600.0 + fBGy);
  //fBVy = ((2*atan(pkAC->m_kData.m_dIndicatedAirspeed/200 - 3))/D3DX_PI + 0.8)/1.85;

  //FLOAT Y    = (1.0f - m_kBV.x)*m_kYokeIn.x;
  //m_kYokeOut.x  = (m_bBypassLimiter)? m_kYokeIn.x : fHAy*Y*Y*Y + (1.0f - c_fA.x)*Y;

  FLOAT FLCS_fC_G   = 0.12f;
  FLOAT FLCS_fC_AoA = 0.015f;

  m_fB.x  = (1.0f / (pow(FLCS_fC_G * pkAC->m_kData.m_dG, 2.0f) + 1.0f))* (1.0f / (pow(FLCS_fC_AoA * pkAC->m_fAoA, 2.0f) + 1.0f));
  FLOAT Y    = m_kYokeIn.x;
  m_kYokeOut.x = (m_bBypassLimiter)? m_kYokeIn.x : m_fB.x*(c_fA.x*pow(Y, 7.0f) + (1.0f - c_fA.x)*Y);

  if(m_kElevatorControl.dYokeX != m_kYokeOut.x)
  {
    m_kElevatorControl.dYokeX  = m_kElevatorControl.dOutput = m_kYokeOut.x;
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_ELEVATOR, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(ELEVATOR_CONTROL), &m_kElevatorControl);

    //Save pitch reference value.
    m_kReferenceAttitudeVector.x = pkAC->m_kBodyVelocityNormalized.x;

    //Set thrust vector for pitch assistance (if no hover mode initiated).
    if((pkECS != NULL) && (pkECS->GetGainThrustVectorValue() != ECS::THRUST_VECTOR_VALUE_FULL_DOWN))
    {
      FLOAT fGainThrustVectorValue = (1.0f - m_kYokeOut.x)*ECS::THRUST_VECTOR_VALUE_LEVEL_OFF;
      pkECS->SetGainThrustVectorValue(fGainThrustVectorValue);
    }
  }
}

VOID FLCS::ProcessRudderInput()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  //FLOAT fBVz = K*B.y*( (pkAC->m_kData.m_dIndicatedAirspeed - C.y)/sqrt(1 + pow(B.y*(pkAC->m_kData.m_dIndicatedAirspeed - C.y), 2)) + C.y/sqrt(1 + pow(B.y*C.y, 2)) );
  //FLOAT fBVz = pkAC->m_kData.m_dIndicatedAirspeed/(pkAC->m_kData.m_dIndicatedAirspeed + 1600.0);
  //FLOAT fBVy = ((2*atan(pkAC->m_kData.m_dIndicatedAirspeed/200 - 3))/D3DX_PI + 0.8)/1.85;
  FLOAT fBGz = 0.0; 
  FLOAT Z    = (1.0f - m_kBV.y)*m_kYokeIn.y;
  m_kYokeOut.y  = (m_bBypassLimiter)? m_kYokeIn.y : c_fA.y*Z*Z*Z + (1.0f - c_fA.y)*Z;
  if(m_kRudderControl.dYokeY != m_kYokeOut.y)
  {
    m_kRudderControl.dYokeY  = m_kRudderControl.dOutput = m_kYokeOut.y;
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_RUDDER, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(RUDDER_CONTROL), &m_kRudderControl);

    //Save yaw reference value.
    m_kReferenceAttitudeVector.x = 0;
  }
}

VOID FLCS::ProcessThrottleInput()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  ECS   *pkECS  = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));

  //Keep throttle level off the afterburner activation zone (Defined in Aircraft.cfg::428) in hove mode.
  m_dThrottleIn = (((pkECS != NULL) && (pkECS->GetThrustVectorValue() > ECS::THRUST_VECTOR_VALUE_PITCH_DOWN)) && (m_dThrottleIn > 84.9f)) ? 84.9f : m_dThrottleIn;

  if(m_kThrottleControl.dOutput[0] != m_dThrottleIn)
  {
    m_kThrottleControl.dOutput[0] = m_dThrottleIn;
    m_kThrottleControl.dOutput[1] = m_dThrottleIn;
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(THROTTLE_CONTROL), &m_kThrottleControl);
  }
}

VOID FLCS::ProcessThrottleInput_HOVER()
{
  ProcessThrottleInput();
}

VOID FLCS::ProcessSpoilerControl()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  HOTAS *pkHOTAS = static_cast<HOTAS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HOTAS));
  AAV_ASSERT_RETURN_VOID(pkHOTAS  != NULL);

//TODO: Move to global HOTAS config
  FLOAT fSpoilerIncrement = 0.02f*(FLOAT)((pkHOTAS->m_kJS.rgbButtons[27] >> 7) - (pkHOTAS->m_kJS.rgbButtons[26] >> 7));
  if(fSpoilerIncrement != 0)
  {
    m_dSpoilerHandlePosition += fSpoilerIncrement;
    if((m_dSpoilerHandlePosition < 1) && (m_dSpoilerHandlePosition >= 0))
    {
      m_kSpoilerControl.dOutput = m_dSpoilerHandlePosition;
      m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_SPOILER, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(SPOILER_CONTROL), &m_kSpoilerControl);
    }
    else
    {
      m_dSpoilerHandlePosition = (m_dSpoilerHandlePosition > 1) ? 1 : ((m_dSpoilerHandlePosition < 0) ? 0 : m_dSpoilerHandlePosition);
    }
  }
}

VOID FLCS::ProcessFlapsControl()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  if(m_bAutomaticFlapsControl)
  {
    if(m_eState == FLCS::FLCS_STATE_HOVER)
    {
      m_dFlapsHandlePosition = 10.0;

      //m_kLeadingEdgeFlapControl.dLeft  = 1.0;
      //m_kLeadingEdgeFlapControl.dRight = 1.0;
      //m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_LEAD_FLAPS, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(FLCS::LEAD_FLAPS_CONTROL), &m_kLeadingEdgeFlapControl);

      //m_kTrailingEdgeFlapControl.dLeft  = 1.0;
      //m_kTrailingEdgeFlapControl.dRight = 1.0;
      //m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_TRAIL_FLAPS, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(FLCS::TRAIL_FLAPS_CONTROL), &m_kTrailingEdgeFlapControl);
    }
    else if(pkAC->m_fAoA > 9.0f)
    {
      //HACK: Calculate flaps handle position as a function of AoA
      m_dFlapsHandlePosition = pkAC->m_fAoA/3.0f;
      if(m_dFlapsHandlePosition > 10.0f)
      {
        m_dFlapsHandlePosition = 10.0f;
      }
    }
    else
    {
      m_dFlapsHandlePosition = 0.0;
    }

    if(pkAC->m_kData.m_dFlapsHandlePosition != m_dFlapsHandlePosition)
    {
      m_kFlapsControl.dFlapsHandlePosition  = m_dFlapsHandlePosition;
      m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_FLAPS, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(FLCS::FLAPS_CONTROL), &m_kFlapsControl);
    }
  }
}

VOID FLCS::ProcessVectorThrustControl()
{
  AC   *pkAC     = static_cast<AC    *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  ECS   *pkECS   = static_cast<ECS   *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  HOTAS *pkHOTAS = static_cast<HOTAS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HOTAS));

  //TODO: Move to global HOTAS config.
  if((pkECS != NULL) && (pkHOTAS != NULL))
  {
    if( ((pkHOTAS->m_kJS.rgbButtons[0] & 0x80) == 0x80) && ((pkHOTAS->m_kJS.rgbButtons[3] & 0x80) == 0x80) )
    {
      if((pkHOTAS->m_kJS.rgbButtons[14] & 0x80) == 0x80)
      {
        //Check list to activate HOVER mode.
        FLCS_ASSERT_RETURN_VOID(pkAC->m_kData.m_dIndicatedAirspeed  < FLCS::MAX_HOVER_ACTIVATION_AIRSPEED);
        FLCS_ASSERT_RETURN_VOID(abs(pkAC->m_kBodyAttitude.z)        < FLCS::MAX_HOVER_ACTIVATION_BANK_ANGLE);
        FLCS_ASSERT_RETURN_VOID(pkECS->m_akEngine[0].fN[0]          < FLCS::MAX_HOVER_ACTIVATION_RPM_PERCENT_N1);
        FLCS_ASSERT_RETURN_VOID(!pkAC->m_kData.m_bOnGround || (pkAC->m_kData.m_dBrakeParking != 0));

        pkECS->SetGainThrustVectorValue(ECS::THRUST_VECTOR_VALUE_FULL_DOWN);
        //TODO:HACK
        pkECS->SetGainLiftFanValue(100);
      }

      if((pkHOTAS->m_kJS.rgbButtons[16] & 0x80) == 0x80)
      {
        //Check list to resume from HOVER mode.
        pkECS->SetGainThrustVectorValue(ECS::THRUST_VECTOR_VALUE_LEVEL_OFF);
        //TODO:HACK
        pkECS->SetGainLiftFanValue(0);
      }
    }
  }
}

VOID FLCS::ProcessTorqueControl()
{
  ECS  *pkECS  = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));

  if(pkECS  != NULL)
  {
    pkECS->m_kTorqueControlVector = AAVVector3<FLOAT>(0, 0, 0);
  }
}

VOID FLCS::ProcessTorqueControl_HOVER()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkData  != NULL);
  ECS   *pkECS  = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  AAV_ASSERT_RETURN_VOID(pkECS  != NULL);
  SIM   *pkSIM  = static_cast<SIM *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SIM));
  AAV_ASSERT_RETURN_VOID(pkSIM  != NULL);
  HOTAS *pkHOTAS = static_cast<HOTAS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HOTAS));
  AAV_ASSERT_RETURN_VOID(pkHOTAS  != NULL);
  AAV_ASSERT_RETURN_VOID(pkHOTAS->IsValid() != NULL);
  
  //Input Lift Control Flag.
  FLOAT dInputLiftControlFlag = (FLOAT)((pkHOTAS->m_kJS.rgbButtons[20] >> 7) - (pkHOTAS->m_kJS.rgbButtons[19] >> 7));

  FLOAT FLCS_PITCH_STABILIZATION_FACTOR = 0.11f;
  FLOAT FLCS_YAW_STABILIZATION_FACTOR   = 0.15f;
  FLOAT FLCS_ROLL_STABILIZATION_FACTOR  = 0.15f;

  //Input and Linear Stabilization Control.
  if((pkData->m_dBrakeParking == 0) && (pkData->m_dBrakePosition != 1))
  {
    //NO BRAKE MASK SET: Adjust pitch command to assist in forward movement.
    m_fInputPitchCommand = (((m_kYokeIn.x < 0) && (m_kYokeIn.x < m_fInputPitchCommand)) || (m_kYokeIn.x > 0.25f)) ? m_kYokeIn.x : m_fInputPitchCommand; 

    m_kInputControlVector                               = AAVVector3<FLOAT>(/*Pitch */-0.55f*m_fInputPitchCommand,
                                                                             /*Yaw   */ 0.30f*m_kYokeIn.y,
                                                                             /*Roll  */-0.75f*m_kYokeIn.z);

    //NO BRAKE MASK SET: If no Roll command is given minimize sideslip in forward movement.
    m_kLinearStabilizationControlVector                 = AAVVector3<FLOAT>(/*Pitch */((m_fInputPitchCommand < 0) && (pkSIM->m_kBodyVelocity.z < 0)) ? -__SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(pkSIM->m_kBodyVelocity.z, 0, FLCS_PITCH_STABILIZATION_FACTOR) : 0, 
                                                                             /*Yaw   */ 0, 
                                                                             /*Roll  */(abs(m_kInputControlVector.z) < 0.25f )                        ?  __SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(pkSIM->m_kBodyVelocity.x, 0, FLCS_ROLL_STABILIZATION_FACTOR) : 0);
  }
  else
  {
    //BRAKE MASK SET: Ignore Pitch and Roll input commands.
    m_kInputControlVector                               = AAVVector3<FLOAT>(/*Pitch */-0.55f*m_kYokeIn.x,
                                                                             /*Yaw   */ 0.40f*m_kYokeIn.y,
                                                                             /*Roll  */-0.75f*m_kYokeIn.z);

    //BRAKE MASK SET: Stabilize and hold position.
    m_kLinearStabilizationControlVector                 = AAVVector3<FLOAT>(/*Pitch */-__SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(pkSIM->m_kBodyVelocity.z, 0, FLCS_PITCH_STABILIZATION_FACTOR),
                                                                             /*Yaw   */  0, 
                                                                             /*Roll  */ __SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(pkSIM->m_kBodyVelocity.x, 0, FLCS_ROLL_STABILIZATION_FACTOR));
  }

  //Angular Stabilization control.
  m_kAngularStabilizationControlVector                  = AAVVector3<FLOAT>(/*Pitch */-5.70f*pkAC->m_kBodyAttitude.x - 1.95f*pkData->m_kBodyAngularVelocity.x,
                                                                             /*Yaw   */                               - 0.50f*pkData->m_kBodyAngularVelocity.y,
                                                                             /*Roll  */-3.50f*pkAC->m_kBodyAttitude.z - 1.50f*pkData->m_kBodyAngularVelocity.z);

  //Hold heading vector if Yaw rotaion falls below 2 degrees/s.
  if(abs(pkData->m_kBodyAngularVelocity.y) < 0.0349)
  {
    if(!m_bHeadingSet)
    {
      m_fHeadingToGain = pkData->m_dIndicatedMagneticHeading;
      m_bHeadingSet = true;
    }
  }
  else
  {
    m_bHeadingSet = false;
  }

  FLOAT m_fTrueHeadingError = m_fHeadingToGain - pkData->m_dIndicatedMagneticHeading;

  //Attitude Correction control.
  m_kAttitudeCorrectionControlVector                    = AAVVector3<FLOAT>(/*Pitch */ 0.30f*dInputLiftControlFlag,
                                                                             /*Yaw   */  (m_bHeadingSet) ? __SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(m_fTrueHeadingError, 0, FLCS_YAW_STABILIZATION_FACTOR) : 0,
                                                                             /*Roll  */-0.60f*m_kYokeIn.y);


  pkECS->m_kTorqueControlVector = m_kInputControlVector + m_kLinearStabilizationControlVector + m_kAngularStabilizationControlVector + m_kAttitudeCorrectionControlVector;
}

VOID FLCS::ProcessThrustControl_HOVER()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkData  != NULL);
  ECS   *pkECS  = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  AAV_ASSERT_RETURN_VOID(pkECS  != NULL);
  SIM   *pkSIM  = static_cast<SIM *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SIM));
  AAV_ASSERT_RETURN_VOID(pkSIM  != NULL);
  HOTAS *pkHOTAS = static_cast<HOTAS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HOTAS));
  AAV_ASSERT_RETURN_VOID(pkHOTAS  != NULL);
  AAV_ASSERT_RETURN_VOID(pkHOTAS->IsValid() != NULL);
  
  //Input Lift Control Flag.
  //FLOAT dInputLiftControlFlag = (FLOAT)((pkHOTAS->m_kJS.rgbButtons[2] >> 7) | (pkHOTAS->m_kJS.rgbButtons[3] >> 7))*((pkHOTAS->m_kJS.rgbButtons[6] >> 7) - (pkHOTAS->m_kJS.rgbButtons[8] >> 7));
  FLOAT dInputLiftControlFlag = (FLOAT)((pkHOTAS->m_kJS.rgbButtons[20] >> 7) - (pkHOTAS->m_kJS.rgbButtons[19] >> 7));
  if(dInputLiftControlFlag != 0)
  {
    //Set reference altitude (either Barometric or Radar altitude (LTF)).
    m_dReferenceAltitude = pkData->m_dIndicatedAltitude;
  }

  //Calculate tangential velocity
  m_kTangetialVelocity = pkAC->m_kBodyToTangentialTransformMatrix*pkSIM->m_kBodyVelocity;

  //Adjust engine thrust according to referenced altitude value.
  FLOAT fCurrentThrustCapacityToWeightRatio = pkECS->m_akEngine[0].dCurrentThrustCapacity/pkData->m_dTotalWeight;
  if(fCurrentThrustCapacityToWeightRatio > 1.0f)
  {
    FLOAT fMaxAugmentedThrust = pkECS->m_akEngine[0].dCurrentThrustCapacity - pkData->m_dTotalWeight;

    FLOAT fAltitudeError = pkData->m_dIndicatedAltitude - m_dReferenceAltitude;
    FLOAT fX = -fAltitudeError + 0.7f*pkAC->m_dRadarAltitude*dInputLiftControlFlag - 1.7f*m_kTangetialVelocity.y;
    //Zero vertical accelearation at -fMaxAugmentedThrust.
    pkECS->m_dAugmentedThrust = -fMaxAugmentedThrust + fMaxAugmentedThrust*__SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(fX, 0, 0.5f);
  }
  else
  {
    pkECS->m_dAugmentedThrust = 0;
  }
}

VOID FLCS::ProcessThrustControl()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkData  != NULL);
  ECS  *pkECS  = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));

  if(pkECS  != NULL)
  {
    pkECS->m_dAugmentedThrust = 0;
  }
}

VOID FLCS::ProcessElevatorTrimControl_AIRBORNE()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkData  != NULL);

  AAVVector3<FLOAT> kLocalVelocityError;
  FLOAT              fElevatorTrimDegrees;
  FLOAT              fElevatorTrimDegreesFLCS;

  //PITCH HOLD
  //if((abs(m_kYokeIn.x) > 0.05f))
  //{
  //  m_dReferencePitch = pkAC->m_kBodyAttitude.x;
  //}
  //// Hold Pitch reference within 80 Degrees Bank Angle.
  //else if(abs(pkAC->m_kBodyAttitude.z) < 1.3963) 
  //{
  // FLOAT dReferencePitchDegrees = -180.0f*(m_dReferencePitch/D3DX_PI);
  // FLOAT dPitchError            =  -150.70f*(m_dReferencePitch - pkAC->m_kBodyAttitude.x) - 5.95f*pkData->m_kBodyAngularVelocity.x;
  // dElevatorTrim                =  FLCS::MAXIMAL_ELEVATOR_TRIM_DEFLECTION_ANGLE*__SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(dPitchError, 0, 0.5f);

  //  if(m_kElevatorTrimControl.dOutput != dElevatorTrim)
  //  {
  //    m_kElevatorTrimControl.dOutput = dElevatorTrim;
  //    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_ELEVATOR_TRIM, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(ELEVATOR_TRIM_CONTROL), &m_kElevatorTrimControl);
  //  }
  //}

  // Hold Flight Path reference within +/-80 and beyond +/- 100 Degrees Bank Angle when no pitch command applied.
  FLOAT fBodyAttitudeZAbs = abs(pkAC->m_kBodyAttitude.z);
  if( ((m_kYokeIn.x >  0.05f) && (pkAC->m_kLocalVelocityNormalized.y > m_kReferenceLocalVelocityNormalized.y)) ||
      ((m_kYokeIn.x < -0.05f) && (pkAC->m_kLocalVelocityNormalized.y < m_kReferenceLocalVelocityNormalized.y)) ||
      ((fBodyAttitudeZAbs > 1.3963) && (fBodyAttitudeZAbs < 1.7453)) )
  {
    m_kReferenceLocalVelocityNormalized.y = pkAC->m_kLocalVelocityNormalized.y;
    m_fFPMReferenceAngleDegrees = 180*(asinf(m_kReferenceLocalVelocityNormalized.y)/D3DX_PI);
    fElevatorTrimDegrees        = 180*(pkData->m_dElevatorTrimRadians/D3DX_PI);
    fElevatorTrimDegreesFLCS    = 180*(m_kElevatorTrimControl.dOutput/D3DX_PI);
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_ELEVATOR_TRIM, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(ELEVATOR_TRIM_CONTROL), &m_kElevatorTrimControl);
  }
  else
  {
    kLocalVelocityError.y = pkAC->m_kLocalVelocityNormalized.y - m_kReferenceLocalVelocityNormalized.y;

    FLOAT fY =  40.70f*kLocalVelocityError.y - 2.95f*abs(pkData->m_kBodyAngularVelocity.x);

    m_kElevatorTrimControl.dOutput = sign<FLOAT>(fBodyAttitudeZAbs - D3DX_PI/2)*FLCS::MAXIMAL_ELEVATOR_TRIM_DEFLECTION_ANGLE*__SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(fY, m_kElevatorTrimControl.dOutput, 0.5f);
    fElevatorTrimDegreesFLCS       = 180*(m_kElevatorTrimControl.dOutput/D3DX_PI);
    m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, FLCS::CONTROL_DATA_ID_ELEVATOR_TRIM, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(ELEVATOR_TRIM_CONTROL), &m_kElevatorTrimControl);
  }
}

HRESULT FLCS::ProcessStateSlew()
{
  ProcessSteerpointNavigationControl();
  ProcessAileronInput();
  ProcessElevatorInput();
  ProcessRudderInput();
  ProcessThrottleInput();
  ProcessSpoilerControl();
  ProcessFlapsControl();
  ProcessVectorThrustControl();
  ProcessTorqueControl();
  ProcessThrustControl();

  return S_OK;
}

HRESULT FLCS::ProcessStateHover()
{
  ProcessSteerpointNavigationControl();
  ProcessAileronInput();
  ProcessElevatorInput();
  ProcessRudderInput();
  ProcessThrottleInput_HOVER();
  ProcessSpoilerControl();
  ProcessFlapsControl();
  ProcessVectorThrustControl();
  ProcessTorqueControl_HOVER();
  ProcessThrustControl_HOVER();

  return S_OK;
}

HRESULT FLCS::ProcessStateOnGround()
{
  ProcessSteerpointNavigationControl();
  ProcessAileronInput();
  ProcessElevatorInput();
  ProcessRudderInput();
  ProcessThrottleInput();
  ProcessSpoilerControl();
  ProcessFlapsControl();
  ProcessVectorThrustControl();
  ProcessTorqueControl();
  ProcessThrustControl();

  return S_OK;
}

HRESULT FLCS::ProcessStateAirborneGearDown()
{
  ProcessSteerpointNavigationControl();
  ProcessAileronInput();
  ProcessElevatorInput();
  ProcessRudderInput();
  ProcessThrottleInput();
  ProcessSpoilerControl();
  ProcessFlapsControl();
  ProcessVectorThrustControl();
  ProcessTorqueControl();
  ProcessThrustControl();
  ProcessElevatorTrimControl_AIRBORNE();

  return S_OK;
}

HRESULT FLCS::ProcessStateAirborne()
{
  ProcessSteerpointNavigationControl();
  ProcessAileronInput();
  ProcessElevatorInput();
  ProcessRudderInput();
  ProcessThrottleInput();
  ProcessSpoilerControl();
  ProcessFlapsControl();
  ProcessVectorThrustControl();
  ProcessTorqueControl();
  ProcessThrustControl();
  ProcessElevatorTrimControl_AIRBORNE();

  return S_OK;
}




VOID FLCS::Start()
{
  AC   *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  m_kYokeIn                      = AAVVector3<FLOAT>(0, 0, 0);
  m_kYokeInPrevious              = AAVVector3<FLOAT>(0, 0, 0);
  m_kYokeOut                     = AAVVector3<FLOAT>(0, 0, 0);
  m_dThrottleIn                  = 0;
  m_bBypassLimiter               = false;
  m_dReferenceAltitude           = 0;
  m_dReferencePitch              = 0;
  m_fInputPitchCommand           = 0;
  m_fHeadingToGain               = pkAC->m_kData.m_dIndicatedMagneticHeading;
  m_bHeadingSet                  = true;
  m_bValid                       = true;

  __SVSPModule::Start();
}


VOID FLCS::Process()
{

  AAVVector3<FLOAT> kMinCtrlPointVector;
  AAVVector3<FLOAT> kMaxCtrlPointVector;
  INT                iMinCtrlPointIndex = 0;
  INT                iMaxCtrlPointIndex = 0;

  AC    *pkAC    = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  ECS   *pkECS   = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  //AAV_ASSERT_RETURN_VOID(pkECS  != NULL);
  //SIM  *pkSIM  = static_cast<SIM *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SIM));
  //AAV_ASSERT_RETURN_VOID(pkSIM  != NULL);
  HOTAS *pkHOTAS = static_cast<HOTAS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HOTAS));

  if(m_pkAvionics->IsValid() && (pkHOTAS != NULL) && pkHOTAS->IsValid() && m_bValid)
  {
    m_kYokeIn   = AAVVector3<FLOAT>(/*Pitch*/      ((FLOAT)(pkHOTAS->m_kJS.lY - 32767L))/32767.0f,
                                     /*Yaw  */2.0f*((FLOAT)pkHOTAS->m_kJS.lRz)/65536.0f - 1,
                                     /*Roll */     ((FLOAT)(pkHOTAS->m_kJS.lX - 32767L))/32767.0f);
  //m_kYokeIn   = AAVVector3<FLOAT>(/*Pitch*/ __SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(      (FLOAT)pkHOTAS->m_kJS.lY, 32767.0f, 2.5f*(1/EFFECTIVE_STICK_RANGE_Y))
  //                                 /*Yaw  */                                         2.0f*((FLOAT)pkHOTAS->m_kJS.lRz)/65536.0f - 1,
  //                                 /*Roll */ __SVSPMath::SIGMOID_DISTRIBUTION_FUNCTION(     (FLOAT)pkHOTAS->m_kJS.lX, 32767.0f, 2.5f*(1/EFFECTIVE_STICK_RANGE_X))),
    m_dThrottleIn = 100.0f*(1 - ((FLOAT)pkHOTAS->m_kJS.lZ)/65536.0f);

    FLOAT K = 0.5;
    AAVVector3<FLOAT> B = AAVVector3<FLOAT>(0.0025f, 0.0018f, 0.0025f);
    AAVVector3<FLOAT> C = AAVVector3<FLOAT>(1100.0f, 550.0f, 1700.0f);

    FLOAT dT             = (FLOAT)((UINT)(pkAC->m_kData.m_dIndicatedAirspeed)%100)/100.0f;
    iMinCtrlPointIndex   = (INT)floor((pkAC->m_kData.m_dIndicatedAirspeed)/100.0f);
    iMinCtrlPointIndex   = (iMinCtrlPointIndex > 0) ? ((iMinCtrlPointIndex < (FLCS::MAX_NUM_LIMITER_CTRL_POINTS - 1)) ? iMinCtrlPointIndex : FLCS::MAX_NUM_LIMITER_CTRL_POINTS - 2) : 0;
    iMaxCtrlPointIndex   = iMinCtrlPointIndex + 1;

    kMinCtrlPointVector  = c_akLimiterCtrlPointVectorTable[iMinCtrlPointIndex];
    kMaxCtrlPointVector  = c_akLimiterCtrlPointVectorTable[iMaxCtrlPointIndex];
    m_kBV                = (kMaxCtrlPointVector - kMinCtrlPointVector)*dT + kMinCtrlPointVector;

    m_bBypassLimiter     = (((pkHOTAS->m_kJS.rgbButtons[FLCS::MANUALL_PITCH_OVERRIDE_BUTTON] & 0x80) == 0x80) && (pkAC->m_kData.m_dIndicatedAirspeed < FLCS::MANUALL_PITCH_OVERRIDE_MAXIMIUM_SPEED));


    if(m_pkAvionics->IsSlewActive())
    {
      m_eState = FLCS::FLCS_STATE_SLEW;
    }
    else if( (pkECS != NULL) && ((pkECS->GetThrustVectorValue() > ECS::THRUST_VECTOR_VALUE_PITCH_DOWN) && (pkECS->m_akEngine[0].fN[1] > 85)) )
    {
      m_eState = FLCS::FLCS_STATE_HOVER;
    }
    else if(pkAC->m_kData.m_bOnGround)
    {
      m_eState = FLCS::FLCS_STATE_ON_GROUND;
    }
    else if(pkAC->m_kData.m_dGearPosition == 1)
    {
      m_eState = FLCS::FLCS_STATE_AIRBORNE_GEAR_DOWN;
    }
    else
    {
      m_eState = FLCS::FLCS_STATE_AIRBORNE;
    }


    AAV_ASSERT(m_eState < FLCS::FLCS_STATE_MAX);
    HRESULT (__thiscall FLCS::*__ProcessStateCallback)(VOID) = m_apkProcessStateCallback[m_eState];
    AAV_ASSERT(__ProcessStateCallback != NULL);
    (this->*__ProcessStateCallback)();
  }
}

