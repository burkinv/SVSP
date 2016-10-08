#ifndef __CARRIER_MODULE_H__
#define __CARRIER_MODULE_H__

#include "__SVSPModule.h"
#include "__SVSPGeoCoordinates3.h"


class CARRIER : public __SVSPModule
{
public:
  static const FLOAT              AIRCRAFT_EVALUATION_RANGE;
  static const FLOAT              AIRCRAFT_APPROACH_COSINUS;
  static const FLOAT              AIRCRAFT_EVALUATION_TIME;
  static const FLOAT              TOUCH_AND_GO_EVALUATION_TIME;
  static const AAVVector3<FLOAT> kBodyOptimalApproachVector;
  static const UINT               MAX_NUM_APPROACH_WAYPOINTS = 2;

  enum 
  {
    MESSAGE_CAT_HOOK_STATE_CHANGED    = (WM_USER + 1405),
  };

  enum
  {
    REQUEST_ID_CREATE_OBJECT           = 0x5,
    REQUEST_ID_REMOVE_OBJECT,
    REQUEST_ID_RELEASE_CONTROL,
    REQUEST_ID_READ_OBJECT_STATE_DATA,
    REQUEST_ID_SET_OBJECT_STATE_DATA,
    REQUEST_ID_SET_USER_AC_SIM_DATA,
  } REQUEST_ID;

  enum
  {
    EVENT_ID_OBJECT_ADDED,
    EVENT_ID_OBJECT_REMOVED,
  } EVENT_ID;

  enum  
  {
    DATA_ID_READ_OBJECT_STATE_DATA,
    DATA_ID_SET_OBJECT_STATE_DATA,
    DATA_ID_SET_AUX_OBJECT_STATE_DATA,
    DATA_ID_SET_USER_AC_SIM_DATA,
  } DATA_ID;

  struct READ_OBJECT_STATE_DATA
  {
    __SVSPGeoCoordinates3<FLOAT>    kLocation;
    AAVVector3<FLOAT>       kBodyAttitude;
  } m_kReadObjectStateData;

  struct SET_OBJECT_STATE_DATA
  {
    AAVVector3<FLOAT>       kBodyAngularVelocity;
    AAVVector3<FLOAT>       kBodyVelocity;
  } m_kSetObjectStateData;

  struct SET_USER_AC_SIM_DATA
  {
    AAVVector3<FLOAT> kBodyAcceleration;
    AAVVector3<FLOAT> kBodyVelocity;
    //AAVVector3<FLOAT> kBodyAngularVelocity;
    //AAVVector3<FLOAT> kBodyAttitude;
  } m_kSetUserAcSimData;

  typedef                    VOID (__thiscall CARRIER::*ProcessStateCallback)(VOID);

typedef enum
{
  CARRIER_STATE_NONE,
  CARRIER_STATE_IDLE,
  CARRIER_STATE_ENROUTE,
  CARRIER_STATE_APPROACH_EVALUATION,
  CARRIER_STATE_FLYBY_EVALUATION,
  CARRIER_STATE_USER_AC_ON_APPROACH,
  CARRIER_STATE_ARRESTING,
  CARRIER_STATE_TOUCH_AND_GO_EVALUATION,
  CARRIER_STATE_TOUCH_DOWN_EVALUATION,
  CARRIER_STATE_CAT_ARMED,
  CARRIER_STATE_CAT_LAUNCHED,
  CARRIER_STATE_MAX
} eState;

                             CARRIER(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, CHAR *pcName, __SVSPGeoCoordinates3<FLOAT> *pkLocation, AAVVector3<FLOAT> *pkAttitude);
  virtual                   ~CARRIER();
  VOID                       RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  VOID                       RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix, FLOAT fScale);
  inline const D3DXVECTOR3*  GetLocalOffsetVector() const { return &m_kLocalOffsetVector; }

protected:
  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_APPROACH_MAP,
    GEOM_OBJECT_NUM_OBJECTS
  };

  static const UINT          NUM_APPROACH_MAP_VERTICES = 2;

  virtual VOID               Start();
  virtual VOID               Process();
  virtual VOID               DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);
  friend  VOID CALLBACK      CARRIER__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);
          D3DXVECTOR2        GetACLSErrorVector();

  ///////////////////////////////////////////////////////////////
  // FSX SPECIFIC OBJECT DATA
  ///////////////////////////////////////////////////////////////
  DWORD                     m_dwObjectID;
  AAVVector3<FLOAT>        m_kBodyAngularVelocity;
  AAVVector3<FLOAT>        m_kBodyVelocity;

  ///////////////////////////////////////////////////////////////
  // CARRIER POSITION AND ORIENTATION DATA
  ///////////////////////////////////////////////////////////////
  __SVSPGeoCoordinates3<FLOAT>    m_kLocation;
  AAVVector3<FLOAT>        m_kApproachStartVector;
  AAVVector3<FLOAT>        m_kAttitude;
  UINT                     m_eState;
  AAVWAYPOINT<FLOAT>       m_akApproachWaypoint[CARRIER::MAX_NUM_APPROACH_WAYPOINTS];
  D3DXVECTOR3              m_kLocalOffsetVector;

  ProcessStateCallback     m_apkProcessStateCallback[CARRIER::CARRIER_STATE_MAX];

  D3DXMATRIXA16            m_kGlobalToLocalTransformMatrix;
  D3DXMATRIXA16            m_kLocalToBodyXMatrix;
  D3DXMATRIXA16            m_kLocalToBodyYMatrix;
  D3DXMATRIXA16            m_kLocalToBodyZMatrix;
  D3DXMATRIXA16            m_kLocalToBodyTransformMatrix;
  D3DXMATRIXA16            m_kLocalToOptimalApproachTransformMatrix;


  //////////////////////////////////////////////////////////////
  // USER AIRCRAFT DATA
  //////////////////////////////////////////////////////////////
  AAVVector3<FLOAT>        m_kGlobalOffsetVector;
  AAVVector3<FLOAT>        m_kGlobalOffsetVectorNormalized;
  AAVVector3<FLOAT>        m_kLocalUserAcOffsetVectorNormalized;
  AAVVector3<FLOAT>        m_kGlobalUserAcDirectionVector;
  FLOAT                    m_fDistanceToUserAircraft;
  DOUBLE                   m_dStartEvaluationTime;
  DOUBLE                   m_dEvaluationTime;
  AAVVector3<FLOAT>        m_kLocalArrestingVector;
  AAVVector3<FLOAT>        m_kLocalArrestingVectorNormalized;
  AAVVector3<FLOAT>        m_kBodyArrestingVectorNormalized;
  AAVVector3<FLOAT>        m_kACLSVector;
  FLOAT                    m_dApproachCosinus;


  VOID                     ProcessStateNone();
  VOID                     ProcessStateIdle();
  VOID                     ProcessStateEnroute();
  VOID                     ProcessStateCatArmed();
  VOID                     ProcessStateCatLaunched();
  VOID                     ProcessStateApproachEvaluation();
  VOID                     ProcessStateFlybyEvaluation();
  VOID                     ProcessStateUserAcOnApproach();
  VOID                     ProcessStateTouchDownEvaluation();
  VOID                     ProcessStateArresting();
  VOID                     ProcessStateTouchAndGoEvaluation();

  UINT                     m_uiThreadSleepPeriod;
};


#endif	/* __CARRIER_MODULE_H__ */
