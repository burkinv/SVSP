#ifndef	__SVSPD3D9_MODULE_H__
#define	__SVSPD3D9_MODULE_H__

#include <Windows.h>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>

#ifdef  __cplusplus
extern "C"
{
#endif

class __SVSPD3DTexture9 : public IDirect3DTexture9
{
private:
  IDirect3DTexture9 *m_pkTexture;

public:
           __SVSPD3DTexture9(LPDIRECT3DDEVICE9 pkDevice,UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,HANDLE* pSharedHandle);
  virtual ~__SVSPD3DTexture9();

  /*** IUnknown methods ***/
  STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);
  STDMETHOD_(ULONG,AddRef)(THIS);
  STDMETHOD_(ULONG,Release)(THIS);

  /*** IDirect3DBaseTexture9 methods ***/
  STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice);
  STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags);
  STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid,void* pData,DWORD* pSizeOfData);
  STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid);
  STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD PriorityNew);
  STDMETHOD_(DWORD, GetPriority)(THIS);
  STDMETHOD_(void, PreLoad)(THIS);
  STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS);
  STDMETHOD_(DWORD, SetLOD)(THIS_ DWORD LODNew);
  STDMETHOD_(DWORD, GetLOD)(THIS);
  STDMETHOD_(DWORD, GetLevelCount)(THIS);
  STDMETHOD(SetAutoGenFilterType)(THIS_ D3DTEXTUREFILTERTYPE FilterType);
  STDMETHOD_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)(THIS);
  STDMETHOD_(void, GenerateMipSubLevels)(THIS);
  STDMETHOD(GetLevelDesc)(THIS_ UINT Level,D3DSURFACE_DESC *pDesc);
  STDMETHOD(GetSurfaceLevel)(THIS_ UINT Level,IDirect3DSurface9** ppSurfaceLevel);
  STDMETHOD(LockRect)(THIS_ UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags);
  STDMETHOD(UnlockRect)(THIS_ UINT Level);
  STDMETHOD(AddDirtyRect)(THIS_ CONST RECT* pDirtyRect);

#ifdef D3D_DEBUG_INFO
  LPCWSTR Name;
  UINT Width;
  UINT Height;
  UINT Levels;
  DWORD Usage;
  D3DFORMAT Format;
  D3DPOOL Pool;
  DWORD Priority;
  DWORD LOD;
  D3DTEXTUREFILTERTYPE FilterType;
  UINT LockCount;
  LPCWSTR CreationCallStack;
#endif
};


class __SVSPD3D9 : public IDirect3D9
{
private:
    LPDIRECT3D9            m_pkD3D;

public:
           __SVSPD3D9(UINT SDKVersion);
 virtual  ~__SVSPD3D9();

  /*** Own methods ***/
  inline LPDIRECT3D9 GetSystemD3D9() const { return m_pkD3D; }

  /*** IUnknown methods ***/
  STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);
  STDMETHOD_(ULONG,AddRef)(THIS);
  STDMETHOD_(ULONG,Release)(THIS);

  /*** IDirect3D9 methods ***/
  STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction);
  STDMETHOD_(UINT, GetAdapterCount)(THIS);
  STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier);
  STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter,D3DFORMAT Format);
  STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode);
  STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter,D3DDISPLAYMODE* pMode);
  STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed);
  STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat);
  STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels);
  STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat);
  STDMETHOD(CheckDeviceFormatConversion)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat);
  STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps);
  STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter);
  STDMETHOD(CreateDevice)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface);

#ifdef D3D_DEBUG_INFO
  LPCWSTR Version;
#endif
};

#define FASTCALL __fastcall

// Make sure the VOID *__THIS comes as the first parameter to comply to __thiscall convention since these may point to member functions as well.
typedef ULONG   (FASTCALL * __SVSPD3DDevice9_Release_Callback)                   (VOID *__THIS, LPDIRECT3DDEVICE9 pkDevice);
typedef HRESULT (FASTCALL * __SVSPD3DDevice9_Reset_Callback)                     (VOID *__THIS, LPDIRECT3DDEVICE9 pkDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT (FASTCALL * __SVSPD3DDevice9_Present_Callback)                   (VOID *__THIS, LPDIRECT3DDEVICE9 pkDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
typedef HRESULT (FASTCALL * __SVSPD3DDevice9_EndScene_Callback)                  (VOID *__THIS, LPDIRECT3DDEVICE9 pkDevice);
typedef HRESULT (FASTCALL * __SVSPD3DDevice9_CreateTexture_Callback)             (VOID *__THIS, LPDIRECT3DDEVICE9 pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
typedef HRESULT (FASTCALL * __SVSPD3DDevice9_SetTexture_Callback)                (VOID *__THIS, LPDIRECT3DDEVICE9 pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture);
typedef HRESULT (FASTCALL * __SVSPD3DDevice9_DrawIndexedPrimitive_Callback)      (VOID *__THIS, LPDIRECT3DDEVICE9 pkDevice, D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);

typedef enum 
{
  __SVSPD3D9_CALLBACK_RELEASE                   = 0,
  __SVSPD3D9_CALLBACK_RESET,
  __SVSPD3D9_CALLBACK_PRESENT,
  __SVSPD3D9_CALLBACK_END_SCENE,
  __SVSPD3D9_CALLBACK_CREATE_TEXTURE,
  __SVSPD3D9_CALLBACK_SET_TEXTURE,
  __SVSPD3D9_CALLBACK_DRAW_INDEXED_PRIMITIVE,

  __SVSPD3D9_CALLBACK_MAX_NUM_CALLBACKS
}__SVSPD3D9_CALLBACK_TYPE;

typedef DWORD (FASTCALL *__SVSPD3D9_Callback)(VOID *);

//typedef struct
//{
//      __SVSPD3D9_Callback      Callback;
//        VOID*               __THIS;
//  const CHAR*                 pcCallbackName;
//} __SVSPD3D9_CallbackStruct;


//#define __SVSPD3DDevice9_STDMETHODDEC(method)         COM_DECLSPEC_NOTHROW HRESULT     __fastcall method
//#define __SVSPD3DDevice9_STDMETHODDEC_(type, method)  COM_DECLSPEC_NOTHROW type        __fastcall method
#define __SVSPD3DDevice9_STDMETHODDEC(method)         COM_DECLSPEC_NOTHROW HRESULT     STDMETHODCALLTYPE method
#define __SVSPD3DDevice9_STDMETHODDEC_(type, method)  COM_DECLSPEC_NOTHROW type        STDMETHODCALLTYPE method

class __SVSPD3DDevice9 : public IDirect3DDevice9
{
private:
  __SVSPD3D9                *m_pkD3D;
  HWND                      m_hWnd;
  UINT                      m_uiAdapter;
  D3DDEVTYPE                m_DeviceType;
  LPDIRECT3DDEVICE9         m_pkDevice;
  UINT                      m_uiDeviceNum;
  D3DPRESENT_PARAMETERS     m_kPresentationParameters;

public:
           __SVSPD3DDevice9(__SVSPD3D9 *pD3D,THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters);
  virtual ~__SVSPD3DDevice9();

  /*** Own methods ***/
  inline UINT               GetAdapter() const { return m_uiAdapter; }
  inline D3DDEVTYPE         GetDeviceType() const { return m_DeviceType; }
  inline LPDIRECT3DDEVICE9  GetSystemDevice() const { return m_pkDevice; }
  inline UINT               GetDeviceNum() const { return m_uiDeviceNum; }

  /*** IUnknown methods ***/
  __SVSPD3DDevice9_STDMETHODDEC(QueryInterface)(THIS_ REFIID riid, void** ppvObj);
  __SVSPD3DDevice9_STDMETHODDEC_(ULONG,AddRef)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC_(ULONG,Release)(THIS);

  /*** IDirect3DDevice9 methods ***/
  __SVSPD3DDevice9_STDMETHODDEC(TestCooperativeLevel)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC_(UINT, GetAvailableTextureMem)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC(EvictManagedResources)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC(GetDirect3D)(THIS_ IDirect3D9** ppD3D9);
  __SVSPD3DDevice9_STDMETHODDEC(GetDeviceCaps)(THIS_ D3DCAPS9* pCaps);
  __SVSPD3DDevice9_STDMETHODDEC(GetDisplayMode)(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode);
  __SVSPD3DDevice9_STDMETHODDEC(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters);
  __SVSPD3DDevice9_STDMETHODDEC(SetCursorProperties)(THIS_ UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap);
  __SVSPD3DDevice9_STDMETHODDEC_(void, SetCursorPosition)(THIS_ int X,int Y,DWORD Flags);
  __SVSPD3DDevice9_STDMETHODDEC_(BOOL, ShowCursor)(THIS_ BOOL bShow);
  __SVSPD3DDevice9_STDMETHODDEC(CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain);
  __SVSPD3DDevice9_STDMETHODDEC(GetSwapChain)(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain);
  __SVSPD3DDevice9_STDMETHODDEC_(UINT, GetNumberOfSwapChains)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC(Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters);
  __SVSPD3DDevice9_STDMETHODDEC(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
  __SVSPD3DDevice9_STDMETHODDEC(GetBackBuffer)(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
  __SVSPD3DDevice9_STDMETHODDEC(GetRasterStatus)(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus);
  __SVSPD3DDevice9_STDMETHODDEC(SetDialogBoxMode)(THIS_ BOOL bEnableDialogs);
  __SVSPD3DDevice9_STDMETHODDEC_(void, SetGammaRamp)(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp);
  __SVSPD3DDevice9_STDMETHODDEC_(void, GetGammaRamp)(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp);
  __SVSPD3DDevice9_STDMETHODDEC(CreateTexture)(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
  __SVSPD3DDevice9_STDMETHODDEC(CreateVolumeTexture)(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle);
  __SVSPD3DDevice9_STDMETHODDEC(CreateCubeTexture)(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle);
  __SVSPD3DDevice9_STDMETHODDEC(CreateVertexBuffer)(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle);
  __SVSPD3DDevice9_STDMETHODDEC(CreateIndexBuffer)(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle);
  __SVSPD3DDevice9_STDMETHODDEC(CreateRenderTarget)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
  __SVSPD3DDevice9_STDMETHODDEC(CreateDepthStencilSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
  __SVSPD3DDevice9_STDMETHODDEC(UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint);
  __SVSPD3DDevice9_STDMETHODDEC(UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture);
  __SVSPD3DDevice9_STDMETHODDEC(GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface);
  __SVSPD3DDevice9_STDMETHODDEC(GetFrontBufferData)(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface);
  __SVSPD3DDevice9_STDMETHODDEC(StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
  __SVSPD3DDevice9_STDMETHODDEC(ColorFill)(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color);
  __SVSPD3DDevice9_STDMETHODDEC(CreateOffscreenPlainSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
  __SVSPD3DDevice9_STDMETHODDEC(SetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
  __SVSPD3DDevice9_STDMETHODDEC(GetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
  __SVSPD3DDevice9_STDMETHODDEC(SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil);
  __SVSPD3DDevice9_STDMETHODDEC(GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface);
  __SVSPD3DDevice9_STDMETHODDEC(BeginScene)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC(EndScene)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC(Clear)(THIS_ DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
  __SVSPD3DDevice9_STDMETHODDEC(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
  __SVSPD3DDevice9_STDMETHODDEC(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix);
  __SVSPD3DDevice9_STDMETHODDEC(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE,CONST D3DMATRIX*);
  __SVSPD3DDevice9_STDMETHODDEC(SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport);
  __SVSPD3DDevice9_STDMETHODDEC(GetViewport)(THIS_ D3DVIEWPORT9* pViewport);
  __SVSPD3DDevice9_STDMETHODDEC(SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial);
  __SVSPD3DDevice9_STDMETHODDEC(GetMaterial)(THIS_ D3DMATERIAL9* pMaterial);
  __SVSPD3DDevice9_STDMETHODDEC(SetLight)(THIS_ DWORD Index,CONST D3DLIGHT9*);
  __SVSPD3DDevice9_STDMETHODDEC(GetLight)(THIS_ DWORD Index,D3DLIGHT9*);
  __SVSPD3DDevice9_STDMETHODDEC(LightEnable)(THIS_ DWORD Index,BOOL Enable);
  __SVSPD3DDevice9_STDMETHODDEC(GetLightEnable)(THIS_ DWORD Index,BOOL* pEnable);
  __SVSPD3DDevice9_STDMETHODDEC(SetClipPlane)(THIS_ DWORD Index,CONST float* pPlane);
  __SVSPD3DDevice9_STDMETHODDEC(GetClipPlane)(THIS_ DWORD Index,float* pPlane);
  __SVSPD3DDevice9_STDMETHODDEC(SetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD Value);
  __SVSPD3DDevice9_STDMETHODDEC(GetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD* pValue);
  __SVSPD3DDevice9_STDMETHODDEC(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB);
  __SVSPD3DDevice9_STDMETHODDEC(BeginStateBlock)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC(EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB);
  __SVSPD3DDevice9_STDMETHODDEC(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus);
  __SVSPD3DDevice9_STDMETHODDEC(GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus);
  __SVSPD3DDevice9_STDMETHODDEC(GetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9** ppTexture);
  __SVSPD3DDevice9_STDMETHODDEC(SetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9* pTexture);
  __SVSPD3DDevice9_STDMETHODDEC(GetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue);
  __SVSPD3DDevice9_STDMETHODDEC(SetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value);
  __SVSPD3DDevice9_STDMETHODDEC(GetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue);
  __SVSPD3DDevice9_STDMETHODDEC(SetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
  __SVSPD3DDevice9_STDMETHODDEC(ValidateDevice)(THIS_ DWORD* pNumPasses);
  __SVSPD3DDevice9_STDMETHODDEC(SetPaletteEntries)(THIS_ UINT PaletteNumber,CONST PALETTEENTRY* pEntries);
  __SVSPD3DDevice9_STDMETHODDEC(GetPaletteEntries)(THIS_ UINT PaletteNumber,PALETTEENTRY* pEntries);
  __SVSPD3DDevice9_STDMETHODDEC(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber);
  __SVSPD3DDevice9_STDMETHODDEC(GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber);
  __SVSPD3DDevice9_STDMETHODDEC(SetScissorRect)(THIS_ CONST RECT* pRect);
  __SVSPD3DDevice9_STDMETHODDEC(GetScissorRect)(THIS_ RECT* pRect);
  __SVSPD3DDevice9_STDMETHODDEC(SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware);
  __SVSPD3DDevice9_STDMETHODDEC_(BOOL, GetSoftwareVertexProcessing)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC(SetNPatchMode)(THIS_ float nSegments);
  __SVSPD3DDevice9_STDMETHODDEC_(float, GetNPatchMode)(THIS);
  __SVSPD3DDevice9_STDMETHODDEC(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
  __SVSPD3DDevice9_STDMETHODDEC(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
  __SVSPD3DDevice9_STDMETHODDEC(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
  __SVSPD3DDevice9_STDMETHODDEC(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
  __SVSPD3DDevice9_STDMETHODDEC(ProcessVertices)(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags);
  __SVSPD3DDevice9_STDMETHODDEC(CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
  __SVSPD3DDevice9_STDMETHODDEC(SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl);
  __SVSPD3DDevice9_STDMETHODDEC(GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl);
  __SVSPD3DDevice9_STDMETHODDEC(SetFVF)(THIS_ DWORD FVF);
  __SVSPD3DDevice9_STDMETHODDEC(GetFVF)(THIS_ DWORD* pFVF);
  __SVSPD3DDevice9_STDMETHODDEC(CreateVertexShader)(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);
  __SVSPD3DDevice9_STDMETHODDEC(SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader);
  __SVSPD3DDevice9_STDMETHODDEC(GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader);
  __SVSPD3DDevice9_STDMETHODDEC(SetVertexShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
  __SVSPD3DDevice9_STDMETHODDEC(GetVertexShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount);
  __SVSPD3DDevice9_STDMETHODDEC(SetVertexShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
  __SVSPD3DDevice9_STDMETHODDEC(GetVertexShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount);
  __SVSPD3DDevice9_STDMETHODDEC(SetVertexShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
  __SVSPD3DDevice9_STDMETHODDEC(GetVertexShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
  __SVSPD3DDevice9_STDMETHODDEC(SetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);
  __SVSPD3DDevice9_STDMETHODDEC(GetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride);
  __SVSPD3DDevice9_STDMETHODDEC(SetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT Setting);
  __SVSPD3DDevice9_STDMETHODDEC(GetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT* pSetting);
  __SVSPD3DDevice9_STDMETHODDEC(SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData);
  __SVSPD3DDevice9_STDMETHODDEC(GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData);
  __SVSPD3DDevice9_STDMETHODDEC(CreatePixelShader)(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);
  __SVSPD3DDevice9_STDMETHODDEC(SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader);
  __SVSPD3DDevice9_STDMETHODDEC(GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader);
  __SVSPD3DDevice9_STDMETHODDEC(SetPixelShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
  __SVSPD3DDevice9_STDMETHODDEC(GetPixelShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount);
  __SVSPD3DDevice9_STDMETHODDEC(SetPixelShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
  __SVSPD3DDevice9_STDMETHODDEC(GetPixelShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount);
  __SVSPD3DDevice9_STDMETHODDEC(SetPixelShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
  __SVSPD3DDevice9_STDMETHODDEC(GetPixelShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount);
  __SVSPD3DDevice9_STDMETHODDEC(DrawRectPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
  __SVSPD3DDevice9_STDMETHODDEC(DrawTriPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);
  __SVSPD3DDevice9_STDMETHODDEC(DeletePatch)(THIS_ UINT Handle);
  __SVSPD3DDevice9_STDMETHODDEC(CreateQuery)(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery);

#ifdef D3D_DEBUG_INFO
  D3DDEVICE_CREATION_PARAMETERS CreationParameters;
  D3DPRESENT_PARAMETERS PresentParameters;
  D3DDISPLAYMODE DisplayMode;
  D3DCAPS9 Caps;

  UINT AvailableTextureMem;
  UINT SwapChains;
  UINT Textures;
  UINT VertexBuffers;
  UINT IndexBuffers;
  UINT VertexShaders;
  UINT PixelShaders;

  D3DVIEWPORT9 Viewport;
  D3DMATRIX ProjectionMatrix;
  D3DMATRIX ViewMatrix;
  D3DMATRIX WorldMatrix;
  D3DMATRIX TextureMatrices[8];

  DWORD FVF;
  UINT VertexSize;
  DWORD VertexShaderVersion;
  DWORD PixelShaderVersion;
  BOOL SoftwareVertexProcessing;

  D3DMATERIAL9 Material;
  D3DLIGHT9 Lights[16];
  BOOL LightsEnabled[16];

  D3DGAMMARAMP GammaRamp;
  RECT ScissorRect;
  BOOL DialogBoxMode;
#endif
};

#ifdef  __cplusplus
}
#endif

extern "C" HWND    WINAPI __SVSPD3D9_GetFocusWindow();
extern "C" HRESULT WINAPI __SVSPD3D9_RegisterCallback  (__SVSPD3D9_CALLBACK_TYPE CallBackType, __SVSPD3D9_Callback Callback, VOID *Context);
extern "C" HRESULT WINAPI __SVSPD3D9_UnregisterCallback(__SVSPD3D9_CALLBACK_TYPE CallBackType);
extern "C" HRESULT WINAPI __SVSPD3D9_RegisterDrawIndexedPrimitiveCallback  (__SVSPD3DDevice9_DrawIndexedPrimitive_Callback Callback, VOID *Context);
extern "C" HRESULT WINAPI __SVSPD3D9_UnregisterDrawIndexedPrimitiveCallback();
extern "C" HRESULT WINAPI __SVSPD3D9_RegisterSetTextureCallback  (__SVSPD3DDevice9_SetTexture_Callback Callback, VOID *Context);
extern "C" HRESULT WINAPI __SVSPD3D9_UnregisterSetTextureCallback();


#endif	/* __SVSPD3D9_MODULE_H__ */
