#ifndef	__WPN_MODULE_H__
#define	__WPN_MODULE_H__

#include "__SVSPModule.h"

class WPN : public __SVSPModule
{
public:
  const static UINT MAX_NUM_WAYPOINTS = 99;

  typedef enum 
  {
    MESSAGE_UPDATE_CURRENT_WAYPOINT = (WM_USER + 1304),
    MESSAGE_SELECT_NEXT_STP,
    MESSAGE_SELECT_PREV_STP,
  };

                                 WPN(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime);
  virtual                       ~WPN();
  VOID                           AddWaypoint(AAVWAYPOINT<FLOAT> &rkWaypoint);
  VOID                           DeleteAllWaypoints();
  inline UINT                    GetNumWaypoints() const { return m_uiNumWaypoints; }
  inline UINT                    GetCurrentWaypointNum() const { return m_uiCurrentWaypointNum; }
  inline const D3DXVECTOR3*      GetBodySteerpointOffsetVectorNormalized() const { AAV_ASSERT_RETURN((m_uiCurrentWaypointNum < m_uiNumWaypoints) && (m_uiCurrentWaypointNum < WPN::MAX_NUM_WAYPOINTS), NULL); return &(m_akWaypoints[m_uiCurrentWaypointNum].kBodyOffsetVectorNormalized); };
  inline const AAVWAYPOINT<FLOAT>*  GetWaypoint(UINT uiWaypointNum) const { if(uiWaypointNum < m_uiNumWaypoints) return &(m_akWaypoints[uiWaypointNum]); else return NULL; }
  D3DXVECTOR3                    GetLocalWaypointOffsetVector(UINT i) const { if(i < WPN::MAX_NUM_WAYPOINTS) return m_akWaypoints[i].kLocalOffsetVector; else return D3DXVECTOR3(0, 0, 0); }
  VOID                           RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix);
  VOID                           RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix, FLOAT fScale);
  VOID                           GenerateRouteMap();

  BOOL                           m_bEnroute;
  BOOL                           m_bRouteDeviationCheck;

protected:
  virtual VOID                   Process();

private:

  enum GEOMETRY_OBJECTS_LIST
  {
    GEOM_OBJECT_ROUTE_MAP,
    GEOM_OBJECT_HUD_WAYPOINT_DESIGNATOR,
    GEOM_OBJECT_HDD_WAYPOINT_DESIGNATOR,
    GEOM_OBJECT_RUNWAY_DESIGNATOR,
    GEOM_OBJECT_NUM_OBJECTS
  };

  enum TEXTURE_OBJECTS_LIST
  {
    TEXTURE_OBJECT_HUD_WAYPOINT_DESIGNATOR,
    TEXTURE_OBJECT_HDD_WAYPOINT_DESIGNATOR,
    TEXTURE_OBJECT_NUM_OBJECTS
  };

  static const UINT         NUM_HUD_WAYPOINT_DESIGNATOR_VERTICES = 4;
  static const UINT         NUM_HDD_WAYPOINT_DESIGNATOR_VERTICES = 4;
  static const UINT         NUM_RUNWAY_DESIGNATOR_VERTICES       = 2;

  AAVWAYPOINT<FLOAT>          m_akWaypoints[WPN::MAX_NUM_WAYPOINTS];
  UINT                     m_uiNumWaypoints;
  UINT                     m_uiCurrentWaypointNum;
  DOUBLE                   m_dRouteDeviationCheckStartTime;
  DOUBLE                   m_dRouteDeviationDuration;
  FLOAT                    m_fBingoDistance;
  D3DXMATRIXA16            m_kSteerpointDesignatorScalingMatrix;
  D3DXMATRIXA16            m_kHeadingRotationMatrix;

  FLOAT                    m_fAlpha;
};



#endif	/* __WPN_MODULE_H__ */
