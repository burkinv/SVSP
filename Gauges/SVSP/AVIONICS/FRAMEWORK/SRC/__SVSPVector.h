#ifndef __SVSP_VECTOR_H__
#define __SVSP_VECTOR_H__

template <class T> class __SVSPVector
{
public:
  __SVSPVector(UINT s = 0)
  {
    clear();
    reallocate(m_uiSize = s);
  }

  ~__SVSPVector()
  {
    free(m_tArray);
  }

  // m_tArray access operator overloading
  T& operator[](UINT i)
  { 
    return m_tArray[i]; 
  }

  // empty the vector
  void clear()
  { 
    m_uiElements = 0; 
  }

  // check whether the vector is empty
  bool empty()
  { 
    return (m_uiElements == 0); 
  }

  // insert to front of list
  void insert_front(const T& ins)
  { 
    // if m_tArray is not large enough, make larger
    if (m_uiElements >= m_uiSize) reallocate(++m_uiSize);

    // determine size of the back half of the m_tArray
    UINT back_size = (m_tArray + m_uiElements) - ptr;

    // copy all of the elements back one (shift it)
    memcpy(ptr + 1, ptr, back_size);

    // copy new value into free slot
    memcpy(ptr, &ins, sizeof(T));

    // increment number of elements
    ++m_uiElements;
  }

  // insert to back of list
  void insert_back(const T& ins)
  { 
    // if m_tArray is not large enough, make larger
    if (m_uiElements >= m_uiSize) reallocate(++m_uiSize);

    // copy new value into last slot of the m_tArray
    memcpy(m_tArray + m_uiElements, &ins, sizeof(T));

    // increment number of elements
    ++m_uiElements;
  }

  // insert to index
  void insert(const T& ins, UINT loc)
  { 
    insert_loc(ins, m_tArray + sizeof(T) * loc); 
  }

  // remove element at loc
  void remove(UINT loc)
  {
    // if the location is within range
    if (loc < m_uiElements && loc >= 0) remove_span(loc, loc + 1);
  }

  // remove elements in [start, end)
  // including element pointed to start, but not element pointed by end
  void remove(UINT start, UINT end)
  {
    // if start and end are within range, and in correct order
    if (start < m_uiElements && start >= 0 && end < m_uiElements && end >= 0 && end >= start)
      remove_span(start, end);
  }

  // get size of vector
  inline UINT size() const 
  { 
    return m_uiElements; 
  }

  // get capacity of vector
  inline UINT capacity()
  { 
    return m_uiSize; 
  }


private:
  T   *m_tArray;
  UINT m_uiSize;
  UINT m_uiElements;

  //// PRIVATE // insert at the end of the list
  //void insert_end(const T& ins)
  //{
  //  // if m_tArray is not large enough, make larger
  //  if (m_uiElements >= m_uiSize) reallocate(++m_uiSize);

  //  // copy new value into last slot of the m_tArray
  //  memcpy(m_tArray + m_uiElements, &ins, sizeof(T));

  //  // increment number of elements
  //  ++m_uiElements;
  //}

  // PRIVATE // insert into location pointed to by 'ptr'
  //void insert_loc(const T& ins, T *ptr)
  //{
  //  // if m_tArray is not large enough, make larger
  //  if (m_uiElements >= m_uiSize) reallocate(++m_uiSize);

  //  // determine size of the back half of the m_tArray
  //  UINT back_size = (m_tArray + m_uiElements) - ptr;

  //  // copy all of the elements back one (shift it)
  //  memcpy(ptr + 1, ptr, back_size);

  //  // copy new value into free slot
  //  memcpy(ptr, &ins, sizeof(T));

  //  // increment number of elements
  //  ++m_uiElements;
  //}

  // PRIVATE // insert into index 'loc'
  void insert_loc(const T& ins, UINT loc)
  { 
    insert_loc(ins, m_tArray + sizeof(T) * loc); 
  }

  // PRIVATE // remove elements in [start, end)
  void remove_span(UINT start, UINT end)
  {
    // calculate number of entries after splice
    UINT back_count = m_uiElements - end;

    // copy entries backwards
    memcpy(m_tArray + start, m_tArray + end, sizeof(T) * back_count);

    // adjust number of elements
    m_uiElements -= end - start;
  }

  // PRIVATE // reallocate the m_tArray in memory
  void reallocate(UINT size_new)
  {
    // reallocate the m_tArray at the new size
    m_tArray = (T*) realloc(m_tArray, size_new * sizeof(T));
  }

};

#endif //__SVSP_VECTOR_H__
