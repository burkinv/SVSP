#ifndef __EICAS_MODULE_H__
#define __EICAS_MODULE_H__
#include <__AAVModule.h>

/**
  Engine Indication and Crue Allert Module.
  - (HDD)Fuel data indication.
*/

class EICAS : public __AAVModule
{
public:
  static FLOAT            FUEL_1500;
  static FLOAT            FUEL_800;
  static FLOAT            FUEL_500;
  static FLOAT            BINGO_FUEL;

  enum
  {
    REQUEST_ID_READ_FUEL_DATA,
  } REQUEST_ID;

  enum  
  {
    DATA_ID_READ_FUEL_DATA,
  } DATA_ID;

                          EICAS(__AAVAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual                ~EICAS();
          VOID            RenderOnHDD(D3DXMATRIX &rkOPMatrix);
  virtual BOOL            EventHandler(DWORD dwEvent, DWORD dwEventData);

 struct FUEL_DATA
 {
   FLOAT fC1PercentLeft;
   FLOAT fC2PercentLeft;
   FLOAT fC3PercentLeft;
   FLOAT fLMPercentLeft;
   FLOAT fRMPercentLeft;
   FLOAT fFuelTotalQuantityWeight;
 } m_kFuelData;

  FLOAT                   m_fFuelTotalQuantityWeightKg;


private:
  virtual VOID            Start();
  virtual VOID            Process();
  virtual VOID            DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);
  friend VOID CALLBACK    EICAS__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);

  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_BACKGROUND, 
    GEOM_OBJECT_TOUCH_RECT,
    GEOM_OBJECT_FRAME_BORDER,
    GEOM_OBJECT_FRAME_CORNERS,
    GEOM_OBJECT_ACTIVE_FRAME_BORDER,
    GEOM_OBJECT_ACTIVE_FRAME_CORNERS,

    GEOM_OBJECT_NUM_OBJECTS
  };

  static const UINT       NUM_BACKGROUND_VERTICES = 4;
  static const UINT       NUM_FRAME_BORDER_VERTICES                = 10;
  static const UINT       NUM_FRAME_CORNERS_VERTICES               = 12;
  static const UINT       NUM_ACTIVE_FRAME_BORDER_VERTICES         = 10;
  static const UINT       NUM_ACTIVE_FRAME_CORNERS_VERTICES        = 12;

  enum TEXTURE_OBJECTS_LIST
  {
    TEXTURE_OBJECT_BACKGROUND, 
    TEXTURE_OBJECT_UP_SIDE_BUTTON,
    TEXTURE_OBJECT_LT_SIDE_BUTTON,
    TEXTURE_OBJECT_RT_SIDE_BUTTON,
    TEXTURE_OBJECT_BM_SIDE_BUTTON,

    TEXTURE_OBJECT_NUM_OBJECTS
  };

  enum DISPLAY_STATE
  {
    DISPLAY_STATE_MAIN = __AAVModule::DISPLAY_STATE_MAX,
    DISPLAY_STATE_MAX
  };

  static __AAVTouchRect g_akTouchMapTable[EICAS::DISPLAY_STATE_MAX][__AAVModule::MAX_NUM_TOUCH_RECTS_PER_STATE];


  char                    m_acStr[32];

  FLOAT                   m_dEstimatedFlightTime;

  D3DXVECTOR3             m_kPitchTrimValueVector;
  D3DXVECTOR3             m_kFuelTotalQuantityValueVector;

  D3DXVECTOR3             m_kEFTValueVector;
  D3DXVECTOR3             m_kC1FuelPercentLeftValueVector;
  D3DXVECTOR3             m_kC2FuelPercentLeftValueVector;
  D3DXVECTOR3             m_kC3FuelPercentLeftValueVector;
  D3DXVECTOR3             m_kLMFuelPercentLeftValueVector;
  D3DXVECTOR3             m_kRMFuelPercentLeftValueVector;
};

#endif /* __EICAS_MODULE_H__ */
