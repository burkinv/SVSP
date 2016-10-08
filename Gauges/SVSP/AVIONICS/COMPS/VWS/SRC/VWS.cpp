#include "stdio.h"
#include "AC.h"
#include "VWS.h"
#include "SS.h"
#include "FLCS.h"
#include "SIM.h"
#include "ECS.h"

VWS::ASSERTION_STATE VWS::g_akCheckListAssertionTable[VWS::MAX_NOF_ASSERTIONS] = 
{//bFailed, bMasterWarningSet, dFirstFailedDetectionTime, dDurationOfFailedAssertion, dLastNotificationTime, dNotificationDelayTime, dNotificationIntervalTime, bMasterWarningRequired, dwSoundId,                                dwSoundFlag,                           acItemName[32],   bBlinkRequired.
  {false,   false,             0,                         0,                          0,                     0,                       0,                        false,                  SS::SOUND_ID_MASTER_WARNING,              SS::MESSAGE_LOOP_SOUND              ,  "WARN"           ,true },
  {false,   false,             0,                         0,                          0,                     1,                       2,                        true,                   SS::SOUND_ID_BINGO_SINKRATE,              SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "PULL UP"        ,true },
  {false,   false,             0,                         0,                          0,                     0,                       2,                        true,                   SS::SOUND_ID_STALL_WARNING,               SS::MESSAGE_LOOP_SOUND              ,  "STALL"          ,true },
  {false,   false,             0,                         0,                          0,                     1,                       0,                        false,                  SS::SOUND_ID_NONE,                        NULL                                ,  "AIRBRAKE"       ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        true,                   SS::SOUND_ID_MINIMUM_SPEED,               SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "MIN SPEED"      ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        true,                   SS::SOUND_ID_MAXIMUM_SPEED,               SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "MAX SPEED"      ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        true,                   SS::SOUND_ID_BINGO_ALTITUDE,              SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "ALTITUDE"       ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        false,                  SS::SOUND_ID_MAXIMUM_G,                   SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "MAX G"          ,true },
  {false,   false,             0,                         0,                          0,                     1,                       5,                        false,                  SS::SOUND_ID_MAXIMUM_ANGLE_OF_ATTACK,     SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "MAX AOA"        ,true },
  {false,   false,             0,                         0,                          0,                     0,                       3,                        true,                   SS::SOUND_ID_ANGLE_OF_ATTACK_OVER_LIMIT,  SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "AOA OVER LIM"   ,true },
  {false,   false,             0,                         0,                          0,                     1,                      15,                        false,                  SS::SOUND_ID_WARNING_GEAR_UP,             SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "GEAR UP"        ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        false,                  SS::SOUND_ID_WARNING_GEAR_DOWN,           SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "GEAR DOWN"      ,true },
  {false,   false,             0,                         0,                          0,                     0,                       0,                        false,                  SS::SOUND_ID_NONE,                        NULL                                ,  "FUEL LOW"       ,false},
  {false,   false,             0,                         0,                          0,                     0,                      60,                        false,                  SS::SOUND_ID_FUEL_1500,                   SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "FUEL 1500"      ,false},
  {false,   false,             0,                         0,                          0,                     0,                      60,                        false,                  SS::SOUND_ID_FUEL_800,                    SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "FUEL 800"       ,false},
  {false,   false,             0,                         0,                          0,                     0,                      30,                        false,                  SS::SOUND_ID_FUEL_500,                    SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "FUEL 500"       ,false},
  {false,   false,             0,                         0,                          0,                     0,                      15,                        false,                  SS::SOUND_ID_BINGO_FUEL,                  SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "BINGO FUEL"     ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        false,                  SS::SOUND_ID_NONE,                        NULL                                ,  "REHEAT"         ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        true,                   SS::SOUND_ID_THROTTLE_BACK_LEFT_ENGINE,   SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "THROTTLE BACK"  ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        true,                   SS::SOUND_ID_THROTTLE_BACK_RIGHT_ENGINE,  SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "THROTTLE BACK"  ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        true,                   SS::SOUND_ID_THROTTLE_BACK_ENGINE,        SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "THROTTLE BACK"  ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        false,                  SS::SOUND_ID_MACH_ONE,                    SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "MACH ONE"       ,true },
  {false,   false,             0,                         0,                          0,                     1,                       3,                        false,                  SS::SOUND_ID_MACH_TWO,                    SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY,  "MACH TWO"       ,true },
  {false,   false,             0,                         0,                          0,                     1,                       2,                        true,                   SS::SOUND_ID_NONE,                        NULL                                ,  "PARKING ON"     ,true },
};

#define VWS_ASSERT(b, dwAssertID) \
{ \
    EvaluateAssertionState(b, GetSystemTime(), pkSS, dwAssertID); \
} \

VOID VWS::EvaluateAssertionState(bool bExpression, DOUBLE dAbsoluteTime, SS *pkSS, DWORD dwAssertID)
{
  AAV_ASSERT_RETURN_VOID((dwAssertID < sizeof(g_akCheckListAssertionTable)/sizeof(ASSERTION_STATE)) != NULL);
  ASSERTION_STATE *pkAssertionState = &g_akCheckListAssertionTable[dwAssertID];

  if(!bExpression)
  {
    if(pkAssertionState->dFirstFailedDetectionTime == 0)
    {
      //Start Assertion timer.
      pkAssertionState->dFirstFailedDetectionTime = dAbsoluteTime;
    }
    else
    {
      pkAssertionState->dDurationOfFailedAssertion = dAbsoluteTime - pkAssertionState->dFirstFailedDetectionTime;
      //Process Assertion after required notification delay.
      if(pkAssertionState->dDurationOfFailedAssertion > pkAssertionState->dNotificationDelayTime)
      {
        //Signal failed Assertion
        pkAssertionState->bFailed = true;

        //Signal Master Warning if required.
        if(pkAssertionState->bMasterWarningRequired && !pkAssertionState->bMasterWarningSet)
        {
          m_iMasterWarningCount += 1;
          pkAssertionState->bMasterWarningSet = true;
        }

        pkAssertionState->dDurationOfFailedAssertion = 0;

        if((pkAssertionState->dwSoundID != SS::SOUND_ID_NONE) && (pkSS != NULL))
        {
          switch(pkAssertionState->dwSoundFlag)
          {
          case SS::MESSAGE_LOOP_SOUND:
            if(pkSS->GetSoundState(pkAssertionState->dwSoundID) == 0)
            {
              pkSS->PlaySound(pkAssertionState->dwSoundID, SS::MESSAGE_LOOP_SOUND);
            }
            break;

          case SS::MESSAGE_PLAY_SOUND_HIGH_PRIORITY:
              pkSS->PlaySound(pkAssertionState->dwSoundID, SS::MESSAGE_PLAY_SOUND_HIGH_PRIORITY);
            break;

          case SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY:
            {
              DOUBLE dDeltaTime = dAbsoluteTime - pkAssertionState->dLastNotificationTime;
              if(dDeltaTime > pkAssertionState->dNotificationIntervalTime)
              {
                pkSS->PlaySound(pkAssertionState->dwSoundID, SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY);
                pkAssertionState->dLastNotificationTime = dAbsoluteTime;
              }
            }
            break;

          default:
            break;
          }
        }
      }
    }
  }
  else
  {
    // Reset duration time of a failed Assertion.
    if(pkAssertionState->dFirstFailedDetectionTime != 0)
    {
      pkAssertionState->bFailed                    = false;
      pkAssertionState->dFirstFailedDetectionTime  = 0;
      pkAssertionState->dDurationOfFailedAssertion = 0;
      pkAssertionState->dLastNotificationTime      = 0;
      if(pkAssertionState->bMasterWarningRequired && pkAssertionState->bMasterWarningSet)
      {
        m_iMasterWarningCount -= 1;
        pkAssertionState->bMasterWarningSet = false;
      }
    }

    if((pkAssertionState->dwSoundID != SS::SOUND_ID_NONE) && (pkSS != NULL))
    {
      switch(pkAssertionState->dwSoundFlag)
      {
      case SS::MESSAGE_LOOP_SOUND:
        {
          if(pkSS->GetSoundState(pkAssertionState->dwSoundID) != 0)
          {
            pkSS->StopSound(pkAssertionState->dwSoundID);
          }
        }
        break;

      default:
        break;
      }
    }
  }

  ////////////////////////////////////////////////////////////
  // PROCESS MASTER WARNING STATE
  ////////////////////////////////////////////////////////////
  ProcessMasterWarningState(pkSS);
}

VWS::VWS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("VWS", pkAvionics, dwThreadSleepTime, false, NULL, NULL, NULL)
{
  for(UINT i = 0; i < sizeof(g_akCheckListAssertionTable)/sizeof(g_akCheckListAssertionTable[0]); ++i)
  {
    ASSERTION_STATE *pkAssertionState = &g_akCheckListAssertionTable[i];
    pkAssertionState->bMasterWarningSet          = false;
    pkAssertionState->dFirstFailedDetectionTime  = 0;
    pkAssertionState->dDurationOfFailedAssertion = 0;
    pkAssertionState->dLastNotificationTime      = 0;
  }

  m_iMasterWarningCount = 0;
}

VWS::~VWS()
{
  __SVSPModule::Stop();
}

VOID VWS::ProcessMasterWarningState(SS *pkSS)
{

  ASSERTION_STATE *pkMasterWarningAssertionState = &g_akCheckListAssertionTable[ASSERT_ID_MASTER_WARNING];

  if((pkMasterWarningAssertionState->dwSoundID != SS::SOUND_ID_NONE) && (pkSS != NULL))
  {
    DWORD dwSoundState = pkSS->GetSoundState(pkMasterWarningAssertionState->dwSoundID);
    if(m_iMasterWarningCount > 0)
    {
      pkMasterWarningAssertionState->bFailed = true;
      if(dwSoundState == 0)
      {
        pkSS->PlaySound(pkMasterWarningAssertionState->dwSoundID, pkMasterWarningAssertionState->dwSoundFlag);
      }
    }
    else
    {
      pkMasterWarningAssertionState->bFailed = false;
      if(dwSoundState != 0)
      {
        pkSS->StopSound(pkMasterWarningAssertionState->dwSoundID);
      }
    }
  }

  if(m_iMasterWarningCount < 0)
  {
    //BUG::
    m_iMasterWarningCount = 0;
    AAV_ASSERT_RETURN_VOID(false);
  }
}

VOID VWS::Start()
{
  m_iMasterWarningCount = 0;

  __SVSPModule::Start();
}

VOID VWS::Process()
{
  AC  *pkAC      = static_cast<AC   *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  AC::AIRCRAFT_STATE_DATA *pkACData = &pkAC->m_kData;
  AAV_ASSERT_RETURN_VOID(pkACData  != NULL);
  SIM  *pkSIM    = static_cast<SIM   *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SIM));
  FLCS *pkFLCS   = static_cast<FLCS  *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FLCS));
  ECS  *pkECS    = static_cast<ECS   *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  SS   *pkSS     = static_cast<SS    *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ISS));

  if(m_pkAvionics->IsValid() && (pkFLCS != 0))
  {
    //if((strcmp(&pkACData->m_acACType[0], "TECH_DEMO_01") == 0) && 
    //{

    INT iFuelTotalQuantityWeightKg    = 0;
    INT iFuelTotalQuantityWeightKgMod = 0;

    if(pkECS != 0)
    {
      INT iFuelTotalQuantityWeightKg = abs(static_cast<INT>(pkECS->GetTotalFuelQuantityKg()));
      INT iFuelTotalQuantityWeightKgMod = iFuelTotalQuantityWeightKg%5;

      if((iFuelTotalQuantityWeightKg <= 1550) && (iFuelTotalQuantityWeightKgMod == 0))
      {
        sprintf_s(&(VWS::g_akCheckListAssertionTable[ASSERT_ID_FUEL_LOW].acItemName)[5], sizeof(g_akCheckListAssertionTable[ASSERT_ID_FUEL_LOW].acItemName) - 5, "%d\0", iFuelTotalQuantityWeightKg);
      }
    }

    /////////////////////////////////////////////////////
    // STATE PARAMETER ASSERTION TABLE
    /////////////////////////////////////////////////////
    switch(pkFLCS->GetState())
    {
    case FLCS::FLCS_STATE_SLEW:
      //FLCS
      if(pkAC != 0)
      {
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_AIRBRAKE                   );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MINIMUM_SPEED              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MAXIMUM_SPEED              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_BINGO_ALTITUDE             );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_BINGO_SINKRATE             );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MAXIMUM_G                  );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MAXIMUM_ANGLE_OF_ATTACK    );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_ANGLE_OF_ATTACK_OVER_LIMIT );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_STALL_WARNING              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_WARNING_GEAR_DOWN          );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_WARNING_GEAR_UP            );
      }
      // ECS
      if(pkECS != 0)
      {
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_ENGINE_REHEAT_ON           );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_LEFT_ENGINE_CRITICAL       );
      //VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_RIGHT_ENGINE_CRITICAL      );
        // FUEL
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_FUEL_LOW                   );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_FUEL_1500                  );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_FUEL_800                   );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_FUEL_500                   );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_BINGO_FUEL                 );
      }
      // SYSTEMS
      VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_PARKING_BRAKE              );
      break;

    case FLCS::FLCS_STATE_HOVER:
      //FLCS
      if(pkAC != 0)
      {
        VWS_ASSERT(pkACData->m_dSpoilerExtentionPercent == 0,                                                                                      VWS::ASSERT_ID_AIRBRAKE                   );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MINIMUM_SPEED              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MAXIMUM_SPEED              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_BINGO_ALTITUDE             );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_BINGO_SINKRATE             );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MAXIMUM_G                  );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MAXIMUM_ANGLE_OF_ATTACK    );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_ANGLE_OF_ATTACK_OVER_LIMIT );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_STALL_WARNING              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_WARNING_GEAR_DOWN          );
        VWS_ASSERT(((pkACData->m_dGearPosition == 1) || (pkAC->m_dRadarAltitude > FLCS::BINGO_ALTITUDE)),                                          VWS::ASSERT_ID_WARNING_GEAR_UP            );
      }
      // ECS
      if(pkECS != 0)
      {
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_ENGINE_REHEAT_ON           );
        VWS_ASSERT((pkECS->m_akEngine[0].fN[1] < ECS::MAX_N2_RPM) && (pkECS->m_akEngine[0].dITT < ECS::MAX_ITT),                                   VWS::ASSERT_ID_LEFT_ENGINE_CRITICAL       );
      //VWS_ASSERT((pkECS->m_akEngine[1].fN[1] < ECS::MAX_N2_RPM) && (pkECS->m_akEngine[1].dITT < ECS::MAX_ITT),                                   VWS::ASSERT_ID_RIGHT_ENGINE_CRITICAL      );
        // FUEL
        VWS_ASSERT((iFuelTotalQuantityWeightKg > 1550),                                                                                            VWS::ASSERT_ID_FUEL_LOW                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg > 1550) || (iFuelTotalQuantityWeightKg < 1450),                                                     VWS::ASSERT_ID_FUEL_1500                  );
        VWS_ASSERT((iFuelTotalQuantityWeightKg >  850) || (iFuelTotalQuantityWeightKg <  750),                                                     VWS::ASSERT_ID_FUEL_800                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg >  550) || (iFuelTotalQuantityWeightKg <  450),                                                     VWS::ASSERT_ID_FUEL_500                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg > ECS::BINGO_FUEL),                                                                                 VWS::ASSERT_ID_BINGO_FUEL                 );
      }
      // SYSTEMS
      VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_PARKING_BRAKE              );
      break;

    case FLCS::FLCS_STATE_ON_GROUND:
      //FLCS
      if(pkAC != 0)
      {
        VWS_ASSERT(pkACData->m_dSpoilerExtentionPercent == 0,                                                                                      VWS::ASSERT_ID_AIRBRAKE                   );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MINIMUM_SPEED              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MAXIMUM_SPEED              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_BINGO_ALTITUDE             );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_BINGO_SINKRATE             );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MAXIMUM_G                  );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_MAXIMUM_ANGLE_OF_ATTACK    );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_ANGLE_OF_ATTACK_OVER_LIMIT );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_STALL_WARNING              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_WARNING_GEAR_DOWN          );
        VWS_ASSERT((pkACData->m_dGearPosition == 1),                                                                                               VWS::ASSERT_ID_WARNING_GEAR_UP            );
      }
      // ECS
      if(pkECS != 0)
      {
        VWS_ASSERT((pkECS->m_akEngine[0].dwAB == 0),                                                                                               VWS::ASSERT_ID_ENGINE_REHEAT_ON           );
        VWS_ASSERT((pkECS->m_akEngine[0].fN[1] < ECS::MAX_N2_RPM) && (pkECS->m_akEngine[0].dITT < ECS::MAX_ITT),                                   VWS::ASSERT_ID_LEFT_ENGINE_CRITICAL       );
      //VWS_ASSERT((pkECS->m_akEngine[1].fN[1] < ECS::MAX_N2_RPM) && (pkECS->m_akEngine[1].dITT < ECS::MAX_ITT),                                   VWS::ASSERT_ID_RIGHT_ENGINE_CRITICAL      );
        // FUEL
        VWS_ASSERT((iFuelTotalQuantityWeightKg > 1550),                                                                                            VWS::ASSERT_ID_FUEL_LOW                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg > 1550) || (iFuelTotalQuantityWeightKg < 1450),                                                     VWS::ASSERT_ID_FUEL_1500                  );
        VWS_ASSERT((iFuelTotalQuantityWeightKg >  850) || (iFuelTotalQuantityWeightKg <  750),                                                     VWS::ASSERT_ID_FUEL_800                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg >  550) || (iFuelTotalQuantityWeightKg <  450),                                                     VWS::ASSERT_ID_FUEL_500                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg > ECS::BINGO_FUEL),                                                                                 VWS::ASSERT_ID_BINGO_FUEL                 );
        // SYSTEMS
        VWS_ASSERT((pkACData->m_dBrakeParking == 0) || (pkECS->m_akEngine[0].dThrustPercent < 25),                                                 VWS::ASSERT_ID_PARKING_BRAKE              );
      }
      break;

    case FLCS::FLCS_STATE_AIRBORNE_GEAR_DOWN:
      //FLCS
      if(pkAC != 0)
      {
        VWS_ASSERT(pkACData->m_dSpoilerExtentionPercent == 0,                                                                                      VWS::ASSERT_ID_AIRBRAKE                   );
        VWS_ASSERT(pkACData->m_dIndicatedAirspeed > FLCS::MINIMUM_SPEED,                                                                           VWS::ASSERT_ID_MINIMUM_SPEED              );
        VWS_ASSERT(pkACData->m_dIndicatedAirspeed < FLCS::MAXIMUM_SPEED,                                                                           VWS::ASSERT_ID_MAXIMUM_SPEED              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_BINGO_ALTITUDE             );
        VWS_ASSERT((pkAC->m_dRadarVerticalVelocity > -pkAC->m_dRadarAltitude/7) || (pkAC->m_kBodyAttitude.x < FLCS::MINIMAL_LANDING_PITCH_ANGLE),  VWS::ASSERT_ID_BINGO_SINKRATE             );
        VWS_ASSERT(pkACData->m_dG < FLCS::MAXIMUM_G,                                                                                               VWS::ASSERT_ID_MAXIMUM_G                  );
        VWS_ASSERT((pkAC->m_fAoA < FLCS::MAXIMUM_ANGLE_OF_ATTACK) || (pkAC->m_fAoA > FLCS::CRITICAL_ANGLE_OF_ATTACK),                              VWS::ASSERT_ID_MAXIMUM_ANGLE_OF_ATTACK    );
        VWS_ASSERT((pkAC->m_fAoA < FLCS::CRITICAL_ANGLE_OF_ATTACK),                                                                                VWS::ASSERT_ID_ANGLE_OF_ATTACK_OVER_LIMIT );
        VWS_ASSERT((pkAC->m_fAoA < FLCS::CRITICAL_ANGLE_OF_ATTACK),                                                                                VWS::ASSERT_ID_STALL_WARNING              );
        VWS_ASSERT((pkACData->m_dIndicatedAirspeed < FLCS::GEAR_DOWN_WARNING_SPEED),                                                               VWS::ASSERT_ID_WARNING_GEAR_DOWN          );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_WARNING_GEAR_UP            );
      }
      // ECS
      if(pkECS != 0)
      {
        VWS_ASSERT((pkECS->m_akEngine[0].dwAB == 0),                                                                                               VWS::ASSERT_ID_ENGINE_REHEAT_ON           );
        VWS_ASSERT((pkECS->m_akEngine[0].fN[1] < ECS::MAX_N2_RPM) && (pkECS->m_akEngine[0].dITT < ECS::MAX_ITT),                                   VWS::ASSERT_ID_LEFT_ENGINE_CRITICAL       );
      //VWS_ASSERT((pkECS->m_akEngine[1].fN[1] < ECS::MAX_N2_RPM) && (pkECS->m_akEngine[1].dITT < ECS::MAX_ITT),                                   VWS::ASSERT_ID_RIGHT_ENGINE_CRITICAL      );
        // FUEL
        VWS_ASSERT((iFuelTotalQuantityWeightKg > 1550),                                                                                            VWS::ASSERT_ID_FUEL_LOW                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg > 1550) || (iFuelTotalQuantityWeightKg < 1450),                                                     VWS::ASSERT_ID_FUEL_1500                  );
        VWS_ASSERT((iFuelTotalQuantityWeightKg >  850) || (iFuelTotalQuantityWeightKg <  750),                                                     VWS::ASSERT_ID_FUEL_800                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg >  550) || (iFuelTotalQuantityWeightKg <  450),                                                     VWS::ASSERT_ID_FUEL_500                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg > ECS::BINGO_FUEL),                                                                                 VWS::ASSERT_ID_BINGO_FUEL                 );
        // SYSTEMS
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_PARKING_BRAKE              );
      }
      break;

    case FLCS::FLCS_STATE_AIRBORNE:
      //FLCS
      if(pkAC != 0)
      {
        VWS_ASSERT(pkACData->m_dSpoilerExtentionPercent == 0,                                                                                      VWS::ASSERT_ID_AIRBRAKE                   );
        VWS_ASSERT(pkACData->m_dIndicatedAirspeed > FLCS::MINIMUM_SPEED,                                                                           VWS::ASSERT_ID_MINIMUM_SPEED              );
        VWS_ASSERT(pkACData->m_dIndicatedAirspeed < FLCS::MAXIMUM_SPEED,                                                                           VWS::ASSERT_ID_MAXIMUM_SPEED              );
        VWS_ASSERT((pkAC->m_dRadarAltitude > FLCS::BINGO_ALTITUDE),                                                                                VWS::ASSERT_ID_BINGO_ALTITUDE             );
        VWS_ASSERT((pkAC->m_dRadarVerticalVelocity > -pkAC->m_dRadarAltitude/7),                                                                   VWS::ASSERT_ID_BINGO_SINKRATE             );
        VWS_ASSERT(pkACData->m_dG < FLCS::MAXIMUM_G,                                                                                               VWS::ASSERT_ID_MAXIMUM_G                  );
        VWS_ASSERT((pkAC->m_fAoA < FLCS::MAXIMUM_ANGLE_OF_ATTACK) || (pkAC->m_fAoA > FLCS::CRITICAL_ANGLE_OF_ATTACK),                              VWS::ASSERT_ID_MAXIMUM_ANGLE_OF_ATTACK    );
        VWS_ASSERT((pkAC->m_fAoA < FLCS::CRITICAL_ANGLE_OF_ATTACK),                                                                                VWS::ASSERT_ID_ANGLE_OF_ATTACK_OVER_LIMIT );
        VWS_ASSERT((pkAC->m_fAoA < FLCS::CRITICAL_ANGLE_OF_ATTACK),                                                                                VWS::ASSERT_ID_STALL_WARNING              );
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_WARNING_GEAR_DOWN          );
        VWS_ASSERT((pkACData->m_dIndicatedAirspeed > FLCS::GEAR_UP_WARNING_SPEED) || 
                  ((pkAC->m_dRadarAltitude > FLCS::BINGO_ALTITUDE) || (pkACData->m_dIndicatedVerticalSpeed > 0)),                                  VWS::ASSERT_ID_WARNING_GEAR_UP            );
      }
      // ECS
      if(pkECS != 0)
      {
        VWS_ASSERT((pkECS->m_akEngine[0].dwAB == 0),                                                                                               VWS::ASSERT_ID_ENGINE_REHEAT_ON           );
        VWS_ASSERT((pkECS->m_akEngine[0].fN[1] < ECS::MAX_N2_RPM) && (pkECS->m_akEngine[0].dITT < ECS::MAX_ITT),                                   VWS::ASSERT_ID_LEFT_ENGINE_CRITICAL       );
      //VWS_ASSERT((pkECS->m_akEngine[1].fN[1] < ECS::MAX_N2_RPM) && (pkECS->m_akEngine[1].dITT < ECS::MAX_ITT),                                   VWS::ASSERT_ID_RIGHT_ENGINE_CRITICAL      );
        // FUEL
        VWS_ASSERT((iFuelTotalQuantityWeightKg > 1550),                                                                                            VWS::ASSERT_ID_FUEL_LOW                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg > 1550) || (iFuelTotalQuantityWeightKg < 1450),                                                     VWS::ASSERT_ID_FUEL_1500                  );
        VWS_ASSERT((iFuelTotalQuantityWeightKg >  850) || (iFuelTotalQuantityWeightKg <  750),                                                     VWS::ASSERT_ID_FUEL_800                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg >  550) || (iFuelTotalQuantityWeightKg <  450),                                                     VWS::ASSERT_ID_FUEL_500                   );
        VWS_ASSERT((iFuelTotalQuantityWeightKg > ECS::BINGO_FUEL),                                                                                 VWS::ASSERT_ID_BINGO_FUEL                 );
        // SYSTEMS
        VWS_ASSERT(TRUE,                                                                                                                           VWS::ASSERT_ID_PARKING_BRAKE              );
      }
      break;


    default:
      break;
    }
  }
}
