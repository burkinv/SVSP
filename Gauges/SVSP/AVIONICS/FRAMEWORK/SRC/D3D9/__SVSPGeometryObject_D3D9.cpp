#include "__SVSPGeometryObject_D3D9.h"
#include "__SVSPDebug.h"

IDirect3DPixelShader9*  __SVSPGeometryObject_D3D9<__SVSPVertex>::g_pkDefaultPixelShader  = 0;
IDirect3DVertexShader9* __SVSPGeometryObject_D3D9<__SVSPVertex>::g_pkDefaultVertexShader = 0;
DWORD                   __SVSPGeometryObject_D3D9<__SVSPVertex>::g_dwDefaultFVF          = 0;
LPDIRECT3DSTATEBLOCK9   __SVSPGeometryObject_D3D9<__SVSPVertex>::g_pkStateBlock[__SVSPGeometryObject_D3D9<__SVSPVertex>::STATE_BLOCK_NUM_STATE_BLOCKS] = {NULL, NULL};