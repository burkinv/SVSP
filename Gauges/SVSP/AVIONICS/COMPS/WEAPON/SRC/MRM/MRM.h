#ifndef __MRM_MODULE_H__
#define __MRM_MODULE_H__

#include "MISSILE.h"

class MRM : public MISSILE
{
public:
  static const FLOAT               MASS;
  static const FLOAT               FUSE_DETONATION_DISTANCE;
  static const FLOAT               MIN_AIRSPEED;
  static const FLOAT               MAX_AIRSPEED;
  static const FLOAT               MAX_G;
  static const FLOAT               MAX_THRUST;
  static const FLOAT               TIME_TO_SELFDISTRUCT;
  static const FLOAT               MIN_LOCK_DISTANCE;
  static const FLOAT               MAX_LOCK_DISTANCE;
  static const FLOAT               PROPELANT_BURNOUT_TIME;
  static const FLOAT               MIN_COS_OF_SEEKER_ASPECT_ANGLE;
  static const FLOAT               MIN_COS_OF_LOCKED_TARGET_ASPECT_ANGLE;
  static const AAVVector3<FLOAT>  AIR_RESISTANCE_FACTOR;

                             MRM(CHAR *pcName, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude);
  virtual                   ~MRM();
  virtual VOID               Render(D3DXMATRIX &rkOPMatrix);

};

#endif	/* __MRM_MODULE_H__ */
