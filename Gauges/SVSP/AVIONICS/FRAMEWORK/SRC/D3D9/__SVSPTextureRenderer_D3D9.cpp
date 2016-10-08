#include "__SVSPGraphics_D3D9.h"
#include "__SVSPDebug.h"
#include <__SVSPTextureRenderer_D3D9.h>
#include <__SVSPGeometryObject_D3D9.h>
#include <__SVSPD3D9.h>
#include <GAUGES.h>

__SVSPTextureRenderer_D3D9::__SVSPTextureRenderer_D3D9(UINT uiWidth, UINT uiHeight)
{
  m_uiWidth  = uiWidth;
  m_uiHeight = uiHeight;

  m_uiDisplayBufferIndex = 0;
  m_uiRenderBufferIndex  = m_uiDisplayBufferIndex + MIN_SWAP_BUFFER_OFFSET;

  memset(&m_apkGPURenderTextureMS[0], 0, sizeof(m_apkGPURenderTextureMS));
  memset(&m_apkGPURenderTextureDS[0], 0, sizeof(m_apkGPURenderTextureDS));
  //memset(&m_apkCPURenderTexture[0],   0, sizeof(m_apkCPURenderTexture));
  m_pkSavedRenderTarget = NULL;

  /////////////////////////////////////////////////////////////////////
  // QUERY MULTISAMPLING CAPABILITY
  /////////////////////////////////////////////////////////////////////
  m_eFormat          = D3DFMT_X8R8G8B8;
  m_eMultisampleType = D3DMULTISAMPLE_8_SAMPLES;
  m_dwQualityLevels  = 3;

  //IDirect3D9 *pkD3D9;
  //pkDevice->GetDirect3D(&pkD3D9);
  //pkD3D9->CheckDeviceMultiSampleType(0, 
  //                                     D3DDEVTYPE_HAL, 
  //                                     m_eFormat, 
  //                                     true, 
  //                                     m_eMultisampleType, 
  //                                     &m_dwQualityLevels);

  //m_eFormat          = D3DFMT_X8R8G8B8;
  //m_eMultisampleType = D3DMULTISAMPLE_8_SAMPLES;
  //m_dwQualityLevels  = 3;


  m_bMultisamplingEnabled = false;



  /////////////////////////////////////////////////////////////////////
  // SETUP PROJECTION MATRIX
  /////////////////////////////////////////////////////////////////////
  //D3DXMatrixPerspectiveFovLH( &m_pkProjectionMatrix, ( float )D3DX_PI * 0.5f, m_uiWidth / ( FLOAT )m_uiHeight, 0.1f, 100.0f );
  //D3DXMatrixOrthoOffCenterLH( &m_kProjectionMatrix, -( float )m_uiWidth/2.0f, ( float )m_uiWidth/2.0f, -( float )m_uiHeight/2.0f, ( float )m_uiHeight/2.0f, 0.1f, 100.0f );
  D3DXMatrixOrthoOffCenterLH( &m_kProjectionMatrix, -( float )m_uiWidth/2.0f, ( float )m_uiWidth/2.0f, ( float )m_uiHeight/2.0f, -( float )m_uiHeight/2.0f, -100000.0f, 100000.0f );
  //D3DXMatrixOrthoOffCenterLH( &m_kProjectionMatrix, -( float )m_uiWidth/2.0f, ( float )m_uiWidth/2.0f, ( float )m_uiHeight/2.0f, -( float )m_uiHeight/2, 0.1f, 100.0f );

  /////////////////////////////////////////////////////////////////////
  // SETUP VIEW MATRIX
  /////////////////////////////////////////////////////////////////////
  D3DXVECTOR3 Eye( 0.0f, 0.0f, -1.0f );
  D3DXVECTOR3 At( 0.0f, 0.0f, 0.0f );
  D3DXVECTOR3 Up( 0.0f,  1.0f, 0.0f );
  D3DXMatrixLookAtLH( &m_kViewMatrix, &Eye, &At, &Up );

  /////////////////////////////////////////////////////////////////////
  // SETUP VIEWPORT
  /////////////////////////////////////////////////////////////////////
  m_kViewport.Width    = m_uiWidth;
  m_kViewport.Height   = m_uiHeight;
  m_kViewport.MinZ = 0.0f;
  m_kViewport.MaxZ = 1.0f;
  m_kViewport.X = 0;
  m_kViewport.Y = 0;
}

__SVSPTextureRenderer_D3D9::~__SVSPTextureRenderer_D3D9()
{
  Release();
}

VOID __SVSPTextureRenderer_D3D9::Release()
{
  /////////////////////////////////////////////////////////////////////
  // RELEASE ALL RENDER TARGETS
  /////////////////////////////////////////////////////////////////////
  for(UINT i = 0; i < __SVSPTextureRenderer_D3D9::MAX_NUM_SWAP_BUFFERS; ++i)
  {
    if(m_apkGPURenderTextureMS[i] != NULL) m_apkGPURenderTextureMS[i]->Release();
    if(m_apkGPURenderTextureDS[i] != NULL) m_apkGPURenderTextureDS[i]->Release();
    //if(m_apkCPURenderTexture[i]   != NULL) m_apkCPURenderTexture[i]->Release();
  }
  if(m_pkSavedRenderTarget != NULL) m_pkSavedRenderTarget->Release();

  memset(&m_apkGPURenderTextureMS[0], 0, sizeof(m_apkGPURenderTextureMS));
  memset(&m_apkGPURenderTextureDS[0], 0, sizeof(m_apkGPURenderTextureDS));
  //memset(&m_apkCPURenderTexture[0],   0, sizeof(m_apkCPURenderTexture));
  m_pkSavedRenderTarget = NULL;

}

HRESULT __SVSPTextureRenderer_D3D9::Reset(LPD3DXDEVICE pkDevice)
{
  IDirect3D9 *pkD3D9;
  pkDevice->GetDirect3D(&pkD3D9);
  m_hResult = pkD3D9->CheckDeviceMultiSampleType(0, 
                                                 D3DDEVTYPE_HAL, 
                                                 m_eFormat, 
                                                 true, 
                                                 m_eMultisampleType, 
                                                 &m_dwQualityLevels);
  AAV_ASSERT_RETURN(SUCCEEDED(m_hResult) && (m_dwQualityLevels >= 1), S_FALSE);


  for(UINT i = 0; i < __SVSPTextureRenderer_D3D9::MAX_NUM_SWAP_BUFFERS; ++i)
  {
    /////////////////////////////////////////////////////////////////////
    // CREATE MULTISAMPLE GPU RENDER TARGET
    /////////////////////////////////////////////////////////////////////
    m_hResult = pkDevice->CreateRenderTarget(m_uiWidth,
                                               m_uiHeight,
                                               m_eFormat,
                                               m_eMultisampleType,
                                               m_dwQualityLevels - 1,
                                               FALSE,
                                               &m_apkGPURenderTextureMS[i],
                                               NULL);
    AAV_ASSERT_RETURN(SUCCEEDED(m_hResult) && (m_apkGPURenderTextureMS[i] != NULL), S_FALSE);

    /////////////////////////////////////////////////////////////////////
    // CREATE DOWNSAMPLE GPU RENDER TARGET
    /////////////////////////////////////////////////////////////////////
    //m_hResult = pkDevice->CreateRenderTarget(m_uiWidth,
    //                                           m_uiHeight,
    //                                           m_eFormat,
    //                                           D3DMULTISAMPLE_NONE,
    //                                           0,
    //                                           FALSE,
    //                                           &m_apkGPURenderTexture[i],
    //                                           NULL);
    //AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_apkGPURenderTexture[i] != NULL));
    m_hResult = pkDevice->CreateTexture(m_uiWidth,
                                          m_uiHeight,
                                          1,
                                          D3DUSAGE_RENDERTARGET,
                                          m_eFormat,
                                          D3DPOOL_DEFAULT ,
                                          &m_apkGPURenderTextureDS[i],
                                          NULL);
    AAV_ASSERT_RETURN(SUCCEEDED(m_hResult) && (m_apkGPURenderTextureDS[i] != NULL), S_FALSE);

    /////////////////////////////////////////////////////////////////////
    // CREATE CPU READ RESOURCE
    /////////////////////////////////////////////////////////////////////
    //m_hResult = pkDevice->CreateOffscreenPlainSurface(m_uiWidth,
    //                                                    m_uiHeight,
    //                                                    m_eFormat,
    //                                                    D3DPOOL_SYSTEMMEM,
    //                                                    &m_apkCPURenderTexture[i],
    //                                                    NULL);
    //AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_apkCPURenderTexture[i] != NULL));
  }

  return S_OK;
}

VOID __SVSPTextureRenderer_D3D9::Clear(LPD3DXDEVICE pkDevice, DWORD dwColor)
{
}

VOID __SVSPTextureRenderer_D3D9::Begin(LPD3DXDEVICE pkDevice)
{
  /////////////////////////////////////////////////////////////////////
  // SAVE CURRENT RENDER TARGET
  /////////////////////////////////////////////////////////////////////
  m_hResult = pkDevice->GetRenderTarget(0, &m_pkSavedRenderTarget);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  pkDevice->GetRenderState(D3DRS_ZENABLE,               &m_dwSavedZEnable);


  /////////////////////////////////////////////////////////////////////
  // SET RENDER TARGET
  /////////////////////////////////////////////////////////////////////
  //AAV_ASSERT_RETURN_VOID(m_apkGPURenderTextureMS[m_uiRenderBufferIndex] != NULL);
  //m_hResult = pkDevice->SetRenderTarget(0, m_apkGPURenderTextureMS[m_uiRenderBufferIndex]);
  //AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  IDirect3DSurface9 *pkRenderTarget = NULL;
  if(m_bMultisamplingEnabled)
  {
    AAV_ASSERT_RETURN_VOID(m_apkGPURenderTextureMS[m_uiRenderBufferIndex] != NULL);
    m_hResult = pkDevice->SetRenderTarget(0, m_apkGPURenderTextureMS[m_uiRenderBufferIndex]);
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));
    //pkDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
  }
  else
  {
    AAV_ASSERT_RETURN_VOID(m_apkGPURenderTextureDS[m_uiRenderBufferIndex] != NULL);
    m_hResult = m_apkGPURenderTextureDS[m_uiRenderBufferIndex]->GetSurfaceLevel(0, &pkRenderTarget);
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (pkRenderTarget != NULL));
    m_hResult = pkDevice->SetRenderTarget(0, pkRenderTarget);
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));
    //pkDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
  }
  /////////////////////////////////////////////////////////////////////
  // SET SHADERS
  /////////////////////////////////////////////////////////////////////
  pkDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

    pkDevice->SetRenderState(D3DRS_ZENABLE ,               D3DZB_FALSE);

  /////////////////////////////////////////////////////////////////////
  // SET PROJECTION
  /////////////////////////////////////////////////////////////////////
  pkDevice->SetTransform(D3DTS_PROJECTION, &m_kProjectionMatrix);

  /////////////////////////////////////////////////////////////////////
  // SET VIEW
  /////////////////////////////////////////////////////////////////////
  pkDevice->SetTransform(D3DTS_VIEW, &m_kViewMatrix);

  /////////////////////////////////////////////////////////////////////
  // SET VIEWPORT
  /////////////////////////////////////////////////////////////////////
  pkDevice->SetViewport(&m_kViewport);

  /////////////////////////////////////////////////////////////////////
  // CLEAR RENDER TARGET
  /////////////////////////////////////////////////////////////////////
  pkDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x0f, 0x0f, 0x0f), 1.0f, 0);

  if(pkRenderTarget != NULL)
  {
    pkRenderTarget->Release();
    pkRenderTarget = NULL;
  }
}

VOID __SVSPTextureRenderer_D3D9::End(LPD3DXDEVICE pkDevice)
{
  if(m_bMultisamplingEnabled)
  {
    IDirect3DSurface9 *pkSurface = NULL;
    AAV_ASSERT_RETURN_VOID(m_apkGPURenderTextureMS[m_uiRenderBufferIndex] != NULL);
    AAV_ASSERT_RETURN_VOID(m_apkGPURenderTextureDS[m_uiRenderBufferIndex] != NULL);
    m_hResult = m_apkGPURenderTextureDS[m_uiRenderBufferIndex]->GetSurfaceLevel(0, &pkSurface);
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (pkSurface != NULL));

    m_hResult = pkDevice->StretchRect(m_apkGPURenderTextureMS[m_uiRenderBufferIndex], NULL, pkSurface, NULL, D3DTEXF_POINT);
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

    pkSurface->Release();
    pkSurface = NULL;
  }


  //--HACK-- (DISABLED CODE)
  //AAV_ASSERT_RETURN_VOID(m_apkCPURenderTexture[m_uiRenderBufferIndex] != NULL);
  //m_hResult = pkDevice->GetRenderTargetData(m_apkGPURenderTexture[m_uiRenderBufferIndex], m_apkCPURenderTexture[m_uiRenderBufferIndex]);
  //AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  ///////////////////////////////////////////////////////////////
  // SWAP BUFFERS
  ///////////////////////////////////////////////////////////////
  m_uiRenderBufferIndex  = (m_uiRenderBufferIndex  < (__SVSPTextureRenderer_D3D9::MAX_NUM_SWAP_BUFFERS - 1)) ? m_uiRenderBufferIndex + 1 : 0;
  m_uiDisplayBufferIndex = (m_uiDisplayBufferIndex < (__SVSPTextureRenderer_D3D9::MAX_NUM_SWAP_BUFFERS - 1)) ? m_uiDisplayBufferIndex + 1 : 0;

  /////////////////////////////////////////////////////////////////////
  // RESTORE SAVED RENDER TARGET
  /////////////////////////////////////////////////////////////////////
  AAV_ASSERT_RETURN_VOID(m_pkSavedRenderTarget != NULL);
  m_hResult = pkDevice->SetRenderTarget(0, m_pkSavedRenderTarget);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  pkDevice->SetRenderState(D3DRS_ZENABLE,               m_dwSavedZEnable);


  m_pkSavedRenderTarget->Release();
  m_pkSavedRenderTarget = NULL;
}

IDirect3DTexture9* __SVSPTextureRenderer_D3D9::GetDisplayTexture()
{
  return m_apkGPURenderTextureDS[m_uiDisplayBufferIndex];
  //return m_pkTestFont->GetFontBitmapTexture();

}

VOID __SVSPTextureRenderer_D3D9::GetBackbufferData(PIMAGE pImage)
{
  return;

#if 0
  // --HACK-- (ENABLED CODE)
  //AAV_ASSERT_RETURN_VOID(m_apkCPURenderTexture[m_uiDisplayBufferIndex] != NULL);
  //m_hResult = pkDevice->GetRenderTargetData(m_apkGPURenderTexture[m_uiDisplayBufferIndex], m_apkCPURenderTexture[m_uiDisplayBufferIndex]);
  //AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  D3DLOCKED_RECT kLockedRect;
  AAV_ASSERT_RETURN_VOID(m_apkCPURenderTexture[m_uiDisplayBufferIndex] != NULL);
  m_hResult = m_apkCPURenderTexture[m_uiDisplayBufferIndex]->LockRect(&kLockedRect, NULL, D3DLOCK_READONLY);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  if( SUCCEEDED( m_hResult ) )
  {
    UINT* pTexels = (UINT *)kLockedRect.pBits;

    switch(pImage->format)
    {
    case IMG_15_BIT: //A1R5G5B5
      {
        USHORT *pImageData  = (USHORT *)pImage->image;
        UINT    uiNumPixels = pImage->len/sizeof(USHORT);
        ///////////////////////////////////////////////////////////////
        // TO BE OPIMIZED BY CUSTOM COPY FUNCTION (D3D10 PIXEL SHADER)
        ///////////////////////////////////////////////////////////////
        for(UINT i = 0; i < uiNumPixels; ++i)
        {
          UINT uiColor = *pTexels;
          //*pImageData =   (1 << 15) | (((uiColor & 0x000000F8) >> 3 ) << 10) | (((uiColor & 0x0000F800) >> 11) << 5) | ((uiColor & 0x00F80000) >> 19);
          *pImageData =   (0 << 15) | ((uiColor & 0x000000F8) << 7) | ((uiColor & 0x0000F800) >> 6) | ((uiColor & 0x00F80000) >> 19);
          pTexels++;
          pImageData++;
        }
      }
      break;

    case IMG_32_BIT: // R8G8B8
      {
        UINT *pImageData  = (UINT *)pImage->image;
        UINT  uiNumPixels = pImage->len/sizeof(UINT);
        memcpy_s(pImageData, pImage->len, pTexels, pImage->len);
        //for(UINT i = 0; i < uiNumPixels; ++i)
        //{
        //  UINT uiColor = *pTexels;
        //  //*pImageData = (uiColor & 0xFF000000) | ((uiColor & 0x000000FF) << 16) | (uiColor & 0x0000FF00) | ((uiColor & 0x00FF0000) >> 16);
        //  pTexels++;
        //  pImageData++;
        //}
      }
      break;

    case IMG_32A_BIT: // A8R8G8B8
      //for(UINT i = 0; i < uiNumPixels; ++i)
      //{
      //  UINT uiColor = *pTexels;
      //  //*pImageData =   (1 << 15) | (((uiColor & 0x000000F8) >> 3 ) << 10) | (((uiColor & 0x0000F800) >> 11) << 5) | ((uiColor & 0x00F80000) >> 19);
      //  *pImageData =   uiColor;
      //  pTexels++;
      //  pImageData++;
      //}
      break;
    }

    m_apkCPURenderTexture[m_uiDisplayBufferIndex]->UnlockRect();
  }
#endif //0
}



