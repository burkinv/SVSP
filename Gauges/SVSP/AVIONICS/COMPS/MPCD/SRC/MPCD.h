#ifndef	__MPCD_MODULE_H__
#define	__MPCD_MODULE_H__

#include "HDD.h"

class MPCD : public HDD
{
public:
                          MPCD(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
                         ~MPCD();
          VOID            Render(LPD3DXDEVICE pkDevice);
  virtual BOOL            EventHandler(DWORD dwEvent, DWORD dwEventData);

protected:

private:
  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_FRAME_BORDER,
    GEOM_OBJECT_NUM_OBJECTS
  };

  enum TEXTURE_OBJECTS_LIST
  {
    TEXTURE_OBJECT_NUM_OBJECTS
  };

  static const UINT       NUM_FRAME_BORDER_VERTICES             = 5;

};

#endif	/* __MPCD_MODULE_H__ */
