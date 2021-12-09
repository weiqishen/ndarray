/**
 * @file ndarray.h
 * @brief A 2D templated matrix class based on ndarray
 * @author Weiqi Shen weiqishen1994@gmail.com
 * @version 0.1
 * @date 2021-12-9
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <utility> // std::swap
#include "ndarray.h"

template <typename T>
class Matrix
{
public:
    /// default constructor
    Matrix();
    ///
    Matrix(size_t m, size_t n);

    /// copy constructor
    Matrix(const Matrix<T> &rhs);

    void Setup(size_t m, size_t n);

    void Resize(size_t m, size_t n);

    T *GetData(size_t m, size_t n);

    T *GetData(size_t idx = 0);

    T &operator()(size_t m, size_t n);

    T &operator()(size_t idx);

    /// Assignment
    Matrix<T> &operator=(const Matrix<T> &rhs);

    /// fill with value
    Matrix<T> &operator=(const T &val);

    size_t GetDim(size_t n);

    void GetShape(size_t &m, size_t &n);

    size_t GetLength(void);

    void Transpose();
    
    void Transpose(Matrix<T>&out_mat);
    ~Matrix();

    /**
     * @brief Overload stream output operator
     *
     * @param out ostream object
     * @param s Matrix to output
     * @return ostream& reference to the ostream object
     */
    template <class U>
    friend std::ostream &operator<<(std::ostream &out, Matrix<U> &s);

protected:
    ndarray<T> m_data;
};

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &rhs)
{
    this->m_data = rhs.m_data;
}

template <typename T>
Matrix<T>::Matrix()
{
    m_data.Setup({1, 1});
}

template <typename T>
Matrix<T>::Matrix(size_t m, size_t n)
{
    m_data.Setup({m, n});
}

template <typename T>
void Matrix<T>::Setup(size_t m, size_t n)
{
    m_data.Setup({m, n});
}

template <typename T>
void Matrix<T>::Resize(size_t m, size_t n)
{
    ndarray<T> temp = m_data;
    m_data.Setup({m, n});

    size_t col_bd = std::min(m, temp.GetDim(0));
    size_t row_bd = std::min(n, temp.GetDim(1));
    for (size_t i = 0; i < col_bd; i++)
    {
        for (size_t j = 0; j < row_bd; j++)
        {
            m_data({i, j}) = temp({i, j});
        }
    }
}

template <typename T>
size_t Matrix<T>::GetDim(size_t n)
{
    return m_data.GetDim(n);
}

template <typename T>
void Matrix<T>::GetShape(size_t &m, size_t &n)
{
    m = m_data.GetDim(0);
    n = m_data.GetDim(1);
}

template <typename T>
size_t Matrix<T>::GetLength(void)
{
    return m_data.GetLength();
}

template <typename T>
T *Matrix<T>::GetData(size_t idx)
{
    return m_data.GetData(idx);
}

template <typename T>
T *Matrix<T>::GetData(size_t m, size_t n)
{
    return m_data.GetData({m,n});
}

template <typename T>
T &Matrix<T>::operator()(size_t m, size_t n)
{
    return m_data({m, n});
}

template <typename T>
T &Matrix<T>::operator()(size_t idx)
{
    return m_data(idx);
}

// assignment

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &rhs)
{

    if (this == &rhs)
    {
        return (*this);
    }
    else
    {
        m_data = rhs.m_data;
    }
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const T &val)
{
    m_data = val;
    return *this;
}

template <typename T>
Matrix<T>::~Matrix()
{
}

template <typename T>
void Matrix<T>::Transpose()
{
    size_t c_s = m_data.GetDim(0);
    size_t r_s = m_data.GetDim(1);
    for (size_t k = 0; k < c_s * r_s; k++) // loop over the target array
    {
        size_t idx = k;
        do
        {
            size_t id_0t = idx % r_s;
            size_t id_1t = idx / r_s;
            idx = id_0t * c_s + id_1t; // idx in original array
        } while (idx < k);
        std::swap(m_data(k), m_data(idx));
    }
    m_data.Reshape({r_s, c_s});
}

template <typename T>
void Matrix<T>::Transpose(Matrix<T> &out_mat)
{
    // Get dimensions of arrays
    size_t dim_0 = m_data.GetDim(0);
    size_t dim_1 = m_data.GetDim(1);

    // Only 2D square arrays
    if (out_mat.GetDim(0) == dim_1 && out_mat.GetDim(1) == dim_0)
    {
        for (size_t i = 0; i < dim_0; i++)
        {
            for (size_t j = 0; j < dim_1; j++)
            {
                out_mat(j, i) = m_data({i, j});
            }
        }
    }
    else
    {
        std::cout << "ERROR: Array dimension not agree" << std::endl;
        exit(EXIT_FAILURE);
    }
}

//-------------output---------------------
template <typename U>
std::ostream &operator<<(std::ostream &out, Matrix<U> &s)
{
    return (out << s.m_data);
}
