#include "WPN.h"
#include "AC.h"
#include "__SVSPAvionics.h"
#include "CARRIER.h"

WPN::WPN(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime) : __SVSPModule("WPN", pkAvionics, dwThreadSleepTime, false, NULL, NULL, GEOM_OBJECT_NUM_OBJECTS, NULL, TEXTURE_OBJECT_NUM_OBJECTS)
{
  memset(&m_akWaypoints[0], 0, sizeof(m_akWaypoints)); 
  m_uiNumWaypoints       = 0;
  m_uiCurrentWaypointNum = 0;
  m_bEnroute             = true;
  m_bRouteDeviationCheck = false;

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;

  ///////////////////////////////////////////////////////////////////
  // RUNWAY DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINELIST, NUM_RUNWAY_DESIGNATOR_VERTICES);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex( 0.0f,  10.0f, 0.0f, 0, 0, 0xffff28f2);
  pVerts[1] = __SVSPVertex( 0.0f, -10.0f, 0.0f, 0, 0, 0xffff28f2);
  m_ppkGeomObjects[GEOM_OBJECT_RUNWAY_DESIGNATOR] = pkGeomObject;

  /////////////////////////////////////////////////////////////////////
  //// GEOM_OBJECT_HUD_WAYPOINT_DESIGNATOR
  /////////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_HUD_WAYPOINT_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(1.0f, 0.3f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  FLOAT r = 0.3f;
  pVerts[0] = __SVSPVertex(-r,  r, 0, 0.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[1] = __SVSPVertex( r,  r, 0, 1.0f, 0.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[2] = __SVSPVertex(-r, -r, 0, 0.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  pVerts[3] = __SVSPVertex( r, -r, 0, 1.0f, 1.0f, __SVSPColor::dwHUDCOLOR);
  m_ppkGeomObjects[GEOM_OBJECT_HUD_WAYPOINT_DESIGNATOR] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_HUD_WAYPOINT_DESIGNATOR] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/WPN/RES/TEXTURES/SP_128x128.dds");

  /////////////////////////////////////////////////////////////////////
  //// GEOM_OBJECT_HDD_WAYPOINT_DESIGNATOR
  /////////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_HDD_WAYPOINT_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(1.0f, 0.3f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  r = 8;
  pVerts[0] = __SVSPVertex(-r,  r, 0, 0.0f, 0.0f, 0xFFFFFFFF);
  pVerts[1] = __SVSPVertex( r,  r, 0, 1.0f, 0.0f, 0xFFFFFFFF);
  pVerts[2] = __SVSPVertex(-r, -r, 0, 0.0f, 1.0f, 0xFFFFFFFF);
  pVerts[3] = __SVSPVertex( r, -r, 0, 1.0f, 1.0f, 0xFFFFFFFF);
  m_ppkGeomObjects[GEOM_OBJECT_HDD_WAYPOINT_DESIGNATOR] = pkGeomObject;
  m_ppkTextureObjects[TEXTURE_OBJECT_HDD_WAYPOINT_DESIGNATOR] = new D3DXTEXTUREOBJECT("Gauges/SVSP/AVIONICS/COMPS/WPN/RES/TEXTURES/SP_16x16.dds");

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_ROUTE_MAP
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::LINESTRIP, WPN::MAX_NUM_WAYPOINTS);
  pkGeomObject->SetMaterial(1.0f, 0.3f, 0.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  for(UINT i = 0; i < WPN::MAX_NUM_WAYPOINTS; ++i)
  {
    pVerts[i] = __SVSPVertex(0, 0, 0, 0, 0, 0xFFFFFFFF);
  }
  m_ppkGeomObjects[GEOM_OBJECT_ROUTE_MAP] = pkGeomObject;

  UINT uiUpdateWaypointNum = 0;
}

WPN::~WPN()
{
  this->Stop();

}

VOID WPN::AddWaypoint(AAVWAYPOINT<FLOAT> &rkWaypoint)
{
  if(m_uiNumWaypoints < WPN::MAX_NUM_WAYPOINTS)
  {
    m_akWaypoints[m_uiNumWaypoints] = rkWaypoint;
    m_uiNumWaypoints++;
  }
}

VOID WPN::DeleteAllWaypoints()
{
  m_uiNumWaypoints = 0;
}


VOID WPN::GenerateRouteMap()
{
  if((m_uiNumWaypoints != 0) && (m_uiNumWaypoints < WPN::MAX_NUM_WAYPOINTS))
  {
    LPD3DXGEOMETRYOBJECT pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_ROUTE_MAP];
    AAV_ASSERT_RETURN_VOID(pkGeomObject != NULL);

    __SVSPVertex* pVerts       = pkGeomObject->GetVerts();

    for(UINT i = 0; i < m_uiNumWaypoints; ++i)
    {
      AAVWAYPOINT<FLOAT> *pkWaypoint = &m_akWaypoints[i];
      AAVVector3<FLOAT> kGlobalSteerpointVectorNauticalMiles = pkWaypoint->kGlobalVector/1852;
      pVerts[i] = __SVSPVertex(kGlobalSteerpointVectorNauticalMiles.x, kGlobalSteerpointVectorNauticalMiles.y, kGlobalSteerpointVectorNauticalMiles.z, 0, 0, 0xFFFFFFFF);
    }
  }
}

VOID WPN::Process()
{
  MSG       kMsg;

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT(pkAC  != NULL);

  /////////////////////////////////////////////////////////
  // UPDATE LOCAL OFFSET VECTORS
  /////////////////////////////////////////////////////////
  //if((m_uiNumWaypoints != 0) && (uiUpdateWaypointNum < m_uiNumWaypoints) && (uiUpdateWaypointNum < WPN::MAX_NUM_WAYPOINTS))
  //{
  //  D3DXVECTOR3 kGlobalOffsetVec = D3DXVECTOR3(m_akWaypoints[uiUpdateWaypointNum].kGlobalVector.x, m_akWaypoints[uiUpdateWaypointNum].kGlobalVector.y, m_akWaypoints[uiUpdateWaypointNum].kGlobalVector.z)  - pkAC->m_kGlobalVector;
  //  D3DXVECTOR3 kLocalOffsetVec = *D3DXVec3TransformCoord(&kLocalOffsetVec, &kGlobalOffsetVec, &pkAC->m_kGlobalToLocalTransformMatrix);
  //  D3DXVec3TransformCoord(&m_akWaypoints[uiUpdateWaypointNum].kLocalOffsetVector, &kLocalOffsetVec, &pkAC->m_kBodyToLocalYRotMatrix);

  //  uiUpdateWaypointNum++;
  //  uiUpdateWaypointNum  = (uiUpdateWaypointNum < m_uiNumWaypoints) ? uiUpdateWaypointNum : 0;
  //}

  if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  {
    switch(kMsg.message)
    {
    case WPN::MESSAGE_SELECT_NEXT_STP:
      m_uiCurrentWaypointNum = (m_uiCurrentWaypointNum < (m_uiNumWaypoints - 1)) ? m_uiCurrentWaypointNum + 1 : m_uiCurrentWaypointNum;
      break;

    case WPN::MESSAGE_SELECT_PREV_STP:
      m_uiCurrentWaypointNum = (m_uiCurrentWaypointNum > 0)                      ? m_uiCurrentWaypointNum - 1 : m_uiCurrentWaypointNum;
      break;
    }
  }

  // Update all STP bearing data.
  for(UINT i = 0; ((i < m_uiNumWaypoints) && (i < WPN::MAX_NUM_WAYPOINTS)); ++i)
  {
    AAVWAYPOINT<FLOAT> *pkSTP                   = &m_akWaypoints[i];
    if(pkSTP->dwRadioFrequency == 0)
    {
      pkSTP->kGlobalOffsetVector   = pkSTP->kGlobalVector - pkAC->m_kVector;
      D3DXVec3TransformCoord(&pkSTP->kLocalOffsetVector, (D3DXVECTOR3 *)&pkSTP->kGlobalOffsetVector, &pkAC->m_kGlobalToLocalTransformMatrix);
      pkSTP->kLocalOffsetVector.x  = -pkSTP->kLocalOffsetVector.x;

      if(i == (m_uiNumWaypoints - 1))
      {
        m_fBingoDistance = pkSTP->kGlobalOffsetVector.Length(); //
      }
    }
  }

  // Update current STP navigation data.
  AAV_ASSERT_RETURN_VOID(m_uiCurrentWaypointNum < WPN::MAX_NUM_WAYPOINTS);

  FLOAT fSlantRangeMeters;
  AAVWAYPOINT<FLOAT> *pkSTP                   = &m_akWaypoints[m_uiCurrentWaypointNum];
  if(pkSTP->dwRadioFrequency != 0)
  {
    //Check active NAV1 frequency.
    if(pkAC->m_kNavRadioData.m_dNavFrequency != pkSTP->dwRadioFrequency)
    {
      // Tune NAV1 to current STP radio frequency.
      send_key_event(KEY_NAV1_STBY_SET, pkSTP->dwRadioFrequency);
      send_key_event(KEY_NAV1_RADIO_SWAP,TRUE);
    }
    else
    {
      if(pkAC->m_kNavRadioData.m_bNavHasDME)
      {
        // fSlantRange (Miles)
        pkSTP->fSlantRangeMiles             = pkAC->m_kNavRadioData.m_dNavDMEDistance;
        // Slant range (Meters)
        fSlantRangeMeters                   = pkSTP->fSlantRangeMiles/0.62137E-3f;
        pkSTP->bHasRangeData = true;
      }
      else
      {
        pkSTP->bHasRangeData = false;
      }
    }
  }
  else
  {
    // Slant range (Meters)
    fSlantRangeMeters                         = pkSTP->kGlobalOffsetVector.Length();
    // fSlantRange (Miles)
    pkSTP->fSlantRangeMiles                   = 0.62137E-3f*fSlantRangeMeters;
    pkSTP->bHasRangeData                      = true;
  }

  //Update DTI and TTI data if range information available.
  if(pkSTP->bHasRangeData)
  {
    FLOAT fVelocity                           = (FLOAT)pkAC->m_dVelocity/3.28f; // m/s
    // Cos and Sin of steerpoint aspect anlge.
    D3DXVec3TransformCoord(&pkSTP->kBodyOffsetVector, &pkSTP->kLocalOffsetVector, &pkAC->m_kLocalToBodyYMatrix);
    D3DXVec3Normalize(&pkSTP->kBodyOffsetVectorNormalized, &pkSTP->kBodyOffsetVector);
    D3DXVECTOR2 kBodySteerpointOffsetVector           = D3DXVECTOR2(pkSTP->kBodyOffsetVector.x, pkSTP->kBodyOffsetVector.z);
    //D3DXVECTOR2 kBodySteerpointOffsetVector           = D3DXVECTOR2(0, 0);
    //AAV_ASSERT(D3DXVec2Length(&kBodySteerpointOffsetVector) != 0);
    D3DXVECTOR2 kBodySteerpointOffsetVectorNormalized;
    D3DXVec2Normalize(&kBodySteerpointOffsetVectorNormalized, &kBodySteerpointOffsetVector);
    //D3DXVECTOR2 kBodySteerpointOffsetVectorNormalized = D3DXVECTOR2(1, 0);
    FLOAT fCosOfSTPAspectAngle                        = kBodySteerpointOffsetVectorNormalized.y;
    FLOAT fSinOfSTPAspectAngle                        = kBodySteerpointOffsetVectorNormalized.x;
    pkSTP->fClosingInVelocity                         = fVelocity*fCosOfSTPAspectAngle;
    // TIME-TO-INTERCEPT (TTI)
    pkSTP->usTTI                              = (pkSTP->fClosingInVelocity > 10.0f) ? (USHORT)(fSlantRangeMeters/pkSTP->fClosingInVelocity) : pkSTP->usTTI;
    pkSTP->usTTIMinutes                       = pkSTP->usTTI/60;
    pkSTP->usTTISeconds                       = pkSTP->usTTI%60;
    // DISTANCE-TO-INTERCEPT (DTI)
    pkSTP->uiDTI                              = (fmod(pkSTP->fSlantRangeMiles, 1.0f) < 0.5f) ? (UINT)pkSTP->fSlantRangeMiles : (UINT)pkSTP->fSlantRangeMiles + 1;

    // kSteercueRotationMatrix
    D3DXMATRIXA16 kSteercueRotationMatrix;
    D3DXMATRIX *pkM = &kSteercueRotationMatrix;

    pkM->_11 = fCosOfSTPAspectAngle; pkM->_12 = -fSinOfSTPAspectAngle; pkM->_13 = 0; pkM->_14 = 0;
    pkM->_21 = fSinOfSTPAspectAngle; pkM->_22 =  fCosOfSTPAspectAngle; pkM->_23 = 0; pkM->_24 = 0;
    pkM->_31 = 0;                    pkM->_32 = 0;                     pkM->_33 = 1; pkM->_34 = 0;
    pkM->_41 = 0;                    pkM->_42 = 0;                     pkM->_43 = 0; pkM->_44 = 1;

    // kSteercueTranslationMatrix
    D3DXMATRIXA16 kSteercueTranslationMatrix;
    FLOAT fX = 20*fSinOfSTPAspectAngle;
    //D3DXMatrixTranslation(&kSteercueTranslationMatrix, (fCosOfSTPAspectAngle > 0) ? 2.0f*(fX/sqrt(1.0f + fX*fX)) : 2.0f*sign(fSinOfSTPAspectAngle), 0, 0);
    D3DXMatrixTranslation(&kSteercueTranslationMatrix, ((fCosOfSTPAspectAngle > 0) && (abs(fX) < 2.0f)) ? fX : 2.0f*sign(fSinOfSTPAspectAngle), 0, 0);

    // kSteercueTransformMatrix
    pkSTP->kSteercueTransformMatrix = pkAC->m_kBankRotationMatrix*kSteercueRotationMatrix*kSteercueTranslationMatrix;


    // Track route schedule.
    if(!pkAC->m_kData.m_bOnGround)
    {
      if(pkSTP->fClosingInVelocity < 0)
      {
        if(pkSTP->fSlantRangeMiles < 2.0)
        {
          //Swith to a next STP if any left.
          if(m_uiCurrentWaypointNum < (m_uiNumWaypoints - 1))
          {
            m_uiCurrentWaypointNum++;
          }
        }
        else
        {
          //Warn about offroute deviation and check deviation timings.
          if(!m_bRouteDeviationCheck)
          {
            m_bRouteDeviationCheck = true;
            m_dRouteDeviationCheckStartTime = GetSystemTime();
          }
          else
          {
            m_dRouteDeviationDuration = GetSystemTime() - m_dRouteDeviationCheckStartTime;

            if(m_dRouteDeviationDuration > 60.0)
            {
              m_bEnroute             = false;
              m_bRouteDeviationCheck = false;
            }
          }
        }
      }
      else
      {
        m_bEnroute             = true;
        m_bRouteDeviationCheck = false;
      }
    }
    else
    {
      m_bRouteDeviationCheck = false;
    }
  }
}

VOID WPN::RenderOnHUD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  char acStr[32];
  D3DXMATRIXA16 kTransformMatrix;

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  LPAAVFONT  pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_HUD_WAYPOINT_DESIGNATOR];
  AAV_ASSERT_RETURN_VOID(pkGeomObject  != NULL);
    

  //for(UINT i = 0; i <  m_uiNumWaypoints; ++i)
  //{
  //  D3DXVECTOR3  kGlobalOffsetVector = (D3DXVECTOR3)m_akWaypoints[i].kGlobalVector - pkAC->m_kGlobalVector;
  //  D3DXVECTOR3  kLocalOffsetVector = *D3DXVec3TransformCoord(&kLocalOffsetVector, &kGlobalOffsetVector, &pkAC->m_kGlobalToLocalTransformMatrix);
  //  D3DXVECTOR3  kLocalOffsetVectorNormalized = *D3DXVec3Normalize(&kLocalOffsetVectorNormalized, &kLocalOffsetVector);
  //  GetTransformMatrixFromLocalNormalizedVector(&kTransformMatrix, &kLocalOffsetVectorNormalized);

  //  //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*pkAC->m_kBankRotationMatrix*kTransformMatrix));
  //  pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*kTransformMatrix));
  //  pkGeomObject->SetTexture(pkDevice, NULL);
  //  pkGeomObject->Render(pkDevice);
  //  sprintf_s(&acStr[0], sizeof(acStr), "%d", i + 1);
  //  pkFont->RenderTextExt(pkDevice, (&acStr[0]), (D3DXVECTOR3 *)&D3DXVECTOR3(0, 0, 0), __SVSPFont::FILTERED|__SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 155.0f);
  //}

  if(m_uiCurrentWaypointNum < WPN::MAX_NUM_WAYPOINTS)
  {
    D3DXVECTOR3  kGlobalOffsetVector = (D3DXVECTOR3)m_akWaypoints[m_uiCurrentWaypointNum].kGlobalVector - pkAC->m_kGlobalVector;
    D3DXVECTOR3  kLocalOffsetVector = *D3DXVec3TransformCoord(&kLocalOffsetVector, &kGlobalOffsetVector, &pkAC->m_kGlobalToLocalTransformMatrix);
    D3DXVECTOR3  kLocalOffsetVectorNormalized = *D3DXVec3Normalize(&kLocalOffsetVectorNormalized, &kLocalOffsetVector);
    GetTransformMatrixFromLocalNormalizedVector(&kTransformMatrix, &kLocalOffsetVectorNormalized);

    //pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*pkAC->m_kBankRotationMatrix*kTransformMatrix));
    pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*kTransformMatrix));
    pkGeomObject->SetTexture(pkDevice, m_ppkTextureObjects[TEXTURE_OBJECT_HUD_WAYPOINT_DESIGNATOR]);
    pkGeomObject->Render(pkDevice);
    sprintf_s(&acStr[0], sizeof(acStr), "%d", m_uiCurrentWaypointNum + 1);
    pkFont->RenderTextExt(pkDevice, &acStr[0], __SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y);
  }
}

VOID WPN::RenderOnHDD(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix, FLOAT fCurrentNumPixelsInNauticalMile)
{
  D3DXMATRIXA16 kObjectOffsetMatrix;
  D3DXVECTOR3   kBodyOffsetVector;
  D3DXVECTOR3   kBodyOffsetVectorNauticalMiles;

  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObjectWaypointDesignator = m_ppkGeomObjects[GEOM_OBJECT_HDD_WAYPOINT_DESIGNATOR];
  AAV_ASSERT_RETURN_VOID(pkGeomObjectWaypointDesignator  != NULL);
  LPD3DXTEXTUREOBJECT pkTextureObjectWaypointDesignator = m_ppkTextureObjects[TEXTURE_OBJECT_HDD_WAYPOINT_DESIGNATOR];
  LPD3DXGEOMETRYOBJECT pkGeomObjectRunwayDesignator = m_ppkGeomObjects[GEOM_OBJECT_RUNWAY_DESIGNATOR];
  AAV_ASSERT_RETURN_VOID(pkGeomObjectRunwayDesignator  != NULL);

  /////////////////////////////////////////////////////////////////////
  // RENDER ROUTE MAP
  /////////////////////////////////////////////////////////////////////
  if(m_uiNumWaypoints > 1)
  {

    __SVSPFont* pkFont = m_pkAvionics->GetFont();
    AAV_ASSERT_RETURN_VOID(pkFont  != NULL);


    AAV_ASSERT_RETURN_VOID(fCurrentNumPixelsInNauticalMile > 0);

    D3DXMATRIXA16 kTranslationMatrix;
    D3DXMATRIXA16 kRotationMatrix;
    D3DXMATRIXA16 kHeadingMatrix;
    D3DXMATRIXA16 kScalingMatrix;

    pkGeomObjectWaypointDesignator->SetTexture(pkDevice, pkTextureObjectWaypointDesignator);
    for(UINT i = 0; ((i < (m_uiNumWaypoints - 1)) && (i < (WPN::MAX_NUM_WAYPOINTS - 1))); ++i)
    {
      D3DXVec3TransformCoord(&kBodyOffsetVector, &m_akWaypoints[i].kLocalOffsetVector, &pkAC->m_kLocalToBodyYMatrix);
      D3DXVECTOR3 kBodyOffsetVectorNauticalMiles = kBodyOffsetVector/1852;
      D3DXVECTOR3 kBodyOffsetVectorPixels = fCurrentNumPixelsInNauticalMile*kBodyOffsetVectorNauticalMiles;
      D3DXMatrixTranslation(&kObjectOffsetMatrix, kBodyOffsetVectorPixels.x, kBodyOffsetVectorPixels.z, 0);

      pkGeomObjectWaypointDesignator->SetTransform(pkDevice, &(kObjectOffsetMatrix*rkOPMatrix));
      pkGeomObjectWaypointDesignator->Render(pkDevice);
      //if(i == 0)
      //{
      //  D3DXMatrixRotationZ(&kRotationMatrix, -D3DX_PI*m_akWaypoints[0].fHeading/180.0f + pkAC->m_kData.m_dIndicatedMagneticHeading);
      //  pkGeomObjectWaypointDesignator->SetTransform(pkDevice, &(kRotationMatrix*kTranslationMatrix*rkOPMatrix));
      //  pkGeomObjectRunwayDesignator->Render(pkDevice);
      //}
      //sprintf_s(&m_acStr[0], sizeof(m_acStr), "%d", i);
      //pkFont->RenderText((&m_acStr[0]), (D3DXVECTOR3 *)&UNIT_VECTOR, __SVSPFont::FILTERED|__SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 155.0f);
      //sprintf_s(&m_acStr[0], sizeof(m_acStr), "  %3.1f", D3DXVec3Length(&kGlobalVec)/1852.0f);
      //pkFont->RenderText((&m_acStr[0]), (D3DXVECTOR3 *)&UNIT_VECTOR, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y, 155.0f);
    }


    LPD3DXGEOMETRYOBJECT pkRouteMap = m_ppkGeomObjects[GEOM_OBJECT_ROUTE_MAP];
    D3DXMatrixRotationX(&kRotationMatrix, D3DX_PI/2);
    D3DXMatrixRotationZ(&kHeadingMatrix, pkAC->m_kBodyAttitude.y + D3DX_PI);
    //FLOAT fRouteMapScale = fScale*
    D3DXMatrixScaling(&kScalingMatrix, fCurrentNumPixelsInNauticalMile, fCurrentNumPixelsInNauticalMile, 1);
    pkRouteMap->SetTransform(pkDevice, &(pkAC->m_kGlobalToLocalTransformMatrix*kRotationMatrix*kHeadingMatrix*kScalingMatrix*rkOPMatrix));
    pkRouteMap->SetTexture(pkDevice, NULL);
    pkRouteMap->RenderPrimitives(pkDevice, 0, m_uiNumWaypoints - 1);
  }

  /////////////////////////////////////////////////////////////////////
  // RENDER CARRIER MAP
  /////////////////////////////////////////////////////////////////////
  //CARRIER *pkCARRIER  = static_cast<CARRIER *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_CARRIER));
  //if(pkCARRIER != NULL)
  //{
  //  D3DXVec3TransformCoord(&kBodyOffsetVector, pkCARRIER->GetLocalOffsetVector(), &pkAC->m_kLocalToBodyYMatrix);
  //  D3DXVECTOR3 kBodyOffsetVectorNauticalMiles = kBodyOffsetVector/1852;
  //  D3DXVECTOR3 kBodyOffsetVectorPixels = fCurrentNumPixelsInNauticalMile*kBodyOffsetVectorNauticalMiles;
  //  D3DXMatrixTranslation(&kObjectOffsetMatrix, kBodyOffsetVectorPixels.x, kBodyOffsetVectorPixels.z, 0);

  //  pkGeomObjectWaypointDesignator->SetTransform(pkDevice, &(kObjectOffsetMatrix*rkOPMatrix));
  //  pkGeomObjectWaypointDesignator->Render(pkDevice);
  //}

}
