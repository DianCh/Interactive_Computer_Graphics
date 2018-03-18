// written by Dian Chen for CIS 560 homework 01

#include "vec4.h"
#include <iostream>
#include <math.h>


///----------------------------------------------------------------------
/// Constructors
///----------------------------------------------------------------------
///
/// Default constructor sets all components to 0.
vec4::vec4() : data{0, 0, 0, 0} {}

/// constructor with specified values for each component
vec4::vec4(float x, float y, float z, float w) : data{x, y, z, w} {}

/// copy constructor copying corresponding components
vec4::vec4(const vec4 &v2) : data{v2.data[0], v2.data[1], v2.data[2], v2.data[3]} {}

///----------------------------------------------------------------------
/// Getters/Setters
///----------------------------------------------------------------------
///
/// overloading [] operator to return the indexed value
float vec4::operator[](unsigned int index) const
{
    if (index > 3)
    {
        throw std::out_of_range("The provided index goes out of bound.");
    }
    return data[index];
}

/// overloading [] operator to return a reference of the indexed value
float& vec4::operator[](unsigned int index)
{
    if (index > 3)
    {
        throw std::out_of_range("The provided index goes out of bound.");
    }
    return data[index];
}

///----------------------------------------------------------------------
/// Operator Functions
///----------------------------------------------------------------------
///
/// overloading + operator to do element-wise assigning
vec4& vec4::operator=(const vec4 &v2)
{
    data[0] = v2.data[0];
    data[1] = v2.data[1];
    data[2] = v2.data[2];
    data[3] = v2.data[3];
    return *this;
}

/// overloading == operator to do element-wise comparing
bool vec4::operator==(const vec4 &v2) const
{
    // The discrepancy of each pair should all lie in range EPSILON
    // so that two vectors can be viewed as equal.
    return fabsf(data[0] - v2.data[0]) < EPSILON &&
           fabsf(data[1] - v2.data[1]) < EPSILON &&
           fabsf(data[2] - v2.data[2]) < EPSILON &&
           fabsf(data[3] - v2.data[3]) < EPSILON;
}

/// overloading != operator to do element-wise comparing
bool vec4::operator!=(const vec4 &v2) const
{
    // The opposite of "equal".
    return !(*this == v2);
}

/// Arithmetic:
/// e.g. += adds v2 to this and return this (like regular +=)
///      +  returns a new vector that is sum of this and v2
///
/// First we implement the basic ones: +, -, *, /
vec4 vec4::operator+(const vec4 &v2) const
{
    return vec4(data[0] + v2.data[0], data[1] + v2.data[1],
                data[2] + v2.data[2], data[3] + v2.data[3]);
}

vec4 vec4::operator-(const vec4 &v2) const
{
    return vec4(data[0] - v2.data[0], data[1] - v2.data[1],
                data[2] - v2.data[2], data[3] - v2.data[3]);
}

vec4 vec4::operator*(float c) const
{
    return vec4(data[0] * c, data[1] * c,
                data[2] * c, data[3] * c);
}

vec4 vec4::operator/(float c) const
{
    return vec4(data[0] / c, data[1] / c,
                data[2] / c, data[3] / c);
}

/// Then we make use of the basic operators to do operation & assignment
vec4& vec4::operator+=(const vec4 &v2)
{
    *this = *this + v2;
    return *this;
}

vec4& vec4::operator-=(const vec4 &v2)
{
    *this = *this - v2;
    return *this;
}

vec4& vec4::operator*=(float c)
{
    *this = *this * c;
    return *this;
}

vec4& vec4::operator/=(float c)
{
    *this = *this / c;
    return *this;
}

/// Now implement functions that are not vec4 class methods.
float dot(const vec4 &v1, const vec4 &v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
}

vec4 cross(const vec4 &v1, const vec4 &v2)
{
    return vec4(v1[1] * v2[2] - v1[2] * v2[1],
                v1[2] * v2[0] - v1[0] * v2[2],
                v1[0] * v2[1] - v1[1] * v2[0],
                0);
}

float length(const vec4 &v)
{
    // Make use of the dot product implemented.
    return sqrtf(dot(v, v));
}

vec4 operator*(float c, const vec4 &v)
{
    return vec4(v[0] * c, v[1] * c,
                v[2] * c, v[3] * c);
}

vec4 normalize(const vec4& v)
{
    // Make use of the length function implemented.
    return v / length(v);
}

std::ostream& operator<<(std::ostream &o, const vec4 &v)
{
    o << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
    return o;
}
