#pragma once

#include <cstddef>

namespace uazips
{
    template<typename T>
    struct ArrayView
    {
        const T* data;
        size_t length;

        constexpr T operator[](size_t get) const
        {
            return data[get];
        }
    };

    template<typename T>
    struct ArrayView2D
    {
        const T* data;
        size_t rows, cols;
        
        constexpr T operator()(size_t r, size_t c) const
        {
            return data[r * cols + c];
        }
    };

    template<typename T>
    struct ArrayView3D
    {
        const T* data;
        size_t rows, cols, depth;

        constexpr T operator()(size_t r, size_t c, size_t d) const
        {
            return data[r * cols * depth + c * depth + d];
        }
    };

    template<typename T, size_t l>
    constexpr ArrayView<T> make_view(const T (&array)[l])
    {
        return ArrayView<T>{
            &array[0],
            l
        };
    }

    template<typename T>
    constexpr ArrayView<T> make_view(const T* ptr, size_t length)
    {
        return ArrayView<T>{
            ptr,
            length
        };
    }

    template<typename T, size_t r, size_t c>
    constexpr ArrayView2D<T> make_view(const T (&array)[r][c])
    {
        return ArrayView2D<T>{
            &array[0][0],
            r, c
        };
    }

    // Do not wrap a heap-allocated double-pointer array into the ArrayView.
    template<typename T>
    constexpr ArrayView2D<T> make_view(const T* ptr, size_t row, size_t col)
    {
        return ArrayView2D<T>{
            ptr,
            row, col
        };
    }

    template<typename T, size_t r, size_t c, size_t d>
    constexpr ArrayView3D<T> make_view(const T (&array)[r][c][d])
    {
        return ArrayView3D<T>{
            &array[0][0][0],
            r, c, d
        };
    }

    // Do not wrap a heap-allocated triple-pointer array into the ArrayView
    template<typename T>
    constexpr ArrayView3D<T> make_view(const T* ptr, size_t row, size_t col, size_t dep)
    {
        return ArrayView3D<T>{
            ptr,
            row, col, dep
        };
    }

}