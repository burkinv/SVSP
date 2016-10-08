#ifndef	__COM_MODULE_H__
#define	__COM_MODULE_H__

#include "__SVSPModule.h"
/**
  COM Gauge module
  - Touch screen gauge.
  - Manual control data entry.
  - COM Radio control.
  - NAV Radio control.
  - IFF control data entry.
  - APP control data entry.
  - HUD/HMD control data entry.
  - Critical Engine Data Indication gauge.
  - Check List assertion failure indication stack.
*/
class COM : public __SVSPModule
{
public:
  static const UINT       CHECK_LIST_FAILURE_STACK_SIZE = 5;
  static const FLOAT      DISPLAY_WINDOW_WIDTH;
  static const FLOAT      DISPLAY_WINDOW_HEIGHT;
  static const AAVArea<FLOAT>  kRTouchRectArea;

                          COM(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual                ~COM();
  virtual VOID            Start();
          VOID            RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  virtual BOOL            EventHandler(DWORD dwEvent, DWORD dwEventData);

protected:
  virtual VOID            Process();

private:
  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_BACKGROUND,
    GEOM_OBJECT_CHECK_LIST_INDICATOR,
    GEOM_OBJECT_CHECK_LIST_WARN_INDICATOR,
    GEOM_OBJECT_TOUCH_RECT,
    GEOM_OBJECT_ACTIVE_TOUCH_RECT,
    GEOM_OBJECT_FRAME_BORDER_1,
    GEOM_OBJECT_FRAME_BORDER_2,
    GEOM_OBJECT_FRAME_CORNERS_1,
    GEOM_OBJECT_FRAME_CORNERS_2,
    GEOM_OBJECT_ACTIVE_FRAME_BORDER_1,
    GEOM_OBJECT_ACTIVE_FRAME_CORNERS_1,
    GEOM_OBJECT_NUM_OBJECTS
  };

  enum TEXTURE_OBJECTS_LIST
  {
    TEXTURE_OBJECT_NONE,
    TEXTURE_OBJECT_R_BUTTON,
    TEXTURE_OBJECT_NUM_OBJECTS
  };

  enum EVENT_ID 
  {
    EVENT_ID_BTN_0         = 1,
    EVENT_ID_BTN_1,
    EVENT_ID_BTN_2, 
    EVENT_ID_BTN_3,
    EVENT_ID_BTN_4,
    EVENT_ID_BTN_5,
    EVENT_ID_BTN_6,
    EVENT_ID_BTN_7,
    EVENT_ID_BTN_8,
    EVENT_ID_BTN_9,
    EVENT_ID_BTN_ENT,
    EVENT_ID_BTN_CSL,

    // Main Menu
    EVENT_ID_BTN_NAV,
    EVENT_ID_BTN_COM,
    EVENT_ID_BTN_IFF,
    EVENT_ID_BTN_TBN,
    EVENT_ID_BTN_APP,
    EVENT_ID_BTN_LHT,
    EVENT_ID_BTN_HUD,

    // COM
    EVENT_ID_BTN_COM_1_1,
    EVENT_ID_BTN_COM_1_2,
    EVENT_ID_BTN_COM_2_1,
    EVENT_ID_BTN_COM_2_2,
    EVENT_ID_BTN_UHF,
    EVENT_ID_BTN_VHF,

    //HUD
    EVENT_ID_SET_HUD_MODE,
    EVENT_ID_SET_HMD_MODE,
    EVENT_ID_INC_SCALE,
    EVENT_ID_DEC_SCALE,
  };

  enum DISPLAY_STATE
  {
    DISPLAY_STATE_NAV = 0,
    DISPLAY_STATE_COM,
    DISPLAY_STATE_IFF,
    DISPLAY_STATE_TBN,
    DISPLAY_STATE_APP,
    DISPLAY_STATE_LHT,
    DISPLAY_STATE_HUD,
    DISPLAY_STATE_MAX,
  };

  static const UINT       NUM_BACKGROUND_VERTICES                    = 4;
  static const UINT       NUM_CHECK_LIST_INDICATOR_VERTICES          = 4;
  static const UINT       NUM_FRAME_BORDER_1_VERTICES                = 14;
  static const UINT       NUM_FRAME_BORDER_2_VERTICES                = 10;
  static const UINT       NUM_FRAME_CORNERS_1_VERTICES               = 6;
  static const UINT       NUM_FRAME_CORNERS_2_VERTICES               = 12;
  static const UINT       NUM_ACTIVE_FRAME_BORDER_1_VERTICES         = 14;
  static const UINT       NUM_ACTIVE_FRAME_CORNERS_1_VERTICES        = 6;
  static const UINT       NUM_ACTIVE_FRAME_SEPARATION_LINE2_VERTICES = 4;

  static       __SVSPTouchRect g_akTouchMapTable[COM::DISPLAY_STATE_MAX][__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE];

  typedef      VOID       (__thiscall COM::*ProcessStateCallback)(VOID);
  typedef      VOID       (__thiscall COM::*RenderStateCallback)(LPD3DXDEVICE pkDevice, D3DXMATRIX &);

  ProcessStateCallback    m_apkProcessStateCallback[COM::DISPLAY_STATE_MAX];
  RenderStateCallback     m_apkRenderStateCallback[COM::DISPLAY_STATE_MAX];

  VOID                    ProcessStateNAV(); //DISPLAY_STATE_NAV,
  VOID                    ProcessStateCOM(); //DISPLAY_STATE_COM,
  VOID                    ProcessStateIFF(); //DISPLAY_STATE_IFF,
  VOID                    ProcessStateTBN(); //DISPLAY_STATE_TBN,
  VOID                    ProcessStateAPP(); //DISPLAY_STATE_APP,
  VOID                    ProcessStateLHT(); //DISPLAY_STATE_LHT,
  VOID                    ProcessStateHUD(); //DISPLAY_STATE_HUD,

  VOID                    RenderStateNAV(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  VOID                    RenderStateCOM(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  VOID                    RenderStateIFF(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  VOID                    RenderStateTBN(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  VOID                    RenderStateAPP(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  VOID                    RenderStateLHT(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  VOID                    RenderStateHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);


  D3DXMATRIXA16           m_kLatitudeIndicatorMatrix;
  D3DXMATRIXA16           m_kLongitudeIndicatorMatrix;

  typedef struct
  {
    const CHAR          *pcFailureName;
    D3DXMATRIXA16        kLeftIndicatorMatrix;
    D3DXMATRIXA16        kRightIndicatorMatrix;
    D3DXVECTOR3          kLeftTextOffsetVector;
    D3DXVECTOR3          kRightTextOffsetVector;
    BOOL                 bBlinkRequired;
  } CHECK_LIST_FAILURE;

  CHECK_LIST_FAILURE      m_akCheckListFailureStack[COM::CHECK_LIST_FAILURE_STACK_SIZE];
  UINT                    m_uiCurrentCheckListFailureStackIndex;
  BOOL                    m_bCheckListFailureStackMatriciesSet;
  D3DXVECTOR3             m_kThrustPercentValueVector;
  D3DXVECTOR3             m_kN1PercentValueVector;
  D3DXVECTOR3             m_kN2PercentValueVector;
  D3DXVECTOR3             m_kEGTValueVector;

};

#endif	/* __COM_MODULE_H__ */
