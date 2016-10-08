#ifndef __SVSP_GEO_COORDINATES_3__
#define __SVSP_GEO_COORDINATES_3__

#include "math.h"
#include "d3dx9.h"
#include "__SVSPVector3.h"

template <typename T> class __SVSPGeoCoordinates3
{
public:
  T Latitude;   //DEGREES
  T Longitude;  //DEGREES
  T Altitude;   //FEET(MSL)

  __SVSPGeoCoordinates3() {}

  __SVSPGeoCoordinates3(T _Latitude, T _Longitude, T _Altitude) : Latitude(_Latitude), Longitude(_Longitude), Altitude(_Altitude) 
  {
  }

  __SVSPGeoCoordinates3(AAVVector3<T> kVector)
  {
    T r     = kVector.Length();
    T phi   = asin(kVector.z/r);
    T theta = asin(kVector.y/sqrt(kVector.x*kVector.x + kVector.y*kVector.y));

    Latitude  = (T)(phi*180.0f/D3DX_PI);
    Longitude = (T)((theta < 0) ? (-theta/D3DX_PI - 1)*180.0f :(theta/D3DX_PI)*180.0f);
    Altitude  = (T)(r - 6378500.0f);
  }

  ~__SVSPGeoCoordinates3()
  {
  }

  inline bool  operator== (const __SVSPGeoCoordinates3<T>& c2) const
  {
    return ((this->Latitude == c2.Latitude) && (this->Longitude == c2.Longitude) && (this->Altitude == c2.Altitude));
  }

  inline bool operator!= (const __SVSPGeoCoordinates3<T>& v2) const
  {
    return ((this->Latitude != c2.Latitude) && (this->Longitude != c2.Longitude) && (this->Altitude != c2.Altitude));
  }

  /* The function assumes that Latitude, Longitude and Altitude have TRUE navigational format:
     Altitude - in MSL (meters), Latitude and Longitude - in Degrees */
  //inline D3DXVECTOR3 GetVector3FromTrueCoords()
  //{
  //  FLOAT r     = (FLOAT)Altitude + 6378500.0f;
  //  FLOAT phi   = (FLOAT)Latitude*D3DX_PI/180.0f;
  //  FLOAT theta = (FLOAT)Longitude*D3DX_PI/180.0f;

  //  return D3DXVECTOR3((FLOAT)((r*cos(phi))*cos(theta)), (FLOAT)((r*cos(phi))*sin(theta)), (FLOAT)(r*sin(phi)));
  //}

  template< typename T> inline AAVVector3<T> GetVector3()
  {
    T r     = GetAltitude();
    T phi   = GetLatitudeAngle(); //0.37170145f
    T theta = GetLongitudeAngle();//-2.7589521f

    return AAVVector3<T>(r*cos(phi)*cos(theta), r*cos(phi)*sin(theta), r*sin(phi));
  }

  inline FLOAT GetLatitudeAngle()
  {
    return D3DX_PI/180.0f*(Latitude);     //rad
  }

  inline FLOAT GetLongitudeAngle()
  {
    return D3DX_PI/180.0f*(Longitude);    //rad
  }

  inline FLOAT GetAltitude()
  {
    return Altitude/3.2808f + 6378500.0f; //meters
  }
};

#endif //__SVSP_GEO_COORDINATES_3__
