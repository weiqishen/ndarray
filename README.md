# ndarray

This is a simple single header implementation of n-dimensional array in C++11 standard. The storage layout is column major (same as Fortran/Matlab). Current implementation only supports limited features. More features will be added in the future, ie. iterator. 

## Installation

Simply include the header in your source file to use.

## Documentation

Many member functions of ndarray accept variable length argument.
Safe and efficient variable length argument is implemented using `initializer_list`.

> To use debug features such as boundary check, add
`#define _DEBUG` at the beginning of header or use compiler flag `-D_DEBUG`.

Some major usage of ndarray are listed below:

1. Declare array

    ```c++
    ndarray <double> a(); //declare empty array
    ndarray <double> a({2,2,2}); //declare an ndarray using inializer_list with number of element in each dimension
    ```

2. Use setup to create a new array

    ```c++
    a.setup({0,0,1});
    ```

3. Initialize array elements to value

    ```c++
    a=5.;
    ```

4. Access/set array element

    ```c++
    a({0,0,1}); //access/set using initializer_list
    a(1); //access/set as if the array is 1D array
    ```

5. Get pointer of array element

    ```c++
    a.get_ptr({0,0,1}); //get pointer using initializer_list
    a.get_ptr(1); //get pointer as if the array is 1D array
    ```

6. Reshape array

    ```c++
    a.reshape({1,4,2}); //reshape using initializer_list
    ```

## License

[MIT License](LICENSE)
