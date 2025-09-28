// Zips Baja 2025 Vector3
// Author: Zack Young

// TODO: support for pico2 risc-v

#pragma once

#include <Quaternion.h>

constexpr double degrees_to_radians = 0.0174532925199;
constexpr double radians_to_degrees = 57.2957795131;

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
            return std::sqrt(x*x + y*y + z*z);
        }

        inline Vec3 Normalized() const
        {
            D len = std::sqrt(x*x + y*y + z*z);
            return Vec3(x / len, y / len, z / len);
        }       

        inline D Dot(const Vec3& Other) const
        {
            return x * Other.x + y * Other.y + z * Other.z;
        }

        inline Vec3 Cross(const Vec3& Other) const
        {
            return Vec3(
                y*Other.z - z*Other.y,
                z*Other.x - x*Other.z,
                x*Other.y - y*Other.x
            );
        }

        inline Vec3 ToDegrees() const
        {
            return Vec3(x * radians_to_degrees, y * radians_to_degrees, z * radians_to_degrees);
        }

        inline Vec3 ToRadians() const
        {
            return Vec3(x * degrees_to_radians, y * degrees_to_radians, z * degrees_to_radians);
        }

        inline Vec3 Rotate(const Quaternion<D> q) const
        {
            Quaternion<D> p(0, x, y, z);
            p = q * p;
            p = p * q.Conjugate();

            return {p.x, p.y, p.z};
        }

        inline Vec3 Rotate(const Vec3& Rotation, bool in_degrees = 0)
        {
            Vec3 radians_half = (in_degrees ? Rotation.ToRadians() : Rotation) * (D)0.5;

            D croll = std::cos(radians_half.x), sroll = std::sin(radians_half.x);
            D cpitch = std::cos(radians_half.y), spitch = std::sin(radians_half.y);
            D cyaw = std::cos(radians_half.z), syaw = std::sin(radians_half.z);
            
            D w = croll*cpitch*cyaw + sroll*spitch*syaw;
            D x = sroll*cpitch*cyaw - croll*spitch*syaw;
            D y = croll*spitch*cyaw + sroll*cpitch*syaw;
            D z = croll*cpitch*syaw - sroll*spitch*cyaw;

            Vec3 quat_vec = {x, y, z};
            Vec3 t = Cross(quat_vec) * (D)-2.0;
            
            return *this + (t * w) + quat_vec.Cross(t);
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

    template<typename D>
    inline Vec3<D> operator*(D Scalar, Vec3<D> Vector)
    {
        return Vector * Scalar;
    }

    template<typename D>
    inline Vec3<D> operator/(D Scalar, Vec3<D> Vector)
    {
        return Vector / Scalar;
    }

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