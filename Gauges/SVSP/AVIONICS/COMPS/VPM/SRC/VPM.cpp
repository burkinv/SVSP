#include "VPM.h"
#include "AC.h"
#include "SS.h"

VPM::VPM(__AAVAvionics *pkAvionics, DWORD dwThreadSleepTime) : __AAVModule("VPM", pkAvionics, dwThreadSleepTime, NULL, NULL)
{
}

VPM::~VPM()
{
}

VOID VPM::Process()
{
  //MSG       kMsg;
  DWORD dwResult;

  AAVVector3<float>    kVP_PreviousVector;
  AAVVector3<float>    kTP_Vector;

  UINT                  uiDist;
  UINT                  uiPreviousDist = 0;
  bool                  bClosing       = false;

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN(pkAC  != NULL, 0xFFFFFFFF);

  SS *__pkSS = static_cast<SS *>(m_pkAvionics->GetModule(__AAVAvionics::AVIONICS_MODULE_ISS));
  AAV_ASSERT_RETURN(__pkSS != NULL, 0xFFFFFFFF);

#if 0
  uiDist = (UINT)(__pAC->m_kVP_Vector - __pAC->m_kVector).Length();

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case MESSAGE_UPDATE_TOWER_VECTOR:
      if((kVP_PreviousVector == __pAC->m_kVP_Vector) || (uiDist > 1500))
      {
        ////////////////////////////////////////////////////////
        // EXTERNAL FLY-BY MODE
        ////////////////////////////////////////////////////////
        if(__pAC->m_kAC_GlobalVelocityVector.Length() != 0)
        {
          kTP_Vector = __pAC->m_kVector + __pAC->m_kAC_GlobalVelocityVector.Normalize()*((__pAC->m_bOnGround) ? 300.0f*(1.0f + __pAC->m_dMach) : 1000.0f*(1.0f + __pAC->m_dMach));
        }
        else
        {
          kTP_Vector = __pAC->m_kVector + __pAC->m_kVector.Normalize()*500;
        }

        __AAVGeoCoordinates3<long long> kNewTP_Coord = __AAVGeoCoordinates3<long long>(kTP_Vector);

        m_pIPC->Write(0x0D50, 24, &kNewTP_Coord,         &dwResult);
        m_pIPC->Process(&dwResult);
      }
      break;
    }
  }

  //if(uiDist > ((__pAC->m_bOnGround)? 300U:1000U))
  //{
  //  if(__pAC->m_kAC_GlobalVelocityVector.Length() != 0)
  //  {
  //    kTP_Vector = __pAC->m_kVector + __pAC->m_kAC_GlobalVelocityVector.Normalize()*((__pAC->m_bOnGround)? 300.0f:1000.0f);
  //  }
  //  else
  //  {
  //    kTP_Vector = __pAC->m_kVector + __pAC->m_kVector.Normalize()*500;
  //  }

  //  __AAVGeoCoordinates3<long long> kNewTP_Coord = __AAVGeoCoordinates3<long long>(kTP_Vector);

  //  m_pIPC->Write(0x0D50, 24, &kNewTP_Coord,         &dwResult);
  //  m_pIPC->Process(&dwResult);
  //}

  if((kVP_PreviousVector == __pAC->m_kVP_Vector) && (__pAC->m_usPaused == 0))
  {
    if(uiDist > uiPreviousDist)
    {
      if(bClosing)
      {
        bClosing = false;
        if(__pAC->m_dMach > 1.0f)
        {
          ////////////////////////////////////////////////////////
          // SUPERSONIC FLY-BY
          ////////////////////////////////////////////////////////
          if(__pkSS != NULL)
          {
            PostThreadMessage(__pkSS->GetThreadId(), SS::MESSAGE_PLAY_SOUND, SS::SOUND_ID_SONIC_BOOM, SS::MESSAGE_PLAY_SOUND_HIGH_PRIORITY);
          }
        }
      }
    }
    else
    {
      bClosing = true;
    }

    uiPreviousDist = uiDist;
  }

  kVP_PreviousVector = __pAC->m_kVP_Vector;

#endif //0
}
