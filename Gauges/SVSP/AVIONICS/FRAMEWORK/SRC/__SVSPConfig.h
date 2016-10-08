#ifndef __SVSP_CONFIG_H__
#define __SVSP_CONFIG_H__

#if   defined( D3D9 )
#include <WINDOWS.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "__SVSPTypes.h"
#include "__SVSPGeometryObject_D3D9.h"
#include "__SVSPTextureObject_D3D9.h"
#include "__SVSPTextureRenderer_D3D9.h"
#include "__SVSPFont_D3D9.h"

typedef   IDirect3DDevice9                           D3DXDEVICE;
typedef   IDirect3DDevice9*                          LPD3DXDEVICE;
typedef   IDirect3DVertexBuffer9                     D3DXVERTEXBUFFER;
typedef   IDirect3DVertexBuffer9*                    LPD3DXVERTEXBUFFER;
typedef   IDirect3DTexture9                          D3DXTEXTURE;
typedef   IDirect3DTexture9*                         LPD3DXTEXTURE;
typedef   IDirect3DTexture9**                        LPPD3DXTEXTURE;
typedef   IDirect3DSurface9                          D3DXSURFACE;
typedef   IDirect3DSurface9*                         LPD3DXSURFACE;
typedef   IDirect3DSurface9**                        LPPD3DXSURFACE;
//typedef __SVSPGraphics_D3D9                         D3DXGRAPHICS;
//typedef __SVSPGraphics_D3D9*                        LPD3DXGRAPHICS;
typedef __SVSPGeometryObject_D3D9<__SVSPVertex>        D3DXGEOMETRYOBJECT;
typedef __SVSPGeometryObject_D3D9<__SVSPVertex>*       LPD3DXGEOMETRYOBJECT;
typedef __SVSPGeometryObject_D3D9<__SVSPVertex>**      LPPD3DXGEOMETRYOBJECT;
//typedef __SVSPGeometryObject_D3D9<__SVSPVertex>      D3DXTEXGEOMETRYOBJECT;
//typedef __SVSPGeometryObject_D3D9<__SVSPVertex>*     LPD3DXTEXGEOMETRYOBJECT;
//typedef __SVSPGeometryObject_D3D9<__SVSPVertex>**    LPPD3DXTEXGEOMETRYOBJECT;
typedef __SVSPTextureObject_D3D9                      D3DXTEXTUREOBJECT;
typedef __SVSPTextureObject_D3D9*                     LPD3DXTEXTUREOBJECT;
typedef __SVSPTextureRenderer_D3D9                    D3DXTEXTURERENDERER;
typedef __SVSPTextureRenderer_D3D9*                   LPD3DXTEXTURERENDERER;
typedef __SVSPTextureObject_D3D9**                    LPPD3DXTEXTUREOBJECT;
typedef __SVSPFont_D3D9                               D3DXFONTCUSTOM;
typedef __SVSPFont_D3D9*                              LPD3DXFONTCUSTOM;

#elif defined( D3D10 )
#include <WINDOWS.h>
#include <d3d10.h>
#include <d3dx10.h>
#include "__SVSPTypes.h"
#include "__SVSPGeometryObject_D3D10.h"
#include "__SVSPFont_D3D10.h"

typedef __SVSPGraphics_D3D10                          D3DXGRAPHICS;
typedef __SVSPGraphics_D3D10*                         LPD3DXGRAPHICS;
typedef __SVSPGeometryObject_D3D10<__SVSPVertex>       D3DXGEOMETRYOBJECT;
typedef __SVSPGeometryObject_D3D10<__SVSPVertex>*      LPD3DXGEOMETRYOBJECT;
typedef __SVSPGeometryObject_D3D10<__SVSPVertex>**     LPPD3DXGEOMETRYOBJECT;
typedef __SVSPGeometryObject_D3D10<__SVSPVertex>       D3DXTEXGEOMETRYOBJECT;
typedef __SVSPGeometryObject_D3D10<__SVSPVertex>*      LPD3DXTEXGEOMETRYOBJECT;
typedef __SVSPGeometryObject_D3D10<__SVSPVertex>**     LPPD3DXTEXGEOMETRYOBJECT;
typedef __SVSPTextureObject_D3D10                     D3DXTEXTUREOBJECT;
typedef __SVSPTextureObject_D3D10*                    LPD3DXTEXTUREOBJECT;
typedef __SVSPTextureRenderer_D3D10                   D3DXTEXTURERENDERER;
typedef __SVSPTextureRenderer_D3D10*                  LPD3DXTEXTURERENDERER;
typedef __SVSPFont_D3D10                              D3DXFONTCUSTOM;
typedef __SVSPFont_D3D10*                             LPD3DXFONTCUSTOM;

#elif defined( OPENGL )
#include <WINDOWS.h>
#include "__SVSPTypes.h"
#include "__SVSPGeometryObject_OPENGL.h"
#include "__SVSPFont_OPENGL.h"

typedef __SVSPGraphics_OPENGL                         D3DXGRAPHICS;
typedef __SVSPGraphics_OPENGL*                        LPD3DXGRAPHICS;
typedef __SVSPGeometryObject_OPENGL<__SVSPVertex>      D3DXGEOMETRYOBJECT;
typedef __SVSPGeometryObject_OPENGL<__SVSPVertex>*     LPD3DXGEOMETRYOBJECT;
typedef __SVSPGeometryObject_OPENGL<__SVSPVertex>**    LPPD3DXGEOMETRYOBJECT;
typedef __SVSPTextureObject_OPENGL                    D3DXTEXTUREOBJECT;
typedef __SVSPTextureObject_OPENGL*                   LPD3DXTEXTUREOBJECT;
typedef __SVSPTextureRenderer_OPENGL                  D3DXTEXTURERENDERER;
typedef __SVSPTextureRenderer_OPENGL*                 LPD3DXTEXTURERENDERER;
typedef __SVSPFont_OPENGL                             D3DXFONTCUSTOM;
typedef __SVSPFont_OPENGL*                            LPD3DXFONTCUSTOM;
#endif

#endif	/* __SVSP_CONFIG_H__ */


