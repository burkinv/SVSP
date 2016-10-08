#ifndef	__VPM_MODULE_H__
#define	__VPM_MODULE_H__

#include "__SVSPModule.h"
#include "__SVSPAvionics.h"

class VPM : public __SVSPModule
{
public:
  enum 
  {
    MESSAGE_UPDATE_TOWER_VECTOR = (WM_USER + 1304),
  };

               VPM(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
 virtual      ~VPM();

private:
  virtual VOID Process();
};

#endif	/* __VPM_MODULE_H__ */
