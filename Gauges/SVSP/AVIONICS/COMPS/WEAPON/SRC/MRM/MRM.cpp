#include "MRM.h"

const FLOAT               MRM::MASS                                   = 335*0.45359237f;
const FLOAT               MRM::FUSE_DETONATION_DISTANCE               = 30;
const FLOAT               MRM::MIN_AIRSPEED                           = 740;
const FLOAT               MRM::MAX_AIRSPEED                           = 2200;
const FLOAT               MRM::MAX_G                                  = 22;
const FLOAT               MRM::MAX_THRUST                             = 0;
const FLOAT               MRM::TIME_TO_SELFDISTRUCT                   = 25;
const FLOAT               MRM::MIN_LOCK_DISTANCE                      = 1500;
const FLOAT               MRM::MAX_LOCK_DISTANCE                      = 70000;
const FLOAT               MRM::PROPELANT_BURNOUT_TIME                 = 45;
const FLOAT               MRM::MIN_COS_OF_SEEKER_ASPECT_ANGLE         = cos((45.0f/180.0f)*D3DX_PI);  //45 Degrees.
const FLOAT               MRM::MIN_COS_OF_LOCKED_TARGET_ASPECT_ANGLE  = cos(( 1.5f/180.0f)*D3DX_PI);  //03 Degrees.
const AAVVector3<FLOAT>  MRM::AIR_RESISTANCE_FACTOR                  = AAVVector3<FLOAT>(0.15f, 0.15f, MRM::MAX_THRUST/4840000);

MRM::MRM(CHAR *pcName, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude) :
  MISSILE(pcName, "MRM", uiStationID, uiUnitID, uiWeight, pkAvionics, rkPilonVector, rkPilonAttitude)
{
  c_fMass                                                   = MRM::MASS;
  c_fFuseDetonationDistance                                 = MRM::FUSE_DETONATION_DISTANCE;
  c_fMinAirspeed                                            = MRM::MIN_AIRSPEED;
  c_fMaxAirspeed                                            = MRM::MAX_AIRSPEED;
  c_fMaxG                                                   = MRM::MAX_G;
  c_fMaxTrust                                               = MRM::MAX_THRUST;
  c_fTimeToSelfDestruct                                     = MRM::TIME_TO_SELFDISTRUCT;
  c_fMinLockDistance                                        = MRM::MIN_LOCK_DISTANCE;
  c_fMaxLockDistance                                        = MRM::MAX_LOCK_DISTANCE;
  c_fPropelantBurnoutTime                                   = MRM::PROPELANT_BURNOUT_TIME;
  c_kAirResistanceFactor                                    = MRM::AIR_RESISTANCE_FACTOR;
}

MRM::~MRM()
{
  MISSILE::Stop();
}

VOID MRM::Render(D3DXMATRIX &rkOPMatrix)
{
}

