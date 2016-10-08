#include <Windows.h>
#include "__AAVDebug.h"
#include "__AAVFont_D3D10.h"
#include "__AAVGraphics_D3D10.h"

__AAVFont_D3D10::__AAVFont_D3D10(__AAVGraphics_D3D10* pkGraphics, const TCHAR* sFontName, DWORD dwHeight, DWORD dwFlags) : __AAVFont(pkGraphics, sFontName, dwHeight, dwFlags)
{
  AAV_ASSERT_RETURN_VOID(pkGraphics != NULL);
  AAV_ASSERT_RETURN_VOID(pkGraphics->GetDevice() != NULL);

  /////////////////////////////////////////////////////////////////////
  // CREATE HDC
  /////////////////////////////////////////////////////////////////////
  HDC hDC = CreateCompatibleDC( NULL );
  SetMapMode( hDC, MM_TEXT );

  /////////////////////////////////////////////////////////////////////
  // CREATE GDI FONT
  /////////////////////////////////////////////////////////////////////
  INT nHeight    = -MulDiv( m_dwFontHeight, (INT)(GetDeviceCaps(hDC, LOGPIXELSY) * m_fTextScale), 72 );
  DWORD dwBold   = (m_dwFontFlags & __AAVFont_D3D10::BOLD)   ? FW_BOLD : FW_NORMAL;
  DWORD dwItalic = (m_dwFontFlags & __AAVFont_D3D10::ITALIC) ? TRUE    : FALSE;
  HFONT hFont    = CreateFont(nHeight, 0, 0, 0, dwBold, dwItalic,
                              FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                              VARIABLE_PITCH, sFontName );
  SelectObject( hDC, hFont );

  /////////////////////////////////////////////////////////////////////
  // Calculate the dimensions for the smallest power-of-two texture which
  // can hold all the printable characters
  /////////////////////////////////////////////////////////////////////
  m_dwFontBitmapTextureWidth  = 128;
  m_dwFontBitmapTextureHeight = 128;
  while(( m_hResult = PaintAlphabet( hDC, true ) ) == D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS)
  {
    m_dwFontBitmapTextureWidth  *= 2;
    m_dwFontBitmapTextureHeight *= 2;
  }


  /////////////////////////////////////////////////////////////////////
  // CREATE CPU RENDER RESOURCE
  /////////////////////////////////////////////////////////////////////
  D3D10_TEXTURE2D_DESC kCPURenderTextureDesc;
  ZeroMemory( &kCPURenderTextureDesc, sizeof( D3D10_TEXTURE2D_DESC ) );
  kCPURenderTextureDesc.Width                    = m_dwFontBitmapTextureWidth;
  kCPURenderTextureDesc.Height                   = m_dwFontBitmapTextureHeight;
  kCPURenderTextureDesc.MipLevels                = 1;
  kCPURenderTextureDesc.ArraySize                = 1;
  kCPURenderTextureDesc.Format                   = DXGI_FORMAT_R8G8B8A8_UNORM; 
  kCPURenderTextureDesc.SampleDesc.Count         = 1;
  kCPURenderTextureDesc.SampleDesc.Quality       = 0;
  kCPURenderTextureDesc.Usage                    = D3D10_USAGE_STAGING;
  kCPURenderTextureDesc.BindFlags                = 0;
  kCPURenderTextureDesc.CPUAccessFlags           = D3D10_CPU_ACCESS_WRITE;
  kCPURenderTextureDesc.MiscFlags                = 0;

  ID3D10Texture2D *pkCPURenderTexture = NULL;
  m_hResult = pkGraphics->GetDevice()->CreateTexture2D(&kCPURenderTextureDesc, NULL, &pkCPURenderTexture);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (pkCPURenderTexture != NULL));

  /////////////////////////////////////////////////////////////////////
  // CREATE SINGLE-SAMPLE GPU RENDER RESOURCE
  /////////////////////////////////////////////////////////////////////
  D3D10_TEXTURE2D_DESC kFontBitmapTextureDesc;
  ZeroMemory( &kFontBitmapTextureDesc, sizeof( D3D10_TEXTURE2D_DESC ) );
  kFontBitmapTextureDesc.Width                    = m_dwFontBitmapTextureWidth;
  kFontBitmapTextureDesc.Height                   = m_dwFontBitmapTextureHeight;
  kFontBitmapTextureDesc.MipLevels                = 1;
  kFontBitmapTextureDesc.ArraySize                = 1;
  kFontBitmapTextureDesc.Format                   = DXGI_FORMAT_R8G8B8A8_UNORM;
  kFontBitmapTextureDesc.SampleDesc.Count         = 1;
  kFontBitmapTextureDesc.SampleDesc.Quality       = 0;
  kFontBitmapTextureDesc.Usage                    = D3D10_USAGE_DEFAULT;
  kFontBitmapTextureDesc.BindFlags                = D3D10_BIND_SHADER_RESOURCE;
  kFontBitmapTextureDesc.CPUAccessFlags           = 0;
  kFontBitmapTextureDesc.MiscFlags                = 0;

  m_hResult = pkGraphics->GetDevice()->CreateTexture2D(&kFontBitmapTextureDesc, NULL, &m_pkFontBitmapTexture);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult) && (m_pkFontBitmapTexture != NULL));

  /////////////////////////////////////////////////////////////////////
  // CREATE FONT BITMAP
  /////////////////////////////////////////////////////////////////////
  DWORD*      pBitmapBits;
  BITMAPINFO bmi;
  ZeroMemory( &bmi.bmiHeader, sizeof(BITMAPINFOHEADER) );
  bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth       =  (int)m_dwFontBitmapTextureWidth;
  bmi.bmiHeader.biHeight      = -(int)m_dwFontBitmapTextureHeight;
  bmi.bmiHeader.biPlanes      = 1;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biBitCount    = 32;

  HBITMAP hBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS, (void**)&pBitmapBits, NULL, 0 );
  SelectObject( hDC, hBitmap );

  SetTextColor( hDC, RGB(255, 255, 255) );
  //SetTextColor( hDC, 0xFFFFFFFF );
  SetBkColor(   hDC, 0x00000000 );
  SetTextAlign( hDC, TA_TOP );

  m_hResult = PaintAlphabet( hDC, false );
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  /////////////////////////////////////////////////////////////////////
  // COPY FONT BITMAP TO CPU RESOURCE
  /////////////////////////////////////////////////////////////////////
  D3D10_MAPPED_TEXTURE2D kMappedTexture;
  m_hResult = pkCPURenderTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE , 0, &kMappedTexture);
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));
  
  DWORD* pTexels = (DWORD *)kMappedTexture.pData;
  DWORD dwNumPixels = m_dwFontBitmapTextureWidth*m_dwFontBitmapTextureHeight;
  for(UINT i = 0; i < dwNumPixels; ++i)
  {
     DWORD uiPixColor = *pBitmapBits;
    *pTexels = uiPixColor | ((uiPixColor & 0x0000FF00) << 16);
     pTexels++;
     pBitmapBits++;
  }
  //memset(pTexels, 0xFF00007F, dwNumPixels*sizeof(UINT));
  //memcpy_s(pTexels, dwNumPixels, pBitmapBits, dwNumPixels);
  pkCPURenderTexture->Unmap(D3D10CalcSubresource(0, 0, 1));

  /////////////////////////////////////////////////////////////////////
  // COPY CPU RESOURCE TO GPU RESOURCE
  /////////////////////////////////////////////////////////////////////
  pkGraphics->GetDevice()->CopyResource(m_pkFontBitmapTexture, pkCPURenderTexture);

  /////////////////////////////////////////////////////////////////////
  // CLEAN UP TEMPORARY RESOURCES
  /////////////////////////////////////////////////////////////////////
  DeleteObject( hBitmap );
  DeleteObject( hFont );
  DeleteDC( hDC );
  pkCPURenderTexture->Release();

  /////////////////////////////////////////////////////////////////////
  // TEXT QUAD OBJECT
  /////////////////////////////////////////////////////////////////////
  m_pkTextRect = new __AAVGeometryObject_D3D10<__AAVVertex>(pkGraphics, __AAVGeometryObject_D3D10<__AAVVertex>::TRIANGLELIST, __AAVFont_D3D10::NUM_TEXT_RECT_VERTICES);

  m_pkTextRect->SetTexture((LPD3DXTEXTURE)m_pkFontBitmapTexture);
}

__AAVFont_D3D10::~__AAVFont_D3D10() 
{
  Release();

  m_pkFontBitmapTexture->Release();
  m_pkFontBitmapTexture = NULL;
}

VOID __AAVFont_D3D10::SetTransform(struct D3DXMATRIX *pkTransformMatrix)
{
  ((__AAVGraphics_D3D10 *)m_pkGraphics)->m_pkTransformVariable->SetMatrix(( float* )pkTransformMatrix);
}

VOID __AAVFont_D3D10::Begin()
{
}

VOID __AAVFont_D3D10::End()
{
}

VOID __AAVFont_D3D10::Release()
{
  delete m_pkTextRect;
  m_pkTextRect = NULL;
}

HRESULT __AAVFont_D3D10::Reset()
{
  return S_OK;
}

VOID __AAVFont_D3D10::RenderText( const TCHAR* strText, struct D3DXVECTOR3 *v, DWORD dwFlags, FLOAT fScaling)
{
  FLOAT tx1;
  FLOAT ty1;
  FLOAT tx2;
  FLOAT ty2;
  FLOAT w;
  FLOAT h;

  AAV_ASSERT_RETURN_VOID(m_pkFontBitmapTexture != NULL);

 // //// Setup renderstate
 // //if(m_pStateBlockSaved)m_pStateBlockSaved->Capture();
 // //if(m_pStateBlockDrawText)m_pStateBlockDrawText->Apply();
 // //m_pd3dDevice->SetFVF( D3DFVF_FONT3DVERTEX );
 // //m_pd3dDevice->SetPixelShader( NULL );
 // //m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(FONT3DVERTEX) );

 // //// Set filter states
 // //if( dwFlags & __AAVFont::FILTERED )
 // //{
 // //  m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
 // //  m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
 // //  //m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
 // //  //m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );
 // //  m_pd3dDevice->SetSamplerState( 0, D3DSAMP_BORDERCOLOR , 0xFFFFFFFF );
 // //}

  // Position for each text element
  FLOAT x = v->x;
  FLOAT y = v->y;
  FLOAT z = v->z;

  FLOAT fLineSpacing = (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwFontBitmapTextureHeight;

  // Center the text block at the origin (not the viewport)
  SIZE sz;
  GetTextExtent( strText, dwFlags, &sz );

  if( dwFlags & __AAVFont::CENTERED_X )
  {
    x -= (((FLOAT)sz.cx)*fScaling)/2.0f;
  }
  else if( dwFlags & __AAVFont::ALIGN_RIGHT )
  {
    x -= (((FLOAT)sz.cx)*fScaling);
  }

  if( dwFlags & __AAVFont::CENTERED_Y )
  {
    if( dwFlags & __AAVFont::VERTICAL)
    {
      y += (((FLOAT)sz.cy)*fScaling)/2.0f - fLineSpacing;
    }
    else
    {
      y -= (((FLOAT)sz.cy)*fScaling)/2.0f;
    }
  }

  // Turn off culling for two-sided text
  //if( dwFlags & __AAVFont::TWOSIDED )
 // //  m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

  // Adjust for character spacing
  x -= m_dwSpacing *fScaling;
  FLOAT fStartX = x;
  TCHAR c;

 // // Fill vertex buffer
 // FONT3DVERTEX* pkVerts;
  DWORD         dwNumTriangles = 0L;
 // m_pVB->Lock( 0, 0, (void**)&pkVerts, D3DLOCK_DISCARD );
  __AAVVertex *pkVerts = m_pkTextRect->GetVerts();

  while( (c = *strText++) != 0 )
  {
    if( c == '\n' )
    {
      x = fStartX;
      y -= fLineSpacing;
    }

    if( (c-32) < 0 || (c-32) >= 128-32 )
      continue;

    //if(c == ' ')
    //{
    //  tx1 = m_fTexCoords['0'-32][0];
    //  ty1 = m_fTexCoords['0'-32][1];
    //  tx2 = m_fTexCoords['0'-32][2];
    //  ty2 = m_fTexCoords['0'-32][3];
    //}
    //else
    //{
    tx1 = m_fTexCoords[c-32][0];
    ty1 = m_fTexCoords[c-32][1];
    tx2 = m_fTexCoords[c-32][2];
    ty2 = m_fTexCoords[c-32][3];
    //}

    w = (tx2-tx1) * m_dwFontBitmapTextureWidth  / ( fScaling * m_fTextScale );
    h = (ty2-ty1) * m_dwFontBitmapTextureHeight / ( fScaling * m_fTextScale );

    if( c != _T(' ') )
    {
      //*pkVerts++ = InitFont3DVertex( D3DXVECTOR3(x+0,y+0,z), D3DXVECTOR3(0,0,-1), tx1, ty2 );
      //*pkVerts++ = InitFont3DVertex( D3DXVECTOR3(x+0,y+h,z), D3DXVECTOR3(0,0,-1), tx1, ty1 );
      //*pkVerts++ = InitFont3DVertex( D3DXVECTOR3(x+w,y+0,z), D3DXVECTOR3(0,0,-1), tx2, ty2 );
      //*pkVerts++ = InitFont3DVertex( D3DXVECTOR3(x+w,y+h,z), D3DXVECTOR3(0,0,-1), tx2, ty1 );
      //*pkVerts++ = InitFont3DVertex( D3DXVECTOR3(x+w,y+0,z), D3DXVECTOR3(0,0,-1), tx2, ty2 );
      //*pkVerts++ = InitFont3DVertex( D3DXVECTOR3(x+0,y+h,z), D3DXVECTOR3(0,0,-1), tx1, ty1 );
      *pkVerts = __AAVVertex( D3DXVECTOR3(x+0,y+0,z), 0xFFFFFFFF, tx1, ty2  ); pkVerts++;
      *pkVerts = __AAVVertex( D3DXVECTOR3(x+0,y+h,z), 0xFFFFFFFF, tx1, ty1  ); pkVerts++;
      *pkVerts = __AAVVertex( D3DXVECTOR3(x+w,y+0,z), 0xFFFFFFFF, tx2, ty2  ); pkVerts++;
      *pkVerts = __AAVVertex( D3DXVECTOR3(x+w,y+h,z), 0xFFFFFFFF, tx2, ty1  ); pkVerts++;
      *pkVerts = __AAVVertex( D3DXVECTOR3(x+w,y+0,z), 0xFFFFFFFF, tx2, ty2  ); pkVerts++;
      *pkVerts = __AAVVertex( D3DXVECTOR3(x+0,y+h,z), 0xFFFFFFFF, tx1, ty1  ); pkVerts++;
      dwNumTriangles += 2;

      if( dwNumTriangles*3 > (__AAVFont_D3D10::NUM_TEXT_RECT_VERTICES - 6) )
      {
        // Unlock, render, and relock the vertex buffer
        //m_pVB->Unlock();
        //m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
        //m_pVB->Lock( 0, 0, (void**)&pkVerts, D3DLOCK_DISCARD );
        m_pkTextRect->Update();
        m_pkTextRect->Render(dwNumTriangles*3, 0);

        dwNumTriangles = 0L;
      }
    }

    if( dwFlags & __AAVFont::VERTICAL )
    {
      y -= fLineSpacing *fScaling;
    }
    else
    {
      x += w - (2 * m_dwSpacing) *fScaling;
    }
  }

 // // Unlock and render the vertex buffer
 // m_pVB->Unlock();
 // if( dwNumTriangles > 0 )
 //   m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
  m_pkTextRect->Update();
 m_pkTextRect->Render(dwNumTriangles*3, 0);
  //m_pkTextRect->Render(6, 6);

 // // Restore the modified renderstates
 //// m_pStateBlockSaved->Apply();
}

VOID __AAVFont_D3D10::RenderText( const TCHAR* strText, DWORD dwFlags, FLOAT fScaling)
{
  RenderText(strText, &D3DXVECTOR3(0, 0, 0), dwFlags, fScaling);
}

HRESULT __AAVFont_D3D10::GetTextExtent( const TCHAR* strText, DWORD dwFlags, SIZE* pSize )
{
  if( NULL==strText || NULL==pSize )
    return E_FAIL;

  FLOAT fRowWidth  = 0.0f;
  FLOAT fRowHeight = (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwFontBitmapTextureHeight;
  FLOAT fWidth     = 0.0f;
  FLOAT fHeight    = fRowHeight;

  while( *strText )
  {
    TCHAR c = *strText++;

    if( (dwFlags & VERTICAL) || (c == _T('\n')) )
    {
      fRowWidth = 0.0f;
      fHeight  += fRowHeight;
    }

    if( (c-32) < 0 || (c-32) >= 128-32 )
      continue;

    FLOAT tx1 = m_fTexCoords[c-32][0];
    FLOAT tx2 = m_fTexCoords[c-32][2];

    fRowWidth += (tx2-tx1)*m_dwFontBitmapTextureWidth - 2*m_dwSpacing;

    if( fRowWidth > fWidth )
      fWidth = fRowWidth;
  }

  pSize->cx = (int)fWidth;
  pSize->cy = (int)fHeight;

  return S_OK;
}


HRESULT __AAVFont_D3D10::PaintAlphabet( HDC hDC, BOOL bMeasureOnly )
{
  SIZE size;
  TCHAR acStr[2] = _T("x"); // One-character, null-terminated string

  // Calculate the spacing between characters based on line height
  if( 0 == GetTextExtentPoint32( hDC, acStr, 1, &size ) )
    return E_FAIL;
  m_dwSpacing = (DWORD) ceil(size.cy * 0.3f);

  // Set the starting point for the drawing
  DWORD x = m_dwSpacing;
  DWORD y = 0;

  // For each character, draw text on the DC and advance the current position
  for( char c = 32; c < 127; c++ )
  {
    acStr[0] = c;
    if( 0 == GetTextExtentPoint32( hDC, acStr, 1, &size ) )
      return E_FAIL;

    if( (DWORD)(x + size.cx + m_dwSpacing) > m_dwFontBitmapTextureWidth )
    {
      x  = m_dwSpacing;
      y += size.cy + 1;
    }

    // Check to see if there's room to write the character here
    if( y + size.cy > m_dwFontBitmapTextureHeight )
      return D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS;

    if( !bMeasureOnly )
    {
      // Perform the actual drawing
      if( 0 == ExtTextOut( hDC, x+0, y+0, ETO_OPAQUE, NULL, acStr, 1, NULL ) )
        return E_FAIL;

      m_fTexCoords[c-32][0] = ((FLOAT)(x + 0       - m_dwSpacing))/m_dwFontBitmapTextureWidth;
      m_fTexCoords[c-32][1] = ((FLOAT)(y + 0       + 0          ))/m_dwFontBitmapTextureHeight;
      m_fTexCoords[c-32][2] = ((FLOAT)(x + size.cx + m_dwSpacing))/m_dwFontBitmapTextureWidth;
      m_fTexCoords[c-32][3] = ((FLOAT)(y + size.cy + 0          ))/m_dwFontBitmapTextureHeight;
    }

    x += size.cx + (2 * m_dwSpacing);
  }

  return S_OK;
}
