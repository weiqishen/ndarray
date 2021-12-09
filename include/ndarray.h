/**
 * @file ndarray.h
 * @brief A simple N-dimensional array in Column Major
 * @author Weiqi Shen weiqishen1994@ufl.edu
 * @version 0.2
 * @date 2021-12-9
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iomanip>

/**
 * @brief A simple N-dimensional array in Column Major
 * @class ndarray
 * @tparam T data type
 */
template <typename T>
class ndarray
{
public:
  // #### constructors ####

  /// Default constructor to construct an empty ndarray
  ndarray(void);

  /**
   * @brief N-Dimension constructor
   *
   * @param list A list to specify dimension of the ndarray
   */
  ndarray(std::initializer_list<size_t> list);

  /**
   * @brief 1-D constructor
   *
   * @param size number of element
   */
  ndarray(size_t size);

  /// Copy constructor
  ndarray(const ndarray<T> &rhs);

  /// destructor
  ~ndarray(void);

  // #### methods ####

  /**
   * @brief Overload stream output operator
   *
   * @param out ostream object
   * @param s array to output
   * @return ostream& reference to the ostream object
   */
  template <class U>
  friend std::ostream &operator<<(std::ostream &out, ndarray<U> &s);

  /**
   * @brief Setup a new ndarray object
   *
   * @param list A list to specify dimension of the ndarray
   */
  void Setup(std::initializer_list<size_t> list);

  /**
   * @brief Setup a new 1-D ndarray object
   *
   * @param list A list to specify dimension of the ndarray
   */
  void Setup(size_t size);

  /// Assignment
  ndarray<T> &operator=(const ndarray<T> &rhs);

  /// fill with value
  ndarray<T> &operator=(const T &val);

  /// Access/set ndarray element
  T &operator()(std::initializer_list<size_t> list);

  /// 1-D access/set
  T &operator()(size_t idx);

  /// Return pointer of ndarray element
  T *GetData(std::initializer_list<size_t> list);

  /// Return pointer of ndarray element in 1-D
  T *GetData(size_t idx = 0);

  /// Get number of elements along one axis
  size_t GetDim(size_t n);

  /// Get number of dimension of the ndarray
  size_t GetNumDim(void);

  /// get the length of the ndarray
  size_t GetLength(void);

  /// Method to get maximum value of ndarray
  T GetMax(void);

  /// Method to get minimum value of ndarray
  T GetMin(void);

  /// Reshape the array
  void Reshape(std::initializer_list<size_t> list);

  /// Get the index of each dimension given the 1-D index
  void GetIndex(size_t idx, ndarray<size_t> &out_idx);

protected:
  size_t *p_shape;
  T *p_data;
  size_t m_nDim;
  size_t m_length;

private:
  /// helper method to calculate length of ndarray
  void CalcLength(void);
};

// #### constructors ####

// default constructor

template <typename T>
ndarray<T>::ndarray()
{
  m_length = 0;
  m_nDim = 0;
  p_shape = NULL;
  p_data = NULL;
}

// constructor 1

template <typename T>
ndarray<T>::ndarray(std::initializer_list<size_t> list)
{
  // store dimension array
  m_nDim = list.size();
  p_shape = new size_t[m_nDim];

  size_t i = 0;
  for (auto l : list)
    p_shape[i++] = l;

  CalcLength();
  p_data = new T[m_length];
}

// constructor 2

template <typename T>
ndarray<T>::ndarray(size_t size)
{
  // store dimension array
  m_nDim = 1;
  p_shape = new size_t[m_nDim];
  p_shape[0] = size;
  CalcLength();
  p_data = new T[m_length];
}

// copy constructor

template <typename T>
ndarray<T>::ndarray(const ndarray<T> &rhs)
{
  m_nDim = rhs.m_nDim;
  p_shape = new size_t[m_nDim];
  std::copy(rhs.p_shape, rhs.p_shape + m_nDim, this->p_shape);

  CalcLength();
  p_data = new T[m_length];
  std::copy(rhs.p_data, rhs.p_data + m_length, this->p_data);
}

// assignment

template <typename T>
ndarray<T> &ndarray<T>::operator=(const ndarray<T> &rhs)
{

  if (this == &rhs)
  {
    return (*this);
  }
  else
  {
    delete[] p_data;
    delete[] p_shape;

    m_nDim = rhs.m_nDim;
    p_shape = new size_t[m_nDim];
    std::copy(rhs.p_shape, rhs.p_shape + m_nDim, this->p_shape);

    CalcLength();
    p_data = new T[m_length];
    std::copy(rhs.p_data, rhs.p_data + m_length, this->p_data);
    return (*this);
  }
}

template <typename T>
ndarray<T> &ndarray<T>::operator=(const T &val)
{
  fill_n(this->p_data, m_length, val);
  return *this;
}
// destructor

template <typename T>
ndarray<T>::~ndarray()
{
  delete[] p_data;
  delete[] p_shape;
}

// #### methods ####

// Setup

template <typename T>
void ndarray<T>::Setup(std::initializer_list<size_t> list)
{
  // delete previous p_data
  delete[] p_shape;
  delete[] p_data;

  // store dimension array
  m_nDim = list.size();
  p_shape = new size_t[m_nDim];

  size_t i = 0;
  for (auto l : list)
    p_shape[i++] = l;

  CalcLength();
  p_data = new T[m_length];
}

// 1-D Setup
template <typename T>
void ndarray<T>::Setup(size_t size)
{
  // delete previous p_data
  delete[] p_shape;
  delete[] p_data;

  // store dimension array
  m_nDim = 1;
  p_shape = new size_t[m_nDim];
  p_shape[0] = size;
  CalcLength();
  p_data = new T[m_length];
}

template <typename T>
T &ndarray<T>::operator()(size_t idx)
{
#ifdef _DEBUG
  if (idx >= m_length)
  {
    std::cout << "Error: ndarray out of bound" << std::endl;
    exit(EXIT_FAILURE);
  }
#endif
  return p_data[idx];
}

template <typename T>
T &ndarray<T>::operator()(std::initializer_list<size_t> list)
{
  size_t idx = 0, acc = 1;
  size_t i = 0;

  for (auto l : list)
  {
    idx += acc * l;
    acc *= p_shape[i++];
  }
#ifdef _DEBUG
  if (idx >= m_length)
  {
    std::cout << "Error: ndarray out of bound" << std::endl;
    exit(EXIT_FAILURE);
  }
#endif
  return p_data[idx];
}

// return pointer

template <typename T>
T *ndarray<T>::GetData(size_t idx)
{
#ifdef _DEBUG
  if (idx >= m_length)
  {
    std::cout << "Error: ndarray out of bound" << std::endl;
    exit(EXIT_FAILURE);
  }
#endif
  return p_data + idx;
}

template <typename T>
T *ndarray<T>::GetData(std::initializer_list<size_t> list)
{
  size_t idx = 0, acc = 1;
  size_t i = 0;

  for (auto l : list)
  {
    idx += acc * l;
    acc *= p_shape[i++];
  }
#ifdef _DEBUG
  if (idx >= m_length)
  {
    std::cout << "Error: ndarray out of bound" << std::endl;
    exit(EXIT_FAILURE);
  }
#endif
  return p_data + idx;
}

// obtain dimension

template <typename T>
size_t ndarray<T>::GetDim(size_t n)
{
  if (n < m_nDim)
    return p_shape[n];
  else
    return 0;
}

template <typename T>
void ndarray<T>::GetIndex(size_t idx, ndarray<size_t> &out_idx)
{
  size_t dim_bf = 1;
  for (size_t i = 0; i < m_nDim; i++)
  {
    out_idx(i) = idx / dim_bf;
    dim_bf *= p_shape[i];
    out_idx(i) = out_idx(i) % p_shape[i];
  }
}

template <typename T>
size_t ndarray<T>::GetNumDim()
{
  return m_nDim;
}

template <typename T>
void ndarray<T>::CalcLength(void)
{
  m_length = 1;
  for (size_t i = 0; i < m_nDim; i++)
    m_length *= p_shape[i];
}

template <typename T>
size_t ndarray<T>::GetLength(void)
{
  return m_length;
}
// method to calculate maximum value of ndarray
// Template specialization
template <typename T>
T ndarray<T>::GetMax(void)
{
  return *std::max_element(p_data, p_data + m_length);
}

// method to calculate minimum value of ndarray
// Template specialization
template <typename T>
T ndarray<T>::GetMin(void)
{
  return *std::min_element(p_data, p_data + m_length);
}

template <typename T>
void ndarray<T>::Reshape(std::initializer_list<size_t> list)
{

  size_t i = 0;

  if (m_nDim != list.size())
  {
    m_nDim = list.size();
    // delete p_shape
    delete[] p_shape;
    p_shape = new size_t[m_nDim];
  }

#ifdef _DEBUG
  size_t acc = 1;
#endif

  for (auto l : list)
  {
#ifdef _DEBUG
    acc *= l;
#endif
    p_shape[i++] = l;
  }

#ifdef _DEBUG
  if (acc != m_length)
  {
    std::cout << "ERROR: Total number of element doesn't agree" << endl;
    exit(EXIT_FAILURE);
  }
#endif
}

//-------------friend---------------------
// output
template <typename U>
std::ostream &operator<<(std::ostream &out, ndarray<U> &s)
{
  if (s.m_nDim == 1) // 1-D output
  {
    for (size_t i = 0; i < s.p_shape[0]; i++)
      out << std::setprecision(4) << std::setw(10) << s(i);
    out << std::endl;
  }
  else if (s.m_nDim == 2) // 2d output
  {
    for (size_t i = 0; i < s.p_shape[0]; i++)
    {
      for (size_t j = 0; j < s.p_shape[1]; j++)
      {
        out << std::setprecision(4) << std::setw(10) << s({i, j});
      }
      out << std::endl;
    }
  }
  else if (s.m_nDim == 3) // 3d output
  {
    for (size_t k = 0; k < s.p_shape[2]; k++)
    {
      out << "slice: (:,:," << k << ")" << std::endl;
      for (size_t i = 0; i < s.p_shape[0]; i++)
      {
        for (size_t j = 0; j < s.p_shape[1]; j++)
        {
          out << std::setprecision(4) << std::setw(10) << s({i, j, k});
        }
        out << std::endl;
      }
    }
  }
  else
  {
    std::cout << "ERROR: Output not supported" << std::endl;
    exit(EXIT_FAILURE);
  }

  return out;
}
