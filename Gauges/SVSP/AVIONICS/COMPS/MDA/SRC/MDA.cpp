#include "AC.h"
#include "MDA.h"
#include "WPN.h"
#include "SS.h"
#include "CARRIER.h"
#include "AIP.h"
#include "SMS.h"
#include "HARM.h"
#include "AGM.h"
#include "SRM.h"
#include "MRM.h"
#include "LGB.h"
#include "SAMCS.h"
#include "msxml2.h"

MDA::MDA(__SVSPAvionics *pkAvionics, DWORD dwThreadSleepTime, LPWSTR lpMissionFile) : __SVSPModule("MDA", pkAvionics, dwThreadSleepTime, false, NULL, NULL, GEOM_OBJECT_NUM_OBJECTS)
{
  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  __SVSPVertex*        pVerts       = NULL;

  ///////////////////////////////////////////////////////////////////
  // GEOM_OBJECT_GROUND_TARGET_DESIGNATOR
  ///////////////////////////////////////////////////////////////////
  pkGeomObject = new D3DXGEOMETRYOBJECT(D3DXGEOMETRYOBJECT::TRIANGLESTRIP, NUM_GROUND_TARGET_DESIGNATOR_VERTICES);
  pkGeomObject->SetMaterial(1.0f, 1.0f, 1.0f, 1.0f);
  pVerts = pkGeomObject->GetVerts();
  pVerts[0] = __SVSPVertex(-2.0f,  2.0f, 0, 0, 0, 0xff0010ff);
  pVerts[1] = __SVSPVertex( 2.0f,  2.0f, 0, 0, 0, 0xff0010ff);
  pVerts[2] = __SVSPVertex(-2.0f, -2.0f, 0, 0, 0, 0xff0010ff);
  pVerts[3] = __SVSPVertex( 2.0f, -2.0f, 0, 0, 0, 0xff0010ff);
  m_ppkGeomObjects[GEOM_OBJECT_GROUND_TARGET_DESIGNATOR] = pkGeomObject;

  memset(&m_apkTargets[0],  0, sizeof(m_apkTargets));  
  m_uiNumTargets = 0;

  if(lpMissionFile != NULL)
  {
    //Read mission data.
    ReadConfigFile(lpMissionFile);
  }
}

MDA::~MDA()
{
  Stop();

  DeleteAllTargets();
}

VOID MDA::DeleteAllTargets()
{
  Stop();

  for(UINT i = 0; i < MDA::MAX_NUM_TARGETS; ++i)
  {
    __SVSPModule *pkTarget = m_apkTargets[i];
    if((pkTarget != NULL))
    {
      delete pkTarget;
      m_apkTargets[i] = NULL;
    }
  }

  //Zero memory.
  memset(&m_apkTargets[0],  0, sizeof(m_apkTargets));  
  m_uiNumTargets = 0;
}

FLOAT MDA::GetRandomValueWithinLatitudeRange(FLOAT dValue1, FLOAT dValue2) const
{
  //Boundary checks
  AAV_ASSERT_RETURN(((abs(dValue1) <= 90.0f) && (abs(dValue2) <= 90.0f)), 0); 

  //Get range
  FLOAT dMinValue = min(dValue1, dValue2);
  FLOAT dMaxValue = max(dValue1, dValue2);
  FLOAT dRange = dMaxValue - dMinValue;

  if(dRange != 0)
  {
    INT iRange = (INT)(dRange*100.0);
    //Set random Seed
    srand(GetTickCount());
    //Get random value within the given integer range
    INT iRandomNumber = rand()%iRange;
    FLOAT dRandomNumber = iRandomNumber/100.0f;

    return dMinValue + dRandomNumber;
  }
  else
  {
    return dMinValue;
  }
}

FLOAT MDA::GetRandomValueWithinLongitudeRange(FLOAT dValue1, FLOAT dValue2) const
{
  //Boundary checks
  FLOAT dAbsValue1 = abs(dValue1);
  FLOAT dAbsValue2 = abs(dValue2);

  AAV_ASSERT_RETURN(((abs(dValue1) < 180.0f) && (abs(dValue2) < 180.0f)), 0); 

  //Get range
  FLOAT dMinValue = min(dValue1, dValue2);
  FLOAT dMaxValue = max(dValue1, dValue2);
  FLOAT dRange = dMaxValue - dMinValue;
  dRange = (dRange > 180.0f) ? (dRange - 360.0f) : dRange;

  if(dRange != 0)
  {
    INT iRange = (INT)(dRange*100.0);
    //Set random Seed
    srand(GetTickCount());
    //Get random value within the given integer range
    INT iRandomNumber = sign(iRange)*(rand()%iRange);
    FLOAT dRandomNumber = iRandomNumber/100.0f;

    FLOAT dPreliminaryValue = dMinValue + dRandomNumber;
    return (abs(dPreliminaryValue) > 180.0f) ? sign(dPreliminaryValue)*(360.0f - abs(dPreliminaryValue)) : dPreliminaryValue;
  }
  else
  {
    return dMinValue;
  }
}


VOID MDA::InitWeaponObjects(IXMLDOMNodeList *pObjectsNodeList)
{
  HRESULT              hResult;
  IXMLDOMNamedNodeMap *pAttributeMap          = NULL;
  IXMLDOMNode         *pNodeAttribute         = NULL;
  BSTR                 pStr                   = NULL;
  UINT                 uiStationID;
  UINT                 uiUnitID;
  VARIANT              kValue;

  LONG uiNumObjects = 0;
  if(SUCCEEDED(pObjectsNodeList->get_length(&uiNumObjects)))
  {
    for(LONG i = 0; i < uiNumObjects; ++i)
    {
      IXMLDOMNode *pObjectNode;
      if(SUCCEEDED(pObjectsNodeList->get_item(i, &pObjectNode)))
      {
        if(pStr != NULL) 
          SysFreeString(pStr);
        if(SUCCEEDED(pObjectNode->get_nodeName(&pStr)))
        {
          /////////////////////////////////////////////////
          // PILONS
          /////////////////////////////////////////////////
          if((wcscmp(pStr, L"Pilon") == 0))
          {
            hResult = pObjectNode->get_attributes(&pAttributeMap);
            AAV_ASSERT(SUCCEEDED(hResult));

            //station_id
            hResult = pAttributeMap->getNamedItem(L"station_id", &pNodeAttribute);
            AAV_ASSERT(SUCCEEDED(hResult));
            pNodeAttribute->get_nodeValue(&kValue);
            uiStationID = _wtoi(kValue.bstrVal);
            pNodeAttribute->Release();
            pNodeAttribute = NULL;

            //unit_id
            hResult = pAttributeMap->getNamedItem(L"unit_id", &pNodeAttribute);
            AAV_ASSERT(SUCCEEDED(hResult));
            pNodeAttribute->get_nodeValue(&kValue);
            uiUnitID = _wtoi(kValue.bstrVal);
            pNodeAttribute->Release();
            pNodeAttribute = NULL;

            CHAR acStr[128];
            sprintf_s(&acStr[0], sizeof(acStr), "GENERAL_VISIBILITY_ID_%d_%d", uiStationID, uiUnitID);
            ID kID = check_named_variable(&acStr[0]);
            set_named_variable_value(kID, (BOOL)1);

            pAttributeMap->Release();
            pAttributeMap = NULL;

            IXMLDOMNodeList *pPilonObjectsNodeList = NULL;
            if(SUCCEEDED(pObjectNode->get_childNodes(&pPilonObjectsNodeList)))
            {
              InitWeaponObjects(pPilonObjectsNodeList);
            }
            pPilonObjectsNodeList->Release();
            pPilonObjectsNodeList = NULL;
          }
          else
          /////////////////////////////////////////////////
          // LAUNCHERS
          /////////////////////////////////////////////////
          if((wcscmp(pStr, L"Launcher") == 0))
          {
            hResult = pObjectNode->get_attributes(&pAttributeMap);
            AAV_ASSERT(SUCCEEDED(hResult));

            //station_id
            hResult = pAttributeMap->getNamedItem(L"station_id", &pNodeAttribute);
            AAV_ASSERT(SUCCEEDED(hResult));
            pNodeAttribute->get_nodeValue(&kValue);
            uiStationID = _wtoi(kValue.bstrVal);
            pNodeAttribute->Release();
            pNodeAttribute = NULL;

            //unit_id
            hResult = pAttributeMap->getNamedItem(L"unit_id", &pNodeAttribute);
            AAV_ASSERT(SUCCEEDED(hResult));
            pNodeAttribute->get_nodeValue(&kValue);
            uiUnitID = _wtoi(kValue.bstrVal);
            pNodeAttribute->Release();
            pNodeAttribute = NULL;

            CHAR acStr[128];
            sprintf_s(&acStr[0], sizeof(acStr), "GENERAL_VISIBILITY_ID_%d_%d", uiStationID, uiUnitID);
            ID kID = check_named_variable(&acStr[0]);
            set_named_variable_value(kID, (BOOL)1);

            pAttributeMap->Release();
            pAttributeMap = NULL;

            IXMLDOMNodeList *pLauncherObjectsNodeList = NULL;
            if(SUCCEEDED(pObjectNode->get_childNodes(&pLauncherObjectsNodeList)))
            {
              InitWeaponObjects(pLauncherObjectsNodeList);
            }
            pLauncherObjectsNodeList->Release();
            pLauncherObjectsNodeList = NULL;
          }
          else
          /////////////////////////////////////////////////
          // WEAPONS
          /////////////////////////////////////////////////
          if((wcscmp(pStr, L"Missile") == 0))
          {
            SMS *pkSMS = static_cast<SMS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS));
            if(pkSMS != NULL)
            {
              AAVVector3<FLOAT>   kPilonVector;
              AAVVector3<FLOAT>   kPilonAttitude;
              WCHAR                awcName[128];
              CHAR                 acName[128];
              CHAR                 acType[128];
              UINT                 uiWeight;

              hResult = pObjectNode->get_attributes(&pAttributeMap);
              AAV_ASSERT(SUCCEEDED(hResult));

              //station_id
              hResult = pAttributeMap->getNamedItem(L"station_id", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              uiStationID = _wtoi(kValue.bstrVal);
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //unit_id
              hResult = pAttributeMap->getNamedItem(L"unit_id", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              uiUnitID = _wtoi(kValue.bstrVal);
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //name
              hResult = pAttributeMap->getNamedItem(L"name", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              wcscpy_s(&awcName[0], sizeof(awcName)/sizeof(WCHAR), kValue.bstrVal);
              wcstombs(&acName[0], &awcName[0], sizeof(acName));
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //type
              hResult = pAttributeMap->getNamedItem(L"type", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              wcscpy_s(&awcName[0], sizeof(awcName)/sizeof(WCHAR), kValue.bstrVal);
              wcstombs(&acType[0], &awcName[0], sizeof(acType));
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //weight
              hResult = pAttributeMap->getNamedItem(L"weight", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              uiWeight = _wtoi(kValue.bstrVal);
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //x
              hResult = pAttributeMap->getNamedItem(L"x", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              kPilonVector.x = (FLOAT)_wtof(kValue.bstrVal);
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //y
              hResult = pAttributeMap->getNamedItem(L"y", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              kPilonVector.y = (FLOAT)_wtof(kValue.bstrVal);
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //z
              hResult = pAttributeMap->getNamedItem(L"z", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              kPilonVector.z = (FLOAT)_wtof(kValue.bstrVal);
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //pitch
              hResult = pAttributeMap->getNamedItem(L"pitch", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              kPilonAttitude.x = (FLOAT)_wtof(kValue.bstrVal);
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //yaw
              hResult = pAttributeMap->getNamedItem(L"yaw", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              kPilonAttitude.y = (FLOAT)_wtof(kValue.bstrVal);
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              //roll
              hResult = pAttributeMap->getNamedItem(L"roll", &pNodeAttribute);
              AAV_ASSERT(SUCCEEDED(hResult));
              pNodeAttribute->get_nodeValue(&kValue);
              kPilonAttitude.z = (FLOAT)_wtof(kValue.bstrVal);
              pNodeAttribute->Release();
              pNodeAttribute = NULL;

              if(strlen(&acType[0]) != 0)
              {
                if(strcmp(&acType[0], "SRM") == 0)
                {
                  pkSMS->AddStore(new SRM(&acName[0], uiStationID, uiUnitID, uiWeight, m_pkAvionics, kPilonVector, kPilonAttitude));
                }
                else 
                if(strcmp(&acType[0], "MRM") == 0)
                {
                  pkSMS->AddStore(new MRM(&acName[0], uiStationID, uiUnitID, uiWeight, m_pkAvionics, kPilonVector, kPilonAttitude));
                }
                else 
                if(strcmp(&acType[0], "ARM") == 0)
                {
                  pkSMS->AddStore(new HARM(&acName[0], uiStationID, uiUnitID, uiWeight, m_pkAvionics, kPilonVector, kPilonAttitude));
                }
                else 
                if(strcmp(&acType[0], "AGM") == 0)
                {
                  pkSMS->AddStore(new AGM(&acName[0], uiStationID, uiUnitID, uiWeight, m_pkAvionics, kPilonVector, kPilonAttitude));
                }
                else 
                if(strcmp(&acType[0], "LGB") == 0)
                {
                  pkSMS->AddStore(new LGB(&acName[0], uiStationID, uiUnitID, uiWeight, m_pkAvionics, kPilonVector, kPilonAttitude));
                }
              }

              pAttributeMap->Release();
              pAttributeMap = NULL;
            }
          }
        }
        pObjectNode->Release();
        pObjectNode = NULL;
      }
    }
  }
}

VOID MDA::ReadConfigFile(LPWSTR lpMissionDataFileName, LPWSTR lpReadSectionName)
{
  HRESULT              hResult;
  VARIANT              vSrc;
  VARIANT_BOOL         bStatus;
  IXMLDOMDocument     *pMissionConfig         = NULL;
  IXMLDOMNodeList     *pMissionConfigNodeList = NULL;
  IXMLDOMNode         *pMissionConfigNode     = NULL;
  IXMLDOMNode         *pMissionConfigNodeAttribute = NULL;
  IXMLDOMNamedNodeMap *pAttributeMap          = NULL;
  BSTR                 pStr                   = NULL;

  CoInitialize(NULL);
  if(SUCCEEDED(CoCreateInstance(__uuidof(DOMDocument40), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pMissionConfig)))
  {
    if(pMissionConfig != NULL)
    {
      pMissionConfig->put_async(VARIANT_FALSE);
      pMissionConfig->put_validateOnParse(VARIANT_FALSE);
      pMissionConfig->put_resolveExternals(VARIANT_FALSE);
      VariantInit(&vSrc);
      V_BSTR(&vSrc) = SysAllocString(lpMissionDataFileName);
      V_VT(&vSrc) = VT_BSTR;
      if(SUCCEEDED(pMissionConfig->load(vSrc, &bStatus)))
      {
        if(bStatus == VARIANT_TRUE)
        {
          if(SUCCEEDED(pMissionConfig->selectNodes(L"//MissionConfig[1]/*", &pMissionConfigNodeList)))
          {
            LONG uiNumNodes = 0;
            if(SUCCEEDED(pMissionConfigNodeList->get_length(&uiNumNodes)))
            {
              for(LONG i = 0; i < uiNumNodes; ++i)
              {
                if(SUCCEEDED(pMissionConfigNodeList->get_item(i, &pMissionConfigNode)))
                {
                  if(pStr != NULL) 
                    SysFreeString(pStr);
                  if(SUCCEEDED(pMissionConfigNode->get_nodeName(&pStr)))
                  {
                    ///////////////////////////////////////////////////////////////////
                    // <!--CARRIER SECTION-->
                    ///////////////////////////////////////////////////////////////////
                    if(wcscmp(pStr, L"CarrierList") == 0)
                    {
                      if((lpReadSectionName == NULL) || (wcscmp(lpReadSectionName, pStr) == 0))
                      {
                        IXMLDOMNodeList     *pCarrierListNodeList = NULL;
                        if(SUCCEEDED(pMissionConfigNode->get_childNodes(&pCarrierListNodeList)))
                        {
                          LONG uiNumCarrierListNodes = 0;
                          if(SUCCEEDED(pCarrierListNodeList->get_length(&uiNumCarrierListNodes)))
                          {
                            for(LONG i = 0; i < uiNumCarrierListNodes; ++i)
                            {
                              IXMLDOMNode *pCarrierNode;
                              if(SUCCEEDED(pCarrierListNodeList->get_item(i, &pCarrierNode)))
                              {
                                if(pStr != NULL) 
                                  SysFreeString(pStr);
                                if(SUCCEEDED(pCarrierNode->get_nodeName(&pStr)))
                                {
                                  if((wcscmp(pStr, L"Carrier") == 0) && 
                                    (m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_CARRIER) == NULL) && 
                                    (m_uiNumTargets < MDA::MAX_NUM_TARGETS))
                                  {
                                    FLOAT                 dLatitudeMin;
                                    FLOAT                 dLatitudeMax;
                                    FLOAT                 dLongitudeMin;
                                    FLOAT                 dLongitudeMax;
                                    __SVSPGeoCoordinates3<FLOAT> kLocation;
                                    AAVVector3<FLOAT>    kAttitude;
                                    WCHAR                  awcName[128];
                                    CHAR                   acName[128];
                                    VARIANT                kValue;

                                    hResult = pCarrierNode->get_attributes(&pAttributeMap);
                                    AAV_ASSERT(SUCCEEDED(hResult));

                                    //Name
                                    hResult = pAttributeMap->getNamedItem(L"name", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    wcscpy_s(&awcName[0], sizeof(awcName)/sizeof(WCHAR), kValue.bstrVal);
                                    wcstombs(&acName[0], &awcName[0], sizeof(acName));
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Latitude (Minimal)
                                    hResult = pAttributeMap->getNamedItem(L"lat_min", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    dLatitudeMin = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Latitude (Maximal)
                                    hResult = pAttributeMap->getNamedItem(L"lat_max", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    dLatitudeMax = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Longitude (Minimal)
                                    hResult = pAttributeMap->getNamedItem(L"lon_min", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    dLongitudeMin = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Longitude (Maximal)
                                    hResult = pAttributeMap->getNamedItem(L"lon_max", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    dLongitudeMax = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Altitude
                                    hResult = pAttributeMap->getNamedItem(L"alt", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    kLocation.Altitude = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //pitch
                                    hResult = pAttributeMap->getNamedItem(L"pitch", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    kAttitude.x = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //yaw
                                    hResult = pAttributeMap->getNamedItem(L"hdg", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    kAttitude.y = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //roll
                                    hResult = pAttributeMap->getNamedItem(L"roll", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    kAttitude.z = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    kLocation.Latitude  = GetRandomValueWithinLatitudeRange(dLatitudeMin, dLatitudeMax);
                                    kLocation.Longitude = GetRandomValueWithinLongitudeRange(dLongitudeMin, dLongitudeMax);

                                    CARRIER *pkCARRIER = new CARRIER(m_pkAvionics, 100, &acName[0], &kLocation, &kAttitude);
                                    m_pkAvionics->SetModule(__SVSPAvionics::AVIONICS_MODULE_CARRIER, pkCARRIER);
                                    //AddTarget(pkCARRIER);
                                    pAttributeMap->Release();
                                    pAttributeMap = NULL;
                                  }
                                }
                                pCarrierNode->Release();
                              }
                            }
                          }
                          pCarrierListNodeList->Release();
                        }
                      }
                    }
                    /////////////////////////////////////////////////////////////////////
                    //   <!--AAVWAYPOINT LIST SECTION-->
                    ///////////////////////////////////////////////////////////////////
                    //else 
                    if(wcscmp(pStr, L"WaypointList") == 0)
                    {
                      if((lpReadSectionName == NULL) || (wcscmp(lpReadSectionName, pStr) == 0))
                      {
                        WPN *pkWPN = static_cast<WPN *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_WPN));
                        if(pkWPN != NULL)
                        {
                          IXMLDOMNodeList     *pWaypointListNodeList = NULL;
                          if(SUCCEEDED(pMissionConfigNode->get_childNodes(&pWaypointListNodeList)))
                          {
                            LONG uiNumWaypointListNodes = 0;
                            if(SUCCEEDED(pWaypointListNodeList->get_length(&uiNumWaypointListNodes)))
                            {
                              for(LONG i = 0; i < uiNumWaypointListNodes; ++i)
                              {
                                IXMLDOMNode *pWaypointNode;
                                if(SUCCEEDED(pWaypointListNodeList->get_item(i, &pWaypointNode)))
                                {
                                  if(pStr != NULL) 
                                    SysFreeString(pStr);
                                  if(SUCCEEDED(pWaypointNode->get_nodeName(&pStr)))
                                  {
                                    if((wcscmp(pStr, L"Waypoint") == 0) && (pkWPN->GetNumWaypoints() < WPN::MAX_NUM_WAYPOINTS))
                                    {
                                      __SVSPGeoCoordinates3<FLOAT> kLocation;
                                      FLOAT                  fHeading;
                                      WCHAR                  awcName[128];
                                      CHAR                   acName[128];
                                      VARIANT                kValue;

                                      hResult = pWaypointNode->get_attributes(&pAttributeMap);
                                      AAV_ASSERT(SUCCEEDED(hResult));
                                      //Name
                                      hResult = pAttributeMap->getNamedItem(L"name", &pMissionConfigNodeAttribute);
                                      AAV_ASSERT(SUCCEEDED(hResult));
                                      pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                      wcscpy_s(&awcName[0], sizeof(awcName)/sizeof(WCHAR), kValue.bstrVal);
                                      wcstombs(&acName[0], &awcName[0], sizeof(acName));
                                      pMissionConfigNodeAttribute->Release();
                                      pMissionConfigNodeAttribute = NULL;

                                      //Latitude
                                      hResult = pAttributeMap->getNamedItem(L"lat", &pMissionConfigNodeAttribute);
                                      AAV_ASSERT(SUCCEEDED(hResult));
                                      pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                      kLocation.Latitude = (FLOAT)_wtof(kValue.bstrVal);
                                      pMissionConfigNodeAttribute->Release();
                                      pMissionConfigNodeAttribute = NULL;

                                      //Longitude
                                      hResult = pAttributeMap->getNamedItem(L"lon", &pMissionConfigNodeAttribute);
                                      AAV_ASSERT(SUCCEEDED(hResult));
                                      pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                      kLocation.Longitude = (FLOAT)_wtof(kValue.bstrVal);
                                      pMissionConfigNodeAttribute->Release();
                                      pMissionConfigNodeAttribute = NULL;

                                      //Altitude
                                      hResult = pAttributeMap->getNamedItem(L"alt", &pMissionConfigNodeAttribute);
                                      AAV_ASSERT(SUCCEEDED(hResult));
                                      pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                      kLocation.Altitude = (FLOAT)_wtof(kValue.bstrVal);
                                      pMissionConfigNodeAttribute->Release();
                                      pMissionConfigNodeAttribute = NULL;

                                      //Heading
                                      hResult = pAttributeMap->getNamedItem(L"hdg", &pMissionConfigNodeAttribute);
                                      AAV_ASSERT(SUCCEEDED(hResult));
                                      pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                      fHeading = (FLOAT)_wtof(kValue.bstrVal);
                                      pMissionConfigNodeAttribute->Release();
                                      pMissionConfigNodeAttribute = NULL;

                                      pkWPN->AddWaypoint(AAVWAYPOINT<FLOAT>(&acName[0], kLocation, fHeading));
                                      pAttributeMap->Release();
                                      pAttributeMap = NULL;
                                    }
                                  }
                                }
                                pWaypointNode->Release();
                              }
                            }
                          }
                          pWaypointListNodeList->Release();
                        }
                      }
                    }
                    ///////////////////////////////////////////////////////////////////
                    // <!--WINGMAN SECTION-->
                    ///////////////////////////////////////////////////////////////////
                    //else if(wcscmp(pStr, L"Wingman") == 0)
                    //{
                    //  if(m_uiNumAIP < MDA::MAX_NUM_AIP)
                    //  {
                    //    WCHAR                  awcName[128];
                    //    CHAR                   acName[128];
                    //    VARIANT                kValue;

                    //    hResult = pMissionConfigNode->get_attributes(&pAttributeMap);
                    //    AAV_ASSERT(SUCCEEDED(hResult));
                    //    //Name
                    //    hResult = pAttributeMap->getNamedItem(L"name", &pMissionConfigNodeAttribute);
                    //    AAV_ASSERT(SUCCEEDED(hResult));
                    //    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                    //    wcscpy_s(&awcName[0], sizeof(awcName)/sizeof(WCHAR), kValue.bstrVal);
                    //    wcstombs(&acName[0], &awcName[0], sizeof(acName));
                    //    pMissionConfigNodeAttribute->Release();
                    //    pMissionConfigNodeAttribute = NULL;

                    //    m_apkAIP[m_uiNumAIP] = new AIP(m_pkAvionics, &acName[0], "NK306");
                    //    AddTarget(m_apkAIP[m_uiNumAIP]);
                    //    m_uiNumAIP++;
                    //    pAttributeMap->Release();
                    //    pAttributeMap = NULL;
                    //  }
                    //}
                    ///////////////////////////////////////////////////////////////////
                    // <!--TARGETS SECTION-->
                    ///////////////////////////////////////////////////////////////////
                    else if(wcscmp(pStr, L"TargetList") == 0)
                    {
                      if((lpReadSectionName == NULL) || (wcscmp(lpReadSectionName, pStr) == 0))
                      {
                        IXMLDOMNodeList     *pTargetListNodeList = NULL;
                        if(SUCCEEDED(pMissionConfigNode->get_childNodes(&pTargetListNodeList)))
                        {
                          LONG uiNumTargetListNodes = 0;
                          if(SUCCEEDED(pTargetListNodeList->get_length(&uiNumTargetListNodes)))
                          {
                            for(LONG i = 0; i < uiNumTargetListNodes; ++i)
                            {
                              IXMLDOMNode *pTargetNode;
                              if(SUCCEEDED(pTargetListNodeList->get_item(i, &pTargetNode)))
                              {
                                if(pStr != NULL) 
                                  SysFreeString(pStr);
                                if(SUCCEEDED(pTargetNode->get_nodeName(&pStr)))
                                {
                                  if((wcscmp(pStr, L"Target") == 0) && (m_uiNumTargets < MDA::MAX_NUM_TARGETS))
                                  {
                                    FLOAT dLatitudeMin;
                                    FLOAT dLatitudeMax;
                                    FLOAT dLongitudeMin;
                                    FLOAT dLongitudeMax;
                                    __SVSPGeoCoordinates3<FLOAT> kLocation;
                                    AAVVector3<FLOAT>    kAttitude;
                                    WCHAR                  awcName[128];
                                    CHAR                   acName[128];
                                    VARIANT                kValue;

                                    hResult = pTargetNode->get_attributes(&pAttributeMap);
                                    AAV_ASSERT(SUCCEEDED(hResult));

                                    //Name
                                    hResult = pAttributeMap->getNamedItem(L"name", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    wcscpy_s(&awcName[0], sizeof(awcName)/sizeof(WCHAR), kValue.bstrVal);
                                    wcstombs(&acName[0], &awcName[0], sizeof(acName));
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Latitude (Minimal)
                                    hResult = pAttributeMap->getNamedItem(L"lat_min", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    dLatitudeMin = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Latitude (Maximal)
                                    hResult = pAttributeMap->getNamedItem(L"lat_max", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    dLatitudeMax = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Longitude (Minimal)
                                    hResult = pAttributeMap->getNamedItem(L"lon_min", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    dLongitudeMin = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Longitude (Maximal)
                                    hResult = pAttributeMap->getNamedItem(L"lon_max", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    dLongitudeMax = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //Altitude
                                    hResult = pAttributeMap->getNamedItem(L"alt", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    kLocation.Altitude = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //pitch
                                    hResult = pAttributeMap->getNamedItem(L"pitch", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    kAttitude.x = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //yaw
                                    hResult = pAttributeMap->getNamedItem(L"hdg", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    kAttitude.y = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    //roll
                                    hResult = pAttributeMap->getNamedItem(L"roll", &pMissionConfigNodeAttribute);
                                    AAV_ASSERT(SUCCEEDED(hResult));
                                    pMissionConfigNodeAttribute->get_nodeValue(&kValue);
                                    kAttitude.z = (FLOAT)_wtof(kValue.bstrVal);
                                    pMissionConfigNodeAttribute->Release();
                                    pMissionConfigNodeAttribute = NULL;

                                    kLocation.Latitude  = GetRandomValueWithinLatitudeRange(dLatitudeMin, dLatitudeMax);
                                    kLocation.Longitude = GetRandomValueWithinLongitudeRange(dLongitudeMin, dLongitudeMax);

                                    SAMCS *pkSAMCS = new SAMCS(m_pkAvionics, 100, &acName[0], &kLocation, &kAttitude);
                                    AddTarget(pkSAMCS);

                                    pAttributeMap->Release();
                                    pAttributeMap = NULL;
                                  }
                                }
                              }
                              pTargetNode->Release();
                            }
                          }
                        }
                        pTargetListNodeList->Release();
                      }
                      }
                    ///////////////////////////////////////////////////////////////////
                    // <!--WEAPONS SECTION-->
                    ///////////////////////////////////////////////////////////////////
                    else if(wcscmp(pStr, L"WeaponList") == 0)
                    {
                      if((lpReadSectionName == NULL) || (wcscmp(lpReadSectionName, pStr) == 0))
                      {
                        SMS *pkSMS = static_cast<SMS *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS));
                        if(pkSMS != NULL)
                        {
                          IXMLDOMNodeList     *pWeaponListNodeList = NULL;
                          if(SUCCEEDED(pMissionConfigNode->get_childNodes(&pWeaponListNodeList)))
                          {
                            InitWeaponObjects(pWeaponListNodeList);
                            pWeaponListNodeList->Release();
                            pWeaponListNodeList = NULL;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }

          if(pStr != NULL) 
            SysFreeString(pStr);
        }
      }

      VariantClear(&vSrc);
      pMissionConfig->Release();

      // Update WPN::RouteMap vertices
      WPN *pkWPN = (WPN *)m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_WPN);
      if(pkWPN != NULL)
      {
        pkWPN->GenerateRouteMap();
      }
    }
  }
  CoUninitialize();
}

VOID MDA::AddTarget(__SVSPModule *pkTarget)
{
  AAV_ASSERT_RETURN_VOID(pkTarget != NULL);
  AAV_ASSERT_RETURN_VOID(m_uiNumTargets < MDA::MAX_NUM_TARGETS);
  m_apkTargets[m_uiNumTargets] = pkTarget;
  m_uiNumTargets++;
}

VOID MDA::Start()
{
  // START ALL TARGETS
  for(UINT i = 0; i < MDA::MAX_NUM_TARGETS; ++i)
  {
    __SVSPModule *pkTarget = m_apkTargets[i];
    if((pkTarget != NULL) && !pkTarget->IsStarted())
    {
      pkTarget->Start();
    }
  }

  __SVSPModule::Start();
}

VOID MDA::Stop()
{
  __SVSPModule::Stop();

  // STOP ALL TARGETS
  for(UINT i = 0; i < MDA::MAX_NUM_TARGETS; ++i)
  {
    __SVSPModule *pkTarget = m_apkTargets[i];
    if((pkTarget != NULL) && pkTarget->IsStarted())
    {
      pkTarget->Stop();
    }
  }
}

VOID MDA::Process()
{
  //MSG kMsg;

  __SVSPModule *pkFLCS = m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_FLCS);
  __SVSPModule *pkSMS  = m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_SMS);

  //if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
  //{
  //  switch(kMsg.message)
  //  {
  //  case MDA::MESSAGE_CREATE_AIP:
  //    for(UINT i = 0; i < m_uiNumTargets; ++i)
  //    {
  //      __SVSPModule *pkTarget = m_apkTargets[i];
  //      if(pkTarget != NULL) 
  //      {
  //        if(pkTarget->GetName() == __SVSPString<CHAR>("AIP")) == 0)
  //        {
  //          PostThreadMessage(pkTarget->GetThreadId(), AIP::MESSAGE_CREATE, NULL, NULL);
  //        }
  //      }
  //    }
  //    break;

  //  case MDA::MESSAGE_DESTROY_AIP:
  //    for(UINT i = 0; i < m_uiNumTargets; ++i)
  //    {
  //      __SVSPModule *pkTarget = m_apkTargets[i];
  //      if(pkTarget != NULL) 
  //      {
  //        if(strcmp(pkTarget->GetName(), "AIP") == 0)
  //        {
  //          PostThreadMessage(pkTarget->GetThreadId(), AIP::MESSAGE_DESTROY, NULL, NULL);
  //        }
  //      }
  //    }
  //    break;

  //  default:
  //    break;
  //  }
  //}
}

VOID MDA::Release()
{
  __SVSPModule::Release();
  for(UINT i = 0; i < m_uiNumTargets; ++i)
  {
    __SVSPModule *pkTarget = m_apkTargets[i];
    if(pkTarget != NULL)
    {
      pkTarget->Release();
    }
  }
}

HRESULT MDA::Reset(LPD3DXDEVICE pkDevice)
{
  HRESULT hResult = S_OK;
  __SVSPModule::Reset(pkDevice);
  for(UINT i = 0; i < m_uiNumTargets; ++i)
  {
    __SVSPModule *pkTarget = m_apkTargets[i];
    if(pkTarget != NULL)
    {
      hResult = pkTarget->Reset(pkDevice);
    }
  }

  return hResult;
}

VOID MDA::Render(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix)
{
  AC  *pkAC  = static_cast<AC *>(m_pkAvionics->GetModule(__SVSPAvionics::AVIONICS_MODULE_AC));
  AAV_ASSERT_RETURN_VOID(pkAC  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObject = NULL;
  LPAAVFONT      pkFont            = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  ////////////////////////////////////////////////////////////////////////////////
  // GROUND TARGETS
  ////////////////////////////////////////////////////////////////////////////////
  for(UINT i = 0; i <  m_uiNumTargets; ++i)
  {
    __SVSPModule *pkTarget = m_apkTargets[i];
    if((pkTarget != NULL) && pkTarget->IsValid())
    {
      D3DXVECTOR3  kLocVector;
      D3DXVECTOR3 *pkLocVector = &kLocVector;
      D3DXVECTOR3  kGlobVector = D3DXVECTOR3(pkTarget->m_kVector.x, pkTarget->m_kVector.y, pkTarget->m_kVector.z)  - pkAC->m_kGlobalVector;

      D3DXVec3TransformCoord(pkLocVector, &kGlobVector, &pkAC->m_kGlobalToLocalTransformMatrix);
      FLOAT fVXZ      =  sqrt(pkLocVector->x*pkLocVector->x + pkLocVector->z*pkLocVector->z);
      FLOAT fVXYZ     =  D3DXVec3Length(pkLocVector);
      FLOAT fSinYaw   =  pkLocVector->x/fVXZ;
      FLOAT fCosYaw   =  pkLocVector->z/fVXZ;
      FLOAT fSinPitch =  pkLocVector->y/fVXYZ;
      FLOAT fCosPitch =  fVXZ/fVXYZ;

      D3DXMATRIXA16 mkTrans;

      mkTrans._11 =  fCosYaw;           mkTrans._12 =  0;         mkTrans._13 =  fSinYaw;           mkTrans._14 =  0;
      mkTrans._21 =  fSinPitch*fSinYaw; mkTrans._22 =  fCosPitch; mkTrans._23 = -fSinPitch*fCosYaw; mkTrans._24 =  0;
      mkTrans._31 = -fCosPitch*fSinYaw; mkTrans._32 =  fSinPitch; mkTrans._33 =  fCosPitch*fCosYaw; mkTrans._34 =  0;
      mkTrans._41 =  0;                 mkTrans._42 =  0;         mkTrans._43 =  0;                 mkTrans._44 =  1;

      pkGeomObject = m_ppkGeomObjects[GEOM_OBJECT_GROUND_TARGET_DESIGNATOR];
      if(pkGeomObject != NULL)
      {
        pkGeomObject->SetTransform(pkDevice, &(rkOPMatrix*mkTrans));
        pkGeomObject->Render(pkDevice, 0, 2);
      }

      pkTarget->RenderOnHDD(pkDevice, rkOPMatrix);
    }
  }
}

VOID MDA::Render(LPD3DXDEVICE pkDevice, D3DXMATRIX &rkOPMatrix, FLOAT fScale)
{
  __SVSPFont* pkFont = m_pkAvionics->GetFont();
  AAV_ASSERT_RETURN_VOID(pkFont  != NULL);

  LPD3DXGEOMETRYOBJECT pkGeomObjectTargetDesignator = m_ppkGeomObjects[GEOM_OBJECT_GROUND_TARGET_DESIGNATOR];
  AAV_ASSERT_RETURN_VOID(pkGeomObjectTargetDesignator  != NULL);

  D3DXMATRIXA16 kTranslationMatrix;
  D3DXMATRIXA16 kRotationMatrix;

  for(UINT i = 0; i <  m_uiNumTargets; ++i)
  {
    __SVSPModule *pkTarget = m_apkTargets[i];
    if((pkTarget != NULL) && pkTarget->IsValid())
    {
      D3DXVECTOR3 kLocalTargetVector = pkTarget->m_kLocalOffsetVector*fScale;
      D3DXMatrixTranslation(&kTranslationMatrix, -kLocalTargetVector.x, kLocalTargetVector.z, 0);
      pkGeomObjectTargetDesignator->SetTransform(pkDevice, &(kTranslationMatrix*rkOPMatrix));
      pkGeomObjectTargetDesignator->Render(pkDevice);
      //sprintf_s(&m_acStr[0], sizeof(m_acStr), "%d", i);
      //pkFont->RenderText((&m_acStr[0]), (D3DXVECTOR3 *)&UNIT_VECTOR, __SVSPFont::FILTERED|__SVSPFont::CENTERED_X|__SVSPFont::CENTERED_Y, 155.0f);
      //sprintf_s(&m_acStr[0], sizeof(m_acStr), "  %3.1f", D3DXVec3Length(&kGlobalVec)/1852.0f);
      //pkFont->RenderText((&m_acStr[0]), (D3DXVECTOR3 *)&UNIT_VECTOR, __SVSPFont::FILTERED|__SVSPFont::CENTERED_Y, 155.0f);
    }
  }
}
