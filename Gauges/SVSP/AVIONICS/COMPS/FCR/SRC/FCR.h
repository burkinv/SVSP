#ifndef	__FCR_MODULE_H__
#define	__FCR_MODULE_H__

#include "__SVSPModule.h"

/**
  Fire Control RADAR module.
  - ADD/REMOVE Object event based Sim trafic handling.
  - 16K object table limit.
  - Single object at a time swap-ring handling.
  - AWACS (Full range 360 degrees) and RADAR Frustum confined object selection.
  - Visual Target aquisition.
  - Single Track Locked target tracking.
  - Range selection.
  - (HDD) Touch screen and RADAR gauge indication.
*/
class FCR : public __SVSPModule
{
  friend class MISSILE;
  friend class HUD;

public:
  static const UINT  MAX_NUM_OBJECTS                      = 0xFFFF;
  static const UINT  MAX_NUM_LOCKED_OBJECTS               = 4;
  static const UINT  MAX_NUM_RADAR_RANGES                 = 8;
  static const FLOAT PIXELS_IN_NAUTICAL_MILE;
  static const FLOAT TEN_MILES_CIRCLE_RANGE;
  static const FLOAT INITIAL_FCR_RANGE_MILES;
  static const FLOAT INITIAL_NUM_PIXELS_IN_NAUTICAL_MILE;
  static const FLOAT DISPLAY_WINDOW_WIDTH;
  static const FLOAT DISPLAY_WINDOW_HEIGHT;
  static const FLOAT MAX_LOCK_DISTANCE;
  static const FLOAT MIN_LOCK_DISTANCE;
  static const FLOAT AQUISITION_ASPECT_CAGED;
  static const FLOAT AQUISITION_ASPECT_UNCAGED;

  enum 
  {
    MESSAGE_ON_VISUAL_RANGE = (WM_USER + 1304),
    MESSAGE_ON_BEYOND_VISUAL_RANGE,
    MESSAGE_OFF,
    MESSAGE_TOGGLE_UNCAGE_FCR,
    MESSAGE_TOGGLE_LOCK_ON_NEAREST_TARGET,
    MESSAGE_SET_RADAR_RANGE_INDEX,
  };

  typedef enum 
  {
    FCR_STATE_OFF,
    FCR_STATE_ON,
    FCR_STATE_LOCKED,
    FCR_STATE_MAX,
  } STATE;

  enum EVENT_ID 
  {
    EVENT_ID_OBJECT_ADDED   = __SVSPModule::EVENT_ID_MAX,
    EVENT_ID_OBJECT_REMOVED, 
    EVENT_ID_HDG,
    EVENT_ID_RANGE_DEC,
    EVENT_ID_RANGE_INC,
    EVENT_ID_ZOOM_IN,
    EVENT_ID_ZOOM_OUT,
    EVENT_ID_ROOT_MENU,
    EVENT_ID_01,
    EVENT_ID_02,
    EVENT_ID_03,
    EVENT_ID_04,
    EVENT_ID_05,
    EVENT_ID_06,
    EVENT_ID_07,
    EVENT_ID_08,
    EVENT_ID_09,
    EVENT_ID_10,
  };

  enum OBJECT_DATA_REQUEST_ID
  {
    AI_OBJECT_DATA_REQUEST_ID,
  };

  enum OBJECT_DATA_ID
  {
    AI_OBJECT_DATA_ID,
  };

  enum 
  {
    MESSAGE_INVALIDATE_TARGET    = (WM_USER + 3304),
  };

  typedef HRESULT (__thiscall FCR::*ProcessStateCallback)(VOID);

                            FCR(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual                  ~FCR();
          VOID              RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
          VOID              RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  virtual BOOL              EventHandler(DWORD dwEvent, DWORD dwEventData);
  inline FCR::STATE         GetState() const { return m_eState; }
  inline OBJECT*            GetTarget() const { return m_pkTarget; }
  inline DWORD              GetTargetID() const { return (m_pkTarget != NULL) ? m_pkTarget->dwObjectID : 0xFFFFFFFF; }
  OBJECT*                   SelectNearestTarget();
  inline const D3DXVECTOR3* GetLocalTargetDirectionVector() const { return &m_kLocalTargetDirectionNormalizedVector; }
         VOID               SetRadarRange(UINT uiRadarRangeIndex);
  inline UINT               GetRadarRangeIndex() const { return m_uiCurrentRadarRangeIndex; }

protected:
  virtual VOID              Start();
  virtual VOID              Process();
  virtual HRESULT           ProcessStateOff();               //FCR::FCR_STATE_OFF,
  virtual HRESULT           ProcessStateOn();                //FCR::FCR_STATE_ON,
  virtual HRESULT           ProcessStateLocked();            //FCR::FCR_STATE_LOCKED,
  virtual VOID              DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);
  friend VOID CALLBACK      FCR__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);
  VOID                      AddObject(DWORD dwObjectId);
  VOID                      RemoveObject(DWORD dwObjectId);
  VOID                      RotateAntenaTowardViewDirection(LPD3DXDEVICE pkDevice);
  VOID                      ToggleUncageRadar();
  VOID                      UpdateTargetBearingData();

  FCR::STATE                m_eState;
  ProcessStateCallback      m_apkProcessStateCallback[FCR::FCR_STATE_MAX];

  // VISUAL TARGET ACQUISITION
  D3DXMATRIXA16             m_kLocalAntenaMatrix;
  D3DXVECTOR3               m_kLocalAntenaDirectionNormalizedVector;
  D3DXVECTOR3               m_kLocalTargetDirectionVector;
  D3DXVECTOR3               m_kLocalTargetDirectionNormalizedVector;
  // FCR TARGET ACQUISITION
  D3DXMATRIX               *m_pkLocalRadarDirectionMatrix;
  FLOAT                     m_fMinObjectAquisitionAspect;
  //
  D3DXVECTOR3               m_kTargetAirspeedValueVector;
  D3DXVECTOR3               m_kTargetAltitudeValueVector;

private:
  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_BACKGROUND,
    GEOM_OBJECT_CENTER_POINT_DESIGNATOR,
    GEOM_OBJECT_RANGE,
    GEOM_OBJECT_HSI_SCALE,
    GEOM_OBJECT_TARGET_DESIGNATOR,
    GEOM_OBJECT_SELECTED_TARGET_DESIGNATOR,
    GEOM_OBJECT_FCR_CONE_DESIGNATOR,
    GEOM_OBJECT_FCR_HEADING_DESIGNATOR,
    GEOM_OBJECT_TIRIS_DESIGNATOR,
    GEOM_OBJECT_LOCKED_TARGET_ASPECT,
    GEOM_OBJECT_LOCKED_TARGET_DESIGNATOR,
    GEOM_OBJECT_LOCKED_TARGET_DESIGNATOR_2,
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
    TEXTURE_OBJECT_TARGET_DESIGNATOR,
    TEXTURE_OBJECT_FCR_HEADING_DESIGNATOR,
    TEXTURE_OBJECT_UP_SIDE_BUTTON,
    TEXTURE_OBJECT_LT_SIDE_BUTTON,
    TEXTURE_OBJECT_RT_SIDE_BUTTON,
    TEXTURE_OBJECT_BM_SIDE_BUTTON,

    TEXTURE_OBJECT_NUM_OBJECTS
  };

  enum DISPLAY_STATE
  {
    DISPLAY_STATE_MAIN = __SVSPModule::DISPLAY_STATE_MAX,
    DISPLAY_STATE_MAX
  };

  static const UINT       NUM_BACKGROUND_VERTICES                 = 4;
  static const UINT       NUM_TOUCH_RECT_VERTICES                 = 4;
  static const UINT       NUM_CENTER_POINT_DESIGNATOR_VERTICES    = 8;
  static const UINT       NUM_RANGE_VERTICES                      = 36 + 1;
  static const UINT       NUM_HSI_SCALE_VERTS                     = 24*2;
  static const UINT       NUM_TARGET_DESIGNATOR_VERTICES          = 4;
  static const UINT       NUM_SELECTED_TARGET_DESIGNATOR_VERTICES = 4;
  static const UINT       NUM_FCR_CONE_DESIGNATOR_VERTICES        = 72;
  static const UINT       NUM_FCR_HEADING_DESIGNATOR_VERTICES     = 4;
  static const UINT       NUM_TIRIS_DESIGNATOR_VERTICES           = 72;
  static const UINT       NUM_LOCKED_TARGET_ASPECT_VERTICES       = 72 + 4;
  static const UINT       NUM_LOCKED_TARGET_DESIGNATOR_VERTICES   = 10;
  static const UINT       NUM_FRAME_BORDER_VERTICES                  = 10;
  static const UINT       NUM_FRAME_CORNERS_VERTICES                 = 12;
  static const UINT       NUM_ACTIVE_FRAME_BORDER_VERTICES           = 10;
  static const UINT       NUM_ACTIVE_FRAME_CORNERS_VERTICES          = 12;
  static const UINT       NUM_ACTIVE_FRAME_SEPARATION_LINE2_VERTICES = 4;

  static __SVSPTouchRect g_akTouchMapTable[FCR::DISPLAY_STATE_MAX][__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE];
         VOID             InitObjectsList();


  INT                     m_iNumObjects;
  OBJECT                  m_akObjects[FCR::MAX_NUM_OBJECTS];
  OBJECT                 *m_pkTarget;
  OBJECT                 *m_pkProcessedObject;
  FCR::HEADING            m_akHSIHeading[12];
  FLOAT                   m_fCurrentZoomFactor;
  FLOAT                   m_fCurrentScaleFactor;
  FLOAT                   m_fCurrentRadarRangeMiles;
  UINT                    m_uiCurrentRadarRangeIndex;
  FLOAT                   m_fCurrentNumPixelsInNauticalMile;
  RECT                    m_kClippRect;
  D3DXMATRIXA16           m_kHSIRotationMatrix;
  D3DXMATRIXA16           m_kHSIRotationTransposedMatrix;

  D3DXVECTOR3             m_kCurrentRadarRangeIndicatorVector;
  D3DXVECTOR3             m_kGTSIndicatorVector;
  D3DXVECTOR3             m_kSPDIndicatorVector;
  D3DXVECTOR3             m_kHDGIndicatorVector;
  D3DXVECTOR3             m_kRSTATIndicatorVector;
  D3DXVECTOR3             m_kALTIndicatorVector;
  D3DXVECTOR3             m_kRNGIndicatorVector;
  D3DXVECTOR3             m_kLATLONIndicatorVector;

  static const FLOAT      g_afRadarRangeTables[FCR::MAX_NUM_RADAR_RANGES];


};

#endif	/* __FCR_MODULE_H__ */
