#include <glad/glad.h>
#include <cmath>
#include <algorithm>
#include "Matrix.h"


const float RAD2DEG = 180.0f / 3.14159265359f;
const float EPSILON = 0.00001f;



Matrix4& Matrix4::translate(const Vector3& v)
{
	return translate(v.x, v.y, v.z);
}

Matrix4& Matrix4::translate(float x, float y, float z)
{
	m[0] += m[3] * x;   m[4] += m[7] * x;   m[8] += m[11] * x;   m[12] += m[15] * x;
	m[1] += m[3] * y;   m[5] += m[7] * y;   m[9] += m[11] * y;   m[13] += m[15] * y;
	m[2] += m[3] * z;   m[6] += m[7] * z;   m[10] += m[11] * z;   m[14] += m[15] * z;

	return *this;
}

Matrix4& Matrix4::scale(float s)
{
	return scale(s, s, s);
}

Matrix4& Matrix4::scale(float x, float y, float z)
{
	m[0] *= x;   m[4] *= x;   m[8] *= x;   m[12] *= x;
	m[1] *= y;   m[5] *= y;   m[9] *= y;   m[13] *= y;
	m[2] *= z;   m[6] *= z;   m[10] *= z;   m[14] *= z;
	return *this;
}


Matrix4& Matrix4::rotate(float angle, float x, float y, float z)
{
	float c = cosf(angle * DEG2RAD);    // cos
	float s = sinf(angle * DEG2RAD);    // sin
	float c1 = 1.0f - c;                // 1 - c
	float m0 = m[0], m4 = m[4], m8 = m[8], m12 = m[12],
		m1 = m[1], m5 = m[5], m9 = m[9], m13 = m[13],
		m2 = m[2], m6 = m[6], m10 = m[10], m14 = m[14];


	float r0 = x * x * c1 + c;
	float r1 = x * y * c1 + z * s;
	float r2 = x * z * c1 - y * s;
	float r4 = x * y * c1 - z * s;
	float r5 = y * y * c1 + c;
	float r6 = y * z * c1 + x * s;
	float r8 = x * z * c1 + y * s;
	float r9 = y * z * c1 - x * s;
	float r10 = z * z * c1 + c;

	m[0] = r0 * m0 + r4 * m1 + r8 * m2;
	m[1] = r1 * m0 + r5 * m1 + r9 * m2;
	m[2] = r2 * m0 + r6 * m1 + r10 * m2;
	m[4] = r0 * m4 + r4 * m5 + r8 * m6;
	m[5] = r1 * m4 + r5 * m5 + r9 * m6;
	m[6] = r2 * m4 + r6 * m5 + r10 * m6;
	m[8] = r0 * m8 + r4 * m9 + r8 * m10;
	m[9] = r1 * m8 + r5 * m9 + r9 * m10;
	m[10] = r2 * m8 + r6 * m9 + r10 * m10;
	m[12] = r0 * m12 + r4 * m13 + r8 * m14;
	m[13] = r1 * m12 + r5 * m13 + r9 * m14;
	m[14] = r2 * m12 + r6 * m13 + r10 * m14;

	return *this;
}


Matrix4 Matrix4::camlookAt(Vector3 & eye, Vector3 & target, Vector3 & upDir)
{
	Vector3 forward = eye - target;
	forward.normalize();                

	Vector3 left = upDir.cross(forward); 
	left.normalize();

	Vector3 up = forward.cross(left);  

	Matrix4 matrix;
	matrix.identity();

	matrix[0] = left.x;
	matrix[4] = left.y;
	matrix[8] = left.z;
	matrix[1] = up.x;
	matrix[5] = up.y;
	matrix[9] = up.z;
	matrix[2] = forward.x;
	matrix[6] = forward.y;
	matrix[10] = forward.z;

	matrix[12] = -left.x * eye.x - left.y * eye.y - left.z * eye.z;
	matrix[13] = -up.x * eye.x - up.y * eye.y - up.z * eye.z;
	matrix[14] = -forward.x * eye.x - forward.y * eye.y - forward.z * eye.z;

	return matrix;
}

Matrix4 Matrix4::perspective(double fovY, double aspectRatio, double front, double back)
{
	double tangent = tan(fovY / 2 * DEG2RAD);   
	double height = front * tangent;          
	double width = height * aspectRatio;      							 
	return setFrustum(-width, width, -height, height, front, back);
}


Matrix4 Matrix4::setFrustum(float l, float r, float b, float t, float n, float f)
{
	Matrix4 matrix;
	matrix[0] = 2 * n / (r - l);
	matrix[5] = 2 * n / (t - b);
	matrix[8] = (r + l) / (r - l);
	matrix[9] = (t + b) / (t - b);
	matrix[10] = -(f + n) / (f - n);
	matrix[11] = -1;
	matrix[14] = -(2 * f * n) / (f - n);
	matrix[15] = 0;
	return matrix;
}
