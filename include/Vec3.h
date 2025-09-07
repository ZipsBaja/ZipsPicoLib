// Zips Baja 2025 Vector3
// Author: Zack Young

// TODO: support for pico2 risc-v

#pragma once

#include <stdint.h>

namespace uazips
{
    template<typename D>
    struct Vec3
    {
        inline Vec3(D x, D y, D z) : x(x), y(y), z(z)
        {
        }

        inline Vec3() : Vec3((D)0.0, (D)0.0, (D)0.0)
        {
        }

        D x, y, z;

        inline Vec3 operator+(const Vec3& Other) const
        {
            return Vec3(x + Other.x, y + Other.y, z + Other.z);
        }

        inline Vec3 operator-(const Vec3& Other) const
        {
            return Vec3(x - Other.x, y - Other.y, z - Other.z);
        }

        inline Vec3 operator*(const Vec3& Other) const
        {
            return Vec3(x * Other.x, y * Other.y, z * Other.z);
        }

        inline Vec3 operator/(const Vec3& Other) const
        {
            return Vec3(x / Other.x, y / Other.y, z / Other.z);
        }

        inline Vec3& operator+=(const Vec3& Other)
        {
            x += Other.x;
            y += Other.y;
            z += Other.z;
            return *this;
        }

        inline Vec3& operator-=(const Vec3& Other)
        {
            x -= Other.x;
            y -= Other.y;
            z -= Other.z;
            return *this;
        }

        inline Vec3& operator*=(const Vec3& Other)
        {
            x *= Other.x;
            y *= Other.y;
            z *= Other.z;
            return *this;
        }

        inline Vec3& operator/=(const Vec3& Other)
        {
            x /= Other.x;
            y /= Other.y;
            z /= Other.z;
            return *this;
        }

        inline Vec3 operator*(D Scalar) const
        {
            return Vec3(x * Scalar, y * Scalar, z * Scalar);
        }

        inline Vec3 operator/(D Scalar) const
        {
            return Vec3(x / Scalar, y / Scalar, z / Scalar);
        }

        inline Vec3& operator*=(D Scalar)
        {
            x *= Scalar;
            y *= Scalar;
            z *= Scalar;
            return *this;
        }

        inline Vec3& operator/=(D Scalar)
        {
            x /= Scalar;
            y /= Scalar;
            z /= Scalar;
            return *this;
        }

        inline D Length() const
        {
            return sqrtf(x*x + y*y + z*z);
        }

        inline Vec3 Normalized() const
        {
            D len = sqrtf(x*x + y*y + z*z);
            return Vec3(x / len, y / len, z / len);
        }       

        inline D Dot(const Vec3& Other) const
        {
            return x * Other.x + y * Other.y + z * Other.z;
        }

        inline Vec3& operator++()
        {
            x += (D)1.0;
            y += (D)1.0;
            z += (D)1.0;
            return *this;
        }

        inline Vec3& operator--()
        {
            x -= (D)1.0;
            y -= (D)1.0;
            z -= (D)1.0;
            return *this;
        }

        inline D operator|(const Vec3& Other) const
        {
            return (Other - *this).Length();
        }
    };

    using Vec3f = Vec3<float>;
    using Vec3d = Vec3<double>;
    using Vec3i8 = Vec3<int8_t>;
    using Vec3u8 = Vec3<uint8_t>;
    using Vec3i16 = Vec3<int16_t>;
    using Vec3u16 = Vec3<uint16_t>;
    using Vec3i32 = Vec3<int32_t>;
    using Vec3u32 = Vec3<uint32_t>;
    using Vec3i64 = Vec3<int64_t>;
    using Vec3u64 = Vec3<uint64_t>;

// Not guaranteed to be functional.
//  using Vec3i128 = Vec3<__int128_t>;
//  using Vec3u128 = Vec3<__uint128_t>;
}