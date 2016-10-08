#ifndef	__AIP_MODULE_H__
#define	__AIP_MODULE_H__

#include "__SVSPModule.h"
#include "__SVSPGeoCoordinates3.h"

class AIP : public __SVSPModule
{
  friend class MDA;

  enum
  {
    REQUEST_ID_CREATE_OBJECT           = 0x5,
    REQUEST_ID_REMOVE_OBJECT,
    REQUEST_ID_RELEASE_CONTROL,
    REQUEST_ID_READ_OBJECT_STATE_DATA,
    REQUEST_ID_SET_OBJECT_STATE_DATA,
  } REQUEST_ID;

  enum
  {
    EVENT_ID_OBJECT_ADDED,
    EVENT_ID_OBJECT_REMOVED,
  } EVENT_ID;

  enum  
  {
    DATA_ID_READ_OBJECT_STATE_DATA,
    DATA_ID_SET_OBJECT_VELOCITY_DATA,
    DATA_ID_SET_OBJECT_GEAR_HANDLE,
    DATA_ID_SET_OBJECT_PL,
    DATA_ID_SET_OBJECT_ET,
    DATA_ID_SET_AUX_OBJECT_STATE_DATA,
  } DATA_ID;


  struct READ_OBJECT_STATE_DATA
  {
    FLOAT                    bOnGround;
    AAVVector3<FLOAT>       kBodyAttitude;
    AAVVector3<FLOAT>       kBodyAngularVelocity;
    AAVVector3<FLOAT>       kBodyVelocity;
    __SVSPGeoCoordinates3<FLOAT>    kLocation;

    FLOAT                    fIndicatedAirspeed;
    FLOAT                    dTotalWeight;
    FLOAT                    dGearPosition;

    FLOAT                    dPL;
    FLOAT                    dET;
    FLOAT                    dThrust;
  } m_kData;

  struct SET_OBJECT_VELOCITY_DATA
  {
    AAVVector3<FLOAT>       kBodyAngularVelocity;
    //AAVVector3<FLOAT>       kBodyVelocity;
  } m_kSetObjectVelocityData;

  struct SET_OBJECT_GEAR_HANDLE
  {
    INT                      bGearHandle;
  } m_kSetObjectGearHandle;

  struct SET_OBJECT_PL
  {
    FLOAT                    dPL;
  } m_kSetObjectPL;

  struct SET_OBJECT_ET
  {
    FLOAT                    dET;
  } m_kSetObjectET;

public:
  typedef enum 
  {
    STATE_ID_NONE,
    STATE_ID_IDLE,
    STATE_ID_TAXING,
    STATE_ID_TAKEOFF,
    STATE_ID_ENROUTE,
    STATE_ID_APPROACH,
    STATE_ID_LANDING,
    STATE_ID_ARMED,
    STATE_ID_LOCKED,
    STATE_ID_MAX,
  } STATE;

                             AIP(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, CHAR *pcName, __SVSPGeoCoordinates3<FLOAT> *pkLocation, AAVVector3<FLOAT> *pkAttitude);
  virtual                   ~AIP();
  HRESULT                    Create();
  HRESULT                    Destroy();
  HRESULT                    SetState(AIP::STATE eState);
  HRESULT                    SetAltitude(FLOAT fAltitude);

protected:
  enum 
  {
    MESSAGE_CREATE = (WM_USER + 1304),
    MESSAGE_DESTROY,
    MESSAGE_MASTER_ARM,
    MESSAGE_MASTER_UNARM,
    MESSAGE_MASTER_REARM,
    MESSAGE_RESET,
  };

  typedef                    VOID (__thiscall AIP::*ProcessStateCallback)(VOID);
  ProcessStateCallback       m_apkProcessStateCallback[AIP::STATE_ID_MAX];

  HRESULT                    CreateObject();
  HRESULT                    RemoveObject();
  VOID                       AdjustEnginePower(FLOAT fReferenceSpeed);
  VOID                       SetObjectGearData(INT bGearHandle);
  VOID                       SetObjectVelocityData();
  VOID                       SetObjectPL(FLOAT fPL);
  VOID                       SetObjectET(FLOAT fET);

  virtual VOID               Start();
  virtual VOID               Process();
  virtual VOID               DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);
  friend VOID CALLBACK       AIP__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);
  VOID                       ProcessStateNone();
  VOID                       ProcessStateIdle();
  VOID                       ProcessStateTaxing();
  VOID                       ProcessStateTakeoff();
  VOID                       ProcessStateEnroute();
  VOID                       ProcessStateApproach();
  VOID                       ProcessStateLanding();

  ///////////////////////////////////////////////////////////////
  // FSX SPECIFIC OBJECT DATA
  ///////////////////////////////////////////////////////////////
  DWORD                      m_dwObjectID;
  UINT                       m_uiPilonID;
  ID                         m_kStaticModelVisibilityVar;

  ///////////////////////////////////////////////////////////////
  // AIP POSITION AND ORIENTATION DATA
  ///////////////////////////////////////////////////////////////
  AIP::STATE                m_eState;
  __SVSPGeoCoordinates3<FLOAT>     m_kLocation;
  AAVVector3<FLOAT>        m_kAttitude;

  FLOAT                     m_fReferenceSpeed;
  FLOAT                     m_fReferenceAltitude;
  FLOAT                     m_dMass;
  AAVVector3<FLOAT>        m_kBodyVelocity;
  AAVVector3<FLOAT>        m_kBodyAngularVelocity;
  AAVVector3<FLOAT>        m_kBodyAcceleration;
};


#endif	/* __AIP_MODULE_H__ */
