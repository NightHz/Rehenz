// MathLib.h
//
// By NightHz
// 
// The file defines: 
//		struct Matrix 
//		funcions which are uesd to get some special Matrix
//		struct Quaternion (Vector Point)
//		struct EulerAngles
//		struct Point2D
//		some common functions
// 

#pragma once

const float pi = 3.14159265f;



// 4*4����
struct Matrix;
// ��Ԫ��
struct Quaternion;
// ����(3D) ��(3D)
typedef Quaternion Vector, Point;
// ŷ����
struct EulerAngles;
// 2D��
struct Point2D;



// �ṹ 4*4����
struct Matrix
{
public:
	float m[4][4];

	Matrix();   // Ĭ��Ϊ��λ����
	Matrix(float value);   // ������0
	Matrix(float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33);

	// access date
	float& operator()(int row, int col);   // ����ǰ������Խ���飡
	Matrix& operator=(Matrix);

	// common operators
	Matrix& operator*=(Matrix);
	Matrix& operator+=(Matrix);
	Matrix& operator-=(Matrix);
	Matrix& operator*=(float);
	Matrix& operator/=(float);

	Matrix operator-();

	Matrix operator*(Matrix);
	Matrix operator+(Matrix);
	Matrix operator-(Matrix);
	Matrix operator*(float);
	friend Matrix operator*(float, Matrix);
	Matrix operator/(float);

	bool operator==(Matrix);
	bool operator!=(Matrix);
};

// �õ�ƽ�ƾ���
Matrix GetMatrixT(float tx, float ty, float tz);

// �õ�ƽ�ƾ���������
Matrix GetInverseMatrixT(float tx, float ty, float tz);

// �õ�ƽ�ƾ���
Matrix GetMatrixT(Point translation);

// �õ�ƽ�ƾ���������
Matrix GetInverseMatrixT(Point translation);

// �õ���ת����(ͨ����������(ԭ+z��at)����������(ԭ+y��Aup+Bat)������ת)
Matrix GetMatrixR(Vector at, Vector up);

// �õ���ת����������(ͨ����������(ԭ+z��at)����������(ԭ+y��Aup+Bat)������ת)
Matrix GetInverseMatrixR(Vector at, Vector up);

Matrix GetMatrixRx(float theta);

Matrix GetMatrixRy(float theta);

Matrix GetMatrixRz(float theta);

// �õ����ž���
Matrix GetMatrixS(float scale_x, float scale_y, float scale_z);

// �õ����ž���������
Matrix GetInverseMatrixS(float scale_x, float scale_y, float scale_z);

// �õ����ž���
Matrix GetMatrixS(float scale_xyz);

// �õ����ž���������
Matrix GetInverseMatrixS(float scale_xyz);

// �õ�ͶӰ����(���Ӿ���任Ϊ(-1,-1,0)(1,1,1)����ĳ�����) ʹ������ʹ�õ�(3D)����׼
Matrix GetMatrixP(float fovy, float aspect, float z_near, float z_far);

Matrix GetMatrixE(float psi, float theta, float phi);

Matrix GetMatrixE(EulerAngles euler_angles);

Matrix GetInverseMatrixE(EulerAngles euler_angles);

// �ṹ ��Ԫ��
struct Quaternion
{
public:
	union {
		float q[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
	};

	Quaternion();   // Ĭ�� w = 1 
	Quaternion(float value);   // ������0
	Quaternion(float _x, float _y, float _z, float _w);

	// access date
	float& operator()(int index);   // ����ǰ������Խ���飡
	Quaternion& operator=(Quaternion);

	// common operators
	Quaternion& operator*=(Matrix);
	Quaternion& operator+=(Quaternion);
	Quaternion& operator-=(Quaternion);
	Quaternion& operator*=(float);
	Quaternion& operator/=(float);

	Quaternion operator-();

	Quaternion operator*(Matrix);
	Quaternion operator+(Quaternion);
	Quaternion operator-(Quaternion);
	Quaternion operator*(float);
	friend Quaternion operator*(float, Quaternion);
	Quaternion operator/(float);

	bool operator==(Quaternion);
	bool operator!=(Quaternion);
};

// �ṹ ŷ����
struct EulerAngles
{
public:
	float psi;
	float theta;
	float phi;

	EulerAngles();
	EulerAngles(float _psi, float _theta, float _phi);
};

// �ṹ 2D��
struct Point2D
{
public:
	int x;
	int y;

	Point2D();
	Point2D(int _x, int _y);
};


// ����x��min��max֮��
int MiddleValue(int x, int min, int max);
float MiddleValue(float x, float min, float max);

// ��ֵ����
template <class T>
T Interp(T x1, T x2, float t);

// ʸ������
float VectorLength(Vector v1);

// ʸ�����
float VectorDot(Vector v1, Vector v2);

// ʸ�����(w��Ϊһ)
Vector VectorCross(Vector v1, Vector v2);

// ʸ����ֵ(w��Ϊһ)
Vector VectorInterp(Vector v1, Vector v2, float t);

// ʸ����λ��(w��Ϊһ)
Vector VectorUnit(Vector v1);

// w��һ(ͨ������)
Point PointStandard(Point p1);

// ������������
float PointDistance(Point p1, Point p2);

// ���������ε�λ������
Vector VectorNormal(Point p1, Point p2, Point p3);




