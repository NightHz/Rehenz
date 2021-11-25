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



// 绘制像素点
// 绘制像素点 版本1 不检查点的位置 mempitch为行间内存偏移量(byte)
bool DrawPixel(UCHAR * buffer, int x, int y, int mempitch, UINT color);
// 绘制像素点 版本2 检查点的位置 mempitch为行间内存偏移量(byte)
bool DrawPixel(UCHAR * buffer, int x, int y, int mempitch, int width,int height, UINT color);
// 绘制像素点 版本3 不检查点的位置 适用于每行图像数据间无缝隙的图像缓存
bool DrawPixel(UINT * image, int x, int y, int width, UINT color);
// 绘制像素点 版本4 检查点的位置 适用于每行图像数据间无缝隙的图像缓存
bool DrawPixel(UINT * image, int x, int y, int width, int height, UINT color);

// 裁切线段 该线段全部被裁切时返回false  裁切规则: x->[x_min, x_max)  y->[y_min, y_max)
bool ClipLine(int &x1, int &y1, int &x2, int &y2, int x_min, int y_min, int x_max, int y_max);

// 绘制线段
// 绘制线段 版本1 不检查点的位置 mempitch为行间内存偏移量(byte)
bool DrawLine(UCHAR * buffer, int x0, int y0, int x1, int y1, int mempitch, UINT color);
// 绘制线段 版本2 检查点的位置 mempitch为行间内存偏移量(byte)
bool DrawLine(UCHAR * buffer, int x0, int y0, int x1, int y1, int mempitch, int width, int height, UINT color);
// 绘制线段 版本3 不检查点的位置 适用于每行图像数据间无缝隙的图像缓存
bool DrawLine(UINT * image, int x0, int y0, int x1, int y1, int width, UINT color);
// 绘制线段 版本4 检查点的位置 适用于每行图像数据间无缝隙的图像缓存
bool DrawLine(UINT * image, int x0, int y0, int x1, int y1, int width, int height, UINT color);

// 裁切并绘制线段
// 裁切并绘制线段 版本1 mempitch为行间内存偏移量(byte)
bool DrawClipLine(UCHAR * buffer, int x1, int y1, int x2, int y2, int mempitch, int width, int height, UINT color);
// 裁切并绘制线段 版本2 适用于每行图像数据间无缝隙的图像缓存
bool DrawClipLine(UINT * image, int x1, int y1, int x2, int y2, int width, int height, UINT color);


void DrawTriangleTop(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color1, UINT color2, UINT color3);
void DrawTriangleBottom(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color1, UINT color2, UINT color3);
void DrawTriangle(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color1, UINT color2, UINT color3);

