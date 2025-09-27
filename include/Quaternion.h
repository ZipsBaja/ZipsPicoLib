#include <cmath>
#include <cstdint>

namespace uazips
{
    template<typename D>
    struct Quaternion
    {

        D w, x, y, z;

        inline Quaternion() : w((D)1.0), x((D)0.0), y((D)0.0), z((D)0.0)
        {
        }

        inline Quaternion(D w, D x, D y, D z) : w(w), x(x), y(y), z(z)
        {
        }

        inline D Magnitude() const
        {
            return std::sqrt(w*w+x*x+y*y+z*z);
        }

        inline Quaternion Normalize() const
        {
            D mag = Magnitude();
            return {
                w / mag,
                x / mag,
                y / mag,
                z / mag
            };
        }

        inline Quaternion Inverse() const
        {
            D mag = Magnitude();
            return Conjugate() * ((D)1.0 / (mag*mag));
        }

        inline Quaternion operator*(const Quaternion& q) const
        {
            return Quaternion(
                w*q.w - x*q.x - y*q.y - z*q.z,  // new w
                w*q.x + x*q.w + y*q.z - z*q.y,  // new x
                w*q.y - x*q.z + y*q.w + z*q.x,  // new y
                w*q.z + x*q.y - y*q.x + z*q.w   // new z
            );
        }

        inline Quaternion operator*(D k) const
        {
            return {w*k, x*k, y*k, z*k};
        }

        inline Quaternion& operator*=(const Quaternion& q)
        {
            return *this * q;
        }

        inline Quaternion& operator*=(D k)
        {
            return *this * k;
        }

        inline Quaternion operator/(const Quaternion& other) const
        {
            return *this * other.Inverse();
        }

        inline Quaternion operator/(D k) const
        {
            return {w/k, x/k, y/k, z/k};
        }

        inline Quaternion& operator/=(const Quaternion& q)
        {
            return *this / q;
        }

        inline Quaternion& operator/=(D k)
        {
            return *this / k;
        }

        inline Quaternion Conjugate() const
        {
            return {w, -x, -y, -z};
        }

        inline Quaternion operator-() const
        {
            return {-w, -x, -y, -z};
        }

        inline Quaternion& operator++()
        {
            return {
                w + (D)1.0,
                x + (D)1.0,
                y + (D)1.0,
                z + (D)1.0
            };
        }

        inline Quaternion& operator--()
        {
            return {
                w - (D)1.0,
                x - (D)1.0,
                y - (D)1.0,
                z - (D)1.0
            };
        }

        inline bool operator==(const Quaternion& other) const
        {
            return (w == other.w && x == other.x && y == other.y && z == other.z);
        }

        inline bool operator!=(const Quaternion& other) const
        {
            return !(*this == other);
        }

    };

    template<typename D>
    Quaternion<D> operator*(D k, const Quaternion<D>& q)
    {
        return q * k;
    }
    template<typename D>
    Quaternion<D> operator/(D k, const Quaternion<D>& q)
    {
        return q / k;
    }

    using Quaternionf = Quaternion<float>;
    using Quaterniond = Quaternion<double>;
    using Quaternioni8 = Quaternion<int8_t>;
    using Quaternionu8 = Quaternion<uint8_t>;
    using Quaternioni16 = Quaternion<int16_t>;
    using Quaternionu16 = Quaternion<uint16_t>;
    using Quaternioni32 = Quaternion<int32_t>;
    using Quaternionu32 = Quaternion<uint32_t>;
    using Quaternioni64 = Quaternion<int64_t>;
    using Quaternionu64 = Quaternion<uint64_t>;

}