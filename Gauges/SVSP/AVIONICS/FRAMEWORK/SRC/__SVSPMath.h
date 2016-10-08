#ifndef __SVSP_MATH_H__
#define __SVSP_MATH_H__

#include <windows.h>
#include "__SVSPVector3.h"
#include "__SVSPGeoCoordinates3.h"

class __SVSPMath
{
public:
  static inline FLOAT      DISCRET_SIGMA(FLOAT X, FLOAT Cx)
  {
    return (X > Cx)? 1.0f : ((X != Cx)? -1.0f : 0); 
  }

  static inline FLOAT      SIGMOID_DISTRIBUTION_FUNCTION(FLOAT X, FLOAT Cx, FLOAT K)
  {
    FLOAT dError = K*(X - Cx);
    return dError/sqrt(1.0f + dError*dError);
  }

  static inline FLOAT      SIGMOID_DISTRIBUTION_FUNCTION(FLOAT X, FLOAT Cx, FLOAT K, INT Pow)
  {
    FLOAT dError = K*(X - Cx);
    FLOAT dInvPow = (FLOAT)1/Pow;
    return dError/pow((1.0f + pow(dError, Pow)), dInvPow);
  }

  static inline FLOAT      ASYMPTOTIC_TO_ONE(FLOAT X, FLOAT Cx, FLOAT K)
  {
    FLOAT dDif = X - Cx;
    return 1.0f - 1.0f/(1.0f + K*dDif);
  }
};

#endif	/* __SVSP_MATH_H__ */


