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

	// 4*4����
	struct Matrix;
	// ���� ����1*4���� ����任Ϊ�ҳ�
	struct Vector;
	// ��(3D)
	typedef Vector Point;
	// ŷ����
	struct EulerAngles;
	// 2D��
	struct Point2D;


	// 4*4����
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

	// �õ�ƽ�ƾ���
	Matrix GetMatrixT(float tx, float ty, float tz);

	// �õ�ƽ�ƾ���������
	Matrix GetInverseMatrixT(float tx, float ty, float tz);

	// �õ�ƽ�ƾ���
	Matrix GetMatrixT(Vector translation);

	// �õ�ƽ�ƾ���������
	Matrix GetInverseMatrixT(Vector translation);

	// �õ���ת����(ͨ����������(ԭ+z��at)����������(ԭ+y��up)������ת) up��at���Բ���ֱ
	Matrix GetMatrixR(Vector at, Vector up);

	// �õ���ת����������(ͨ����������(ԭ+z��at)����������(ԭ+y��up)������ת) up��at���Բ���ֱ
	Matrix GetInverseMatrixR(Vector at, Vector up);

	Matrix GetMatrixRx(float theta);

	Matrix GetMatrixRy(float theta);

	Matrix GetMatrixRz(float theta);

	// �õ����ž���
	Matrix GetMatrixS(float scaleX, float scaleY , float scaleZ);

	// �õ����ž���������
	Matrix GetInverseMatrixS(float scaleX, float scaleY, float scaleZ);

	// �õ����ž���
	Matrix GetMatrixS(float scaleXYZ);

	// �õ����ž���������
	Matrix GetInverseMatrixS(float scaleXYZ);

	// �õ����ž���
	Matrix GetMatrixS(Vector scale);

	// �õ����ž���������
	Matrix GetInverseMatrixS(Vector scale);

	// �õ�ͶӰ����(���Ӿ���任Ϊ(-1,-1,0)(1,1,1)����ĳ�����) ʹ������ʹ�õ�(3D)����׼
	Matrix GetMatrixP(float fovy, float aspect, float z_near, float z_far);

	// �õ�ŷ������ת����
	Matrix GetMatrixE(float psi, float theta, float phi);

	// �õ�ŷ������ת����
	Matrix GetMatrixE(EulerAngles euler_angles);

	// �õ�ŷ������ת����������
	Matrix GetInverseMatrixE(EulerAngles euler_angles);

	// ���� 1*4����
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

		Vector();   // Ĭ�� w = 1 
		Vector(float value);   // ������0
		Vector(float _x, float _y, float _z, float _w);

		// access date
		float& operator()(int index);   // ����ǰ������Խ���飡
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

	// ŷ����
	struct EulerAngles
	{
	public:
		float psi;
		float theta;
		float phi;

		EulerAngles();
		EulerAngles(float _psi, float _theta, float _phi);
	};

	// 2D��
	struct Point2D
	{
	public:
		int x;
		int y;

		Point2D();
		Point2D(int _x, int _y);
	};


	// ����x��min��max֮��
	template <typename T>
	inline T Clamp(T x, T min, T max)
	{
		return (x < min) ? min : ((x > max) ? max : x);
	}

	// ���Բ�ֵ
	template <typename T>
	inline T Lerp(T x1, T x2, float t)
	{
		return x1 + (T)((x2 - x1)*t);
	}

	// ������ƽ����ֵ
	template <typename T>
	inline T Fade(T x1, T x2, float t)
	{
		t = t * t * t * (10 + t * (-15 + t * 6)); // 6*t^5 - 15*t^4 + 10*t^3
		return x1 + (T)((x2 - x1)*t);
	}

	// ʸ������
	float VectorLength(Vector v1);

	// ʸ�����
	float VectorDot(Vector v1, Vector v2);

	// ʸ�����(w��Ϊһ)
	Vector VectorCross(Vector v1, Vector v2);

	// ʸ����ֵ(w��Ϊһ)
	Vector VectorLerp(Vector v1, Vector v2, float t);

	// ʸ����λ��(w��Ϊһ)
	Vector VectorUnit(Vector v1);

	// w��һ(ͨ������)
	Point PointStandard(Point p1);

	// ������������
	float PointDistance(Point p1, Point p2);

	// ������������
	float PointDistance(Point2D p1, Point2D p2);

	// ���������ε�λ������
	Vector TrianglesNormal(Point p1, Point p2, Point p3);

}

#endif // !_HMATH_H_
