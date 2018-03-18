// written by Dian Chen for CIS 560 homework 01

#include "mat4.h"
#include "vec4.h"
#include <iostream>

///----------------------------------------------------------------------
/// Constructors
///----------------------------------------------------------------------
///
/// Default Constructor. Initialize to identity matrix.
mat4::mat4() : data{vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1)} {}

/// Initializes the diagonal values of the matrix to diag. All other values are 0.
mat4::mat4(float diag) : data{vec4(diag,0,0,0), vec4(0,diag,0,0),
                             vec4(0,0,diag,0), vec4(0,0,0,diag)} {}

/// Initializes matrix with each vector representing a column in the matrix
mat4::mat4(const vec4 &col0, const vec4 &col1, const vec4 &col2, const vec4& col3) :
    data{col0, col1, col2, col3} {}

mat4::mat4(const mat4 &m2) : data{m2.data[0], m2.data[1], m2.data[2], m2.data[3]} {}

///----------------------------------------------------------------------
/// Getters
///----------------------------------------------------------------------
/// Returns the values of the column at the index
vec4 mat4::operator[](unsigned int index) const
{
    if (index > 3)
    {
        throw std::out_of_range("The provided index goes out of bound.");
    }
    return data[index];
}

/// Returns a reference to the column at the index
vec4& mat4::operator[](unsigned int index)
{
    if (index > 3)
    {
        throw std::out_of_range("The provided index goes out of bound.");
    }
    return data[index];
}

///----------------------------------------------------------------------
/// Static Initializers
///----------------------------------------------------------------------
/// Creates a 3-D rotation matrix.
/// Takes an angle in degrees and an axis represented by its xyz components, and outputs a 4x4 rotation matrix
/// You may choose to only handle the three cardinal axes of rotation.
mat4 mat4::rotate(float angle, float x, float y, float z)
{
    // Accept an angle in degrees and convert it to radians for function use.
    angle = angle / 180.0 * M_PI;

    // Make sure that the axis is normalized.
    float length = sqrtf(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;

    // Get the four columns of the matrix using Rodrigues' Rotation Formula.
    vec4 v1(cosf(angle) + x * x * (1 - cosf(angle)),
            z * sinf(angle) + x * y * (1 - cosf(angle)),
            - y * sinf(angle) + x * z * (1 - cos(angle)),
            0);
    vec4 v2(- z * sinf(angle) + x * y * (1 - cosf(angle)),
            cosf(angle) + y * y * (1 - cosf(angle)),
            x * sinf(angle) + y * z * (1 - cosf(angle)),
            0);
    vec4 v3(y * sinf(angle) + x * z * (1 - cosf(angle)),
            - x * sinf(angle) + y * z * (1 - cosf(angle)),
            cosf(angle) + z * z * (1 - cosf(angle)),
            0);
    vec4 v4(0, 0, 0, 1);

    // Construct the matrix and return
    return mat4(v1, v2, v3, v4);
}

/// Takes an xyz displacement and outputs a 4x4 translation matrix
mat4 mat4::translate(float x, float y, float z)
{
    // Get the four columns of the translation matrix.
    vec4 v1(1, 0, 0, 0);
    vec4 v2(0, 1, 0, 0);
    vec4 v3(0, 0, 1, 0);
    vec4 v4(x, y, z, 1);

    // Construct the matrix and return.
    return mat4(v1, v2, v3, v4);
}

/// Takes an xyz scale and outputs a 4x4 scale matrix
mat4 mat4::scale(float x, float y, float z)
{
    // Get the four columns of the scale matrix.
    vec4 v1(x, 0, 0, 0);
    vec4 v2(0, y, 0, 0);
    vec4 v3(0, 0, z, 0);
    vec4 v4(0, 0, 0, 1);

    // Construct the matrix and return.
    return mat4(v1, v2, v3, v4);
}

/// Generates a 4x4 identity matrix
mat4 mat4::identity()
{
    // Get the four columns of the identity matrix.
    vec4 v1(1, 0, 0, 0);
    vec4 v2(0, 1, 0, 0);
    vec4 v3(0, 0, 1, 0);
    vec4 v4(0, 0, 0, 1);

    // Construct the matrix and return.
    return mat4(v1, v2, v3, v4);
}

///----------------------------------------------------------------------
/// Operator Functions
///----------------------------------------------------------------------

/// Assign m2 to this and return it
mat4& mat4::operator=(const mat4 &m2)
{
    data[0] = m2.data[0];
    data[1] = m2.data[1];
    data[2] = m2.data[2];
    data[3] = m2.data[3];
    return *this;
}

/// Test for equality
bool mat4::operator==(const mat4 &m2) const
{
    return data[0] == m2.data[0] &&
           data[1] == m2.data[1] &&
           data[2] == m2.data[2] &&
           data[3] == m2.data[3];
}

/// Test for inequality
bool mat4::operator!=(const mat4 &m2) const
{
    // The opposite of "equal".
    return !(*this == m2);
}

/// Element-wise arithmetic
/// e.g. += adds the elements of m2 to this and returns this (like regular +=)
///      +  returns a new matrix whose elements are the sums of this and v2
///
/// First we implement the basic ones: +, -, *, /
mat4 mat4::operator+(const mat4 &m2) const
{
    return mat4(data[0] + m2[0], data[1] + m2[1], data[2] + m2[2], data[3] + m2[3]);
}

mat4 mat4::operator-(const mat4 &m2) const
{
    return mat4(data[0] - m2[0], data[1] - m2[1], data[2] - m2[2], data[3] - m2[3]);
}

mat4 mat4::operator*(float c) const
{
    return mat4(data[0] * c, data[1] * c, data[2] * c, data[3] * c);
}

mat4 mat4::operator/(float c) const
{
    return mat4(data[0] / c, data[1] / c, data[2] / c, data[3] / c);
}

mat4& mat4::operator+=(const mat4 &m2)
{
    *this = *this + m2;
    return *this;
}

mat4& mat4::operator-=(const mat4 &m2)
{
    *this = *this - m2;
    return *this;
}

mat4& mat4::operator*=(float c)
{
    *this = *this * c;
    return *this;
}

mat4& mat4::operator/=(float c)
{
    *this = *this / c;
    return *this;
}
/// Matrix multiplication (m1 * m2)
mat4 mat4::operator*(const mat4 &m2) const
{
    // Make use of the row function implemented.
    vec4 v1(dot(row(*this, 0), m2[0]), dot(row(*this, 1), m2[0]),
            dot(row(*this, 2), m2[0]), dot(row(*this, 3), m2[0]));

    vec4 v2(dot(row(*this, 0), m2[1]), dot(row(*this, 1), m2[1]),
            dot(row(*this, 2), m2[1]), dot(row(*this, 3), m2[1]));

    vec4 v3(dot(row(*this, 0), m2[2]), dot(row(*this, 1), m2[2]),
            dot(row(*this, 2), m2[2]), dot(row(*this, 3), m2[2]));

    vec4 v4(dot(row(*this, 0), m2[3]), dot(row(*this, 1), m2[3]),
            dot(row(*this, 2), m2[3]), dot(row(*this, 3), m2[3]));

    // Construct and return the final matrix.
    return mat4(v1, v2, v3, v4);
}

/// Matrix/vector multiplication (m * v)
/// Assume v is a column vector (ie. a 4x1 matrix)
vec4 mat4::operator*(const vec4 &v) const
{
    // Because we're multiplying the matrix with a column vector on the
    // right, the result should also be a column vector.

    // Compute the four components of the column vector, construct and
    // return it.
    return vec4(dot(v, row(*this, 0)), dot(v, row(*this, 1)),
                dot(v, row(*this, 2)), dot(v, row(*this, 3)));
}

///----------------------------------------------------------------------
/// Matrix Operations
///----------------------------------------------------------------------
/// Returns the transpose of the input matrix (v_ij == v_ji)
mat4 transpose(const mat4 &m)
{
    // Make use of the row function implemented.
    return mat4(row(m, 0), row(m, 1), row(m, 2), row(m, 3));
}

/// Returns a row of the input matrix
vec4 row(const mat4 &m, unsigned int index)
{
    return vec4(m[0][index], m[1][index], m[2][index], m[3][index]);
}

/// Scalar multiplication (c * m)
mat4 operator*(float c, const mat4 &m)
{
    // Use overloaded * operator on the four column vectors to construct
    // the new matrix and return it.
    return mat4(m[0] * c, m[1] * c, m[2] * c, m[3] * c);
}

/// Vector/matrix multiplication (v * m)
/// Assume v is a row vector (ie. a 1x4 matrix)
vec4 operator*(const vec4 &v, const mat4 &m)
{
    // Because we're multiplying the matrix with a row vector on the left,
    // the result should also be a row vector.
    // Compute the four components of the row vetor, construct and return it.
    return vec4(dot(v, m[0]), dot(v, m[1]), dot(v, m[2]), dot(v, m[3]));
}

/// Prints the matrix to a stream in a nice format
std::ostream &operator<<(std::ostream &o, const mat4 &m)
{
    // Make use of overloaded << operator of vec4 class
    o << "[" << row(m, 0) << " \n " << row(m, 1) << " \n " << row(m, 2) << " \n " << row(m, 3) << "]";
    return o;
}
