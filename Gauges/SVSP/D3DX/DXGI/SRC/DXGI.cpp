#include <Windows.h>
#include <string>
#include <DXGI.h>
//#include <__AAVAvionics.h>
//#include <D3DX10Core.h>

//static HANDLE                 g_hMapObject = NULL;
//static __AAVAvionics::SHARED_DATA *g_pkSharedData = NULL;
//static __AAVAvionics             **m_pkAvionics = NULL;
//

static  HMODULE __DXGI = NULL;

#define TEST \
  if(__DXGI == NULL) { \
  __DXGI = LoadLibraryA("C:\\WINDOWS\\SYSTEM32\\dxgi.dll"); \
  } \

#define DllEXPORT __declspec( dllexport )
extern "C" VOID    WINAPI DXGIDumpJournal();
extern "C" HRESULT WINAPI CreateDXGIFactory(REFIID riid, void **factory);
extern "C" HRESULT WINAPI DXGID3D10CreateDevice(HMODULE d3d10core, IDXGIFactory *factory, IDXGIAdapter *adapter, UINT flags, DWORD arg5, void **ppDevice);
extern "C" HRESULT WINAPI DXGID3D10CreateLayeredDevice(HMODULE d3d10core, IDXGIFactory *factory, IDXGIAdapter *adapter, UINT flags, DWORD arg5, void **ppDevice);
extern "C" DWORD WINAPI DXGID3D10GetLayeredDeviceSize(void *pDevice, DWORD dwUnknown);
extern "C" HRESULT WINAPI DXGID3D10RegisterLayers(struct dxgi_device_layer *pLayers, UINT uiNumLayers);

extern "C" VOID    WINAPI DXGIDumpJournal()
{

}

extern "C" HRESULT WINAPI CreateDXGIFactory(REFIID riid, void **factory)
{
  HRESULT hResult = S_FALSE;
  FARPROC __D3D10ProcAddress = NULL;

  TEST

  if(__DXGI != NULL)
  {
    if( (__D3D10ProcAddress = GetProcAddress(__DXGI, "CreateDXGIFactory")) != NULL)
    {
      hResult = ((HRESULT (*)(REFIID, void **))__D3D10ProcAddress)(riid, factory);
      //sprintf_s(acStr, sizeof(acStr), "D3D_MOD=0x%x, _sys_Direct3DCreate9 = 0x%x, m_pD3D = 0x%x", 
      //          (UINT)_hd3d9Mdl, (UINT)_sys_Direct3DCreate9, (UINT)m_pD3D);
      //__AAVDEBUG::OUTPUT(acStr);
    }
  }
  return hResult;
}

extern "C" HRESULT WINAPI DXGID3D10CreateDevice(HMODULE d3d10core, IDXGIFactory *factory, IDXGIAdapter *adapter, UINT flags, DWORD arg5, void **ppDevice)
{
  HRESULT hResult = S_FALSE;
  FARPROC __D3D10ProcAddress = NULL;

  TEST

  if(__DXGI != NULL)
  {
    if( (__D3D10ProcAddress = GetProcAddress(__DXGI, "DXGID3D10CreateDevice")) != NULL)
    {
      hResult = ((HRESULT (*)(HMODULE , IDXGIFactory *, IDXGIAdapter *, UINT , DWORD , void **))__D3D10ProcAddress)(d3d10core, factory, adapter, flags, arg5, ppDevice);
    }
  }
  return hResult;
}

extern "C" HRESULT WINAPI DXGID3D10CreateLayeredDevice(HMODULE d3d10core, IDXGIFactory *factory, IDXGIAdapter *adapter, UINT flags, DWORD arg5, void **ppDevice)
{
  HRESULT hResult = S_FALSE;
  FARPROC __D3D10ProcAddress = NULL;

  TEST

  if(__DXGI != NULL)
  {
    if( (__D3D10ProcAddress = GetProcAddress(__DXGI, "DXGID3D10CreateLayeredDevice")) != NULL)
    {
      hResult = ((HRESULT (*)(HMODULE , IDXGIFactory *, IDXGIAdapter *, UINT , DWORD , void **))__D3D10ProcAddress)(d3d10core, factory, adapter, flags, arg5, ppDevice);
      //sprintf_s(acStr, sizeof(acStr), "D3D_MOD=0x%x, _sys_Direct3DCreate9 = 0x%x, m_pD3D = 0x%x", 
      //          (UINT)_hd3d9Mdl, (UINT)_sys_Direct3DCreate9, (UINT)m_pD3D);
      //__AAVDEBUG::OUTPUT(acStr);
    }
  }
  return hResult;
}

extern "C" DWORD WINAPI DXGID3D10GetLayeredDeviceSize(void *pDevice, DWORD dwUnknown)
{
  DWORD dwRet = 0;
  FARPROC __D3D10ProcAddress = NULL;

  TEST

  if(__DXGI != NULL)
  {
    if( (__D3D10ProcAddress = GetProcAddress(__DXGI, "DXGID3D10GetLayeredDeviceSize")) != NULL)
    {
      dwRet = ((DWORD (*)(void *, DWORD))__D3D10ProcAddress)(pDevice, dwUnknown);
      //sprintf_s(acStr, sizeof(acStr), "D3D_MOD=0x%x, _sys_Direct3DCreate9 = 0x%x, m_pD3D = 0x%x", 
      //          (UINT)_hd3d9Mdl, (UINT)_sys_Direct3DCreate9, (UINT)m_pD3D);
      //__AAVDEBUG::OUTPUT(acStr);
    }
  }
  return dwRet;
}

//extern "C" HRESULT WINAPI DXGID3D10GetLayeredDeviceSize(void *pDevice)
//{
//  HRESULT hResult = S_FALSE;
//  char    acWinDir[MAX_PATH];
//  HMODULE __DXGI                  = NULL;
//  FARPROC __DXGID3D10GetLayeredDeviceSize = NULL;
//
//  GetWindowsDirectory(&acWinDir[0], sizeof(acWinDir));
//  strcat_s(&acWinDir[0], sizeof(acWinDir), "\\System32\\DXGI.dll"); 
//  if( (__DXGI = LoadLibrary(&acWinDir[0])) != NULL)
//  {
//    if( (__DXGID3D10GetLayeredDeviceSize = GetProcAddress(__DXGI, "DXGID3D10GetLayeredDeviceSize")) != NULL)
//    {
//      hResult = ((HRESULT (*)(void *, DWORD *))__DXGID3D10GetLayeredDeviceSize)(pDevice, dwSize);
//      //sprintf_s(acStr, sizeof(acStr), "D3D_MOD=0x%x, _sys_Direct3DCreate9 = 0x%x, m_pD3D = 0x%x", 
//      //          (UINT)_hd3d9Mdl, (UINT)_sys_Direct3DCreate9, (UINT)m_pD3D);
//      //__AAVDEBUG::OUTPUT(acStr);
//    }
//  }
//  return hResult;
//}


extern "C" HRESULT WINAPI DXGID3D10RegisterLayers(struct dxgi_device_layer *pLayers, UINT uiNumLayers)
{
  HRESULT hResult = S_FALSE;
  FARPROC __DXGID3D10RegisterLayers = NULL;

  TEST

  if(__DXGI != NULL)
  {
    if( (__DXGID3D10RegisterLayers = GetProcAddress(__DXGI, "DXGID3D10RegisterLayers")) != NULL)
    {
      hResult = ((HRESULT (*)(struct dxgi_device_layer *, UINT))__DXGID3D10RegisterLayers)(pLayers, uiNumLayers);
      //sprintf_s(acStr, sizeof(acStr), "D3D_MOD=0x%x, _sys_Direct3DCreate9 = 0x%x, m_pD3D = 0x%x", 
      //          (UINT)_hd3d9Mdl, (UINT)_sys_Direct3DCreate9, (UINT)m_pD3D);
      //__AAVDEBUG::OUTPUT(acStr);
    }
  }
  return hResult;
}



BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpvReserved)
{
  HRESULT hResult;

  switch(fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      ////////////////////////////////////////////////////////////////
      //// CREATE SHARED MAP
      ////////////////////////////////////////////////////////////////
      //g_hMapObject = CreateFileMapping( INVALID_HANDLE_VALUE,                // use paging file
      //                                  NULL,                                // default security attributes
      //                                  PAGE_READWRITE,                      // read/write access
      //                                  0,                                   // size: high 32-bits
      //                                  sizeof(__AAVAvionics::SHARED_DATA),       // size: low 32-bits
      //                                  TEXT(__AAVAvionics::g_pcSharedDataGuid)); // name of map object
      //AAV_ASSERT(g_hMapObject != NULL);
      ////////////////////////////////////////////////////////////////
      //// CLEAR SHARED DATA
      ////////////////////////////////////////////////////////////////
      //g_pkSharedData = (__AAVAvionics::SHARED_DATA *) MapViewOfFile(g_hMapObject, // handle to map object
      //                                                         FILE_MAP_ALL_ACCESS,  // read/write permission
      //                                                         0,
      //                                                         0,
      //                                                         sizeof(__AAVAvionics::SHARED_DATA));
      //AAV_ASSERT(g_pkSharedData != NULL);
      //memset(g_pkSharedData, 0, sizeof(__AAVAvionics::SHARED_DATA));

      ////////////////////////////////////////////////////////////////
      //// GET ADDRESS OF POINTER TO __AAVAvionics INSTANCE
      ////////////////////////////////////////////////////////////////
      //m_pkAvionics = &g_pkSharedData->pkAvionics;

      //__AAVDEBUG::OUTPUT("D3D9::CREATED SHARED MAP");
      break;

    case DLL_PROCESS_DETACH:
      //hResult = UnmapViewOfFile(g_pkSharedData);
      //AAV_ASSERT(SUCCEEDED(hResult));
      //g_pkSharedData = NULL;
      //hResult = CloseHandle(g_hMapObject); 
      //AAV_ASSERT(SUCCEEDED(hResult));
      //g_hMapObject = NULL;
      //__AAVDEBUG::OUTPUT("D3D9::DELETED SHARED MAP");
      break;

    default:
      break;
  }
  return TRUE;
}
