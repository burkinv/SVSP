#include "__SVSPAvionics.h"
#include "AC.h"
#include "SIM.h"
#include "FLCS.h"
#include "ECS.h"

#define INPUT_TO_ACCELERATION_FACTOR 0.8

const FLOAT                 SIM::HOVER_THRUST_VECTOR_ANGLE           = D3DX_PI*(25.0f/180.0f);
const FLOAT                 SIM::PITCH_ASSISTANCE_ANGLE              = D3DX_PI*(5.0f/180.0f);
const FLOAT                 SIM::ANGLE_OF_ATTACK_MAX_CRITICAL_LIMIT  = D3DX_PI*(47.0f/180.0f);
const FLOAT                 SIM::ANGLE_OF_ATTACK_MIN_CRITICAL_LIMIT  = D3DX_PI*(-37.0f/180.0f);
const FLOAT                 SIM::ANGLE_OF_ATTACK_MAX_CONTROL_LIMIT   = D3DX_PI*(36.0f/180.0f);
const FLOAT                 SIM::ANGLE_OF_ATTACK_MIN_CONTROL_LIMIT   = D3DX_PI*(-15.0f/180.0f);
const FLOAT                 SIM::AIRSPEED_MIN_CRITICAL_LIMIT         = 110.0f;
const FLOAT                 SIM::AIRSPEED_MAX_CRITICAL_LIMIT         = 3200.0f;
const AAVVector3<FLOAT>    SIM::g_kLocalGravityVector               = AAVVector3<FLOAT>(0, -1, 0);
const AAVVector3<FLOAT>    SIM::g_kLocalGravityAcceleration         = AAVVector3<FLOAT>(0, -9.81f*3.33f, 0);

SIM::SIM(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("SIM", pkAvionics, dwThreadSleepTime, true, NULL, NULL, NULL)
{
  ///////////////////////////////////////////////////////////////////
  // SIM::DATA_ID_SET_OBJECT_STATE_DATA
  ///////////////////////////////////////////////////////////////////
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SIM::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY X",           "Radians per second",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SIM::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Y",           "Radians per second",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SIM::DATA_ID_SET_OBJECT_STATE_DATA, "ROTATION VELOCITY BODY Z",           "Radians per second",         SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SIM::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY X",                    "Feet per second",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SIM::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY Y",                    "Feet per second",            SIMCONNECT_DATATYPE_FLOAT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, SIM::DATA_ID_SET_OBJECT_STATE_DATA, "VELOCITY BODY Z",                    "Feet per second",            SIMCONNECT_DATATYPE_FLOAT32);

}

SIM::~SIM()
{
  __SVSPModule::Stop();
}

//HRESULT SIM::ProcessSim()
//{
//  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
//  AAV_ASSERT(pkAC != NULL);
//
//  ECS *pkECS = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
//  AAV_ASSERT(pkECS != NULL);
//
//  HOTAS* pkHOTAS = static_cast<HOTAS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HOTAS));
//  AAV_ASSERT(pkHOTAS != NULL);
//
//  AC::AIRCRAFT_STATE_DATA     *pkData            = &pkAC->m_kData;
//
//
//  ////////////////////////////////////////////////
//  // ALTITUDE STABILIZATION CONTROL
//  ////////////////////////////////////////////////
//  m_bAltitudeHold = ((!pkData->m_bOnGround) && (m_dLiftToWeightRatio > 1.0))? true : false;
//
//  //BOOL bBrake     = ((pkData->m_dBrakeParking != 0) | (pkData->m_dBrakePosition != 0));
//  BOOL bBrake     = ((pkData->m_dBrakeParking != 0));
//
//
//  ////////////////////////////////////////////////
//  // BODY LINEAR ACCELERATION
//  ////////////////////////////////////////////////
//  FLOAT dLiftControlFlag = (FLOAT)((pkHOTAS->m_kJS.rgbButtons[20] >> 7) - (pkHOTAS->m_kJS.rgbButtons[19] >> 7));
//  if(m_bAltitudeHold)
//  {
//    if(dLiftControlFlag != 0)
//    {
//      //TODO: Either Barometric or Radar altitude (LTF).
//      m_dReferenceAltitude = pkData->m_dIndicatedAltitude;
//    }
//
//    FLOAT dMaxIncrementalThrustAcceleration = m_dThrust/m_dMass - abs(g_kLocalGravityAcceleration.y);
//    m_kTangetialVelocity                    = pkAC->m_kBodyToTangentialTransformMatrix*m_kBodyVelocity;
//    m_kBodyGravity                          = pkAC->m_kLocalToBodyTransformMatrix*g_kLocalGravityAcceleration;
//    FLOAT dZeroLiftThrustAcceleration       = abs(m_kBodyGravity.y);
//    ////////////////////////////////////////////////////////////////
//    // P.I.D STABILISED CONTROL FOR BODY VERTICAL ACCELERATION
//    ////////////////////////////////////////////////////////////////
//    FLOAT dIncrementalThrustAcceleration    = dMaxIncrementalThrustAcceleration*dLiftControlFlag + 0.35f*(m_dReferenceAltitude - pkData->m_dIndicatedAltitude) - m_kTangetialVelocity.y;
//    dIncrementalThrustAcceleration          = (abs(dIncrementalThrustAcceleration) > dMaxIncrementalThrustAcceleration) ? sign(dIncrementalThrustAcceleration)*dMaxIncrementalThrustAcceleration : dIncrementalThrustAcceleration;
//    m_kBodyThrustAcceleration.y              = dZeroLiftThrustAcceleration + dIncrementalThrustAcceleration;
//  }
//  else
//  {
//    m_dReferenceAltitude                    = pkData->m_dIndicatedAltitude;
//    m_kBodyThrustAcceleration.y              = m_dThrust/m_dMass;
//  }
//
//  //m_kBodyThrustAcceleration.y                = 
//
//  m_kBodyAcceleration = AAVVector3<FLOAT>(pkData->m_kBodyAcceleration.x                     - m_kAirDragAcceleration.x + m_kBodyThrustAcceleration.x,
//                                           pkData->m_kBodyAcceleration.y                     - m_kAirDragAcceleration.y + m_kBodyThrustAcceleration.y,
//                                           pkData->m_kBodyAcceleration.z - m_dThrust/m_dMass - m_kAirDragAcceleration.z + m_kBodyThrustAcceleration.z);
//  ////////////////////////////////////////////////////////////////
//  // BODY LINEAR VELOCITY
//  ////////////////////////////////////////////////////////////////
//   m_kBodyVelocity                          = m_kBodyVelocity + m_kBodyAcceleration*m_dDeltaTime;
//
//  ////////////////////////////////////////////////////////////////
//  // TANGENTIAL VELOCITY
//  ////////////////////////////////////////////////////////////////
//  D3DXVec3TransformCoord((D3DXVECTOR3 *)&m_kTangetialVelocity, (D3DXVECTOR3 *)&m_kBodyVelocity, &pkAC->m_kBodyToTangentialTransformMatrix);
//
//  ////////////////////////////////////////////////////////////////
//  // P.I.D STABILISED CONTROL FOR BODY ANGULAR ACCELERATION
//  ////////////////////////////////////////////////////////////////
//  m_kBodyFlightSurfaceTorque         = AAVVector3<FLOAT>(/*Pitch*/0,
//                                                          /*Yaw  */0,
//                                                          /*Roll */0);
//
//  m_kBodyReactionTorque              = AAVVector3<FLOAT>(/*Pitch*/0,
//                                                          /*Yaw  */0,
//                                                          /*Roll */0);
//
//  m_kBodyAngularStabilizationTorque  = AAVVector3<FLOAT>(/*Pitch*/ 50000.0f*(- 2.4f*pkAC->m_kBodyAttitude.x - 0.7f*pkData->m_kBodyAngularVelocity.x),
//                                                          /*Yaw  */ 30000.0f*(                               - 0.6f*pkData->m_kBodyAngularVelocity.y),
//                                                          /*Roll */ 50000.0f*(- 2.2f*pkAC->m_kBodyAttitude.z - 0.7f*pkData->m_kBodyAngularVelocity.z));
//
//  ////////////////////////////////////////////////////////////////
//  // CONTROL INPUT OVERRIDE (TANGENTIAL VELOCITY STABILIZATION)
//  ////////////////////////////////////////////////////////////////
//  AAVVector3<FLOAT> kA              = AAVVector3<FLOAT>(/*Pitch*/ 0.02f,
//                                                          /*Yaw  */ 0,
//                                                          /*Roll */ 0.07f);
//
//  FLOAT fPitchControlInput = pkData->m_kControlInput.x;
//  if((fPitchControlInput < 0.2f) && !bBrake)
//  {
//    if( (fPitchControlInput < m_fCurrentPitchControlInput) && (fPitchControlInput > -0.5f) )
//    {
//      m_fCurrentPitchControlInput = fPitchControlInput;
//    }
//  }
//  else
//  {
//    m_fCurrentPitchControlInput = fPitchControlInput;
//  }
//
//  m_kControlInput                    = AAVVector3<FLOAT>(/*Pitch*/ (abs(m_fCurrentPitchControlInput) > 0.05f) ? m_fCurrentPitchControlInput : (bBrake ?  (kA.x*m_kTangetialVelocity.z/sqrt(1.0f + pow(kA.x*m_kTangetialVelocity.z, 2))) : m_fCurrentPitchControlInput),
//                                                          /*Yaw  */                                              pkData->m_kControlInput.y,
//                                                          /*Roll */ (abs(pkData->m_kControlInput.z) > 0.05f)   ? pkData->m_kControlInput.z :             -(kA.z*m_kTangetialVelocity.x/sqrt(1.0f + pow(kA.z*m_kTangetialVelocity.x, 2))));
//
//
//
//  m_kBodyVectoredThrustTorque        = AAVVector3<FLOAT>(/*Pitch*/ 60000.0f*(-0.4f*m_dLiftToWeightRatio*m_kControlInput.x - abs(0.07f*m_dLiftToWeightRatio*pkData->m_kControlInput.y) + 0.07f*dLiftControlFlag),
//                                                          /*Yaw  */ 10000.0f*( 0.7f*m_dLiftToWeightRatio*m_kControlInput.y                                                                                           ),
//                                                          /*Roll */ 60000.0f*(-0.3f*m_dLiftToWeightRatio*m_kControlInput.z -     0.10f*m_dLiftToWeightRatio*pkData->m_kControlInput.y                                ));
//
//  ////////////////////////////////////////////////////////////////
//  // TOTAL TORQUE
//  ////////////////////////////////////////////////////////////////
//  m_kTotalTorque = m_kBodyFlightSurfaceTorque + m_kBodyReactionTorque + m_kBodyAngularStabilizationTorque + m_kBodyVectoredThrustTorque;
//
//  m_kBodyAngularAcceleration  = AAVVector3<FLOAT>(m_kTotalTorque.x/m_kMomentOfInertia.x,
//                                                   m_kTotalTorque.y/m_kMomentOfInertia.y,
//                                                   m_kTotalTorque.z/m_kMomentOfInertia.z);
//
//
//  ////////////////////////////////////////////////////////////////
//  // BODY ANGULAR VELOCITY
//  ////////////////////////////////////////////////////////////////
//  m_kBodyAngularVelocity     = m_kBodyAngularVelocity + m_kBodyAngularAcceleration*m_dDeltaTime;
//
//
//  //m_kLocalVelocity          = pkAC->m_kBodyToLocalTransformMatrix*m_kBodyVelocity;
//  m_kBodyVelocityNormalized = m_kBodyVelocity.Normalize();
//
//  ////////////////////////////////////////////////////////////////
//  // UPDATE FS SIM VARIABLES
//  ////////////////////////////////////////////////////////////////
//  m_kSetObjectStateData.kBodyAngularVelocity = m_kBodyAngularVelocity;
//  m_kSetObjectStateData.kBodyVelocity        = m_kBodyVelocity;
//  m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, SIM::DATA_ID_SET_OBJECT_STATE_DATA, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(SIM::SET_OBJECT_STATE_DATA), &m_kSetObjectStateData);
//  
//  return m_hResult;
//}


//HRESULT SIM::ProcessStatePitchAssistance()
//{
//  //AC  *pkAC  = GET_MODULE_OR_RETURN(__SVSPAvionics::AVIONICS_MODULE_AC, S_FAILED);
//
//  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
//  AAV_ASSERT(pkAC != NULL);
//
//  FLCS *pkFLCS = static_cast<FLCS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FLCS));
//  AAV_ASSERT(pkFLCS != NULL);
//
//  ECS *pkECS = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
//  AAV_ASSERT(pkECS != NULL);
//
//  HOTAS* pkHOTAS = static_cast<HOTAS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HOTAS));
//  AAV_ASSERT(pkHOTAS != NULL);
//
//  AC::AIRCRAFT_STATE_DATA     *pkData            = &pkAC->m_kData;
//
//
//  m_kBodyAirDrag = AAVVector3<FLOAT>(-0.005f*pow(m_kBodyVelocity.x, 3), -0.015f*pow(m_kBodyVelocity.y, 3), -0.0012f*pow(m_kBodyVelocity.z, 3));
//
//  ////////////////////////////////////////////////////////////////
//  // BODY NET FORCE
//  ////////////////////////////////////////////////////////////////
//  m_kBodyNetForce = m_kBodyAirDrag + m_kBodyGravity + m_kBodyThrust;
//
//  ////////////////////////////////////////////////////////////////
//  // BODY LINEAR ACCELERATION
//  ////////////////////////////////////////////////////////////////
//  m_kBodyAcceleration = m_kBodyNetForce/m_dMass;
//
//  ////////////////////////////////////////////////////////////////
//  // BODY LINEAR VELOCITY
//  ////////////////////////////////////////////////////////////////
//  m_kBodyVelocity  = m_kLocalVelocity*pkAC->m_kLocalToBodyTransformMatrix + m_kBodyAcceleration*m_dDeltaTime;
//  m_kLocalVelocity = m_kBodyVelocity*pkAC->m_kBodyToLocalTransformMatrix;
//  m_kBodyVelocityNormalized = m_kBodyVelocity.Normalize();
//
//
//  ////////////////////////////////////////////////////////////////
//  // BODY ANGULAR ACCELERATION
//  ////////////////////////////////////////////////////////////////
//  m_kAirspeedFactor                  = AAVVector3<FLOAT>(/*Pitch*/ 0.0f , //(1 - 1./(1 + Bx*(X - B)));
//                                                          /*Yaw  */ 1.0f ,
//                                                          /*Roll */ 0.0f );
//  //m_kAirspeedFactor           = AAVVector3<FLOAT>(/*Pitch*/ 1.0 - 1.0/(1.0 + 0.02*pkData->m_dIndicatedAirspeed), //(1 - 1./(1 + Bx*(X - B)));
//  //                                                 /*Yaw  */ 1.0 - 1.0/(1.0 + 0.01*pkData->m_dIndicatedAirspeed),
//  //                                                 /*Roll */ 1.0 - 1.0/(1.0 + 0.05*pkData->m_dIndicatedAirspeed));
//  //
//  //m_kAoAFactor                = AAVVector3<FLOAT>(/*Pitch*/ ), //-K*(Cx*(X - C)./sqrt(1 + Cx^2*(X - C).^2));
//  //                                                 /*Yaw  */ 5.0*(0.872 - pkAC->m_fAoA)/sqrt(1.0 + 25.0*pow((pkAC->m_fAoA - 0.872), 2)),
//  //                                                 /*Roll */ 5.0*(0.872 - pkAC->m_fAoA)/sqrt(1.0 + 25.0*pow((pkAC->m_fAoA - 0.872), 2)));
//  //
//  m_kBodyFlightSurfaceTorque         = AAVVector3<FLOAT>(/*Pitch*/   11750.0f*m_kAirspeedFactor.x*pkData->m_kControlInput.x + 0.4f*m_kBodyAirDrag.y,
//                                                          /*Yaw  */   30100.0f*m_kAirspeedFactor.y*pkData->m_kControlInput.y,
//                                                          /*Roll */-  11800.0f*m_kAirspeedFactor.z*pkData->m_kControlInput.z);
//
//  m_kBodyAngularStabilizationTorque  = AAVVector3<FLOAT>(/*Pitch*/- 100000.0f*m_kBodyAngularVelocity.x,
//                                                          /*Yaw  */-  70000.0f*m_kBodyAngularVelocity.y,
//                                                          /*Roll */- 200000.0f*m_kBodyAngularVelocity.z);
//
//  m_kBodyReactionTorque              = AAVVector3<FLOAT>(/*Pitch*/0.000001f*m_kMomentOfInertia.x*m_kBodyVelocityNormalized.x*m_kBodyFlightSurfaceTorque.y,
//                                                          /*Yaw  */0.000001f*m_kMomentOfInertia.y*m_kBodyVelocityNormalized.y*m_kBodyFlightSurfaceTorque.y,
//                                                          /*Roll */0.000001f*m_kMomentOfInertia.z*m_kBodyVelocityNormalized.z*m_kBodyFlightSurfaceTorque.y);
//
//  m_kBodyVectoredThrustTorque        = AAVVector3<FLOAT>(/*Pitch*/m_kBodyThrust.y*1.7f, 
//                                                          /*Yaw  */0,
//                                                          /*Roll */0);
//
//  m_kTotalTorque = m_kBodyFlightSurfaceTorque + m_kBodyVectoredThrustTorque + m_kBodyAngularStabilizationTorque;//m_kBodyReactionTorque + 
//
//  m_kBodyAngularAcceleration  = AAVVector3<FLOAT>(m_kTotalTorque.x/m_kMomentOfInertia.x,
//                                                   m_kTotalTorque.y/m_kMomentOfInertia.y,
//                                                   m_kTotalTorque.z/m_kMomentOfInertia.z);
//
//  ////////////////////////////////////////////////////////////////
//  // BODY ANGULAR VELOCITY
//  ////////////////////////////////////////////////////////////////
//  m_kBodyAngularVelocity     = m_kBodyAngularVelocity + m_kBodyAngularAcceleration*m_dDeltaTime;
//
//
//  // Set new state data
//  m_kSetObjectStateData.kBodyAngularVelocity = m_kBodyAngularVelocity;
//  m_kSetObjectStateData.kBodyVelocity        = m_kBodyVelocity;
//  m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, SIM::DATA_ID_SET_OBJECT_STATE_DATA, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(SIM::SET_OBJECT_STATE_DATA), &m_kSetObjectStateData);
//
//  return m_hResult;
//}
//

///////////////////////////////////////////////////////
// SIM
///////////////////////////////////////////////////////

AAVVector3<FLOAT> SIM::CalculateBodyAngularVelocity()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN(pkAC != NULL, AAVVector3<FLOAT>(0, 0, 0));
  AC::AIRCRAFT_STATE_DATA *pkData = &pkAC->m_kData;

  ECS *pkECS = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  AAV_ASSERT_RETURN(pkECS != NULL, AAVVector3<FLOAT>(0, 0, 0));
  ECS::ENGINE *pkEngine = &pkECS->m_akEngine[0];

  // FLIGHT SURFACE TORQUE
  m_kBodyFlightSurfaceTorque         = AAVVector3<FLOAT>(/*Pitch*/45000*(-0.0015f*(m_kBodyVelocity.y + m_kBodyVelocity.z) - 3.85f*m_kBodyAngularVelocity.x),
                                                          /*Yaw  */0,
                                                          /*Roll */0);

  // REACTION TORQUE
  m_kBodyReactionTorque              = AAVVector3<FLOAT>(/*Pitch*/45000*(0.00015f*m_kBodyVelocity.x),
                                                          /*Yaw  */45000*(0.00045f*m_kBodyVelocity.y),
                                                          /*Roll */45000*(0.00015f*m_kBodyVelocity.z));

  // ANGULAR STABILIZATION TORQUE
  m_kBodyAngularStabilizationTorque  = AAVVector3<FLOAT>(/*Pitch*/ 0,
                                                          /*Yaw  */ 0,
                                                          /*Roll */ 0);
  //m_kBodyAngularStabilizationTorque  = AAVVector3<FLOAT>(/*Pitch*/ 50000.0f*(- 2.4f*pkAC->m_kBodyAttitude.x - 0.7f*pkData->m_kBodyAngularVelocity.x),
  //                                                        /*Yaw  */ 30000.0f*(                               - 0.6f*pkData->m_kBodyAngularVelocity.y),
  //                                                        /*Roll */ 50000.0f*(- 2.2f*pkAC->m_kBodyAttitude.z - 0.7f*pkData->m_kBodyAngularVelocity.z));

  ////////////////////////////////////////////////////////////////
  // TOTAL TORQUE
  ////////////////////////////////////////////////////////////////
  m_kTotalTorque = m_kBodyFlightSurfaceTorque + m_kBodyReactionTorque + m_kBodyAngularStabilizationTorque + pkEngine->kBodyTorque;

  // TOTAL ANGULAR ACCLERATION
  m_kBodyAngularAcceleration  = AAVVector3<FLOAT>(m_kTotalTorque.x/m_kMomentOfInertia.x,
                                                   m_kTotalTorque.y/m_kMomentOfInertia.y,
                                                   m_kTotalTorque.z/m_kMomentOfInertia.z);

  // BODY ANGULAR VELOCITY
  return m_kBodyAngularVelocity + m_kBodyAngularAcceleration*m_dDeltaTime;
}


AAVVector3<FLOAT> SIM::CalculateBodyVelocity()
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN(pkAC != NULL, AAVVector3<FLOAT>(0, 0, 0));
  AC::AIRCRAFT_STATE_DATA *pkData = &pkAC->m_kData;

  ECS *pkECS = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  AAV_ASSERT_RETURN(pkECS != NULL, AAVVector3<FLOAT>(0, 0, 0));
  ECS::ENGINE *pkEngine = &pkECS->m_akEngine[0];

  // BODY GRAVITY
  AAVVector3<FLOAT> kBodyGravityVector = pkAC->m_kLocalToBodyTransformMatrix*g_kLocalGravityVector;
  AAVVector3<FLOAT> kBodyGravityAcceleration = pkAC->m_kLocalToBodyTransformMatrix*g_kLocalGravityAcceleration;
  m_kBodyGravity                  = kBodyGravityAcceleration*pkAC->m_dMass;

  //FSX CALCULATED FORCE
  m_kBodyFSXCalculatedFoce        = pkData->m_kBodyAcceleration*pkAC->m_dMass + AAVVector3<FLOAT>(0, 0, - pkEngine->dCurrentThrustCapacity);

  // BODY FLIGHT SURFACE LIFT
  m_kBodyFlightSurfaceLift        = AAVVector3<FLOAT>(/*Lateral     */0, 
                                                       /*Vertical    */0, 
                                                       /*Longitudinal*/0);

  // BODY INDUCED DRAG
  m_kBodyFlightSurfaceInducedDrag = AAVVector3<FLOAT>(/*Lateral     */0, 
                                                       /*Vertical    */0, 
                                                       /*Longitudinal*/0);

  // BODY AIR DRAG
  m_kBodyAirDrag                  = AAVVector3<FLOAT>(/*Lateral     */0, 
                                                       /*Vertical    */0, 
                                                       /*Longitudinal*/0);

  // NET FORCE
  m_kBodyNetForce = m_kBodyFSXCalculatedFoce + m_kBodyFlightSurfaceLift + m_kBodyFlightSurfaceInducedDrag + m_kBodyAirDrag + pkEngine->kBodyThrust;
  //m_kBodyNetForce = m_kBodyGravity + m_kBodyFlightSurfaceLift + m_kBodyFlightSurfaceInducedDrag + m_kBodyAirDrag + pkEngine->kBodyThrust;

  // BODY LINEAR ACCELERATION
  m_kBodyAcceleration = m_kBodyNetForce/pkAC->m_dMass;

  // BODY LINEAR VELOCITY
  return m_kBodyVelocity + m_kBodyAcceleration*m_dDeltaTime;
}

VOID SIM::Start()
{
  AAVVector3<FLOAT> kInitRotationVelocity = AAVVector3<FLOAT>(0, 0, 0);
  AAVVector3<FLOAT> kSpinRotationVelocity = AAVVector3<FLOAT>(0, 0, 0);
  m_dReferenceAltitude                     = 0;
  m_fCurrentPitchControlInput              = 0;
  m_kBodyVelocity                          = AAVVector3<FLOAT>(0, 0, 0);
  m_kBodyAngularVelocity                   = AAVVector3<FLOAT>(0, 0, 0);
  m_kInertiaThensor                        = AAVVector3<FLOAT>(/*Pitch*/180.0, 
                                                                /*Yaw  */100.0, 
                                                                /*Roll */ 50.0);

  __SVSPModule::Start();
}


VOID SIM::Process()
{
  AC  *pkAC    = static_cast<AC   *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC != NULL);
  ECS *pkECS   = static_cast<ECS  *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  AAV_ASSERT_RETURN_VOID(pkECS != NULL);
  FLCS *pkFLCS = static_cast<FLCS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FLCS));
  AAV_ASSERT_RETURN_VOID(pkFLCS != NULL);
  AC::AIRCRAFT_STATE_DATA *pkData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkData != NULL);

  ECS::ENGINE *pkEngine = &pkECS->m_akEngine[0];


  if(m_pkAvionics->IsValid())
  {
    AAV_ASSERT_RETURN_VOID(pkAC->m_dMass != 0);

    m_kMomentOfInertia        = m_kInertiaThensor*pkAC->m_dMass;
    m_kBodyAirDrag            = AAVVector3<FLOAT>(0, 0, 0);
    m_dTotalT2WRatio          = pkEngine->dThrust/pkData->m_dTotalWeight;
    m_dLiftToWeightRatio      = m_kBodyThrust.y/pkData->m_dTotalWeight;
    m_kAirDragAcceleration    = AAVVector3<FLOAT>(0, 0, 0);
    m_dMach                   = m_kBodyVelocity.Length()/1105.56f;


    if(((!pkData->m_bOnGround) && ((pkAC->m_fAoA > FLCS::CRITICAL_ANGLE_OF_ATTACK) || 
       (pkData->m_dIndicatedAirspeed < FLCS::MINIMUM_SPEED))) ||
       (pkFLCS->GetState() == FLCS::FLCS_STATE_HOVER))
    {
      //  m_kLocalStallEntryVelocity       = pkAC->m_kLocalVelocity;
      //  m_kLocalStallEntryVelocity.x     = -m_kLocalStallEntryVelocity.x;
      //  m_kLocalVelocity                 = m_kLocalStallEntryVelocity;
      //  m_kBodyStallEntryAngularVelocity = 0.5f*pkAC->m_kData.m_kBodyAngularVelocity;

      m_bValid                  = true;
      m_kBodyAngularVelocity    = CalculateBodyAngularVelocity();
      m_kBodyVelocity           = CalculateBodyVelocity();
      m_dVelocity               = m_kBodyVelocity.Length();
      m_kBodyVelocityNormalized = (m_dVelocity != 0) ? m_kBodyVelocity/m_dVelocity : AAVVector3<FLOAT>(0, 0, 0);

      //Set SIM variables.
      m_kSetObjectStateData.kBodyAngularVelocity = m_kBodyAngularVelocity;
      m_kSetObjectStateData.kBodyVelocity        = m_kBodyVelocity;
      m_hResult = SimConnect_SetDataOnSimObject(m_hSimConnect, SIM::DATA_ID_SET_OBJECT_STATE_DATA, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(SIM::SET_OBJECT_STATE_DATA), &m_kSetObjectStateData);
    }
    else
    {
      m_bValid                  = false;
      m_kBodyAngularVelocity    = 0.5f*pkData->m_kBodyAngularVelocity;
      m_kBodyVelocity           = pkAC->m_kBodyVelocity;
      m_dVelocity               = pkAC->m_dVelocity;
      m_kBodyVelocityNormalized = pkAC->m_kBodyVelocityNormalized;
      //m_kLocalVelocity        = pkAC->m_kBodyToLocalTransformMatrix*m_kBodyVelocity;
    }
  }
}

