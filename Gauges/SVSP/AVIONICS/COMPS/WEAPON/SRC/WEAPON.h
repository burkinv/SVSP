#ifndef	__WEAPON_MODULE_H__
#define	__WEAPON_MODULE_H__

#include <__SVSPModule.h>
#include "__SVSPAvionics.h"

class WEAPON : public __SVSPModule
{
public:
  enum 
  {
    MESSAGE_ARM = (WM_USER + 3304),
    MESSAGE_UNARM,
    MESSAGE_REARM,
    MESSAGE_UNLOCK,
    MESSAGE_LAUNCH,
    MESSAGE_TEST,
  };

  typedef enum 
  {
    WEAPON_STATE_IDLE,
    WEAPON_STATE_ARMED,
    WEAPON_STATE_ACQUISITION,
    WEAPON_STATE_LOCKED,
    WEAPON_STATE_LAUNCH,
    WEAPON_STATE_WAIT_LAUNCH_ACKNOWLEDGE,
    WEAPON_STATE_PURSUIT,
    WEAPON_STATE_MISSED,
    WEAPON_STATE_DETONATE,
    WEAPON_STATE_NONE,
    WEAPON_STATE_MAX
  } STATE;

  enum 
  {
    PROCESS_UPDATE_WEAPON_STATE_DATA     = 0x1,
  };

  enum
  {
    REQUEST_ID_CREATE_OBJECT           = 0x5,
    REQUEST_ID_REMOVE_OBJECT,
    REQUEST_ID_RELEASE_CONTROL,
    REQUEST_ID_READ_OBJECT_STATE_DATA,
    REQUEST_ID_SET_OBJECT_STATE_DATA,
    REQUEST_ID_READ_TARGET_OBJECT_DATA,
    REQUEST_ID_SET_AUX_OBJECT_DATA,
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

    DATA_ID_READ_TARGET_OBJECT_DATA,
    DATA_ID_SET_AUX_OBJECT_DATA,

  } DATA_ID;

  struct READ_OBJECT_STATE_DATA
  {
    FLOAT                    bOnGround;
    FLOAT                    eCrashFlag;
    FLOAT                    dAGLAltitude;
    AAVVector3<FLOAT>       kBodyAttitude;
    AAVVector3<FLOAT>       kBodyAngularVelocity;
    AAVVector3<FLOAT>       kBodyVelocity;
    __SVSPGeoCoordinates3<FLOAT>    kLocation;
  } m_kData;

  struct SET_OBJECT_STATE_DATA
  {
    AAVVector3<FLOAT>       kBodyAngularVelocity;
    AAVVector3<FLOAT>       kBodyVelocity;
    //AAVVector3<FLOAT>       kBodyAttitude;
  } m_kSetObjectStateData;


  struct WEAPON_GROUP
  {
    static const UINT MAX_NUM_WEAPONS = 8;

    inline WEAPON      *GetNextIdleWeapon()
    {
      if(uiAmount != 0)
      {
        for(UINT i = 0; i < WEAPON_GROUP::MAX_NUM_WEAPONS; ++i)
        {
          WEAPON *pkWeapon = apkWeapon[i];
          if((pkWeapon != NULL) && (pkWeapon->GetState() == WEAPON::WEAPON_STATE_IDLE))
          {
            return pkWeapon;
          }
        }
      }
      return NULL;
    }

    inline VOID Release()
    {
      for(UINT i = 0; i < WEAPON_GROUP::MAX_NUM_WEAPONS; ++i)
      {
        WEAPON *pkWeapon = apkWeapon[i];
        if(pkWeapon != NULL)
        {
          pkWeapon->Release();
        }
      }
    }

    inline HRESULT Reset(LPD3DXDEVICE pkDevice)
    {
      HRESULT hResult = S_OK;
      for(UINT i = 0; i < WEAPON_GROUP::MAX_NUM_WEAPONS; ++i)
      {
        WEAPON *pkWeapon = apkWeapon[i];
        if(pkWeapon != NULL)
        {
          hResult = pkWeapon->Reset(pkDevice);
        }
      }
      return hResult;
    }

    inline VOID RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
    {
      for(UINT i = 0; i < WEAPON_GROUP::MAX_NUM_WEAPONS; ++i)
      {
        WEAPON *pkWeapon = apkWeapon[i];
        if(pkWeapon != NULL)
        {
          pkWeapon->RenderOnHUD(pkDevice, rkOPMatrix);
        }
      }
    }

    inline const CHAR* GetName() const 
    {
      return acName.getCStr();
    }

    inline UINT GetAmount() const 
    {
      return uiAmount;
    }

    __SVSPString<CHAR>   acName;
    UINT                eType;
    UINT                uiAmount;
    WEAPON             *apkWeapon[WEAPON_GROUP::MAX_NUM_WEAPONS];
  };

  typedef                    VOID (__thiscall WEAPON::*ProcessStateCallback)(VOID);

                             WEAPON(CHAR *pcName, CHAR *pcType, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude, 
                                    LPPD3DXGEOMETRYOBJECT ppkGeomObjectsList = NULL, UINT uiNumGeomObjects = 0, LPPD3DXTEXTUREOBJECT ppkTextureObjectsList = NULL, UINT uiNumTextureObjects = 0);
  virtual                   ~WEAPON();
  virtual VOID               Render(D3DXMATRIX &rkOPMatrix)      = 0;
  virtual VOID               RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix) = 0;
  //virtual VOID               Render(D3DXMATRIX &rkOPMatrix, FLOAT fScale);
  virtual HRESULT            SetState(WEAPON::STATE eState);
  inline STATE               GetState() const { return m_eState; }
  virtual LPD3DXTEXTUREOBJECT GetWeaponTexture() const;
  //inline D3DXVECTOR3       GetVector() const { return D3DXVECTOR3(m_kVector.x, m_kVector.y, m_kVector.z); }
  VOID                       ProcessLaunchState(AAVVector3<FLOAT> kGlobMissileVector, AAVVector3<FLOAT> kGlobMissileVelocityVector, FLOAT fMissileVelocity);
  inline VOID                SetWeaponGroup(WEAPON::WEAPON_GROUP *pkGroup) { m_pkWeaponGroup = pkGroup; }
  inline WEAPON_GROUP*       GetWeaponGroup() const { return m_pkWeaponGroup; }
  VOID                       SetStaticModelVisibilityVar(ID kStaticModelVisibilityVar) { m_kStaticModelVisibilityVar = kStaticModelVisibilityVar; }
  ID                         GetStaticModelVisibilityVar() const { return m_kStaticModelVisibilityVar; }
  const AAVVector3<FLOAT>*   GetPilonVector() const { return &m_kPilonVector; }
  virtual VOID               SetTargetID(DWORD dwObjectID);
  inline DWORD               GetTargetID() const { return m_kTarget.dwObjectID; }
  inline const CHAR*         GetType()  const { return m_acType.getCStr(); }
  inline const UINT          GetStationId() const { return m_uiStationID; }
  inline const UINT          GetUnitId()    const { return m_uiUnitID; }
  inline const UINT          GetWeight()    const { return m_uiWeight; }
  virtual VOID               Start();
  virtual VOID               Stop();

protected:
  virtual VOID               Process();
  virtual VOID               DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData) = 0;
  friend VOID CALLBACK       WEAPON__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);

  ProcessStateCallback       m_apkProcessStateCallback[WEAPON::WEAPON_STATE_MAX];

  virtual VOID               ProcessStateIdle()                  =0; //WEAPON_STATE_IDLE,
  virtual VOID               ProcessStateArmed()                 =0; //WEAPON_STATE_ARMED,
  virtual VOID               ProcessStateAcquisition()           =0; //WEAPON_STATE_ACQUISITION,
  virtual VOID               ProcessStateLocked()                =0; //WEAPON_STATE_LOCKED,
  virtual VOID               ProcessStateLaunch()                =0; //WEAPON_STATE_LAUNCH,
  virtual VOID               ProcessStateWaitLaunchAcknowledge() =0; //WEAPON_STATE_WAIT_LAUNCH_ACKNOWLEDGE,
  virtual VOID               ProcessStatePursuit()               =0; //WEAPON_STATE_PURSUIT,
  virtual VOID               ProcessStateMissed()                =0; //WEAPON_STATE_MISSED,
  virtual VOID               ProcessStateDetonate()              =0; //WEAPON_STATE_DETONATE,
  virtual VOID               ProcessStateNone()                  =0; //WEAPON_STATE_NONE

  ////////////////////////////////////////////////////////////////////////////////////
  // WEAPON SPECIFICATION DATA
  ////////////////////////////////////////////////////////////////////////////////////
  __SVSPString<CHAR>          m_acType;
  UINT                       m_uiWeight;

  ///////////////////////////////////////////////////////////////
  // FSX SPECIFIC OBJECT DATA
  ///////////////////////////////////////////////////////////////
  DWORD                      m_dwObjectID;
  UINT                       m_uiStationID;
  UINT                       m_uiUnitID;
  ID                         m_kStaticModelVisibilityVar;

  ///////////////////////////////////////////////////////////////
  // WEAPON POSITION AND ORIENTATION DATA
  ///////////////////////////////////////////////////////////////
  AAVVector3<FLOAT>         m_kPilonVector;
  AAVVector3<FLOAT>         m_kPilonAttitude;
  D3DXMATRIXA16             m_kGlobalToLocalTransformMatrix;
  D3DXMATRIXA16             m_kLocalToGlobalTransformMatrix;
  D3DXMATRIXA16             m_kLocalToBodyTransformMatrix;
  D3DXMATRIXA16             m_kBodyToLocalTransformMatrix;

  ///////////////////////////////////////////////////////////////
  // WEAPON STATE DATA
  ///////////////////////////////////////////////////////////////
  WEAPON::WEAPON_GROUP     *m_pkWeaponGroup;
  STATE                     m_eState;
  DOUBLE                    m_dReleaseTime;
  DOUBLE                    m_dTotalAirborneTime;
  AAVVector3<FLOAT>         m_kBodyThrust;
  AAVVector3<FLOAT>         m_kBodyGravityVector;
  AAVVector3<FLOAT>         m_kBodyAirResistance;
  AAVVector3<FLOAT>         m_kBodyAcceleration;
  AAVVector3<FLOAT>         m_kBodyAngularVelocity;
  AAVVector3<FLOAT>         m_kBodyVelocity;
  FLOAT                     m_fAirspeed;
  FLOAT                     m_fAngularVelocity;

  ///////////////////////////////////////////////////////////////
  // TARGET DATA
  ///////////////////////////////////////////////////////////////
  OBJECT                    m_kTarget;

};

#endif	/* __WEAPON_MODULE_H__ */
