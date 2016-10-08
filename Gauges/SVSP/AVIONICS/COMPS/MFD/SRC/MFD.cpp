#include "__SVSPAvionics.h"
#include "MFD.h"

MFD::MFD(__SVSPAvionics *__pkAvionics, DWORD dwThreadSleepTime) : 
HDD("MFD", __pkAvionics, dwThreadSleepTime, NULL, MFD::GEOM_OBJECT_NUM_OBJECTS, NULL, MFD::TEXTURE_OBJECT_NUM_OBJECTS)
{
  ///////////////////////////////////////////////////////////////////
  // SETUP DISPLAY ELEMENTS
  ///////////////////////////////////////////////////////////////////
  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINESTRIP, NUM_FRAME_BORDER_VERTICES);
  //m_ppkGeomObjects[GEOM_OBJECT_BACKGROUND]->SetMaterial(0.37f, 0.37f, 0.87f, 1.0f);
  pkGeomObject->SetMaterial(0.1f, 0.1f, 0.1f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + 100, 0, 0, 0);
  pVerts[1] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH,  __SVSPModule::DISPLAY_WINDOW_HEIGHT/2 + 100, 0, 0, 0);
  pVerts[2] = __SVSPVertex( __SVSPModule::DISPLAY_WINDOW_WIDTH, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,       0, 0, 0);
  pVerts[3] = __SVSPVertex(-__SVSPModule::DISPLAY_WINDOW_WIDTH, -__SVSPModule::DISPLAY_WINDOW_HEIGHT/2,       0, 0, 0);
  pVerts[4] = pVerts[0];
  m_ppkGeomObjects[GEOM_OBJECT_FRAME_BORDER] = pkGeomObject;


  ///////////////////////////////////////////////////////////////////
  // SETUP DISPLAY MODULES
  ///////////////////////////////////////////////////////////////////
  FLOAT fLeftFrameOffset  = -1.5f*__SVSPModule::DISPLAY_WINDOW_WIDTH - 5;
  FLOAT fRightFrameOffset =  0.5f*__SVSPModule::DISPLAY_WINDOW_WIDTH + 5;

  SetDisplayModule(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_COM),   AAVVector3<FLOAT>(0,                                                           870, 1), 0);
  SetDisplayModule(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FCR),   AAVVector3<FLOAT>(fLeftFrameOffset   + 0.5f*__SVSPModule::DISPLAY_WINDOW_WIDTH, 290, 1), 1);
  SetDisplayModule(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_ECS),   AAVVector3<FLOAT>(fRightFrameOffset  + 0.0f*__SVSPModule::DISPLAY_WINDOW_WIDTH, 290, 1), 2);
  SetDisplayModule(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS),   AAVVector3<FLOAT>(fRightFrameOffset  + 1.0f*__SVSPModule::DISPLAY_WINDOW_WIDTH, 290, 1), 3);

  D3DXMatrixTranslation(&m_kLeftFrameMatrix,  -__SVSPModule::DISPLAY_WINDOW_WIDTH - 5, 290, 0);
  D3DXMatrixTranslation(&m_kRightFrameMatrix,  __SVSPModule::DISPLAY_WINDOW_WIDTH + 5, 290, 0);
}

MFD::~MFD()
{
  __SVSPModule::Stop();
}

VOID MFD::Render(LPD3DXDEVICE pkDevice)
{
  if(m_uiFrameCount == 0)
  {
    m_uiFrameCount = HDD::RENDERING_FRAME_FREQ;
  }
  else
  {
    --m_uiFrameCount;
    return;
  }

  __SVSPFont* pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  LPD3DXTEXTUREOBJECT  pkTextureObject = NULL;

  AAV_ASSERT_RETURN_VOID(m_pkTextureRenderer != NULL);

  m_pkTextureRenderer->Begin(pkDevice);

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_FRAME_BORDER
  ///////////////////////////////////////////////////////////////////
  pkGeomObject    = m_ppkGeomObjects[GEOM_OBJECT_FRAME_BORDER];
  AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);
  pkGeomObject->SetTexture(pkDevice, NULL);
  pkGeomObject->SetTransform(pkDevice, &m_kLeftFrameMatrix);
  pkGeomObject->Render(pkDevice);
  pkGeomObject->SetTransform(pkDevice, &m_kRightFrameMatrix);
  pkGeomObject->Render(pkDevice);

  for(UINT i = 0; i < m_apkDisplayModule.size(); ++i)
  {
    __SVSPModule *pkDisplayModule = m_apkDisplayModule[i];
    if(pkDisplayModule != NULL)
    {
      pkDisplayModule->RenderOnHDD(pkDevice, pkDisplayModule->m_kOnHddOffsetMatrix);
    }
  }

  m_pkTextureRenderer->End(pkDevice);
}

BOOL MFD::MouseHandler(PPIXPOINT pMousePoint, FLAGS32 mouse_flags) 
{
  //for(INT i = 0; i < sizeof(HDD::g_akTouchMap)/sizeof(HDD::TOUCH_MAP_ELEMENT); ++i)
  //{
  //  HDD::TOUCH_MAP_RECT *pRect = &HDD::g_akTouchMap[i].kRect;
  //  if((pMousePoint->x > (INT)pRect->x) && (pMousePoint->x < (INT)(pRect->x + pRect->size_x)) && 
  //    (pMousePoint->y > (INT)pRect->y) && (pMousePoint->y < (INT)(pRect->y + pRect->size_y)))
  //  {
  //    //trigger_key_event(g_akTouchMap[i].kEvent, 0);
  //    /////////////////////////////////////////////////////////////////////////
  //    // DO NOT TRIGGER ANY GLOBAL EVENTS, CALL EVENTHANDLER DIRECTLY INSTEAD
  //    /////////////////////////////////////////////////////////////////////////
  //    //EventHandler(g_akTouchMap[i].kEvent, 0, this);
  //  }
  //}

  switch(mouse_flags)
  {
  case MOUSE_LEFTSINGLE:
    for(UINT i = 0; i < m_apkDisplayModule.size(); ++i)
    {
      __SVSPModule *pkDisplayWindow = m_apkDisplayModule[i];
      if(pkDisplayWindow != NULL)
      {
        pkDisplayWindow->m_bActive = false;
      }
    }

    for(UINT i = 0; i < m_apkDisplayModule.size(); ++i)
    {
      __SVSPModule *pkModule = m_apkDisplayModule[i];
      if(pkModule != NULL)
      {
        // Check whether the mouse point is within the window area.
        D3DXMATRIX *pkDisplayOffsetMatrix = &pkModule->m_kOnHddOffsetMatrix;
        INT iWindowHalfWidth  = ((INT)(pkModule->m_kWindowRect.kArea.tWidth) >> 1);
        INT iWindowHalfHeight = ((INT)(pkModule->m_kWindowRect.kArea.tHeight) >> 1);

        if((pMousePoint->x > ((INT)pkDisplayOffsetMatrix->_41 - iWindowHalfWidth))  && (pMousePoint->x < ((INT)pkDisplayOffsetMatrix->_41 + iWindowHalfWidth)) && 
           (pMousePoint->y > ((INT)pkDisplayOffsetMatrix->_42 - iWindowHalfHeight)) && (pMousePoint->y < ((INT)pkDisplayOffsetMatrix->_42 + iWindowHalfHeight)))
        {
          // Forward the event to the window
          PIXPOINT kWindowRelativeMousePoint;
          kWindowRelativeMousePoint.x = pMousePoint->x - (INT)pkDisplayOffsetMatrix->_41;
          kWindowRelativeMousePoint.y = pMousePoint->y - (INT)pkDisplayOffsetMatrix->_42;

          pkModule->m_bActive = true;

          pkModule->MouseHandler(*pkDisplayOffsetMatrix, &kWindowRelativeMousePoint, mouse_flags);
          break;
        }
      }
    }
    break;

  //case MOUSE_LEFTSINGLE:
  //  m_bUpdateWindowOffsetMatrix = false;
  //  for(INT i = (m_apkDisplayModule.size() - 1); i >= 0; i--)
  //  {
  //    __SVSPModule *pkWindow = m_apkDisplayModule[i];
  //    if(pkWindow != NULL)
  //    {
  //      D3DXMATRIX *pkWindowOffsetMatrix = &pkWindow->m_kWindowRect.kMatrix;
  //      INT iWindowHalfWidth  = ((INT)(pkWindow->m_kWindowRect.kArea.tWidth) >> 1);
  //      INT iWindowHalfHeight = ((INT)(pkWindow->m_kWindowRect.kArea.tHeight) >> 1);

  //      if((pMousePoint->x > ((INT)pkWindowOffsetMatrix->_41 - iWindowHalfWidth))  && (pMousePoint->x < ((INT)pkWindowOffsetMatrix->_41 + iWindowHalfWidth)) && 
  //         (pMousePoint->y > ((INT)pkWindowOffsetMatrix->_42 - iWindowHalfHeight)) && (pMousePoint->y < ((INT)pkWindowOffsetMatrix->_42 + iWindowHalfHeight)))
  //      {
  //        if(i == (m_apkDisplayModule.size() - 1))
  //        {
  //          // Forward the event to the top window
  //          PIXPOINT kWindowRelativeMousePoint;
  //          kWindowRelativeMousePoint.x = pMousePoint->x - (INT)pkWindowOffsetMatrix->_41;
  //          kWindowRelativeMousePoint.y = pMousePoint->y - (INT)pkWindowOffsetMatrix->_42;
  //          if(!pkWindow->MouseHandler(&kWindowRelativeMousePoint, mouse_flags))
  //          {
  //            // Allow window dragging when no touch zones hit
  //            m_pkActiveWindow = pkWindow;
  //          }
  //          break;
  //        }
  //        else
  //        {
  //          for(INT k = i; k < m_apkDisplayModule.size() - 1; ++k)
  //          {
  //            if((k + 1) < m_apkDisplayModule.size())
  //            {
  //              m_apkDisplayModule[k] = m_apkDisplayModule[k + 1];
  //            }
  //          }
  //          m_apkDisplayModule[m_apkDisplayModule.size() - 1] = m_pkActiveWindow = pkWindow;
  //          break;
  //        }
  //      }
  //    }
  //  }
  //  break;

  //case MOUSE_LEFTDRAG:
  //  if(m_pkActiveWindow != NULL)
  //  {
  //    if(m_bUpdateWindowOffsetMatrix)
  //    {
  //      D3DXMATRIX  *pkActiveWindowOffsetMatrix  = &m_pkActiveWindow->m_kWindowRect.kMatrix;
  //      INT iActiveWindowHalfWidth               = ((INT)(m_pkActiveWindow->m_kWindowRect.kArea.tWidth) >> 1);
  //      INT iActiveWindowHalfHeight              = ((INT)(m_pkActiveWindow->m_kWindowRect.kArea.tHeight) >> 1);
  //      INT iHDDWindowHalfWidth                  = ((INT)(m_kWindowRect.kArea.tWidth) >> 1);
  //      INT iHDDWindowHalfHeight                 = ((INT)(m_kWindowRect.kArea.tHeight) >> 1);
  //      D3DXVECTOR2 kMouseDragVector             = D3DXVECTOR2((FLOAT)pMousePoint->x, (FLOAT)pMousePoint->y) - m_pkActiveWindow->m_kMouseVector;
  //      D3DXVECTOR2 kNewActiveWindowOffsetVector = D3DXVECTOR2(pkActiveWindowOffsetMatrix->_41, pkActiveWindowOffsetMatrix->_42) + kMouseDragVector;

  //      if(((kNewActiveWindowOffsetVector.x - iActiveWindowHalfWidth) > -iHDDWindowHalfWidth) && ((kNewActiveWindowOffsetVector.x + iActiveWindowHalfWidth) < iHDDWindowHalfWidth))
  //      {
  //        pkActiveWindowOffsetMatrix->_41 = kNewActiveWindowOffsetVector.x;
  //      }
  //      if(((kNewActiveWindowOffsetVector.y - iActiveWindowHalfHeight) > -iHDDWindowHalfHeight) && ((kNewActiveWindowOffsetVector.y + iActiveWindowHalfHeight) < iHDDWindowHalfHeight))
  //      {
  //        pkActiveWindowOffsetMatrix->_42 = kNewActiveWindowOffsetVector.y;
  //      }
  //      m_pkActiveWindow->m_kMouseVector = D3DXVECTOR2((FLOAT)pMousePoint->x, (FLOAT)pMousePoint->y);
  //    }
  //    else
  //    {
  //      m_pkActiveWindow->m_kMouseVector = D3DXVECTOR2((FLOAT)pMousePoint->x, (FLOAT)pMousePoint->y);
  //      m_bUpdateWindowOffsetMatrix = true;
  //    }
  //  }
  //  break;

  case MOUSE_LEFTRELEASE:
    //m_pkActiveWindow = NULL;
    break;
  }

  return true;
}



BOOL MFD::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  AAV_ASSERT_RETURN(dwEventData < __SVSPAvionics::AVIONICS_MODULE_COUNT, false);

  return true;
}
