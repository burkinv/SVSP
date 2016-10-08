
#include "__SVSPAvionics.h"
#include "MFD.h"
#include "RESOURCE.h"
#include "__SVSPD3D9.h"

static __SVSPAvionics         *g_pkAvionics   = NULL;
static HWND                   g_hWnd         = NULL;

VOID FSAPI AVIONICS_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )
{
  if(pgauge != NULL)
  {
    switch (service_id)
    {
    case PANEL_SERVICE_CONNECT_TO_WINDOW:
      //register_key_event_handler((GAUGE_KEY_EVENT_HANDLER)(MFD_EventHandler), pkModule);
      break;

    case PANEL_SERVICE_DISCONNECT:
      //unregister_key_event_handler((GAUGE_KEY_EVENT_HANDLER)(MFD_EventHandler), pkModule);

        // Delete __SVSPAvionics instance.
      if(g_pkAvionics != NULL)
      {
        delete g_pkAvionics;
        g_pkAvionics = NULL;
      }
      break;

    case PANEL_SERVICE_PRE_DRAW:
      if(g_pkAvionics == NULL)
      {
        // Create __SVSPAvionics instance.
        g_pkAvionics = new __SVSPAvionics(g_hWnd);
        AAV_ASSERT(g_pkAvionics != NULL);
      }
      break;

    default:
      break;
    }
  }
}


VOID FSAPI GENERAL_MODULE_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )
{
  UINT32 eModuleName = userdata;

  if(pgauge != NULL)
  {
    switch (service_id)
    {
    case PANEL_SERVICE_CONNECT_TO_WINDOW:
      //register_key_event_handler((GAUGE_KEY_EVENT_HANDLER)(MFD_EventHandler), pkModule);
      break;

    case PANEL_SERVICE_DISCONNECT:
      //unregister_key_event_handler((GAUGE_KEY_EVENT_HANDLER)(MFD_EventHandler), pkModule);

      // Delete GAUGE_Module instance.
      if(g_pkAvionics != NULL)
      {
        if(g_pkAvionics->GetModule(eModuleName) != NULL)
        {
          g_pkAvionics->DeleteModule(eModuleName);
        }
      }
      break;

    case PANEL_SERVICE_PRE_DRAW:
      {
        //PELEMENT_STATIC_IMAGE pelement = (PELEMENT_STATIC_IMAGE)(pgauge->elements_list[0]->next_element[0]);
        PELEMENT_STATIC_IMAGE pelement = (PELEMENT_STATIC_IMAGE)(pgauge->elements_list[0]);

        if(pelement != NULL)
        {
          // Create GAUGE_Module instance.
          AAV_ASSERT_RETURN_VOID(g_pkAvionics != NULL);
          if(g_pkAvionics->GetModule(eModuleName) == NULL)
          {
            g_pkAvionics->CreateModule(eModuleName);
          }
        }
      }
      break;

    default:
      break;
    }
  }
}


// AVIONICS
#define GAUGEHDR_VAR_NAME g_kAVIONICS_GaugeHeader
extern  PELEMENT_HEADER   g_pkAVIONICS_List;
GAUGE_HEADER_FS700(100, "AVIONICS", &g_pkAVIONICS_List, NULL, AVIONICS_GAUGE_Callback, 0, 0, 0);
MAKE_STATIC(AVIONICS_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkAVIONICS_List  = &AVIONICS_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// HOTAS
#define GAUGEHDR_VAR_NAME g_kHOTAS_GaugeHeader
extern  PELEMENT_HEADER   g_pkHOTAS_List;
VOID FSAPI HOTAS_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata ) { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_HOTAS); }
GAUGE_HEADER_FS700(100, "HOTAS",   &g_pkHOTAS_List,    NULL, HOTAS_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_HOTAS, 0, 0);
MAKE_STATIC(HOTAS_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkHOTAS_List  = &HOTAS_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// ISS
#define GAUGEHDR_VAR_NAME g_kISS_GaugeHeader
extern  PELEMENT_HEADER   g_pkISS_List;
VOID FSAPI ISS_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )   { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_ISS); }
GAUGE_HEADER_FS700(100, "ISS",   &g_pkISS_List,    NULL, ISS_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_ISS, 0, 0);
MAKE_STATIC(ISS_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkISS_List  = &ISS_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// FLCS
#define GAUGEHDR_VAR_NAME g_kFLCS_GaugeHeader
extern  PELEMENT_HEADER   g_pkFLCS_List;
VOID FSAPI FLCS_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata ) { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_FLCS); }
GAUGE_HEADER_FS700(100, "FLCS",   &g_pkFLCS_List,    NULL, FLCS_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_FLCS, 0, 0);
MAKE_STATIC(FLCS_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkFLCS_List  = &FLCS_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// ECS
#define GAUGEHDR_VAR_NAME g_kECS_GaugeHeader
extern  PELEMENT_HEADER   g_pkECS_List;
VOID FSAPI ECS_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_ECS); }
GAUGE_HEADER_FS700(100, "ECS",   &g_pkECS_List,    NULL, ECS_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_ECS, 0, 0);
MAKE_STATIC(ECS_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkECS_List  = &ECS_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// SMS
#define GAUGEHDR_VAR_NAME g_kSMS_GaugeHeader
extern  PELEMENT_HEADER   g_pkSMS_List;
VOID FSAPI SMS_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_SMS); }
GAUGE_HEADER_FS700(100, "SMS",   &g_pkSMS_List,    NULL, SMS_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_SMS, 0, 0);
MAKE_STATIC(SMS_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkSMS_List  = &SMS_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// MDA
#define GAUGEHDR_VAR_NAME g_kMDA_GaugeHeader
extern  PELEMENT_HEADER   g_pkMDA_List;
VOID FSAPI MDA_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_MDA); }
GAUGE_HEADER_FS700(100, "MDA",   &g_pkMDA_List,    NULL, MDA_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_MDA, 0, 0);
MAKE_STATIC(MDA_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkMDA_List  = &MDA_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// VWS
#define GAUGEHDR_VAR_NAME g_kVWS_GaugeHeader
extern  PELEMENT_HEADER   g_pkVWS_List;
VOID FSAPI VWS_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_VWS); }
GAUGE_HEADER_FS700(100, "VWS",   &g_pkVWS_List,    NULL, VWS_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_VWS, 0, 0);
MAKE_STATIC(VWS_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkVWS_List  = &VWS_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// FCR
#define GAUGEHDR_VAR_NAME g_kFCR_GaugeHeader
extern  PELEMENT_HEADER   g_pkFCR_List;
VOID FSAPI FCR_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_FCR); }
GAUGE_HEADER_FS700(100, "FCR",   &g_pkFCR_List,    NULL, FCR_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_FCR, 0, 0);
MAKE_STATIC(FCR_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkFCR_List  = &FCR_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// WPN
#define GAUGEHDR_VAR_NAME g_kWPN_GaugeHeader
extern  PELEMENT_HEADER   g_pkWPN_List;
VOID FSAPI WPN_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_WPN); }
GAUGE_HEADER_FS700(100, "WPN",   &g_pkWPN_List,    NULL, WPN_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_WPN, 0, 0);
MAKE_STATIC(WPN_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkWPN_List  = &WPN_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// COM
#define GAUGEHDR_VAR_NAME g_kCOM_GaugeHeader
extern  PELEMENT_HEADER   g_pkCOM_List;
VOID FSAPI COM_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_COM); }
GAUGE_HEADER_FS700(100, "COM",   &g_pkCOM_List,    NULL, COM_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_COM, 0, 0);
MAKE_STATIC(COM_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkCOM_List  = &COM_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// SIM
#define GAUGEHDR_VAR_NAME g_kSIM_GaugeHeader
extern  PELEMENT_HEADER   g_pkSIM_List;
VOID FSAPI SIM_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_SIM); }
GAUGE_HEADER_FS700(100, "SIM",   &g_pkSIM_List,    NULL, SIM_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_SIM, 0, 0);
MAKE_STATIC(SIM_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkSIM_List  = &SIM_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// CARRIER
#define GAUGEHDR_VAR_NAME g_kCARRIER_GaugeHeader
extern  PELEMENT_HEADER   g_pkCARRIER_List;
VOID FSAPI CARRIER_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_CARRIER); }
GAUGE_HEADER_FS700(100, "CARRIER",   &g_pkCARRIER_List,    NULL, CARRIER_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_CARRIER, 0, 0);
MAKE_STATIC(CARRIER_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkCARRIER_List  = &CARRIER_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// AIP
#define GAUGEHDR_VAR_NAME g_kAIP_GaugeHeader
extern  PELEMENT_HEADER   g_pkAIP_List;
VOID FSAPI AIP_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_AIP); }
GAUGE_HEADER_FS700(100, "AIP",   &g_pkAIP_List,    NULL, AIP_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_AIP, 0, 0);
MAKE_STATIC(AIP_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkAIP_List  = &AIP_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// HUD
#define GAUGEHDR_VAR_NAME g_kHUD_GaugeHeader
extern  PELEMENT_HEADER   g_pkHUD_List;
VOID FSAPI HUD_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_HUD); }
GAUGE_HEADER_FS700(100, "HUD",   &g_pkHUD_List,    NULL, HUD_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_HUD, 0, 0);
MAKE_STATIC(HUD_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkHUD_List  = &HUD_background_blank.header;
#undef GAUGEHDR_VAR_NAME

// MFD
#define GAUGEHDR_VAR_NAME g_kMFD_GaugeHeader
extern  PELEMENT_HEADER   g_pkMFD_List;
extern  MOUSERECT         g_akMFD_MouseRect[];
#define MOUSE_RECT_W      1500
#define MOUSE_RECT_H      1500


BOOL __stdcall MFD_MouseCallback(PPIXPOINT relative_point, FLAGS32 mouse_flags )
{
  //CHAR acStr[128];
  AAV_ASSERT_RETURN(g_pkAvionics != NULL, FALSE);
  MFD *pkMFD = static_cast<MFD *>(g_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_MFD));
  AAV_ASSERT_RETURN(pkMFD != NULL, FALSE);
  //sprintf_s(acStr, 128, "%d %d", relative_point->x, relative_point->y);
  //__SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);

  // Convert to MFD framebuffer coordinates with OrtoCenterLH projection matrix 
  relative_point->x = (PIXEL)( (relative_point->x - 512)*(2048.0f/1024.0f) );
  relative_point->y = (PIXEL)( (relative_point->y - 512)*(2048.0f/1024.0f) );
  pkMFD->MouseHandler(relative_point, mouse_flags);
  return TRUE;
}

MOUSE_BEGIN( g_akMFD_MouseRect, HELPID_GAUGE_FUEL_SELECTOR, 0, 0 )
      MOUSE_CHILD_FUNCT( 0,0,MOUSE_RECT_W,MOUSE_RECT_H, CURSOR_HAND, MOUSE_LEFTSINGLE | MOUSE_LEFTDRAG | MOUSE_LEFTRELEASE, MFD_MouseCallback )
MOUSE_END 

VOID FSAPI MFD_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata ) { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_MFD); }
GAUGE_HEADER_FS700(100, "MFD", &g_pkMFD_List, g_akMFD_MouseRect, MFD_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_MFD, 0, 0);
MAKE_STATIC(MFD_background_blank, BMP_HDD_BLANK, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkMFD_List  = &MFD_background_blank.header;
#undef GAUGEHDR_VAR_NAME
#undef MOUSE_RECT_W
#undef MOUSE_RECT_H


// MPCD
#define GAUGEHDR_VAR_NAME g_kMPCD_GaugeHeader
extern  PELEMENT_HEADER   g_pkMPCD_List;
VOID FSAPI MPCD_GAUGE_Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 userdata )  { return GENERAL_MODULE_GAUGE_Callback(pgauge, service_id, __SVSPAvionics::AVIONICS_MODULE_MPCD); }
GAUGE_HEADER_FS700(100, "MPCD",   &g_pkMPCD_List,    NULL, MPCD_GAUGE_Callback, __SVSPAvionics::AVIONICS_MODULE_MPCD, 0, 0);
MAKE_STATIC(MPCD_background_blank, NULL, NULL, NULL, IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION|IMAGE_USE_ALPHA, 0, 0, 0);
PELEMENT_HEADER g_pkMPCD_List  = &MPCD_background_blank.header;
#undef GAUGEHDR_VAR_NAME


#if 0

GAUGE_TABLE_BEGIN()
    GAUGE_TABLE_ENTRY(&g_kTechDemoGaugeHeader)
GAUGE_TABLE_END()

#endif //0


void FSAPI module_init(void)
{
  g_pkAvionics = NULL;
}

void FSAPI  module_deinit(void)
{
  g_pkAvionics = NULL;
}

BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
  switch(dwReason)
  {
  case DLL_PROCESS_ATTACH:
    g_hWnd = __SVSPD3D9_GetFocusWindow();
    break;

  default:
    break;
  }

  return TRUE;
}

/* This is the module's import table.   */
GAUGESIMPORT    ImportTable =
{
  { 0x0000000F, (PPANELS)NULL },
  { 0x00000000, NULL }
};

/* This is the module's export table.   */
GAUGESLINKAGE   Linkage =
{
  0x00000013,
  module_init,
  module_deinit,
  0,
  0,
  FS9LINK_VERSION, 
  {
    GAUGE_TABLE_ENTRY(&g_kAVIONICS_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kHOTAS_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kISS_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kFLCS_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kECS_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kSMS_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kMDA_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kVWS_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kFCR_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kWPN_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kCOM_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kSIM_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kCARRIER_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kAIP_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kHUD_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kMFD_GaugeHeader)
    GAUGE_TABLE_ENTRY(&g_kMPCD_GaugeHeader)
    0 
  }
};

