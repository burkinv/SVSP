#include "HARM.h"

const FLOAT              HARM::MASS                         = 780*0.45359237f;
const FLOAT              HARM::FUSE_DETONATION_DISTANCE     = 30;
const FLOAT              HARM::MIN_AIRSPEED                 = 740;
const FLOAT              HARM::MAX_AIRSPEED                 = 2200;
const FLOAT              HARM::MAX_G                        = 22;
const FLOAT              HARM::MAX_THRUST                   = 2200;
const FLOAT              HARM::TIME_TO_SELFDISTRUCT         = 25;
const FLOAT              HARM::MIN_LOCK_DISTANCE            = 1500;
const FLOAT              HARM::MAX_LOCK_DISTANCE            = 106000;
const FLOAT              HARM::PROPELANT_BURNOUT_TIME       = 45;
const AAVVector3<FLOAT>  HARM::AIR_RESISTANCE_FACTOR        = AAVVector3<FLOAT>(0.15f, 0.15f, HARM::MAX_THRUST/4840000);

HARM::HARM(CHAR *pcName, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude) :
  MISSILE(pcName, "HARM", uiStationID, uiUnitID, uiWeight, pkAvionics, rkPilonVector, rkPilonAttitude)
{
  c_fMass                                                   = HARM::MASS;
  c_fFuseDetonationDistance                                 = HARM::FUSE_DETONATION_DISTANCE;
  c_fMinAirspeed                                            = HARM::MIN_AIRSPEED;
  c_fMaxAirspeed                                            = HARM::MAX_AIRSPEED;
  c_fMaxG                                                   = HARM::MAX_G;
  c_fMaxTrust                                               = HARM::MAX_THRUST;
  c_fTimeToSelfDestruct                                     = HARM::TIME_TO_SELFDISTRUCT;
  c_fMinLockDistance                                        = HARM::MIN_LOCK_DISTANCE;
  c_fMaxLockDistance                                        = HARM::MAX_LOCK_DISTANCE;
  c_fPropelantBurnoutTime                                   = HARM::PROPELANT_BURNOUT_TIME;
  c_kAirResistanceFactor                                    = HARM::AIR_RESISTANCE_FACTOR;
}

HARM::~HARM()
{
  MISSILE::Stop();
}

VOID HARM::Render(D3DXMATRIX &rkOPMatrix)
{
}

