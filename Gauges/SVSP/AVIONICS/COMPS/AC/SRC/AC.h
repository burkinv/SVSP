#ifndef __AC_MODULE_H__
#define __AC_MODULE_H__

#include "__SVSPModule.h"
#include "__SVSPVector3.h"
#include "__SVSPGeoCoordinates3.h"

/**
  Flight Sim Communication Module
  - General FS Data Communication Interface.
  - Frame Critical Airborne Data Handling.
  - Preparation of Global-To-Local and Local-To-Body convertion matricies.
  - Preparation of auxiliary reference data.
*/
class AC : public __SVSPModule
{
public:
  enum 
  {
    PROCESS_UPDATE_AIRCRAFT_LOCATION_DATA  = 0x1,
    PROCESS_UPDATE_AIRCRAFT_STATE_DATA     = 0x2,
    PROCESS_UPDATE_AIRCRAFT_NAV_RADIO_DATA = 0x4,
    PROCESS_UPDATE_AIRCRAFT_COM_RADIO_DATA = 0x8,
  };

  enum
  {
    REQUEST_ID_READ_LATLONALT_DATA,
    REQUEST_ID_READ_OBJECT_CRITICAL_DATA,
    REQUEST_ID_READ_OBJECT_STATE_DATA,
    REQUEST_ID_READ_OBJECT_NAV_RADIO_DATA,
    REQUEST_ID_READ_OBJECT_COM_RADIO_DATA,
  } REQUEST_ID;

  enum  
  {
    DATA_ID_READ_LATLONALT_DATA,
    DATA_ID_READ_OBJECT_CRITICAL_DATA,
    DATA_ID_READ_OBJECT_STATE_DATA,
    DATA_ID_READ_OBJECT_NAV_RADIO_DATA,
    DATA_ID_READ_OBJECT_COM_RADIO_DATA,
  } DATA_ID;

  struct AICRAFT_CRITICAL_DATA
  {
    AAVVector3<FLOAT>       m_kBodyAttitude;
    AAVVector3<FLOAT>       m_kBodyVelocity;
    FLOAT                    m_dRadarAltitude;
  };

  struct AIRCRAFT_STATE_DATA
  {
    BOOL                     m_bOnGround;
    FLOAT                    m_dTotalWeight;
    FLOAT                    m_dIncidenceAlpha;
    FLOAT                    m_dIndicatedAoA;
    FLOAT                    m_dIndicatedAltitude;
    FLOAT                    m_dIndicatedAirspeed;
    FLOAT                    m_dGroundTrackSpeed;
    FLOAT                    m_dIndicatedVerticalSpeed;
    FLOAT                    m_dIndicatedMagneticHeading;
    FLOAT                    m_dMach;
    FLOAT                    m_dG;
    FLOAT                    m_dMaxG;
    FLOAT                    m_dGearPosition;
    AAVVector3<FLOAT>       m_kBodyAcceleration;
    AAVVector3<FLOAT>       m_kBodyAngularVelocity;
    FLOAT                    m_dFlapsHandlePosition;
    AAVVector3<FLOAT>       m_kControlInput;
    FLOAT                    m_dElevatorTrimRadians;
    FLOAT                    m_dFuelTotalQuantityWeightPounds;
    FLOAT                    m_dSpoilerExtentionPercent;
    FLOAT                    m_dLocalTimeSeconds;
    FLOAT                    m_dBrakePosition;
    FLOAT                    m_dBrakeParking;
  } m_kData;
  
  struct AIRCRAFT_NAV_RADIO_DATA
  {
    FLOAT                    m_dNavFrequency;
    BOOL                     m_bNavHasLocalizer;
    BOOL                     m_bNavHasGlideslope;
    BOOL                     m_bNavHasDME;
    D3DXVECTOR2              m_kILSErrorVector;
    FLOAT                    m_dNavDMEDistance;
  } m_kNavRadioData;

  struct AIRCRAFT_COM_RADIO_DATA
  {
    DWORD                    m_dComActiveFrequency[2];
    DWORD                    m_dComStandbyFrequency[2];
  } m_kComRadioData;

  struct AIRCRAFT_STATE_DATA_ENUMS
  {
    AAVVector3<ENUM>        m_kBodyAttitudeEnum;
    AAVVector3<ENUM>        m_kBodyAngularVelocityEnum;
    ENUM                     m_bOnGroundEnum;
    ENUM                     m_dTotalWeightEnum;
    ENUM                     m_dIndicatedAoAEnum;
    ENUM                     m_dIndicatedAltitudeEnum;
    ENUM                     m_dRadarAltitudeEnum;
    ENUM                     m_dIndicatedAirspeedEnum;
    ENUM                     m_dIndicatedVerticalSpeedEnum;
    ENUM                     m_dIndicatedMagneticHeadingEnum;
    ENUM                     m_dMachEnum;
    ENUM                     m_dGEnum;
    ENUM                     m_dMaxGEnum;
    ENUM                     m_dGearPositionEnum;
    ENUM                     m_dNavFrequencyEnum;
    ENUM                     m_bNavHasLocalizerEnum;
    ENUM                     m_bNavHasGlideslopeEnum;
    ENUM                     m_dNavCDIDeflectionEnum;
    ENUM                     m_dNavGSIDeflectionEnum;
    AAVVector3<ENUM>        m_kBodyAccelerationEnum;
    AAVVector3<ENUM>        m_kBodyVelocityEnum;
    __SVSPGeoCoordinates3<ENUM>     m_kLocationEnum;
    ENUM                     m_dFlapsHandlePositionEnum;
    AAVVector3<ENUM>        m_kControlInputEnum;
    DWORD                    m_dComActiveFrequencyEnum[2];
    DWORD                    m_dComStandbyFrequencyEnum[2];
  } m_kDataEnums;

  __SVSPGeoCoordinates3<FLOAT>     m_kLocation;
  AAVVector3<FLOAT>        m_kBodyAttitude;
  AAVVector3<FLOAT>        m_kBodyVelocity;
  FLOAT                     m_dRadarAltitude;
  FLOAT                     m_dRadarVerticalVelocity;
  AAVVector3<FLOAT>        m_kLocalVelocity;

  D3DXVECTOR3               m_kGlobalVector;
  D3DXMATRIXA16             m_kGlobalToLocalTransformMatrix;
  D3DXMATRIXA16             m_kLocalToGlobalTransformMatrix;
  D3DXMATRIXA16             m_kLocalToBodyTransformMatrix;
  D3DXMATRIXA16             m_kBodyToLocalTransformMatrix;
  D3DXMATRIXA16             m_kBodyToTangentialTransformMatrix;
  D3DXMATRIXA16             m_kViewMatrix;
  D3DXMATRIXA16             m_kBankRotationMatrix;
  D3DXMATRIXA16             m_kYawRotationMatrix;

  D3DXMATRIXA16             m_kLocalToBodyXMatrix;
  D3DXMATRIXA16             m_kLocalToBodyYMatrix;
  D3DXMATRIXA16             m_kLocalToBodyZMatrix;


  AAVVector3<FLOAT>        m_kBodyVelocityNormalized;
  AAVVector3<FLOAT>        m_kLocalVelocityNormalized;
  AAVVector3<FLOAT>        m_kPreviousVector;
  AAVVector3<FLOAT>        m_kGlobalVelocityVectorNormalized;
  AAVVector3<FLOAT>        m_kAC_NoseDirectionVector;
  AAVVector3<FLOAT>        m_kVP_Vector;
  AAVVector3<FLOAT>        m_kTP_Vector;
  CHAR                      m_acACType[24];
  USHORT                    m_usPaused;
  USHORT                    m_usSlew;
  CHAR                      m_cDME[6];
  DWORD                     m_dwNAVGPS;
  FLOAT                     m_fIncidenceAlpha;
  FLOAT                     m_fAoA;
  FLOAT                     m_fTrueHeading;
  FLOAT                     m_fALPHA;
  FLOAT                     m_fBETA;
  FLOAT                     m_fInX;
  FLOAT                     m_fInY;
  UINT                      m_uiFlapsLeft;
  UINT                      m_uiFlapsRight;
  FLOAT                     m_fElevatorTrim;
  FLOAT                     m_dLat;
  FLOAT                     m_dLon;
  CHAR                      m_acComActiveFrequency[2][16];
  CHAR                      m_acComStandbyFrequency[2][16];
  USHORT                    m_usCOM1;
  USHORT                    m_usNAV1;
  USHORT                    m_usADF1[2];
  FLOAT                     m_fADF1Bearing;
  FLOAT                     m_fNAV1Bearing;
  FLOAT                     m_fNAV1OBS;
  USHORT                    m_usNAV1ID;
  BOOL                      m_bACLSOn;
  BOOL                      m_bNavHasLocalizer;
  BOOL                      m_bNavHasGlideslope;
  D3DXVECTOR2               m_kILSErrorVector;
  FLOAT                     m_fAirBrake;
  DWORD                     m_dwAP_Master;
  DWORD                     m_dwAP_ALTHold;
  DWORD                     m_dwAP_ALT;
  DWORD                     m_dwAP_ATTHold;
  DWORD                     m_dwAP_APRHold;
  DWORD                     m_dwAP_HDGHold;
  DWORD                     m_dwAP_BCHold;
  DWORD                     m_dwAP_NAVHold;
  FLOAT                     m_fAP_HDG;
  DWORD                     m_dwAP_KIASHold;
  USHORT                    m_usAP_KIAS;
  FLOAT                     m_fFuelLeft;
  FLOAT                     m_fFuelRight;
  USHORT                    m_usWindDirection;
  FLOAT                     m_dVelocity;
  FLOAT                     m_dFuelTotalQuantityWeightKg;
  FLOAT                     m_dMass;

  struct GPS
  {
    DWORD                   dwFlags;
    DWORD                   dwTotalNumWPTs;
    DWORD                   dwCurWPTIndex;
    FLOAT                   dDistToNextWPT;
    FLOAT                   dBearingToNextWPT;
    __SVSPGeoCoordinates3<FLOAT>   kCurWPTCoords;
  } m_kGPS;


                        AC(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual              ~AC();
         VOID           ReadCriticalData();

protected:
  virtual VOID          Start();
  virtual VOID          Process();
  virtual VOID          DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);
  friend VOID CALLBACK  AC__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);
          VOID          GetDataEnums();

private:
  INT                   m_idRadiansEnum;
  INT                   m_idFeetPerSecondEnum;
  D3DXMATRIXA16         m_matACRotX;
  D3DXMATRIXA16         m_matACRotY;
  D3DXMATRIXA16         m_matACRotZ;
  D3DXMATRIXA16         m_matACRot;
  //D3DXMATRIXA16       m_matVPRotX;
  //D3DXMATRIXA16       m_matVPRotY;
  //D3DXMATRIXA16       m_matVPRotZ;
  D3DXMATRIXA16         m_matVPRot;
};

#endif	/* __AC_MODULE_H__ */
