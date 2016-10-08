#include "SS.h"
#include "AC.h"
#include <__SVSPD3D9.h>

#define SS_LIB_PATH "Gauges/SVSP/AVIONICS/COMPS/SS/RES/SOUNDS/"
#define LOCALE      "US/"

static SS::SOUND_SLOT g_akSoundSlots[SS::MAX_NOF_SOUNDS] =
{//uiStreamId,  dwStatus,    pkSoundBuffer,  pcLocale,  pcFileName, 
  {0,           0,           NULL,           LOCALE,    "MinimumSpeed.wav"                    }, //SOUND_ID_MINIMUM_SPEED              
  {0,           0,           NULL,           LOCALE,    "MaximumSpeed.wav"                    }, //SOUND_ID_MAXIMUM_SPEED              
  {0,           0,           NULL,           LOCALE,    "Altitude.wav"                        }, //SOUND_ID_BINGO_ALTITUDE             
  {0,           0,           NULL,           LOCALE,    "MaximumG.wav"                        }, //SOUND_ID_MAXIMUM_G                  
  {0,           0,           NULL,           LOCALE,    "MaximumAngleOfAttack.wav"            }, //SOUND_ID_MAXIMUM_ANGLE_OF_ATTACK    
  {0,           0,           NULL,           LOCALE,    "AngleOfAttackOverLimit.wav"          }, //SOUND_ID_ANGLE_OF_ATTACK_OVER_LIMIT 
  {0,           0,           NULL,           LOCALE,    "WarningGearUp.wav"                   }, //SOUND_ID_WARNING_GEAR_UP            
  {0,           0,           NULL,           LOCALE,    "WarningGearDown.wav"                 }, //SOUND_ID_WARNING_GEAR_DOWN          
  {0,           0,           NULL,           LOCALE,    "Fuel1500.wav"                        }, //SOUND_ID_FUEL_1500                  
  {0,           0,           NULL,           LOCALE,    "Fuel800.wav"                         }, //SOUND_ID_FUEL_800                   
  {0,           0,           NULL,           LOCALE,    "Fuel500.wav"                         }, //SOUND_ID_FUEL_500                   
  {0,           0,           NULL,           LOCALE,    "BingoFuel.wav"                       }, //SOUND_ID_BINGO_FUEL                 
  {0,           0,           NULL,           LOCALE,    "ThrottleBackLeftEngine.wav"          }, //SOUND_ID_LEFT_ENGINE_CRITICAL       
  {0,           0,           NULL,           LOCALE,    "ThrottleBackRightEngine.wav"         }, //SOUND_ID_RIGHT_ENGINE_CRITICAL      
  {0,           0,           NULL,           LOCALE,    "ThrottleBackLeftEngine.wav"          }, //SOUND_ID_THROTTLE_BACK_LEFT_ENGINE  
  {0,           0,           NULL,           LOCALE,    "ThrottleBackRightEnginewav"          }, //SOUND_ID_THROTTLE_BACK_RIGHT_ENGINE 
  {0,           0,           NULL,           LOCALE,    NULL                                  }, //SOUND_ID_THROTTLE_ENGINE            
  {0,           0,           NULL,           LOCALE,    NULL                                  }, //SOUND_ID_MACH_ONE                   
  {0,           0,           NULL,           LOCALE,    NULL                                  }, //SOUND_ID_MACH_TWO                   
  {1,           0,           NULL,           NULL,      "MasterWarning.wav"                   }, //SOUND_ID_MASTER_WARNING             
  {1,           0,           NULL,           NULL,      NULL                                  }, //SOUND_ID_SONIC_BOOM                 
  {1,           0,           NULL,           NULL,      NULL                                  }, //SOUND_ID_CAT_LAUNCH                 
  {1,           0,           NULL,           NULL,      NULL                                  }, //SOUND_ID_CABLE_CATCH                
  {1,           0,           NULL,           NULL,      "MissileAquisition.wav"               }, //SOUND_ID_MISSILE_AQUISITION         
  {1,           0,           NULL,           NULL,      "MissileLocked.wav"                   }, //SOUND_ID_MISSILE_LOCKED             
  {1,           0,           NULL,           NULL,      NULL                                  }, //SOUND_ID_MISSILE_LAUNCH             
  {1,           0,           NULL,           NULL,      NULL                                  }, //SOUND_ID_EXPLODE_AIR                
  {0,           0,           NULL,           LOCALE,    "PullUp.wav"                          }, //SOUND_ID_BINGO_SINKRATE             
  {2,           0,           NULL,           NULL,      "StallWarning.wav"                    }, //SOUND_ID_STALL_WARNING              
};

SS::SS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("SS", pkAvionics, dwThreadSleepTime, false, NULL, NULL, NULL)
{
  for(UINT i = 0; i < SS::MAX_NOF_SOUNDS; ++i)
  {
    SS::SOUND_SLOT *pkSoundSlot = &g_akSoundSlots[i];
    pkSoundSlot->dwStatus      = 0x0;
    pkSoundSlot->pkSoundBuffer = NULL;
  }

  m_uiCurrentSoundSlotIndex = 0;
}

SS::~SS()
{
  StopAllSounds();

  __SVSPModule::Stop();

  for(UINT i = 0; i < SS::MAX_NOF_SOUNDS; ++i)
  {
    SS::SOUND_SLOT *pkSoundSlot = &g_akSoundSlots[i];
    if(pkSoundSlot->pkSoundBuffer != NULL)
    {
      pkSoundSlot->pkSoundBuffer->Release();
      pkSoundSlot->dwStatus      = 0x0;
      pkSoundSlot->pkSoundBuffer = NULL;
    }
  }
}

VOID SS::InitSoundSlotTable()
{
  LPDIRECTSOUND8       m_pDS8                = NULL;
  LPDIRECTSOUNDBUFFER  m_pDSBufferPrimary    = NULL;
//static LPDIRECTSOUNDBUFFER  m_pDSBufferSecondary  = NULL;

  //DS8
  DirectSoundCreate8(NULL, &m_pDS8, NULL);
  m_hResult = m_pDS8->SetCooperativeLevel(__SVSPD3D9_GetFocusWindow(), DSSCL_PRIORITY);
  AAV_ASSERT(m_hResult == S_OK);

  //PRIMARY
  DSBUFFERDESC dsbd;
  ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
  dsbd.dwSize        = sizeof(DSBUFFERDESC);
  dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
  dsbd.dwBufferBytes = 0;
  dsbd.lpwfxFormat   = NULL;

  m_hResult = m_pDS8->CreateSoundBuffer(&dsbd, &m_pDSBufferPrimary, NULL);
  AAV_ASSERT(m_hResult == S_OK);

  WAVEFORMATEX wfx;
  ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
  wfx.wFormatTag      = (WORD) WAVE_FORMAT_PCM;
  wfx.nChannels       = (WORD) 2;
  wfx.nSamplesPerSec  = (DWORD)22050;
  wfx.wBitsPerSample  = (WORD) 16;
  wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
  wfx.nAvgBytesPerSec = (DWORD)(wfx.nSamplesPerSec * wfx.nBlockAlign);

  m_pDSBufferPrimary->SetFormat(&wfx);
  m_pDSBufferPrimary->Release();

  CHAR acAbsoluteFileName[1024];

  for(int i = 0; i < MAX_NOF_SOUNDS; ++i)
  {
    SOUND_SLOT *pkSoundSlot = &g_akSoundSlots[i];
    if(pkSoundSlot->pcFileName != NULL)
    {
      sprintf_s(&acAbsoluteFileName[0], sizeof(acAbsoluteFileName), "%s%s%s", SS_LIB_PATH, (pkSoundSlot->pcLocale != NULL) ? pkSoundSlot->pcLocale : "", pkSoundSlot->pcFileName);
      HMMIO hmmio = mmioOpen(&acAbsoluteFileName[0], 0, MMIO_READ|MMIO_ALLOCBUF);

      if(hmmio)
      {
        MMCKINFO mmioWaveHeaderChunk;
        mmioWaveHeaderChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E'); 
        if(mmioDescend(hmmio, (LPMMCKINFO)&mmioWaveHeaderChunk, NULL, MMIO_FINDRIFF) == MMIOERR_CHUNKNOTFOUND)
        {
          mmioClose(hmmio, MMIO_FHOPEN);
          continue;
        }

        MMCKINFO mmioWaveDataChunk;
        mmioWaveDataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
        if(mmioDescend(hmmio, (LPMMCKINFO)&mmioWaveDataChunk, (LPMMCKINFO)&mmioWaveHeaderChunk, MMIO_FINDCHUNK) == MMIOERR_CHUNKNOTFOUND)
        {
          mmioClose(hmmio, MMIO_FHOPEN);
          continue;
        }

        ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
        dsbd.dwSize          = sizeof(DSBUFFERDESC);
        dsbd.dwFlags         = DSBCAPS_CTRLVOLUME ;
        dsbd.dwBufferBytes   = mmioWaveDataChunk.cksize;
        dsbd.guid3DAlgorithm = GUID_NULL;
        dsbd.lpwfxFormat     = &wfx;

        m_pDS8->CreateSoundBuffer(&dsbd, &pkSoundSlot->pkSoundBuffer, NULL );

        LPVOID pBuffer = NULL;
        DWORD  dwBufferSize;
        if(FAILED(pkSoundSlot->pkSoundBuffer->Lock(0, mmioWaveDataChunk.cksize, &pBuffer, &dwBufferSize, NULL, NULL, 0L)))
        {
          mmioClose(hmmio, MMIO_FHOPEN);
          continue;
        }

        DWORD  dwBytesRead;
        dwBytesRead = mmioRead(hmmio, (HPSTR)pBuffer, dwBufferSize);

        if(FAILED(pkSoundSlot->pkSoundBuffer->Unlock(pBuffer, dwBufferSize, NULL, 0L)))
        {
          mmioClose(hmmio, MMIO_FHOPEN);
          continue;
        }

        mmioClose(hmmio, MMIO_FHOPEN);
      }
    }
  }
}

VOID SS::PlaySound(UINT uiSoundId, UINT uiMessage)
{
  LONG  lVolume         = -1500L;
  AAV_ASSERT_RETURN_VOID(uiSoundId < SS::MAX_NOF_SOUNDS);

  SOUND_SLOT *pkSoundSlot = &g_akSoundSlots[uiSoundId];
  AAV_ASSERT_RETURN_VOID(pkSoundSlot->pkSoundBuffer != NULL);

  switch(uiMessage)
  {
  case SS::MESSAGE_PLAY_SOUND_HIGH_PRIORITY:
    pkSoundSlot->pkSoundBuffer->SetVolume(0);
    pkSoundSlot->pkSoundBuffer->SetCurrentPosition(0);
    pkSoundSlot->pkSoundBuffer->Play(0, 0, 0L);
    pkSoundSlot->dwStatus = SS::SOUND_STATUS_PLAYING;
    break;

  case SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY:
    if((pkSoundSlot->dwStatus & SS::SOUND_STATUS_QUEUED_PLAY) != SS::SOUND_STATUS_QUEUED_PLAY)
    {
      if(PostThreadMessage(GetThreadId(), uiMessage, uiSoundId, 0))
      {
        pkSoundSlot->dwStatus |= SS::SOUND_STATUS_QUEUED_PLAY;
      }
    }
    break;

  case SS::MESSAGE_LOOP_SOUND:
    pkSoundSlot->pkSoundBuffer->SetVolume(lVolume);
    pkSoundSlot->pkSoundBuffer->SetCurrentPosition(0);
    pkSoundSlot->pkSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
    pkSoundSlot->dwStatus = SS::SOUND_STATUS_LOOPING;
    break;

  default:
    break;
  }

}

VOID SS::StopSound(UINT uiSoundId)
{
  AAV_ASSERT_RETURN_VOID(uiSoundId < SS::MAX_NOF_SOUNDS);

  SOUND_SLOT *pkSoundSlot = &g_akSoundSlots[uiSoundId];
  AAV_ASSERT_RETURN_VOID(pkSoundSlot->pkSoundBuffer != NULL);

  pkSoundSlot->pkSoundBuffer->SetVolume(0);
  pkSoundSlot->pkSoundBuffer->SetCurrentPosition(0);
  pkSoundSlot->pkSoundBuffer->Stop();
  pkSoundSlot->dwStatus = 0;
}

DWORD SS::GetSoundState(UINT uiSoundId)
{
  AAV_ASSERT_RETURN(uiSoundId < SS::MAX_NOF_SOUNDS, 0);
  SOUND_SLOT *pkSoundSlot = &g_akSoundSlots[uiSoundId];
  AAV_ASSERT_RETURN(pkSoundSlot->pkSoundBuffer != NULL, 0);

  return pkSoundSlot->dwStatus;
}

VOID SS::StopAllSounds()
{
  DWORD dwStatus = 0;
  for(UINT i = 0; i < MAX_NOF_SOUNDS; ++i)
  {
    SOUND_SLOT *pkSoundSlot = &g_akSoundSlots[i];
    if(pkSoundSlot->pkSoundBuffer != NULL)
    {
      pkSoundSlot->pkSoundBuffer->GetStatus(&dwStatus);
      if((dwStatus & DSBSTATUS_PLAYING) || (dwStatus & DSBSTATUS_LOOPING ))
      {
        pkSoundSlot->pkSoundBuffer->SetVolume(0);
        pkSoundSlot->pkSoundBuffer->Stop();
        pkSoundSlot->pkSoundBuffer->SetCurrentPosition(0);
        pkSoundSlot->dwStatus = 0x0;
      }
    }
  }
}

VOID SS::Start()
{
  InitSoundSlotTable();

  m_uiCurrentSoundSlotIndex = 0;

  __SVSPModule::Start();
}

VOID SS::Pause()
{
  StopAllSounds();
}


VOID SS::Process()
{
  MSG   msg;
  LONG  lVolume         = -1500L;
  DWORD dwStatus        = 0;
  BOOL  bPlayAuthorized = false;

  if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
  {
    UINT uiSoundId = (UINT)msg.wParam;
    AAV_ASSERT_RETURN_VOID(uiSoundId < SS::MAX_NOF_SOUNDS);

    SOUND_SLOT *pkSoundSlot = &g_akSoundSlots[uiSoundId];
    AAV_ASSERT_RETURN_VOID(pkSoundSlot != NULL);

    switch(msg.message)
    {
    case SS::MESSAGE_PLAY_SOUND_NORM_PRIORITY:
      bPlayAuthorized = true;
      for(UINT k = 0; k < MAX_NOF_SOUNDS; ++k)
      {
        if((pkSoundSlot->uiStreamId == g_akSoundSlots[k].uiStreamId) && (g_akSoundSlots[k].pkSoundBuffer != NULL))
        {
          g_akSoundSlots[k].pkSoundBuffer->GetStatus(&dwStatus);
          if((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
          {
            bPlayAuthorized = false;
          }
        }
      }
      if(bPlayAuthorized)
      {
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        pkSoundSlot->dwStatus &= ~ SS::SOUND_STATUS_QUEUED_PLAY;
        pkSoundSlot->pkSoundBuffer->SetVolume(lVolume);
        pkSoundSlot->pkSoundBuffer->SetCurrentPosition(0);
        pkSoundSlot->pkSoundBuffer->Play(0, 0, 0L);
      }
      break;

    default:
      break;
    }
  }

  //Update sound slot status.
  AAV_ASSERT_RETURN_VOID(m_uiCurrentSoundSlotIndex < SS::MAX_NOF_SOUNDS);
  SOUND_SLOT *pkSoundSlot = &g_akSoundSlots[m_uiCurrentSoundSlotIndex];
  if(pkSoundSlot->pkSoundBuffer != NULL)
  {
    if((pkSoundSlot->dwStatus & SS::SOUND_STATUS_QUEUED_PLAY) != SS::SOUND_STATUS_QUEUED_PLAY)
    {
      pkSoundSlot->pkSoundBuffer->GetStatus(&(pkSoundSlot->dwStatus));
    }
  }
  m_uiCurrentSoundSlotIndex = (m_uiCurrentSoundSlotIndex < (SS::MAX_NOF_SOUNDS - 1)) ? m_uiCurrentSoundSlotIndex + 1 : 0;
}
