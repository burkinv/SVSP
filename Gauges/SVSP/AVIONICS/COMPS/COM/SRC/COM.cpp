#include "AC.h"
#include "COM.h"
#include "HUD.h"
#include "SS.h"
#include "math.h"
#include "__SVSPGeoCoordinates3.h"
#include "__SVSPVector3.h"
#include "AIP.h"
#include "HDD.h"
#include "COM.h"
#include "VWS.h"
#include "ECS.h"

const FLOAT COM::DISPLAY_WINDOW_WIDTH  = 850;
const FLOAT COM::DISPLAY_WINDOW_HEIGHT = 290;

const AAVArea<FLOAT> COM::kRTouchRectArea = AAVArea<FLOAT>(80, 40);
static const D3DXVECTOR3 kMenuVector                = D3DXVECTOR3(-COM::DISPLAY_WINDOW_WIDTH/3.6f, -20, 0);
static const D3DXVECTOR3 kInitialVector             = D3DXVECTOR3(kMenuVector.x + COM::kRTouchRectArea.tWidth, kMenuVector.y + 3*(COM::kRTouchRectArea.tHeight + 2.0f), 0);

__SVSPTouchRect COM::g_akTouchMapTable[COM::DISPLAY_STATE_MAX][__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE] = 
{
  /* DISPLAY_STATE_NAV */
  {
    __SVSPTouchRect("NAV\0", EVENT_ID_BTN_NAV,          NULL, D3DXVECTOR3(kMenuVector.x + 0*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_ACTIVE_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("COM\0", EVENT_ID_BTN_COM,          NULL, D3DXVECTOR3(kMenuVector.x + 1*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("IFF\0", EVENT_ID_BTN_IFF,          NULL, D3DXVECTOR3(kMenuVector.x + 2*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("TBN\0", EVENT_ID_BTN_TBN,          NULL, D3DXVECTOR3(kMenuVector.x + 3*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("APP\0", EVENT_ID_BTN_APP,          NULL, D3DXVECTOR3(kMenuVector.x + 4*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("LHT\0", EVENT_ID_BTN_LHT,          NULL, D3DXVECTOR3(kMenuVector.x + 5*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("HUD\0", EVENT_ID_BTN_HUD,          NULL, D3DXVECTOR3(kMenuVector.x + 6*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),

    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
  },

  /* DISPLAY_STATE_COM */
  {

    __SVSPTouchRect("NAV\0", EVENT_ID_BTN_NAV,          NULL, D3DXVECTOR3(kMenuVector.x + 0*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("COM\0", EVENT_ID_BTN_COM,          NULL, D3DXVECTOR3(kMenuVector.x + 1*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_ACTIVE_TOUCH_RECT,  COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("IFF\0", EVENT_ID_BTN_IFF,          NULL, D3DXVECTOR3(kMenuVector.x + 2*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("TBN\0", EVENT_ID_BTN_TBN,          NULL, D3DXVECTOR3(kMenuVector.x + 3*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("APP\0", EVENT_ID_BTN_APP,          NULL, D3DXVECTOR3(kMenuVector.x + 4*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("LHT\0", EVENT_ID_BTN_LHT,          NULL, D3DXVECTOR3(kMenuVector.x + 5*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("HUD\0", EVENT_ID_BTN_HUD,          NULL, D3DXVECTOR3(kMenuVector.x + 6*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),

    __SVSPTouchRect("7\0",   EVENT_ID_BTN_7,            NULL, D3DXVECTOR3(kInitialVector.x + 0*(kRTouchRectArea.tWidth), kInitialVector.y - 0*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("8\0",   EVENT_ID_BTN_8,            NULL, D3DXVECTOR3(kInitialVector.x + 1*(kRTouchRectArea.tWidth), kInitialVector.y - 0*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("9\0",   EVENT_ID_BTN_9,            NULL, D3DXVECTOR3(kInitialVector.x + 2*(kRTouchRectArea.tWidth), kInitialVector.y - 0*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("ENT\0", EVENT_ID_BTN_ENT,          NULL, D3DXVECTOR3(kInitialVector.x + 3*(kRTouchRectArea.tWidth), kInitialVector.y - 0*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect( NULL,   EVENT_ID_BTN_COM_1_1,      NULL, D3DXVECTOR3(kInitialVector.x + 4*(kRTouchRectArea.tWidth), kInitialVector.y - 0*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_NONE),
    __SVSPTouchRect( NULL,   EVENT_ID_BTN_COM_2_1,      NULL, D3DXVECTOR3(kInitialVector.x + 5*(kRTouchRectArea.tWidth), kInitialVector.y - 0*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_NONE),

    __SVSPTouchRect("4\0",   EVENT_ID_BTN_4,            NULL, D3DXVECTOR3(kInitialVector.x + 0*(kRTouchRectArea.tWidth), kInitialVector.y - 1*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("5\0",   EVENT_ID_BTN_5,            NULL, D3DXVECTOR3(kInitialVector.x + 1*(kRTouchRectArea.tWidth), kInitialVector.y - 1*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("6\0",   EVENT_ID_BTN_6,            NULL, D3DXVECTOR3(kInitialVector.x + 2*(kRTouchRectArea.tWidth), kInitialVector.y - 1*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("0\0",   EVENT_ID_BTN_0,            NULL, D3DXVECTOR3(kInitialVector.x + 3*(kRTouchRectArea.tWidth), kInitialVector.y - 1*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect( NULL,   EVENT_ID_BTN_COM_1_2,      NULL, D3DXVECTOR3(kInitialVector.x + 4*(kRTouchRectArea.tWidth), kInitialVector.y - 1*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_NONE),
    __SVSPTouchRect( NULL,   EVENT_ID_BTN_COM_2_2,      NULL, D3DXVECTOR3(kInitialVector.x + 5*(kRTouchRectArea.tWidth), kInitialVector.y - 1*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_NONE),

    __SVSPTouchRect("1\0",   EVENT_ID_BTN_1,            NULL, D3DXVECTOR3(kInitialVector.x + 0*(kRTouchRectArea.tWidth), kInitialVector.y - 2*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("2\0",   EVENT_ID_BTN_2,            NULL, D3DXVECTOR3(kInitialVector.x + 1*(kRTouchRectArea.tWidth), kInitialVector.y - 2*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("3\0",   EVENT_ID_BTN_3,            NULL, D3DXVECTOR3(kInitialVector.x + 2*(kRTouchRectArea.tWidth), kInitialVector.y - 2*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("CSL\0", EVENT_ID_BTN_CSL,          NULL, D3DXVECTOR3(kInitialVector.x + 3*(kRTouchRectArea.tWidth), kInitialVector.y - 2*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("UHF\0", EVENT_ID_BTN_UHF,          NULL, D3DXVECTOR3(kInitialVector.x + 4*(kRTouchRectArea.tWidth), kInitialVector.y - 2*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_ACTIVE_TOUCH_RECT,  COM::TEXTURE_OBJECT_NONE),
    __SVSPTouchRect("VHF\0", EVENT_ID_BTN_VHF,          NULL, D3DXVECTOR3(kInitialVector.x + 5*(kRTouchRectArea.tWidth), kInitialVector.y - 2*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_NONE),

    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
  },

  /* DISPLAY_STATE_IFF */
  {
    __SVSPTouchRect("NAV\0", EVENT_ID_BTN_NAV,          NULL, D3DXVECTOR3(kMenuVector.x + 0*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("COM\0", EVENT_ID_BTN_COM,          NULL, D3DXVECTOR3(kMenuVector.x + 1*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("IFF\0", EVENT_ID_BTN_IFF,          NULL, D3DXVECTOR3(kMenuVector.x + 2*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_ACTIVE_TOUCH_RECT , COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("TBN\0", EVENT_ID_BTN_TBN,          NULL, D3DXVECTOR3(kMenuVector.x + 3*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("APP\0", EVENT_ID_BTN_APP,          NULL, D3DXVECTOR3(kMenuVector.x + 4*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("LHT\0", EVENT_ID_BTN_LHT,          NULL, D3DXVECTOR3(kMenuVector.x + 5*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("HUD\0", EVENT_ID_BTN_HUD,          NULL, D3DXVECTOR3(kMenuVector.x + 6*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                            COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),

    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
  },

  /* DISPLAY_STATE_TBN */
  {
    __SVSPTouchRect("NAV\0", EVENT_ID_BTN_NAV,          NULL, D3DXVECTOR3(kMenuVector.x + 0*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("COM\0", EVENT_ID_BTN_COM,          NULL, D3DXVECTOR3(kMenuVector.x + 1*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("IFF\0", EVENT_ID_BTN_IFF,          NULL, D3DXVECTOR3(kMenuVector.x + 2*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("TBN\0", EVENT_ID_BTN_TBN,          NULL, D3DXVECTOR3(kMenuVector.x + 3*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_ACTIVE_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("APP\0", EVENT_ID_BTN_APP,          NULL, D3DXVECTOR3(kMenuVector.x + 4*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("LHT\0", EVENT_ID_BTN_LHT,          NULL, D3DXVECTOR3(kMenuVector.x + 5*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("HUD\0", EVENT_ID_BTN_HUD,          NULL, D3DXVECTOR3(kMenuVector.x + 6*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,        COM::TEXTURE_OBJECT_R_BUTTON),

    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
  },

  /* DISPLAY_STATE_APP */
  {
    __SVSPTouchRect("NAV\0", EVENT_ID_BTN_NAV,          NULL, D3DXVECTOR3(kMenuVector.x + 0*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("COM\0", EVENT_ID_BTN_COM,          NULL, D3DXVECTOR3(kMenuVector.x + 1*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("IFF\0", EVENT_ID_BTN_IFF,          NULL, D3DXVECTOR3(kMenuVector.x + 2*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("TBN\0", EVENT_ID_BTN_TBN,          NULL, D3DXVECTOR3(kMenuVector.x + 3*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("APP\0", EVENT_ID_BTN_APP,          NULL, D3DXVECTOR3(kMenuVector.x + 4*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_ACTIVE_TOUCH_RECT , COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("LHT\0", EVENT_ID_BTN_LHT,          NULL, D3DXVECTOR3(kMenuVector.x + 5*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("HUD\0", EVENT_ID_BTN_HUD,          NULL, D3DXVECTOR3(kMenuVector.x + 6*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea,                                           COM::GEOM_OBJECT_TOUCH_RECT,         COM::TEXTURE_OBJECT_R_BUTTON),

    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
  },

  /* DISPLAY_STATE_LHT */
  {
    __SVSPTouchRect("NAV\0", EVENT_ID_BTN_NAV,          NULL, D3DXVECTOR3(kMenuVector.x + 0*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("COM\0", EVENT_ID_BTN_COM,          NULL, D3DXVECTOR3(kMenuVector.x + 1*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("IFF\0", EVENT_ID_BTN_IFF,          NULL, D3DXVECTOR3(kMenuVector.x + 2*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("TBN\0", EVENT_ID_BTN_TBN,          NULL, D3DXVECTOR3(kMenuVector.x + 3*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("APP\0", EVENT_ID_BTN_APP,          NULL, D3DXVECTOR3(kMenuVector.x + 4*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("LHT\0", EVENT_ID_BTN_LHT,          NULL, D3DXVECTOR3(kMenuVector.x + 5*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_ACTIVE_TOUCH_RECT , COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("HUD\0", EVENT_ID_BTN_HUD,          NULL, D3DXVECTOR3(kMenuVector.x + 6*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),

    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
  },

  /* DISPLAY_STATE_HUD */
  {
    __SVSPTouchRect("NAV\0", EVENT_ID_BTN_NAV,          NULL, D3DXVECTOR3(kMenuVector.x + 0*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("COM\0", EVENT_ID_BTN_COM,          NULL, D3DXVECTOR3(kMenuVector.x + 1*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("IFF\0", EVENT_ID_BTN_IFF,          NULL, D3DXVECTOR3(kMenuVector.x + 2*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("TBN\0", EVENT_ID_BTN_TBN,          NULL, D3DXVECTOR3(kMenuVector.x + 3*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("APP\0", EVENT_ID_BTN_APP,          NULL, D3DXVECTOR3(kMenuVector.x + 4*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("LHT\0", EVENT_ID_BTN_LHT,          NULL, D3DXVECTOR3(kMenuVector.x + 5*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("HUD\0", EVENT_ID_BTN_HUD,          NULL, D3DXVECTOR3(kMenuVector.x + 6*(kRTouchRectArea.tWidth), kMenuVector.y, 0), kRTouchRectArea, COM::GEOM_OBJECT_ACTIVE_TOUCH_RECT , COM::TEXTURE_OBJECT_R_BUTTON),

    __SVSPTouchRect("HUD\0", EVENT_ID_SET_HUD_MODE,     NULL, D3DXVECTOR3(kInitialVector.x + 0*(kRTouchRectArea.tWidth), kInitialVector.y - 0*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("HMD\0", EVENT_ID_SET_HMD_MODE,     NULL, D3DXVECTOR3(kInitialVector.x + 1*(kRTouchRectArea.tWidth), kInitialVector.y - 0*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("INC\0", EVENT_ID_INC_SCALE,        NULL, D3DXVECTOR3(kInitialVector.x + 0*(kRTouchRectArea.tWidth), kInitialVector.y - 1*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect("DEC\0", EVENT_ID_DEC_SCALE,        NULL, D3DXVECTOR3(kInitialVector.x + 1*(kRTouchRectArea.tWidth), kInitialVector.y - 1*(kRTouchRectArea.tHeight + 2.0f), 0), kRTouchRectArea, COM::GEOM_OBJECT_TOUCH_RECT, COM::TEXTURE_OBJECT_R_BUTTON),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
    __SVSPTouchRect( NULL,   NULL,                      NULL, D3DXVECTOR3(0, 0, 0), kRTouchRectArea),
  },
};


COM::COM(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : 
    __SVSPModule("COM", pkAvionics, dwThreadSleepTime, false, NULL, NULL, COM::GEOM_OBJECT_NUM_OBJECTS, NULL, COM::TEXTURE_OBJECT_NUM_OBJECTS, COM::g_akTouchMapTable, COM::DISPLAY_STATE_MAX)
{
  //Defines COM Area Window for drawing on Upper HDD display.
  D3DXMATRIXA16 kWindowMatrix = *D3DXMatrixIdentity(&kWindowMatrix);
  m_kWindowRect = __SVSPTouchRect(NULL, NULL, NULL, &kWindowMatrix, AAVArea<FLOAT>(COM::DISPLAY_WINDOW_WIDTH, COM::DISPLAY_WINDOW_HEIGHT));

  FLOAT fBackgroundWidth = 5.0f;

  AAVArea<FLOAT>   kComFreqBtnArea = AAVArea<FLOAT>(64, 32);

  D3DXMatrixTranslation(&m_kLatitudeIndicatorMatrix,  kMenuVector.x + 0*(kRTouchRectArea.tWidth + 5.0f), kMenuVector.y - 1.0f*(kRTouchRectArea.tHeight + 5.0f), 0);
  D3DXMatrixTranslation(&m_kLongitudeIndicatorMatrix, kMenuVector.x + 0*(kRTouchRectArea.tWidth + 5.0f), kMenuVector.y - 1.5f*(kRTouchRectArea.tHeight + 5.0f), 0);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_BORDER_1
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, COM::NUM_FRAME_BORDER_1_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,        COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth - 22,  0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 1] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,        COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth - 22,  0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 2] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth + 150,  COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth,       0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 3] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth + 150,  COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth,       0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 4] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth - 150,  COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth,       0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 5] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth - 150,  COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth,       0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 6] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,        COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth - 22,  0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 7] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,        COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth - 22,  0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 8] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,       -COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth + 100, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 9] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,       -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth + 100, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[10] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,       -COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth + 100, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[11] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,       -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth + 100, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[12] = pVerts[0];
  pVerts[13] = pVerts[1];
  m_ppkGeomObjects[COM::GEOM_OBJECT_FRAME_BORDER_1] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_BORDER_2
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, COM::NUM_FRAME_BORDER_2_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth + 100 - 2.5f*__SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[1] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth + 100 - 2.5f*__SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[2] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth + 100 - 2.5f*__SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[3] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth + 100 - 2.5f*__SVSPModule::fHalfLineWidth, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[4] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth,                                          0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[5] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth,                                          0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[6] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth, -COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth,                                          0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[7] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth, -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth,                                          0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[8] = pVerts[0];
  pVerts[9] = pVerts[1];
  m_ppkGeomObjects[COM::GEOM_OBJECT_FRAME_BORDER_2] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_CORNERS_1
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, COM::NUM_FRAME_CORNERS_1_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100,                             0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 1] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,  -COM::DISPLAY_WINDOW_HEIGHT/2 + 100,                             0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 2] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR);

  pVerts[ 3] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100,                             0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 4] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 5] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,  -COM::DISPLAY_WINDOW_HEIGHT/2 + 100,                             0, 0, 0, __SVSPColor::dwCOLOR);
  m_ppkGeomObjects[COM::GEOM_OBJECT_FRAME_CORNERS_1] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_CORNERS_2
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, COM::NUM_FRAME_CORNERS_2_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 - 2.5f*__SVSPModule::fHalfLineWidth,                             0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 1] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,  -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 - 2.5f*__SVSPModule::fHalfLineWidth,                             0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 2] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 - 2.5f*__SVSPModule::fHalfLineWidth - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR);

  pVerts[ 3] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 - 2.5f*__SVSPModule::fHalfLineWidth,                             0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 4] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 - 2.5f*__SVSPModule::fHalfLineWidth - __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 5] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,  -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 - 2.5f*__SVSPModule::fHalfLineWidth,                             0, 0, 0, __SVSPColor::dwCOLOR);

  pVerts[ 6] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2,                                   0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 7] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,  -COM::DISPLAY_WINDOW_HEIGHT/2,                                   0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[ 8] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth,       0, 0, 0, __SVSPColor::dwCOLOR);

  pVerts[ 9] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2,                                   0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[10] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fCornerWidth,       0, 0, 0, __SVSPColor::dwCOLOR);
  pVerts[11] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,  -COM::DISPLAY_WINDOW_HEIGHT/2,                                   0, 0, 0, __SVSPColor::dwCOLOR);
  m_ppkGeomObjects[COM::GEOM_OBJECT_FRAME_CORNERS_2] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_BORDER_1
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, COM::NUM_ACTIVE_FRAME_BORDER_1_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,        COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth - 22,  0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 1] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,        COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth - 22,  0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 2] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth + 150,  COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth,       0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 3] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth + 150,  COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth,       0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 4] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth - 150,  COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth,       0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 5] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth - 150,  COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth,       0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 6] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,        COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth - 22,  0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 7] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,        COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth - 22,  0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 8] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,       -COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth + 100, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 9] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,       -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth + 100, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[10] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fHalfLineWidth,       -COM::DISPLAY_WINDOW_HEIGHT/2 - __SVSPModule::fHalfLineWidth + 100, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[11] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fHalfLineWidth,       -COM::DISPLAY_WINDOW_HEIGHT/2 + __SVSPModule::fHalfLineWidth + 100, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[12] = pVerts[0];
  pVerts[13] = pVerts[1];
  m_ppkGeomObjects[COM::GEOM_OBJECT_ACTIVE_FRAME_BORDER_1] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_FRAME_CORNERS_1
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLELIST, COM::NUM_ACTIVE_FRAME_CORNERS_1_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[ 0] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 1] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2 - __SVSPModule::fCornerWidth,  -COM::DISPLAY_WINDOW_HEIGHT/2 + 100,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 2] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);

  pVerts[ 3] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 4] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2,                              -COM::DISPLAY_WINDOW_HEIGHT/2 + 100 + __SVSPModule::fCornerWidth, 0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  pVerts[ 5] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2 + __SVSPModule::fCornerWidth,  -COM::DISPLAY_WINDOW_HEIGHT/2 + 100,                             0, 0, 0, __SVSPColor::dwACTIVEWINDOWCOLOR);
  m_ppkGeomObjects[COM::GEOM_OBJECT_ACTIVE_FRAME_CORNERS_1] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_TOUCH_RECT
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, 4);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-kRTouchRectArea.tWidth/2,  kRTouchRectArea.tHeight/2, 0, 0.0f, 0.0f, __SVSPColor::dwGUICOLOR);
  pVerts[1] = __SVSPVertex( kRTouchRectArea.tWidth/2,  kRTouchRectArea.tHeight/2, 0, 1.0f, 0.0f, __SVSPColor::dwGUICOLOR);
  pVerts[2] = __SVSPVertex(-kRTouchRectArea.tWidth/2, -kRTouchRectArea.tHeight/2, 0, 0.0f, 1.0f, __SVSPColor::dwGUICOLOR);
  pVerts[3] = __SVSPVertex( kRTouchRectArea.tWidth/2, -kRTouchRectArea.tHeight/2, 0, 1.0f, 1.0f, __SVSPColor::dwGUICOLOR);
  m_ppkGeomObjects[COM::GEOM_OBJECT_TOUCH_RECT] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ACTIVE_TOUCH_RECT
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, 4);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-kRTouchRectArea.tWidth/2,  kRTouchRectArea.tHeight/2, 0, 0.0f, 0.0f, __SVSPColor::dwACTIVEGUICOLOR);
  pVerts[1] = __SVSPVertex( kRTouchRectArea.tWidth/2,  kRTouchRectArea.tHeight/2, 0, 1.0f, 0.0f, __SVSPColor::dwACTIVEGUICOLOR);
  pVerts[2] = __SVSPVertex(-kRTouchRectArea.tWidth/2, -kRTouchRectArea.tHeight/2, 0, 0.0f, 1.0f, __SVSPColor::dwACTIVEGUICOLOR);
  pVerts[3] = __SVSPVertex( kRTouchRectArea.tWidth/2, -kRTouchRectArea.tHeight/2, 0, 1.0f, 1.0f, __SVSPColor::dwACTIVEGUICOLOR);
  m_ppkGeomObjects[COM::GEOM_OBJECT_ACTIVE_TOUCH_RECT] = pkGeomObject;

  m_ppkTextureObjects[COM::TEXTURE_OBJECT_NONE]           = NULL;
  m_ppkTextureObjects[COM::TEXTURE_OBJECT_R_BUTTON]       = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/COM/RES/TEXTURES/RBUTTON.dds");


  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_BACKGROUND
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_BACKGROUND_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();

  pVerts[0] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2,  COM::DISPLAY_WINDOW_HEIGHT/2, 0.01f, 0, 0, __SVSPColor::dwBACKGROUNDCOLOR);
  pVerts[1] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2,  COM::DISPLAY_WINDOW_HEIGHT/2, 0.01f, 0, 0, __SVSPColor::dwBACKGROUNDCOLOR);
  pVerts[2] = __SVSPVertex(-COM::DISPLAY_WINDOW_WIDTH/2, -COM::DISPLAY_WINDOW_HEIGHT/2, 0.01f, 0, 0, __SVSPColor::dwBACKGROUNDCOLOR);
  pVerts[3] = __SVSPVertex( COM::DISPLAY_WINDOW_WIDTH/2, -COM::DISPLAY_WINDOW_HEIGHT/2, 0.01f, 0, 0, __SVSPColor::dwBACKGROUNDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_CHECK_LIST_INDICATOR
  ///////////////////////////////////////////////////////////////////
  AAVArea<FLOAT> kIndicatorArea = AAVArea<FLOAT>(140, 25);
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_CHECK_LIST_INDICATOR_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-kIndicatorArea.tWidth/2,  kIndicatorArea.tHeight/2, 0, 0.0f, 0.0f, __SVSPColor::dwGUICOLOR);
  pVerts[1] = __SVSPVertex( kIndicatorArea.tWidth/2,  kIndicatorArea.tHeight/2, 0, 1.0f, 0.0f, __SVSPColor::dwGUICOLOR);
  pVerts[2] = __SVSPVertex(-kIndicatorArea.tWidth/2, -kIndicatorArea.tHeight/2, 0, 0.0f, 1.0f, __SVSPColor::dwGUICOLOR);
  pVerts[3] = __SVSPVertex( kIndicatorArea.tWidth/2, -kIndicatorArea.tHeight/2, 0, 1.0f, 1.0f, __SVSPColor::dwGUICOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_CHECK_LIST_INDICATOR] = pkGeomObject;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_CHECK_LIST_WARN_INDICATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_CHECK_LIST_INDICATOR_VERTICES);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-kIndicatorArea.tWidth/2,  kIndicatorArea.tHeight/2, 0, 0.0f, 0.0f, 0x77FF0000);
  pVerts[1] = __SVSPVertex( kIndicatorArea.tWidth/2,  kIndicatorArea.tHeight/2, 0, 1.0f, 0.0f, 0x77FF0000);
  pVerts[2] = __SVSPVertex(-kIndicatorArea.tWidth/2, -kIndicatorArea.tHeight/2, 0, 0.0f, 1.0f, 0x77FF0000);
  pVerts[3] = __SVSPVertex( kIndicatorArea.tWidth/2, -kIndicatorArea.tHeight/2, 0, 1.0f, 1.0f, 0x77FF0000);
  m_ppkGeomObjects[GEOM_OBJECT_CHECK_LIST_WARN_INDICATOR] = pkGeomObject;

  D3DXVECTOR3 kLeftIndicatorVector   = D3DXVECTOR3(-350, 100, 0);
  D3DXVECTOR3 kRightIndicatorVector  = D3DXVECTOR3( 350, 100, 0);

  for(UINT i = 0; i < COM::CHECK_LIST_FAILURE_STACK_SIZE; ++i)
  {
    COM::CHECK_LIST_FAILURE *pCheckListFailure = &m_akCheckListFailureStack[i];

    D3DXVECTOR3 kLeftIndicatorOffsetVector  = kLeftIndicatorVector  - D3DXVECTOR3(0, (FLOAT)i*(kIndicatorArea.tHeight + 3), 0);
    D3DXMatrixTranslation(&pCheckListFailure->kLeftIndicatorMatrix, kLeftIndicatorOffsetVector.x, kLeftIndicatorOffsetVector.y, kLeftIndicatorOffsetVector.z);
    pCheckListFailure->kLeftTextOffsetVector = D3DXVECTOR3(-kIndicatorArea.tWidth/2, 0, 0);

    D3DXVECTOR3 kRightIndicatorOffsetVector = kRightIndicatorVector - D3DXVECTOR3(0, (FLOAT)i*(kIndicatorArea.tHeight + 3), 0);
    D3DXMatrixTranslation(&pCheckListFailure->kRightIndicatorMatrix, kRightIndicatorOffsetVector.x, kRightIndicatorOffsetVector.y, kRightIndicatorOffsetVector.z);
    pCheckListFailure->kRightTextOffsetVector = D3DXVECTOR3(kIndicatorArea.tWidth/2, 0, 0);

    pCheckListFailure->pcFailureName         = NULL;
    pCheckListFailure->bBlinkRequired        = false;
  }

  //Set Stack to Top.
  m_uiCurrentCheckListFailureStackIndex = 0;


  AAVArea<FLOAT>   kGaugeCellArea     = AAVArea<FLOAT>(64, 64);
  m_kThrustPercentValueVector      = D3DXVECTOR3(-COM::DISPLAY_WINDOW_WIDTH/2 +  2*kGaugeCellArea.tWidth, -COM::DISPLAY_WINDOW_HEIGHT/2 + kGaugeCellArea.tHeight/1.2f, 0);
  m_kN1PercentValueVector          = m_kThrustPercentValueVector + D3DXVECTOR3(1.5f*kGaugeCellArea.tWidth,  0,                                 0);
  m_kN2PercentValueVector          = m_kN1PercentValueVector     + D3DXVECTOR3(1.5f*kGaugeCellArea.tWidth,  0,                                 0);
  m_kEGTValueVector                = m_kN2PercentValueVector     + D3DXVECTOR3(1.5f*kGaugeCellArea.tWidth,  0,                                 0);


  //Initialize Process callback table.
  m_apkProcessStateCallback[COM::DISPLAY_STATE_NAV]               = &COM::ProcessStateNAV;
  m_apkProcessStateCallback[COM::DISPLAY_STATE_COM]               = &COM::ProcessStateCOM;
  m_apkProcessStateCallback[COM::DISPLAY_STATE_IFF]               = &COM::ProcessStateIFF;
  m_apkProcessStateCallback[COM::DISPLAY_STATE_TBN]               = &COM::ProcessStateTBN;
  m_apkProcessStateCallback[COM::DISPLAY_STATE_APP]               = &COM::ProcessStateAPP;
  m_apkProcessStateCallback[COM::DISPLAY_STATE_LHT]               = &COM::ProcessStateLHT;
  m_apkProcessStateCallback[COM::DISPLAY_STATE_HUD]               = &COM::ProcessStateHUD;

  //Initialize Render callback table.
  m_apkRenderStateCallback[COM::DISPLAY_STATE_NAV]               = &COM::RenderStateNAV;
  m_apkRenderStateCallback[COM::DISPLAY_STATE_COM]               = &COM::RenderStateCOM;
  m_apkRenderStateCallback[COM::DISPLAY_STATE_IFF]               = &COM::RenderStateIFF;
  m_apkRenderStateCallback[COM::DISPLAY_STATE_TBN]               = &COM::RenderStateTBN;
  m_apkRenderStateCallback[COM::DISPLAY_STATE_APP]               = &COM::RenderStateAPP;
  m_apkRenderStateCallback[COM::DISPLAY_STATE_LHT]               = &COM::RenderStateLHT;
  m_apkRenderStateCallback[COM::DISPLAY_STATE_HUD]               = &COM::RenderStateHUD;

  m_eDisplayState = COM::DISPLAY_STATE_COM;

  m_bCheckListFailureStackMatriciesSet = false;
}

COM::~COM()
{
  __SVSPModule::Stop();
}

VOID COM::ProcessStateNAV()
{
  __SVSPTouchRect *apkCurrentTouchMap = &g_akTouchMapTable[m_eDisplayState][0];
}

VOID COM::ProcessStateCOM()
{
  __SVSPTouchRect *apkCurrentTouchMap = &g_akTouchMapTable[m_eDisplayState][0];
}

VOID COM::ProcessStateIFF()
{
  __SVSPTouchRect *apkCurrentTouchMap = &g_akTouchMapTable[m_eDisplayState][0];
}

VOID COM::ProcessStateTBN()
{
  __SVSPTouchRect *apkCurrentTouchMap = &g_akTouchMapTable[m_eDisplayState][0];
}

VOID COM::ProcessStateAPP()
{
  __SVSPTouchRect *apkCurrentTouchMap = &g_akTouchMapTable[m_eDisplayState][0];
}

VOID COM::ProcessStateLHT()
{
  __SVSPTouchRect *apkCurrentTouchMap = &g_akTouchMapTable[m_eDisplayState][0];
}

VOID COM::ProcessStateHUD()
{
  __SVSPTouchRect *apkCurrentTouchMap = &g_akTouchMapTable[m_eDisplayState][0];
}

VOID COM::RenderStateNAV(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  //RenderNumTable(rkOPMatrix);
  //RenderMenu(rkOPMatrix);
  __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}

VOID COM::RenderStateCOM(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  __SVSPFont* pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  const __SVSPTouchRect *pkTouchMap = &m_pkTouchMapTable[COM::DISPLAY_STATE_COM][0];

  pkFont->SetTransform(pkDevice, &(pkTouchMap[11].kMatrix*rkOPMatrix));
  pkFont->RenderTextExt(pkDevice, &pkAC->m_acComActiveFrequency[0][0], __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);

  pkFont->SetTransform(pkDevice, &(pkTouchMap[12].kMatrix*rkOPMatrix));
  pkFont->RenderTextExt(pkDevice, &pkAC->m_acComStandbyFrequency[0][0], __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);

  pkFont->SetTransform(pkDevice, &(pkTouchMap[17].kMatrix*rkOPMatrix));
  pkFont->RenderTextExt(pkDevice, &pkAC->m_acComActiveFrequency[1][0], __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);

  pkFont->SetTransform(pkDevice, &(pkTouchMap[18].kMatrix*rkOPMatrix));
  pkFont->RenderTextExt(pkDevice, &pkAC->m_acComStandbyFrequency[1][0], __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);

  __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}

VOID COM::RenderStateIFF(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  //RenderNumTable(rkOPMatrix);
  //RenderMenu(rkOPMatrix);
  __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}

VOID COM::RenderStateTBN(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  //RenderNumTable(rkOPMatrix);
  //RenderMenu(rkOPMatrix);
  __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}

VOID COM::RenderStateAPP(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  //RenderNumTable(rkOPMatrix);
  //RenderMenu(rkOPMatrix);
  __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}

VOID COM::RenderStateLHT(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  //RenderNumTable(rkOPMatrix);
  //RenderMenu(rkOPMatrix);
  __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}

VOID COM::RenderStateHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  //RenderNumTable(rkOPMatrix);
  //RenderMenu(rkOPMatrix);
  __SVSPModule::RenderOnHDD(pkDevice, rkOPMatrix);
}

VOID COM::RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);
  ECS *pkECS = static_cast<ECS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS));
  AAV_ASSERT_RETURN_VOID(pkECS  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  LPD3DXGEOMETRYOBJECT pkGeomObjectActive = NULL;
  LPD3DXTEXTUREOBJECT  pkTextureObject = NULL;
  LPD3DXGEOMETRYOBJECT pkGeomObjectFrameBorder1         = NULL;
  LPD3DXGEOMETRYOBJECT pkGeomObjectFrameBorder2         = NULL;
  LPD3DXGEOMETRYOBJECT pkGeomObjectFrameCorners1         = NULL;
  LPD3DXGEOMETRYOBJECT pkGeomObjectFrameCorners2         = NULL;
  LPD3DXGEOMETRYOBJECT pkGeomObjectFrameSeparationLine1 = NULL;
  LPD3DXGEOMETRYOBJECT pkGeomObjectFrameSeparationLine2 = NULL;

  if(m_bActive)
  {
    pkGeomObjectFrameBorder1         = m_ppkGeomObjects[GEOM_OBJECT_ACTIVE_FRAME_BORDER_1];
    pkGeomObjectFrameCorners1        = m_ppkGeomObjects[GEOM_OBJECT_ACTIVE_FRAME_CORNERS_1];
  }
  else
  {
    pkGeomObjectFrameBorder1         = m_ppkGeomObjects[GEOM_OBJECT_FRAME_BORDER_1];
    pkGeomObjectFrameCorners1        = m_ppkGeomObjects[GEOM_OBJECT_FRAME_CORNERS_1];
  }
  pkGeomObjectFrameBorder2          = m_ppkGeomObjects[GEOM_OBJECT_FRAME_BORDER_2];
  pkGeomObjectFrameCorners2         = m_ppkGeomObjects[GEOM_OBJECT_FRAME_CORNERS_2];

  // GEOM_OBJECT_FRAME_BORDER_1
  AAV_ASSERT_RETURN_VOID(pkGeomObjectFrameBorder1  != NULL);
  pkGeomObjectFrameBorder1->SetTransform(pkDevice, &rkOPMatrix);
  pkGeomObjectFrameBorder1->SetTexture(pkDevice, NULL);
  pkGeomObjectFrameBorder1->Render(pkDevice);

  // GEOM_OBJECT_FRAME_CORNERS_1
  AAV_ASSERT_RETURN_VOID(pkGeomObjectFrameCorners1  != NULL);
  pkGeomObjectFrameCorners1->Render(pkDevice);

  // GEOM_OBJECT_FRAME_BORDER_2
  AAV_ASSERT_RETURN_VOID(pkGeomObjectFrameBorder2  != NULL);
  pkGeomObjectFrameBorder2->Render(pkDevice);

  // GEOM_OBJECT_FRAME_CORNERS_2
  AAV_ASSERT_RETURN_VOID(pkGeomObjectFrameCorners2  != NULL);
  pkGeomObjectFrameCorners2->Render(pkDevice);

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_BACKGROUND
  ///////////////////////////////////////////////////////////////////
  //pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND];
  //if(pkGeomObject != NULL)
  //{
  //  pkGeomObject->SetTransform(pkDevice, &rkOPMatrix);
  //  pkGeomObject->SetTexture(pkDevice, NULL);
  //  pkGeomObject->Render(pkDevice);
  //}

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  //pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_FRAME_BORDER];
  //if(pkGeomObject != NULL)
  //{
  //  pkGeomObject->SetTransform(pkDevice, &rkOPMatrix);
  //  pkGeomObject->SetTexture(pkDevice, NULL);
  //  pkGeomObject->Render(pkDevice);
  //}

  if(!m_bCheckListFailureStackMatriciesSet)
  {
    for(UINT i = 0; i < COM::CHECK_LIST_FAILURE_STACK_SIZE; ++i)
    {
      COM::CHECK_LIST_FAILURE *pCheckListFailure = &m_akCheckListFailureStack[i];

      pCheckListFailure->kLeftIndicatorMatrix  = pCheckListFailure->kLeftIndicatorMatrix*rkOPMatrix;
      pCheckListFailure->kRightIndicatorMatrix = pCheckListFailure->kRightIndicatorMatrix*rkOPMatrix;
    }
    m_bCheckListFailureStackMatriciesSet = true;
  }

  LPD3DXGEOMETRYOBJECT pkGeomObjectCheckListWarnIndicator = m_ppkGeomObjects[GEOM_OBJECT_CHECK_LIST_WARN_INDICATOR];
  AAV_ASSERT_RETURN_VOID(pkGeomObjectCheckListWarnIndicator != NULL);
  LPD3DXGEOMETRYOBJECT pkGeomObjectCheckListIndicator     = m_ppkGeomObjects[GEOM_OBJECT_CHECK_LIST_INDICATOR];
  AAV_ASSERT_RETURN_VOID(pkGeomObjectCheckListIndicator  != NULL);

  //Render Check List Failure stack.
  __SVSPFont *pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont != NULL);
  for(UINT i = 0; ((i < m_uiCurrentCheckListFailureStackIndex) && (i < COM::CHECK_LIST_FAILURE_STACK_SIZE)); ++i)
  {
    COM::CHECK_LIST_FAILURE *pkCheckListFailure = &m_akCheckListFailureStack[i];
    if(pkCheckListFailure->pcFailureName != NULL)
    {
      if(pkCheckListFailure->bBlinkRequired)
      {
        if(m_pkAvionics->g_bWarnClockEdgeUP)
        {
          pkGeomObject = pkGeomObjectCheckListWarnIndicator;

          pkGeomObject->SetTransform(pkDevice, &pkCheckListFailure->kLeftIndicatorMatrix);
          pkGeomObject->SetTexture(pkDevice, NULL);
          pkGeomObject->Render(pkDevice);
          pkFont->RenderTextExt(pkDevice, pkCheckListFailure->pcFailureName, __SVSPFont::CENTERED_X | __SVSPFont::CENTERED_Y, 0.8f, __SVSPColor::dwACTIVEGUICOLOR);

          pkGeomObject->SetTransform(pkDevice, &pkCheckListFailure->kRightIndicatorMatrix);
          pkGeomObject->SetTexture(pkDevice, NULL);
          pkGeomObject->Render(pkDevice);
          pkFont->RenderTextExt(pkDevice, pkCheckListFailure->pcFailureName, __SVSPFont::CENTERED_X | __SVSPFont::CENTERED_Y, 0.8f, __SVSPColor::dwACTIVEGUICOLOR);
        }
      }
      else
      {
          pkGeomObject = pkGeomObjectCheckListIndicator;

          pkGeomObject->SetTransform(pkDevice, &(pkCheckListFailure->kLeftIndicatorMatrix));
          pkGeomObject->SetTexture(pkDevice, NULL);
          pkGeomObject->Render(pkDevice);
          pkFont->RenderTextExt(pkDevice, pkCheckListFailure->pcFailureName, __SVSPFont::CENTERED_X | __SVSPFont::CENTERED_Y, 0.8f, __SVSPColor::dwACTIVEGUICOLOR);

          pkGeomObject->SetTransform(pkDevice, &(pkCheckListFailure->kRightIndicatorMatrix));
          pkGeomObject->SetTexture(pkDevice, NULL);
          pkGeomObject->Render(pkDevice);
          pkFont->RenderTextExt(pkDevice, pkCheckListFailure->pcFailureName, __SVSPFont::CENTERED_X | __SVSPFont::CENTERED_Y, 0.8f, __SVSPColor::dwACTIVEGUICOLOR);
      }
    }
  }

  //Unnecessary.
  //for(UINT i = m_uiCurrentCheckListFailureStackIndex; i < COM::CHECK_LIST_FAILURE_STACK_SIZE; ++i)
  //{
  //  COM::CHECK_LIST_FAILURE *pkCheckListFailure = &m_akCheckListFailureStack[i];
  //  pkGeomObject = pkGeomObjectCheckListIndicator;
  //  pkGeomObject->SetTransform(pkDevice, &(pkCheckListFailure->kLeftIndicatorMatrix));
  //  pkGeomObject->SetTexture(pkDevice, NULL);
  //  pkGeomObject->Render(pkDevice);
  //  pkGeomObject->SetTransform(pkDevice, &(pkCheckListFailure->kRightIndicatorMatrix));
  //  pkGeomObject->SetTexture(pkDevice, NULL);
  //  pkGeomObject->Render(pkDevice);
  //}

  // ENGINE DATA
  pkECS->RenderGaugeElement(pkDevice, ECS::GAUGE_ELEMENT_THRUST,  rkOPMatrix, &m_kThrustPercentValueVector);
  pkECS->RenderGaugeElement(pkDevice, ECS::GAUGE_ELEMENT_N1_RPM,  rkOPMatrix, &m_kN1PercentValueVector);
  pkECS->RenderGaugeElement(pkDevice, ECS::GAUGE_ELEMENT_N2_RPM,  rkOPMatrix, &m_kN2PercentValueVector);
  pkECS->RenderGaugeElement(pkDevice, ECS::GAUGE_ELEMENT_EGT_RPM, rkOPMatrix, &m_kEGTValueVector);

  AAV_ASSERT(m_eDisplayState < COM::DISPLAY_STATE_MAX);
  VOID (__thiscall COM::*__RenderStateCallback)(LPD3DXDEVICE pkDevice, D3DXMATRIX &) = m_apkRenderStateCallback[m_eDisplayState];
  AAV_ASSERT(__RenderStateCallback != NULL);
  (this->*__RenderStateCallback)(pkDevice, rkOPMatrix);
}

BOOL COM::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  switch(dwEvent)
  {
  case COM::EVENT_ID_BTN_7:
    break;

  case COM::EVENT_ID_BTN_8:
    break;

  case COM::EVENT_ID_BTN_NAV:
    m_eDisplayState = DISPLAY_STATE_NAV;
    break;

  case COM::EVENT_ID_BTN_COM:
    m_eDisplayState = DISPLAY_STATE_COM;
    break;

  case COM::EVENT_ID_BTN_IFF:
    m_eDisplayState = DISPLAY_STATE_IFF;
    break;

  case COM::EVENT_ID_BTN_TBN:
    m_eDisplayState = DISPLAY_STATE_TBN;
    break;

  case COM::EVENT_ID_BTN_APP:
    m_eDisplayState = DISPLAY_STATE_APP;
    break;

  case COM::EVENT_ID_BTN_LHT:
    m_eDisplayState = DISPLAY_STATE_LHT;
    break;

  case COM::EVENT_ID_BTN_HUD:
    {
      HUD *pkHUD = static_cast<HUD *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HUD));
      AAV_ASSERT_RETURN(pkHUD != NULL, S_FALSE);
      //COM::g_akTouchMapTable[COM::DISPLAY_STATE_HUD][7].bActive = (BOOL)(pkHUD->GetMode() == HUD::DISPLAY_MODE_HUD);
      //COM::g_akTouchMapTable[COM::DISPLAY_STATE_HUD][8].bActive = (BOOL)(pkHUD->GetMode() == HUD::DISPLAY_MODE_HMD);
    }
    m_eDisplayState = DISPLAY_STATE_HUD;
    break;

  case COM::EVENT_ID_SET_HUD_MODE:
    {
      HUD *pkHUD = static_cast<HUD *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HUD));
      AAV_ASSERT_RETURN(pkHUD != NULL, S_FALSE);
      pkHUD->SetMode(HUD::DISPLAY_MODE_HUD);
      //COM::g_akTouchMapTable[COM::DISPLAY_STATE_HUD][7].bActive = true;
      //COM::g_akTouchMapTable[COM::DISPLAY_STATE_HUD][8].bActive = false;
    }
    break;

  case COM::EVENT_ID_SET_HMD_MODE:
    {
      HUD *pkHUD = static_cast<HUD *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HUD));
      AAV_ASSERT_RETURN(pkHUD != NULL, S_FALSE);
      pkHUD->SetMode(HUD::DISPLAY_MODE_HMD);
      //COM::g_akTouchMapTable[COM::DISPLAY_STATE_HUD][7].bActive = false;
      //COM::g_akTouchMapTable[COM::DISPLAY_STATE_HUD][8].bActive = true;
    }
    break;

  case COM::EVENT_ID_INC_SCALE:
    {
      HUD *pkHUD = static_cast<HUD *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HUD));
      AAV_ASSERT_RETURN(pkHUD != NULL, S_FALSE);
      pkHUD->IncreaseScale();
    }
    break;

  case COM::EVENT_ID_DEC_SCALE:
    {
      HUD *pkHUD = static_cast<HUD *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_HUD));
      AAV_ASSERT_RETURN(pkHUD != NULL, S_FALSE);
      pkHUD->DecreaseScale();
    }
    break;
  }
  return false;
}

VOID COM::Start()
{
  __SVSPModule::Start();

  //Clear Check List Failure stack.
  for(UINT i = 1; i < COM::CHECK_LIST_FAILURE_STACK_SIZE; ++i)
  {
    COM::CHECK_LIST_FAILURE *pCheckListFailure = &m_akCheckListFailureStack[i];
    pCheckListFailure->pcFailureName         = NULL;
    pCheckListFailure->bBlinkRequired        = false;
  }

  //Set Stack to Top.
  m_uiCurrentCheckListFailureStackIndex = 0;
}

VOID COM::Process()
{
  VWS *pkVWS = (VWS *)m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_VWS);

  //Set Check List Failure Stack index to Top.
  m_uiCurrentCheckListFailureStackIndex = 0;

  //Process VWS Check List Assertion Table.
  for(UINT i = 0; ((i < VWS::MAX_NOF_ASSERTIONS) && (m_uiCurrentCheckListFailureStackIndex < COM::CHECK_LIST_FAILURE_STACK_SIZE)); ++i)
  {
    //Stack is not full, proceed with the check list.
    if((i < VWS::ASSERT_ID_FUEL_1500) || (i > VWS::ASSERT_ID_FUEL_500))
    {
      if(VWS::g_akCheckListAssertionTable[i].bFailed)
      {
        //Push into m_akCheckListFailureStack.
        m_akCheckListFailureStack[m_uiCurrentCheckListFailureStackIndex].pcFailureName  = &(VWS::g_akCheckListAssertionTable[i].acItemName)[0];
        m_akCheckListFailureStack[m_uiCurrentCheckListFailureStackIndex].bBlinkRequired =   VWS::g_akCheckListAssertionTable[i].bBlinkRequired;
        ++m_uiCurrentCheckListFailureStackIndex;
      }
    }
  }
  //Check list OK.
  if(m_uiCurrentCheckListFailureStackIndex == 0)
  {
    m_akCheckListFailureStack[0].pcFailureName = "CL OK";
    m_akCheckListFailureStack[0].bBlinkRequired = false;
    ++m_uiCurrentCheckListFailureStackIndex;
  }

  AAV_ASSERT(m_eDisplayState < COM::DISPLAY_STATE_MAX);
  VOID (__thiscall COM::*__ProcessStateCallback)(VOID) = m_apkProcessStateCallback[m_eDisplayState];
  AAV_ASSERT(__ProcessStateCallback != NULL);
  (this->*__ProcessStateCallback)();
}
