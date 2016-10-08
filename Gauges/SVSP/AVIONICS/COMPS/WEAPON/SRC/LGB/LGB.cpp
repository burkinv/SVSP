#include "LGB.h"

LGB::LGB(CHAR *pcName, UINT uiStationID, UINT uiUnitID, UINT uiWeight, __SVSPAvionics *pkAvionics, AAVVector3<FLOAT> &rkPilonVector, AAVVector3<FLOAT> &rkPilonAttitude) :
  MISSILE(pcName, "LGB", uiStationID, uiUnitID, uiWeight, pkAvionics, rkPilonVector, rkPilonAttitude)
{

}

LGB::~LGB()
{
  MISSILE::Stop();
}

VOID LGB::Render(D3DXMATRIX &rkOPMatrix)
{
}

