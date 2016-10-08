#ifndef	__HOTAS_MODULE_H__
#define	__HOTAS_MODULE_H__

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "__SVSPModule.h"

//#define FORCE_FEEDBACK

typedef enum 
{
  HOTAS_MESSAGE_SYSTEM_FAILURE = (WM_USER + 1304),
} HOTAS_MESSAGE;

class HOTAS : public __SVSPModule
{
public:
  static const UINT            SINGLE_RANGE_ZONE;

                               HOTAS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual                     ~HOTAS();
  inline const LPDIRECTINPUT8  GetDI() const { return m_pDI; }
  inline VOID                  SetDevice(LPDIRECTINPUTDEVICE8 pkDevice) { m_pDevice = pkDevice; }
  DIJOYSTATE                   m_kJS;

private:
  DIJOYSTATE                   m_kDeviceState;
  virtual VOID                 Process();
  VOID                         TryDeviceDetect();


  inline VOID                __CHECK_BUTTON_STATE(UINT uiButtonNum,  BOOL bEnableRepeat, __SVSPModule *pkModule, UINT BUTTON_PRESSED_MESSAGE, UINT BUTTON_RELEASED_MESSAGE)
  {
    if(pkModule != NULL)
    {
      if(uiButtonNum < sizeof(m_kJS.rgbButtons))
      {
        if(m_kJS.rgbButtons[uiButtonNum] & 0x80)
        {
          if(bEnableRepeat || !m_bButtonState[uiButtonNum])
          {
            if(BUTTON_PRESSED_MESSAGE != NULL)
            {
              PostThreadMessage(pkModule->GetThreadId(), BUTTON_PRESSED_MESSAGE, 0, 0);
            }
            m_bButtonState[uiButtonNum] = true;
          }
        }
        else
        {
          if(m_bButtonState[uiButtonNum])
          {
            if(BUTTON_RELEASED_MESSAGE != NULL)
            {
              PostThreadMessage(pkModule->GetThreadId(), BUTTON_RELEASED_MESSAGE, 0, 0);
            }
            m_bButtonState[uiButtonNum] = false;
          }
        }
      }
    }
  }

  inline VOID               __CHECK_AXYS_STATE(UINT uiAxisNum, __SVSPModule *pkModule, UINT AXYS_VALUE_CHANGED_MESSAGE)
  {
    if(pkModule != NULL)
      {
      if(m_kJS.lRx != __m_kJS.lRx)
      {
        PostThreadMessage(pkModule->GetThreadId(), AXYS_VALUE_CHANGED_MESSAGE, m_kJS.lRx, 0);
        __m_kJS.lRx = m_kJS.lRx;
      }
    }
  }

  inline VOID               __CHECK_SLIDER_STATE_INCREMENT(__SVSPModule *pkModule, UINT SLIDER_OUTBOUND_MESSAGE, UINT SLIDER_INBOUND_MESSAGE)
  {
    if(pkModule != NULL)
    {
      if(m_kJS.rglSlider[0] > 60000)
      {
        if(__m_kJS.rglSlider[0] <= 60000)
        {
          if((SLIDER_OUTBOUND_MESSAGE != NULL) && (pkModule != NULL))
            PostThreadMessage(pkModule->GetThreadId(), SLIDER_OUTBOUND_MESSAGE, 0, 0);

          __m_kJS.rglSlider[0] = m_kJS.rglSlider[0];
        }
      }
      else
      {
        if(__m_kJS.rglSlider[0] > 60000)
        {
          if((SLIDER_INBOUND_MESSAGE != NULL) && (pkModule != NULL))
            PostThreadMessage(pkModule->GetThreadId(), SLIDER_INBOUND_MESSAGE, 0, 0);

          __m_kJS.rglSlider[0] = m_kJS.rglSlider[0];
        }
      }
    }
  }

  inline VOID               __CHECK_SLIDER_STATE_DECREMENT(__SVSPModule *pkModule, UINT SLIDER_OUTBOUND_MESSAGE, UINT SLIDER_INBOUND_MESSAGE)
  {
    if(pkModule != NULL)
    {
      if(m_kJS.rglSlider[0] < 1000)
      {
        if(__m_kJS.rglSlider[0] >= 1000)
        {
          if((SLIDER_OUTBOUND_MESSAGE != NULL) && (pkModule != NULL))
            PostThreadMessage(pkModule->GetThreadId(), SLIDER_OUTBOUND_MESSAGE, 0, 0);

          __m_kJS.rglSlider[0] = m_kJS.rglSlider[0];
        }
      }
      else
      {
        if(__m_kJS.rglSlider[0] < 1000)
        {
          if((SLIDER_INBOUND_MESSAGE != NULL) && (pkModule != NULL))
            PostThreadMessage(pkModule->GetThreadId(), SLIDER_INBOUND_MESSAGE, 0, 0);

          __m_kJS.rglSlider[0] = m_kJS.rglSlider[0];
        }
      }
    }
  }

  VOID                       __CHECK_RADAR_RANGE(UINT uiRadarAxisValue, class FCR *pkFCR, UINT AXYS_VALUE_CHANGED_MESSAGE);

  DIJOYSTATE                 __m_kJS;
  bool                         m_bButtonState[32];
  UINT                         m_uiCurrentRadarRangeIndex;
  UINT                         m_uiRadarAxisValue;

  LPDIRECTINPUT8               m_pDI;
  LPDIRECTINPUTDEVICE8         m_pDevice;

#ifdef FORCE_FEEDBACK
  LPDIRECTINPUTEFFECT   m_pEffect;
  BOOL                  m_bActive;
  DWORD                 m_dwNumForceFeedbackAxis;
  INT                   m_nXForce;
  INT                   m_nYForce;
  DWORD                 m_dwLastEffectSet;
#endif //FORCE_FEEDBACK
};


#endif	/* __HOTAS_MODULE_H__ */
