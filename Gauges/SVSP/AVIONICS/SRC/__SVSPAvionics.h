#ifndef __SVSP_AVIONICS_H__
#define __SVSP_AVIONICS_H__

#include <__SVSPModule.h>

class __SVSPAvionics : public __SVSPModule
{
public:
  enum 
  {
    AVIONICS_MODULE_AC      =  0, // GENERAL-AIRCRAFT-SYSTEMS MODULE                                (DEFAULT INSTANCE)
    AVIONICS_MODULE_HOTAS   =  1, // HANDS-ON-THROTTLE-AND-STICK MODULE
    AVIONICS_MODULE_ISS     =  2, // INTERNAL-SOUND-SERVER MODULE
    AVIONICS_MODULE_FLCS    =  3, // FLIGHT CONTROL SYSTEM MODULE
    AVIONICS_MODULE_ECS     =  4, // ENGINE CONTROL SYSTEM MODULE
    AVIONICS_MODULE_SMS     =  5, // STORES-MANAGEMENT-SYSTEM MODULE
    AVIONICS_MODULE_MDA     =  6, // MISSION-DATA-AUGMENTATION MODULE
    AVIONICS_MODULE_VWS     =  7, // VOCAL-WARNING-SYSTEM MODULE
    AVIONICS_MODULE_FCR     =  8, // SINGLE-TARGET-TRACK-AIRBORNE-FIRE-CONTROL-RADAR MODULE         (AGP-87 STT MODE)
    AVIONICS_MODULE_VPM     =  9, // VIEW-POINT-MANAGEMENT MODULE
    AVIONICS_MODULE_WPN     = 10, // STEER-POINT-NAVIGATION MODULE
    AVIONICS_MODULE_EICAS   = 11, // ENGINE-INDICATION MODULE (OFF)
    AVIONICS_MODULE_COM     = 12, // DATA-ENTRY-CONSOLE (RADIO-COM) MODULE
    AVIONICS_MODULE_SIM     = 13, // INTERNAL-SIMULATOR MODULE
    AVIONICS_MODULE_CARRIER = 14, // AIRCRAFT-CARRIER-CONTROL-AND-ON-DECK-LANDING-ASSISTANCE MODULE (INSTANCIATED FROM MDA)
    AVIONICS_MODULE_AIP     = 15, // ARTIFICIAL-INTELLIGENCE-PILOT MODULE                           (INSTANCIATED FROM MDA)
    AVIONICS_MODULE_HUD     = 16, // HEADS-UP-DISPLAY MODULE
    AVIONICS_MODULE_MFD     = 17, // MULTI-FUNCTIONAL DISPLAY MODULE
    AVIONICS_MODULE_MPCD    = 18, // MULTI-PURPOSE COLOR DISPLAY MODULE (F/A-18AC)

    AVIONICS_MODULE_COUNT
  } eModuleName;

  enum {
     EVENT_ID_MENU_ITEM_AVIONICS_SELECTED,
     EVENT_ID_MENU_ITEM_LOAD_MISSION_DATA_SELECTED,
     EVENT_ID_MENU_ITEM_LOAD_WEAPONS_DATA_SELECTED,
     EVENT_ID_SIM_START,
     EVENT_ID_SIM_STOP,
     EVENT_ID_SIM_PAUSED,
     EVENT_ID_SIM_UNPAUSED,
     EVENT_ID_SIM_CRASHED,
     EVENT_ID_SIM_CRASHED_RESET,
  } EVENT_ID;

  enum
  {
    REQUEST_ID_READ_OBJECT_STATE_DATA,
  } REQUEST_ID;

  enum  
  {
    DATA_ID_READ_OBJECT_STATE_DATA,
  } DATA_ID;

  enum
  {
    AVIONICS_MENU,
  } GROUP_ID;

  enum
  {
    PAUSED_FLAG          = 0x1,
    CRASHED_FLAG         = 0x2,
    SLEW_FLAG            = 0x4,
    STOP_FLAG            = 0x8,
  } SIMULATION_STATE_FLAG;

  struct READ_OBJECT_STATE_DATA
  {
    BOOL            bSlewActive;
    DOUBLE          dAbsoluteTime;
  } m_kReadObjectStateData;

  const static DWORD         g_adwModuleThreadSleepMilisecondsTables[__SVSPAvionics::AVIONICS_MODULE_COUNT];
  static bool                g_bCautionClockEdgeUP;
  inline friend VOID CALLBACK GetTimerProc (HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime) { g_bCautionClockEdgeUP = !g_bCautionClockEdgeUP; }
  static bool                g_bWarnClockEdgeUP;
  inline friend VOID CALLBACK WarnTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime) { g_bWarnClockEdgeUP = !g_bWarnClockEdgeUP; }

                             __SVSPAvionics(HWND hWnd);
  virtual                   ~__SVSPAvionics();
          VOID               Start();
          VOID               Stop();
  inline  HWND               GetHWND() const { return m_hWnd; }
  inline __SVSPModule*       GetModule(UINT uiModuleNum) const { /* AAV_ASSERT_RETURN(uiModuleNum < __SVSPAvionics::AVIONICS_MODULE_COUNT, NULL);*/ return m_apkModules[uiModuleNum]; }
  inline HRESULT             SetModule(UINT uiModuleNum, __SVSPModule *pkModule) { /* AAV_ASSERT_RETURN(uiModuleNum < __SVSPAvionics::AVIONICS_MODULE_COUNT, S_FALSE);*/ m_apkModules[uiModuleNum] = pkModule; return S_OK; }
  inline __SVSPFont*         GetFont() const { return m_pkFont; }
  //inline DOUBLE              GetSystemTime() const { return ((DOUBLE)GetTickCount())/1000.0; }
  //inline DOUBLE              GetSimTime() const { return m_dAbsoluteTime; }
  inline BOOL                IsPaused() const { return (m_dwSimulationPaused != 0); }
  inline BOOL                IsSlewActive() const { return m_bSlewActive; }
  inline BOOL                IsReset() const { return m_bReset; }
  virtual __SVSPModule*      CreateModule(UINT uiModuleNum)                ;
  virtual VOID               DeleteModule(UINT uiModuleNum)                ;
  virtual VOID               LoadMission(WCHAR *pwcFilename)               ;
  virtual VOID               LoadWeaponsFromMissionFile(WCHAR *pwcFilename);

  DOUBLE                     m_dDeltaTime;

protected:
  static  ULONG   FASTCALL   Release_Callback              (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice)                                                                                                                                   ;
  static  HRESULT FASTCALL   Reset_Callback                (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)                                                                                   ;
  static  HRESULT FASTCALL   Present_Callback              (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)                               ;
  static  HRESULT FASTCALL   EndScene_Callback             (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice)                                                                                                                                   ;
  static  HRESULT FASTCALL   CreateTexture_Callback        (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) ;
  static  HRESULT FASTCALL   SetTexture_Callback           (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)                                                                                      ;
  static  HRESULT FASTCALL   DrawIndexedPrimitive_Callback (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)           ;

  virtual VOID                    Release             ()                                                                                                                                                             ;
  virtual HRESULT                 Reset               (LPD3DXDEVICE pkDevice)                                                                                                                                   ;
  virtual HRESULT                 Present             (LPD3DXDEVICE pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)                               ;
  virtual HRESULT                 EndScene            (LPD3DXDEVICE pkDevice)                                                                                                                                   ;
  virtual HRESULT                 CreateTexture       (LPD3DXDEVICE pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) ;
  virtual HRESULT                 SetTexture          (LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)                                                                                      ;
//virtual HRESULT                 DrawIndexedPrimitive(LPD3DXDEVICE pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)           ;

  virtual VOID               DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);
  friend VOID CALLBACK       AVIONICS__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);

  HWND                        m_hWnd;
  __SVSPFont                 *m_pkFont;
  DWORD                       m_dwSimulationPaused;
  BOOL                        m_bSlewActive;
  UINT_PTR                    m_uiWarnClockTimerID;
  UINT_PTR                    m_uiCautionClockTimerID;
  FLOAT                       m_dSystemStartTime;
  DOUBLE                      m_dLastWarnClockCheckTime;
  BOOL                        m_bReset;
  BOOL                        m_bRender;
  __SVSPModule               *m_apkModules[__SVSPAvionics::AVIONICS_MODULE_COUNT];
};


#endif	/* __SVSP_AVIONICS_H__ */


