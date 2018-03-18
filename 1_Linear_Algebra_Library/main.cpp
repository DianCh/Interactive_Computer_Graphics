// Base code written by Jan Allbeck, Chris Czyzewicz, Cory Boatright, Tiantian Liu, Benedict Brown, and Adam Mally
// University of Pennsylvania

// At least some C++ compilers do funny things
// to C's math.h header if you don't define this

// written by Dian Chen for CIS 560 homework 01

#define _USE_MATH_DEFINES

#include "vec4.h"
#include "mat4.h"
#include <iostream>
#include <math.h>

using namespace std;

int main() {

    /// An index holder for [] testing.
    unsigned int index;

    ///----------------------------------------------------------------------
    /// vvvvvTests for vec4
    ///----------------------------------------------------------------------

    cout << "///--------Tests for vec4--------///" << endl;
    getchar();

    /// Construct vectors using different constructors.
    cout << "Constructing vectors using different constructors." << endl;
    getchar();
    vec4 v1, v2;
    vec4 v3(5, 6, 7, 8);
    vec4 v4 = v3;
    vec4 v5(3, 4, 5, 5);
    float c = 2.5;

    /// Test vec4 stream.
    cout << "The constructed vectors are (also test for vec4 stream):\n" ;
    cout << "v1 = " << v1 << endl;
    cout << "v2 = " << v2 << endl;
    cout << "v3 = " << v3 << endl;
    cout << "v4 = " << v4 << endl;
    cout << "v5 = " << v5 << endl;

    /// Test getters and setters.
    cout << "Test getters and setters." << endl;
    getchar();
    cout << "The first element in v1 is: " << v1[0] << endl;
    v1[0] = 1.0;
    cout << "After v1[0] = 1.0, the first element in v1 is now changed to: " << v1[0] << endl;
    cout << "Specify an element you want from v1: ";
    cin >> index;
    cout << "v1[" << index << "] is " << v1[index] << endl;

    /// Test assignment, equality and inequality.
    cout << "\nTest assignment, equality and inequality." << endl;
    getchar();
    cout << "v2 is originally: " << v2 << "and v5 is " << v5 << endl;
    v2 = v5;
    cout << "After v2 = v5, v2 is now: " << v2 << endl;
    cout << "Consider three vectors, v3 = " << v3 << ", v4 = " << v4 << ", and v5 = "
         << v5 << endl;
    cout << "Is v3 equal to v4? " << ((v3 == v4)? "Yes" : "No") << endl;
    cout << "Is v3 equal to v5? " << ((v3 == v5)? "Yes" : "No") << endl;
    cout << "Is v3 inequal to v4? " << ((v3 != v4)? "Yes" : "No") << endl;
    cout << "Is v3 inequal to v5? " << ((v3 != v5)? "Yes" : "No") << endl;

    /// Test basic arithmetics.
    cout << "\nTest basic arithmetics." << endl;
    getchar();
    cout << "Consider two vectors, v1 = " << v1 << ", v3 = " << v3 << ", and a float number c = "
         << c << endl;
    cout << "v1 + v3 = " << (v1 + v3) << endl;
    cout << "v1 - v3 = " << (v1 - v3) << endl;
    cout << "v3 * c = " << (v3 * c) << endl;
    cout << "v3 / c = " << (v3 / c) << endl;

    cout << "Consider vectors v1 = " << v1 << ", v2 =" << v2 << ",v3 = " << v3 << ", v4 = "
         << v4 << ", and a float number c = " << c << endl;
    v1 += v3;
    v2 -= v3;
    v3 *= c;
    v4 /= c;

    cout << "After performing v1 += v3, v1 is now: " << v1 << endl;
    cout << "After performing v2 -= v3, v2 is now: " << v2 << endl;
    cout << "After performing v3 *= c, v3 is now: " << v3 << endl;
    cout << "After performing v4 /= c, v4 is now: " << v4 << endl;

    /// Test dot product, cross product and length.
    cout << "\nTest dot product, cross product and length." << endl;
    getchar();
    cout << "Consider two vectors, v1 = " << v1 << " and v2 = " << v2 << endl;
    cout << "The dot product of v1 and v2 is dot(v1, v2) = " << dot(v1, v2) << " as confirmed in Matlab." << endl;
    cout << "The cross product of v1 and v2 is cross(v1, v2) = " << cross(v1, v2) << " as confirmed in Matlab." << endl;
    cout << "The length of v1 is length(v1) = " << length(v1) << " as confirmed in Matlab." << endl;

    /// Test scalar multiply (c * v) and the normalizer.
    cout << "\nTest scalar multiply (c * v) and the normalizer." << endl;
    getchar();
    cout << "Finally, consider vector v1 = " << v1 << " and float number c = " << c << endl;
    cout << "c * v1 = " << (c * v1) << endl;
    cout << "Normalized v1 is " << normalize(v1) << " as confirmed in Matlab." << endl;

    cout << "///--------End of tests for vec4--------///" << endl;
    getchar();

    ///----------------------------------------------------------------------
    /// ^^^^^Tests for vec4
    ///----------------------------------------------------------------------


    ///----------------------------------------------------------------------
    /// vvvvvTests for mat4
    ///----------------------------------------------------------------------

    cout << "///--------Tests for mat4--------///" << endl;
    getchar();
    /// Construct matrices using different constructors.
    /// Vectors to be used are:
    /// v1 = (6, 6, 7, 8)
    /// v2 = (-2, -2, -2, -3)
    /// v3 = (12.5, 15, 17.5, 20)
    /// v4 = (2, 2.4, 2.8, 3.2)
    cout << "Constructing matrices using different constructors." << endl;
    getchar();
    mat4 m1, m2;
    mat4 m3(3.0);
    mat4 m4(v1, v2, v3, v4);
    mat4 m5(v1, v3, v1, v1);
    mat4 m6 = m3;

    /// Test mat4 stream.
    cout << "The constructed matrices are (also test for mat4 stream):\n" ;
    cout << "m1 = \n" << m1 << endl;
    cout << "m2 = \n" << m2 << endl;
    cout << "m3 = \n" << m3 << endl;
    cout << "m4 = \n" << m4 << endl;
    cout << "m5 = \n" << m5 << endl;
    cout << "m6 = \n" << m6 << endl;

    /// Test getters and setters.
    cout << "Test getters and setters." << endl;
    getchar();
    cout << "The first column of m1 is " << m1[0] << endl;
    m1[0] = v1;
    cout << "After m1[0] = v1, the first column of m1 is now changed to: " << m1[0] << endl;
    cout << "Specify a column you want from m1: ";
    cin >> index;
    cout << "m1[" << index << "] is " << m1[index] << endl;

    /// Test static initializers.
    cout << "\nTest static initializers." << endl;
    getchar();
    cout << "Generate a rotation matrix that rotate 30 degrees around (1, 0, 0):\n"
         << mat4::rotate(30, 1, 0, 0) << endl;
    cout << "Generate a rotation matrix that rotate 30 degrees around (0, 1, 0):\n"
         << mat4::rotate(30, 0, 1, 0) << endl;
    cout << "Generate a rotation matrix that rotate 30 degrees around (0, 0, 1):\n"
         << mat4::rotate(30, 0, 0, 1) << endl;
    cout << "Generate a rotation matrix that basically does nothing:\n"
         << mat4::rotate(0, 1, 0, 0) << endl;
    cout << "Generate a translation matrix that translates 1, 2, 3 units in x, y, z:\n"
         << mat4::translate(1, 2, 3) << endl;
    cout << "Generate a scale matrix that scales 1, 2, 3 units in x, y, z:\n"
         << mat4::scale(1, 2, 3) << endl;
    cout << "Generate an identity matrix:\n" << mat4::identity() << endl;

    /// Test assignment, equality and inequality.
    cout << "\nTest assignment, equality and inequality." << endl;
    getchar();
    cout << "m2 is originally :\n" << m2 << "\nand m3 is :\n" << m3 << endl;
    m2 = m3;
    cout << "After m2 = m3, m2 is now:\n" << m2 << endl;
    cout << "Consider three matrices\n"
         << "m3 = \n" << m3 << "\n"
         << "m5 = \n" << m5 << "\n"
         << "m6 = \n" << m6 << endl;
    cout << "Is m3 equal to m6? " << ((m3 == m6)? "Yes" : "No") << endl;
    cout << "Is m3 equal to m5? " << ((m3 == m5)? "Yes" : "No") << endl;
    cout << "Is m3 inequal to m6? " << ((m3 != m6)? "Yes" : "No") << endl;
    cout << "Is m3 inequal to m5? " << ((m3 != m5)? "Yes" : "No") << endl;

    /// Test basic arithmetics: +, -, *, /.
    cout << "\nTest basic arithmetics: +, -, *, /." << endl;
    getchar();
    cout << "Consider a float number c = " << c << ", and two matrices:\n"
         << "m5 = \n" << m5 << "\n"
         << "m3 = \n" << m3 << endl;
    cout << "m5 + m3 = \n" << (m5 + m3) << " as confirmed in Matlab." << endl;
    cout << "m5 - m3 = \n" << (m5 - m3) << " as confirmed in Matlab." << endl;
    cout << "m5 * c = \n" << (m5 * c) << " as confirmed in Matlab." << endl;
    cout << "c * m5 = \n" << (c * m5) << " as confirmed in Matlab." << endl;
    cout << "m5 / c = \n" << (m5 / c) << " as confirmed in Matlab." << endl;

    /// Test basic arithmetics: +=, -=, *=, /=.
    cout << "\nTest basic arithmetics: +=, -=, *=, /=." << endl;
    getchar();
    cout << "Consider a float number c = " << c << "and five matrices\n"
         << "m1 = \n" << m1 << "\n"
         << "m2 = \n" << m2 << "\n"
         << "m3 = \n" << m3 << "\n"
         << "m4 = \n" << m4 << "\n"
         << "m5 = \n" << m5 << endl;
    m4 += m2;
    m5 -= m2;
    m1 *= c;
    m3 /= c;
    cout << "After performing m4 += m2, m4 is now:\n" << m4 << " as confirmed in Matlab." << endl;
    cout << "After performing m5 -= m2, m5 is now:\n" << m5 << " as confirmed in Matlab." << endl;
    cout << "After performing m1 *= c, m1 is now:\n" << m4 << " as confirmed in Matlab." << endl;
    cout << "After performing m3 /= c, m3 is now:\n" << m4 << " as confirmed in Matlab." << endl;

    /// Test matrix multiplication, matrix-vector multiplication and vector-matrix multiplication.
    cout << "\nTest matrix multiplication, matrix-vector multiplication and vector-matrix multiplication." << endl;
    getchar();
    cout << "Consider a column vector v2 = " << v2 << "and two matrices\n"
         << "m1 = \n" << m1 << "\n"
         << "m2 = \n" << m2 << "\n";
    cout << "m1 * m2 = \n" << (m1 * m2) << " as confirmed in Matlab." << endl;
    cout << "m1 * v2 = " << (m1 * v2) << " as confirmed in Matlab." << endl;
    cout << "View v2 as a row vector, then\n v2 * m1 = " << (v2 * m1) << " as confirmed in Matlab." << endl;

    /// Test getting transposes (stream is actually realized using row getters, so no further tests
    /// are necessary for row(const mat4&, unsigned int)).
    cout << "\nTest getting transposes (stream is actually realized using row getters, so no further tests are necessary for row(const mat4&, unsigned int))." << endl;
    getchar();
    cout << "Consider matrix m1 = \n" << m1 << endl;
    cout << "The transpose of m1 is \n" << transpose(m1) << endl;

    cout << "///--------End of tests for mat4--------///" << endl;
    getchar();

    ///----------------------------------------------------------------------
    /// ^^^^^Tests for mat4
    ///----------------------------------------------------------------------

    return 0;
}
