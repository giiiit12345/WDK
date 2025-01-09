#pragma once

#pragma once

#ifndef BASIC_SYMBOL_H
#define BASIC_SYMBOL_H
#endif

#include <wx/wx.h>
#include <cmath>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <cstdlib>
#include <wx/graphics.h>

//定义常用颜色
#define RED wxColor(132, 0, 0)
#define GREEN wxColor(0, 100, 100)
#define YELLOW wxColor(255, 255, 194)
#define WHITE wxColor(255, 255, 255)

//USB模块默认的长宽
#define USB_LENGTH 216
#define USB_WIDTH 144

//绘制直线，接受端点坐标,线条颜色
void DrawLine(wxMemoryDC& memDC, int x1, int y1, int x2, int y2, const wxColour& penColor);

//绘制三角形，接受三顶点坐标,颜色
void DrawTriangle(wxMemoryDC& memDC, int x1, int y1, int x2, int y2, int x3, int y3, const wxColour& borderColor);

//绘制矩形，接受对角顶点坐标,颜色
void DrawRectangle(wxMemoryDC& memDC, int x1, int y1, int x2, int y2, const wxColour& borderColor);

//绘制圆形，接受圆心坐标，半径,颜色
void DrawCircle(wxMemoryDC& memDC, int x, int y, int radius, const wxColour& borderColor);

//绘制箭头，接受起始点坐标,颜色
void DrawArrow(wxMemoryDC& memDC, int x1, int y1, int x2, int y2, const wxColour& borderColor);

//绘制文本，接受文本坐标，颜色，大小
void DrawText(wxMemoryDC& memDC, const char* txt, int x, int y, const wxColour& color);

// 绘制半圆，接受圆心坐标、半径、颜色、左半部分还是右半部分
void DrawHalfCircle(wxMemoryDC& memDC, int x, int y, int radiusX, const wxColour& borderColor, bool drawLeftHalf);

//绘制半椭圆，接受圆心坐标、半径、颜色、左半部分还是右半部分
void DrawHalfEllipse(wxMemoryDC& memDC, int x, int y, int radiusX, int radiusY, const wxColour& borderColor, bool drawLeftHalf);

class Symbol
{
};
