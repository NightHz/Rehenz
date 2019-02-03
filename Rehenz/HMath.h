// HMath.h
//
// By NightHz
//
// Math library
//
// The file defines: 
//		struct Matrix 
//		funcions which can get some special Matrix
//		struct Vector (Point)
//		struct EulerAngles
//		struct Point2D
//		more functions
//      PerlinNoise generator
//

#ifndef _HMATH_H_
#define _HMATH_H

namespace Rehenz
{
	const float pi = 3.14159265f;

	// 4*4矩阵
	struct Matrix;
	// 向量 视作1*4矩阵 矩阵变换为右乘
	struct Vector;
	// 点(3D)
	typedef Vector Point;
	// 欧拉角
	struct EulerAngles;
	// 2D点
	struct Point2D;


	// 4*4矩阵
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

		// some operator
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
		Matrix operator/(float);

		bool operator==(Matrix);
		bool operator!=(Matrix);
	};
	Matrix operator*(float, Matrix);

	// 得到平移矩阵
	Matrix GetMatrixT(float tx, float ty, float tz);

	// 得到平移矩阵的逆矩阵
	Matrix GetInverseMatrixT(float tx, float ty, float tz);

	// 得到平移矩阵
	Matrix GetMatrixT(Vector translation);

	// 得到平移矩阵的逆矩阵
	Matrix GetInverseMatrixT(Vector translation);

	// 得到旋转矩阵(通过朝向向量(原+z现at)与向上向量(原+y现up)定义旋转) up与at可以不垂直
	Matrix GetMatrixR(Vector at, Vector up);

	// 得到旋转矩阵的逆矩阵(通过朝向向量(原+z现at)与向上向量(原+y现up)定义旋转) up与at可以不垂直
	Matrix GetInverseMatrixR(Vector at, Vector up);

	Matrix GetMatrixRx(float theta);

	Matrix GetMatrixRy(float theta);

	Matrix GetMatrixRz(float theta);

	// 得到缩放矩阵
	Matrix GetMatrixS(float scaleX, float scaleY , float scaleZ);

	// 得到缩放矩阵的逆矩阵
	Matrix GetInverseMatrixS(float scaleX, float scaleY, float scaleZ);

	// 得到缩放矩阵
	Matrix GetMatrixS(float scaleXYZ);

	// 得到缩放矩阵的逆矩阵
	Matrix GetInverseMatrixS(float scaleXYZ);

	// 得到缩放矩阵
	Matrix GetMatrixS(Vector scale);

	// 得到缩放矩阵的逆矩阵
	Matrix GetInverseMatrixS(Vector scale);

	// 得到投影矩阵(将视景体变换为(-1,-1,0)(1,1,1)定义的长方体) 使用它会使得点(3D)不标准
	Matrix GetMatrixP(float fovy, float aspect, float z_near, float z_far);

	// 得到欧拉角旋转矩阵
	Matrix GetMatrixE(float psi, float theta, float phi);

	// 得到欧拉角旋转矩阵
	Matrix GetMatrixE(EulerAngles euler_angles);

	// 得到欧拉角旋转矩阵的逆矩阵
	Matrix GetInverseMatrixE(EulerAngles euler_angles);

	// 向量 1*4矩阵
	struct Vector
	{
	public:
		union {
			float v[4];
			struct {
				float x;
				float y;
				float z;
				float w;
			};
		};

		Vector();   // 默认 w = 1 
		Vector(float value);   // 用于置0
		Vector(float _x, float _y, float _z, float _w);

		// access date
		float& operator()(int index);   // 调用前做数组越界检查！
		Vector& operator=(Vector);

		// some operator
		Vector& operator*=(Matrix);
		Vector& operator+=(Vector);
		Vector& operator-=(Vector);
		Vector& operator*=(float);
		Vector& operator/=(float);

		Vector operator-();

		Vector operator*(Matrix);
		Vector operator+(Vector);
		Vector operator-(Vector);
		Vector operator*(float);
		Vector operator/(float);

		bool operator==(Vector);
		bool operator!=(Vector);
	};
	Vector operator*(float, Vector);

	// 欧拉角
	struct EulerAngles
	{
	public:
		float psi;
		float theta;
		float phi;

		EulerAngles();
		EulerAngles(float _psi, float _theta, float _phi);
	};

	// 2D点
	struct Point2D
	{
	public:
		int x;
		int y;

		Point2D();
		Point2D(int _x, int _y);
	};


	// 限制x在min到max之间
	template <typename T>
	inline T Clamp(T x, T min, T max)
	{
		return (x < min) ? min : ((x > max) ? max : x);
	}

	// 线性插值
	template <typename T>
	inline T Lerp(T x1, T x2, float t)
	{
		return x1 + (T)((x2 - x1)*t);
	}

	// 非线性平滑插值
	template <typename T>
	inline T Fade(T x1, T x2, float t)
	{
		t = t * t * t * (10 + t * (-15 + t * 6)); // 6*t^5 - 15*t^4 + 10*t^3
		return x1 + (T)((x2 - x1)*t);
	}

	// 矢量长度
	float VectorLength(Vector v1);

	// 矢量点积
	float VectorDot(Vector v1, Vector v2);

	// 矢量叉积(w设为一)
	Vector VectorCross(Vector v1, Vector v2);

	// 矢量插值(w设为一)
	Vector VectorLerp(Vector v1, Vector v2, float t);

	// 矢量单位化(w设为一)
	Vector VectorUnit(Vector v1);

	// w归一(通过缩放)
	Point PointStandard(Point p1);

	// 计算两点间距离
	float PointDistance(Point p1, Point p2);

	// 计算两点间距离
	float PointDistance(Point2D p1, Point2D p2);

	// 计算三角形单位法向量
	Vector TrianglesNormal(Point p1, Point p2, Point p3);

}

#endif // !_HMATH_H_
