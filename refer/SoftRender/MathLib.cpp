// MathLib.cpp
//
// By NightHz
//

#include "MathLib.h"
#include <cmath>





Matrix::Matrix()
{
	float *p = (float*)m;
	for (int i = 0;i < 16;i++)
		(*(p++)) = 0.0f;
	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;
}

Matrix::Matrix(float value)
{
	float *p = (float*)m;
	for (int i = 0;i < 16;i++)
		(*(p++)) = value;
}

Matrix::Matrix(float _00, float _01, float _02, float _03, 
	float _10, float _11, float _12, float _13, 
	float _20, float _21, float _22, float _23, 
	float _30, float _31, float _32, float _33)
{
	m[0][0] = _00; m[0][1] = _01; m[0][2] = _02; m[0][3] = _03;
	m[1][0] = _10; m[1][1] = _11; m[1][2] = _12; m[1][3] = _13;
	m[2][0] = _20; m[2][1] = _21; m[2][2] = _22; m[2][3] = _23;
	m[3][0] = _30; m[3][1] = _31; m[3][2] = _32; m[3][3] = _33;
}

float & Matrix::operator()(int row, int col)
{
//	if (row < 0 || row >= 4 || col < 0 || col >= 4)
//		return m[0][0];
	return m[row][col];
}

Matrix & Matrix::operator=(Matrix matrix0)
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			m[i][j] = matrix0(i, j);
	return (*this);
}

Matrix & Matrix::operator*=(Matrix matrix0)
{
	Matrix result;
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			result(i, j) = m[i][0] * matrix0(0, j) + m[i][1] * matrix0(1, j) + m[i][2] * matrix0(2, j) + m[i][3] * matrix0(3, j);
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			m[i][j] = result(i, j);
	return (*this);
}

Matrix & Matrix::operator+=(Matrix matrix0)
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			m[i][j] = m[i][j] + matrix0(i, j);
	return (*this);
}

Matrix & Matrix::operator-=(Matrix matrix0)
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			m[i][j] = m[i][j] - matrix0(i, j);
	return (*this);
}

Matrix & Matrix::operator*=(float f0)
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			m[i][j] = m[i][j] * f0;
	return (*this);
}

Matrix & Matrix::operator/=(float f0)
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			m[i][j] = m[i][j] / f0;
	return (*this);
}

Matrix Matrix::operator-()
{
	Matrix result;
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			result(i, j) = -m[i][j];
	return result;
}

Matrix Matrix::operator*(Matrix matrix0)
{
	Matrix result;
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			result(i, j) = m[i][0] * matrix0(0, j) + m[i][1] * matrix0(1, j) + m[i][2] * matrix0(2, j) + m[i][3] * matrix0(3, j);
	return result;
}

Matrix Matrix::operator+(Matrix matrix0)
{
	Matrix result;
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			result(i, j) = result(i, j) + matrix0(i, j);
	return result;
}

Matrix Matrix::operator-(Matrix matrix0)
{
	Matrix result;
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			result(i, j) = result(i, j) - matrix0(i, j);
	return result;
}

Matrix Matrix::operator*(float f0)
{
	Matrix result;
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			result(i, j) = result(i, j) * f0;
	return result;
}

Matrix operator*(float f0, Matrix matrix0)
{
	Matrix result;
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			result(i, j) = f0 * matrix0(i, j);
	return result;
}

Matrix Matrix::operator/(float f0)
{
	Matrix result;
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			result(i, j) = result(i, j) / f0;
	return result;
}

bool Matrix::operator==(Matrix matrix0)
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			if (m[i][j] != matrix0(i, j))
				return false;
	return true;
}

bool Matrix::operator!=(Matrix matrix0)
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			if (m[i][j] != matrix0(i, j))
				return true;
	return false;
}

Matrix GetMatrixT(float tx, float ty, float tz)
{
	Matrix result;
	result(3, 0) = tx;
	result(3, 1) = ty;
	result(3, 2) = tz;
	return result;
}

Matrix GetInverseMatrixT(float tx, float ty, float tz)
{
	Matrix result;
	result(3, 0) = -tx;
	result(3, 1) = -ty;
	result(3, 2) = -tz;
	return result;
}

Matrix GetMatrixT(Point translation)
{
	Matrix result;
	result(3, 0) = translation.x;
	result(3, 1) = translation.y;
	result(3, 2) = translation.z;
	return result;
}

Matrix GetInverseMatrixT(Point translation)
{
	Matrix result;
	result(3, 0) = -translation.x;
	result(3, 1) = -translation.y;
	result(3, 2) = -translation.z;
	return result;
}

Matrix GetMatrixR(Vector at, Vector up)
{
	Matrix result;
	Vector xaxis, yaxis, zaxis;

	zaxis = VectorUnit(at);
	xaxis = VectorCross(up, zaxis);
	xaxis = VectorUnit(xaxis);
	yaxis = VectorCross(zaxis, xaxis);

	result(0, 0) = xaxis.x;
	result(1, 0) = yaxis.x;
	result(2, 0) = zaxis.x;

	result(0, 1) = xaxis.y;
	result(1, 1) = yaxis.y;
	result(2, 1) = zaxis.y;

	result(0, 2) = xaxis.z;
	result(1, 2) = yaxis.z;
	result(2, 2) = zaxis.z;
	return result;
}

Matrix GetInverseMatrixR(Vector at, Vector up)
{
	Matrix result;
	Vector xaxis, yaxis, zaxis;

	zaxis = VectorUnit(at);
	xaxis = VectorCross(up, zaxis);
	xaxis = VectorUnit(xaxis);
	yaxis = VectorCross(zaxis, xaxis);

	result(0, 0) = xaxis.x;
	result(1, 0) = xaxis.y;
	result(2, 0) = xaxis.z;

	result(0, 1) = yaxis.x;
	result(1, 1) = yaxis.y;
	result(2, 1) = yaxis.z;

	result(0, 2) = zaxis.x;
	result(1, 2) = zaxis.y;
	result(2, 2) = zaxis.z;

	return result;
}

Matrix GetMatrixRx(float theta)
{
	Matrix result;
	result(1, 1) = cosf(theta);
	result(1, 2) = sinf(theta);
	result(2, 1) = -result(1, 2);
	result(2, 2) = result(1, 1);
	return result;
}

Matrix GetMatrixRy(float theta)
{
	Matrix result;
	result(0, 0) = cosf(theta);
	result(2, 0) = sinf(theta);
	result(0, 2) = -result(2, 0);
	result(2, 2) = result(0, 0);
	return result;
}

Matrix GetMatrixRz(float theta)
{
	Matrix result;
	result(0, 0) = cosf(theta);
	result(0, 1) = sinf(theta);
	result(1, 0) = -result(0, 1);
	result(1, 1) = result(0, 0);
	return result;
}

Matrix GetMatrixS(float scale_x, float scale_y, float scale_z)
{
	Matrix result;
	result(0, 0) = scale_x;
	result(1, 1) = scale_y;
	result(2, 2) = scale_z;
	return result;
}

Matrix GetInverseMatrixS(float scale_x, float scale_y, float scale_z)
{
	Matrix result;
	result(0, 0) = 1/scale_x;
	result(1, 1) = 1/scale_y;
	result(2, 2) = 1/scale_z;
	return result;
}

Matrix GetMatrixS(float scale_xyz)
{
	Matrix result;
	result(0, 0) = scale_xyz;
	result(1, 1) = scale_xyz;
	result(2, 2) = scale_xyz;
	return result;
}

Matrix GetInverseMatrixS(float scale_xyz)
{
	Matrix result;
	result(0, 0) = 1/scale_xyz;
	result(1, 1) = 1/scale_xyz;
	result(2, 2) = 1/scale_xyz;
	return result;
}

Matrix GetMatrixP(float fovy, float aspect, float z_near, float z_far)
{
	Matrix result(0.0f);
	float f1 = 1 / tanf(fovy * 0.5f);
	result(0, 0) = (float)(f1 / aspect);
	result(1, 1) = (float)(f1);
	result(2, 2) = z_far / (z_far - z_near);
	result(3, 2) = -z_near * z_far / (z_far - z_near);
	result(2, 3) = 1.0f;
	return result;
}

Matrix GetMatrixE(float psi, float theta, float phi)
{
	Matrix result;
	result = GetMatrixRz(phi) * GetMatrixRx(theta) * GetMatrixRz(psi);
	return result;
}

Matrix GetMatrixE(EulerAngles euler_angles)
{
	Matrix result;
	result = GetMatrixRz(euler_angles.phi) * GetMatrixRx(euler_angles.theta) * GetMatrixRz(euler_angles.psi);
	return result;
}

Matrix GetInverseMatrixE(EulerAngles euler_angles)
{
	Matrix result;
	result = GetMatrixRz(-euler_angles.psi) * GetMatrixRx(-euler_angles.theta) * GetMatrixRz(-euler_angles.phi);
	return result;
}

Quaternion::Quaternion()
{
	q[0] = 0.0f;
	q[1] = 0.0f;
	q[2] = 0.0f;
	q[3] = 1.0f;
}

Quaternion::Quaternion(float value)
{
	q[0] = value;
	q[1] = value;
	q[2] = value;
	q[3] = value;
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

float & Quaternion::operator()(int index)
{
//	if (index < 0 || index >= 4)
//		return q[0];
	return q[index];
}

Quaternion & Quaternion::operator =(Quaternion quaternion0)
{
	for (int i = 0;i < 4;i++)
		q[i] = quaternion0(i);
	return (*this);
}

Quaternion & Quaternion::operator*=(Matrix matrix0)
{
	Quaternion result;
	for (int i = 0;i < 4;i++)
		result(i) = q[0] * matrix0(0, i) + q[1] * matrix0(1, i) + q[2] * matrix0(2, i) + q[3] * matrix0(3, i);
	for (int i = 0;i < 4;i++)
		q[i] = result(i);
	return (*this);
}

Quaternion & Quaternion::operator+=(Quaternion quaternion0)
{
	for (int i = 0;i < 4;i++)
		q[i] = q[i] + quaternion0(i);
	return (*this);
}

Quaternion & Quaternion::operator-=(Quaternion quaternion0)
{
	for (int i = 0;i < 4;i++)
		q[i] = q[i] - quaternion0(i);
	return (*this);
}

Quaternion & Quaternion::operator*=(float f0)
{
	for (int i = 0;i < 4;i++)
		q[i] = q[i] * f0;
	return (*this);
}

Quaternion & Quaternion::operator/=(float f0)
{
	for (int i = 0;i < 4;i++)
		q[i] = q[i] / f0;
	return (*this);
}

Quaternion Quaternion::operator-()
{
	Quaternion result;
	for (int i = 0;i < 4;i++)
		result(i) = -q[i];
	return result;
}

Quaternion Quaternion::operator*(Matrix matrix0)
{
	Quaternion result;
	for (int i = 0;i < 4;i++)
		result(i) = q[0] * matrix0(0, i) + q[1] * matrix0(1, i) + q[2] * matrix0(2, i) + q[3] * matrix0(3, i);
	return result;
}

Quaternion Quaternion::operator+(Quaternion quaternion0)
{
	Quaternion result;
	for (int i = 0;i < 4;i++)
		result(i) = q[i] + quaternion0(i);
	return result;
}

Quaternion Quaternion::operator-(Quaternion quaternion0)
{
	Quaternion result;
	for (int i = 0;i < 4;i++)
		result(i) = q[i] - quaternion0(i);
	return result;
}

Quaternion Quaternion::operator*(float f0)
{
	Quaternion result;
	for (int i = 0;i < 4;i++)
		result(i) = q[i] * f0;
	return result;
}

Quaternion operator*(float f0, Quaternion quaternion0)
{
	Quaternion result;
	for (int i = 0;i < 4;i++)
		result(i) = f0 * quaternion0(i);
	return result;
}

Quaternion Quaternion::operator/(float f0)
{
	Quaternion result;
	for (int i = 0;i < 4;i++)
		result(i) = q[i] / f0;
	return result;
}

bool Quaternion::operator==(Quaternion quaternion0)
{
	for (int i = 0;i < 4;i++)
		if (q[i] != quaternion0(i))
			return false;
	return true;
}

bool Quaternion::operator!=(Quaternion quaternion0)
{
	for (int i = 0;i < 4;i++)
		if (q[i] != quaternion0(i))
			return true;
	return false;
}

EulerAngles::EulerAngles()
{
	psi = 0;
	theta = 0;
	phi = 0;
}

EulerAngles::EulerAngles(float _psi, float _theta, float _phi)
{
	psi = _psi;
	theta = _theta;
	phi = _phi;
}

Point2D::Point2D()
{
	x = 0;
	y = 0;
}

Point2D::Point2D(int _x, int _y)
{
	x = _x;
	y = _y;
}

int MiddleValue(int x, int min, int max)
{
	if (x < min)
		return min;
	else if (x > max)
		return max;
	else
		return x;
}

float MiddleValue(float x, float min, float max)
{
	if (x < min)
		return min;
	else if (x > max)
		return max;
	else
		return x;
}

template<class T>
inline T Interp(T x1, T x2, float t)
{
	return x1 + (T)((x2 - x1)*t);
}

float VectorLength(Vector v1)
{
	return sqrtf(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
}

float VectorDot(Vector v1, Vector v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Vector VectorCross(Vector v1, Vector v2)
{
	Vector result;
	result.x = v1.y*v2.z - v1.z*v2.y;
	result.y = v1.z*v2.x - v1.x*v2.z;
	result.z = v1.x*v2.y - v1.y*v2.x;
	result.w = 1.0f;
	return result;
}

Vector VectorInterp(Vector v1, Vector v2, float t)
{
	Vector result;
	result.x = Interp(v1.x, v2.x, t);
	result.y = Interp(v1.y, v2.y, t);
	result.z = Interp(v1.z, v2.z, t);
	result.w = 1.0f;
	return result;
}

Vector VectorUnit(Vector v1)
{
	Vector result;
	float length = VectorLength(v1);
	if (length == 0)
		return v1;


	float f0 = 1 / length;
	result.x = v1.x*f0;
	result.y = v1.y*f0;
	result.z = v1.z*f0;
	result.w = 1.0f;

	return result;
}

Point PointStandard(Point p1)
{
	Point result;

	if (p1.w == 0)
		return p1;
	float f0 = 1 / p1.w;
	result.x = p1.x*f0;
	result.y = p1.y*f0;
	result.z = p1.z*f0;
	result.w = 1.0f;

	return result;
}

float PointDistance(Point p1, Point p2)
{
	Vector v1 = p1 - p2;
	return sqrtf(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
}

Vector VectorNormal(Point p1, Point p2, Point p3)
{
	return VectorUnit(VectorCross(p2 - p1, p3 - p1));
}

