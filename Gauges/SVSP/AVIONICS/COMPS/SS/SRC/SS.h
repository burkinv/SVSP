#ifndef __SS_MODULE_H__
#define __SS_MODULE_H__

#include "__SVSPModule.h"
#include "__SVSPAvionics.h"
#include <dsound.h>

class SS : public __SVSPModule
{
public:
  enum 
  {
    MESSAGE_PLAY_SOUND_HIGH_PRIORITY   = (WM_USER + 1204),
    MESSAGE_PLAY_SOUND_NORM_PRIORITY,
    MESSAGE_LOOP_SOUND,
    MESSAGE_STOP_SOUND,
  };

  enum
  {
    SOUND_STATUS_PLAYING        = 0x00000001,
    SOUND_STATUS_BUFFERLOST     = 0x00000002,
    SOUND_STATUS_LOOPING        = 0x00000004,
    SOUND_STATUS_LOCHARDWARE    = 0x00000008,
    SOUND_STATUS_LOCSOFTWARE    = 0x00000010,
    SOUND_STATUS_TERMINATED     = 0x00000020,
    SOUND_STATUS_QUEUED_PLAY    = 0x00000080,
    SOUND_STATUS_QUEUED_STOP    = 0x00000100
  };

  enum
  {
    SOUND_ID_NONE                            = 0xFFFFFFFF,
    SOUND_ID_MINIMUM_SPEED                   = 0,
    SOUND_ID_MAXIMUM_SPEED                   = 1,
    SOUND_ID_BINGO_ALTITUDE                  = 2,
    SOUND_ID_MAXIMUM_G                       = 3,
    SOUND_ID_MAXIMUM_ANGLE_OF_ATTACK         = 4,
    SOUND_ID_ANGLE_OF_ATTACK_OVER_LIMIT      = 5,
    SOUND_ID_WARNING_GEAR_UP                 = 6,
    SOUND_ID_WARNING_GEAR_DOWN               = 7,
    SOUND_ID_FUEL_1500                       = 8,
    SOUND_ID_FUEL_800                        = 9,
    SOUND_ID_FUEL_500                        = 10,
    SOUND_ID_BINGO_FUEL                      = 11,
    SOUND_ID_LEFT_ENGINE_CRITICAL            = 12,
    SOUND_ID_RIGHT_ENGINE_CRITICAL           = 13,
    SOUND_ID_THROTTLE_BACK_LEFT_ENGINE       = 14,
    SOUND_ID_THROTTLE_BACK_RIGHT_ENGINE      = 15,
    SOUND_ID_THROTTLE_BACK_ENGINE            = 16,
    SOUND_ID_MACH_ONE                        = 17,
    SOUND_ID_MACH_TWO                        = 18,
    SOUND_ID_MASTER_WARNING                  = 19,
    SOUND_ID_SONIC_BOOM                      = 20,
    SOUND_ID_CAT_LAUNCH                      = 21,
    SOUND_ID_CABLE_CATCH                     = 22,
    SOUND_ID_MISSILE_AQUISITION              = 23,
    SOUND_ID_MISSILE_LOCKED                  = 24,
    SOUND_ID_MISSILE_LAUNCH                  = 25,
    SOUND_ID_EXPLODE_AIR                     = 26,
    SOUND_ID_BINGO_SINKRATE                  = 27,
    SOUND_ID_STALL_WARNING                   = 28,

    MAX_NOF_SOUNDS
  };

  typedef struct
  {
    UINT                  uiStreamId;
    DWORD                 dwStatus;
    LPDIRECTSOUNDBUFFER   pkSoundBuffer;
    CHAR                 *pcLocale;     // CONST
    CHAR                 *pcFileName;   // CONST
  } SOUND_SLOT;

                    SS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual          ~SS();
               VOID PlaySound(UINT uiSoundId, UINT uiMessagePriority = SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY);
               VOID StopSound(UINT uiSoundId);
               VOID StopAllSounds();
              DWORD GetSoundState(UINT uiSoundId);


private:
              VOID  InitSoundSlotTable();
  virtual     VOID  Start();
  virtual     VOID  Pause();
  virtual     VOID  Process();

  UINT              m_uiCurrentSoundSlotIndex;
};


#endif  /* __SS_MODULE_H__ */
