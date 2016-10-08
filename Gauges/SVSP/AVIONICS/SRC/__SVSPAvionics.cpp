#include "__SVSPD3D9.h"
#include "__SVSPAvionics.h"
#include "AC.h"
#include "SIM.h"
#include "FLCS.h"
#include "ECS.h"
#include "HOTAS.h"
#include "SS.h"
#include "CARRIER.h"
#include "WPN.h"
#include "VWS.h"
#include "SMS.h"
#include "VPM.h"
#include "HUD.h"
#include "AIP.h"
#include "FCR.h"
#include "COM.h"
#include "MFD.h"
#include "MPCD.h"

const DWORD __SVSPAvionics::g_adwModuleThreadSleepMilisecondsTables[__SVSPAvionics::AVIONICS_MODULE_COUNT] = 
{
  /* AVIONICS_MODULE_AC      */  75,
  /* AVIONICS_MODULE_HOTAS   */  100,
  /* AVIONICS_MODULE_ISS     */  75,
  /* AVIONICS_MODULE_FLCS    */  50,
  /* AVIONICS_MODULE_ECS     */  45,
  /* AVIONICS_MODULE_SMS     */  100,
  /* AVIONICS_MODULE_MDA     */  150,
  /* AVIONICS_MODULE_VWS     */  115,
  /* AVIONICS_MODULE_FCR     */  75,
  /* AVIONICS_MODULE_VPM     */  500,
  /* AVIONICS_MODULE_WPN     */  100,
  /* AVIONICS_MODULE_EICAS   */  500,
  /* AVIONICS_MODULE_COM     */  150,
  /* AVIONICS_MODULE_SIM     */  50,
  /* AVIONICS_MODULE_CARRIER */  50,
  /* AVIONICS_MODULE_AIP     */  25,
  /* AVIONICS_MODULE_HUD     */  100,
  /* AVIONICS_MODULE_MFD     */  1000,
  /* AVIONICS_MODULE_MPCD    */  1000,
};

bool __SVSPAvionics::g_bCautionClockEdgeUP     = false;
bool __SVSPAvionics::g_bWarnClockEdgeUP        = false;

ULONG   FASTCALL __SVSPAvionics::Release_Callback      (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice)                                                                                                                                   {        pkAvionics->Release      (); return 0;                                                                     }
HRESULT FASTCALL __SVSPAvionics::Reset_Callback        (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)                                                                                   { return pkAvionics->Reset        (pkDevice);                                                                       }
HRESULT FASTCALL __SVSPAvionics::Present_Callback      (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)                               { return pkAvionics->Present      (pkDevice, pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);               }
HRESULT FASTCALL __SVSPAvionics::EndScene_Callback     (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice)                                                                                                                                   { return pkAvionics->EndScene     (pkDevice);                                                                       }
HRESULT FASTCALL __SVSPAvionics::CreateTexture_Callback(__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) { return pkAvionics->CreateTexture(pkDevice, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle); }
HRESULT FASTCALL __SVSPAvionics::SetTexture_Callback   (__SVSPAvionics *pkAvionics, LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)                                                                                      { return pkAvionics->SetTexture   (pkDevice, Stage, pTexture);                                                      }

__SVSPAvionics::__SVSPAvionics(HWND hWnd) : __SVSPModule("__SVSPAvionics", NULL, 100, true, AVIONICS__DispatchProcess, NULL, 0),
  m_hWnd                     (hWnd),
  m_pkFont                   (NULL),
  m_dwSimulationPaused       (0),
  m_bSlewActive              (false),
  m_uiWarnClockTimerID       (0),
  m_uiCautionClockTimerID    (0),
  m_dSystemStartTime         (0),
  m_dLastWarnClockCheckTime  (0),
  m_bReset                   (false),
  m_bRender                  (false)
{
  memset(&m_apkModules[0], 0, sizeof(m_apkModules));

  //Connect to __SVSPD3D9 layer. (Will couse "Entry Point Not Found" exception if no __SVSPD3D9.DLL code is loaded.
  //Connect to __SVSPD3D9 layer through the static member wrapers.
  __SVSPD3D9_RegisterCallback(__SVSPD3D9_CALLBACK_RELEASE,                (__SVSPD3D9_Callback)&__SVSPAvionics::Release_Callback,              this);
//__SVSPD3D9_RegisterCallback(__SVSPD3D9_CALLBACK_RESET,                  (__SVSPD3D9_Callback)&__SVSPAvionics::Reset_Callback,                this);
  __SVSPD3D9_RegisterCallback(__SVSPD3D9_CALLBACK_PRESENT,                (__SVSPD3D9_Callback)&__SVSPAvionics::Present_Callback,              this);
  __SVSPD3D9_RegisterCallback(__SVSPD3D9_CALLBACK_END_SCENE,              (__SVSPD3D9_Callback)&__SVSPAvionics::EndScene_Callback,             this);
  __SVSPD3D9_RegisterCallback(__SVSPD3D9_CALLBACK_CREATE_TEXTURE,         (__SVSPD3D9_Callback)&__SVSPAvionics::CreateTexture_Callback,        this);
  __SVSPD3D9_RegisterCallback(__SVSPD3D9_CALLBACK_SET_TEXTURE,            (__SVSPD3D9_Callback)&__SVSPAvionics::SetTexture_Callback,           this);

  //Instanciate fonts.
  //m_pkFont = new D3DXFONTCUSTOM("Arial", 84/*72*/, D3DXFONTCUSTOM::BOLD);
  //m_pkFont = new D3DXFONTCUSTOM("Arial", 16/*72*/);
  //m_pkFont = new D3DXFONTCUSTOM("Courier New", 20, __SVSPFont_D3D9::BOLD);
  //m_pkFont = new D3DXFONTCUSTOM("Makisupa", 20, __SVSPFont_D3D9::BOLD);
  //m_pkFont = new D3DXFONTCUSTOM("Novamono", 30, __SVSPFont_D3D9::BOLD);
  //m_pkFont = new D3DXFONTCUSTOM("NORMT___", 18, __SVSPFont_D3D9::BOLD);
  m_pkFont = new D3DXFONTCUSTOM("ProFontWindows", 16, __SVSPFont_D3D9::BOLD);

  //Subscribe to system events
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_START,         "SimStart");
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_STOP,          "SimStop");
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_PAUSED,        "Pause");
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_UNPAUSED,      "Unpaused");
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_CRASHED,       "Crashed");
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_SubscribeToSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_CRASHED_RESET, "CrashReset");
  AAV_ASSERT(SUCCEEDED(m_hResult));

  // __SVSPAvionics::DATA_ID_READ_OBJECT_STATE_DATA
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, __SVSPAvionics::DATA_ID_READ_OBJECT_STATE_DATA, "IS SLEW ACTIVE",                      "Bool",        SIMCONNECT_DATATYPE_INT32);
  m_hResult = SimConnect_AddToDataDefinition(m_hSimConnect, __SVSPAvionics::DATA_ID_READ_OBJECT_STATE_DATA, "ABSOLUTE TIME",                       "Seconds",     SIMCONNECT_DATATYPE_FLOAT64);

  // SETUP MENU
  if(m_apkModules[__SVSPAvionics::AVIONICS_MODULE_MDA] != NULL)
  {
    m_hResult = SimConnect_MapClientEventToSimEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_MISSION_DATA_SELECTED);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_MenuAddItem(m_hSimConnect, "__SVSPAvionics", __SVSPAvionics::EVENT_ID_MENU_ITEM_AVIONICS_SELECTED, (DWORD)this);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_MenuAddSubItem (m_hSimConnect, __SVSPAvionics::EVENT_ID_MENU_ITEM_AVIONICS_SELECTED, "Load Mission Data", __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_MISSION_DATA_SELECTED, (DWORD)this);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_AddClientEventToNotificationGroup(m_hSimConnect,  __SVSPAvionics::AVIONICS_MENU, __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_MISSION_DATA_SELECTED);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_MenuAddSubItem (m_hSimConnect, __SVSPAvionics::EVENT_ID_MENU_ITEM_AVIONICS_SELECTED, "Load Weapons From Mission File", __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_WEAPONS_DATA_SELECTED, (DWORD)this);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_AddClientEventToNotificationGroup(m_hSimConnect,  __SVSPAvionics::AVIONICS_MENU, __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_MISSION_DATA_SELECTED);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_SetNotificationGroupPriority(m_hSimConnect, __SVSPAvionics::AVIONICS_MENU, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
    AAV_ASSERT(SUCCEEDED(m_hResult));
  }

  m_hResult = SimConnect_RequestDataOnSimObject(m_hSimConnect, __SVSPAvionics::REQUEST_ID_READ_OBJECT_STATE_DATA, __SVSPAvionics::DATA_ID_READ_OBJECT_STATE_DATA, SIMCONNECT_SIMOBJECT_TYPE_USER, SIMCONNECT_PERIOD_SECOND, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
  AAV_ASSERT(SUCCEEDED(m_hResult));

  //START TIMERS
  m_uiWarnClockTimerID    = SetTimer(NULL, 0, 150, WarnTimerProc);
  m_uiCautionClockTimerID = SetTimer(NULL, 0, 250, GetTimerProc);


  __SVSPModule *pkModule = NULL;
  // GENERAL AICRAFT MONITORING MODULE (DEFAULT MODULE TO BE STARTED)
  pkModule = m_apkModules[__SVSPAvionics::AVIONICS_MODULE_AC]      = new AC   (this, g_adwModuleThreadSleepMilisecondsTables[__SVSPAvionics::AVIONICS_MODULE_AC]);
  AAV_ASSERT_RETURN_VOID(pkModule != NULL);

}

__SVSPAvionics::~__SVSPAvionics()
{
  //STOP TIMERS
  KillTimer(NULL, m_uiWarnClockTimerID);
  KillTimer(NULL, m_uiCautionClockTimerID);

  //STOP ALL MODULES
  Stop();

  //Disconnect from __SVSPD3D9 layer. (Will couse "Entry Point Not Found" exception if no __SVSPD3D9.DLL code is loaded.
  __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_RELEASE);
  __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_RESET);
  __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_PRESENT);
  __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_END_SCENE);
  __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_CREATE_TEXTURE);
  __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_SET_TEXTURE);
  __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE);

  //Release graphics.
  Release();

  //DELETE ALL MODULES
  for(UINT i = 0; i < __SVSPAvionics::AVIONICS_MODULE_COUNT; ++i)
  {
    if((m_apkModules[i] != NULL) && !m_apkModules[i]->IsStarted())
    {
      delete m_apkModules[i];
      m_apkModules[i] = NULL;
    }
  }

  //Delete system fonts.
  if(m_pkFont != NULL)
  {
    delete m_pkFont;
    m_pkFont = NULL;
  }

  // CLEANUP MENU
  if(m_apkModules[__SVSPAvionics::AVIONICS_MODULE_MDA] != NULL)
  {
    m_hResult = SimConnect_RemoveClientEvent(m_hSimConnect, __SVSPAvionics::AVIONICS_MENU, __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_MISSION_DATA_SELECTED);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_MenuDeleteSubItem (m_hSimConnect, __SVSPAvionics::EVENT_ID_MENU_ITEM_AVIONICS_SELECTED, __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_MISSION_DATA_SELECTED);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_MenuDeleteSubItem (m_hSimConnect, __SVSPAvionics::EVENT_ID_MENU_ITEM_AVIONICS_SELECTED, __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_WEAPONS_DATA_SELECTED);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hResult = SimConnect_MenuDeleteItem (m_hSimConnect, __SVSPAvionics::EVENT_ID_MENU_ITEM_AVIONICS_SELECTED);
    AAV_ASSERT(SUCCEEDED(m_hResult));
  }

  // UNSUBSCRIBE FROM SYSTEM EVENTS
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_START);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_STOP);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_PAUSED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_UNPAUSED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_CRASHED);
  AAV_ASSERT(SUCCEEDED(m_hResult));
  m_hResult = SimConnect_UnsubscribeFromSystemEvent(m_hSimConnect, __SVSPAvionics::EVENT_ID_SIM_CRASHED_RESET);
  AAV_ASSERT(SUCCEEDED(m_hResult));

  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "DELETED");
}

__SVSPModule* __SVSPAvionics::CreateModule(UINT uiModuleNum)
{
  D3DXMATRIXA16  kRotationMatrix;
  D3DXMATRIXA16  kScalingMatrix;
  D3DXMATRIXA16  kTranslationMatrix;

  AAV_ASSERT_RETURN(uiModuleNum < AVIONICS_MODULE_COUNT, NULL)
  __SVSPModule **ppkModule = &m_apkModules[uiModuleNum];
    AAV_ASSERT_RETURN(*ppkModule == NULL, NULL);

  switch(uiModuleNum)
  {
    // HANDS ON THROTTLE AND STICK MODULE
  case __SVSPAvionics::AVIONICS_MODULE_HOTAS:
    *ppkModule   = new HOTAS(this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum]);
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // FLIGHT CONTROL SYSTEM MODULE
  case __SVSPAvionics::AVIONICS_MODULE_FLCS:
    *ppkModule     = new FLCS (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum] );
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // ENGINE CONTROL SYSTEM MODULE
  case __SVSPAvionics::AVIONICS_MODULE_ECS:
    *ppkModule     = new ECS  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum] );
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // SIMULATION MODULE
  case __SVSPAvionics::AVIONICS_MODULE_SIM:
    *ppkModule     = new SIM  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum] );
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // STEERPOINT NAVIGATION MODULE
  case __SVSPAvionics::AVIONICS_MODULE_WPN:
    *ppkModule     = new WPN  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum]);
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // RADIO COMMUNICATION AND AIRBORNE SYSTEMS MANAGEMENT MODULE
  case __SVSPAvionics::AVIONICS_MODULE_COM:
    *ppkModule     = new COM  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum]);
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // ENGINE INDICATION AND CREW ALERT SYSTEM MODULE
  //case __SVSPAvionics::AVIONICS_MODULE_AICAS:
  // *ppkModule   = new EICAS(this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum]); 
  //  AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
  //  break;

    // STORES MANAGEMENT SYSTEM MODULE
  case __SVSPAvionics::AVIONICS_MODULE_SMS:
    *ppkModule     = new SMS  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum]); 
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // SIGNLE-TARGET-TRACK FIRE CONTROL RADAR MODULE
  case __SVSPAvionics::AVIONICS_MODULE_FCR:
    *ppkModule     = new FCR  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum]); 
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // ON-BOARD SOUND SERVER MODULE
  case __SVSPAvionics::AVIONICS_MODULE_ISS:
    *ppkModule     = new SS   (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum]); 
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // VOCAL WARNING SYSTEM MODULE
  case __SVSPAvionics::AVIONICS_MODULE_VWS:
    *ppkModule     = new VWS  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum]); 
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // MULTI-FUNCTIONAL HEAD-DOWN-DISPLAY MODULE
  case __SVSPAvionics::AVIONICS_MODULE_MFD:
    *ppkModule     = new MFD(this, g_adwModuleThreadSleepMilisecondsTables[AVIONICS_MODULE_MFD]);
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // MULTI-PURPOSE COLOR DISPLAY MODULE (F/A-18AC)
  case __SVSPAvionics::AVIONICS_MODULE_MPCD:
    *ppkModule     = new MPCD(this, g_adwModuleThreadSleepMilisecondsTables[AVIONICS_MODULE_MPCD]);
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    // HEADS-UP-DISPLAY MODULE
  case __SVSPAvionics::AVIONICS_MODULE_HUD:
    *ppkModule     = new HUD  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum]);
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    D3DXMatrixTranslation(&kTranslationMatrix, 0, 0, 2.1f);
    (*ppkModule)->SetOPM(kTranslationMatrix);
    (static_cast<HUD *>(*ppkModule))->SetMode(HUD::DISPLAY_MODE_HUD);
    (static_cast<HUD *>(*ppkModule))->SetScale(0.044f);
    (static_cast<HUD *>(*ppkModule))->SetProjectorScale(1.0f);
    break;

  // MISSION DATA ASSEMBLY MODULE
  case __SVSPAvionics::AVIONICS_MODULE_MDA:
  //*ppkModule     = new MDA  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum], L"./SimObjects/Airplanes/TECH_DEMO_01/mission/Mission_PHJR.xml");
  //*ppkModule     = new MDA  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum], L"./SimObjects/Airplanes/TECH_DEMO_01/mission/Mission_PHJR_1AGM01.xml");
  //*ppkModule     = new MDA  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum], L"./SimObjects/Airplanes/TECH_DEMO_01/mission/Mission_PHJR_2AGM01_2AIM120A.xml");
  //*ppkModule     = new MDA  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum], L"./SimObjects/Airplanes/TECH_DEMO_01/mission/Mission_PHJR_2AGM01_4AIM120A_2AIM9M_2AGM88E.xml");
    *ppkModule     = new MDA  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum], L"./SimObjects/Airplanes/TECH_DEMO_01/mission/Mission_PHJR_6AGM01_2AIM120A.xml");
  //*ppkModule     = new MDA  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum], L"./SimObjects/Airplanes/TECH_DEMO_01/mission/Mission_PHJR_6AGM01_4AIM120A.xml");
  //*ppkModule     = new MDA  (this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum], L"./SimObjects/Airplanes/TECH_DEMO_01/mission/Mission_PHJR_2AGM01_6AIM120A_2AIM9M.xml");
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    //DEBUG
    //AIRCRAFT CARRIER CONTROL MODULE
  case __SVSPAvionics::AVIONICS_MODULE_CARRIER:
    *ppkModule     = new CARRIER(this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum], "VEH_CVN-76", &(__SVSPGeoCoordinates3<FLOAT>(21.289001f, -158.069102f, 0)), &(AAVVector3<FLOAT>(0.0, 270.0, 0.0)));
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

    //ARTIFICIAL INTELLIGENCE PILOT MODULE
  case __SVSPAvionics::AVIONICS_MODULE_AIP:
    *ppkModule     = new AIP(this, g_adwModuleThreadSleepMilisecondsTables[uiModuleNum], "TECH_DEMO_01 /01", &(__SVSPGeoCoordinates3<FLOAT>(21.297900f, -158.074800f, 33)), &(AAVVector3<FLOAT>(0.0, 224 + 10.0, 0.0)));
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;

  default:
    AAV_ASSERT_RETURN(*ppkModule != NULL, NULL);
    break;
  }

  return *ppkModule;
}

VOID __SVSPAvionics::DeleteModule(UINT uiModuleNum)
{
  AAV_ASSERT_RETURN_VOID(uiModuleNum < AVIONICS_MODULE_COUNT)
  AAV_ASSERT_RETURN_VOID(m_apkModules[uiModuleNum] != NULL);
  delete m_apkModules[uiModuleNum];
  m_apkModules[uiModuleNum] = NULL;
}


VOID __SVSPAvionics::LoadMission(WCHAR *pwcFilename)
{
  SMS *pkSMS = (SMS *)m_apkModules[__SVSPAvionics::AVIONICS_MODULE_SMS];
  WPN *pkWPN = (WPN *)m_apkModules[__SVSPAvionics::AVIONICS_MODULE_WPN];
  MDA *pkMDA = (MDA *)m_apkModules[__SVSPAvionics::AVIONICS_MODULE_MDA];

  ///////////////////////////////////////////////////////////
  // HALT MISSION DEPENDENT SYSTEMS
  ///////////////////////////////////////////////////////////
  __SVSPAvionics::Stop();
  __SVSPAvionics::Release();

  if(pkSMS != NULL)
  {
    pkSMS->DeleteAllStores();
  }

  if(pkWPN != NULL) 
  {
    pkWPN->DeleteAllWaypoints();
  }

  if(pkMDA != NULL)
  {
    pkMDA->DeleteAllTargets();
    //Read mission data.
    pkMDA->ReadConfigFile(pwcFilename);
  }

  // RESUME MISSION DEPENDENT SYSTEMS
  __SVSPAvionics::Start();
}

VOID __SVSPAvionics::LoadWeaponsFromMissionFile(WCHAR *pwcFilename)
{
  SMS *pkSMS = (SMS *)m_apkModules[__SVSPAvionics::AVIONICS_MODULE_SMS];
  MDA *pkMDA = (MDA *)m_apkModules[__SVSPAvionics::AVIONICS_MODULE_MDA];

  // HALT MISSION DEPENDENT SYSTEMS
  __SVSPAvionics::Stop();
  __SVSPAvionics::Release();

  if(pkSMS != NULL)
  {
    pkSMS->DeleteAllStores();
  }

  if(pkMDA != NULL)
  {
    //Read weapons data.
    pkMDA->ReadConfigFile(pwcFilename, L"WeaponList");
  }

  ///////////////////////////////////////////////////////////
  // RESUME MISSION DEPENDENT SYSTEMS
  ///////////////////////////////////////////////////////////
  __SVSPAvionics::Start();
}

VOID __SVSPAvionics::Start()
{
  // START ALL MODULES
  for(UINT i = 0; i < __SVSPAvionics::AVIONICS_MODULE_COUNT; ++i)
  {
    __SVSPModule *pkModule = m_apkModules[i];
    if((pkModule != NULL) && !pkModule->IsStarted())
    {
      pkModule->Start();
    }
  }
  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "STARTED");
}

VOID __SVSPAvionics::Stop()
{
  // STOP ALL MODULES
  for(UINT i = 0; i < __SVSPAvionics::AVIONICS_MODULE_COUNT; ++i)
  {
    __SVSPModule *pkModule = m_apkModules[i];
    if((pkModule != NULL) && pkModule->IsStarted())
    {
      pkModule->Stop();
    }
  }
  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "STOPED");
}

VOID CALLBACK  AVIONICS__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  __SVSPAvionics *__THIS = static_cast<__SVSPAvionics *>(pContext);
  if(!__THIS->m_bAbort)
  {
    __THIS->DispatchProcess(pData, cbData);
  }
}

VOID __SVSPAvionics::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
{
  switch(pData->dwID)
  {
  case SIMCONNECT_RECV_ID_EVENT:
    {
      SIMCONNECT_RECV_EVENT *pEvt = (SIMCONNECT_RECV_EVENT*)pData;
      switch(pEvt->uEventID)
      {
      case __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_MISSION_DATA_SELECTED:
        {
          ///////////////////////////////////////////////////////////////////
          // READ MISSION DATA FILE
          ///////////////////////////////////////////////////////////////////
          CHAR         szCurrentPath[MAX_PATH];
          CHAR         szFilename[MAX_PATH];
          WCHAR        wszFilename[MAX_PATH];
          wszFilename[0] = L'\0';
          OPENFILENAME kOfn;

          ZeroMemory(&kOfn, sizeof(OPENFILENAME));
          kOfn.lStructSize     = sizeof(OPENFILENAME);
          kOfn.hwndOwner       = NULL;
          kOfn.lpstrFile       = &szFilename[0];
          kOfn.lpstrFile[0]    = '\0';
          kOfn.nMaxFile        = sizeof(szFilename);
          kOfn.lpstrFilter     = "All\0*.*\0*.XML\0";
          kOfn.nFilterIndex    = 1;
          kOfn.lpstrFileTitle  = NULL;
          kOfn.nMaxFileTitle   = 0;
          kOfn.lpstrInitialDir = NULL;
          kOfn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


          GetCurrentDirectory(MAX_PATH, &szCurrentPath[0]);
          if(GetOpenFileName(&kOfn))
          {
            SetCurrentDirectory(&szCurrentPath[0]);
            if(strlen(kOfn.lpstrFile) != 0)
            {
              MultiByteToWideChar(CP_UTF8, 0, kOfn.lpstrFile, strlen(kOfn.lpstrFile) + 1, &wszFilename[0], sizeof(wszFilename)/sizeof(WCHAR));
              LoadMission(&wszFilename[0]);
            }
          }
        }
        break;

      case __SVSPAvionics::EVENT_ID_MENU_ITEM_LOAD_WEAPONS_DATA_SELECTED:
        {
          ///////////////////////////////////////////////////////////////////
          // READ MISSION DATA FILE
          ///////////////////////////////////////////////////////////////////
          CHAR         szCurrentPath[MAX_PATH];
          CHAR         szFilename[MAX_PATH];
          WCHAR        wszFilename[MAX_PATH];
          wszFilename[0] = L'\0';
          OPENFILENAME kOfn;

          ZeroMemory(&kOfn, sizeof(OPENFILENAME));
          kOfn.lStructSize     = sizeof(OPENFILENAME);
          kOfn.hwndOwner       = NULL;
          kOfn.lpstrFile       = &szFilename[0];
          kOfn.lpstrFile[0]    = '\0';
          kOfn.nMaxFile        = sizeof(szFilename);
          kOfn.lpstrFilter     = "All\0*.*\0*.XML\0";
          kOfn.nFilterIndex    = 1;
          kOfn.lpstrFileTitle  = NULL;
          kOfn.nMaxFileTitle   = 0;
          kOfn.lpstrInitialDir = NULL;
          kOfn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


          GetCurrentDirectory(MAX_PATH, &szCurrentPath[0]);
          if(GetOpenFileName(&kOfn))
          {
            SetCurrentDirectory(&szCurrentPath[0]);
            if(strlen(kOfn.lpstrFile) != 0)
            {
              MultiByteToWideChar(CP_UTF8, 0, kOfn.lpstrFile, strlen(kOfn.lpstrFile) + 1, &wszFilename[0], sizeof(wszFilename)/sizeof(WCHAR));
              LoadWeaponsFromMissionFile(&wszFilename[0]);
            }
          }
        }
        break;

      case __SVSPAvionics::EVENT_ID_SIM_START:
        //START ALL MODULES
        Start();
        m_bValid = true;
        m_dwSimulationPaused &= ~__SVSPAvionics::STOP_FLAG;
        break;

      case __SVSPAvionics::EVENT_ID_SIM_STOP:
        m_dwSimulationPaused |= __SVSPAvionics::STOP_FLAG;
        m_bValid = false;
        break;

      case __SVSPAvionics::EVENT_ID_SIM_PAUSED:
        m_dwSimulationPaused = (pEvt->dwData == 0) ? m_dwSimulationPaused &= ~__SVSPAvionics::PAUSED_FLAG : m_dwSimulationPaused |= __SVSPAvionics::PAUSED_FLAG;
        break;

      case __SVSPAvionics::EVENT_ID_SIM_CRASHED:
        m_dwSimulationPaused |= __SVSPAvionics::CRASHED_FLAG;
        break;

      case __SVSPAvionics::EVENT_ID_SIM_CRASHED_RESET:
        m_dwSimulationPaused &= ~__SVSPAvionics::CRASHED_FLAG;
        break;
      }
    }
    break;

  case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
    {
      SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
      if(pObjData->dwRequestID == __SVSPAvionics::REQUEST_ID_READ_OBJECT_STATE_DATA)
      {
        __SVSPAvionics::READ_OBJECT_STATE_DATA *pkData = (__SVSPAvionics::READ_OBJECT_STATE_DATA*)&pObjData->dwData;
        //m_dwSimulationPaused         = (pkData->bSlewActive) ? m_dwSimulationPaused |= __SVSPAvionics::SLEW_FLAG : m_dwSimulationPaused &= ~__SVSPAvionics::SLEW_FLAG;
        m_bSlewActive                = pkData->bSlewActive;
        //m_dDeltaTime                 = pkData->dAbsoluteTime - m_dAbsoluteTime;
        //m_dAbsoluteTime              = pkData->dAbsoluteTime;
      }
    }
    break;

  //case SIMCONNECT_RECV_ID_EVENT:
  //  {
  //    SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;
  //    switch(evt->uEventID)
  //    {
  //      case MDA::EVENT_ID_MENU_ITEM_LOAD_MISSION_DATA_SELECTED:
  //      {
  //      }
  //      break;
  //    }
  //  }
  //  break;
  }
}


VOID __SVSPAvionics::Release()
{
  if(m_bReset)
  {
    // RELEASE ALL MODULES
    for(UINT i = 0; i < __SVSPAvionics::AVIONICS_MODULE_COUNT; ++i)
    {
      __SVSPModule *pkModule = m_apkModules[i];
      if(pkModule != NULL)
      {
        pkModule->Release();
      }
    }

    // Release font
    if(m_pkFont != NULL)
    {
      m_pkFont->Release();
    }

    //Release __SVSPGeometryObject_D3D9<__SVSPVertex> specific
    __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalRelease();

    m_bReset = false;
    __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "RELEASED");
  }
}

HRESULT __SVSPAvionics::Reset(LPD3DXDEVICE pkDevice)
{
  HRESULT hResult = S_OK;

  if(!m_bReset)
  {
    // Reset font
    if(m_pkFont != NULL)
    {
      hResult = m_pkFont->Reset(pkDevice);
    }

    // Reset all modules.
    for(UINT i = 0; i < __SVSPAvionics::AVIONICS_MODULE_COUNT; ++i)
    {
      __SVSPModule *pkModule = m_apkModules[i];
      if(pkModule != NULL)
      {
        hResult = pkModule->Reset(pkDevice);
      }
    }

    //Reset __SVSPGeometryObject_D3D9<__SVSPVertex> specific
    __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalReset(pkDevice);

    m_bReset = true;
    __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "RESET");
  }

  return hResult;
}

HRESULT __SVSPAvionics::Present(LPD3DXDEVICE pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
  m_bRender = m_bReset;
  return pkDevice->Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
}

HRESULT __SVSPAvionics::EndScene(LPD3DXDEVICE pkDevice)
{
  __SVSPModule *pkMFD  = GetModule(__SVSPAvionics::AVIONICS_MODULE_MFD);
  __SVSPModule *pkMPCD = GetModule(__SVSPAvionics::AVIONICS_MODULE_MPCD);

  if(m_bRender && m_bReset)
  {
    if(pkMFD != NULL)
    {
      pkMFD->EndScene(pkDevice);
    }
    if(pkMPCD != NULL)
    {
      pkMPCD->EndScene(pkDevice);
    }
    m_bRender = false;
  }
  return pkDevice->EndScene();
}

HRESULT  __SVSPAvionics::CreateTexture(LPD3DXDEVICE pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
  HRESULT hResult = S_FALSE;

  HUD  *pkHUD  = static_cast<HUD  *>(GetModule(__SVSPAvionics::AVIONICS_MODULE_HUD));
  MFD  *pkMFD  = static_cast<MFD  *>(GetModule(__SVSPAvionics::AVIONICS_MODULE_MFD));
  MPCD *pkMPCD = static_cast<MPCD *>(GetModule(__SVSPAvionics::AVIONICS_MODULE_MPCD));

  //Perform reset if required.
  if(!m_bReset)
  {
    Reset(pkDevice);
  }

  if((pkHUD != NULL) && (Width == HUD::STENCIL_TEXTURE_WIDTH) && (Height == HUD:: STENCIL_TEXTURE_HEIGHT) &&
     (pkHUD->CreateTexture(pkDevice, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle) == S_OK))
  {
    hResult = S_OK;
  }
  else if((pkMFD != NULL) && (Format == HDD::HDD_GAUGE_TEXTURE_FORMAT) && (Width == HDD::HDD_GAUGE_TEXTURE_WIDTH) && (Height == HDD:: HDD_GAUGE_TEXTURE_HEIGHT) &&
          (pkMFD->CreateTexture(pkDevice, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle) == S_OK))
  {
    hResult = S_OK;
  }
  else if((pkMPCD != NULL) && (Format == HDD::HDD_GAUGE_TEXTURE_FORMAT) && (Width == HDD::HDD_GAUGE_TEXTURE_WIDTH) && (Height == HDD:: HDD_GAUGE_TEXTURE_HEIGHT) &&
          (pkMPCD->CreateTexture(pkDevice, Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle) == S_OK))
  {
    hResult = S_OK;
  }
  else
  {
    hResult = pkDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
  }

  return hResult;
}

HRESULT  __SVSPAvionics::SetTexture(LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
  HRESULT hResult = S_FALSE;

  HUD  *pkHUD  = static_cast<HUD  *>(GetModule(__SVSPAvionics::AVIONICS_MODULE_HUD));
  MFD  *pkMFD  = static_cast<MFD  *>(GetModule(__SVSPAvionics::AVIONICS_MODULE_MFD));
  MPCD *pkMPCD = static_cast<MPCD *>(GetModule(__SVSPAvionics::AVIONICS_MODULE_MPCD));

  if(m_bReset)
  {
    if(pTexture == 0)
    {
      hResult = pkDevice->SetTexture(Stage, pTexture);
    }
    else if((pkHUD != NULL) && (pkHUD->GetStencilTexture() == pTexture))
    {
      hResult = pkHUD->SetTexture(pkDevice, Stage, pTexture);
    }
    else if((pkMFD != NULL) && (pkMFD->GetGaugeTexture() == pTexture))
    {
      hResult = pkMFD->SetTexture(pkDevice, Stage, pTexture);
    }
    else if((pkMPCD != NULL) && (pkMPCD->GetGaugeTexture() == pTexture))
    {
      hResult = pkMPCD->SetTexture(pkDevice, Stage, pTexture);
    }
    else
    {
      hResult = pkDevice->SetTexture(Stage, pTexture);
    }
  }
  else
  {
    hResult = pkDevice->SetTexture(Stage, pTexture);
  }

  return hResult;
}
