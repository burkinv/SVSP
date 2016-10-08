#define STRICT

#include "SIM.h"
#include "HOTAS.h"
#include "__SVSPAvionics.h"
#include "FLCS.h"
#include "FCR.h"
#include "WPN.h"
#include "VPM.h"
#include "SMS.h"
#include "HUD.h"
#include "AIP.h"
#include "MDA.h"
#include "HUD.h"

const UINT HOTAS::SINGLE_RANGE_ZONE = 65535/FCR::MAX_NUM_RADAR_RANGES;

BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
  DWORD* pdwNumForceFeedbackAxis = (DWORD*) pContext;

  if( (pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0 )
    (*pdwNumForceFeedbackAxis)++;

  return DIENUM_CONTINUE;
}

BOOL CALLBACK EnumDevicesCallback( const DIDEVICEINSTANCE* pInst, VOID* pContext )
{
  HRESULT               m_hResult;
  LPDIRECTINPUTDEVICE8  pkDevice;

  HOTAS *__pkTHIS = (HOTAS *)pContext;

  if(__pkTHIS != NULL)
  {
    m_hResult = (__pkTHIS->GetDI())->CreateDevice( pInst->guidInstance, &pkDevice, NULL );

    if( FAILED(m_hResult) ) 
      return DIENUM_CONTINUE;

    __pkTHIS->SetDevice(pkDevice);
  }

  return DIENUM_STOP;
}

VOID HOTAS::TryDeviceDetect()
{
#ifdef FORCE_FEEDBACK
  m_pEffect                = NULL;
  m_dwNumForceFeedbackAxis = 0;
  m_bActive                = TRUE;
#endif //FORCE_FEEDBACK
  DIPROPDWORD dipdw;

#ifdef FORCE_FEEDBACK
  if( FAILED( m_hResult = m_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, this, DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK ) ) )
#else
  if( FAILED( m_hResult = m_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, this, DIEDFL_ATTACHEDONLY ) ) )
#endif
  {
    return;
  }

  if(m_pDevice != NULL)
  {
    if( FAILED( m_hResult = m_pDevice->SetDataFormat( &c_dfDIJoystick ) ) )
      return;

    HWND hWnd = GetActiveWindow();


    //if( FAILED( m_hResult = m_pDevice->SetCooperativeLevel(m_pkAvionics->GetHWND(), DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
    if( FAILED( m_hResult = m_pDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
    {
      return;
    }

    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = FALSE;

#ifdef FORCE_FEEDBACK
    if( FAILED( m_hResult = m_pDevice->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph ) ) )
      return;

    if ( FAILED( m_hResult = m_pDevice->EnumObjects(EnumAxesCallback, (VOID*)&m_dwNumForceFeedbackAxis, DIDFT_AXIS ) ) )
      return;

    if( m_dwNumForceFeedbackAxis > 2 )
      m_dwNumForceFeedbackAxis = 2;

    DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
    LONG            rglDirection[2] = { 0, 0 };
    DICONSTANTFORCE cf              = { 0 };

    DIEFFECT eff;
    ZeroMemory( &eff, sizeof(eff) );
    eff.dwSize                  = sizeof(DIEFFECT);
    eff.dwFlags                 = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    eff.dwDuration              = INFINITE;
    eff.dwSamplePeriod          = 0;
    eff.dwGain                  = DI_FFNOMINALMAX;
    eff.dwTriggerButton         = DIEB_NOTRIGGER;
    eff.dwTriggerRepeatInterval = 0;
    eff.cAxes                   = m_dwNumForceFeedbackAxis;
    eff.rgdwAxes                = rgdwAxes;
    eff.rglDirection            = rglDirection;
    eff.lpEnvelope              = 0;
    eff.cbTypeSpecificParams    = sizeof(DICONSTANTFORCE);
    eff.lpvTypeSpecificParams   = &cf;
    eff.dwStartDelay            = 0;

    if( FAILED( m_hResult = m_pDevice->CreateEffect( GUID_ConstantForce, &eff, &m_pEffect, NULL ) ) )
    {
      return;
    }

    if( NULL == m_pEffect )
      return;

#endif //FORCE_FEEDBACK
  }

}


HOTAS::HOTAS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("HOTAS", pkAvionics, dwThreadSleepTime, false, NULL, NULL, NULL),
  m_pDevice(NULL),
  m_uiCurrentRadarRangeIndex(0),
  m_uiRadarAxisValue(0)
{
  memset(&m_bButtonState, 0, sizeof(m_bButtonState));
  memset(&m_kJS,          0, sizeof(m_kJS));

  

  if( FAILED( m_hResult = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
  {
    return;
  }
}

HOTAS::~HOTAS()
{
  __SVSPModule::Stop();

  if(m_pDevice != NULL)
  {
    m_pDevice->Unacquire();
    m_pDevice->Release();
    m_pDevice = NULL;
  }

  if(m_pDI != NULL)
  {
    m_pDI->Release();
    m_pDI = NULL;
  }
}

VOID HOTAS::__CHECK_RADAR_RANGE(UINT uiRadarAxisValue, FCR *pkFCR, UINT AXYS_VALUE_CHANGED_MESSAGE)
{
  if(pkFCR != NULL)
  {
    if(uiRadarAxisValue != m_uiRadarAxisValue)
    {
      for(UINT uiRadarRangeIndex = 0; uiRadarRangeIndex < FCR::MAX_NUM_RADAR_RANGES; ++uiRadarRangeIndex)
      {
        if((uiRadarAxisValue > HOTAS::SINGLE_RANGE_ZONE*uiRadarRangeIndex) && (uiRadarAxisValue < HOTAS::SINGLE_RANGE_ZONE*(uiRadarRangeIndex + 1)) && (uiRadarRangeIndex != pkFCR->GetRadarRangeIndex()))
        {
          m_uiCurrentRadarRangeIndex = uiRadarRangeIndex;
          PostThreadMessage(pkFCR->GetThreadId(), AXYS_VALUE_CHANGED_MESSAGE, m_uiCurrentRadarRangeIndex, 0);
          break;
        }
      }
      m_uiRadarAxisValue = uiRadarAxisValue;
    }
  }
}


VOID HOTAS::Process()
{
  MSG         kMsg;
  bool        bSaveWaypointMsgSent          = false;
  bool        bCatHookStateChangedSent      = false;
  bool        bReleaseWeaponMsgSent         = false;
  bool        bSelectWeaponMsgSent          = false;
  bool        bSelectNextMsgSent            = false;
  bool        bDecreaseScaleMsgSent         = false;
  bool        bIncreaseScaleMsgSent         = false;
  bool        bRudderCenterMsgSent          = false;

  bool bHOTASFailure                         = false;

  HUD  *pkHUD  = static_cast<HUD  *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HUD));
  FLCS *pkFLCS = static_cast<FLCS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FLCS));
  FCR  *pkFCR  = static_cast<FCR  *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FCR));
  WPN  *pkWPN  = static_cast<WPN  *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_WPN));
  SMS  *pkSMS  = static_cast<SMS  *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS));
  MDA  *pkMDA  = static_cast<MDA  *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_MDA));

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case HOTAS_MESSAGE_SYSTEM_FAILURE:
      bHOTASFailure = true;
      break;
    default:
      break;
    }
  }

  if(m_pDevice == NULL)
  {
    TryDeviceDetect();
  }
  else
  {
    if(SUCCEEDED(m_hResult = m_pDevice->Poll()))
    {
      if(SUCCEEDED(m_hResult = m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &m_kDeviceState)))
      {
        m_bValid = true;
        memcpy(&m_kJS, &m_kDeviceState, sizeof(DIJOYSTATE));

        __CHECK_BUTTON_STATE(15, TRUE,          pkHUD,         HUD::MESSAGE_SCALE_INCREASE,                NULL);
        __CHECK_BUTTON_STATE(17, TRUE,          pkHUD,         HUD::MESSAGE_SCALE_DECREASE,                NULL);

        __CHECK_BUTTON_STATE(3,  FALSE,         pkFLCS,        FLCS::MESSAGE_MANUAL_PITCH_OVERRIDE_ENGAGE, FLCS::MESSAGE_MANUAL_PITCH_OVERRIDE_DISENGAGE);

        __CHECK_BUTTON_STATE(14, FALSE,         pkWPN,         WPN::MESSAGE_SELECT_NEXT_STP,               NULL);
        __CHECK_BUTTON_STATE(16, FALSE,         pkWPN,         WPN::MESSAGE_SELECT_PREV_STP,               NULL);

        __CHECK_BUTTON_STATE(24, FALSE,         pkFCR,         FCR::MESSAGE_ON_BEYOND_VISUAL_RANGE,        FCR::MESSAGE_OFF);
        __CHECK_BUTTON_STATE(25, FALSE,         pkFCR,         FCR::MESSAGE_ON_VISUAL_RANGE,               FCR::MESSAGE_OFF);
        __CHECK_BUTTON_STATE(18, FALSE,         pkFCR,         FCR::MESSAGE_TOGGLE_LOCK_ON_NEAREST_TARGET, NULL);
        __CHECK_BUTTON_STATE(23, FALSE,         pkFCR,         FCR::MESSAGE_TOGGLE_UNCAGE_FCR,             NULL);

        __CHECK_BUTTON_STATE(6,  FALSE,         pkSMS,         SMS::MESSAGE_SELECT_NEXT_WEAPON_GROUP,      NULL);
        __CHECK_BUTTON_STATE(1,  FALSE,         pkSMS,         SMS::MESSAGE_RELEASE_WEAPON,                NULL);
        __CHECK_BUTTON_STATE(2,  FALSE,         pkSMS,         SMS::MESSAGE_MASTER_REARM,                  NULL);

        __CHECK_BUTTON_STATE(15, FALSE,         pkMDA,         MDA::MESSAGE_REARM_TARGETS,                 NULL);
        __CHECK_BUTTON_STATE(17, FALSE,         pkMDA,         MDA::MESSAGE_UNARM_TARGETS,                 NULL);
        __CHECK_BUTTON_STATE(4,  FALSE,         pkMDA ,        MDA::MESSAGE_CREATE_AIP,                    NULL);
        __CHECK_BUTTON_STATE(2,  FALSE,         pkMDA,         MDA::MESSAGE_DESTROY_AIP,                   NULL);

        __CHECK_RADAR_RANGE(m_kJS.rglSlider[0], pkFCR,         FCR::MESSAGE_SET_RADAR_RANGE_INDEX);

        __CHECK_SLIDER_STATE_INCREMENT(pkFLCS,                 FLCS::MESSAGE_AIRBRAKE_INCREMENT,           FLCS::MESSAGE_AIRBRAKE_HOLD);
        __CHECK_SLIDER_STATE_DECREMENT(pkFLCS,                 FLCS::MESSAGE_AIRBRAKE_DECREMENT,           FLCS::MESSAGE_AIRBRAKE_HOLD);


#ifdef FORCE_FEEDBACK
        ////////////////////////////////////////////////////
        // FORCE FEEDBACK UPDATE
        ////////////////////////////////////////////////////
        LONG rglDirection[2] = { 0, 0 };
        DICONSTANTFORCE cf;

        rglDirection[0] = (LONG)(DI_FFNOMINALMAX*((FLOAT)m_kJS.lX/32768.0f - 1.0f)); //DI_FFNOMINALMAX;
        rglDirection[1] = (LONG)(DI_FFNOMINALMAX*((FLOAT)m_kJS.lY/32768.0f - 1.0f)); //DI_FFNOMINALMAX;
        if(!bHOTASFailure)
        {
          cf.lMagnitude = (DWORD)(sqrt( (double)rglDirection[0] * (double)rglDirection[0] + (double)rglDirection[1] * (double)rglDirection[1] ));
        }
        else
        {
          cf.lMagnitude = 0;
        }
        //cf.lMagnitude = (DWORD)( rglDirection[0] * rglDirection[0] + rglDirection[1] * rglDirection[1] );
        //cf.lMagnitude = 5000;
        DIEFFECT eff;
        ZeroMemory(&eff, sizeof(eff));
        eff.dwSize                = sizeof(DIEFFECT);
        eff.dwFlags               = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
        eff.cAxes                 = m_dwNumForceFeedbackAxis;
        eff.rglDirection          = rglDirection;
        eff.lpEnvelope            = 0;
        eff.cbTypeSpecificParams  = sizeof(DICONSTANTFORCE);
        eff.lpvTypeSpecificParams = &cf;
        eff.dwStartDelay          = 0;

        m_pEffect->SetParameters( &eff, DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS | DIEP_START );
#endif //FORCE_FEEDBACK
      }
      else
      {
        switch(m_hResult)
        {
        case DIERR_INPUTLOST:
          __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "DIERR_INPUTLOST");
          break;
        case DIERR_INVALIDPARAM:
          __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "DIERR_INVALIDPARAM");
          break;
        case DIERR_NOTACQUIRED:
          __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "DIERR_NOTACQUIRED");
          break;
        case DIERR_NOTINITIALIZED:
          __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "DIERR_NOTINITIALIZED");
          break;
        case E_PENDING:
          __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "E_PENDING");
          break;
        }
      }
    }
    else
    {
      m_hResult = m_pDevice->Acquire();
      while( m_hResult == DIERR_INPUTLOST )
      {
        __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "STICK LOST");
        m_hResult = m_pDevice->Acquire();
      }
    }

  }

  //CHAR acStr[64];
  //if(m_kJS.lX != __m_kJS.lX)
  //{
  //  sprintf_s(&acStr[0], sizeof(acStr), "lX: %d", m_kJS.lX);
  //  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
  //}
  //if(m_kJS.lY != __m_kJS.lY)
  //{
  //  sprintf_s(&acStr[0], sizeof(acStr), "lY: %d", m_kJS.lY);
  //  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
  //}
  //if(m_kJS.lZ != __m_kJS.lZ)
  //{
  //  sprintf_s(&acStr[0], sizeof(acStr), "lZ: %d", m_kJS.lZ);
  //  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
  //}
  //if(m_kJS.lRx != __m_kJS.lRx)
  //{
  //  sprintf_s(&acStr[0], sizeof(acStr), "lRx: %d", m_kJS.lRx);
  //  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
  //}
  //if(m_kJS.lRy != __m_kJS.lRy)
  //{
  //  sprintf_s(&acStr[0], sizeof(acStr), "lRy: %d", m_kJS.lRy);
  //  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
  //}
  //if(m_kJS.rglSlider[0] != __m_kJS.rglSlider[0])
  //{
  //  sprintf_s(&acStr[0], sizeof(acStr), "S0: %d", m_kJS.rglSlider[0]);
  //  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
  //}

  //__m_kJS.lX           = m_kJS.lX;
  //__m_kJS.lY           = m_kJS.lY;
  //__m_kJS.lZ           = m_kJS.lZ;
  //__m_kJS.lRy          = m_kJS.lRy;
  //__m_kJS.lRz          = m_kJS.lRz;
  //__m_kJS.rglSlider[0] = m_kJS.rglSlider[0];
  //__m_kJS.rglSlider[1] = m_kJS.rglSlider[1];

}
