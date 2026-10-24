#ifndef VECTOR_STYLES_HPP
#define VECTOR_STYLES_HPP

#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

// Style 1 : membres separes (x, y, z)

class VectorSeparate {
    double x, y, z;
public:
    VectorSeparate() : x(0), y(0), z(0) {}
    VectorSeparate(double x, double y, double z) : x(x), y(y), z(z) {}

    VectorSeparate(const VectorSeparate& a) : x(a.x), y(a.y), z(a.z) {}
    VectorSeparate(VectorSeparate&& a) noexcept : x(a.x), y(a.y), z(a.z) {}
    VectorSeparate& operator=(const VectorSeparate& a) {
        if (this != &a) { x = a.x; y = a.y; z = a.z; }
        return *this;
    }
    VectorSeparate& operator=(VectorSeparate&& a) noexcept {
        if (this != &a) { x = a.x; y = a.y; z = a.z; }
        return *this;
    }

    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    VectorSeparate& operator+=(const VectorSeparate& a) { x += a.x; y += a.y; z += a.z; return *this; }
    VectorSeparate& operator-=(const VectorSeparate& a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
    VectorSeparate& operator*=(double k) { x *= k; y *= k; z *= k; return *this; }

    VectorSeparate operator-() const { return VectorSeparate(-x, -y, -z); }

    bool operator==(const VectorSeparate& a) const { return x == a.x && y == a.y && z == a.z; }
    bool operator!=(const VectorSeparate& a) const { return !(*this == a); }

    double dot(const VectorSeparate& a) const { return x*a.x + y*a.y + z*a.z; }
    double norm() const { return std::sqrt(x*x + y*y + z*z); }
};
inline VectorSeparate operator+(VectorSeparate a, const VectorSeparate& b) { return a += b; }
inline VectorSeparate operator-(VectorSeparate a, const VectorSeparate& b) { return a -= b; }
inline VectorSeparate operator*(VectorSeparate v, double k) { return v *= k; }

// Style 2 : std::array<double, 3>

class VectorArray {
    std::array<double, 3> d;
public:
    VectorArray() : d{0, 0, 0} {}
    VectorArray(double x, double y, double z) : d{x, y, z} {}

    VectorArray(const VectorArray& a) : d(a.d) {}
    VectorArray(VectorArray&& a) noexcept : d(a.d) {}
    VectorArray& operator=(const VectorArray& a) { d = a.d; return *this; }
    VectorArray& operator=(VectorArray&& a) noexcept { d = a.d; return *this; }

    double getX() const { return d[0]; }
    double getY() const { return d[1]; }
    double getZ() const { return d[2]; }

    VectorArray& operator+=(const VectorArray& a) {
        for (int i = 0; i < 3; ++i) d[i] += a.d[i];
        return *this;
    }
    VectorArray& operator-=(const VectorArray& a) {
        for (int i = 0; i < 3; ++i) d[i] -= a.d[i];
        return *this;
    }
    VectorArray& operator*=(double k) {
        for (int i = 0; i < 3; ++i) d[i] *= k;
        return *this;
    }

    VectorArray operator-() const { return VectorArray(-d[0], -d[1], -d[2]); }

    bool operator==(const VectorArray& a) const { return d == a.d; }
    bool operator!=(const VectorArray& a) const { return !(*this == a); }

    double dot(const VectorArray& a) const { return d[0]*a.d[0] + d[1]*a.d[1] + d[2]*a.d[2]; }
    double norm() const { return std::sqrt(dot(*this)); }
};
inline VectorArray operator+(VectorArray a, const VectorArray& b) { return a += b; }
inline VectorArray operator-(VectorArray a, const VectorArray& b) { return a -= b; }
inline VectorArray operator*(VectorArray v, double k) { return v *= k; }

#endif // VECTOR_STYLES_HPP