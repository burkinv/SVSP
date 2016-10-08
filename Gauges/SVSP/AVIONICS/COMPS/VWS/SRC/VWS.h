#ifndef	__VWS_MODULE_H__
#define	__VWS_MODULE_H__

#include "__SVSPModule.h"
#include "__SVSPAvionics.h"
#include "SS.h"

class VWS : public __SVSPModule
{
public:
  enum
  {
    ASSERT_ID_MASTER_WARNING,
    ASSERT_ID_BINGO_SINKRATE,
    ASSERT_ID_STALL_WARNING,
    ASSERT_ID_AIRBRAKE,
    ASSERT_ID_MINIMUM_SPEED,
    ASSERT_ID_MAXIMUM_SPEED,
    ASSERT_ID_BINGO_ALTITUDE,
    ASSERT_ID_MAXIMUM_G,
    ASSERT_ID_MAXIMUM_ANGLE_OF_ATTACK,
    ASSERT_ID_ANGLE_OF_ATTACK_OVER_LIMIT,
    ASSERT_ID_WARNING_GEAR_UP,
    ASSERT_ID_WARNING_GEAR_DOWN,
    ASSERT_ID_FUEL_LOW,
    ASSERT_ID_FUEL_1500,
    ASSERT_ID_FUEL_800,
    ASSERT_ID_FUEL_500,
    ASSERT_ID_BINGO_FUEL,
    ASSERT_ID_ENGINE_REHEAT_ON,
    ASSERT_ID_LEFT_ENGINE_CRITICAL,
    ASSERT_ID_RIGHT_ENGINE_CRITICAL,
    ASSERT_ID_ENGINE_CRITICAL,
    ASSERT_ID_MACH_ONE,
    ASSERT_ID_MACH_TWO,
    ASSERT_ID_PARKING_BRAKE,

    MAX_NOF_ASSERTIONS,
  };

  typedef struct
  {
          BOOL   bFailed;
          BOOL   bMasterWarningSet;
          DOUBLE dFirstFailedDetectionTime;
          DOUBLE dDurationOfFailedAssertion;
          DOUBLE dLastNotificationTime;
    const DOUBLE dNotificationDelayTime;
    const DOUBLE dNotificationIntervalTime;
    const BOOL   bMasterWarningRequired;
    const DWORD  dwSoundID;
    const DWORD  dwSoundFlag;
          CHAR   acItemName[32];
    const BOOL   bBlinkRequired;
  } ASSERTION_STATE;


               VWS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual     ~VWS();
          BOOL IsAssertionFailed(DWORD dwAssertionIndex) const 
          { 
            if(dwAssertionIndex < sizeof(g_akCheckListAssertionTable)/sizeof(ASSERTION_STATE)) 
              return g_akCheckListAssertionTable[dwAssertionIndex].bFailed; 
            else 
              return FALSE; 
          }

  static  VWS::ASSERTION_STATE g_akCheckListAssertionTable[VWS::MAX_NOF_ASSERTIONS];

protected:
  virtual VOID  Start();
  virtual VOID  Process();
          VOID  EvaluateAssertionState(bool bExpression, DOUBLE fAbsoluteTime, SS *pkSS, DWORD dwAssertID);
          VOID  ProcessMasterWarningState(SS *pkSS);

private:
          INT   m_iMasterWarningCount;

};


#endif	/* __VWS_MODULE_H__ */
