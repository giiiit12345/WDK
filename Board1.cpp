#include "Board.h"

wxBEGIN_EVENT_TABLE(Board, wxPanel)
EVT_PAINT(Board::onPaint)
EVT_LEFT_DOWN(Board::onLeftClick)
EVT_LEFT_UP(Board::onLeftUp)
EVT_MOTION(Board::onMouseMove)
wxEND_EVENT_TABLE();

Board::Board(wxFrame* parent) : wxPanel(parent)
{
    SetFocus();

    circleClicked = false;
    circleSelected = false;

    lineClicked = false;
    lineStarted = false;
    lineSelected = false;

    resistorClicked=false;
    andgateClicked=false;
    notgateClicked=false;

    componentSelected = false;
    selectedComponent = (Component*)NULL;

    SetBackgroundStyle(wxBG_STYLE_PAINT);
}


void Board::onPaint(wxPaintEvent& event)
{
    //创建画板
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxBitmap bufferBitmap(GetClientSize());
    wxMemoryDC memDC;
    memDC.SelectObject(bufferBitmap);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();


    //创建画笔
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc)
    {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);

        //画格子
        DrawGrid(gc, 20);

        //画直线
        if (lineClicked && lineStarted)
        {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            gc->StrokeLine(lineStartPos.x, lineStartPos.y, lineEndPos.x, lineEndPos.y);
        }

        //将已绘制的直线显示出来
        for (const auto& line : lines) {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            gc->StrokeLine(line.first.x, line.first.y, line.second.x, line.second.y);
        }

        //画圆
        if (circleClicked || circleSelected)
        {
            gc->SetPen(wxPen(wxColor(0, 0, 0), 1));
            gc->DrawEllipse(circlePos.x, circlePos.y, 50, 50);
        }

        //将已绘制的圆显示出来
        for (const auto& circle : circles) {
            gc->SetPen(wxPen(wxColor(0, 0, 0), 1));
            gc->DrawEllipse(std::get<0>(circle).x, std::get<0>(circle).y, std::get<1>(circle), std::get<2>(circle));
        }

        delete gc;
    }

    //将已绘制的元器件显示出来
    DrawComponents(memDC);

    dc.Blit(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight(), &memDC, 0, 0);
}

//绘制网格
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

    //直线绘制状态
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
    }

    //圆绘制状态
    else if (circleClicked)
    {
        circlePos = mousePosition;
        circles.push_back(std::make_tuple(circlePos, 50, 50));
        circleClicked = false;
    }

    //电阻器绘制状态
    else if (resistorClicked)
    {
        componentPos = mousePosition;
        AddComponent("000", componentPos);
        resistorClicked = false;

    }

    //与门绘制状态
    else if (andgateClicked)
    {
        componentPos = mousePosition;
        AddComponent("001", componentPos);
        andgateClicked = false;

    }

    //非门绘制状态
    else if (notgateClicked)
    {
        componentPos = mousePosition;
        AddComponent("002", componentPos);
        notgateClicked = false;

    }

    //判断是否点击到了可移动元素
    else
    {
        //遍历已绘制的直线
        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            wxPoint startPos = std::get<0>(*it);
            wxPoint endPos = std::get<1>(*it);

            int minX = (startPos.x < endPos.x) ? startPos.x : endPos.x;
            int maxX = (startPos.x > endPos.x) ? startPos.x : endPos.x;
            int minY = (startPos.y < endPos.y) ? startPos.y : endPos.y;
            int maxY = (startPos.y > endPos.y) ? startPos.y : endPos.y;

            //判断鼠标点击位置是否在线上
            if (mousePosition.x >= minX && mousePosition.x <= maxX && mousePosition.y == mousePosition.x * (maxY - minY) / (maxX - minX))
            {
                lineSelected = true;
                lines.erase(it);
                break;
            }
        }



        //遍历已绘制的圆
        for (auto it = circles.begin(); it != circles.end(); ++it)
        {
            wxPoint circlePos = std::get<0>(*it);
            int circleRadius = std::get<1>(*it) / 2;
            // 判断鼠标点击位置是否在圆内
            if ((mousePosition.x - circlePos.x - circleRadius) * (mousePosition.x - circlePos.x - circleRadius) +
                (mousePosition.y - circlePos.y - circleRadius) * (mousePosition.y - circlePos.y - circleRadius) <= circleRadius * circleRadius)
            {
                circleSelected = true;
                circles.erase(it);
                break;
            }
        }

        //遍历已绘制元器件
        Component* component = FindComponent(mousePosition);
        if (component) {
            selectedComponent = component;
            componentSelected = true;
        }
    }


    Refresh();
}

void Board::onLeftUp(wxMouseEvent& event)
{
    wxPoint mousePosition = event.GetPosition();

    //圆移动状态
    if (circleSelected)
    {
        circlePos = mousePosition;
        circles.push_back(std::make_tuple(circlePos, 50, 50));
        circleSelected = false;
    }

    //元器件移动状态
    if (componentSelected)
    {
        componentPos = mousePosition;
        if (selectedComponent) {
            selectedComponent->Move(componentPos);
            selectedComponent = (Component*)NULL;
        }
        componentSelected = false;

    }

    Refresh();
}

void Board::onMouseMove(wxMouseEvent& event)
{
    wxPoint mousePosition = event.GetPosition();

    //直线绘制状态
    if (lineClicked && lineStarted)
    {
        lineEndPos = mousePosition;
    }

    //圆绘制状态
    if (circleClicked)
    {
        circlePos = mousePosition;

    }

    //圆移动状态
    if (circleSelected)
    {
        circlePos = mousePosition;
    }

    //元器件移动状态
    if (componentSelected)
    {
        componentPos = mousePosition;
        selectedComponent->Move(componentPos);
    }

    Refresh();
}

//将已绘制的元器件显示出来
void Board::DrawComponents(wxMemoryDC& memDC)
{
    wxList::compatibility_iterator node = componentList.GetFirst();
    while (node)
    {
        Component* component = (Component*)node->GetData();
        if (component->IsShown()) {
            component->Draw(memDC, root[component->GetId()]);
        }
        node = node->GetNext();
    }
}


//绘制一个元器件
void Board::AddComponent(wxString id, wxPoint& pos)
{
    Json::Value root = ReadJson("test.json"); //后续用全局变量代替
    Json::Value target = root[atoi(id)];
    Component* newComponent = new Component(target);
    newComponent->Move(pos);
    this->GetComponentList()->Append(newComponent);

    Refresh(true);
    Update();
}

//在componentList中寻找选中元器件并返回
Component* Board::FindComponent(const wxPoint& pos)
{
    wxList::compatibility_iterator node = componentList.GetFirst();
    while (node) {
        Component* component = (Component*)node->GetData();
        if (component->isSelected(pos)) {
            return component;
        }
        node = node->GetNext();
    }
    return (Component*)NULL;
}


