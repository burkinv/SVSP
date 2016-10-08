#ifndef __MISSILE_MODULE_H__
#define __MISSILE_MODULE_H__

#include "WEAPON.h"

class MISSILE : public WEAPON
{
public:
  static const FLOAT               MASS;
  static const FLOAT               FUSE_DETONATION_DISTANCE;
  static const FLOAT               MIN_AIRSPEED;
  static const FLOAT               MAX_AIRSPEED;
  static const FLOAT               MAX_G;
  static const FLOAT               MAX_THRUST;
  static const FLOAT               TIME_TO_SELFDISTRUCT;
  static const FLOAT               MIN_LOCK_DISTANCE;
  static const FLOAT               MAX_LOCK_DISTANCE;
  static const FLOAT               EFFECTIVE_LOCK_DISTANCE;
  static const FLOAT               PROPELANT_BURNOUT_TIME;
  static const FLOAT               MIN_COS_OF_SEEKER_ASPECT_ANGLE;
  static const FLOAT               MIN_COS_OF_LOCKED_TARGET_ASPECT_ANGLE;
  static const AAVVector3<FLOAT>  AIR_RESISTANCE_FACTOR;

                              MISSILE(CHAR *pcName, CHAR *pcType, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude);
  virtual                    ~MISSILE();
  virtual HRESULT             SetState(WEAPON::STATE eState);
  virtual VOID                SetTargetID(DWORD dwObjectID);
  virtual VOID                Render(D3DXMATRIX &rkOPMatrix);
  virtual VOID                RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  virtual LPD3DXTEXTUREOBJECT GetWeaponTexture() const;


  //inline D3DXVECTOR3       GetVector() const { return D3DXVECTOR3(m_kVector.x, m_kVector.y, m_kVector.z); }
  //inline LPD3DXMATRIX      GetSeekerMatrix() const { return m_pkSeekerMatrix; }

protected:
  struct SET_AUX_OBJECT_STATE_DATA
  {
    FLOAT                    bSmokeEnable;
  } m_kSetAuxObjectStateData;

  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_BORESITE_VIEW_DESIGNATOR,
    GEOM_OBJECT_SEEKER_DIRECTION_DESIGNATOR,
    GEOM_OBJECT_DLZ_BAR_R1,
    GEOM_OBJECT_DLZ_BAR_R2,
    GEOM_OBJECT_DLZ_DESIGNATOR_R1,
    GEOM_OBJECT_DLZ_DESIGNATOR_R2,

    GEOM_OBJECT_NUM_OBJECTS
  };

  enum TEXTURE_OBJECTS_LIST
  {
    TEXTURE_OBJECT_SEEKER_DIRECTION_DESIGNATOR, 
    TEXTURE_OBJECT_LOCKED_TARGET_DESIGNATOR, 
    TEXTURE_OBJECT_WEAPON_IMAGE,

    TEXTURE_OBJECT_NUM_OBJECTS
  };

  static const UINT       NUM_BORESITE_VIEW_DESIGNATOR_VERTICES    = 144;
  static const UINT       NUM_SEEKER_DIRECTION_DESIGNATOR_VERTICES = 4;
  static const UINT       NUM_DLZ_BAR_R1_VERTICES                  = 4;
  static const UINT       NUM_DLZ_BAR_R2_VERTICES                  = 4;
  static const UINT       NUM_DLZ_DESIGNATOR_R1_VERTICES           = 72;
  static const UINT       NUM_DLZ_DESIGNATOR_R2_VERTICES           = 72;

  virtual VOID               DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);

  virtual VOID               ProcessStateIdle()                  ; //WEAPON_STATE_IDLE,
  virtual VOID               ProcessStateArmed()                 ; //WEAPON_STATE_ARMED,
  virtual VOID               ProcessStateAcquisition()           ; //WEAPON_STATE_ACQUISITION,
  virtual VOID               ProcessStateLocked()                ; //WEAPON_STATE_LOCKED,
  virtual VOID               ProcessStateLaunch()                ; //WEAPON_STATE_LAUNCH,
  virtual VOID               ProcessStateWaitLaunchAcknowledge() ; //WEAPON_STATE_WAIT_LAUNCH_ACKNOWLEDGE,
  virtual VOID               ProcessStatePursuit()               ; //WEAPON_STATE_PURSUIT,
  virtual VOID               ProcessStateMissed()                ; //WEAPON_STATE_MISSED,
  virtual VOID               ProcessStateDetonate()              ; //WEAPON_STATE_DETONATE,
  virtual VOID               ProcessStateNone()                  ; //WEAPON_STATE_NONE

  VOID                       UpdateTargetData();
  VOID                       RotateSeekerTowardTargetDirection();

  ////////////////////////////////////////////////////////////////////////////////////
  // MISSILE SPECIFICATION DATA
  ////////////////////////////////////////////////////////////////////////////////////
  FLOAT                      c_fMass;
  FLOAT                      c_fFuseDetonationDistance;
  FLOAT                      c_fMinAirspeed;
  FLOAT                      c_fMaxAirspeed;
  FLOAT                      c_fMaxG;
  FLOAT                      c_fMaxTrust;
  FLOAT                      c_fTimeToSelfDestruct;
  FLOAT                      c_fMinLockDistance;
  FLOAT                      c_fMaxLockDistance;
  FLOAT                      c_fEffectiveLockDistance;
  FLOAT                      c_fPropelantBurnoutTime;
  FLOAT                      c_fMinCosOfSeekerAspectAngle;
  FLOAT                      c_fMinCosOfLockedTargetAspectAngle;
  AAVVector3<FLOAT>         c_kAirResistanceFactor;

  ////////////////////////////////////////////////////////////////
  // MISSILE STATE DATA
  ////////////////////////////////////////////////////////////////
  FLOAT                      m_fSelectBoresiteTime;
  DWORD                      m_dwTargetMissedTime;
  FLOAT                      m_fStateUpdateDeltaTime;
  FLOAT                      m_fSpeedAspectRatio;
  FLOAT                      m_fCosInterceptAngle;
  FLOAT                      m_fMaxCosAngularRes;
  D3DXVECTOR3                m_kBodyTargetDirectionVector;
  D3DXVECTOR3                m_kBodySeekerDirectionVector;
  D3DXMATRIXA16              m_kLocalSeekerDirectionMatrix;
  const D3DXMATRIXA16       *m_pkSeekerMatrix;
  FLOAT                      m_fCosOfSeekerAspectAngle;

  ///////////////////////////////////////////////////////////////
  // TARGET STATE DATA
  ///////////////////////////////////////////////////////////////
  AAVVector3<FLOAT>         m_kTargetPreviousVector;
  AAVVector3<FLOAT>         m_kGlobalTargetVelocityVector;
  AAVVector3<FLOAT>         m_kGlobalTargetNormalizedVelocityVector;
  FLOAT                      m_fTargetVelocity;
  AAVVector3<FLOAT>         m_kGlobalTargetInterseptVector;
  AAVVector3<FLOAT>         m_kGlobalTargetInterseptDirectionVector;
  AAVVector3<FLOAT>         m_kGlobalTargetInterseptDirectionNormalizedVector;
  AAVVector3<FLOAT>         m_kLocalTargetInterceptDirectionVector;
  AAVVector3<FLOAT>         m_kBodyTargetInterceptDirectionVector;
  FLOAT                      m_fDistanceToIntercept;
  FLOAT                      m_fLunchEfficiencyFactor;
  FLOAT                      m_fPreviousDistanceToIntercept;
  D3DXMATRIXA16              m_kDLZ_R1Matrix;
  D3DXMATRIXA16              m_kDLZ_R2Matrix;

};

#endif	/* __MISSILE_MODULE_H__ */
