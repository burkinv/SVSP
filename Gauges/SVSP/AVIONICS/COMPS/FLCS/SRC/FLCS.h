#ifndef	__FCS_MODULE_H__
#define	__FCS_MODULE_H__

#include "__SVSPModule.h"

/**
  Full Authority Flight Control System Module.
  - Sim Tracking state machine.
  - AoA and G dependent flight surface deflection control.
  - AoA dependent automatic flaps control.
  - Conformal Velocity Vector and Flight Path Hold control (within +/-80 and beyond +/- 100 Degrees Bank Angle).
  - Automatic Pitch control. (Not implemented)
  - Full Authority Steerpoint navigation (Shared functionality with WPN module).
  - Primary Throttle and Augmented Engine Thrust control.
  - HOVER linear and angular stabilization control.
  - HOVER Reference Altitude control.
  - HOVER Reference Heading control.
  - HOVER Reference Terrain following.
  - Automatic Airbrake control.
  - Vectored Thrust Assistance control.
*/
class FLCS : public __SVSPModule
{
public:
  static const FLOAT MINIMUM_SPEED;
  static const FLOAT MAXIMUM_SPEED;
  static const FLOAT MAXIMUM_MACH;
  static const FLOAT BINGO_ALTITUDE;
  static const FLOAT MAXIMUM_G;
  static const FLOAT MAXIMUM_ANGLE_OF_ATTACK;
  static const FLOAT MAXIMUM_ANGLE_OF_ATTACK_RADIAN;
  static const FLOAT LEF_DEPLOYMENT_ANGLE_OF_ATTACK;
  static const FLOAT CRITICAL_ANGLE_OF_ATTACK;
  static const FLOAT GEAR_DOWN_WARNING_SPEED;
  static const FLOAT GEAR_UP_WARNING_SPEED;
  static const FLOAT MINIMAL_LANDING_PITCH_ANGLE;
  static const FLOAT MAXIMAL_LANDING_PITCH_ANGLE;
  static const FLOAT MAXIMAL_ELEVATOR_TRIM_DEFLECTION_ANGLE;
  static const ULONG OVERRIDE_PITCH_INPUT;
  static const ULONG OVERRIDE_ROLL_INPUT;
  static const ULONG OVERRIDE_YAW_INPUT;
  static const ULONG OVERRIDE_PITCH_TRIM_INPUT;
  static const ULONG OVERRIDE_SPOILER_OPERATION;
  static const ULONG OVERRIDE_FLAPS_OPERATION;
  static const ULONG VV_HOLD_Y_AREA;
  static const ULONG MANUALL_PITCH_OVERRIDE_BUTTON;
  static const FLOAT MANUALL_PITCH_OVERRIDE_MAXIMIUM_SPEED;
  static const FLOAT EFFECTIVE_STICK_RANGE_X;
  static const FLOAT EFFECTIVE_STICK_RANGE_Y;
  static const FLOAT MAX_HOVER_ACTIVATION_AIRSPEED;
  static const FLOAT MAX_HOVER_ACTIVATION_BANK_ANGLE;
  static const FLOAT MAX_HOVER_ACTIVATION_RPM_PERCENT_N1;
  static const UINT  MAX_NUM_LIMITER_CTRL_POINTS = 30;

  enum
  {
    MESSAGE_SYSTEM_FAILURE            = (WM_USER + 1304),
    MESSAGE_RUDDER_LEFT_INCREMENT,
    MESSAGE_RUDDER_RIGHT_INCREMENT,
    MESSAGE_RUDDER_CENTER,
    MESSAGE_TOGGLE_ELEVATORS_OVERRIDE,
    MESSAGE_MANUAL_PITCH_OVERRIDE_ENGAGE,
    MESSAGE_MANUAL_PITCH_OVERRIDE_DISENGAGE,
    MESSAGE_AIRBRAKE_INCREMENT,
    MESSAGE_AIRBRAKE_DECREMENT,
    MESSAGE_AIRBRAKE_HOLD
  };

 typedef enum 
 {
   FLCS_STATE_SLEW,
   FLCS_STATE_HOVER,
   FLCS_STATE_ON_GROUND,
   FLCS_STATE_AIRBORNE_GEAR_DOWN,
   FLCS_STATE_AIRBORNE,

   FLCS_STATE_MAX,
 } STATE;

  typedef  HRESULT (__thiscall FLCS::*ProcessStateCallback)(VOID);

                                 FLCS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual                       ~FLCS();
  inline FLCS::STATE             GetState() const { return m_eState; }

#define                          FLCS_ASSERT_RETURN_VOID(b) \
  if(!(b)) \
  { \
    return; \
  } \


  bool                           m_bAutomaticFlapsControl;
  AAVVector3<FLOAT>             m_kReferenceAttitudeVector;

protected:
  enum
  {
    RUDDER_STATE_CENTERED,
    RUDDER_STATE_LEFT,
    RUDDER_STATE_RIGHT
  };

  enum
  {
    AIRBRAKE_STATE_HOLD,
    AIRBRAKE_STATE_INCREMENT,
    AIRBRAKE_STATE_DECREMENT
  };

  enum
  {
    CONTROL_DATA_ID_ELEVATOR,
    CONTROL_DATA_ID_AILERON,
    CONTROL_DATA_ID_RUDDER,
    CONTROL_DATA_ID_SPOILER,
    CONTROL_DATA_ID_THROTTLE,
    CONTROL_DATA_ID_FLAPS,
    CONTROL_DATA_ID_LEAD_FLAPS,
    CONTROL_DATA_ID_TRAIL_FLAPS,
    CONTROL_DATA_ID_ELEVATOR_TRIM,
  };

  struct ELEVATOR_CONTROL
  {
    FLOAT           dYokeX;
    FLOAT           dOutput;
  };

  struct RUDDER_CONTROL
  {
    FLOAT           dYokeY;
    FLOAT           dOutput;
  };

  struct AILERON_CONTROL
  {
    FLOAT           dYokeZ;
    FLOAT           dOutput;
  };

  struct SPOILER_CONTROL
  {
    FLOAT           dOutput;
  };

  struct THROTTLE_CONTROL
  {
    FLOAT           dOutput[2];
  };

  struct FLAPS_CONTROL
  {
    FLOAT           dFlapsHandlePosition;
  } m_kFlapsControl;

  struct LEAD_FLAPS_CONTROL
  {
    FLOAT           dLeft;
    FLOAT           dRight;
  } m_kLeadingEdgeFlapControl;

  struct TRAIL_FLAPS_CONTROL
  {
    FLOAT           dLeft;
    FLOAT           dRight;
  } m_kTrailingEdgeFlapControl;

  struct ELEVATOR_TRIM_CONTROL
  {
    FLOAT           dOutput;
  };

  virtual VOID                   Start();
  virtual VOID                   Process();
  HRESULT                        ProcessStateSlew();              //FLCS::FLCS_STATE_SLEW,
  HRESULT                        ProcessStateHover();             //FLCS::FLCS_STATE_HOVER,
  HRESULT                        ProcessStateOnGround();          //FLCS::FLCS_STATE_ON_GROUND,
  HRESULT                        ProcessStateAirborneGearDown();  //FLCS::FLCS_STATE_AIRBORNE,
  HRESULT                        ProcessStateAirborne();          //FLCS::FLCS_STATE_AIRBORNE,

  FLCS::STATE                    m_eState;
  FLOAT                          m_dFlapsHandlePosition;
  FLOAT                          m_dSpoilerHandlePosition;
  ProcessStateCallback           m_apkProcessStateCallback[FLCS::FLCS_STATE_MAX];

private:
  static const AAVVector3<FLOAT>  c_akLimiterCtrlPointVectorTable[FLCS::MAX_NUM_LIMITER_CTRL_POINTS];
  static const AAVVector3<FLOAT>  c_fA;

  AAVVector3<FLOAT>             m_fB;

  VOID                           ProcessSteerpointNavigationControl();
  VOID                           ProcessAileronInput();
  VOID                           ProcessElevatorInput();
  VOID                           ProcessRudderInput();
  VOID                           ProcessThrottleInput();
  VOID                           ProcessThrottleInput_HOVER();
  VOID                           ProcessSpoilerControl();
  VOID                           ProcessFlapsControl();
  VOID                           ProcessVectorThrustControl();
  VOID                           ProcessTorqueControl();
  VOID                           ProcessTorqueControl_HOVER();
  VOID                           ProcessThrustControl();
  VOID                           ProcessThrustControl_HOVER();
  VOID                           ProcessElevatorTrimControl_AIRBORNE();

  AILERON_CONTROL                m_kAileronControl;
  ELEVATOR_CONTROL               m_kElevatorControl;
  RUDDER_CONTROL                 m_kRudderControl;
  SPOILER_CONTROL                m_kSpoilerControl;
  THROTTLE_CONTROL               m_kThrottleControl;
  LEAD_FLAPS_CONTROL             m_kLEFControl;
  ELEVATOR_TRIM_CONTROL          m_kElevatorTrimControl;

  AAVVector3<FLOAT>             m_kYokeIn;
  FLOAT                          m_dThrottleIn;
  AAVVector3<FLOAT>             m_kYokeInPrevious;
  AAVVector3<FLOAT>             m_kYokeOut;
  AAVVector3<FLOAT>             m_kBV;
  BOOL                           m_bAutomFlapSpeedControl;
  BOOL                           m_bBypassLimiter;
  FLOAT                          m_dReferenceAltitude;
  FLOAT                          m_dReferencePitch;
  FLOAT                          m_fFPMReferenceAngleDegrees;
  AAVVector3<FLOAT>             m_kReferenceLocalVelocityNormalized;
  AAVVector3<FLOAT>             m_kTangetialVelocity;

  FLOAT                          m_fInputPitchCommand;
  FLOAT                          m_fHeadingToGain;
  BOOL                           m_bHeadingSet;
  AAVVector3<FLOAT>             m_kInputControlVector;
  AAVVector3<FLOAT>             m_kAttitudeCorrectionControlVector;
  AAVVector3<FLOAT>             m_kAngularStabilizationControlVector;
  AAVVector3<FLOAT>             m_kLinearStabilizationControlVector;


};


#endif	/* __FCS_MODULE_H__ */
