#include "AGM.h"

const FLOAT               AGM::MASS                                   = 790*0.45359237f;
const FLOAT               AGM::FUSE_DETONATION_DISTANCE               = 30;
const FLOAT               AGM::MIN_AIRSPEED                           = 740;
const FLOAT               AGM::MAX_AIRSPEED                           = 2200;
const FLOAT               AGM::MAX_G                                  = 22;
const FLOAT               AGM::MAX_THRUST                             = 12200;
const FLOAT               AGM::TIME_TO_SELFDISTRUCT                   = 25;
const FLOAT               AGM::MIN_LOCK_DISTANCE                      = 900;
const FLOAT               AGM::MAX_LOCK_DISTANCE                      = 28000;
const FLOAT               AGM::PROPELANT_BURNOUT_TIME                 = 45;
const FLOAT               AGM::MIN_COS_OF_SEEKER_ASPECT_ANGLE         = cos((45.0f/180.0f)*D3DX_PI);  //45 Degrees.
const FLOAT               AGM::MIN_COS_OF_LOCKED_TARGET_ASPECT_ANGLE  = cos(( 1.5f/180.0f)*D3DX_PI);  //03 Degrees.
const AAVVector3<FLOAT>  AGM::AIR_RESISTANCE_FACTOR                  = AAVVector3<FLOAT>(0.15f, 0.15f, AGM::MAX_THRUST/4840000);


AGM::AGM(CHAR *pcName, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude) :
  MISSILE(pcName, "AGM", uiStationID, uiUnitID, uiWeight, pkAvionics, rkPilonVector, rkPilonAttitude)
{
  c_fMass                                                   = AGM::MASS;
  c_fFuseDetonationDistance                                 = AGM::FUSE_DETONATION_DISTANCE;
  c_fMinAirspeed                                            = AGM::MIN_AIRSPEED;
  c_fMaxAirspeed                                            = AGM::MAX_AIRSPEED;
  c_fMaxG                                                   = AGM::MAX_G;
  c_fMaxTrust                                               = AGM::MAX_THRUST;
  c_fTimeToSelfDestruct                                     = AGM::TIME_TO_SELFDISTRUCT;
  c_fMinLockDistance                                        = AGM::MIN_LOCK_DISTANCE;
  c_fMaxLockDistance                                        = AGM::MAX_LOCK_DISTANCE;
  c_fPropelantBurnoutTime                                   = AGM::PROPELANT_BURNOUT_TIME;
  c_kAirResistanceFactor                                    = AGM::AIR_RESISTANCE_FACTOR;
}

AGM::~AGM()
{
  MISSILE::Stop();
}

VOID AGM::Render(D3DXMATRIX &rkOPMatrix)
{
}

