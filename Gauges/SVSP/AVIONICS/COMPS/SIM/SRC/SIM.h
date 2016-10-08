#ifndef	__VTOL_H__
#define	__VTOL_H__

#include "__SVSPModule.h"
#include "__SVSPVector3.h"

class SIM : public __SVSPModule
{
public:
  static const FLOAT                     HOVER_THRUST_VECTOR_ANGLE;
  static const FLOAT                     PITCH_ASSISTANCE_ANGLE;
  static const FLOAT                     ANGLE_OF_ATTACK_MAX_CRITICAL_LIMIT;
  static const FLOAT                     ANGLE_OF_ATTACK_MIN_CRITICAL_LIMIT;
  static const FLOAT                     ANGLE_OF_ATTACK_MAX_CONTROL_LIMIT;
  static const FLOAT                     ANGLE_OF_ATTACK_MIN_CONTROL_LIMIT;
  static const FLOAT                     AIRSPEED_MIN_CRITICAL_LIMIT;
  static const FLOAT                     AIRSPEED_MAX_CRITICAL_LIMIT;
  static const AAVVector3<FLOAT>        g_kLocalGravityVector;
  static const AAVVector3<FLOAT>        g_kLocalGravityAcceleration;

  enum  
  {
    DATA_ID_SET_OBJECT_STATE_DATA,
  } DATA_ID;

  struct SET_OBJECT_STATE_DATA
  {
    AAVVector3<FLOAT>       kBodyAngularVelocity;
    AAVVector3<FLOAT>       kBodyVelocity;
  } m_kSetObjectStateData;

  AAVVector3<FLOAT>       m_kBodyNetForce;
  AAVVector3<FLOAT>       m_kBodyAirDrag;
  AAVVector3<FLOAT>       m_kBodyGravity;
  AAVVector3<FLOAT>       m_kBodyThrust;
  AAVVector3<FLOAT>       m_kBodyLiftFanThrust;

  FLOAT                    m_dHDDThrustAcceleration;
  FLOAT                    m_dLiftRatio ;
  FLOAT                    m_dTotalT2WRatio;
  FLOAT                    m_dMaxLiftAcceleration;
  FLOAT                    m_dLiftAcceleration;
  D3DXMATRIX               m_kAttitudeRotationMatrix;

  AAVVector3<FLOAT>       m_kBodyFSXCalculatedFoce;
  AAVVector3<FLOAT>       m_kBodyFlightSurfaceLift;
  AAVVector3<FLOAT>       m_kBodyFlightSurfaceInducedDrag;
  FLOAT                    m_dLinearSpeed;
  AAVVector3<FLOAT>       m_kBodyThrustAcceleration;
  AAVVector3<FLOAT>       m_kAirDragAcceleration;
  FLOAT                    m_dLiftToWeightRatio;
  FLOAT                    m_dThrustToWeightRatio;
  BOOL                     m_bAltitudeHold;
  FLOAT                    m_dReferenceAltitude;
  FLOAT                    m_dMach;
  FLOAT                    m_dVelocity;
  AAVVector3<FLOAT>       m_kBodyAcceleration;
  AAVVector3<FLOAT>       m_kBodyVelocity;
  AAVVector3<FLOAT>       m_kBodyVelocityNormalized;
  AAVVector3<FLOAT>       m_kBodyAngularAcceleration;
  AAVVector3<FLOAT>       m_kBodyAngularVelocity;
  AAVVector3<FLOAT>       m_kTangetialVelocity;
  AAVVector3<FLOAT>       m_kLocalVelocity;
  AAVVector3<FLOAT>       m_kLocalStallEntryVelocity;
  AAVVector3<FLOAT>       m_kBodyStallEntryAngularVelocity;
  AAVVector3<FLOAT>       m_kInertiaThensor;
  AAVVector3<FLOAT>       m_kMomentOfInertia;
  AAVVector3<FLOAT>       m_kBodyFlightSurfaceTorque;
  AAVVector3<FLOAT>       m_kBodyAngularStabilizationTorque;
  AAVVector3<FLOAT>       m_kControlInput;
  AAVVector3<FLOAT>       m_kBodyVelocityStabilizationTorque;
  AAVVector3<FLOAT>       m_kBodyReactionTorque;
  AAVVector3<FLOAT>       m_kBodyVectoredThrustTorque;
  AAVVector3<FLOAT>       m_kTotalTorque;
  AAVVector3<FLOAT>       m_kAirspeedFactor;
  AAVVector3<FLOAT>       m_kAoAFactor;

                            SIM(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual                  ~SIM();

private:
  AAVVector3<FLOAT>        m_kAngularAttitudeVector;
  AAVVector3<SHORT>        m_kInputVector;
  FLOAT                     m_fCurrentPitchControlInput;
  BOOL                      m_bActive;

  AAVVector3<FLOAT>        CalculateBodyAngularVelocity();
  AAVVector3<FLOAT>        CalculateBodyVelocity();
  virtual VOID              Start();
  virtual VOID              Process();

};

#endif	/* __VTOL_H__ */
