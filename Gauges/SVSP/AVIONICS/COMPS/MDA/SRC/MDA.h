#ifndef	__MDA_MODULE_H__
#define	__MDA_MODULE_H__

#include "__SVSPModule.h"

class MDA : public __SVSPModule
{
public:

  enum 
  {
    MESSAGE_INVALIDATE_TARGET    = (WM_USER + 3304),
    MESSAGE_REARM_TARGETS,
    MESSAGE_UNARM_TARGETS,
    MESSAGE_CREATE_AIP,
    MESSAGE_DESTROY_AIP,
  };
  const static UINT AIRCRAFT_INDEX   = 0;
  const static UINT MAX_NUM_CARRIERS = 2;
  const static UINT MAX_NUM_AIP      = 1;
  const static UINT MAX_NUM_TARGETS  = 20;
  const static UINT MAX_NUM_WAYPOINTS= 32;

                     MDA(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, LPWSTR lpMissionFile);
  virtual           ~MDA();
  virtual VOID       Start();
  virtual VOID       Stop();
  VOID               InitWeaponObjects(struct IXMLDOMNodeList *pObjectsNodeList);
  VOID               ReadConfigFile(LPWSTR lpMissionDataFileName, LPWSTR lpReadSectionName = NULL);
  VOID               Render(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  VOID               Render(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix, FLOAT fScale);
  VOID               AddTarget(__SVSPModule *pkTarget);
  VOID               DeleteAllTargets();
  UINT               GetNumTargets() const { return m_uiNumTargets; }
  virtual VOID       Release                  ();
  virtual HRESULT    Reset                    (LPD3DXDEVICE pkDevice);
  __SVSPModule*       GetTargetID(UINT i) const 
  { 
    AAV_ASSERT(i < MDA::MAX_NUM_TARGETS)
    return m_apkTargets[i]; 
  }
  VOID               LoadMission(WCHAR *pwcFilename);

protected:
  virtual VOID         Process();
  FLOAT                GetRandomValueWithinLatitudeRange(FLOAT dValue1, FLOAT dValue2) const;
  FLOAT                GetRandomValueWithinLongitudeRange(FLOAT dValue1, FLOAT dValue2) const;

private:
  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_HUD_WAYPOINT_DESIGNATOR,
    GEOM_OBJECT_GROUND_TARGET_DESIGNATOR,
    GEOM_OBJECT_NUM_OBJECTS
  };

  static const UINT         NUM_GROUND_TARGET_DESIGNATOR_VERTICES = 4;

  UINT               m_uiNumTargets;
  __SVSPModule       *m_apkTargets[MAX_NUM_TARGETS];

  CHAR               m_acStr[64];

};

#endif	/* __MDA_MODULE_H__ */
