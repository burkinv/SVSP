#ifndef	__MFD_MODULE_H__
#define	__MFD_MODULE_H__

#include "HDD.h"

class MFD : public HDD
{
public:
                          MFD(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
                         ~MFD();
          VOID            Render(LPD3DXDEVICE pkDevice);
  virtual BOOL            MouseHandler(PPIXPOINT relative_point, FLAGS32 mouse_flags);
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

  D3DXMATRIXA16           m_kLeftFrameMatrix;
  D3DXMATRIXA16           m_kRightFrameMatrix;

};

#endif	/* __MFD_MODULE_H__ */
