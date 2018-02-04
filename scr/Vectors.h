#ifndef VECTORS_H_DEF
#define VECTORS_H_DEF

#include <cmath>
#include <iostream>


const float DEG2RAD = 3.141593f / 180.0f;

/********************************************************************************************************/

// 3D vector

	struct Vector3
{
	float x;
	float y;
	float z;

	// konstruktory//////////////////////////////////////////////////////////////////////////////////////

	Vector3() : x(0), y(0), z(0) {};
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

	// metody //////////////////////////////////////////////////////////////////////////////////////////

	void        set(float x, float y, float z);
	Vector3&    normalize();                     
	float       dot(const Vector3& vec) const;         
	Vector3     cross(const Vector3& vec) const;      


														
	Vector3     operator-() const;                     
	Vector3     operator+(const Vector3& rhs) const;   
	Vector3     operator-(const Vector3& rhs) const;   
	Vector3&    operator+=(const Vector3& rhs);        
	Vector3&    operator-=(const Vector3& rhs);        
	Vector3     operator*(const float scale) const;    
	Vector3     operator*(const Vector3& rhs) const;   
	Vector3&    operator*=(const float scale);         
	Vector3&    operator*=(const Vector3& rhs);        
	Vector3     operator/(const float scale) const;    
	Vector3&    operator/=(const float scale);         
	float       operator[](int index) const;           

	friend Vector3 operator*(const float a, const Vector3 vec);

};




// inline  ///////////////////////////////////////////////////////////////////////////////////////////////////

inline Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

inline Vector3 Vector3::operator+(const Vector3& rhs) const {
    return Vector3(x+rhs.x, y+rhs.y, z+rhs.z);
}

inline Vector3 Vector3::operator-(const Vector3& rhs) const {
    return Vector3(x-rhs.x, y-rhs.y, z-rhs.z);
}

inline Vector3& Vector3::operator+=(const Vector3& rhs) {
    x += rhs.x; y += rhs.y; z += rhs.z; return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& rhs) {
    x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
}

inline Vector3 Vector3::operator*(const float a) const {
    return Vector3(x*a, y*a, z*a);
}

inline Vector3 Vector3::operator*(const Vector3& rhs) const {
    return Vector3(x*rhs.x, y*rhs.y, z*rhs.z);
}

inline Vector3& Vector3::operator*=(const float a) {
    x *= a; y *= a; z *= a; return *this;
}

inline Vector3& Vector3::operator*=(const Vector3& rhs) {
    x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this;
}

inline Vector3 Vector3::operator/(const float a) const {
    return Vector3(x/a, y/a, z/a);
}

inline Vector3& Vector3::operator/=(const float a) {
    x /= a; y /= a; z /= a; return *this;
}

inline float Vector3::operator[](int index) const {
    return (&x)[index];
}


inline void Vector3::set(float x, float y, float z) {
    this->x = x; this->y = y; this->z = z;
}

inline Vector3& Vector3::normalize() {

    float xxyyzz = x*x + y*y + z*z;
    float invLength = 1.0f / sqrtf(xxyyzz);
    x *= invLength;
    y *= invLength;
    z *= invLength;
    return *this;
}

inline float Vector3::dot(const Vector3& rhs) const {
    return (x*rhs.x + y*rhs.y + z*rhs.z);
}

inline Vector3 Vector3::cross(const Vector3& rhs) const {
    return Vector3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
}

inline Vector3 operator*(const float a, const Vector3 vec) {
    return Vector3(a*vec.x, a*vec.y, a*vec.z);
}

#endif
