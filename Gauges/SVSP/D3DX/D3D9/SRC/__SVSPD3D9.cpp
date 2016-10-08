#include "__SVSPD3D9.h"
#include "__SVSPDebug.h"

HWND g_hFocusWindow = NULL;

//static __SVSPD3D9_CallbackStruct g_AAVD3D9_Callback_Table[__SVSPD3D9_CALLBACK_MAX_NUM_CALLBACKS] = 
//{
//  {NULL, NULL, "__SVSPD3D9_CALLBACK_RELEASE"                       },  //__SVSPD3D9_CALLBACK_RELEASE,
//  {NULL, NULL, "__SVSPD3D9_CALLBACK_RESET"                         },  //__SVSPD3D9_CALLBACK_RESET,
//  {NULL, NULL, "__SVSPD3D9_CALLBACK_CREATE_TEXTURE"                },  //__SVSPD3D9_CALLBACK_CREATE_TEXTURE,
//  {NULL, NULL, "__SVSPD3D9_CALLBACK_SET_TEXTURE"                   },  //__SVSPD3D9_CALLBACK_SET_TEXTURE,
//  {NULL, NULL, "__SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE"        },  //__SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE,
//};

static __SVSPD3DDevice9_Release_Callback              g_ReleaseCallback                      = NULL;
static                             VOID*             g_ReleaseCallbackContext               = NULL;
static __SVSPD3DDevice9_Reset_Callback                g_ResetCallback                        = NULL;
static                             VOID*             g_ResetCallbackContext                 = NULL;
static __SVSPD3DDevice9_Present_Callback              g_PresentCallback                      = NULL;
static                             VOID*             g_PresentCallbackContext               = NULL;
static __SVSPD3DDevice9_EndScene_Callback             g_EndSceneCallback                     = NULL;
static                             VOID*             g_EndSceneCallbackContext              = NULL;
static __SVSPD3DDevice9_CreateTexture_Callback        g_CreateTextureCallback                = NULL;
static                             VOID*             g_CreateTextureCallbackContext         = NULL;
static __SVSPD3DDevice9_SetTexture_Callback           g_SetTextureCallback                   = NULL;
static                             VOID*             g_SetTextureCallbackContext            = NULL;
static __SVSPD3DDevice9_DrawIndexedPrimitive_Callback g_DrawIndexedPrimitiveCallback         = NULL;
static                             VOID*             g_DrawIndexedPrimitiveCallbackContext  = NULL;

#define __SVSPD3DTexture9_STDMETHODDEF(method)  COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE __SVSPD3DTexture9::method
#define __SVSPD3DTexture9_STDMETHODDEF_(type, method)  COM_DECLSPEC_NOTHROW type STDMETHODCALLTYPE __SVSPD3DTexture9::method


__SVSPD3DTexture9::__SVSPD3DTexture9(LPDIRECT3DDEVICE9 pkDevice,UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,HANDLE* pSharedHandle)
{
  HRESULT hRes = pkDevice->CreateTexture(Width,Height,Levels,Usage,Format,Pool, &m_pkTexture, pSharedHandle);
}
__SVSPD3DTexture9::~__SVSPD3DTexture9()
{
}

/*** IUnknown methods ***/
__SVSPD3DTexture9_STDMETHODDEF(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
{
  return m_pkTexture->QueryInterface(riid, ppvObj);
}
__SVSPD3DTexture9_STDMETHODDEF_(ULONG,AddRef)()
{
  return m_pkTexture->AddRef();
}
__SVSPD3DTexture9_STDMETHODDEF_(ULONG,Release)()
{
  return m_pkTexture->Release();
}

/*** IDirect3DBaseTexture9 methods ***/
__SVSPD3DTexture9_STDMETHODDEF(GetDevice)(THIS_ IDirect3DDevice9** ppDevice)
{
  return m_pkTexture->GetDevice(ppDevice);
}
__SVSPD3DTexture9_STDMETHODDEF(SetPrivateData)(THIS_ REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)
{
  return m_pkTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}
__SVSPD3DTexture9_STDMETHODDEF(GetPrivateData)(THIS_ REFGUID refguid,void* pData,DWORD* pSizeOfData)
{
  return m_pkTexture->GetPrivateData(refguid, pData, pSizeOfData);
}
__SVSPD3DTexture9_STDMETHODDEF(FreePrivateData)(THIS_ REFGUID refguid)
{
  return m_pkTexture->FreePrivateData(refguid);
}
__SVSPD3DTexture9_STDMETHODDEF_(DWORD, SetPriority)(THIS_ DWORD PriorityNew)
{
  return m_pkTexture->SetPriority(PriorityNew);
}
__SVSPD3DTexture9_STDMETHODDEF_(DWORD, GetPriority)(THIS)
{
  return m_pkTexture->GetPriority();
}
__SVSPD3DTexture9_STDMETHODDEF_(void, PreLoad)(THIS)
{
  m_pkTexture->PreLoad();
}
__SVSPD3DTexture9_STDMETHODDEF_(D3DRESOURCETYPE, GetType)(THIS)
{
  return m_pkTexture->GetType();
}
__SVSPD3DTexture9_STDMETHODDEF_(DWORD, SetLOD)(THIS_ DWORD LODNew)
{
  return m_pkTexture->SetLOD(LODNew);
}
__SVSPD3DTexture9_STDMETHODDEF_(DWORD, GetLOD)(THIS)
{
  return m_pkTexture->GetLOD();
}
__SVSPD3DTexture9_STDMETHODDEF_(DWORD, GetLevelCount)(THIS)
{
  return m_pkTexture->GetLevelCount();
}
__SVSPD3DTexture9_STDMETHODDEF(SetAutoGenFilterType)(THIS_ D3DTEXTUREFILTERTYPE FilterType)
{
  return m_pkTexture->SetAutoGenFilterType(FilterType);
}
__SVSPD3DTexture9_STDMETHODDEF_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)(THIS)
{
  return m_pkTexture->GetAutoGenFilterType();
}
__SVSPD3DTexture9_STDMETHODDEF_(void, GenerateMipSubLevels)(THIS)
{
  m_pkTexture->GenerateMipSubLevels();
}
__SVSPD3DTexture9_STDMETHODDEF(GetLevelDesc)(THIS_ UINT Level,D3DSURFACE_DESC *pDesc)
{
  return m_pkTexture->GetLevelDesc(Level, pDesc);
}
__SVSPD3DTexture9_STDMETHODDEF(GetSurfaceLevel)(THIS_ UINT Level,IDirect3DSurface9** ppSurfaceLevel)
{
  return m_pkTexture->GetSurfaceLevel(Level, ppSurfaceLevel);
}
__SVSPD3DTexture9_STDMETHODDEF(LockRect)(THIS_ UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
  return m_pkTexture->LockRect(Level, pLockedRect, pRect, Flags);
}
__SVSPD3DTexture9_STDMETHODDEF(UnlockRect)(THIS_ UINT Level)
{
  return m_pkTexture->UnlockRect(Level);
}
__SVSPD3DTexture9_STDMETHODDEF(AddDirtyRect)(THIS_ CONST RECT* pDirtyRect)
{
  return m_pkTexture->AddDirtyRect(pDirtyRect);
}


#define __SVSPD3DDevice9_STDMETHODDEF(method)  COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE __SVSPD3DDevice9::method
#define __SVSPD3DDevice9_STDMETHODDEF_(type, method)  COM_DECLSPEC_NOTHROW type STDMETHODCALLTYPE __SVSPD3DDevice9::method


/////////////////////////////////////////////////////////////////////
// class __SVSPD3DDevice9
/////////////////////////////////////////////////////////////////////
__SVSPD3DDevice9::__SVSPD3DDevice9(__SVSPD3D9 *pkD3D,THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  m_pkD3D                   = pkD3D;
  m_hWnd                    = hFocusWindow;
  m_uiAdapter               = Adapter;
  m_DeviceType              = DeviceType;
  m_pkDevice                = NULL;
  m_kPresentationParameters = *pPresentationParameters;

  (m_pkD3D->GetSystemD3D9())->CreateDevice(Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters, &m_pkDevice);

  __SVSPDEBUG::OUTPUT("__SVSPD3DDevice9", "NEW HAL DEVICE CREATED");
}
__SVSPD3DDevice9::~__SVSPD3DDevice9()
{
  //Do not delete m_pkDevice since the IDirect3DDevice9::Release() has already freed the instance.
  __SVSPDEBUG::OUTPUT("__SVSPD3DDevice9", "HAL DEVICE DELETED");
}
/*** Own methods ***/

/*** IUnknown methods ***/
__SVSPD3DDevice9_STDMETHODDEF(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
{
  return m_pkDevice->QueryInterface(riid,ppvObj);
}
__SVSPD3DDevice9_STDMETHODDEF_(ULONG,AddRef)(THIS)
{
  return m_pkDevice->AddRef();
}
__SVSPD3DDevice9_STDMETHODDEF_(ULONG,Release)(THIS)
{
  //__SVSPD3DDevice9_Release_Callback
  //__SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[__SVSPD3D9_CALLBACK_RELEASE];
  //__SVSPD3DDevice9_Release_Callback Callback = (__SVSPD3DDevice9_Release_Callback)(pkCallbackStruct->Callback);
  //ULONG ulRefCount = (Callback != NULL) ? Callback(pkCallbackStruct->__THIS, m_pkDevice) : m_pkDevice->Release();
  if(g_ReleaseCallback != NULL) g_ReleaseCallback(g_ReleaseCallbackContext, m_pkDevice);

  ULONG ulRefCount = m_pkDevice->Release();

  if(ulRefCount == 0)
  {
    delete this;
  }

  return ulRefCount;
}
__SVSPD3DDevice9_STDMETHODDEF(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  //__SVSPD3DDevice9_Reset_Callback
  //__SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[__SVSPD3D9_CALLBACK_RESET];
  //__SVSPD3DDevice9_Reset_Callback Callback = (__SVSPD3DDevice9_Reset_Callback)(pkCallbackStruct->Callback);
  //return (Callback != NULL) ? Callback(pkCallbackStruct->__THIS, m_pkDevice, pPresentationParameters) : m_pkDevice->Reset(pPresentationParameters);
  if(g_ResetCallback != NULL) g_ResetCallback(g_ResetCallbackContext, m_pkDevice, pPresentationParameters);

  HRESULT hResult = m_pkDevice->Reset(pPresentationParameters);
  AAV_ASSERT_RETURN(hResult == S_OK, hResult)

  return hResult;
}

/*** IDirect3DDevice9 methods ***/
__SVSPD3DDevice9_STDMETHODDEF(TestCooperativeLevel)(THIS)
{
  return m_pkDevice->TestCooperativeLevel();
}
__SVSPD3DDevice9_STDMETHODDEF_(UINT, GetAvailableTextureMem)(THIS)
{ 
  return m_pkDevice->GetAvailableTextureMem();
}
__SVSPD3DDevice9_STDMETHODDEF(EvictManagedResources)(THIS)
{ 
  return m_pkDevice->EvictManagedResources(); 
}
__SVSPD3DDevice9_STDMETHODDEF(GetDirect3D)(THIS_ IDirect3D9** ppD3D9)
{ 
  return m_pkDevice->GetDirect3D(ppD3D9);
}
__SVSPD3DDevice9_STDMETHODDEF(GetDeviceCaps)(THIS_ D3DCAPS9* pCaps)
{ 
  return m_pkDevice->GetDeviceCaps(pCaps);
}
__SVSPD3DDevice9_STDMETHODDEF(GetDisplayMode)(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode)
{ 
  return m_pkDevice->GetDisplayMode(iSwapChain,pMode);
}
__SVSPD3DDevice9_STDMETHODDEF(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters)
{ 
  return m_pkDevice->GetCreationParameters(pParameters);
}
__SVSPD3DDevice9_STDMETHODDEF(SetCursorProperties)(THIS_ UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{ 
  return m_pkDevice->SetCursorProperties(XHotSpot,YHotSpot,pCursorBitmap);
}
__SVSPD3DDevice9_STDMETHODDEF_(void, SetCursorPosition)(THIS_ int X,int Y,DWORD Flags)
{
  m_pkDevice->SetCursorPosition(X,Y,Flags);
}
__SVSPD3DDevice9_STDMETHODDEF_(BOOL, ShowCursor)(THIS_ BOOL bShow)
{
  return m_pkDevice->ShowCursor(bShow);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
{ 
  return m_pkDevice->CreateAdditionalSwapChain(pPresentationParameters,pSwapChain);
}
__SVSPD3DDevice9_STDMETHODDEF(GetSwapChain)(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{ 
  return m_pkDevice->GetSwapChain(iSwapChain,pSwapChain);
}
__SVSPD3DDevice9_STDMETHODDEF_(UINT, GetNumberOfSwapChains)(THIS)
{ 
  return m_pkDevice->GetNumberOfSwapChains();
}
__SVSPD3DDevice9_STDMETHODDEF(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{ 
  //__SVSPD3DDevice9_Present_Callback
  //__SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[__SVSPD3D9_CALLBACK_PRESENT];
  //__SVSPD3DDevice9_Present_Callback Callback = (__SVSPD3DDevice9_Present_Callback)(pkCallbackStruct->Callback);
  //return (Callback != NULL) ? Callback(pkCallbackStruct->__THIS, m_pkDevice,pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion) : m_pkDevice->Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
  return (g_PresentCallback != NULL) ? g_PresentCallback(g_PresentCallbackContext, m_pkDevice,pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion) : m_pkDevice->Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
}
__SVSPD3DDevice9_STDMETHODDEF(GetBackBuffer)(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{ 
  return m_pkDevice->GetBackBuffer(iSwapChain,iBackBuffer,Type,ppBackBuffer);
}
__SVSPD3DDevice9_STDMETHODDEF(GetRasterStatus)(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{ 
  return m_pkDevice->GetRasterStatus(iSwapChain,pRasterStatus);
}
__SVSPD3DDevice9_STDMETHODDEF(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs)
{ 
  return m_pkDevice->SetDialogBoxMode(bEnableDialogs);
}
__SVSPD3DDevice9_STDMETHODDEF_(void, SetGammaRamp)(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{ 
  m_pkDevice->SetGammaRamp(iSwapChain,Flags,pRamp);
}
__SVSPD3DDevice9_STDMETHODDEF_(void, GetGammaRamp)(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
  m_pkDevice->GetGammaRamp(iSwapChain,pRamp);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateTexture)(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{ 
  //__SVSPD3DDevice9_CreateTexture_Callback
  //__SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[__SVSPD3D9_CALLBACK_CREATE_TEXTURE];
  //__SVSPD3DDevice9_CreateTexture_Callback Callback = (__SVSPD3DDevice9_CreateTexture_Callback)(pkCallbackStruct->Callback);
  //return (Callback != NULL) ? Callback(pkCallbackStruct->__THIS, m_pkDevice,Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle) : m_pkDevice->CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle);
  return (g_CreateTextureCallback != NULL) ? g_CreateTextureCallback(g_CreateTextureCallbackContext, m_pkDevice,Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle) : m_pkDevice->CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateVolumeTexture)(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{ 
  return m_pkDevice->CreateVolumeTexture(Width,Height,Depth,Levels,Usage,Format,Pool,ppVolumeTexture,pSharedHandle);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateCubeTexture)(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{ 
  return m_pkDevice->CreateCubeTexture(EdgeLength,Levels,Usage,Format,Pool,ppCubeTexture,pSharedHandle);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateVertexBuffer)(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{ 
  return m_pkDevice->CreateVertexBuffer(Length,Usage,FVF,Pool,ppVertexBuffer,pSharedHandle);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateIndexBuffer)(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{ 
  return m_pkDevice->CreateIndexBuffer(Length,Usage,Format,Pool, ppIndexBuffer,pSharedHandle);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateRenderTarget)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{ 
  return m_pkDevice->CreateRenderTarget(Width,Height,Format,MultiSample,MultisampleQuality,Lockable,ppSurface,pSharedHandle);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateDepthStencilSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{ 
  return m_pkDevice->CreateDepthStencilSurface(Width,Height,Format,MultiSample,MultisampleQuality,Discard,ppSurface,pSharedHandle);
}
__SVSPD3DDevice9_STDMETHODDEF(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{ 
  return m_pkDevice->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint);
}
__SVSPD3DDevice9_STDMETHODDEF(UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{ 
  return m_pkDevice->UpdateTexture(pSourceTexture,pDestinationTexture);
}
__SVSPD3DDevice9_STDMETHODDEF(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{ 
  return m_pkDevice->GetRenderTargetData(pRenderTarget,pDestSurface);
}
__SVSPD3DDevice9_STDMETHODDEF(GetFrontBufferData)(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{ 
  return m_pkDevice->GetFrontBufferData(iSwapChain,pDestSurface);
}
__SVSPD3DDevice9_STDMETHODDEF(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{ 
  return m_pkDevice->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter);
}
__SVSPD3DDevice9_STDMETHODDEF(ColorFill)(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{ 
  return m_pkDevice->ColorFill(pSurface,pRect,color);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateOffscreenPlainSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{ 
  return m_pkDevice->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle);
}
__SVSPD3DDevice9_STDMETHODDEF(SetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{ 
  return m_pkDevice->SetRenderTarget(RenderTargetIndex,pRenderTarget);
}
__SVSPD3DDevice9_STDMETHODDEF(GetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{ 
  return m_pkDevice->GetRenderTarget(RenderTargetIndex,ppRenderTarget);
}
__SVSPD3DDevice9_STDMETHODDEF(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil)
{ 
  return m_pkDevice->SetDepthStencilSurface(pNewZStencil);
}
__SVSPD3DDevice9_STDMETHODDEF(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface)
{ 
  return m_pkDevice->GetDepthStencilSurface(ppZStencilSurface);
}
__SVSPD3DDevice9_STDMETHODDEF(BeginScene)(THIS)
{
  return m_pkDevice->BeginScene();
}
__SVSPD3DDevice9_STDMETHODDEF(EndScene)(THIS)
{
  //__SVSPD3DDevice9_EndScene_Callback
  //__SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[__SVSPD3D9_CALLBACK_END_SCENE];
  //__SVSPD3DDevice9_EndScene_Callback Callback = (__SVSPD3DDevice9_EndScene_Callback)(pkCallbackStruct->Callback);
  //return (Callback != NULL) ? Callback(pkCallbackStruct->__THIS, m_pkDevice) : m_pkDevice->EndScene();
  return (g_EndSceneCallback != NULL) ? g_EndSceneCallback(g_EndSceneCallbackContext, m_pkDevice) : m_pkDevice->EndScene();
}
__SVSPD3DDevice9_STDMETHODDEF(Clear)(THIS_ DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{ 
  return m_pkDevice->Clear(Count,pRects,Flags,Color,Z,Stencil);
}
__SVSPD3DDevice9_STDMETHODDEF(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
  return m_pkDevice->SetTransform(State, pMatrix);
}
__SVSPD3DDevice9_STDMETHODDEF(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
{ 
  return m_pkDevice->GetTransform(State,pMatrix);
}
__SVSPD3DDevice9_STDMETHODDEF(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{ 
  return m_pkDevice->MultiplyTransform(State,pMatrix);
}
__SVSPD3DDevice9_STDMETHODDEF(SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport)
{ 
  return m_pkDevice->SetViewport(pViewport);
}
__SVSPD3DDevice9_STDMETHODDEF(GetViewport)(THIS_ D3DVIEWPORT9* pViewport)
{ 
  return m_pkDevice->GetViewport(pViewport);
}
__SVSPD3DDevice9_STDMETHODDEF(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial)
{
  return m_pkDevice->SetMaterial(pMaterial);
}
__SVSPD3DDevice9_STDMETHODDEF(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial)
{ 
  return m_pkDevice->GetMaterial(pMaterial);
}
__SVSPD3DDevice9_STDMETHODDEF(SetLight)(THIS_ DWORD Index,CONST D3DLIGHT9* light)
{ 
  return m_pkDevice->SetLight(Index,light);
}
__SVSPD3DDevice9_STDMETHODDEF(GetLight)(THIS_ DWORD Index,D3DLIGHT9* light)
{ 
  return m_pkDevice->GetLight(Index,light);
}
__SVSPD3DDevice9_STDMETHODDEF(LightEnable)(THIS_ DWORD Index,BOOL Enable)
{ 
  return m_pkDevice->LightEnable(Index,Enable);
}
__SVSPD3DDevice9_STDMETHODDEF(GetLightEnable)(THIS_ DWORD Index,BOOL* pEnable)
{ 
  return m_pkDevice->GetLightEnable(Index,pEnable);
}
__SVSPD3DDevice9_STDMETHODDEF(SetClipPlane)(THIS_ DWORD Index,CONST float* pPlane)
{ 
  return m_pkDevice->SetClipPlane(Index,pPlane);
}
__SVSPD3DDevice9_STDMETHODDEF(GetClipPlane)(THIS_ DWORD Index,float* pPlane)
{ 
  return m_pkDevice->GetClipPlane(Index,pPlane);
}
__SVSPD3DDevice9_STDMETHODDEF(SetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD Value)
{
  return m_pkDevice->SetRenderState(State,Value);
}
__SVSPD3DDevice9_STDMETHODDEF(GetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD* pValue)
{ 
  return m_pkDevice->GetRenderState(State,pValue);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
{ 
  return m_pkDevice->CreateStateBlock(Type,ppSB);
}
__SVSPD3DDevice9_STDMETHODDEF(BeginStateBlock)(THIS)
{ 
  return m_pkDevice->BeginStateBlock();
}
__SVSPD3DDevice9_STDMETHODDEF(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB)
{ 
  return m_pkDevice->EndStateBlock(ppSB);
}
__SVSPD3DDevice9_STDMETHODDEF(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus)
{ 
  return m_pkDevice->SetClipStatus(pClipStatus);
}
__SVSPD3DDevice9_STDMETHODDEF(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus)
{ 
  return m_pkDevice->GetClipStatus(pClipStatus);
}
__SVSPD3DDevice9_STDMETHODDEF(GetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9** ppTexture)
{ 
  return m_pkDevice->GetTexture(Stage,ppTexture);
}
__SVSPD3DDevice9_STDMETHODDEF(SetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9* pTexture)
{ 
  //__SVSPD3DDevice9_SetTexture_Callback
  //__SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[__SVSPD3D9_CALLBACK_SET_TEXTURE];
  //__SVSPD3DDevice9_SetTexture_Callback Callback = (__SVSPD3DDevice9_SetTexture_Callback)(pkCallbackStruct->Callback);
  //return (Callback != NULL) ? Callback(pkCallbackStruct->__THIS, m_pkDevice,Stage,pTexture) : m_pkDevice->SetTexture(Stage,pTexture);
  return (g_SetTextureCallback != NULL) ? g_SetTextureCallback(g_SetTextureCallbackContext, m_pkDevice,Stage,pTexture) : m_pkDevice->SetTexture(Stage,pTexture);
}
__SVSPD3DDevice9_STDMETHODDEF(GetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
{ 
  return m_pkDevice->GetTextureStageState(Stage,Type,pValue);
}
__SVSPD3DDevice9_STDMETHODDEF(SetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{ 
  return m_pkDevice->SetTextureStageState(Stage,Type, Value);
}
__SVSPD3DDevice9_STDMETHODDEF(GetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)
{ 
  return m_pkDevice->GetSamplerState(Sampler,Type,pValue);
}
__SVSPD3DDevice9_STDMETHODDEF(SetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{ 
  return m_pkDevice->SetSamplerState(Sampler,Type,Value);
}
__SVSPD3DDevice9_STDMETHODDEF(ValidateDevice)(THIS_ DWORD* pNumPasses)
{ 
  return m_pkDevice->ValidateDevice(pNumPasses);
}
__SVSPD3DDevice9_STDMETHODDEF(SetPaletteEntries)(THIS_ UINT PaletteNumber,CONST PALETTEENTRY* pEntries)
{ 
  return m_pkDevice->SetPaletteEntries(PaletteNumber,pEntries);
}
__SVSPD3DDevice9_STDMETHODDEF(GetPaletteEntries)(THIS_ UINT PaletteNumber,PALETTEENTRY* pEntries)
{ 
  return m_pkDevice->GetPaletteEntries(PaletteNumber,pEntries);
}
__SVSPD3DDevice9_STDMETHODDEF(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber)
{ 
  return m_pkDevice->SetCurrentTexturePalette(PaletteNumber);
}
__SVSPD3DDevice9_STDMETHODDEF(GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber)
{ 
  return m_pkDevice->GetCurrentTexturePalette(PaletteNumber);
}
__SVSPD3DDevice9_STDMETHODDEF(SetScissorRect)(THIS_ CONST RECT* pRect)
{ 
  return m_pkDevice->SetScissorRect(pRect);
}
__SVSPD3DDevice9_STDMETHODDEF(GetScissorRect)(THIS_ RECT* pRect)
{ 
  return m_pkDevice->GetScissorRect(pRect);
}
__SVSPD3DDevice9_STDMETHODDEF(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware)
{ 
  return m_pkDevice->SetSoftwareVertexProcessing(bSoftware);
}
__SVSPD3DDevice9_STDMETHODDEF_(BOOL, GetSoftwareVertexProcessing)(THIS)
{ 
  return m_pkDevice->GetSoftwareVertexProcessing();
}
__SVSPD3DDevice9_STDMETHODDEF(SetNPatchMode)(THIS_ float nSegments)
{ 
  return m_pkDevice->SetNPatchMode(nSegments);
}
__SVSPD3DDevice9_STDMETHODDEF_(float, GetNPatchMode)(THIS)
{ 
  return m_pkDevice->GetNPatchMode();
}
__SVSPD3DDevice9_STDMETHODDEF(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{ 
  return m_pkDevice->DrawPrimitive(PrimitiveType,StartVertex, PrimitiveCount);
}
__SVSPD3DDevice9_STDMETHODDEF(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
  //__SVSPD3DDevice9_DrawIndexedPrimitive_Callback
  //__SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[__SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE];
  //__SVSPD3DDevice9_DrawIndexedPrimitive_Callback Callback = (__SVSPD3DDevice9_DrawIndexedPrimitive_Callback)(pkCallbackStruct->Callback);
  //return (Callback != NULL) ? Callback(pkCallbackStruct->__THIS, m_pkDevice,Stage,pTexture) : m_pkDevice->SetTexture(Stage,pTexture);
  return (g_DrawIndexedPrimitiveCallback != NULL) ? g_DrawIndexedPrimitiveCallback(g_DrawIndexedPrimitiveCallbackContext, m_pkDevice,PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount) : m_pkDevice->DrawIndexedPrimitive(PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
}
__SVSPD3DDevice9_STDMETHODDEF(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{ 
  return m_pkDevice->DrawPrimitiveUP(PrimitiveType,PrimitiveCount,pVertexStreamZeroData,VertexStreamZeroStride);
}
__SVSPD3DDevice9_STDMETHODDEF(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{ 
  return m_pkDevice->DrawIndexedPrimitiveUP(PrimitiveType,MinVertexIndex,NumVertices,PrimitiveCount,pIndexData,IndexDataFormat,pVertexStreamZeroData, VertexStreamZeroStride);
}
__SVSPD3DDevice9_STDMETHODDEF(ProcessVertices)(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
{ 
  return m_pkDevice->ProcessVertices(SrcStartIndex,DestIndex,VertexCount,pDestBuffer,pVertexDecl,Flags);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
{ 
  return m_pkDevice->CreateVertexDeclaration(pVertexElements,ppDecl);
}
__SVSPD3DDevice9_STDMETHODDEF(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl)
{ 
  return m_pkDevice->SetVertexDeclaration(pDecl);
}
__SVSPD3DDevice9_STDMETHODDEF(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl)
{ 
  return m_pkDevice->GetVertexDeclaration(ppDecl);
}
__SVSPD3DDevice9_STDMETHODDEF(SetFVF)(THIS_ DWORD FVF)
{ 
  return m_pkDevice->SetFVF(FVF);
}
__SVSPD3DDevice9_STDMETHODDEF(GetFVF)(THIS_ DWORD* pFVF)
{ 
  return m_pkDevice->GetFVF(pFVF);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateVertexShader)(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{ 
  return m_pkDevice->CreateVertexShader(pFunction,ppShader);
}
__SVSPD3DDevice9_STDMETHODDEF(SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader)
{ 
  return m_pkDevice->SetVertexShader(pShader);
}
__SVSPD3DDevice9_STDMETHODDEF(GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader)
{ 
  return m_pkDevice->GetVertexShader(ppShader);
}
__SVSPD3DDevice9_STDMETHODDEF(SetVertexShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{ 
  return m_pkDevice->SetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}
__SVSPD3DDevice9_STDMETHODDEF(GetVertexShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{ 
  return m_pkDevice->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}
__SVSPD3DDevice9_STDMETHODDEF(SetVertexShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{ 
  return m_pkDevice->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}
__SVSPD3DDevice9_STDMETHODDEF(GetVertexShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{ 
  return m_pkDevice->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}
__SVSPD3DDevice9_STDMETHODDEF(SetVertexShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{ 
  return m_pkDevice->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}
__SVSPD3DDevice9_STDMETHODDEF(GetVertexShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{ 
  return m_pkDevice->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}
__SVSPD3DDevice9_STDMETHODDEF(SetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
{ 
  return m_pkDevice->SetStreamSource(StreamNumber,pStreamData,OffsetInBytes,Stride);
}
__SVSPD3DDevice9_STDMETHODDEF(GetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride)
{ 
  return m_pkDevice->GetStreamSource(StreamNumber,ppStreamData,OffsetInBytes,pStride);
}
__SVSPD3DDevice9_STDMETHODDEF(SetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT Divider)
{ 
  return m_pkDevice->SetStreamSourceFreq(StreamNumber,Divider);
}
__SVSPD3DDevice9_STDMETHODDEF(GetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT* Divider)
{ 
  return m_pkDevice->GetStreamSourceFreq(StreamNumber,Divider);
}
__SVSPD3DDevice9_STDMETHODDEF(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData)
{ 
  return m_pkDevice->SetIndices(pIndexData);
}
__SVSPD3DDevice9_STDMETHODDEF(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData)
{ 
  return m_pkDevice->GetIndices(ppIndexData);
}
__SVSPD3DDevice9_STDMETHODDEF(CreatePixelShader)(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
{ 
  return m_pkDevice->CreatePixelShader(pFunction,ppShader);
}
__SVSPD3DDevice9_STDMETHODDEF(SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader)
{ 
  return m_pkDevice->SetPixelShader(pShader);
}
__SVSPD3DDevice9_STDMETHODDEF(GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader)
{ 
  return m_pkDevice->GetPixelShader(ppShader);
}
__SVSPD3DDevice9_STDMETHODDEF(SetPixelShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{ 
  return m_pkDevice->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}
__SVSPD3DDevice9_STDMETHODDEF(GetPixelShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{ 
  return m_pkDevice->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}
__SVSPD3DDevice9_STDMETHODDEF(SetPixelShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{ 
  return m_pkDevice->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}
__SVSPD3DDevice9_STDMETHODDEF(GetPixelShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{ 
  return m_pkDevice->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}
__SVSPD3DDevice9_STDMETHODDEF(SetPixelShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{ 
  return m_pkDevice->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}
__SVSPD3DDevice9_STDMETHODDEF(GetPixelShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{ 
  return m_pkDevice->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}
__SVSPD3DDevice9_STDMETHODDEF(DrawRectPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{ 
  return m_pkDevice->DrawRectPatch(Handle,pNumSegs,pRectPatchInfo);
}
__SVSPD3DDevice9_STDMETHODDEF(DrawTriPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{ 
  return m_pkDevice->DrawTriPatch(Handle,pNumSegs,pTriPatchInfo);
}
__SVSPD3DDevice9_STDMETHODDEF(DeletePatch)(THIS_ UINT Handle)
{ 
  return m_pkDevice->DeletePatch(Handle);
}
__SVSPD3DDevice9_STDMETHODDEF(CreateQuery)(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
{ 
  return m_pkDevice->CreateQuery(Type,ppQuery);
}


#define __SVSPD3D9_STDMETHODDEF(method)  COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE __SVSPD3D9::method
#define __SVSPD3D9_STDMETHODDEF_(type, method)  COM_DECLSPEC_NOTHROW type STDMETHODCALLTYPE __SVSPD3D9::method

/////////////////////////////////////////////////////////////////////
// class __SVSPD3D9
/////////////////////////////////////////////////////////////////////
__SVSPD3D9::__SVSPD3D9(UINT SDKVersion)
{
  // CREATE __SVSPDEBUG::LOGING CONSOLE
  __SVSPDEBUG::CreateConsole();

  FARPROC _sys_Direct3DCreate9 = NULL;
  HMODULE _hd3d9Mdl             = NULL;
  char acWinDir[MAX_PATH];
  //CHAR acStr[256];
  GetWindowsDirectory(&acWinDir[0], sizeof(acWinDir));
  strcat_s(&acWinDir[0], sizeof(acWinDir), "\\System32\\d3d9.dll"); 
  //__SVSPDEBUG::OUTPUT(&acWinDir[0]);
  if( (_hd3d9Mdl = LoadLibrary(&acWinDir[0])) != NULL)
    //if( (_hd3d9Mdl = LoadLibrary("C:\\Windows\\System32\\d3d9.dll")) != NULL)
  {
    if( (_sys_Direct3DCreate9 = GetProcAddress(_hd3d9Mdl, "Direct3DCreate9")) != NULL)
    {
      m_pkD3D = ((LPDIRECT3D9 (*)(UINT))_sys_Direct3DCreate9)(SDKVersion);
      //sprintf_s(acStr, sizeof(acStr), "D3D_MOD=0x%x, _sys_Direct3DCreate9 = 0x%x, m_pkD3D = 0x%x", 
      //          (UINT)_hd3d9Mdl, (UINT)_sys_Direct3DCreate9, (UINT)m_pkD3D);
      //__SVSPDEBUG::OUTPUT(m_acName.getCStr(), acStr);
    }
  }

  //Clear Callback table
  //for(UINT i = 0; i < __SVSPD3D9_CALLBACK_MAX_NUM_CALLBACKS; ++i)
  //{
  //  __SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[i];
  //  pkCallbackStruct->Callback                 = NULL;
  //  pkCallbackStruct->__THIS                   = NULL;
  //}
  g_ReleaseCallback                      = NULL;
  g_ReleaseCallbackContext               = NULL;
  g_ResetCallback                        = NULL;
  g_ResetCallbackContext                 = NULL;
  g_PresentCallback                      = NULL;
  g_PresentCallbackContext               = NULL;
  g_EndSceneCallback                     = NULL;
  g_EndSceneCallbackContext              = NULL;
  g_CreateTextureCallback                = NULL;
  g_CreateTextureCallbackContext         = NULL;
  g_SetTextureCallback                   = NULL;
  g_SetTextureCallbackContext            = NULL;
  g_DrawIndexedPrimitiveCallback         = NULL;
  g_DrawIndexedPrimitiveCallbackContext  = NULL;
}

__SVSPD3D9::~__SVSPD3D9()
{
  //Clear Callback table
  //for(UINT i = 0; i < __SVSPD3D9_CALLBACK_MAX_NUM_CALLBACKS; ++i)
  //{
  //  __SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[i];
  //  pkCallbackStruct->Callback                 = NULL;
  //  pkCallbackStruct->__THIS                   = NULL;
  //}
  g_ReleaseCallback                      = NULL;
  g_ReleaseCallbackContext               = NULL;
  g_ResetCallback                        = NULL;
  g_ResetCallbackContext                 = NULL;
  g_PresentCallback                      = NULL;
  g_PresentCallbackContext               = NULL;
  g_EndSceneCallback                     = NULL;
  g_EndSceneCallbackContext              = NULL;
  g_CreateTextureCallback                = NULL;
  g_CreateTextureCallbackContext         = NULL;
  g_SetTextureCallback                   = NULL;
  g_SetTextureCallbackContext            = NULL;
  g_DrawIndexedPrimitiveCallback         = NULL;
  g_DrawIndexedPrimitiveCallbackContext  = NULL;

  if(m_pkD3D != NULL)
  {
    m_pkD3D->Release();
  }

  // DELETE __SVSPDEBUG::LOGING CONSOLE
  __SVSPDEBUG::DeleteConsole();
}

/*** IUnknown methods ***/
__SVSPD3D9_STDMETHODDEF(QueryInterface)(THIS_ REFIID riid, void** ppvObj)
{
  return m_pkD3D->QueryInterface(riid, ppvObj);
}
__SVSPD3D9_STDMETHODDEF_(ULONG,AddRef)(THIS)
{
  return m_pkD3D->AddRef();
}
__SVSPD3D9_STDMETHODDEF_(ULONG,Release)(THIS)
{
  return m_pkD3D->Release();
}
/*** IDirect3D9 methods ***/
__SVSPD3D9_STDMETHODDEF(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction)
{
  return m_pkD3D->RegisterSoftwareDevice(pInitializeFunction);
}
__SVSPD3D9_STDMETHODDEF_(UINT, GetAdapterCount)(THIS)
{
  return m_pkD3D->GetAdapterCount();
}
__SVSPD3D9_STDMETHODDEF(GetAdapterIdentifier)(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier)
{
  return m_pkD3D->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}
__SVSPD3D9_STDMETHODDEF_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter,D3DFORMAT Format)
{
  return m_pkD3D->GetAdapterModeCount(Adapter, Format);
}
__SVSPD3D9_STDMETHODDEF(EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode)
{
  return m_pkD3D->EnumAdapterModes(Adapter,Format,Mode,pMode);
}
__SVSPD3D9_STDMETHODDEF(GetAdapterDisplayMode)(THIS_ UINT Adapter,D3DDISPLAYMODE* pMode)
{
  return m_pkD3D->GetAdapterDisplayMode(Adapter, pMode);
}
__SVSPD3D9_STDMETHODDEF(CheckDeviceType)(THIS_ UINT iAdapter,D3DDEVTYPE DevType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed)
{
  return m_pkD3D->CheckDeviceType(iAdapter, DevType, DisplayFormat, BackBufferFormat, bWindowed);
}
__SVSPD3D9_STDMETHODDEF(CheckDeviceFormat)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat)
{
  return m_pkD3D->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}
__SVSPD3D9_STDMETHODDEF(CheckDeviceMultiSampleType)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
{
  return m_pkD3D->CheckDeviceMultiSampleType(Adapter,DeviceType,SurfaceFormat,Windowed, MultiSampleType, pQualityLevels);
}
__SVSPD3D9_STDMETHODDEF(CheckDepthStencilMatch)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat)
{
  return m_pkD3D->CheckDepthStencilMatch(Adapter,DeviceType,AdapterFormat,RenderTargetFormat,DepthStencilFormat);
}
__SVSPD3D9_STDMETHODDEF(CheckDeviceFormatConversion)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat)
{
  return m_pkD3D->CheckDeviceFormatConversion(Adapter,DeviceType,SourceFormat,TargetFormat);
}
__SVSPD3D9_STDMETHODDEF(GetDeviceCaps)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps)
{
  return m_pkD3D->GetDeviceCaps(Adapter,DeviceType,pCaps);
}
__SVSPD3D9_STDMETHODDEF_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter)
{
  return m_pkD3D->GetAdapterMonitor(Adapter);
}
__SVSPD3D9_STDMETHODDEF(CreateDevice)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface)
{
  //Hook only HAL devices.
  if(DeviceType == D3DDEVTYPE_HAL)
  {
    g_hFocusWindow = hFocusWindow;

    *ppReturnedDeviceInterface = (IDirect3DDevice9 *)(new __SVSPD3DDevice9(this, Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters));
  }
  else
  {
    m_pkD3D->CreateDevice(Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters, ppReturnedDeviceInterface);
  }

  return S_OK;
}


#define DllEXPORT __declspec( dllexport )
extern "C" HWND              WINAPI __SVSPD3D9_GetFocusWindow();
extern "C" HRESULT           WINAPI __SVSPD3D9_RegisterCallback(__SVSPD3D9_CALLBACK_TYPE CallBackType, __SVSPD3D9_Callback Callback, VOID *Context);
extern "C" HRESULT           WINAPI __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_TYPE CallBackType);
extern "C" INT               WINAPI D3DPERF_BeginEvent(D3DCOLOR col,LPCWSTR wszName);
extern "C" INT               WINAPI D3DPERF_EndEvent();
extern "C" VOID              WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName);
extern "C" VOID              WINAPI D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName);
extern "C" VOID              WINAPI D3DPERF_SetOptions(DWORD dwOptions);
extern "C" VOID              WINAPI DebugSetMute();
extern "C" LPDIRECT3D9       WINAPI Direct3DCreate9(UINT);
//extern "C" HRESULT Direct3DCreate9Ex(UINT SDKVersion, IDirect3D9EX **ppD3D);

extern "C" HRESULT           WINAPI __SVSPD3D9_RegisterCallback(__SVSPD3D9_CALLBACK_TYPE CallbackType, __SVSPD3D9_Callback Callback, VOID *__THIS)
{
  AAV_ASSERT_RETURN(CallbackType < __SVSPD3D9_CALLBACK_MAX_NUM_CALLBACKS, S_FALSE);

  //__SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[CallbackType];
  //AAV_ASSERT_RETURN((pkCallbackStruct->Callback == NULL), S_FALSE);

  //pkCallbackStruct->Callback = Callback;
  //pkCallbackStruct->__THIS   = __THIS;

  switch(CallbackType)
  {
  case __SVSPD3D9_CALLBACK_RELEASE:
    AAV_ASSERT_RETURN((g_ReleaseCallback == NULL), S_FALSE);
    g_ReleaseCallback                     = (__SVSPD3DDevice9_Release_Callback)Callback;
    g_ReleaseCallbackContext              = __THIS;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Registered __SVSPD3D9_CALLBACK_RELEASE");
    break;
  case __SVSPD3D9_CALLBACK_RESET:
    AAV_ASSERT_RETURN((g_ResetCallback == NULL), S_FALSE);
    g_ResetCallback                       = (__SVSPD3DDevice9_Reset_Callback)Callback;
    g_ResetCallbackContext                = __THIS;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Registered __SVSPD3D9_CALLBACK_RESET");
    break;
  case __SVSPD3D9_CALLBACK_PRESENT:
    AAV_ASSERT_RETURN((g_PresentCallback == NULL), S_FALSE);
    g_PresentCallback                     = (__SVSPD3DDevice9_Present_Callback)Callback;
    g_PresentCallbackContext              = __THIS;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Registered __SVSPD3D9_CALLBACK_PRESENT");
    break;
  case __SVSPD3D9_CALLBACK_END_SCENE:
    AAV_ASSERT_RETURN((g_EndSceneCallback == NULL), S_FALSE);
    g_EndSceneCallback                    = (__SVSPD3DDevice9_EndScene_Callback)Callback;
    g_EndSceneCallbackContext             = __THIS;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Registered __SVSPD3D9_CALLBACK_END_SCENE");
    break;
  case __SVSPD3D9_CALLBACK_CREATE_TEXTURE:
    AAV_ASSERT_RETURN((g_CreateTextureCallback == NULL), S_FALSE);
    g_CreateTextureCallback               = (__SVSPD3DDevice9_CreateTexture_Callback)Callback;
    g_CreateTextureCallbackContext        = __THIS;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Registered __SVSPD3D9_CALLBACK_CREATE_TEXTURE");
    break;
  case __SVSPD3D9_CALLBACK_SET_TEXTURE:
    AAV_ASSERT_RETURN((g_SetTextureCallback == NULL), S_FALSE);
    g_SetTextureCallback                  = (__SVSPD3DDevice9_SetTexture_Callback)Callback;
    g_SetTextureCallbackContext           = __THIS;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Registered __SVSPD3D9_CALLBACK_SET_TEXTURE");
    break;
  case __SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE:
    AAV_ASSERT_RETURN((g_DrawIndexedPrimitiveCallback == NULL), S_FALSE);
    g_DrawIndexedPrimitiveCallback        = (__SVSPD3DDevice9_DrawIndexedPrimitive_Callback)Callback;
    g_DrawIndexedPrimitiveCallbackContext = __THIS;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Registered __SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE");
    break;
  }

  return S_OK;
}

extern "C" HRESULT           WINAPI __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_TYPE CallbackType)
{
  AAV_ASSERT_RETURN(CallbackType < __SVSPD3D9_CALLBACK_MAX_NUM_CALLBACKS, S_FALSE);

  //__SVSPD3D9_CallbackStruct *pkCallbackStruct = &g_AAVD3D9_Callback_Table[CallbackType];
  //AAV_ASSERT_RETURN((pkCallbackStruct->Callback != NULL), S_FALSE);

  //pkCallbackStruct->Callback = NULL;
  //pkCallbackStruct->__THIS   = NULL;

  switch(CallbackType)
  {
  case __SVSPD3D9_CALLBACK_RELEASE:
    g_ReleaseCallback                     = NULL;
    g_ReleaseCallbackContext              = NULL;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Unregistered __SVSPD3D9_CALLBACK_RELEASE");
    break;
  case __SVSPD3D9_CALLBACK_RESET:
    g_ResetCallback                       = NULL;
    g_ResetCallbackContext                = NULL;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Unregistered __SVSPD3D9_CALLBACK_RESET");
    break;
  case __SVSPD3D9_CALLBACK_PRESENT:
    g_PresentCallback                     = NULL;
    g_PresentCallbackContext              = NULL;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Unregistered __SVSPD3D9_CALLBACK_PRESENT");
    break;
  case __SVSPD3D9_CALLBACK_END_SCENE:
    g_EndSceneCallback                    = NULL;
    g_EndSceneCallbackContext             = NULL;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Unregistered __SVSPD3D9_CALLBACK_END_SCENE");
    break;
  case __SVSPD3D9_CALLBACK_CREATE_TEXTURE:
    g_CreateTextureCallback               = NULL;
    g_CreateTextureCallbackContext        = NULL;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Unregistered __SVSPD3D9_CALLBACK_CREATE_TEXTURE");
    break;
  case __SVSPD3D9_CALLBACK_SET_TEXTURE:
    g_SetTextureCallback                  = NULL;
    g_SetTextureCallbackContext           = NULL;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Unregistered __SVSPD3D9_CALLBACK_SET_TEXTURE");
    break;
  case __SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE:
    g_DrawIndexedPrimitiveCallback        = NULL;
    g_DrawIndexedPrimitiveCallbackContext = NULL;
    __SVSPDEBUG::OUTPUT("__SVSPD3D9", "Unregistered __SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE");
    break;
  }

  return S_OK;
}

extern "C" HRESULT           WINAPI __SVSPD3D9_RegisterDrawIndexedPrimitiveCallback(__SVSPD3DDevice9_DrawIndexedPrimitive_Callback Callback, VOID *__THIS)
{
  g_DrawIndexedPrimitiveCallback        = Callback;
  g_DrawIndexedPrimitiveCallbackContext = __THIS;
  return S_OK;
}

extern "C" HRESULT           WINAPI __SVSPD3D9_UnregisterDrawIndexedPrimitiveCallback()
{
  g_DrawIndexedPrimitiveCallback        = NULL;
  g_DrawIndexedPrimitiveCallbackContext = NULL;
  return S_OK;
}

extern "C" HRESULT WINAPI __SVSPD3D9_RegisterSetTextureCallback(__SVSPD3DDevice9_SetTexture_Callback Callback, VOID *__THIS)
{
  g_SetTextureCallback                  = Callback;
  g_SetTextureCallbackContext           = __THIS;
  return S_OK;
}
extern "C" HRESULT WINAPI __SVSPD3D9_UnregisterSetTextureCallback()
{
  g_SetTextureCallback                  = NULL;
  g_SetTextureCallbackContext           = NULL;
  return S_OK;
}

extern "C" INT WINAPI D3DPERF_BeginEvent(D3DCOLOR col,LPCWSTR wszName)
{
  return 0;
}

extern "C" INT WINAPI D3DPERF_EndEvent()
{
  return 0;
}

extern "C" VOID WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName)
{
}

extern "C" VOID WINAPI D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName)
{
}

extern "C" VOID WINAPI D3DPERF_SetOptions(DWORD dwOptions)
{
}

extern "C" VOID WINAPI DebugSetMute()
{
}

extern "C" LPDIRECT3D9 WINAPI Direct3DCreate9(UINT SDKVersion)
{
  __SVSPD3D9* pkD3D9;
  pkD3D9 = new __SVSPD3D9(SDKVersion);

  return pkD3D9;
}

//extern "C" HRESULT Direct3DCreate9Ex(UINT SDKVersion, IDirect3D9EX **ppD3D)
//{
//  *ppD3D = NULL;
//  return S_OK;
//}

//extern "C" __SVSPD3D9*       WINAPI GetD3D9()
//{
//  __SVSPD3D9 *pkD3D9;
//  m_pkDevice->GetDirect3D(&pkD3D9);
//  return pkD3D9;
//}

extern "C" HWND             WINAPI __SVSPD3D9_GetFocusWindow()
{
  return g_hFocusWindow;
}

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpvReserved)
{
  switch(fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      break;

    case DLL_PROCESS_DETACH:
      break;

    default:
      break;
  }
  return TRUE;
}
