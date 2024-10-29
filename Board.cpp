#include "Board.h"

wxBEGIN_EVENT_TABLE(Board, wxPanel)
    EVT_PAINT(Board::onPaint)
    EVT_LEFT_DOWN(Board::onLeftClick)
    EVT_MOTION(Board::onMouseMove)
wxEND_EVENT_TABLE();

Board::Board(wxFrame* parent) : wxPanel(parent)
{
    SetFocus();
  

    circleClicked=false;
    lineClicked=false;
    lineStarted=false;

    SetBackgroundStyle(wxBG_STYLE_PAINT);
}


void Board::onPaint(wxPaintEvent& event) 
{
    //��������
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxBitmap bufferBitmap(GetClientSize());
    wxMemoryDC memDC;
    memDC.SelectObject(bufferBitmap);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();


    //��������
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc) 
    {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);

        //������
        DrawGrid(gc, 20);

        //��ֱ��
        if (lineClicked && lineStarted) 
        {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            gc->StrokeLine(lineStartPos.x,lineStartPos.y,lineEndPos.x,lineEndPos.y);
        }

        //���ѻ��Ƶ�ֱ����ʾ����
        for (const auto& line : lines) {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            gc->StrokeLine(line.first.x, line.first.y, line.second.x, line.second.y);
        }

        //��Բ
        if (circleClicked)
        {
            gc->SetPen(wxPen(wxColor(0, 0, 0), 1));
            gc->DrawEllipse(circlePos.x,circlePos.y, 50, 50);
        }

        //���ѻ��Ƶ�ֱ����ʾ����
        for (const auto& circle : circles) {
            gc->SetPen(wxPen(wxColor(0, 0, 0), 1));
            gc->DrawEllipse(std::get<0>(circle).x, std::get<0>(circle).y, std::get<1>(circle), std::get<2>(circle));
        }
        delete gc;
    }

    dc.Blit(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight(), &memDC, 0, 0);
}

void Board::DrawGrid(wxGraphicsContext* gc, int gridSize) 
{
    wxSize size = GetSize();

    gc->SetPen(wxPen(wxColour(180, 180, 180), 1, wxPENSTYLE_DOT_DASH));
    for (int x = 0; x < size.x; x += gridSize) 
    {
        gc->StrokeLine(x, 0, x, size.y);
    }
            
    for (int y = 0; y < size.y; y += gridSize) 
    {
        gc->StrokeLine(0, y, size.x, y);
    }


}

void Board::onLeftClick(wxMouseEvent& event) 
{
    wxPoint mousePosition = event.GetPosition();

    if (circleClicked)
    {
        circlePos = mousePosition;
        circles.push_back(std::make_tuple(circlePos, 50, 50));
        circleClicked = false;
    }
    if (lineClicked) 
    {
        if (!lineStarted) 
        {
            lineStartPos = mousePosition;
            lineEndPos = lineStartPos;
            lineStarted = true;
        }
        else
        {
            lineEndPos = mousePosition;
            lines.push_back(std::make_pair(lineStartPos, lineEndPos));
            lineStarted = false;
            lineClicked = false;
        }

        Refresh();
    }

}

void Board::onMouseMove(wxMouseEvent& event)
{
    wxPoint mousePosition = event.GetPosition();

    //ֱ�߻���״̬
    if (lineClicked && lineStarted) 
    {
        lineEndPos = mousePosition;
    }

    //Բ����״̬
    if (circleClicked) {
        circlePos = mousePosition;
    }

    Refresh();
}