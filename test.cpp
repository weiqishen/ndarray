#include "include/ndarray.h"
#include "include/Matrix.h"
#include <iostream>
int main(int argc, char const *argv[])
{
    std::cout << "Testing ndarray..." << std::endl;
    ndarray<int> matrix{3, 4, 5};
    for (size_t i = 0; i < matrix.GetLength(); i++)
    {
        matrix(i) = i;
    }
    std::cout << matrix;

    std::cout << "1-D index to N-D index..." << std::flush;
    for (size_t idx = 0; idx < matrix.GetLength(); idx++)
    {
        ndarray<size_t> temp_idx(matrix.GetNumDim());
        matrix.GetIndex(idx, temp_idx);
        if (!(matrix({temp_idx(0), temp_idx(1), temp_idx(2)}) == matrix(idx)))
        {
            std::cout << std::endl;
            std::cout << "ERROR: 1-D index to N-D index" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::cout << "Pass" << std::endl;

    std::cout << "Testing Matrix" << std::endl;
    Matrix<int> matrix3(4, 3);
    for (size_t i = 0; i < matrix3.GetLength(); i++)
    {
        matrix3(i) = i;
    }
    std::cout << matrix3;
    matrix3.Resize(2, 3);
    std::cout << "After resize to 2x3" << std::endl;
    std::cout << matrix3;
    Matrix<int> matrix4(3, 2);
    std::cout << "Out of place transpose" << std::endl;
    matrix3.Transpose(matrix4);
    std::cout << matrix4;
    std::cout << "Inplace transpose" << std::endl;
    matrix3.Transpose();
    std::cout << matrix3;
    return 0;
}
