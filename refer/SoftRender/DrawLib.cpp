// DrawLib.cpp
//
// By NightHz
//

#include "DrawLib.h"



bool DrawPixel(UCHAR * buffer, int x, int y, int mempitch, UINT color)
{
	buffer += x*(int)sizeof(color) + y*mempitch;
	*(UINT*)buffer = color;
	return true;
}

bool DrawPixel(UCHAR * buffer, int x, int y, int mempitch, int width, int height, UINT color)
{
	if (((UINT)x) < (UINT)width && ((UINT)y) < (UINT)height)
	{
		buffer += x*(int)sizeof(color) + y*mempitch;
		*(UINT*)buffer = color;
	}
	return true;
}

bool DrawPixel(UINT * image, int x, int y, int width, UINT color)
{
	image += x + y*width;
	*image = color;
	return true;
}

bool DrawPixel(UINT * image, int x, int y, int width, int height, UINT color)
{
	if (((UINT)x) < (UINT)width && ((UINT)y) < (UINT)height)
	{
		image += x + y*width;
		*image = color;
	}
	return true;
}

bool ClipLine(int &x1, int &y1, int &x2, int &y2, int x_min,int y_min, int x_max, int y_max)
{
	// Cohen_Sutherland
	// 区域编码
	// 0101(5) 0100(4) 0110(6)
	// 0001(1) 0000(0) 0010(2)
	// 1001(9) 1000(8) 1010(a)
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0004
#define CLIP_CODE_S  0x0008
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001

#define CLIP_CODE_NE 0x0006
#define CLIP_CODE_SE 0x000a
#define CLIP_CODE_NW 0x0005 
#define CLIP_CODE_SW 0x0009

	int p1_code = 0, p2_code = 0;

	if (x1 < x_min)
		p1_code |= CLIP_CODE_W;
	else if (x1 >= x_max)
		p1_code |= CLIP_CODE_E;
	if (y1 < y_min)
		p1_code |= CLIP_CODE_N;
	else if (y1 >= y_max)
		p1_code |= CLIP_CODE_S;

	if (x2 < x_min)
		p2_code |= CLIP_CODE_W;
	else if (x2 >= x_max)
		p2_code |= CLIP_CODE_E;
	if (y2 < y_min)
		p2_code |= CLIP_CODE_N;
	else if (y2 >= y_max)
		p2_code |= CLIP_CODE_S;

	if (p1_code == 0 && p2_code == 0)   // 线段完全在内部
		return true;
	if ((p1_code&p2_code) != 0)   // 线段完全不在内部的一种情况
		return false;

	int xc1 = x1, yc1 = y1, 
		xc2 = x2, yc2 = y2;
	// 裁切p1
	switch (p1_code)
	{
	case CLIP_CODE_C:
		break;

	case CLIP_CODE_N:
		yc1 = y_min;
		xc1 = (int)(x1 + 0.5 + (yc1 - y1)*(x2 - x1) / (y2 - y1));
		break;
	case CLIP_CODE_S:
		yc1 = y_max - 1;
		xc1 = (int)(x1 + 0.5 + (yc1 - y1)*(x2 - x1) / (y2 - y1));
		break;
	case CLIP_CODE_W:
		xc1 = x_min;
		yc1 = (int)(y1 + 0.5 + (xc1 - x1)*(y2 - y1) / (x2 - x1));
		break;
	case CLIP_CODE_E:
		xc1 = x_max - 1;
		yc1 = (int)(y1 + 0.5 + (xc1 - x1)*(y2 - y1) / (x2 - x1));
		break;

	case CLIP_CODE_NE:
		// 按N裁切
		yc1 = y_min;
		xc1 = (int)(x1 + 0.5 + (yc1 - y1)*(x2 - x1) / (y2 - y1));

		if (xc1 < x_min || xc1 >= x_max)
		{
			// 按E裁切
			xc1 = x_max - 1;
			yc1 = (int)(y1 + 0.5 + (xc1 - x1)*(y2 - y1) / (x2 - x1));
		}
		break;

	case CLIP_CODE_SE:
		// 按S裁切
		yc1 = y_max - 1;
		xc1 = (int)(x1 + 0.5 + (yc1 - y1)*(x2 - x1) / (y2 - y1));

		if (xc1 < x_min || xc1 >= x_max)
		{
			// 按E裁切
			xc1 = x_max - 1;
			yc1 = (int)(y1 + 0.5 + (xc1 - x1)*(y2 - y1) / (x2 - x1));
		}
		break;

	case CLIP_CODE_NW:
		// 按N裁切
		yc1 = y_min;
		xc1 = (int)(x1 + 0.5 + (yc1 - y1)*(x2 - x1) / (y2 - y1));

		if (xc1 < x_min || xc1 >= x_max)
		{
			// 按W裁切
			xc1 = x_min;
			yc1 = (int)(y1 + 0.5 + (xc1 - x1)*(y2 - y1) / (x2 - x1));
		}
		break;

	case CLIP_CODE_SW:
		// 按S裁切
		yc1 = y_max - 1;
		xc1 = (int)(x1 + 0.5 + (yc1 - y1)*(x2 - x1) / (y2 - y1));

		if (xc1 < x_min || xc1 >= x_max)
		{
			// 按W裁切
			xc1 = x_min;
			yc1 = (int)(y1 + 0.5 + (xc1 - x1)*(y2 - y1) / (x2 - x1));
		}
		break;
	}

	// 裁切p2
	switch (p2_code)
	{
	case CLIP_CODE_C:
		break;

	case CLIP_CODE_N:
		yc2 = y_min;
		xc2 = (int)(x2 + (yc2 - y2)*(x1 - x2) / (y1 - y2));
		break;
	case CLIP_CODE_S:
		yc2 = y_max - 1;
		xc2 = (int)(x2 + (yc2 - y2)*(x1 - x2) / (y1 - y2));
		break;
	case CLIP_CODE_W:
		xc2 = x_min;
		yc2 = (int)(y2 + (xc2 - x2)*(y1 - y2) / (x1 - x2));
		break;
	case CLIP_CODE_E:
		xc2 = x_max - 1;
		yc2 = (int)(y2 + (xc2 - x2)*(y1 - y2) / (x1 - x2));
		break;

	case CLIP_CODE_NE:
		// 按N裁切
		yc2 = y_min;
		xc2 = (int)(x2 + 0.5 + (yc2 - y2)*(x1 - x2) / (y1 - y2));

		if (xc2 < x_min || xc2 >= x_max)
		{// 按E裁切
			xc2 = x_max - 1;
			yc2 = (int)(y2 + 0.5 + (xc2 - x2)*(y1 - y2) / (x1 - x2));
		}
		break;

	case CLIP_CODE_SE:
		// 按S裁切
		yc2 = y_max - 1;
		xc2 = (int)(x2 + 0.5 + (yc2 - y2)*(x1 - x2) / (y1 - y2));

		if (xc2 < x_min || xc2 >= x_max)
		{
			// 按E裁切
			xc2 = x_max - 1;
			yc2 = (int)(y2 + 0.5 + (xc2 - x2)*(y1 - y2) / (x1 - x2));
		}
		break;

	case CLIP_CODE_NW:
		// 按N裁切
		yc2 = y_min;
		xc2 = (int)(x2 + 0.5 + (yc2 - y2)*(x1 - x2) / (y1 - y2));

		if (xc2 < x_min || xc2 >= x_max)
		{
			// 按W裁切
			xc2 = x_min;
			yc2 = (int)(y2 + 0.5 + (xc2 - x2)*(y1 - y2) / (x1 - x2));
		}
		break;

	case CLIP_CODE_SW:
		// 按S裁切
		yc2 = y_max - 1;
		xc2 = (int)(x2 + 0.5 + (yc2 - y2)*(x1 - x2) / (y1 - y2));

		if (xc2 < x_min || xc2 >= x_max)
		{
			// 按W裁切
			xc2 = x_min;
			yc2 = (int)(y2 + 0.5 + (xc2 - x2)*(y1 - y2) / (x1 - x2));
		}
		break;
	}

	// 切点测试: 裁切后是否在内部
	if ((xc1 < x_min) || (xc1 >= x_max) ||
		(yc1 < y_min) || (yc1 >= y_max) ||
		(xc2 < x_min) || (xc2 >= x_max) ||
		(yc2 < y_min) || (yc2 >= y_max))
		return false;

	x1 = xc1;
	y1 = yc1;
	x2 = xc2;
	y2 = yc2;

	return true;
}

bool DrawLine(UCHAR * buffer, int x0, int y0, int x1, int y1, int mempitch, UINT color)
{
	// based on Bresenham

	int dx, dy, dx2, dy2;   // x(y)的差值及两倍差值
	int x_inc, y_inc;   // 指针的单次移动距离
	int next;   // 用于判定是否移动至下一行(列)
	int i;

	// 移动至第一个点
	buffer += x0*(int)sizeof(color) + y0*mempitch;

	// 计算投影在 x y 上的长度
	dx = x1 - x0;
	dy = y1 - y0;

	// 投影长度取正 设置next判定的内存移动距离
	if (dx >= 0)
		x_inc = (int)sizeof(color);
	else
		x_inc = -(int)sizeof(color), dx = -dx;
	if (dy >= 0)
		y_inc = mempitch;
	else
		y_inc = -mempitch, dy = -dy;

	// 保存两倍长度
	dx2 = dx * 2;
	dy2 = dy * 2;

	if (dx > dy)
	{
		// 此为遍历 x 型
		next = -dx;
		for (i = 0; i <= dx; i++)
		{
			// 绘制
			*(UINT*)buffer = color;

			next += dy2;
			if (next >= 0)
			{
				// 移动绘制指针至下一行
				buffer += y_inc;
				next -= dx2;
			}
			// 移动绘制指针至下一像素
			buffer += x_inc;
		}
	}
	else
	{
		// 此为遍历 y 型
		next = -dy;
		for (i = 0; i <= dy; i++)
		{
			// 绘制
			*(UINT*)buffer = color;

			next += dx2;
			if (next >= 0)
			{
				// 移动绘制指针至下一列
				buffer += x_inc;
				next -= dy2;
			}
			// 移动绘制指针至下一像素
			buffer += y_inc;
		}
	}

	return true;
}

bool DrawLine(UCHAR * buffer, int x0, int y0, int x1, int y1, int mempitch, int width, int height, UINT color)
{
	// based on Bresenham

	int x, y;   // 当前指针指向的位置(当前目标位置)
	int x_move, y_move;   // x(y)的单次移动距离
	int dx, dy, dx2, dy2;   // x(y)的差值及两倍差值
	int x_inc, y_inc;   // 指针的单次移动距离
	int next;   // 用于判定是否移动至下一行(列)
	int i;

	// 移动至第一个点
	x = x0;
	y = y0;
	buffer += x0*(int)sizeof(color) + y0*mempitch;

	// 计算投影在 x y 上的长度
	dx = x1 - x0;
	dy = y1 - y0;

	// 投影长度取正 设置next判定的内存移动距离
	if (dx >= 0)
		x_inc = (int)sizeof(color), x_move = 1;
	else
		x_inc = -(int)sizeof(color), x_move = -1, dx = -dx;
	if (dy >= 0)
		y_inc = mempitch, y_move = 1;
	else
		y_inc = -mempitch, y_move = -1, dy = -dy;

	// 保存两倍长度
	dx2 = dx * 2;
	dy2 = dy * 2;

	if (dx > dy)
	{
		// 此为遍历 x 型
		next = -dx;
		for (i = 0; i <= dx; i++)
		{
			// 绘制
			if (((UINT)x) < (UINT)width && ((UINT)y) < (UINT)height)
				*(UINT*)buffer = color;

			next += dy2;
			if (next >= 0)
			{
				// 移动绘制指针至下一行
				y += y_move;
				buffer += y_inc;
				next -= dx2;
			}
			// 移动绘制指针至下一像素
			x += x_move;
			buffer += x_inc;
		}
	}
	else
	{
		// 此为遍历 y 型
		next = -dy;
		for (i = 0; i <= dy; i++)
		{
			// 绘制
			if (((UINT)x) < (UINT)width && ((UINT)y) < (UINT)height)
				*(UINT*)buffer = color;

			next += dx2;
			if (next >= 0)
			{
				// 移动绘制指针至下一列
				x += x_move;
				buffer += x_inc;
				next -= dy2;
			}
			// 移动绘制指针至下一像素
			y += y_move;
			buffer += y_inc;
		}
	}

	return true;
}

bool DrawLine(UINT * image, int x0, int y0, int x1, int y1, int width, UINT color)
{
	// based on Bresenham

	int dx, dy, dx2, dy2;   // x(y)的差值及两倍差值
	int x_inc, y_inc;   // 指针的单次移动距离
	int next;   // 用于判定是否移动至下一行(列)
	int i;

	// 移动至第一个点
	image += x0 + y0*width;

	// 计算投影在 x y 上的长度
	dx = x1 - x0;
	dy = y1 - y0;

	// 投影长度取正 设置next判定的内存移动距离
	if (dx >= 0)
		x_inc = 1;
	else
		x_inc = -1, dx = -dx;
	if (dy >= 0)
		y_inc = width;
	else
		y_inc = -width, dy = -dy;

	// 保存两倍长度
	dx2 = dx * 2;
	dy2 = dy * 2;

	if (dx > dy)
	{
		// 此为遍历 x 型
		next = -dx;
		for (i = 0; i <= dx; i++)
		{
			// 绘制
			*image = color;

			next += dy2;
			if (next >= 0)
			{
				// 移动绘制指针至下一行
				image += y_inc;
				next -= dx2;
			}
			// 移动绘制指针至下一像素
			image += x_inc;
		}
	}
	else
	{
		// 此为遍历 y 型
		next = -dy;
		for (i = 0; i <= dy; i++)
		{
			// 绘制
			*image = color;

			next += dx2;
			if (next >= 0)
			{
				// 移动绘制指针至下一列
				image += x_inc;
				next -= dy2;
			}
			// 移动绘制指针至下一像素
			image += y_inc;
		}
	}

	return true;
}

bool DrawLine(UINT * image, int x0, int y0, int x1, int y1, int width, int height, UINT color)
{
	// based on Bresenham

	int x, y;   // 当前指针指向的位置(当前目标位置)
	int x_move, y_move;   // x(y)的单次移动距离
	int dx, dy, dx2, dy2;   // x(y)的差值及两倍差值
	int x_inc, y_inc;   // 指针的单次移动距离
	int next;   // 用于判定是否移动至下一行(列)
	int i;

	// 移动至第一个点
	x = x0;
	y = y0;
	image += x0 + y0*width;

	// 计算投影在 x y 上的长度
	dx = x1 - x0;
	dy = y1 - y0;

	// 投影长度取正 设置next判定的内存移动距离
	if (dx >= 0)
		x_inc = 1, x_move = 1;
	else
		x_inc = -1, x_move = -1, dx = -dx;
	if (dy >= 0)
		y_inc = width, y_move = 1;
	else
		y_inc = -width, y_move = -1, dy = -dy;

	// 保存两倍长度
	dx2 = dx * 2;
	dy2 = dy * 2;

	if (dx > dy)
	{
		// 此为遍历 x 型
		next = -dx;
		for (i = 0; i <= dx; i++)
		{
			// 绘制
			if (((UINT)x) < (UINT)width && ((UINT)y) < (UINT)height)
				*image = color;

			next += dy2;
			if (next >= 0)
			{
				// 移动绘制指针至下一行
				y += y_move;
				image += y_inc;
				next -= dx2;
			}
			// 移动绘制指针至下一像素
			x += x_move;
			image += x_inc;
		}
	}
	else
	{
		// 此为遍历 y 型
		next = -dy;
		for (i = 0; i <= dy; i++)
		{
			// 绘制
			if (((UINT)x) < (UINT)width && ((UINT)y) < (UINT)height)
				*image = color;

			next += dx2;
			if (next >= 0)
			{
				// 移动绘制指针至下一列
				x += x_move;
				image += x_inc;
				next -= dy2;
			}
			// 移动绘制指针至下一像素
			y += y_move;
			image += y_inc;
		}
	}

	return true;
}

bool DrawClipLine(UCHAR * buffer, int x1, int y1, int x2, int y2, int mempitch, int width, int height, UINT color)
{
	if (ClipLine(x1, y1, x2, y2, 0, 0, width, height))
		return DrawLine(buffer, x1, y1, x2, y2, mempitch, color);
	else
		return true;
}

bool DrawClipLine(UINT * image, int x1, int y1, int x2, int y2, int width, int height, UINT color)
{
	if (ClipLine(x1, y1, x2, y2, 0, 0, width, height))
		return DrawLine(image, x1, y1, x2, y2, width, color);
	else
		return true;
}

void DrawTriangleTop(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color1, UINT color2, UINT color3)
{
	int x, y;
	UINT color;

	// x1<=x2
	if (x1 > x2)
	{
		x = x1;
		x1 = x2;
		x2 = x;
	}

	int x_left, x_right;
	UINT c_left, c_right;
	for (y=y3; y<=y1; y++)
	{
		if (y1 == y3)
		{
			x_left = x3;
			c_left = color3;
			x_right = x3;
			c_right = color3;
		}
		else
		{
			x_left = (x1 - x3)*(y - y3) / (y1 - y3) + x3;
			c_left = (color1 - color3)*(y - y3) / (y1 - y3) + color3;
			x_right = (x2 - x3)*(y - y3) / (y1 - y3) + x3;
			c_right = (color2 - color3)*(y - y3) / (y1 - y3) + color3;
		}
		for (x = x_left; x <= x_right;x++)
		{
			if (x_right == x_left)
				color = c_left;
			else
				color = (c_right - c_left)*(x - x_left) / (x_right - x_left) + c_left;
			DrawPixel(image, x, y, width, color);
		}
	}

}

void DrawTriangleBottom(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color1, UINT color2, UINT color3)
{
	int x, y;
	UINT color;

	// x1<=x2
	if (x1 > x2)
	{
		x = x1;
		x1 = x2;
		x2 = x;
	}

	int x_left, x_right;
	UINT c_left, c_right;
	for (y = y1; y <= y3; y++)
	{
		if (y3 == y1)
		{
			x_left = x1;
			x_right = x2;
			c_left = color1;
			c_right = color2;
		}
		else
		{
			x_left = (x3 - x1)*(y - y1) / (y3 - y1) + x1;
			x_right = (x3 - x2)*(y - y1) / (y3 - y1) + x2;
			c_left = (color3 - color1)*(y - y1) / (y3 - y1) + color1;
			c_right = (color3 - color2)*(y - y1) / (y3 - y1) + color2;
		}
		for (x = x_left; x <= x_right;x++)
		{
			if (x_right == x_left)
				color = c_left;
			else
				color = (c_right - c_left)*(x - x_left) / (x_right - x_left) + c_left;
			DrawPixel(image, x, y, width, color);
		}
	}

}

void DrawTriangle(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color1, UINT color2, UINT color3)
{
	int x0,y0;

	// y1<=y2<=y3
	if (y1 > y2)
	{
		x0 = x2, y0 = y2;
		x2 = x1, y2 = y1;
		x1 = x0, y1 = y0;
	}
	if (y2 > y3)
	{
		x0 = x2, y0 = y2;
		x2 = x3, y2 = y3;
		x3 = x0, y3 = y0;
	}
	if (y1 > y2)
	{
		x0 = x2, y0 = y2;
		x2 = x1, y2 = y1;
		x1 = x0, y1 = y0;
	}

	if (y2 == y3)
		DrawTriangleTop(image, x2, y2, x3, y3, x1, y1, width, color1, color2, color3);
	else if (y1 == y2)
		DrawTriangleBottom(image, x1, y1, x2, y2, x3, y3, width, color1, color2, color3);
	else
	{
		x0 = (x3 - x1)*(y2 - y1) / (y3 - y1) + x1;
		y0 = y2;
		UINT color0 = (color3 - color1)*(y2 - y1) / (y3 - y1) + color1;
		DrawTriangleTop(image, x2, y2, x0, y0, x1, y1, width, color1, color2, color3);
		DrawTriangleBottom(image, x2, y2, x0, y0, x3, y3, width, color1, color2, color3);
	}

}




/*
//#define CLIP
void DrawTriangleTop(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color)
{
	// this function draws a triangle that has a flat top

	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs, xe,       // the starting and ending points of the edges
		height;      // the height of the triangle

	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;

	// destination address of next scanline
	UINT  *dest_addr = nullptr;

	// test order of x1 and x2
	if (x2 < x1)
	{
		temp_x = x2;
		x2 = x1;
		x1 = temp_x;
	} // end if swap

	  // compute delta's
	height = y3 - y1;

	dx_left = (x3 - x1) / height;
	dx_right = (x3 - x2) / height;

	// set starting points
	xs = (float)x1;
	xe = (float)x2 + (float)0.5;

#ifdef CLIP
	// perform y clipping
	if (y1 < min_clip_y)
	{
		// compute new xs and ys
		xs = xs + dx_left*(float)(-y1 + min_clip_y);
		xe = xe + dx_right*(float)(-y1 + min_clip_y);

		// reset y1
		y1 = min_clip_y;

	} // end if top is off screen

	if (y3>max_clip_y)
		y3 = max_clip_y;
#endif

	// compute starting address in video memory
	dest_addr = image + y1*width;

#ifdef CLIP
	// test if x clipping is needed
	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x)
#endif
	{
		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += width)
		{
			for(
			memset(dest_addr + (unsigned int)xs, color, (unsigned int)((int)xe - (int)xs + 1));

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

		} // end for

	} // end if no x clipping needed
#ifdef CLIP
	else
	{
		// clip x axis with slower version

		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch)
		{
			// do x clip
			left = (int)xs;
			right = (int)xe;

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

			// clip line
			if (left < min_clip_x)
			{
				left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x)
			{
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}

			memset((UCHAR  *)dest_addr + (unsigned int)left, color, (unsigned int)(right - left + 1));

		} // end for

	} // end else x clipping needed
#endif

}

void DrawTriangleBottom(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color)
{
	// this function draws a triangle that has a flat bottom

	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs, xe,       // the starting and ending points of the edges
		height;      // the height of the triangle

	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;

	// destination address of next scanline
	UCHAR  *dest_addr;

	// test order of x1 and x2
	if (x3 < x2)
	{
		temp_x = x2;
		x2 = x3;
		x3 = temp_x;
	} // end if swap

	  // compute delta's
	height = y3 - y1;

	dx_left = (x2 - x1) / height;
	dx_right = (x3 - x1) / height;

	// set starting points
	xs = (float)x1;
	xe = (float)x1; // +(float)0.5;

					// perform y clipping
	if (y1<min_clip_y)
	{
		// compute new xs and ys
		xs = xs + dx_left*(float)(-y1 + min_clip_y);
		xe = xe + dx_right*(float)(-y1 + min_clip_y);

		// reset y1
		y1 = min_clip_y;

	} // end if top is off screen

	if (y3>max_clip_y)
		y3 = max_clip_y;

	// compute starting address in video memory
	dest_addr = dest_buffer + y1*mempitch;

	// test if x clipping is needed
	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x)
	{
		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch)
		{
			memset((UCHAR  *)dest_addr + (unsigned int)xs, color, (unsigned int)((int)xe - (int)xs + 1));

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

		} // end for

	} // end if no x clipping needed
	else
	{
		// clip x axis with slower version

		// draw the triangle

		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch)
		{
			// do x clip
			left = (int)xs;
			right = (int)xe;

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

			// clip line
			if (left < min_clip_x)
			{
				left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x)
			{
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}

			memset((UCHAR  *)dest_addr + (unsigned int)left, color, (unsigned int)(right - left + 1));

		} // end for

	} // end else x clipping needed

}

void DrawTriangle(UINT * image, int x1, int y1, int x2, int y2, int x3, int y3, int width, UINT color)
{
	// this function draws a triangle on the destination buffer
	// it decomposes all triangles into a pair of flat top, flat bottom

	int temp_x, // used for sorting
		temp_y,
		new_x;


	// test for h lines and v lines
	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
		return;

	// sort p1,p2,p3 in ascending y order
	if (y2<y1)
	{
		temp_x = x2;
		temp_y = y2;
		x2 = x1;
		y2 = y1;
		x1 = temp_x;
		y1 = temp_y;
	} // end if

	  // now we know that p1 and p2 are in order
	if (y3<y1)
	{
		temp_x = x3;
		temp_y = y3;
		x3 = x1;
		y3 = y1;
		x1 = temp_x;
		y1 = temp_y;
	} // end if

	  // finally test y3 against y2
	if (y3<y2)
	{
		temp_x = x3;
		temp_y = y3;
		x3 = x2;
		y3 = y2;
		x2 = temp_x;
		y2 = temp_y;

	} // end if

#ifdef CLIP
	  // do trivial rejection tests for clipping
	if (y3<min_clip_y || y1>max_clip_y ||
		(x1<min_clip_x && x2<min_clip_x && x3<min_clip_x) ||
		(x1>max_clip_x && x2>max_clip_x && x3>max_clip_x))
		return;
#endif

	// test if top of triangle is flat
	if (y1 == y2)
	{
		DrawTriangleTop(image, x1, y1, x2, y2, x3, y3, width, color);
	} // end if
	else
		if (y2 == y3)
		{
			DrawTriangleBottom(image, x1, y1, x2, y2, x3, y3, width, color);
		} // end if bottom is flat
		else
		{
			// general triangle that's needs to be broken up along long edge
			new_x = x1 + (int)(0.5 + (float)(y2 - y1)*(float)(x3 - x1) / (float)(y3 - y1));

			// draw each sub-triangle
			DrawTriangleBottom(image, x1, y1, new_x, y2, x2, y2, width, color);
			DrawTriangleTop(image, x2, y2, new_x, y2, x3, y3, width, color);

		} // end else

}
//*/


