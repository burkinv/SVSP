#ifndef	__HDD_MODULE_H__
#define	__HDD_MODULE_H__

#include <__SVSPModule.h>

class HDD : public __SVSPModule
{
public:
  static const UINT  HDD_GAUGE_TEXTURE_WIDTH   = 1024;
  static const UINT  HDD_GAUGE_TEXTURE_HEIGHT  = 1024;
  static const UINT  HDD_GAUGE_TEXTURE_FORMAT  = D3DFMT_A1R5G5B5;
  static const UINT  FRAMEBUFFER_WIDTH         = 2048;
  static const UINT  FRAMEBUFFER_HEIGHT        = 2048;
  static const UINT  MAX_NUM_DISPLAY_MODULES   = 6;
  static const UINT  RENDERING_FRAME_FREQ      = 3;
  static const UINT  MAX_NUM_GAUGE_TEXTURES    = 12;

  //static TOUCH_MAP_ELEMENT g_akTouchMap[MAX_NUM_TOUCHMAP_ELEMENTS];

                          HDD(CHAR *pcName, __SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, LPPD3DXGEOMETRYOBJECT ppkGeomObjectsList, UINT uiNumGeomObjects, LPPD3DXTEXTUREOBJECT ppkTextureObjectsList, UINT uiNumTextureObjects);
  virtual                ~HDD();
          VOID            Render(LPD3DXDEVICE pkDevice);
  virtual VOID            SwapDisplayModules(__SVSPModule *pkModule1, __SVSPModule *pkModule2);
          VOID            SetDisplayModule(__SVSPModule *pkModule, AAVVector3<FLOAT> &rkDisplayOffsetVector, UINT uiDisplayModuleIndex);
          VOID            GetBackbufferData(struct IMAGE* pImage);
  virtual BOOL            EventHandler(DWORD dwEvent, DWORD dwEventData);
  inline  LPD3DXTEXTURE   GetGaugeTexture() const { return m_apkGaugeTexture[0]; }
  virtual VOID            Release                  ();
  virtual HRESULT         Reset                    (LPD3DXDEVICE pkDevice);
  virtual HRESULT         CreateTexture            (LPD3DXDEVICE pkDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
  virtual HRESULT         SetTexture               (LPD3DXDEVICE pkDevice, DWORD Stage,IDirect3DBaseTexture9* pTexture);
  virtual HRESULT         EndScene                 (LPD3DXDEVICE pkDevice);

protected:

  UINT                        m_uiFrameCount;
  LPD3DXTEXTURERENDERER       m_pkTextureRenderer;
  std::vector<__SVSPModule *> m_apkDisplayModule;

private:
  LPD3DXTEXTURE               m_apkGaugeTexture[HDD::MAX_NUM_GAUGE_TEXTURES];
  UINT                        m_uiNumGaugeTextures;

};

#endif	/* __HDD_MODULE_H__ */
