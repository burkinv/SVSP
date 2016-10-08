#ifndef _AAV_VECTOR_3_
#define _AAV_VECTOR_3_

#include "math.h"
#include <d3dx9math.h>


template <typename T> class AAVVector3
{
public:
  AAVVector3() : x((T)0), y((T)0), z((T)0) {}

  AAVVector3(const T X, const T Y, const T Z)
  {
    x = X;
    y = Y;
    z = Z;
  }

  AAVVector3(const D3DXVECTOR3 &rkVector)
  {
    x = (T)rkVector.x;
    y = (T)rkVector.y;
    z = (T)rkVector.z;
  }

  ~AAVVector3(){}

  inline AAVVector3<T> operator= (const AAVVector3<T>& v2)
  {
    x = v2.x;
    y = v2.y;
    z = v2.z;

    return *this;
  }

  inline T& operator[] (int i) const
  {
    return (T&) *(&x + i);
  }

  inline AAVVector3<T> operator* (const float f) const
  {
    return AAVVector3<T>(f*this->x, f*this->y, f*this->z);
  }

  inline AAVVector3<T> operator* (const double d) const
  {
    T t = (T)d;
    return AAVVector3<T>(t*this->x, t*this->y, t*this->z);
  }

  inline AAVVector3<T> operator/ (const float f) const
  {
    return AAVVector3<T>(this->x/f, this->y/f, this->z/f);
  }

  inline AAVVector3<T> operator/ (const int f) const
  {
    return AAVVector3<T>(this->x/f, this->y/f, this->z/f);
  }

  inline AAVVector3<T> operator/ (const double d) const
  {
    T t = (T)d;
    return AAVVector3<T>(this->x/t, this->y/t, this->z/t);
  }

  //Right-operand matrix multiplication
  inline AAVVector3<T> operator* (const D3DXMATRIXA16& rkMatrix) const
  {
    D3DXVECTOR3 kTransformedVector;
    D3DXVec3TransformCoord(&kTransformedVector, &D3DXVECTOR3(this->x, this->y, this->z), &rkMatrix);
    return AAVVector3<T>(kTransformedVector.x, kTransformedVector.y, kTransformedVector.z);
  }

  inline AAVVector3<T> operator+ (const AAVVector3<T>& v2) const
  {
    return AAVVector3<T>(this->x + v2.x, this->y + v2.y, this->z + v2.z);
  }

  inline AAVVector3<T> operator- (const AAVVector3<T>& v2) const
  {
    return AAVVector3<T>(this->x - v2.x, this->y - v2.y, this->z - v2.z);
  }

  inline bool  operator== (const AAVVector3<T>& v2) const
  {
    return ((this->x == v2.x) && (this->y == v2.y) && (this->z == v2.z));
  }

  inline bool operator!= (const AAVVector3<T>& v2) const
  {
    return ((this->x != v2.x) || (this->y != v2.y) || (this->z != v2.z));
  }

  inline operator D3DXVECTOR3 ()
  {
    return D3DXVECTOR3((FLOAT)x, (FLOAT)y, (FLOAT)z);
  }

  inline T QuadLength() const
  {
    return (x*x + y*y + z*z);
  }

  inline T Length() const
  {
    return sqrt(QuadLength());
  }

  inline T XZLength() const
  {
    return sqrt(x*x + z*z);
  }

  inline AAVVector3<T> Normalize() 
  {
    T len = Length();
    if(len != 0)
    {
      return AAVVector3<T>(x/len, y/len, z/len);
    }
    else
    {
      return AAVVector3<T>(0, 0, 0);
    }
  }

  /** Calculates 3D-cross-product of both vectors
  @param rkVector Second vector.
  @return 3D-cross-product (means the vector that is orthogonal to both used vectors). */
  inline AAVVector3<T> Cross(const AAVVector3<T>& rkVector) const
  {
    AAVVector3<T> kCross;

    kCross.x = y*rkVector.z - z*rkVector.y;
    kCross.y = z*rkVector.x - x*rkVector.z;
    kCross.z = x*rkVector.y - y*rkVector.x;

    return kCross;
  }

  /** Calculates scalar-product of both vectors
  @param rkVector Second vector.
  @return scalar-product. */
  inline T Dot(const AAVVector3<T>& rkVector) const
  {
    return (x*rkVector.x + y*rkVector.y + z*rkVector.z);
  }

  T x, y, z;

};


  //Left-operand matrix multiplication
  template<typename T> inline AAVVector3<T> operator* (const D3DXMATRIXA16& rkMatrix, const AAVVector3<T> &rkVector)
  {
    D3DXVECTOR3 kTransformedVector;
    D3DXVec3TransformCoord(&kTransformedVector, &D3DXVECTOR3((FLOAT)rkVector.x, (FLOAT)rkVector.y, (FLOAT)rkVector.z), &rkMatrix);
    return AAVVector3<T>(kTransformedVector.x, kTransformedVector.y, kTransformedVector.z);
  }

  template<typename T> T sign(T value)
  {
    return (value < 0) ? (T)-1 : (T)1;
  }

#endif //_AAV_VECTOR_3_
