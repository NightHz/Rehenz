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



// 4*4矩阵
struct Matrix;
// 四元数
struct Quaternion;
// 向量(3D) 点(3D)
typedef Quaternion Vector, Point;
// 欧拉角
struct EulerAngles;
// 2D点
struct Point2D;



// 结构 4*4矩阵
struct Matrix
{
public:
	float m[4][4];

	Matrix();   // 默认为单位矩阵
	Matrix(float value);   // 用于置0
	Matrix(float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33);

	// access date
	float& operator()(int row, int col);   // 调用前做数组越界检查！
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

// 得到平移矩阵
Matrix GetMatrixT(float tx, float ty, float tz);

// 得到平移矩阵的逆矩阵
Matrix GetInverseMatrixT(float tx, float ty, float tz);

// 得到平移矩阵
Matrix GetMatrixT(Point translation);

// 得到平移矩阵的逆矩阵
Matrix GetInverseMatrixT(Point translation);

// 得到旋转矩阵(通过朝向向量(原+z现at)与向上向量(原+y现Aup+Bat)定义旋转)
Matrix GetMatrixR(Vector at, Vector up);

// 得到旋转矩阵的逆矩阵(通过朝向向量(原+z现at)与向上向量(原+y现Aup+Bat)定义旋转)
Matrix GetInverseMatrixR(Vector at, Vector up);

Matrix GetMatrixRx(float theta);

Matrix GetMatrixRy(float theta);

Matrix GetMatrixRz(float theta);

// 得到缩放矩阵
Matrix GetMatrixS(float scale_x, float scale_y, float scale_z);

// 得到缩放矩阵的逆矩阵
Matrix GetInverseMatrixS(float scale_x, float scale_y, float scale_z);

// 得到缩放矩阵
Matrix GetMatrixS(float scale_xyz);

// 得到缩放矩阵的逆矩阵
Matrix GetInverseMatrixS(float scale_xyz);

// 得到投影矩阵(将视景体变换为(-1,-1,0)(1,1,1)定义的长方体) 使用它会使得点(3D)不标准
Matrix GetMatrixP(float fovy, float aspect, float z_near, float z_far);

Matrix GetMatrixE(float psi, float theta, float phi);

Matrix GetMatrixE(EulerAngles euler_angles);

Matrix GetInverseMatrixE(EulerAngles euler_angles);

// 结构 四元数
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

	Quaternion();   // 默认 w = 1 
	Quaternion(float value);   // 用于置0
	Quaternion(float _x, float _y, float _z, float _w);

	// access date
	float& operator()(int index);   // 调用前做数组越界检查！
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

// 结构 欧拉角
struct EulerAngles
{
public:
	float psi;
	float theta;
	float phi;

	EulerAngles();
	EulerAngles(float _psi, float _theta, float _phi);
};

// 结构 2D点
struct Point2D
{
public:
	int x;
	int y;

	Point2D();
	Point2D(int _x, int _y);
};


// 限制x在min到max之间
int MiddleValue(int x, int min, int max);
float MiddleValue(float x, float min, float max);

// 插值计算
template <class T>
T Interp(T x1, T x2, float t);

// 矢量长度
float VectorLength(Vector v1);

// 矢量点积
float VectorDot(Vector v1, Vector v2);

// 矢量叉积(w设为一)
Vector VectorCross(Vector v1, Vector v2);

// 矢量插值(w设为一)
Vector VectorInterp(Vector v1, Vector v2, float t);

// 矢量单位化(w设为一)
Vector VectorUnit(Vector v1);

// w归一(通过缩放)
Point PointStandard(Point p1);

// 计算两点间距离
float PointDistance(Point p1, Point p2);

// 计算三角形单位法向量
Vector VectorNormal(Point p1, Point p2, Point p3);




