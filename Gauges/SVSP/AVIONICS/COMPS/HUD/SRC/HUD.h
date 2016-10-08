#ifndef __HUD_MODULE_H__
#define __HUD_MODULE_H__
#include <__SVSPModule.h>




class HUD : public __SVSPModule
{
public:
  static const FLOAT      WAY_POINT_ASPECT_RATIO;
  static const UINT       AIRSPEED_SCALE_MODULO;
  static const UINT       ALTITUDE_SCALE_MODULO;
  static const UINT       HEADING_SCALE_MODULO;
  static const FLOAT      OFFSET_X;
  static const UINT       NUM_AGL_ALTITUDE_SCALE_TABLE_ENTRIES   = 7;
  static const UINT       MAX_NUM_STENCIL_TEXTURES                   = 12;
  static const UINT       STENCIL_TEXTURE_WIDTH                      = 4;
  static const UINT       STENCIL_TEXTURE_HEIGHT                     = 512;
  static const FLOAT      cMaxFlightPathMarkerDeviationAngle;
 
  struct AGL_ALTITUDE_SCALE
  {
    D3DXVECTOR3  kVector;
    CHAR s[4];
  };

  enum 
  {
    MESSAGE_SYSTEM_FAILURE = (WM_USER + 1304),
    MESSAGE_SCALE_INCREASE,
    MESSAGE_SCALE_DECREASE,
    MESSAGE_STENCIL_SCALE_INCREASE,
    MESSAGE_STENCIL_SCALE_DECREASE,
  };

  typedef enum 
  {
    DISPLAY_MODE_HUD = 0,
    DISPLAY_MODE_HMD,

    MAX_NUM_DISPLAY_MODES
  } DISPLAY_MODE;

                           HUD(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual                 ~HUD();
  virtual VOID             Render(LPD3DXDEVICE pkDevice);
          VOID             SetScale(FLOAT fScale);
          VOID             SetProjectorScale(FLOAT fScale);
          VOID             SetMode(HUD::DISPLAY_MODE eMode);
          DISPLAY_MODE     GetMode() const { return m_eMode; };
          VOID             IncreaseScale();
          VOID             DecreaseScale();
          VOID             IncreaseProjectorScale();
          VOID             DecreaseProjectorScale();
  inline  D3DXMATRIX       GetTransformMatrix() const { return m_kHUDTransformMatrix; }
  inline  LPD3DXTEXTURE    GetStencilTexture() const { return m_apkStencilTexture[0]; }
  virtual VOID             Release                      ()                                                                                                                                                          ;
  virtual HRESULT          CreateTexture                (LPD3DXDEVICE pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)   ;
  virtual HRESULT          SetTexture                   (LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)                                                                                        ;
          HRESULT          DrawIndexedPrimitive         (LPD3DXDEVICE pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)             ;
   static HRESULT FASTCALL DrawIndexedPrimitive_Callback(HUD *pkThis, LPD3DXDEVICE pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);

private:
  virtual VOID             Process();
          VOID             RenderHSIData            (LPD3DXDEVICE pkDevice);
          VOID             RenderBankPointer        (LPD3DXDEVICE pkDevice);
          VOID             RenderNoseDirectionMarker(LPD3DXDEVICE pkDevice);
          VOID             RenderFlightPathMarker   (LPD3DXDEVICE pkDevice);
          VOID             RenderAoABracket         (LPD3DXDEVICE pkDevice);
          VOID             RenderSteeringData       (LPD3DXDEVICE pkDevice);
          VOID             RenderLockedTargetData   (LPD3DXDEVICE pkDevice);
          VOID             RenderILSData            (LPD3DXDEVICE pkDevice);
          VOID             RenderPitchScale         (LPD3DXDEVICE pkDevice);
          VOID             RenderHeadingScale       (LPD3DXDEVICE pkDevice);
          VOID             RenderAirspeedScale      (LPD3DXDEVICE pkDevice);
          VOID             RenderAltitudeScale      (LPD3DXDEVICE pkDevice);
          VOID             RenderAttentionAreas     (LPD3DXDEVICE pkDevice);
          VOID             RenderNAVMode            (LPD3DXDEVICE pkDevice);
          VOID             RenderFCRMode            (LPD3DXDEVICE pkDevice);
          VOID             RenderMissionData        (LPD3DXDEVICE pkDevice);
          VOID             RenderT2WScale           (LPD3DXDEVICE pkDevice);

  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_HUD_STENCIL_CIRCLED,
    GEOM_OBJECT_NOSE_DIRECTION_MARKER,
    GEOM_OBJECT_HEADING_BOX,
    GEOM_OBJECT_AIRSPEED_BOX,
    GEOM_OBJECT_MSL_ALTITUDE_BOX,
    GEOM_OBJECT_AGL_ALTITUDE_BOX,
    GEOM_OBJECT_AIRBRAKE_BOX,
    GEOM_OBJECT_HEADING_SCALE,
    GEOM_OBJECT_SPEED_SCALE,
    GEOM_OBJECT_ALTITUDE_SCALE,
    GEOM_OBJECT_AGL_ALTITUDE_SCALE,
    GEOM_OBJECT_AGL_ALTITUDE_SCALE_BAR,
    GEOM_OBJECT_AGL_ALTITUDE_SCALE_BAR2,
    GEOM_OBJECT_DIVE_SCALE,
    GEOM_OBJECT_CLIMB_SCALE,
    GEOM_OBJECT_HORIZON,
    GEOM_OBJECT_HORIZON_WARNING,
    GEOM_OBJECT_FLIGHT_PATH_MARKER,
    GEOM_OBJECT_FLIGHT_DIRECTION_WARNING,
    GEOM_OBJECT_STEERING_CUE_MARKER,
    GEOM_OBJECT_BANK_SCALE,
    GEOM_OBJECT_BANK_POINTER,
    GEOM_OBJECT_LOCALIZER,
    GEOM_OBJECT_GLIDESLOPE,
    GEOM_OBJECT_HSI_SCALE,
    GEOM_OBJECT_HSI_HEADING_BUG,
    GEOM_OBJECT_ADF_BEARING,
    GEOM_OBJECT_AOA_BRACKET,
    GEOM_OBJECT_WIND_DIRECTION,
    GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_SCALE,
    GEOM_OBJECT_THRUST_TO_WEIGHT_RATIO_BAR,
    GEOM_OBJECT_WARN_BAR,
    GEOM_OBJECT_NUM_OBJECTS
  };

  static const UINT       NUM_HUD_STENCIL_CIRCLED_VERTS          = 36*3;
  static const UINT       NUM_NOSE_DIRECTION_MARKER_VERTS        = 24;
  static const UINT       NUM_HEADING_BOX_VERTS                  = 4;
  static const UINT       NUM_AIRSPEED_BOX_VERTS                 = 4;
  static const UINT       NUM_MSL_ALTITUDE_BOX_VERTS             = 4;
  static const UINT       NUM_AGL_ALTITUDE_BOX_VERTS             = 4;
  static const UINT       NUM_AIRBRAKE_BOX_VERTS                 = 4;
  static const UINT       NUM_HEADING_SCALE_VERTS                = 8*6;
  static const UINT       NUM_AIRSPEED_SCALE_VERTS               = 15*6;
  static const UINT       NUM_ALTITUDE_SCALE_VERTS               = 15*6;
  static const UINT       NUM_AGL_ALTITUDE_SCALE_VERTS           = 12*6;
  static const UINT       NUM_AGL_ALTITUDE_SCALE_BAR_VERTS       = 4;
  static const UINT       NUM_AGL_ALTITUDE_SCALE_BAR2_VERTS      = 4;
  static const UINT       NUM_DIVE_SCALE_VERTS                   = 60*20;
  static const UINT       NUM_CLIMB_SCALE_VERTS                  = 24*20;
  static const UINT       NUM_HORIZON_VERTS                      = 12;
  static const UINT       NUM_HORIZON_WARNING_VERTS              = 36;
  static const UINT       NUM_FPM_VERTS                          = 4;
  static const UINT       NUM_FDW_VERTS                          = 4;
  static const UINT       NUM_STEERING_CUE_VERTS                 = 4;
  static const UINT       NUM_BANK_SCALE_VERTS                   = 9*6;
  static const UINT       NUM_BANK_POINTER_VERTS                 = 4;
  static const UINT       NUM_LOCALIZER_VERTS                    = 16;
  static const UINT       NUM_GLIDESLOPE_VERTS                   = 6;
  static const UINT       NUM_HSI_SCALE_VERTS                    = 24*2;
  static const UINT       NUM_HSI_HEADING_BUG_VERTS              = 4;
  static const UINT       NUM_ADF_BEARING_VERTS                  = 3;
  static const UINT       NUM_AOA_BRACKET_VERTS                  = 16;
  static const UINT       NUM_WIND_DIRECTION_VERTS               = 6;
  static const UINT       NUM_THRUST_TO_WEIGHT_RATIO_SCALE_VERTS = 5;
  static const UINT       NUM_THRUST_TO_WEIGHT_RATIO_BAR_VERTS   = 4;
  static const UINT       NUM_WARN_BAR_VERTS                     = 4;

  enum TEXTURE_OBJECTS_LIST
  {
    TEXTURE_OBJECT_FPM, 
    TEXTURE_OBJECT_FDW,
    TEXTURE_OBJECT_SCM, 
    TEXTURE_OBJECT_HDG, 
    TEXTURE_OBJECT_ALT_KIAS,
    TEXTURE_OBJECT_BKP, 
    TEXTURE_OBJECT_WARN, 

    TEXTURE_OBJECT_NUM_OBJECTS
  };

  struct PITCH_DEGREE
  {
    D3DXVECTOR3  kLeft;
    D3DXVECTOR3  kRight;
    CHAR         s[5];
  };

  static const D3DXVECTOR2 g_kMaxFlightDirectionArea;

  HEADING                 m_akHSIHeading[12];
  PITCH_DEGREE            m_PitchDegrees[2*20];

  D3DMATERIAL9            m_Mtrl;
  D3DVIEWPORT9            m_ViewPort;
  D3DXMATRIXA16           m_matIdentity;
  D3DXMATRIXA16           m_matHeading;
  D3DXMATRIXA16           m_matAttitudeScaleTransform;
  //D3DXMATRIXA16           m_matAuxTransform;
  D3DXMATRIXA16           m_kVelocityVectorTransformMatrix;
  D3DXMATRIXA16           m_matWPNTransform;
  D3DXMATRIXA16           m_matBank;
  D3DXMATRIXA16           m_matPitch;
  D3DXMATRIXA16           m_matLocaliserTransform;
  D3DXMATRIXA16           m_matGlideslopeTransform;
  D3DXMATRIXA16           m_AoABracketTranslationMatrix;
  D3DXMATRIXA16           m_matHeadingNames;
  D3DXMATRIXA16           m_matWayPoints;
  D3DXMATRIXA16           m_matWayPointsScaling;
  D3DXMATRIXA16           m_kHUDTransformMatrix;
  D3DXMATRIXA16           m_kBankScaleTranslateMatrix; 
  D3DXMATRIXA16           m_kFPMTransformMatrix;
  D3DXMATRIXA16           m_kFPMTranslatioMatrix;
  D3DXMATRIXA16           m_kHUDShiftMatrix;
  D3DXMATRIXA16           m_kAGLAltitudeScaleMatrix;

  FLOAT                   m_fAngle;
  DWORD                   m_dwFS_ReadThreadId;
  UINT                    m_uiHeading[3];
  INT                     m_iAirSpeed[3];
  INT                     m_iAltitude[3];
  D3DXVECTOR3             m_kNoseDirectionMarkerVector; 
  D3DXVECTOR3             m_kSteeringCueMarkerVector; 
  D3DXVECTOR3             m_kAGLDesignatorVector; 
  D3DXVECTOR3             m_kAGLAltitudeValueVector; 
  D3DXVECTOR3             m_kALDesignatorVector; 
  D3DXVECTOR3             m_kALValueVector; 
  D3DXVECTOR3             m_kMachValueVector; 
  D3DXVECTOR3             m_kGValueVector; 
  D3DXVECTOR3             m_kMaxGValueVector; 
  D3DXVECTOR3             m_kFuelWarningVector; 
  D3DXVECTOR3             m_kHeadingValueVector; 
  D3DXVECTOR3             m_kHeadingScaleVector; 
  D3DXVECTOR3             m_akHeadingScaleValueVector[3];
  D3DXVECTOR3             m_kMSLAltitudeValueVector;
  D3DXVECTOR3             m_kMSLAltitudeScaleVector;
  D3DXVECTOR3             m_akMSLAltitudeScaleValueVector[3];
  D3DXVECTOR3             m_kAirspeedValueVector; 
  D3DXVECTOR3             m_kAirspeedScaleVector; 
  D3DXVECTOR3             m_akAirspeedScaleValueVector[3];
  D3DXVECTOR3             m_kAoAValueVector;
  D3DXVECTOR3             m_kMasterModeStateIndicationVector;

  D3DXVECTOR3             m_kCurrentWaypointNameVector;
  D3DXVECTOR3             m_kTTIValueVector;
  D3DXVECTOR3             m_kCurrentWaypointNumVector;

  D3DXVECTOR3             m_kInboundBodyVelocityNormalized;
  D3DXVECTOR3             m_kInboundLocalVelocityNormalized;
  D3DXVECTOR2             m_kXZLocalVelocityVectorNormalized;

  D3DXVECTOR3             m_kLATValueVector;
  D3DXVECTOR3             m_kLNGValueVector;
  D3DXVECTOR3             m_kELVValueVector;
  D3DXVECTOR3             m_kTOSValueVector;


  bool                    m_bDrawFlightDirectionWarning;

  DISPLAY_MODE            m_eMode;
  CHAR                    m_acStr[64];
  D3DXMATRIXA16           m_kViewMatrix;
  D3DXMATRIXA16           m_kTransposedViewMatrix;
  D3DXMATRIXA16           m_kStencilScalingMatrix;
  D3DXMATRIXA16           m_kHUDStencilScalingMatrix;
  D3DXMATRIXA16           m_kHMDStencilScalingMatrix;
  D3DXMATRIXA16           m_kStencilTransformMatrix;

  LPD3DXSURFACE           m_pkDefaultDepthStencilSurface;
  DWORD                   m_dwDepthStencilTestEnable;
  DWORD                   m_dwZEnable;
  DWORD                   m_dwZWriteEnable;
  DWORD                   m_dwZFunction;
  DWORD                   m_dwAphaBlendEnable;
  DWORD                   m_dwBlendOperation;
  DWORD                   m_dwSourceBlend;
  DWORD                   m_dwSourceBlendAlpha;
  DWORD                   m_dwDestBlend;
  DWORD                   m_dwDestBlendAlpha;
  DWORD                   m_dwSeparateAlphaBlendEnable;
  DWORD                   m_dwAphaTestEnable;

  LPD3DXTEXTURE           m_apkStencilTexture[HUD::MAX_NUM_STENCIL_TEXTURES];
  UINT                    m_uiNumStencilTextures;
};

#endif /* __HUD_MODULE_H__ */
