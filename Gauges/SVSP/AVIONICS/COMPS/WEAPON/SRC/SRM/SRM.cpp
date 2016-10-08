#include "SRM.h"
#include "SS.h"

const FLOAT               SRM::MASS                                   = 190*0.45359237f;
const FLOAT               SRM::FUSE_DETONATION_DISTANCE               = 30;
const FLOAT               SRM::MIN_AIRSPEED                           = 740;
const FLOAT               SRM::MAX_AIRSPEED                           = 2200;
const FLOAT               SRM::MAX_G                                  = 22;
const FLOAT               SRM::MAX_THRUST                             = 2200;
const FLOAT               SRM::TIME_TO_SELFDISTRUCT                   = 25;
const FLOAT               SRM::MIN_LOCK_DISTANCE                      = 1000;
const FLOAT               SRM::MAX_LOCK_DISTANCE                      = 18200;
const FLOAT               SRM::EFFECTIVE_LOCK_DISTANCE                = 3000;
const FLOAT               SRM::PROPELANT_BURNOUT_TIME                 = 45;
const FLOAT               SRM::MIN_COS_OF_SEEKER_ASPECT_ANGLE         = cos((45.0f/180.0f)*D3DX_PI);  //45 Degrees.
const FLOAT               SRM::MIN_COS_OF_LOCKED_TARGET_ASPECT_ANGLE  = cos(( 1.5f/180.0f)*D3DX_PI);  //03 Degrees.
const AAVVector3<FLOAT>  SRM::AIR_RESISTANCE_FACTOR                  = AAVVector3<FLOAT>(0.15f, 0.15f, SRM::MAX_THRUST/4840000);

SRM::SRM(CHAR *pcName, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude) :
  MISSILE(pcName, "SRM", uiStationID, uiUnitID, uiWeight, pkAvionics, rkPilonVector, rkPilonAttitude)
{
  c_fMass                                                   = SRM::MASS;
  c_fFuseDetonationDistance                                 = SRM::FUSE_DETONATION_DISTANCE;
  c_fMinAirspeed                                            = SRM::MIN_AIRSPEED;
  c_fMaxAirspeed                                            = SRM::MAX_AIRSPEED;
  c_fMaxG                                                   = SRM::MAX_G;
  c_fMaxTrust                                               = SRM::MAX_THRUST;
  c_fTimeToSelfDestruct                                     = SRM::TIME_TO_SELFDISTRUCT;
  c_fMinLockDistance                                        = SRM::MIN_LOCK_DISTANCE;
  c_fMaxLockDistance                                        = SRM::MAX_LOCK_DISTANCE;
  c_fEffectiveLockDistance                                  = SRM::EFFECTIVE_LOCK_DISTANCE;
  c_fPropelantBurnoutTime                                   = SRM::PROPELANT_BURNOUT_TIME;
  c_kAirResistanceFactor                                    = SRM::AIR_RESISTANCE_FACTOR;
}

SRM::~SRM()
{
  MISSILE::Stop();
}

HRESULT SRM::SetState(WEAPON::STATE eState)
{
  SS *pkSS = static_cast<SS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ISS));

  switch(eState)
  {
  case WEAPON::WEAPON_STATE_ARMED:
  case WEAPON::WEAPON_STATE_ACQUISITION:
    if(pkSS != NULL)
    {
      pkSS->StopSound(SS::SOUND_ID_MISSILE_LOCKED);
      pkSS->PlaySound(SS::SOUND_ID_MISSILE_AQUISITION, SS::MESSAGE_LOOP_SOUND);
    }
    break;

  case WEAPON::WEAPON_STATE_LOCKED:
    if(pkSS != NULL)
    {
      pkSS->StopSound(SS::SOUND_ID_MISSILE_AQUISITION);
      pkSS->PlaySound(SS::SOUND_ID_MISSILE_LOCKED, SS::MESSAGE_LOOP_SOUND);
    }
    break;

  default:
    if(pkSS != NULL)
    {
      pkSS->StopSound(SS::SOUND_ID_MISSILE_AQUISITION);
      pkSS->StopSound(SS::SOUND_ID_MISSILE_LOCKED);
    }
  }

  return MISSILE::SetState(eState);
}

VOID SRM::Render(D3DXMATRIX &rkOPMatrix)
{
}


