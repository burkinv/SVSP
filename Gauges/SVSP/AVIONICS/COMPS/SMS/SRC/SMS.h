#ifndef	__SMS_MODULE_H__
#define	__SMS_MODULE_H__

#include <__SVSPModule.h>
#include "MISSILE.h"
#include "MDA.h"



class SMS : public __SVSPModule
{
  friend class MDA;
public:
  static const UINT        MAX_NUM_WEAPONS           = 20;
  static const UINT        MAX_NUM_WEAPON_GROUPS     = 24;
  static const UINT        MAX_NUM_STATIONS          = 20;
  static const UINT        MAX_NUM_UNITS_PER_STATION = 10;

  enum 
  {
    MESSAGE_MASTER_REARM    = (WM_USER + 1304),
    MESSAGE_RELEASE_WEAPON,
    MESSAGE_SELECT_NEXT_WEAPON_GROUP,
    MESSAGE_SELECT_NEXT_WEAPON_IN_GROUP,
    MESSAGE_DELETE_WEAPON,
    MESSAGE_JETTISON_ALL_PAYLOAD,
    MESSAGE_RESET,
  };

  typedef enum 
  {
    SMS_STATE_IDLE,
    SMS_STATE_ARMED,
    SMS_STATE_MAX,
  } STATE;

 enum EVENT_ID {
    EVENT_ID_JETTISON_LOAD   = __SVSPModule::EVENT_ID_MAX,
    EVENT_ID_ROOT_MENU,
  };

  enum  
  {
    DATA_ID_SET_AIRCRAFT_STATION_LOAD_DATA,
  } DATA_ID;

  struct SET_AIRCRAFT_STATION_LOAD_DATA
  {
    UINT                      uiStationLoadWeight[SMS::MAX_NUM_WEAPONS];
  } m_kSetAircraftStationLoadData;

  typedef HRESULT (__thiscall SMS::*ProcessStateCallback)(VOID);

                        SMS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual              ~SMS();
  virtual VOID          Start();
  virtual VOID          Stop();
  virtual VOID          Release();
  virtual HRESULT       Reset(LPD3DXDEVICE pkDevice);
  virtual VOID          RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  virtual VOID          RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  virtual BOOL          EventHandler(DWORD dwEvent, DWORD dwEventData);
  VOID                  AddStore(WEAPON *pkWeapon);
  VOID                  RemoveStore(WEAPON *pkWeapon);
  VOID                  DeleteAllStores();
  VOID                  RemoveAllStores();
  inline WEAPON*        GetSelectedWeapon() const { return m_pkSelectedWeapon; }
  inline BOOL           IsArmed() const { return (m_eState == SMS::SMS_STATE_ARMED); }
  inline STATE          GetState() const { return m_eState; }

protected:
  virtual VOID          Process();
  virtual HRESULT       ProcessStateIdle();        //SMS::SMS_STATE_IDLE,
  virtual HRESULT       ProcessStateArmed();       //SMS::SMS_STATE_ARMED,
  WEAPON::WEAPON_GROUP* GetNextWeaponGroup();
  VOID                  UnarmAllWeaponGroups();
  WEAPON*               GetNextWeapon();
  VOID                  SetState(STATE eState);
  VOID                  ProcessInternalBayLauncherState();
  VOID                  SelectNextWeaponGroup();
  VOID                  CheckWeaponState();



  SMS::STATE            m_eState;
  WEAPON               *m_pkSelectedWeapon;
  WEAPON::WEAPON_GROUP  m_akWeaponGroup[SMS::MAX_NUM_WEAPON_GROUPS];
  ProcessStateCallback  m_apkProcessStateCallback[SMS::SMS_STATE_MAX];

private:
  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_BACKGROUND, 
    GEOM_OBJECT_WEAPON_IMAGE,
    GEOM_OBJECT_SELECTED_WEAPON_IMAGE,
    GEOM_OBJECT_WEAPON_DESIGNATOR, 
    GEOM_OBJECT_TOUCH_RECT,
    GEOM_OBJECT_FRAME_BORDER,
    GEOM_OBJECT_FRAME_CORNERS,
    GEOM_OBJECT_ACTIVE_FRAME_BORDER,
    GEOM_OBJECT_ACTIVE_FRAME_CORNERS,

    GEOM_OBJECT_NUM_OBJECTS
  };

  enum TEXTURE_OBJECTS_LIST
  {
    TEXTURE_OBJECT_BACKGROUND,
    TEXTURE_OBJECT_UP_SIDE_BUTTON,
    TEXTURE_OBJECT_LT_SIDE_BUTTON,
    TEXTURE_OBJECT_RT_SIDE_BUTTON,
    TEXTURE_OBJECT_BM_SIDE_BUTTON,

    TEXTURE_OBJECT_NUM_OBJECTS
  };

  enum INTERNAL_BAY_LAUNCHER_LIST
  {
    INTERNAL_BAY_LAUNCHER_L1,
    INTERNAL_BAY_LAUNCHER_L2,
    INTERNAL_BAY_LAUNCHER_R1,
    INTERNAL_BAY_LAUNCHER_R2,

    INTERNAL_BAY_LAUNCHER_MAX
  };

  static const UINT       NUM_BACKGROUND_VERTICES                  = 4;
  static const UINT       NUM_SELECTED_WEAPON_GROUP_FRAME_VERTICES = 5;
  static const UINT       NUM_WEAPON_IMAGE_VERTICES                = 4;
  static const UINT       NUM_SELECTED_WEAPON_IMAGE_VERTICES       = 4;
  static const UINT       NUM_WEAPON_DESIGNATOR_VERTICES           = 20;
  static const UINT       NUM_FRAME_BORDER_VERTICES                = 10;
  static const UINT       NUM_FRAME_CORNERS_VERTICES               = 12;
  static const UINT       NUM_ACTIVE_FRAME_BORDER_VERTICES         = 10;
  static const UINT       NUM_ACTIVE_FRAME_CORNERS_VERTICES        = 12;

  enum DISPLAY_STATE
  {
    DISPLAY_STATE_MAIN = __SVSPModule::DISPLAY_STATE_MAX,
    DISPLAY_STATE_MAX
  };

  static __SVSPTouchRect g_akTouchMapTable[SMS::DISPLAY_STATE_MAX][__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE];

  typedef struct
  {
    const UINT  uiStationId;
    const CHAR* pcName;
          FLOAT dValue;
          FLOAT dValueToGain;
  } PART_ANIMATION_TAG;

  ////////////////////////////////////////////////////////////////////
  // INTERNAL BAYS CARRIAGE 
  ////////////////////////////////////////////////////////////////////
 static const FLOAT           INTERNAL_BAY_LAUNCHER_ANIMATION_VELOCITY;
 static PART_ANIMATION_TAG   m_akInternalBayLauncher[SMS::INTERNAL_BAY_LAUNCHER_MAX];
};


#endif	/* __SMS_MODULE_H__ */
