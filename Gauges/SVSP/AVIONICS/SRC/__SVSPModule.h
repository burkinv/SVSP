#ifndef __SVSP_MODULE_H__
#define __SVSP_MODULE_H__

#include <stdio.h>
#include <windows.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <Math.h>
#include <SimConnect.h>
#include "__SVSPConfig.h"
#include "__SVSPDebug.h"
#include "__SVSPMath.h"
#include "__SVSPArea.h"
#include "__SVSPWaypoint.h"
#include "__SVSPTouchRect.h"
#include <GAUGES.h>

typedef DWORD (__thiscall *LPPROCESS)(class __SVSPModule *);

class __SVSPModule
{
public:
  static const D3DXVECTOR3                UNIT_VECTOR;
  static const FLOAT                      DISPLAY_WINDOW_WIDTH;
  static const FLOAT                      DISPLAY_WINDOW_HEIGHT;
  static const DWORD                      MAX_NUM_TOUCH_RECTS_PER_STATE = 30;
  static const AAVArea<FLOAT>             kTouchRectArea;
  static const AAVVector3<FLOAT>          kExtraOffsetVector;
  static const FLOAT                      fHalfLineWidth;
  static const FLOAT                      fCornerWidth;

  enum EVENT_ID
  {
    EVENT_ID_SET_ROOT_MENU = 1,
    EVENT_ID_SET_MODULE,
    EVENT_ID_MAX
  };

  enum DISPLAY_STATE
  {
    DISPLAY_STATE_ROOT_MENU,
    DISPLAY_STATE_MAX
  };

  struct HEADING
  {
    D3DXVECTOR3 v;
    CHAR        s[3];
  };

  struct READ_OBJECT_DATA
  {
    INT                           bOnGround;
    __SVSPGeoCoordinates3<FLOAT>   kLocation;
    AAVVector3<FLOAT>             kBodyVelocity;
    FLOAT                         dHeading;
    INT                           dAirSpeed;
    INT                           dAltitude;
  };

  class OBJECT
  {
  public:
    DWORD                         dwObjectID;
    OBJECT                       *pkPrevObject;
    OBJECT                       *pkNextObject;
    __SVSPModule                  *pkModule;
    AAVVector3<FLOAT>             kGlobalVector;
    AAVVector3<FLOAT>             kGlobalVelocity;
    D3DXMATRIXA16                 kHeadingMatrix;
    DOUBLE                        dLastUpdateTime;
    D3DXVECTOR3                   kLocalOffsetVector;
    FLOAT                         fClosingInVelocity;
    BOOL                          bHasRangeData;
    FLOAT                         fSlantRangeMiles;
    USHORT                        usTTIMinutes;
    USHORT                        usTTISeconds;
    USHORT                        usTTI;
    UINT                          uiDTI;
    READ_OBJECT_DATA              kData;
  };

                                  __SVSPModule(CHAR                 *pcName, 
                                              class __SVSPAvionics  *pkAvionics, 
                                              DWORD                 dwThreadSleepTime, 
                                              BOOL                  bSimConnectRequired, 
                                              DispatchProc          pDispatchProcessCallback,
                                              LPPD3DXGEOMETRYOBJECT ppkGeomObjectsList, 
                                              UINT                  uiNumGeomObjects, 
                                              LPPD3DXTEXTUREOBJECT  ppkTextureObjectsList = NULL, 
                                              UINT                  uiNumTextureObjects = 0, 
                                              __SVSPTouchRect          (*pkTouchMapTable)[__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE] = NULL, 
                                              UINT                  uiMaxNumDisplayStates = 0);
  virtual                        ~__SVSPModule();
  virtual VOID                    Start();
  virtual VOID                    Stop();
  virtual VOID                    Pause();
  inline  VOID                    SetValid(bool bValid) { m_bValid = bValid; }
  inline  BOOL                    IsValid() const { return m_bValid; };
  inline bool                     IsStarted() const { return (m_hThreadHandle != 0); }
  inline DWORD                    GetThreadId() const { return m_dwThreadId; }
  inline const __SVSPString<CHAR> *GetName() const { return &m_acName; }
  virtual VOID                    RenderOnHUD(LPD3DXDEVICE pkDevice);
  virtual VOID                    RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  virtual VOID                    SetOPM(D3DXMATRIX &rkOPMatrix) { m_kOPMatrix = rkOPMatrix; }
  virtual BOOL                    MouseHandler(D3DXMATRIX &rkOPMatrix, PPIXPOINT relative_point, FLAGS32 mouse_flags);
  virtual BOOL                    EventHandler(DWORD dwEvent, DWORD dwEventData);
  inline  UINT                    GetNumDisplayStates() const { return m_uiMaxNumDisplayStates; }

  virtual VOID                    Release             ();
  virtual HRESULT                 Reset               (LPD3DXDEVICE pkDevice);
  virtual HRESULT                 Present             (LPD3DXDEVICE pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)                               ;
  virtual HRESULT                 EndScene            (LPD3DXDEVICE pkDevice)                                                                                                                                   ;
  virtual HRESULT                 CreateTexture       (LPD3DXDEVICE pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) ;
  virtual HRESULT                 SetTexture          (LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)                                                                                      ;
//virtual HRESULT                 DrawIndexedPrimitive(LPD3DXDEVICE pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)           ;

  inline VOID                     GetTransformMatrixFromLocalNormalizedVector(D3DXMATRIXA16 *pkTransformMatrix, D3DXVECTOR3 *pkLocalNormalizedVector) const
  {
    FLOAT fXZLengh  =  sqrt(pkLocalNormalizedVector->x*pkLocalNormalizedVector->x + pkLocalNormalizedVector->z*pkLocalNormalizedVector->z);
    FLOAT fSinYaw   =  pkLocalNormalizedVector->x/fXZLengh;
    FLOAT fCosYaw   =  pkLocalNormalizedVector->z/fXZLengh;
    FLOAT fSinPitch =  pkLocalNormalizedVector->y;
    FLOAT fCosPitch =  fXZLengh;

    pkTransformMatrix->_11 =  fCosYaw;           pkTransformMatrix->_12 =  0;         pkTransformMatrix->_13 =  fSinYaw;           pkTransformMatrix->_14 =  0;
    pkTransformMatrix->_21 =  fSinPitch*fSinYaw; pkTransformMatrix->_22 =  fCosPitch; pkTransformMatrix->_23 = -fSinPitch*fCosYaw; pkTransformMatrix->_24 =  0;
    pkTransformMatrix->_31 = -fCosPitch*fSinYaw; pkTransformMatrix->_32 =  fSinPitch; pkTransformMatrix->_33 =  fCosPitch*fCosYaw; pkTransformMatrix->_34 =  0;
    pkTransformMatrix->_41 =  0;                 pkTransformMatrix->_42 =  0;         pkTransformMatrix->_43 =  0;                 pkTransformMatrix->_44 =  1;
  }

  inline VOID                     GetRotationAroundUnitVector(D3DXMATRIX *pkRotationMatrix, D3DXVECTOR3 *pkUnitVector, FLOAT fAngle)
  {
    FLOAT c = cos(fAngle);
    FLOAT s = sin(fAngle);
    FLOAT t = 1 - cos(fAngle);

    FLOAT x = pkUnitVector->x;
    FLOAT y = pkUnitVector->y;
    FLOAT z = pkUnitVector->z;

    pkRotationMatrix->_11 = t*x*x + c;   pkRotationMatrix->_12 = t*x*y + s*z; pkRotationMatrix->_13 = t*x*z - s*y; pkRotationMatrix->_14 = 0;
    pkRotationMatrix->_21 = t*x*y - s*z; pkRotationMatrix->_22 = t*y*y + c;   pkRotationMatrix->_23 = t*y*z + s*x; pkRotationMatrix->_24 = 0;
    pkRotationMatrix->_31 = t*x*z + s*y; pkRotationMatrix->_32 = t*y*z - s*x; pkRotationMatrix->_33 = t*z*z + c;   pkRotationMatrix->_34 = 0;
    pkRotationMatrix->_41 = 0;           pkRotationMatrix->_42 = 0;           pkRotationMatrix->_43 = 0;           pkRotationMatrix->_44 = 1;
  }


protected:
  inline  DOUBLE                  GetSystemTime() const { return ((DOUBLE)GetTickCount())/1000.0; }
  virtual VOID                    Process();
  virtual VOID                    DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);

  bool                            m_bValid;
  __SVSPString<CHAR>              m_acName;
  class __SVSPAvionics           *m_pkAvionics;
  DWORD                           m_dwThreadSleepTime;
  BOOL                            m_bSimConnectRequired;
  DispatchProc                    m_pDispatchProcessCallback;
  LPPD3DXGEOMETRYOBJECT           m_ppkGeomObjects;
  UINT                            m_uiNumGeomObjects;
  LPPD3DXTEXTUREOBJECT            m_ppkTextureObjects;
  UINT                            m_uiNumTextureObjects;
  const __SVSPTouchRect          (*m_pkTouchMapTable)[MAX_NUM_TOUCH_RECTS_PER_STATE];
  UINT                            m_uiMaxNumDisplayStates;
  DOUBLE                          m_dCurrentTime;
  DOUBLE                          m_dDeltaTime;
  HRESULT                         m_hResult;
  bool                            m_bAbort;
  HANDLE                          m_hSimConnect;
  D3DXMATRIXA16                   m_kOPMatrix;
  DWORD                           m_dwThreadId;
  HANDLE                          m_hThreadHandle;
  HANDLE                          m_hThreadStartEvent;
  HANDLE                          m_hThreadExitEvent;
  DWORD                           m_dwProcessUpdateMask;

public:
  D3DXVECTOR2                     m_kMouseVector;
  __SVSPTouchRect                 m_kWindowRect;
  UINT                            m_eDisplayState;
  bool                            m_bActive;
  AAVVector3<FLOAT>               m_kVector;
  D3DXVECTOR3                     m_kLocalOffsetVector;
  D3DXMATRIXA16                   m_kOnHddOffsetMatrix;

private:
  friend DWORD WINAPI           __Process( LPVOID lpParam );
  friend VOID CALLBACK          __DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);

};


#endif	/* __SVSP_MODULE_H__ */


