#ifndef __SVSP_AREA_H__
#define __SVSP_AREA_H__

#include <windows.h>
#include "__SVSPConfig.h"
#include "__SVSPDebug.h"
#include "__SVSPMath.h"

template<class T> class AAVArea
{
public:
  T tWidth;
  T tHeight;

  AAVArea()
  {
    this->tWidth  = 0;
    this->tHeight = 0;
  }

  AAVArea(T tWidth, T tHeight)
  {
    this->tWidth  = tWidth;
    this->tHeight = tHeight;
  }
};


#endif	/* __SVSP_AREA_H__ */


