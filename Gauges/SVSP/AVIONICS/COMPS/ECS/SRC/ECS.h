#ifndef	__ECS_MODULE_H__
#define	__ECS_MODULE_H__

#include "__SVSPModule.h"

/** 
  Engine Control System.
  - Sim Engine Data Communication Interface.
  - Secondary Lift Fan Control.
  - Secondary Thrust Vector Control.
  - (HDD) Engine Data Indication gauge.
  - (HDD) Fuel Data Indication (Not implemented).
*/
class ECS : public __SVSPModule
{
public:
  static const UINT  NUM_ENGINES          = 1;
  static const FLOAT THRUST_VECTOR_VALUE_LEVEL_OFF;
  static const FLOAT THRUST_VECTOR_VALUE_PITCH_UP;
  static const FLOAT THRUST_VECTOR_VALUE_PITCH_DOWN;
  static const FLOAT THRUST_VECTOR_VALUE_FULL_DOWN;
  static const FLOAT FUEL_1500;
  static const FLOAT FUEL_800;
  static const FLOAT FUEL_500;
  static const FLOAT BINGO_FUEL;

  static const UINT MAX_N1_RPM           = 130;
  static const UINT MAX_INDICATED_N1_RPM = 100;
  static const UINT MAX_N2_RPM           = 155;
  static const UINT MAX_INDICATED_N2_RPM = 100;
  static const UINT MAX_ITT              = 1800;
  static const UINT MAX_INDICATED_ITT    = 1500;
  static const UINT MAX_EGT              = 1300;
  static const UINT MAX_INDICATED_EGT    = 1100;
  static const UINT MAX_THRUST           = 33000;


  enum 
  {
    PROCESS_UPDATE_AIRCRAFT_ENGINE_DATA  = 0x1,
    PROCESS_UPDATE_AIRCRAFT_FUEL_DATA     = 0x2,
  };

  typedef enum
  {
    GAUGE_ELEMENT_THRUST, 
    GAUGE_ELEMENT_N1_RPM, 
    GAUGE_ELEMENT_N2_RPM, 
    GAUGE_ELEMENT_EGT_RPM,

    MAX_NUM_GAUGE_ELEMENTS
  } GAUGE_ELEMENT;

  struct ENGINE 
  {
    FLOAT              dThrottle;
    FLOAT              dTIT;
    FLOAT              fN[2];
    USHORT             usML;
    FLOAT              dITT;                   //Degrees Celsius
    FLOAT              dEGT;                   //Degrees Celsius
    INT                dwAB;
    FLOAT              dNozzle;
    FLOAT              dFF;                    //Kg/Min
    FLOAT              dOilPressure;           //Pounds per square foot 
    FLOAT              dOilTemp;               //Degrees Celsius
    FLOAT              dHydPressure;           //Pounds per square foot 
    FLOAT              dCurrentThrustCapacity;
    FLOAT              dThrust;
    FLOAT              dThrustPercent;
    FLOAT              dThrustVectorAngle;
    AAVVector3<FLOAT> kBodyThrust;
    AAVVector3<FLOAT> kBodyTorque;
  };

  struct GEN_ENGINE_DATA
  {
    FLOAT dThrottle;
    FLOAT dTIT;
    FLOAT dN[2];
    FLOAT dITT;
    FLOAT dEGT;
    INT   dwAB;
    FLOAT dThrust;
    FLOAT dNozzle;
    FLOAT dFuelFlowPPH;
    FLOAT dOilPressure;
    FLOAT dOilTemp;
    FLOAT dHydPressure;
  };

  struct READ_OBJECT_STATE_DATA
  {
    //FLOAT          dType;
    GEN_ENGINE_DATA  akEngineData[ECS::NUM_ENGINES];
  } m_kData;


 struct FUEL_DATA
 {
   FLOAT fC1PercentLeft;
   FLOAT fC2PercentLeft;
   FLOAT fC3PercentLeft;
   FLOAT fLMPercentLeft;
   FLOAT fRMPercentLeft;
   FLOAT fFuelTotalQuantityWeight;
 } m_kFuelData;

                     ECS(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual           ~ECS();
          VOID       SetGainThrustVectorValue(FLOAT fThrustVectorValue);
  inline  FLOAT      GetGainThrustVectorValue()  const { return m_dThrustVectorValueToGain; }
  inline  FLOAT      GetThrustVectorValue()      const { return m_dThrustVectorValue; }
          VOID       SetGainLiftFanValue(FLOAT fLiftFanValue);
  inline  FLOAT      GetGainLiftFanValue()       const { return m_dLiftFanValueToGain; }
  inline  FLOAT      GetLiftFanValue()           const { return m_dLiftFanValue; }
          VOID       RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  virtual BOOL       EventHandler(DWORD dwEvent, DWORD dwEventData);
          FLOAT      GetTotalFuelQuantityKg() const { return m_fFuelTotalQuantityWeightKg; }

  ENGINE             m_akEngine[NUM_ENGINES];
  AAVVector3<FLOAT>  m_kTorqueControlVector;
  FLOAT              m_dAugmentedThrust;

          VOID       RenderGaugeElement(LPD3DXDEVICE pkDevice, ECS::GAUGE_ELEMENT eGaugeElement, D3DXMATRIX &rkEICASMatrix, D3DXVECTOR3 *pkGaugeTranslationVector, FLOAT fScaling = 1);

  template<typename T> inline VOID  RenderGaugeElement(LPD3DXDEVICE pkDevice, __SVSPFont* pkFont, LPD3DXTEXTUREOBJECT pkGaugeCellTextureObject, LPD3DXTEXTUREOBJECT pkGaugeNeedleTextureObject, D3DXMATRIX &rkEICASMatrix, D3DXVECTOR3 *pkGaugeTranslationVector, T tCurrentValue, T tMaxValue, char *pGaugeDesignation = NULL) const 
  {
    char          acStr[32];
    D3DXMATRIXA16 kGaugeTransformMatrix;
    D3DXMATRIXA16 kRotMatrix;
    LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;

    pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_GAUGE_CELL];
    if(pkGeomObject != NULL)
    {
      D3DXMatrixTranslation(&kGaugeTransformMatrix, pkGaugeTranslationVector->x, pkGaugeTranslationVector->y, pkGaugeTranslationVector->z);
      pkGeomObject->SetTransform(pkDevice, &(kGaugeTransformMatrix*rkEICASMatrix));
      pkGeomObject->SetTexture(pkDevice, pkGaugeCellTextureObject);
      pkGeomObject->Render(pkDevice);
    }

    //Current value
    sprintf_s(&acStr[0], sizeof(acStr), "%d", (INT)tCurrentValue);
    pkFont->RenderTextExt(pkDevice, (&acStr[0]),       &D3DXVECTOR3(0,  30, 0), __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwACTIVEWINDOWCOLOR);
    //Designation
    pkFont->RenderTextExt(pkDevice, pGaugeDesignation, &D3DXVECTOR3(0, -40, 0), __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwACTIVEWINDOWCOLOR);

    pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_GAUGE_NEEDLE];
    if(pkGeomObject != NULL)
    {
      FLOAT fAngle = (tCurrentValue < tMaxValue) ? -D3DX_PI*((FLOAT)tCurrentValue/tMaxValue)*270.0f/180.0f : -D3DX_PI*(270.0f/180.0f);
      fAngle += D3DX_PI*0.25f;
      D3DXMatrixRotationZ(&kRotMatrix, fAngle);
      pkGeomObject->SetTransform(pkDevice, &(kRotMatrix*kGaugeTransformMatrix*rkEICASMatrix));
      pkGeomObject->SetTexture(pkDevice, pkGaugeNeedleTextureObject);
      pkGeomObject->Render(pkDevice);
    }

  }


protected:
  virtual VOID            Start();
  virtual VOID            Process();
  virtual VOID            DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);
  friend VOID CALLBACK    ECS__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);

private:
  enum {
    DATA_ID_READ_FUEL_DATA,
    DATA_ID_READ_ENGINE_DATA,
    DATA_ID_WRITE_ENGINE_DATA,
  } DATA_ID;

  enum {
    REQUEST_ID_READ_FUEL_DATA,
    REQUEST_ID_READ_ENGINE_DATA,
    REQUEST_ID_WRITE_ENGINE_DATA,
  } REQUEST_ID;


//struct GEN_ENGINE_WRITE_DATA
  //{
  //  FLOAT dPL;
  //  //FLOAT dAB;
  //};

  //struct SET_OBJECT_STATE_DATA
  //{
  //  //FLOAT                dSmoke;
  //  GEN_ENGINE_WRITE_DATA akEngineData[ECS::NUM_ENGINES];
  //} m_kSetObjectStateData;

  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_BACKGROUND, 
    GEOM_OBJECT_GAUGE_CELL,
    GEOM_OBJECT_GAUGE_NEEDLE, 
    GEOM_OBJECT_TOUCH_RECT,
    GEOM_OBJECT_FRAME_BORDER,
    GEOM_OBJECT_FRAME_CORNERS,
    GEOM_OBJECT_ACTIVE_FRAME_BORDER,
    GEOM_OBJECT_ACTIVE_FRAME_CORNERS,

    GEOM_OBJECT_NUM_OBJECTS
  };

  static const UINT       NUM_BACKGROUND_VERTICES = 4;
  static const UINT       NUM_GAUGE_CELL_VERTICES = 4;
  static const UINT       NUM_GAUGE_NEEDLE_VERTICES = 4;
  static const UINT       NUM_GAUGE_VERTICES = 72;
  static const UINT       NUM_FRAME_BORDER_VERTICES                = 10;
  static const UINT       NUM_FRAME_CORNERS_VERTICES               = 12;
  static const UINT       NUM_ACTIVE_FRAME_BORDER_VERTICES         = 10;
  static const UINT       NUM_ACTIVE_FRAME_CORNERS_VERTICES        = 12;

  enum TEXTURE_OBJECTS_LIST
  {
    TEXTURE_OBJECT_BACKGROUND, 
    TEXTURE_OBJECT_GAUGE_CELL_MAX,
    TEXTURE_OBJECT_GAUGE_CELL_MIN_MAX, 
    TEXTURE_OBJECT_GAUGE_NEEDLE, 
    TEXTURE_OBJECT_UP_SIDE_BUTTON,
    TEXTURE_OBJECT_LT_SIDE_BUTTON,
    TEXTURE_OBJECT_RT_SIDE_BUTTON,
    TEXTURE_OBJECT_BM_SIDE_BUTTON,

    TEXTURE_OBJECT_NUM_OBJECTS
  };

  enum DISPLAY_STATE
  {
    DISPLAY_STATE_MAIN = __SVSPModule::DISPLAY_STATE_MAX,
    DISPLAY_STATE_MAX
  };


  static __SVSPTouchRect g_akTouchMapTable[ECS::DISPLAY_STATE_MAX][__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE];


  FLOAT                   m_dThrustVectorValueToGain;
  FLOAT                   m_dThrustVectorValue;
  FLOAT                   m_dThrustVectorRotationVelocity;
  FLOAT                   m_dLiftFanValueToGain;
  FLOAT                   m_dLiftFanValue;
  FLOAT                   m_dEstimatedFlightTime;
  FLOAT                   m_fFuelTotalQuantityWeightKg;


  D3DXVECTOR3             m_kInletTemperatureValueVector;
  D3DXVECTOR3             m_kThrustPercentValueVector;
  D3DXVECTOR3             m_kN1PercentValueVector;
  D3DXVECTOR3             m_kN2PercentValueVector;
  D3DXVECTOR3             m_kEFFValueVector;
  D3DXVECTOR3             m_kITTValueVector;
  D3DXVECTOR3             m_kEGTValueVector;
  D3DXVECTOR3             m_kNozzleValueVector;
  D3DXVECTOR3             m_kT2WRatioValueVector;
  D3DXVECTOR3             m_kFFValueVector;

  D3DXVECTOR3             m_kEFTValueVector;
  D3DXVECTOR3             m_kC1FuelPercentLeftValueVector;
  D3DXVECTOR3             m_kC2FuelPercentLeftValueVector;
  D3DXVECTOR3             m_kC3FuelPercentLeftValueVector;
  D3DXVECTOR3             m_kLMFuelPercentLeftValueVector;
  D3DXVECTOR3             m_kRMFuelPercentLeftValueVector;

};

#endif	/* __ECS_MODULE_H__ */
