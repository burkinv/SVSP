#ifndef	__DDI_MODULE_H__
#define	__DDI_MODULE_H__

#include "__SVSPModule.h"

class DDI : public __SVSPModule
{
  friend class HDD;
public:
                          DDI(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
                         ~DDI();
          VOID            RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  virtual BOOL            MouseHandler(PPIXPOINT relative_point, FLAGS32 mouse_flags);
  virtual BOOL            EventHandler(DWORD dwEvent, DWORD dwEventData);


protected:
  virtual VOID            Start();
  virtual VOID            Process();
  virtual VOID            DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData);
  friend VOID CALLBACK    DDI__DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext);

private:
  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_BACKGROUND,
    GEOM_OBJECT_BOUNDARY_FRAME,
    GEOM_OBJECT_TOUCH_RECT,
    GEOM_OBJECT_NUM_OBJECTS
  };

  enum TEXTURE_OBJECTS_LIST
  {
    TEXTURE_OBJECT_NUM_OBJECTS
  };

  enum DDI_PAGE
  {
    DDI_PAGE_ROOT_MENU,
    DDI_PAGE_MAX
  };

  enum EVENT_ID
  {
    EVENT_ID_00 = __SVSPModule::EVENT_ID_MAX,
    EVENT_ID_01,
    EVENT_ID_02,
    EVENT_ID_03,
    EVENT_ID_04,
    EVENT_ID_05,
    EVENT_ID_06,
    EVENT_ID_07,
    EVENT_ID_08,
    EVENT_ID_09,
    EVENT_ID_10,
    EVENT_ID_11,
    EVENT_ID_12,
    EVENT_ID_13,
    EVENT_ID_14,
    EVENT_ID_15,
    EVENT_ID_16,
    EVENT_ID_17,
    EVENT_ID_18,
    EVENT_ID_19,

    EVENT_ID_MAX
  };

  static const __SVSPTouchRect g_akTouchMapTable[DDI::DISPLAY_STATE_MAX][DDI::MAX_NUM_TOUCH_RECTS_PER_STATE];
  static const UINT       NUM_BACKGROUND_VERTICES     = 4;
  static const UINT       NUM_BOUNDARY_FRAME_VERTICES = 5;

  __SVSPModule            *m_pkModule;
  DDI_PAGE                m_ePage;

};

#endif	/* __DDI_MODULE_H__ */
