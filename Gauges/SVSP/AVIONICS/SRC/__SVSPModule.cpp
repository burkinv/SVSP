#include "__SVSPModule.h"
#include "__SVSPAvionics.h"

const D3DXVECTOR3              __SVSPModule::UNIT_VECTOR            = D3DXVECTOR3(0, 0, 1.0f);
const FLOAT                    __SVSPModule::DISPLAY_WINDOW_WIDTH   = 505;
const FLOAT                    __SVSPModule::DISPLAY_WINDOW_HEIGHT  = 640;
const AAVArea<FLOAT>           __SVSPModule::kTouchRectArea         = AAVArea<FLOAT>(80, 80);
const AAVVector3<FLOAT>        __SVSPModule::kExtraOffsetVector     = AAVVector3<FLOAT>(25, 25, 0);
const FLOAT                    __SVSPModule::fHalfLineWidth         = 1.5f;
const FLOAT                    __SVSPModule::fCornerWidth           = 15;


__SVSPModule::__SVSPModule(CHAR                   *pcName, 
                         class __SVSPAvionics    *pkAvionics, 
                         DWORD                   dwThreadSleepTime, 
                         BOOL                    bSimConnectRequired, 
                         DispatchProc            pDispatchProcessCallback,
                         LPPD3DXGEOMETRYOBJECT   ppkGeomObjectsList, 
                         UINT                    uiNumGeomObjects, 
                         LPPD3DXTEXTUREOBJECT    ppkTextureObjectsList, 
                         UINT                    uiNumTextureObjects, 
                         __SVSPTouchRect            (*pkTouchMapTable)[__SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE], 
                         UINT                    uiMaxNumDisplayStates) :
  m_bValid                    (false),
  m_acName                    (__SVSPString<CHAR>(pcName)),
  m_pkAvionics                (pkAvionics),
  m_dwThreadSleepTime         (dwThreadSleepTime),
  m_bSimConnectRequired       (bSimConnectRequired),
  m_pDispatchProcessCallback  (pDispatchProcessCallback),
  m_ppkGeomObjects            (ppkGeomObjectsList),
  m_uiNumGeomObjects          (uiNumGeomObjects),
  m_ppkTextureObjects         (ppkTextureObjectsList),
  m_uiNumTextureObjects       (uiNumTextureObjects),
  m_pkTouchMapTable           (pkTouchMapTable),
  m_uiMaxNumDisplayStates     (uiMaxNumDisplayStates),
  m_dCurrentTime              (0),
  m_dDeltaTime                (0),
  m_hResult                   (S_FALSE),
  m_bAbort                    (false),
  m_hSimConnect               (NULL),
  m_kOPMatrix                 (),
  m_hThreadHandle             (0),
  m_hThreadStartEvent         (0),
  m_hThreadExitEvent          (0),
  m_dwProcessUpdateMask       (0),
  m_kMouseVector              (),
  m_kWindowRect               (),
  m_eDisplayState             (DISPLAY_STATE_ROOT_MENU),
  m_bActive                   (false),
  m_kVector                   (),
  m_kLocalOffsetVector        ()
{
  if((m_ppkGeomObjects == NULL) && (m_uiNumGeomObjects != 0))
  {
    m_ppkGeomObjects = new LPD3DXGEOMETRYOBJECT[uiNumGeomObjects];
    memset(m_ppkGeomObjects, 0, uiNumGeomObjects*sizeof(LPD3DXGEOMETRYOBJECT));
  }

  if((m_ppkTextureObjects == NULL) && (m_uiNumTextureObjects != 0))
  {
    m_ppkTextureObjects = new LPD3DXTEXTUREOBJECT[m_uiNumTextureObjects];
    memset(m_ppkTextureObjects, 0, m_uiNumTextureObjects*sizeof(LPD3DXTEXTURE));
  }

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;


  //Defines MFD Area Window for drawing on HDD display.
  D3DXMATRIXA16 kWindowMatrix = *D3DXMatrixIdentity(&kWindowMatrix);
  m_kWindowRect = __SVSPTouchRect(NULL, NULL, NULL, &kWindowMatrix, AAVArea<FLOAT>(__SVSPModule::DISPLAY_WINDOW_WIDTH, __SVSPModule::DISPLAY_WINDOW_HEIGHT));

  //Open SimConnect
  if(m_bSimConnectRequired)
  {
    m_hResult = SimConnect_Open(&m_hSimConnect, m_acName.getCStr(), NULL, 0, 0, 0);
    AAV_ASSERT(SUCCEEDED(m_hResult));

    if(m_pDispatchProcessCallback != NULL)
    {
      m_bAbort               = false;
      m_hResult = SimConnect_CallDispatch(m_hSimConnect, m_pDispatchProcessCallback, this);
      AAV_ASSERT(SUCCEEDED(m_hResult));
    }
    __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "CONNECTED TO FS");
  }
}

__SVSPModule::~__SVSPModule()
{
  //Stop thread
  Stop();

  //Close SimConnect
  if(m_hSimConnect != NULL)
  {
    m_hResult = SimConnect_Close(m_hSimConnect);
    AAV_ASSERT(SUCCEEDED(m_hResult));
    m_hSimConnect = NULL;
    __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "DISCONNECTED FROM FS");
  }

  //Delete graphics objects
  if(m_ppkGeomObjects != NULL)
  {
    for(UINT i = 0; i < m_uiNumGeomObjects; ++i)
    {
      if(m_ppkGeomObjects[i] != NULL)
      {
        delete m_ppkGeomObjects[i];
        m_ppkGeomObjects[i] = NULL;
      }
    }
    delete[] m_ppkGeomObjects;
  }

  //Delete Texture objects
  if(m_ppkTextureObjects != NULL)
  {
    for(UINT i = 0; i < m_uiNumTextureObjects; ++i)
    {
      if(m_ppkTextureObjects[i] != NULL)
      {
        delete m_ppkTextureObjects[i];
        m_ppkTextureObjects[i] = NULL;
      }
    }
    delete[] m_ppkTextureObjects;
  }


  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "DELETED");
}


VOID __SVSPModule::Release()
{
  //__SVSPDEBUG::OUTPUT("__SVSPRenderer::Release:ENTRY");
  if(m_ppkGeomObjects != NULL)
  {
    for(UINT i = 0; i < m_uiNumGeomObjects; ++i)
    {
      D3DXGEOMETRYOBJECT* pkGeomObject = m_ppkGeomObjects[i];
      if(pkGeomObject != NULL)
      {
        pkGeomObject->Release();
      }
    }
  }
  if(m_ppkTextureObjects != NULL)
  {
    for(UINT i = 0; i < m_uiNumTextureObjects; ++i)
    {
      D3DXTEXTUREOBJECT* pkTextureObject = m_ppkTextureObjects[i];
      if(pkTextureObject != NULL)
      {
        pkTextureObject->Release();
      }
    }
  }

  //__SVSPDEBUG::OUTPUT("__SVSPRenderer::Release:EXIT");
}

HRESULT __SVSPModule::Reset(LPD3DXDEVICE pkDevice)
{
  //__SVSPDEBUG::OUTPUT("__SVSPRenderer::Reset:ENTRY");
  if(m_ppkGeomObjects != NULL)
  {
    for(UINT i = 0; i < m_uiNumGeomObjects; ++i)
    {
      D3DXGEOMETRYOBJECT* pkGeomObject = m_ppkGeomObjects[i];
      if(pkGeomObject != NULL)
      {
        pkGeomObject->Reset(pkDevice);
      }
    }
  }
  if(m_ppkTextureObjects != NULL)
  {
    for(UINT i = 0; i < m_uiNumTextureObjects; ++i)
    {
      D3DXTEXTUREOBJECT* pkTextureObject = m_ppkTextureObjects[i];
      if(pkTextureObject != NULL)
      {
        pkTextureObject->Reset(pkDevice);
      }
    }
  }

  //__SVSPDEBUG::OUTPUT("__SVSPRenderer::Reset:EXIT");
    return S_OK;
}

HRESULT __SVSPModule::Present             (LPD3DXDEVICE pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
  return S_OK;
}
HRESULT __SVSPModule::EndScene            (LPD3DXDEVICE pkDevice)
{
  return S_OK;
}
HRESULT __SVSPModule::CreateTexture       (LPD3DXDEVICE pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
  return S_OK;
}
HRESULT __SVSPModule::SetTexture          (LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
  return S_OK;
}


VOID __SVSPModule::RenderOnHUD(LPD3DXDEVICE pkDevice)
{
}

VOID __SVSPModule::RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  AAV_ASSERT_RETURN_VOID(m_pkTouchMapTable != NULL);
  AAV_ASSERT_RETURN_VOID(m_uiMaxNumDisplayStates != 0);
  AAV_ASSERT_RETURN_VOID(m_eDisplayState < m_uiMaxNumDisplayStates);

  const __SVSPTouchRect *pkTouchMap = &m_pkTouchMapTable[m_eDisplayState][0];

  __SVSPFont* pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  __SVSPGeometryObject_D3D9<__SVSPVertex>::GlobalSetTransform(pkDevice, &rkOPMatrix);

  for(UINT i = 0; i < __SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE; ++i)
  {
    const __SVSPTouchRect *pkTouchRect = &pkTouchMap[i];
    if(pkTouchRect->eEvent != NULL)
    {
      AAV_ASSERT_RETURN_VOID(pkTouchRect->uiGeomObjectNum    < m_uiNumGeomObjects);
      AAV_ASSERT_RETURN_VOID(pkTouchRect->uiTextureObjectNum < m_uiNumTextureObjects);

      LPD3DXGEOMETRYOBJECT pkGeomObject    = m_ppkGeomObjects[pkTouchRect->uiGeomObjectNum];
      LPD3DXTEXTUREOBJECT  pkTextureObject = m_ppkTextureObjects[pkTouchRect->uiTextureObjectNum];

      if(pkGeomObject != NULL)
      {
        pkGeomObject->SetTransform(pkDevice, &(pkTouchRect->kMatrix*rkOPMatrix));
      }
      if(pkTextureObject != NULL)
      {
        pkGeomObject->SetTexture(pkDevice, pkTextureObject);
        pkGeomObject->Render(pkDevice);
      }
      if(pkTouchRect->acName.getLength() != 0)
      {
        pkFont->RenderTextExt(pkDevice, pkTouchRect->acName.getCStr(), __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 0.667f, __SVSPColor::dwGUICOLOR);
      }
    }
  }
}


BOOL __SVSPModule::MouseHandler(D3DXMATRIX &rkOPMatrix, PPIXPOINT pMousePoint, FLAGS32 mouse_flags) 
{
  AAV_ASSERT_RETURN(m_pkTouchMapTable != NULL,                 FALSE);
  AAV_ASSERT_RETURN(m_uiMaxNumDisplayStates != 0,              FALSE);
  AAV_ASSERT_RETURN(m_eDisplayState < m_uiMaxNumDisplayStates, FALSE);

  const __SVSPTouchRect *pkTouchMap = &m_pkTouchMapTable[m_eDisplayState][0];

  //Rescale mouse point coordinates to match the windows scaling.
  pMousePoint->x = (PIXEL)(pMousePoint->x/rkOPMatrix._11);
  pMousePoint->y = (PIXEL)(pMousePoint->y/rkOPMatrix._22);

  // GO THROUGH TOUCH MAP AND TRIGGER CORRESPONDING EVENT
  for(UINT i = 0; i < __SVSPModule::MAX_NUM_TOUCH_RECTS_PER_STATE; ++i)
  {
    const __SVSPTouchRect *pkTouchRect = &pkTouchMap[i];
    if(pkTouchRect->acName.getLength() != 0)
    {
      const AAVVector3<FLOAT> *pkOffsetVector = &pkTouchRect->kVector;
      INT iHalfWidth  = ((INT)(pkTouchRect->kArea.tWidth )) >> 1;
      INT iHalfHeight = ((INT)(pkTouchRect->kArea.tHeight)) >> 1;

      if((pMousePoint->x > ((INT)pkOffsetVector->x - iHalfWidth))  && (pMousePoint->x < ((INT)pkOffsetVector->x + iHalfWidth)) && 
         (pMousePoint->y > ((INT)pkOffsetVector->y - iHalfHeight)) && (pMousePoint->y < ((INT)pkOffsetVector->y + iHalfHeight)))
      {
        return EventHandler(pkTouchRect->eEvent, pkTouchRect->dwEventData);
      }
    }
  }
  return false;
}

BOOL __SVSPModule::EventHandler(DWORD dwEvent, DWORD dwEventData)
{
  switch(dwEvent)
  {
  case __SVSPModule::EVENT_ID_SET_ROOT_MENU:
    m_eDisplayState = DISPLAY_STATE_ROOT_MENU;
    break;
  }

  return false;
}

VOID __SVSPModule::Start()
{
  m_bAbort               = false;
  m_dwProcessUpdateMask  = 0;
  m_hThreadStartEvent    = CreateEvent(NULL, false, false, (CHAR*)this);
  m_hThreadExitEvent     = CreateEvent(NULL, false, false, (CHAR*)this);
  m_hThreadHandle        = CreateThread(NULL, 0, __Process, this, 0, &m_dwThreadId);
  WaitForSingleObject(m_hThreadStartEvent, INFINITE);
  __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "STARTED");
}

VOID __SVSPModule::Stop()
{
  if(!m_bAbort)
  {
    m_bAbort               = true;
    m_bValid               = false;
    WaitForSingleObject(m_hThreadExitEvent, INFINITE);
    //TerminateThread(m_hThreadHandle, 0);
    m_hThreadHandle = 0;
    __SVSPDEBUG::OUTPUT(m_acName.getCStr(), "STOPED");
  }
}


VOID __SVSPModule::Pause()
{
}

VOID __SVSPModule::Process()
{ 
}

VOID __SVSPModule::DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData)
{
}

VOID CALLBACK __DispatchProcess(SIMCONNECT_RECV* pData, DWORD cbData, VOID *pContext)
{
  __SVSPModule *__THIS = static_cast<__SVSPModule *>(pContext);

  //if(!__THIS->m_bAbort && !__THIS->m_pkAvionics->IsPaused())
  if(!__THIS->m_bAbort)
  {
      __THIS->DispatchProcess(pData, cbData);
  }
}


DWORD WINAPI __Process( LPVOID lpParam )
{
  __SVSPModule *__THIS = static_cast<__SVSPModule *>(lpParam);

  SetEvent(__THIS->m_hThreadStartEvent);

  while(!__THIS->m_bAbort)
  {
    //Update Timings.
    DOUBLE dSystemTime     = __THIS->GetSystemTime();
    __THIS->m_dDeltaTime   = dSystemTime - __THIS->m_dCurrentTime;
    __THIS->m_dCurrentTime = dSystemTime;

    if(!__THIS->m_pkAvionics->IsPaused())
    {
      __THIS->Process();
      //__THIS->m_dwProcessUpdateMask = 0;
    }
    else
    {
      __THIS->Pause();
     // __THIS->m_dwProcessUpdateMask = 0;
    }

    Sleep(__THIS->m_dwThreadSleepTime);
  }

  SetEvent(__THIS->m_hThreadExitEvent);

  return 0;
}

