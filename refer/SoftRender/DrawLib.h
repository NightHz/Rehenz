// DrawLib.h
//
// By NightHz
//
// The file defines: 
//		some functions which draw 32-bit color to buffer
// About return value:
//		(bool)return true if success, or return false
// 

#pragma once

typedef unsigned char UCHAR;
typedef unsigned int  UINT;



// �������ص�
// �������ص� �汾1 �������λ�� mempitchΪ�м��ڴ�ƫ����(byte)
bool DrawPixel(UCHAR * buffer, int x, int y, int mempitch, UINT color);
// �������ص� �汾2 �����λ�� mempitchΪ�м��ڴ�ƫ����(byte)
bool DrawPixel(UCHAR * buffer, int x, int y, int mempitch, int width,int height, UINT color);
// �������ص� �汾3 �������λ�� ������ÿ��ͼ�����ݼ��޷�϶��ͼ�񻺴�
bool DrawPixel(UINT * image, int x, int y, int width, UINT color);
// �������ص� �汾4 �����λ�� ������ÿ��ͼ�����ݼ��޷�϶��ͼ�񻺴�
bool DrawPixel(UINT * image, int x, int y, int width, int height, UINT color);

// �����߶� ���߶�ȫ��������ʱ����false  ���й���: x->[x_min, x_max)  y->[y_min, y_max)
bool ClipLine(int &x1, int &y1, int &x2, int &y2, int x_min, int y_min, int x_max, int y_max);

// �����߶�
// �����߶� �汾1 �������λ�� mempitchΪ�м��ڴ�ƫ����(byte)
bool DrawLine(UCHAR * buffer, int x0, int y0, int x1, int y1, int mempitch, UINT color);
// �����߶� �汾2 �����λ�� mempitchΪ�м��ڴ�ƫ����(byte)
bool DrawLine(UCHAR * buffer, int x0, int y0, int x1, int y1, int mempitch, int width, int height, UINT color);
// �����߶� �汾3 �������λ�� ������ÿ��ͼ�����ݼ��޷�϶��ͼ�񻺴�
bool DrawLine(UINT * image, int x0, int y0, int x1, int y1, int width, UINT color);
// �����߶� �汾4 �����λ�� ������ÿ��ͼ�����ݼ��޷�϶��ͼ�񻺴�
bool DrawLine(UINT * image, int x0, int y0, int x1, int y1, int width, int height, UINT color);

// ���в������߶�
// ���в������߶� �汾1 mempitchΪ�м��ڴ�ƫ����(byte)
bool DrawClipLine(UCHAR * buffer, int x1, int y1, int x2, int y2, int mempitch, int width, int height, UINT color);
// ���в������߶� �汾2 ������ÿ��ͼ�����ݼ��޷�϶��ͼ�񻺴�
bool DrawClipLine(UINT * image, int x1, int y1, int x2, int y2, int width, int height, UINT color);


void DrawTriangleTop(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color1, UINT color2, UINT color3);
void DrawTriangleBottom(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color1, UINT color2, UINT color3);
void DrawTriangle(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color1, UINT color2, UINT color3);

