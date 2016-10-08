#include "__AAVFont_OPENGL.h"
#include "__AAVDebug.h"

__AAVFont_OPENGL::__AAVFont_OPENGL(__AAVGraphics_OPENGL* pkGraphics, const TCHAR* sFontName, DWORD dwHeight, DWORD dwFlags) : __AAVFont(pkGraphics, sFontName, dwHeight, dwFlags)
{
  AAV_ASSERT_RETURN_VOID(pkGraphics != NULL);

  m_pkGraphics           = pkGraphics;
  m_pkDevice             = NULL;
  m_pkVertexBuffer       = NULL;
  m_pStateBlockSaved     = NULL;
  m_pStateBlockDrawText  = NULL;
  m_pkFontBitmapTexture  = NULL;


  /////////////////////////////////////////////////////////////////////
  // CREATE HDC
  /////////////////////////////////////////////////////////////////////
  HDC hDC = CreateCompatibleDC( NULL );
  SetMapMode( hDC, MM_TEXT );

  /////////////////////////////////////////////////////////////////////
  // CREATE GDI FONT
  /////////////////////////////////////////////////////////////////////
  INT nHeight    = -MulDiv( m_dwFontHeight, (INT)(GetDeviceCaps(hDC, LOGPIXELSY) * m_fTextScale), 72 );
  DWORD dwBold   = (m_dwFontFlags & __AAVFont_OPENGL::BOLD)   ? FW_BOLD : FW_NORMAL;
  DWORD dwItalic = (m_dwFontFlags & __AAVFont_OPENGL::ITALIC) ? TRUE    : FALSE;
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
  while(( m_hResult = PaintAlphabet( hDC, true ) ) == D3DERR_MOREDATA)
  {
    m_dwFontBitmapTextureWidth  *= 2;
    m_dwFontBitmapTextureHeight *= 2;
  }

  /////////////////////////////////////////////////////////////////////
  // CREATE FONT BITMAP
  /////////////////////////////////////////////////////////////////////
  BITMAPINFO bmi;
  ZeroMemory( &bmi.bmiHeader, sizeof(BITMAPINFOHEADER) );
  bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth       =  (int)m_dwFontBitmapTextureWidth;
  bmi.bmiHeader.biHeight      = -(int)m_dwFontBitmapTextureHeight;
  bmi.bmiHeader.biPlanes      = 1;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biBitCount    = 32;

  m_hBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS, (void**)&m_pBitmapBits, NULL, 0 );
  SelectObject( hDC, m_hBitmap );

  SetTextColor( hDC, RGB(255, 255, 255) );
  //SetTextColor( hDC, 0xFFFFFFFF );
  SetBkColor(   hDC, 0x00000000 );
  SetTextAlign( hDC, TA_TOP );

  m_hResult = PaintAlphabet( hDC, false );
  AAV_ASSERT_RETURN_VOID(SUCCEEDED(m_hResult));

  /////////////////////////////////////////////////////////////////////
  // CLEAN UP TEMPORARY RESOURCES
  /////////////////////////////////////////////////////////////////////
  DeleteObject( hFont );
  DeleteDC( hDC );

  /////////////////////////////////////////////////////////////////////
  // TEXT QUAD OBJECT
  /////////////////////////////////////////////////////////////////////
  //m_pkTextRect = new __AAVGeometryObject_OPENGL<__AAVVertex>(pkGraphics, __AAVGeometryObject_OPENGL<__AAVVertex>::TRIANGLELIST, __AAVFont_OPENGL::NUM_TEXT_RECT_VERTICES, __AAVGeometryObject<__AAVVertex>::USAGE_DYNAMIC);
  //m_pkTextRect = new __AAVGeometryObject_OPENGL<__AAVVertex>(pkGraphics, __AAVGeometryObject_OPENGL<__AAVVertex>::TRIANGLELIST, __AAVFont_OPENGL::NUM_TEXT_RECT_VERTICES, __AAVGeometryObject<__AAVVertex>::USAGE_DYNAMIC);

}

__AAVFont_OPENGL::~__AAVFont_OPENGL() 
{
  Release();

  //if(m_pkTextRect != NULL)
  //{
  //  delete m_pkTextRect;
  //  m_pkTextRect = NULL;
  //}

  DeleteObject( m_hBitmap );
}

VOID __AAVFont_OPENGL::SetTransform(struct D3DXMATRIX *pkTransformMatrix)
{
  m_pkDevice->SetTransform(D3DTS_WORLD, pkTransformMatrix);
}

VOID __AAVFont_OPENGL::Begin()
{
}

VOID __AAVFont_OPENGL::End()
{
}

VOID __AAVFont_OPENGL::Release()
{
  //if(m_pkTextRect != NULL)
  //{
  //  m_pkTextRect->Release();
  //}

  if(m_pkFontBitmapTexture != NULL)
  {
    m_pkFontBitmapTexture->Release();
    m_pkFontBitmapTexture = NULL;
  }

  if(m_pkVertexBuffer != NULL)
  {
    m_pkVertexBuffer->Release();
    m_pkVertexBuffer = NULL;
  }

  if(m_pStateBlockSaved != NULL)
  {
    m_pStateBlockSaved->Release();
    m_pStateBlockSaved = NULL;
  }
  if(m_pStateBlockDrawText != NULL)
  {
    m_pStateBlockDrawText->Release();
    m_pStateBlockDrawText = NULL;
  }
}

HRESULT __AAVFont_OPENGL::Reset()
{
  m_pkDevice = pkDevice;

  /////////////////////////////////////////////////////////////////////
  // CREATE VERTEX BUFFER
  /////////////////////////////////////////////////////////////////////
  m_hResult = m_pkDevice->CreateVertexBuffer(__AAVFont_OPENGL::NUM_TEXT_RECT_VERTICES*sizeof(__AAVVertex), 
                                             D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
                                             0, 
                                             D3DPOOL_DEFAULT, 
                                             &m_pkVertexBuffer, 
                                             NULL);
  AAV_ASSERT_RETURN(SUCCEEDED(m_hResult) && (m_pkVertexBuffer != NULL), E_FAIL);

  /////////////////////////////////////////////////////////////////////
  // CREATE FONT BITMAP TEXTURE
  /////////////////////////////////////////////////////////////////////
  m_hResult = m_pkDevice->CreateTexture(m_dwFontBitmapTextureWidth, 
                                        m_dwFontBitmapTextureHeight, 
                                        1,
                                        0, 
                                        D3DFMT_A8R8G8B8,
                                        D3DPOOL_MANAGED, 
                                        &m_pkFontBitmapTexture, 
                                        NULL );
  AAV_ASSERT_RETURN(SUCCEEDED(m_hResult) && (m_pkFontBitmapTexture != NULL), E_FAIL);

  /////////////////////////////////////////////////////////////////////
  // COPY FONT BITMAP TO THE TEXTURE
  /////////////////////////////////////////////////////////////////////
  // Lock the surface and write the alpha values for the set pixels
  D3DLOCKED_RECT kLockedRect;
  m_pkFontBitmapTexture->LockRect( 0, &kLockedRect, 0, 0 );
  DWORD* pDst32 = (DWORD*)kLockedRect.pBits;
  DWORD dwSize = m_dwFontBitmapTextureWidth*m_dwFontBitmapTextureHeight;

  for(DWORD i = 0; i < dwSize; ++i)
  {
    DWORD dwPixelColor = m_pBitmapBits[i];
    DWORD dwTexPixel = 0x00FFFFFF | ((dwPixelColor & 0x000000FF) << 24);
   *pDst32 = dwTexPixel;
    pDst32++;
  }
  // Unlock texture.
  m_pkFontBitmapTexture->UnlockRect(0);

  /////////////////////////////////////////////////////////////////////
  // RESET TEXT RECT
  /////////////////////////////////////////////////////////////////////
  //m_pkTextRect->Reset();

  /////////////////////////////////////////////////////////////////////
  // SET FONT BITMAP TEXTURE
  /////////////////////////////////////////////////////////////////////
  //m_pkTextRect->SetTexture((LPD3DXTEXTURE)m_pkFontBitmapTexture);

  /////////////////////////////////////////////////////////////////////
  // Create the state blocks for rendering text
  /////////////////////////////////////////////////////////////////////
  for( UINT which=0; which<2; which++ )
  {
    m_pkDevice->BeginStateBlock();
    m_pkDevice->SetTexture( 0, m_pkFontBitmapTexture );

    //if ( D3DFONT_ZENABLE & m_dwFontFlags )
    //    m_pkDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    //else
    //    m_pkDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

    //if( bSupportsAlphaBlend )
    //{
    m_pkDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pkDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
    m_pkDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
    //m_pkDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    m_pkDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
    m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

    m_pkDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    m_pkDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    m_pkDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
    //}
    //else
    //{
    //  m_pkDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    //}
    //m_pkDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
    //m_pkDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
    //m_pkDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
    //m_pkDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
    //m_pkDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
    //m_pkDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
    //m_pkDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
    //m_pkDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
    //m_pkDevice->SetRenderState( D3DRS_VERTEXBLEND,      D3DVBF_DISABLE );
    //m_pkDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
    //m_pkDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
    //m_pkDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED  | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
    //m_pkDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    //m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    //m_pkDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    //m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    //m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    //m_pkDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    //m_pkDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
    //m_pkDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
    //m_pkDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    //m_pkDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
    ////m_pkDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
    ////m_pkDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
    //m_pkDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    //m_pkDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    //m_pkDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
    //m_pkDevice->SetSamplerState( 0, D3DSAMP_BORDERCOLOR , 0xFFFFFFFF );

    if( which==0 )
      m_pkDevice->EndStateBlock( &m_pStateBlockSaved );
    else
      m_pkDevice->EndStateBlock( &m_pStateBlockDrawText );
  }

  return S_OK;
}

VOID __AAVFont_OPENGL::RenderTextExt( const TCHAR* strText, DWORD dwFlags, FLOAT fScaling, DWORD dwColor)
{
  RenderTextExt(strText, &D3DXVECTOR3(0, 0, 0), dwFlags, fScaling, dwColor);
}

VOID __AAVFont_OPENGL::RenderTextExt( const TCHAR* strText, struct D3DXVECTOR3 *v, DWORD dwFlags, FLOAT fScaling, DWORD dwColor)
{
  FLOAT tx1;
  FLOAT ty1;
  FLOAT tx2;
  FLOAT ty2;
  FLOAT w;
  FLOAT h;

  if(strText[0] == '\0')
  {
    return;
  }

  AAV_ASSERT_RETURN_VOID(m_pkFontBitmapTexture != NULL);
  AAV_ASSERT_RETURN_VOID(m_pkGraphics != NULL);


  //////////////////////////////////////////////////////////////
  // Setup font required renderstate
  //////////////////////////////////////////////////////////////
  //if(m_pStateBlockSaved)
  //{
  //  m_pStateBlockSaved->Capture();
  //}
  //if(m_pStateBlockDrawText) m_pStateBlockDrawText->Apply();

  m_pkDevice->SetTexture( 0, m_pkFontBitmapTexture );
  m_pkGraphics->m_pkStateBlock[__AAVGraphics_OPENGL::STATE_BLOCK_TRANSPARENT_TEXTURE]->Apply();

  //m_pkDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
  //m_pkDevice->SetPixelShader( NULL );
  m_pkDevice->SetStreamSource( 0, m_pkVertexBuffer, 0, sizeof(__AAVVertex) );


  // Position for each text element
  FLOAT x = v->x;
  FLOAT y = v->y;
  FLOAT z = v->z;

  FLOAT fLineSpacing = (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwFontBitmapTextureHeight;

  // Center the text block at the origin (not the viewport)
  SIZE sz;
  GetTextExtent( strText, dwFlags, &sz );

  //fScaling *= 3.5;

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
 // //  m_pkDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

  // Adjust for character spacing
  x -= m_dwSpacing *fScaling;
  FLOAT fStartX = x;
  TCHAR c;

  /////////////////////////////////////////////////////////
  // Fill vertex buffer
  /////////////////////////////////////////////////////////
  __AAVVertex* pkVerts;
  DWORD         dwNumTriangles = 0L;

  m_pkVertexBuffer->Lock( 0, 0, (void**)&pkVerts, D3DLOCK_DISCARD );
  //__AAVVertex *pkVerts = m_pkTextRect->GetVerts();

  while( (c = *strText++) != 0 )
  {
    if( c == '\n' )
    {
      x = fStartX;
      y -= fLineSpacing;
    }

    if( (c-32) < 0 || (c-32) >= 128-32 )
      continue;

    tx1 = m_fTexCoords[c-32][0];
    ty1 = m_fTexCoords[c-32][1];
    tx2 = m_fTexCoords[c-32][2];
    ty2 = m_fTexCoords[c-32][3];

    w = (tx2-tx1) * m_dwFontBitmapTextureWidth*fScaling  / (m_fTextScale );
    h = (ty2-ty1) * m_dwFontBitmapTextureHeight*fScaling / (m_fTextScale );

    if( c != _T(' ') )
    {
      *pkVerts = __AAVVertex(x+0, y+0, z, tx1, ty2, dwColor); pkVerts++;
      *pkVerts = __AAVVertex(x+0, y+h, z, tx1, ty1, dwColor); pkVerts++;
      *pkVerts = __AAVVertex(x+w, y+0, z, tx2, ty2, dwColor); pkVerts++;
      *pkVerts = __AAVVertex(x+w, y+h, z, tx2, ty1, dwColor); pkVerts++;
      *pkVerts = __AAVVertex(x+w, y+0, z, tx2, ty2, dwColor); pkVerts++;
      *pkVerts = __AAVVertex(x+0, y+h, z, tx1, ty1, dwColor); pkVerts++;
      dwNumTriangles += 2;

      if( dwNumTriangles*3 > (__AAVFont_OPENGL::NUM_TEXT_RECT_VERTICES - 6) )
      {
        // Unlock, render, and relock the vertex buffer
        m_pkVertexBuffer->Unlock();
        m_pkDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
        m_pkVertexBuffer->Lock( 0, 0, (void**)&pkVerts, D3DLOCK_DISCARD );
        //m_pkTextRect->Update();
        //m_pkTextRect->RenderPrimitives(0, dwNumTriangles);

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

  // Unlock and render the vertex buffer
  m_pkVertexBuffer->Unlock();
  if( dwNumTriangles > 0 )
    m_pkDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );

  //////////////////////////////////////////////////////////////
  //Restore the modified renderstates
  //////////////////////////////////////////////////////////////
  //m_pStateBlockSaved->Apply();
}

HRESULT __AAVFont_OPENGL::GetTextExtent( const TCHAR* strText, DWORD dwFlags, SIZE* pSize )
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


HRESULT __AAVFont_OPENGL::PaintAlphabet( HDC hDC, BOOL bMeasureOnly )
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
      return D3DERR_MOREDATA;

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
