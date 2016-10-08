#ifndef __SVSP_TOUCH_RECT_H__
#define __SVSP_TOUCH_RECT_H__

#include <windows.h>
#define _USE_MATH_DEFINES
#include <Math.h>
#include "__SVSPConfig.h"
#include "__SVSPDebug.h"
#include "__SVSPMath.h"
#include "__SVSPArea.h"
#include "__SVSPString.h"

class __SVSPTouchRect
{
public:
  __SVSPString<CHAR>       acName;
  INT                   eEvent;
  DWORD                 dwEventData;
  AAVArea<FLOAT>        kArea;
  AAVVector3<FLOAT>     kVector;
  D3DXMATRIXA16         kMatrix;
  UINT                  uiGeomObjectNum;
  UINT                  uiTextureObjectNum;

  __SVSPTouchRect()
  {
    this->acName             = __SVSPString<CHAR>(NULL);
    this->eEvent = 0;
  }

  __SVSPTouchRect(CHAR *pcName, INT eEvent, DWORD dwEventData, AAVVector3<FLOAT> rkVector, AAVArea<FLOAT> rkArea, UINT uiGeomObjectNum = 0, UINT uiTextureObjectNum = 0)
  {
    this->acName             = __SVSPString<CHAR>(pcName);
    this->eEvent             = eEvent;
    this->dwEventData        = dwEventData;
    D3DXMatrixTranslation(&this->kMatrix, rkVector.x, rkVector.y, rkVector.z); 
    this->kVector            = rkVector;
    this->kArea              = rkArea;
    this->uiGeomObjectNum    = uiGeomObjectNum;
    this->uiTextureObjectNum = uiTextureObjectNum;
  }

  __SVSPTouchRect(CHAR *pcName, INT eEvent, DWORD dwEventData, D3DXMATRIX *pkMatrix, AAVArea<FLOAT> &rkArea, UINT uiGeomObjectNum = 0, UINT uiTextureObjectNum = 0)
  {
    this->acName             = __SVSPString<CHAR>(pcName);
    this->eEvent             = eEvent;
    this->dwEventData        = dwEventData;
    this->kMatrix            = *pkMatrix;
    this->kVector            = AAVVector3<FLOAT>(kMatrix._41, kMatrix._42, kMatrix._43);
    this->kArea              = rkArea;
    this->uiGeomObjectNum    = uiGeomObjectNum;
    this->uiTextureObjectNum = uiTextureObjectNum;
  }
};

#endif	/* __SVSP_TOUCH_RECT_H__ */


