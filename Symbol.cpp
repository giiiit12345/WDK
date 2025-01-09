#include "Symbol.h"

//绘制直线,接受端点坐标,线条颜色
void DrawLine(wxMemoryDC& memDC, int x1, int y1, int x2, int y2, const wxColour& penColor)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc)
    {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);
        gc->SetPen(wxPen(penColor, 1));
        gc->StrokeLine(x1, y1, x2, y2);
        delete gc;
    }
}

//绘制三角形，接受三顶点坐标,颜色
void DrawTriangle(wxMemoryDC& memDC, int x1, int y1, int x2, int y2, int x3, int y3, const wxColour& borderColor)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc)
    {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);
        gc->SetPen(wxPen(borderColor));
        gc->StrokeLine(x1, y1, x2, y2);
        gc->StrokeLine(x1, y1, x3, y3);
        gc->StrokeLine(x3, y3, x2, y2);
        delete gc;
    }
}

//绘制矩形，接受对角顶点坐标,颜色
void DrawRectangle(wxMemoryDC& memDC, int x1, int y1, int x2, int y2, const wxColour& borderColor)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc)
    {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);
        gc->SetPen(wxPen(borderColor, 3));
        gc->DrawRectangle(x1, y1, x2 - x1, y2 - y1);
        delete gc;
    }
}

//绘制圆形，接受圆心坐标，半径,颜色
void DrawCircle(wxMemoryDC& memDC, int x, int y, int radius, const wxColour& borderColor)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc)
    {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);
        gc->SetPen(wxPen(borderColor, 1));
        int h = 2 * radius;
        gc->DrawEllipse(x-radius, y-radius, h, h);
        delete gc;
    }
}


//绘制箭头，接受起始点坐标,颜色
void DrawArrow(wxMemoryDC& memDC, int x1, int y1, int x2, int y2, const wxColour& borderColor)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc)
    {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);

        gc->SetPen(wxPen(borderColor, 1));

        // 绘制箭头的直线部分
        gc->StrokeLine(x1, y1, x2, y2);

        // 计算箭头的角度
        double angle = atan2(static_cast<double>(y2 - y1), static_cast<double>(x2 - x1));

        // 定义箭头的长度和宽度
        int arrowLength = 10;
        int arrowWidth = 5;

        // 绘制箭头的两个边
        int x3 = x2 - static_cast<int>(arrowLength * cos(angle - M_PI / 6));
        int y3 = y2 - static_cast<int>(arrowLength * sin(angle - M_PI / 6));
        int x4 = x2 - static_cast<int>(arrowLength * cos(angle + M_PI / 6));
        int y4 = y2 - static_cast<int>(arrowLength * sin(angle + M_PI / 6));

        gc->StrokeLine(x2, y2, x3, y3);
        gc->StrokeLine(x2, y2, x4, y4);
        delete gc;
    }
}

//绘制文本，接受文本坐标，颜色，大小
void DrawText(wxMemoryDC& memDC, const char* txt, int x, int y, const wxColour& color)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc)
    {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);

        memDC.SetPen(wxPen(color));

        wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        memDC.SetFont(font);

        wxCoord w, h;
        memDC.GetTextExtent(txt, &w, &h);

        w = (-1) * w / 2;
        h = (-1) * h / 2;
        memDC.DrawText(txt, w+x,h+y);

        delete gc;
    }
}