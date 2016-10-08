#ifndef _AAV_MATRIX3_
#define _AAV_MATRIX3_

#include "math.h"
#include "__SVSPVector3.h"

template <typename T> class AAVMatrix3
{
public:
  static T EPSILON;
  /// Zero matrix. All coefficents are 0.
  static AAVMatrix3<T> ZERO;
  /// Unit or identity matrix. <TT>m[0][0]</TT>, <TT>m[1][1]</TT> and <TT>m[2][2]</TT> are 1, the other coefficients are 0.
  static AAVMatrix3<T> IDENTITY;

  // construction

  /** Standard constructor. For efficient construction of large matrix arrays, there is no initialisation. */
  AAVMatrix3();

  /** Constructor. With initialisation of all coefficients by a row of parameters.
  @param vEntry00 Coefficient <TT>m[0][0]</TT> of new matrix.
  @param vEntry01 Coefficient <TT>m[0][1]</TT> of new matrix.
  @param vEntry02 Coefficient <TT>m[0][2]</TT> of new matrix.
  @param vEntry10 Coefficient <TT>m[1][0]</TT> of new matrix.
  @param vEntry11 Coefficient <TT>m[1][1]</TT> of new matrix.
  @param vEntry12 Coefficient <TT>m[1][2]</TT> of new matrix.
  @param vEntry20 Coefficient <TT>m[2][0]</TT> of new matrix.
  @param vEntry21 Coefficient <TT>m[2][1]</TT> of new matrix.
  @param vEntry22 Coefficient <TT>m[2][2]</TT> of new matrix. */
  AAVMatrix3(T vEntry00, T vEntry01, T vEntry02,
              T vEntry10, T vEntry11, T vEntry12,
              T vEntry20, T vEntry21, T vEntry22);

  /** Constructor. With initialisation of all coefficients by an array.
  @param aavEntry Array containing the coefficients of new matrix. */
  AAVMatrix3(const T aavEntry[3][3]);

  /** Constructor. With initialisation of all coefficients by three \Ref{AAVVector3<T>}s.
  @param rkLeft Vector for the left column of the matrix.
  @param rkUp   Vector for the column in the middle of the matrix.
  @param rkDir  Vector for the right column of the matrix. */
  AAVMatrix3(const AAVVector3<T>& rkLeft, const AAVVector3<T>& rkUp, const AAVVector3<T>& rkDir);

  /** Constructor. Top left 2x2 submatrix is initialized with values from rkMatrix. 
  Additional row/column is initialized with (0,0,1)
  @param rkMatrix existing eGML_Matrix2. */
  //AAVMatrix3<T> (const eGML_Matrix2& rkMatrix);

  /** Copyconstructor.
  @param rkMatrix Matrix to copy from for new matrix. */
  AAVMatrix3(const AAVMatrix3<T>& rkMatrix);

  /** Provides access to each coefficient of the matrix. Therefor this operator returns the pointer of the chosen <I>row</I> of the
  matrix; evaluating a reasonable index of this pointer provides the <I>column</I>. So member access by <I>m[row][column]</I>
  is possible. But there is no checkup if the index makes sense. Only the values 0 to 2 for row and column are reasonable.
  @param iRow Row index of an coefficient, only 0 to 2 are defined.
  @return Pointer to the chosen row of the matrix. */
  T* operator[] (int iRow) const;

  /** Cast operator to provide the addresses of the components - the array of coefficient themselves (not of the object!).
  @return Pointer to the coefficient array. */
  operator T* ();

  /** Provides access to each column of the matrix as an \Ref{AAVVector3<T>}.
  @param iCol Column index of the matrix (There is a checkup of the index: only 0, 1 or 2 are allowed).
  @return Coefficents in the chosen column as an AAVVector3<T>.
  @see AAVVector3<T> */
  AAVVector3<T> GetColumn (int iCol) const;

  /** Reinitialise this matrix with the data of another matrix.
  @param rkMatrix Matrix to copy the data from.
  @return Reinitialised matrix. */
  AAVMatrix3<T>& operator= (const AAVMatrix3<T>& rkMatrix);

  /** Compares two matrices and checks if all their coefficients are equal.
  @param rkMatrix Matrix to compare with.
  @return Only if all coefficients are equal <B>true</B> will be returned. */
  bool operator== (const AAVMatrix3<T>& rkMatrix) const;

  /** Compares two matrices and checks if one coefficient is not equal.
  @param rkMatrix Matrix to compare with.
  @return Only if all coefficients are equal <B>false</B> will be returned. */
  bool operator!= (const AAVMatrix3<T>& rkMatrix) const;

  // arithmetic operations

  /** Calculates the sum of two matrices.
  @param rkMatrix Second addend.
  @return Sum of the two matrices. */
  AAVMatrix3<T> operator+ (const AAVMatrix3<T>& rkMatrix) const;

  /** Calculates the difference between two matrices.
  @param rkMatrix Subtrahend.
  @return Result of subtraction (difference). */
  AAVMatrix3<T> operator- (const AAVMatrix3<T>& rkMatrix) const;

  /** Calculates the multiplication of two matrices.
  @param rkMatrix Second multiplicator matrix.
  @return Result of multiplication. */
  AAVMatrix3<T> operator* (const AAVMatrix3<T>& rkMatrix) const;

  /** Calculates the multiplication of the matrix by a scalar.
  @param vScalar Scalar multiplicator.
  @return Result of multiplication. */
  AAVMatrix3<T> operator* (T vScalar) const;

  /** Calculates the multiplication of the matrix by an \Ref{AAVVector3<T>}.
  @param rkVector Vector multiplicator.
  @return Result of multiplication - an AAVVector3<T>.
  @see AAVVector3<T> */
  AAVVector3<T> operator* (const AAVVector3<T>& rkVector) const;

  /** Calculates the multiplication of an \Ref{AAVVector3<T>} by another matrix.
  So it is possible now to make for example following operation: <BR>
  <TT>M1 = V * M2</TT>.
  @param rkVector Vector multiplicator.
  @param rkMatrix Matrix multiplicator.
  @return Result of multiplication - an AAVVector3<T>.
  @see AAVVector3<T> */
  friend AAVVector3<T> operator* (const AAVVector3<T>& rkVector, const AAVMatrix3<T>& rkMatrix);

  /** Calculates the multiplication of a scalar by another matrix.
  So it is possible now to make for example following operation: <BR>
  <TT>M1 = 2 * M2</TT>.
  @param vScalar Scalar multiplicator.
  @param rkMatrix Matrix multiplicator.
  @return Result of multiplication. */
  friend AAVMatrix3<T> operator* (T vScalar, const AAVMatrix3<T>& rkMatrix);

  /** Calculates the negative matrix of this matrix. This operation is similar to the multiplication of this matrix with the scalar '-1'.
  @return Negative matrix. */
  AAVMatrix3<T> operator- () const;

  // vector operations, utilities

  /** Calculates the transpose matrix of this matrix. Therefor all columns and rows are interchanged.
  @return Transpose matrix.
  @see Inverse */
  AAVMatrix3<T> Transpose () const;

  /** Calculates the inverse matrix of another matrix. Therefor the determinant of that matrix has to be calculated.
  If the determinant is smaller than the given tolerance, this method returns <B>false</B>.
  @param rkInverse Matrix to calculate the inverse from.
  @param vTolerance Precision of checking for correct calculation.
  @return If the derterminant of the matrix is <I>smaller</I> than the given tolerance <B>false</B> will be returned.
  @see Inverse, Transpose */
  bool Inverse (AAVMatrix3<T>& rkInverse, T vTolerance = 1e-06) const;

  /** Calculates the inverse matrix of this matrix. Therefor the determinant of the matrix has to be calculated.
  If the determinant is smaller than the given tolerance, this method returns the \Ref{ZERO} matrix.
  @param vTolerance Precision of checking for correct calculation.
  @return Inverse matrix or \Ref{ZERO} matrix if the determinant is <I>smaller</I> than the given tolerance.
  @see Inverse, Transpose, ZERO */
  AAVMatrix3<T> Inverse (T vTolerance = 1e-06) const;

  /** Provides axis and radiants of this rotation matrix. 
  @param rkAxis Resulting axis vector (return value).
  @param rvRadians Resulting angle in radians (return value).
  @see FromAxisAngle, ToEulerAnglesXYZ */
  void ToAxisAngle (AAVVector3<T>& rkAxis, T& rvRadians) const;

  /** Creates rotation matrix out of an axis and radiant.
  @param rkAxis Axis vector to calculate with.
  @param rvRadians Radiant angle.
  @see ToAxisAngle, FromEulerAnglesXYZ */
  void FromAxisAngle (const AAVVector3<T>& rkAxis, T vRadians);

  /** Provides XYZ-based euler angles in radiant.
  @param rfXAngle Resulting X-angle (return value).
  @param rfYAngle Resulting Y-angle (return value).
  @param rfZAngle Resulting Z-angle (return value).
  @return Only if the result is unique <B>true</B> will be returned.
  @see FromEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYXZ, ToEulerAnglesYZX, ToEulerAnglesZXY, ToEulerAnglesZYX */
  bool ToEulerAnglesXYZ (T& rvXAngle, T& rvYAngle, T& rvZAngle) const;

  /** Provides XZY-based euler angles in radiant.
  @param rfXAngle Resulting X-angle (return value).
  @param rfZAngle Resulting Z-angle (return value).
  @param rfYAngle Resulting Y-angle (return value).
  @return Only if the result is unique <B>true</B> will be returned.
  @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesYXZ, ToEulerAnglesYZX, ToEulerAnglesZXY, ToEulerAnglesZYX */
  bool ToEulerAnglesXZY (T& rvXAngle, T& rvZAngle, T& rvYRAngle) const;

  /** Provides YXZ-based euler angles in radiant.
  @param rfYAngle Resulting Y-angle (return value).
  @param rfXAngle Resulting X-angle (return value).
  @param rfZAngle Resulting Z-angle (return value).
  @return Only if the result is unique <B>true</B> will be returned.
  @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYZX, ToEulerAnglesZXY, ToEulerAnglesZYX */
  bool ToEulerAnglesYXZ (T& rfYAngle, T& rfXAngle, T& rfZAngle) const;

  /** Provides YZX-based euler angles in radiant.
  @param rfYAngle Resulting Y-angle (return value).
  @param rfZAngle Resulting Z-angle (return value).
  @param rfXAngle Resulting X-angle (return value).
  @return Only if the result is unique <B>true</B> will be returned.
  @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYXZ, ToEulerAnglesZXY, ToEulerAnglesZYX */
  bool ToEulerAnglesYZX (T& rvYAngle, T& rvZAngle, T& rvXAngle) const;

  /** Provides ZXY-based euler angles in radiant.
  @param rfZAngle Resulting Z-angle (return value).
  @param rfXAngle Resulting X-angle (return value).
  @param rfYAngle Resulting Y-angle (return value).
  @return Only if the result is unique <B>true</B> will be returned.
  @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYXZ, ToEulerAnglesYZX, ToEulerAnglesZYX */
  bool ToEulerAnglesZXY (T& rvZAngle, T& rvXAngle, T& rvYAngle) const;

  /** Provides ZYX-based euler angles in radiant.
  @param rfZAngle Resulting Z-angle (return value).
  @param rfYAngle Resulting Y-angle (return value).
  @param rfXAngle Resulting X-angle (return value).
  @return Only if the result is unique <B>true</B> will be returned.
  @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYXZ, ToEulerAnglesYZX, ToEulerAnglesZXY */
  bool ToEulerAnglesZYX (T& rfYAngle, T& rfPAngle, T& rfRAngle) const;

  /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together Mx*My*Mz.
  @param vXAngle X-angle for rotation matrix.
  @param vYAngle Y-angle for rotation matrix.
  @param vZAngle Z-angle for rotation matrix.
  @see ToEulerAnglesXYZ,  FromEulerAnglesXZY, FromEulerAnglesYXZ, FromEulerAnglesYZX, FromEulerAnglesZXY, FromEulerAnglesZYX*/
  void FromEulerAnglesXYZ (T vXAngle, T vYAngle, T vZAngle);

  /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together Mx*Mz*My.
  @param vXAngle X-angle for rotation matrix.
  @param vZAngle Z-angle for rotation matrix.
  @param vYAngle Y-angle for rotation matrix.
  @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesYXZ, FromEulerAnglesYZX, FromEulerAnglesZXY, FromEulerAnglesZYX*/
  void FromEulerAnglesXZY (T vXAngle, T vZAngle, T vYAngle);

  /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together My*Mx*Mz.
  @param vYAngle Y-angle for rotation matrix.
  @param vXAngle X-angle for rotation matrix.
  @param vZAngle Z-angle for rotation matrix.
  @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesXZY, FromEulerAnglesYZX, FromEulerAnglesZXY, FromEulerAnglesZYX*/
  void FromEulerAnglesYXZ (T vYAngle, T vXAngle, T vZAngle);

  /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together My*Mz*Mx.
  @param vYAngle Y-angle for rotation matrix.
  @param vZAngle Z-angle for rotation matrix.
  @param vXAngle X-angle for rotation matrix.
  @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesXZY, FromEulerAnglesYXZ, FromEulerAnglesZXY, FromEulerAnglesZYX*/
  void FromEulerAnglesYZX (T vYAngle, T vZAngle, T vXAngle);

  /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together Mz*Mx*My.
  @param vZAngle Z-angle for rotation matrix.
  @param vXAngle X-angle for rotation matrix.
  @param vYAngle Y-angle for rotation matrix.
  @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesXZY, FromEulerAnglesYXZ, FromEulerAnglesYZX, FromEulerAnglesZYX*/
  void FromEulerAnglesZXY (T vZAngle, T vXAngle, T vYAngle);

  /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together Mz*My*Mx.
  @param vZAngle Z-angle for rotation matrix.
  @param vYAngle Y-angle for rotation matrix.
  @param vXAngle X-angle for rotation matrix.
  @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesXZY, FromEulerAnglesYXZ, FromEulerAnglesYZX, FromEulerAnglesZXY*/
  void FromEulerAnglesZYX (T vZAngle, T vYAngle, T vXAngle);

protected:

  // !!! INTERNAL USE ONLY !!!
  // all following protected members are for internal use only

  // member access.
  T m_aafEntry[3][3];
};

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T>::AAVMatrix3 ()
{
  // For efficiency reasons, do not initialize matrix.
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T>::AAVMatrix3 (const T aafEntry[3][3])
{
  memcpy(m_aafEntry, aafEntry, 9*sizeof(T));
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T>::AAVMatrix3 (const AAVMatrix3<T>& rkMatrix)
{
  memcpy(m_aafEntry, rkMatrix.m_aafEntry, 9*sizeof(T));
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T>::AAVMatrix3(T fEntry00, T fEntry01, T fEntry02,
                                                             T fEntry10, T fEntry11, T fEntry12,
                                                             T fEntry20, T fEntry21, T fEntry22)
{
  m_aafEntry[0][0] = fEntry00;
  m_aafEntry[0][1] = fEntry01;
  m_aafEntry[0][2] = fEntry02;
  m_aafEntry[1][0] = fEntry10;
  m_aafEntry[1][1] = fEntry11;
  m_aafEntry[1][2] = fEntry12;
  m_aafEntry[2][0] = fEntry20;
  m_aafEntry[2][1] = fEntry21;
  m_aafEntry[2][2] = fEntry22;
}

//----------------------------------------------------------------------------
//template <typename T> inline AAVMatrix3<T>::AAVMatrix3(const eGML_Matrix2& rkMatrix)
//{
//  m_aafEntry[0][0] = rkMatrix[0][0];
//  m_aafEntry[0][1] = rkMatrix[0][1];
//  m_aafEntry[0][2] = eC_FromInt(0);
//  m_aafEntry[1][0] = rkMatrix[1][0];
//  m_aafEntry[1][1] = rkMatrix[1][1];
//  m_aafEntry[1][2] = eC_FromInt(0);
//  m_aafEntry[2][0] = eC_FromInt(0);
//  m_aafEntry[2][1] = eC_FromInt(0);
//  m_aafEntry[2][2] = eC_FromInt(1);
//}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T>::AAVMatrix3(const AAVVector3<T>& rkLeft, const AAVVector3<T>& rkUp, const AAVVector3<T>& rkDir)
{	
  m_aafEntry[0][0] = rkLeft.x;
  m_aafEntry[0][1] = rkUp.x;
  m_aafEntry[0][2] = rkDir.x;
  m_aafEntry[1][0] = rkLeft.y;
  m_aafEntry[1][1] = rkUp.y;
  m_aafEntry[1][2] = rkDir.y;
  m_aafEntry[2][0] = rkLeft.z;
  m_aafEntry[2][1] = rkUp.z;
  m_aafEntry[2][2] = rkDir.z;
}

//----------------------------------------------------------------------------
template <typename T> inline T* AAVMatrix3<T>::operator[] (int iRow) const
{
  return (T*)&m_aafEntry[iRow][0];
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T>::operator T* ()
{
  return &m_aafEntry[0][0];
}

//----------------------------------------------------------------------------
template <typename T> inline AAVVector3<T> AAVMatrix3<T>::GetColumn (int iCol) const
{
  eDBG_ASSERT( 0 <= iCol && iCol < 3, "Column index out of bounds" );
  return AAVVector3<T>(m_aafEntry[0][iCol],m_aafEntry[1][iCol], m_aafEntry[2][iCol]);
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T>& AAVMatrix3<T>::operator= (const AAVMatrix3<T>& rkMatrix)
{
  memcpy(m_aafEntry, rkMatrix.m_aafEntry, 9*sizeof(T));
  return *this;
}

//----------------------------------------------------------------------------
template <typename T> inline bool AAVMatrix3<T>::operator== (const AAVMatrix3<T>& rkMatrix) const
{
  for (int iRow = 0; iRow < 3; iRow++)
  {
    for (int iCol = 0; iCol < 3; iCol++)
    {
      if ( m_aafEntry[iRow][iCol] != rkMatrix.m_aafEntry[iRow][iCol] )
        return false;
    }
  }

  return true;
}

//----------------------------------------------------------------------------
template <typename T> inline bool AAVMatrix3<T>::operator!= (const AAVMatrix3<T>& rkMatrix) const
{
  return !operator==(rkMatrix);
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T> AAVMatrix3<T>::operator+ (const AAVMatrix3<T>& rkMatrix) const
{
  AAVMatrix3<T> kSum;
  for (int iRow = 0; iRow < 3; iRow++)
  {
    for (int iCol = 0; iCol < 3; iCol++)
    {
      kSum.m_aafEntry[iRow][iCol] = m_aafEntry[iRow][iCol] +
        rkMatrix.m_aafEntry[iRow][iCol];
    }
  }
  return kSum;
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T> AAVMatrix3<T>::operator- (const AAVMatrix3<T>& rkMatrix) const
{
  AAVMatrix3<T> kDiff;
  for (int iRow = 0; iRow < 3; iRow++)
  {
    for (int iCol = 0; iCol < 3; iCol++)
    {
      kDiff.m_aafEntry[iRow][iCol] = m_aafEntry[iRow][iCol] -
        rkMatrix.m_aafEntry[iRow][iCol];
    }
  }
  return kDiff;
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T> AAVMatrix3<T>::operator* (const AAVMatrix3<T>& rkMatrix) const
{
  AAVMatrix3<T> kProd;
  for (int iRow = 0; iRow < 3; iRow++)
  {
    for (int iCol = 0; iCol < 3; iCol++)
    {
      kProd.m_aafEntry[iRow][iCol] = m_aafEntry[iRow][0] * rkMatrix.m_aafEntry[0][iCol] +
                                     m_aafEntry[iRow][1] * rkMatrix.m_aafEntry[1][iCol] +
                                     m_aafEntry[iRow][2] * rkMatrix.m_aafEntry[2][iCol];
    }
  }
  return kProd;
}

//----------------------------------------------------------------------------
template <typename T> inline AAVVector3<T> AAVMatrix3<T>::operator* (const AAVVector3<T>& rkPoint) const
{
  AAVVector3<T> kProd;
  for (int iRow = 0; iRow < 3; iRow++)
  {
    kProd[iRow] = m_aafEntry[iRow][0] * rkPoint[0] +
                  m_aafEntry[iRow][1] * rkPoint[1] +
                  m_aafEntry[iRow][2] * rkPoint[2];
  }
  return kProd;
}

//----------------------------------------------------------------------------
template <typename T> inline AAVVector3<T> operator* (const AAVVector3<T>& rkPoint, const AAVMatrix3<T>& rkMatrix)
{
  AAVVector3<T> kProd;
  for (int iRow = 0; iRow < 3; iRow++)
  {
    kProd[iRow] = rkPoint[0] * rkMatrix.m_aafEntry[0][iRow] +
                  rkPoint[1] * rkMatrix.m_aafEntry[1][iRow] +
                  rkPoint[2] * rkMatrix.m_aafEntry[2][iRow];
  }
  return kProd;
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T> AAVMatrix3<T>::operator- () const
{
  AAVMatrix3<T> kNeg;
  for (int iRow = 0; iRow < 3; iRow++)
  {
    for (int iCol = 0; iCol < 3; iCol++)
      kNeg[iRow][iCol] = -m_aafEntry[iRow][iCol];
  }
  return kNeg;
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T> AAVMatrix3<T>::operator* (T fScalar) const
{
  AAVMatrix3<T> kProd;
  for (int iRow = 0; iRow < 3; iRow++)
  {
    for (int iCol = 0; iCol < 3; iCol++)
      kProd[iRow][iCol] = fScalar*m_aafEntry[iRow][iCol];
  }
  return kProd;
}

//----------------------------------------------------------------------------
template <typename T> inline AAVMatrix3<T> operator* (T fScalar, const AAVMatrix3<T>& rkMatrix)
{
  AAVMatrix3<T> kProd;
  for (int iRow = 0; iRow < 3; iRow++)
  {
    for (int iCol = 0; iCol < 3; iCol++)
      kProd[iRow][iCol] = fScalar*rkMatrix.m_aafEntry[iRow][iCol];
  }
  return kProd;
}

//----------------------------------------------------------------------------
#endif //_AAV_MATRIX3_
