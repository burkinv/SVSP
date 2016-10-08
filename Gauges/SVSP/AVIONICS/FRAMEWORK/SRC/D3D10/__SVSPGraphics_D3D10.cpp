#include "__AAVGraphics_D3D10.h"

__AAVGraphics_D3D10::__AAVGraphics_D3D10(UINT uiWidth, UINT uiHeight) : __AAVGraphics()
{
  m_uiDisplayBufferIndex = 0;
  m_uiRenderBufferIndex  = m_uiDisplayBufferIndex + MIN_SWAP_BUFFER_OFFSET;

  /////////////////////////////////////////////////////////////////////
  // CREATE D3D DEVICE
  /////////////////////////////////////////////////////////////////////
  UINT createDeviceFlags = D3D10_CREATE_DEVICE_DEBUG;

  m_hResult = D3D10CreateDevice( NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, D3D10_SDK_VERSION, &m_pkDevice );
  //m_hResult = D3D10CreateDevice1( NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, D3D10_FEATURE_LEVEL_10_0, D3D10_1_SDK_VERSION, &m_pkDevice );
  AAV_ASSERT_RETURN_VOID( m_pkDevice != NULL );

  /////////////////////////////////////////////////////////////////////
  // SETUP CUSTOM SHADERS
  /////////////////////////////////////////////////////////////////////
  DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG;
  m_hResult = D3DX10CreateEffectFromFile("ShadersHLSL/TechDemo/__AAVGeometryObject_D3D9.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0, m_pkDevice, NULL, NULL, &m_pkEffect, NULL, NULL );
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_pkEffect != NULL) );

  /////////////////////////////////////////////////////////////////////
  // GET TECHNIQUE
  /////////////////////////////////////////////////////////////////////
  m_pkTechnique  = m_pkEffect->GetTechniqueByName( "Render" );
  AAV_ASSERT_RETURN_VOID(m_pkTechnique != NULL);

  /////////////////////////////////////////////////////////////////////
  // CLEAR ALL RENDER TARGETS
  /////////////////////////////////////////////////////////////////////
  memset(&m_apkGPURenderTextureMS[0], 0, sizeof(m_apkGPURenderTextureMS));
  memset(&m_apkGPURenderTexture[0],   0, sizeof(m_apkGPURenderTexture));
  memset(&m_apkCPURenderTexture[0],   0, sizeof(m_apkCPURenderTexture));

  for(UINT i = 0; i < __AAVGraphics_D3D10::MAX_NUM_SWAP_BUFFERS; ++i)
  {
    /////////////////////////////////////////////////////////////////////
    // CREATE MULTISAMPLE GPU RENDER TARGET
    /////////////////////////////////////////////////////////////////////
    D3D10_TEXTURE2D_DESC kGPURenderTextureMSDesc;
    ZeroMemory( &kGPURenderTextureMSDesc, sizeof( D3D10_TEXTURE2D_DESC ) );
    kGPURenderTextureMSDesc.Width                    = uiWidth;
    kGPURenderTextureMSDesc.Height                   = uiHeight;
    kGPURenderTextureMSDesc.MipLevels                = 1;
    kGPURenderTextureMSDesc.ArraySize                = 1;
    kGPURenderTextureMSDesc.Format                   = DXGI_FORMAT_R8G8B8A8_UNORM; //DXGI_FORMAT_B5G5R5A1_UNORM;
    kGPURenderTextureMSDesc.SampleDesc.Count         = 8;//1;
    kGPURenderTextureMSDesc.SampleDesc.Quality       = 16;//0;
    kGPURenderTextureMSDesc.Usage                    = D3D10_USAGE_DEFAULT; //D3D10_USAGE_STAGING;
    kGPURenderTextureMSDesc.BindFlags                = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    kGPURenderTextureMSDesc.CPUAccessFlags           = 0;
    kGPURenderTextureMSDesc.MiscFlags                = 0;

    m_hResult = m_pkDevice->CreateTexture2D(&kGPURenderTextureMSDesc, NULL, &m_apkGPURenderTextureMS[i]);
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_apkGPURenderTextureMS[i] != NULL));

    /////////////////////////////////////////////////////////////////////
    // CREATE DOWNSAMPLE GPU RENDER TARGET
    /////////////////////////////////////////////////////////////////////
    D3D10_TEXTURE2D_DESC kGPURenderTextureDesc;
    ZeroMemory( &kGPURenderTextureDesc, sizeof( D3D10_TEXTURE2D_DESC ) );
    kGPURenderTextureDesc.Width                    = uiWidth;
    kGPURenderTextureDesc.Height                   = uiHeight;
    kGPURenderTextureDesc.MipLevels                = 1;
    kGPURenderTextureDesc.ArraySize                = 1;
    kGPURenderTextureDesc.Format                   = DXGI_FORMAT_R8G8B8A8_UNORM; //DXGI_FORMAT_B5G5R5A1_UNORM;
    kGPURenderTextureDesc.SampleDesc.Count         = 1;
    kGPURenderTextureDesc.SampleDesc.Quality       = 0;
    kGPURenderTextureDesc.Usage                    = D3D10_USAGE_DEFAULT;
    kGPURenderTextureDesc.BindFlags                = 0;
    kGPURenderTextureDesc.CPUAccessFlags           = 0;
    kGPURenderTextureDesc.MiscFlags                = 0;

    m_hResult = m_pkDevice->CreateTexture2D(&kGPURenderTextureDesc, NULL, &m_apkGPURenderTexture[i]);
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_apkGPURenderTexture[i] != NULL));

    /////////////////////////////////////////////////////////////////////
    // CREATE CPU READ RESOURCE
    /////////////////////////////////////////////////////////////////////
    D3D10_TEXTURE2D_DESC kCPURenderTextureDesc;
    ZeroMemory( &kCPURenderTextureDesc, sizeof( D3D10_TEXTURE2D_DESC ) );
    kCPURenderTextureDesc.Width                    = uiWidth;
    kCPURenderTextureDesc.Height                   = uiHeight;
    kCPURenderTextureDesc.MipLevels                = 1;
    kCPURenderTextureDesc.ArraySize                = 1;
    kCPURenderTextureDesc.Format                   = DXGI_FORMAT_R8G8B8A8_UNORM; //DXGI_FORMAT_B5G5R5A1_UNORM;
    kCPURenderTextureDesc.SampleDesc.Count         = 1;
    kCPURenderTextureDesc.SampleDesc.Quality       = 0;
    kCPURenderTextureDesc.Usage                    = D3D10_USAGE_STAGING;
    kCPURenderTextureDesc.BindFlags                = 0;
    kCPURenderTextureDesc.CPUAccessFlags           = D3D10_CPU_ACCESS_READ;
    kCPURenderTextureDesc.MiscFlags                = 0;

    m_hResult = m_pkDevice->CreateTexture2D(&kCPURenderTextureDesc, NULL, &m_apkCPURenderTexture[i]);
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_apkCPURenderTexture[i] != NULL));

    /////////////////////////////////////////////////////////////////////
    // CREATE AND SET RENDER TARGET VIEW
    /////////////////////////////////////////////////////////////////////
    m_hResult = m_pkDevice->CreateRenderTargetView(m_apkGPURenderTextureMS[i], NULL, &m_apkRenderTargetView[i]);
    AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_apkRenderTargetView[i] != NULL));
  }

#ifdef USE_DEPTH_STENCIL
  /////////////////////////////////////////////////////////////////////
  // CREATE DEPTH STENCIL VIEW
  /////////////////////////////////////////////////////////////////////
  D3D10_TEXTURE2D_DESC descDepth;
  descDepth.Width              = uiWidth;
  descDepth.Height             = uiHeight;
  descDepth.MipLevels          = 1;
  descDepth.ArraySize          = 1;
  descDepth.Format             = DXGI_FORMAT_D32_FLOAT;
  descDepth.SampleDesc.Count   = kGPURenderTextureMSDesc.SampleDesc.Count;
  descDepth.SampleDesc.Quality = kGPURenderTextureMSDesc.SampleDesc.Quality;
  descDepth.Usage              = D3D10_USAGE_DEFAULT;
  descDepth.BindFlags          = D3D10_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags     = 0;
  descDepth.MiscFlags          = 0;
  m_hResult = m_pkDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencilMS );
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_pDepthStencilMS != NULL));

  D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
  descDSV.Format               = descDepth.Format;
  descDSV.ViewDimension        = D3D10_DSV_DIMENSION_TEXTURE2DMS;
  descDSV.Texture2D.MipSlice   = 0;
  m_hResult = m_pkDevice->CreateDepthStencilView( m_pDepthStencilMS, &descDSV, &m_pDepthStencilView );
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_pDepthStencilView != NULL));
  m_pkDevice->OMSetRenderTargets( 1, &m_apkRenderTargetView, m_pDepthStencilView );
#endif //USE_DEPTH_STENCIL

  /////////////////////////////////////////////////////////////////////
  // CREATE INPUT VERTEX LAYOUT
  /////////////////////////////////////////////////////////////////////
  D3D10_INPUT_ELEMENT_DESC akVertexLayout[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
  };
  UINT uiNumElements = sizeof( akVertexLayout ) / sizeof( akVertexLayout[0] );
  D3D10_PASS_DESC PassDesc;
  m_pkTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
  m_hResult = m_pkDevice->CreateInputLayout(&akVertexLayout[0], uiNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pkVertexLayout );
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_pkVertexLayout != NULL));

  /////////////////////////////////////////////////////////////////////
  // CREATE INPUT VERTEX LAYOUT
  /////////////////////////////////////////////////////////////////////
  D3D10_INPUT_ELEMENT_DESC akTexVertexLayout[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0 },
  };
  uiNumElements = sizeof( akTexVertexLayout ) / sizeof( akTexVertexLayout[0] );
  m_pkTechnique->GetPassByIndex( 1 )->GetDesc( &PassDesc );
  m_hResult = m_pkDevice->CreateInputLayout(&akTexVertexLayout[0], uiNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pkTexVertexLayout );
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_pkTexVertexLayout != NULL));

  /////////////////////////////////////////////////////////////////////
  // SETUP VIEW MATRIX
  /////////////////////////////////////////////////////////////////////
  D3DXVECTOR3 Eye( 0.0f, 0.0f, -5.0f );
  D3DXVECTOR3 At( 0.0f, 0.0f, 0.0f );
  D3DXVECTOR3 Up( 0.0f, 1.0f, 0.0f );
  D3DXMatrixLookAtLH( &m_pkViewMatrix, &Eye, &At, &Up );
  m_pkViewVariable = m_pkEffect->GetVariableByName( "g_kViewMatrix" )->AsMatrix();
  AAV_ASSERT_RETURN_VOID(m_pkViewVariable != NULL);
  m_pkViewVariable->SetMatrix( ( float* )&m_pkViewMatrix );

  /////////////////////////////////////////////////////////////////////
  // SETUP PROJECTION MATRIX
  /////////////////////////////////////////////////////////////////////
  //D3DXMatrixPerspectiveFovLH( &m_pkProjectionMatrix, ( float )D3DX_PI * 0.5f, uiWidth / ( FLOAT )uiHeight, 0.1f, 100.0f );
  D3DXMatrixOrthoOffCenterLH( &m_pkProjectionMatrix, -( float )uiWidth/2.0f, ( float )uiWidth/2.0f, -( float )uiHeight/2.0f, ( float )uiHeight/2.0f, 0.1f, 100.0f );
  m_pkProjectionVariable = m_pkEffect->GetVariableByName( "g_kProjectionMatrix" )->AsMatrix();
  AAV_ASSERT_RETURN_VOID(m_pkProjectionVariable != NULL);
  m_pkProjectionVariable->SetMatrix( ( float* )&m_pkProjectionMatrix );

  /////////////////////////////////////////////////////////////////////
  // SETUP WORLD MATRIX
  /////////////////////////////////////////////////////////////////////
  m_pkTransformVariable = m_pkEffect->GetVariableByName("g_kWorldMatrix")->AsMatrix();
  AAV_ASSERT_RETURN_VOID(m_pkTransformVariable != NULL);

  /////////////////////////////////////////////////////////////////////
  // SETUP SHADER RESOURCE
  /////////////////////////////////////////////////////////////////////
  m_pkDiffuseVariable = m_pkEffect->GetVariableByName( "g_txDiffuse" )->AsShaderResource();
  AAV_ASSERT_RETURN_VOID(m_pkDiffuseVariable != NULL);

  /////////////////////////////////////////////////////////////////////
  // SETUP THE VIEWPORT
  /////////////////////////////////////////////////////////////////////
  D3D10_VIEWPORT vp;
  vp.Width    = uiWidth;
  vp.Height   = uiHeight;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_pkDevice->RSSetViewports( 1, &vp );

  /////////////////////////////////////////////////////////////////////
  // SETUP RASTERIZER STATE
  /////////////////////////////////////////////////////////////////////
  D3D10_RASTERIZER_DESC rsd;
  rsd.FillMode              = D3D10_FILL_SOLID;
  rsd.CullMode              = D3D10_CULL_NONE;//D3D10_CULL_FRONT;
  rsd.FrontCounterClockwise = true;
  rsd.DepthBias             = false;
  rsd.DepthBiasClamp        = 0;
  rsd.SlopeScaledDepthBias  = 0;
  rsd.DepthClipEnable       = false;
  rsd.ScissorEnable         = false;
  rsd.MultisampleEnable     = true;
  rsd.AntialiasedLineEnable = true;
  m_pkDevice->CreateRasterizerState(&rsd, &m_pkRasterState);
  m_pkDevice->RSSetState(m_pkRasterState);
}

__AAVGraphics_D3D10::~__AAVGraphics_D3D10()
{
}

HRESULT __AAVGraphics_D3D10::LoadTextureFromFile(LPCSTR swFilename, __AAVTexture **ppkTexture)
{
  return D3DX10CreateShaderResourceViewFromFile(m_pkDevice, swFilename, NULL, NULL, (ID3D10ShaderResourceView **)ppkTexture, NULL );
}

VOID __AAVGraphics_D3D10::Clear(DWORD dwColor)
{
  /////////////////////////////////////////////////////////////////
  //CLEAR GAUGE
  /////////////////////////////////////////////////////////////////
  FLOAT afClearColor[4] = { (FLOAT)((dwColor & 0x000000FF)      )/255.0f, 
                            (FLOAT)((dwColor & 0x0000FF00) >> 8 )/255.0f, 
                            (FLOAT)((dwColor & 0x00FF0000) >> 16)/255.0f, 
                            (FLOAT)((dwColor & 0xFF000000) >> 24)/255.0f }; // red,green,blue,alpha
  m_pkDevice->ClearRenderTargetView( m_apkRenderTargetView[m_uiRenderBufferIndex], afClearColor );
#ifdef USE_DEPTH_STENCIL
  m_pkDevice->ClearDepthStencilView( m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );
#endif //USE_DEPTH_STENCIL
}

VOID __AAVGraphics_D3D10::Begin()
{
  /////////////////////////////////////////////////////////////////
  //SET SHADERS
  /////////////////////////////////////////////////////////////////
  //m_pkTechnique->GetPassByIndex( 0 )->Apply( 0 );

  /////////////////////////////////////////////////////////////////////
  // SET RENDER TARGET VIEW
  /////////////////////////////////////////////////////////////////////
  m_pkDevice->OMSetRenderTargets( 1, &m_apkRenderTargetView[m_uiRenderBufferIndex], NULL );
}

VOID __AAVGraphics_D3D10::End(PIMAGE  pImage)
{
  m_pkDevice->ResolveSubresource(m_apkGPURenderTexture[m_uiRenderBufferIndex], D3D10CalcSubresource(0, 0, 1), m_apkGPURenderTextureMS[m_uiRenderBufferIndex], D3D10CalcSubresource(0, 0, 1), DXGI_FORMAT_R8G8B8A8_UNORM);
  m_pkDevice->CopyResource(m_apkCPURenderTexture[m_uiRenderBufferIndex], m_apkGPURenderTexture[m_uiRenderBufferIndex]);
  D3D10_MAPPED_TEXTURE2D kMappedTexture;
  m_hResult = m_apkCPURenderTexture[m_uiDisplayBufferIndex]->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_READ , 0, &kMappedTexture);

  if( SUCCEEDED( m_hResult ) )
  {
    UINT*   pTexels     = (UINT *)kMappedTexture.pData;

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

    m_apkCPURenderTexture[m_uiDisplayBufferIndex]->Unmap(D3D10CalcSubresource(0, 0, 1));

    ///////////////////////////////////////////////////////////////
    // SWAP BUFFERS
    ///////////////////////////////////////////////////////////////
    m_uiRenderBufferIndex  = (m_uiRenderBufferIndex < (__AAVGraphics_D3D10::MAX_NUM_SWAP_BUFFERS - 1)) ? m_uiRenderBufferIndex + 1 : 0;
    m_uiDisplayBufferIndex = (m_uiDisplayBufferIndex < (__AAVGraphics_D3D10::MAX_NUM_SWAP_BUFFERS - 1)) ? m_uiDisplayBufferIndex + 1 : 0;
  }
}
