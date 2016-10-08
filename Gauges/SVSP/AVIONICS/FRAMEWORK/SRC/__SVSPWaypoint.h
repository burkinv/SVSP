#ifndef __SVSP_WAYPOINT_H__
#define __SVSP_WAYPOINT_H__

#include <windows.h>
#define _USE_MATH_DEFINES
#include <Math.h>
#include "__SVSPConfig.h"
#include "__SVSPDebug.h"
#include "__SVSPMath.h"
#include "__SVSPArea.h"
#include "__SVSPString.h"

template <typename T> struct AAVWAYPOINT
{
  friend class WPN;
  AAVWAYPOINT()
  {
  }

  AAVWAYPOINT(CHAR *pcName, DWORD dwRadioFrequency)
  {
    __SVSPModule::COPY_STRING(acName, pcName);
    this->dwRadioFrequency = dwRadioFrequency;
  }

  AAVWAYPOINT(CHAR *pcName, __SVSPGeoCoordinates3<T> kLocation, T fHeading)
  {
    this->acName                       = __SVSPString<CHAR>(pcName);
    this->dwRadioFrequency             = 0;
    this->kLocation                    = kLocation;
    this->fHeading                     = fHeading;
    this->kGlobalVector                = kLocation.GetVector3<T>();
    this->kGlobalOffsetVector          = AAVVector3<T>(1, 0, 0);
    this->kLocalOffsetVector           = AAVVector3<T>(1, 0, 0);
    this->kBodyOffsetVector            = AAVVector3<T>(1, 0, 0);
    this->kBodyOffsetVectorNormalized  = AAVVector3<T>(1, 0, 0);
    this->fClosingInVelocity           = 0;
    this->bHasRangeData                = false;
    this->fSlantRangeMiles             = 0;
    this->usTTIMinutes                 = 0;
    this->usTTISeconds                 = 0;
    this->usTTI                        = 0;
    this->uiDTI                        = 0;
    D3DXMatrixIdentity(&this->kSteercueTransformMatrix);
  }

  __SVSPString<CHAR>       acName;
  DWORD                   dwRadioFrequency;
  __SVSPGeoCoordinates3<T> kLocation;
  T                       fHeading;
  AAVVector3<T>           kGlobalVector;
  AAVVector3<T>           kGlobalOffsetVector;
  D3DXVECTOR3             kLocalOffsetVector;
  D3DXVECTOR3             kBodyOffsetVector;
  D3DXVECTOR3             kBodyOffsetVectorNormalized;
  FLOAT                   fClosingInVelocity;
  BOOL                    bHasRangeData;
  FLOAT                   fSlantRangeMiles;
  USHORT                  usTTIMinutes;
  USHORT                  usTTISeconds;
  USHORT                  usTTI;
  UINT                    uiDTI;
  D3DXMATRIXA16           kSteercueTransformMatrix;

private:
  FLOAT            dTimeStamp;
};



#endif	/* __SVSP_WAYPOINT_H__ */


