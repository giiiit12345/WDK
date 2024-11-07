#include "Symbol.h"

//����ֱ��,���ܶ˵�����,������ɫ
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

//���������Σ���������������,��ɫ
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

//���ƾ��Σ����ܶԽǶ�������,��ɫ
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

//����Բ�Σ�����Բ�����꣬�뾶,��ɫ
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


//���Ƽ�ͷ��������ʼ������,��ɫ
void DrawArrow(wxMemoryDC& memDC, int x1, int y1, int x2, int y2, const wxColour& borderColor)
{
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc)
    {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);

        gc->SetPen(wxPen(borderColor, 1));

        // ���Ƽ�ͷ��ֱ�߲���
        gc->StrokeLine(x1, y1, x2, y2);

        // �����ͷ�ĽǶ�
        double angle = atan2(static_cast<double>(y2 - y1), static_cast<double>(x2 - x1));

        // �����ͷ�ĳ��ȺͿ��
        int arrowLength = 10;
        int arrowWidth = 5;

        // ���Ƽ�ͷ��������
        int x3 = x2 - static_cast<int>(arrowLength * cos(angle - M_PI / 6));
        int y3 = y2 - static_cast<int>(arrowLength * sin(angle - M_PI / 6));
        int x4 = x2 - static_cast<int>(arrowLength * cos(angle + M_PI / 6));
        int y4 = y2 - static_cast<int>(arrowLength * sin(angle + M_PI / 6));

        gc->StrokeLine(x2, y2, x3, y3);
        gc->StrokeLine(x2, y2, x4, y4);
        delete gc;
    }
}

//�����ı��������ı����꣬��ɫ����С
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