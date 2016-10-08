#ifndef __SVSP_STRING_H__
#define __SVSP_STRING_H__

#include <stdio.h>
#include <windows.h>
#include "__SVSPConfig.h"
#include "__SVSPDebug.h"

template <typename T> class __SVSPString
{
public:

  __SVSPString()
  {
    AAV_ASSERT_RETURN_VOID(sizeof(atData) > 1);
    atData[0] = static_cast<T>(0);
  }

  __SVSPString(const __SVSPString<typename T>& __rkString)
  {
    AAV_ASSERT_RETURN_VOID(sizeof(atData) > 1);
    atData[0] = static_cast<T>(0);
    UINT uiNumBytesToCopy = min((sizeof(atData) - 1), __rkString.getLength());
    memcpy_s(&atData[0], sizeof(atData), __rkString.getCStr(), uiNumBytesToCopy);
    atData[uiNumBytesToCopy] = '\0';
  }

  __SVSPString(const T *__ptData)
  {
    AAV_ASSERT_RETURN_VOID(sizeof(atData) > 1);
    if(__ptData != NULL)
    {
      atData[0] = static_cast<T>(0);
      UINT uiNumBytesToCopy = min((sizeof(atData) - 1), strlen(__ptData));
      memcpy_s(&atData[0], sizeof(atData), __ptData, uiNumBytesToCopy);
      atData[uiNumBytesToCopy] = '\0';
    }
  }

  inline const T* getCStr() const
  {
    return &atData[0];
  }

  inline INT getLength() const
  {
    return strlen(&atData[0]);
  }

  /**  operators =  */
  inline __SVSPString<typename T>& operator= (const __SVSPString<typename T>& __rkString)
  {
    AAV_ASSERT_RETURN(sizeof(atData) > 1, *this);
    if(__rkString != 0)
    {
      atData[0] = static_cast<T>(0);
      UINT uiNumBytesToCopy = min((sizeof(atData) - 1), __rkString.getLength());
      memcpy_s(&atData[0], sizeof(atData), __rkString.getCStr(), uiNumBytesToCopy);
      atData[uiNumBytesToCopy] = '\0';
    }
    return *this;
  }

  inline __SVSPString<typename T>& operator= (const INT __tData)
  {
    atData[0] = static_cast<T>(__tData);
    return *this;
  }

  /**  bool operators ==  */
  inline bool operator== (const __SVSPString<typename T>& __rkString) const
  {
    return ((__rkString.getLength() == getLength()) && (strcmp(__rkString.getCStr(), getCStr()) == 0));
  }

  inline bool operator== (const T *__ptData) const
  {
    return ((strlen(__ptData) == strlen(&atData[0])) && (strcmp(__ptData, getCStr()) == 0));
  }

  inline bool operator== (const T __tData) const
  {
    return (atData[0] == __tData);
  }

  inline bool operator== (const INT __tData) const
  {
    return (atData[0] == static_cast<T>(__tData));
  }

  /**  bool operators !=  */
  inline bool operator!= (const __SVSPString<typename T>& __rkString) const
  {
    return ((__rkString.getLength() != getLength()) && (strcmp(__rkString.getCStr(), getCStr()) != 0));
  }

  inline bool operator!= (const T *__ptData) const
  {
    return ((strlen(__ptData) != strlen(&atData[0])) && (strcmp(__ptData, getCStr()) != 0));
  }

  inline bool operator!= (const T __tData) const
  {
    return (atData[0] == __tData);
  }

  inline bool operator!= (const INT __tData) const
  {
    return (atData[0] == static_cast<T>(__tData));
  }

private:
  T atData[32];

};

#endif	/* __SVSP_STRING_H__ */


