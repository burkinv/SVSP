#ifndef __SVSP_TYPES_H__
#define __SVSP_TYPES_H__

#if   defined( D3D9 )
#include <WINDOWS.h>
#include <d3d9.h>
#include <d3dx9.h>

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

#elif defined( D3D10 )
#include <WINDOWS.h>
#include <d3d10.h>
#include <d3dx10.h>

typedef   ID3D10Device                               D3DXDEVICE;
typedef   ID3D10Device*                              LPD3DXDEVICE;
typedef   ID3D10Buffer                               D3DXVERTEXBUFFER;
typedef   ID3D10Buffer*                              LPD3DXVERTEXBUFFER;
typedef   ID3D10Texture2D                            D3DXTEXTURE;
typedef   ID3D10Texture2D*                           LPD3DXTEXTURE;
typedef   ID3D10Texture2D**                          LPPD3DXTEXTURE;

#elif defined( OPENGL )
#include <WINDOWS.h>


#elif defined( GDIPLUS )

#endif



#endif	/* __SVSP_TYPES_H__ */


