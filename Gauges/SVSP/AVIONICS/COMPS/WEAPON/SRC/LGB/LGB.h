#ifndef	__LGB_MODULE_H__
#define	__LGB_MODULE_H__

#include "MISSILE.h"

class LGB : public MISSILE
{
public:
                             LGB(CHAR *pcName, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude);
  virtual                   ~LGB();
  virtual VOID               Render(D3DXMATRIX &rkOPMatrix);

};

#endif	/* __LGB_MODULE_H__ */
