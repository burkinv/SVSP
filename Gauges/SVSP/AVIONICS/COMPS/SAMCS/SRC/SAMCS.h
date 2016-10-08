#ifndef	__SAMCS_MODULE_H__
#define	__SAMCS_MODULE_H__

#include "__SVSPModule.h"
#include "__SVSPGeoCoordinates3.h"

class SAMCS : public __SVSPModule
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
    DATA_ID_SET_OBJECT_STATE_DATA,
    DATA_ID_SET_AUX_OBJECT_STATE_DATA,
  } DATA_ID;

  typedef enum 
  {
    STATE_ID_NONE,
    STATE_ID_IDLE,
    STATE_ID_ARMED,
    STATE_ID_LOCKED,
    STATE_ID_MAX,
  } STATE;

  struct READ_OBJECT_STATE_DATA
  {
    AAVVector3<FLOAT>       kBodyAttitude;
    AAVVector3<FLOAT>       kBodyAngularVelocity;
    AAVVector3<FLOAT>       kBodyVelocity;
    __SVSPGeoCoordinates3<FLOAT>    kLocation;
  } m_kReadObjectStateData;

  struct SET_OBJECT_STATE_DATA
  {
    AAVVector3<FLOAT>       kBodyAngularVelocity;
    AAVVector3<FLOAT>       kBodyVelocity;
  } m_kSetObjectStateData;

public:
  enum 
  {
    MESSAGE_MASTER_ARM = (WM_USER + 1304),
    MESSAGE_MASTER_UNARM,
    MESSAGE_MASTER_REARM,
    MESSAGE_RESET,
  };
                             SAMCS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, CHAR *pcName, __SVSPGeoCoordinates3<FLOAT> *pkLocation, AAVVector3<FLOAT> *pkAttitude);
  virtual                   ~SAMCS();

protected:
  typedef                    VOID (__thiscall SAMCS::*ProcessStateCallback)(VOID);
  ProcessStateCallback       m_apkProcessStateCallback[SAMCS::STATE_ID_MAX];

  virtual VOID               Start();
  virtual VOID               Process();
  virtual VOID               DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);
  VOID                       ProcessStateNone();
  VOID                       ProcessStateIdle();
  VOID                       ProcessStateArmed();
  VOID                       ProcessStateLocked();

  ///////////////////////////////////////////////////////////////
  // FSX SPECIFIC OBJECT DATA
  ///////////////////////////////////////////////////////////////
  DWORD                      m_dwObjectID;
  UINT                       m_uiPilonID;
  ID                         m_kStaticModelVisibilityVar;

  ///////////////////////////////////////////////////////////////
  // SAMCS POSITION AND ORIENTATION DATA
  ///////////////////////////////////////////////////////////////
  SAMCS::STATE               m_eState;
  __SVSPGeoCoordinates3<FLOAT>     m_kLocation;
  AAVVector3<FLOAT>        m_kAttitude;
};


#endif	/* __SAMCS_MODULE_H__ */
