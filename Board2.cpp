#include "Board.h"

wxBEGIN_EVENT_TABLE(Board, wxPanel)
EVT_PAINT(Board::onPaint)
EVT_LEFT_DOWN(Board::onLeftClick)
EVT_LEFT_UP(Board::onLeftUp)
EVT_RIGHT_DOWN(Board::onRightClick)
EVT_MOTION(Board::onMouseMove)
wxEND_EVENT_TABLE();

Board::Board(wxFrame* parent) : wxPanel(parent)
{
    SetFocus();

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);//设置布局管理器

    /*
    circleClicked = false;
    circleSelected = false;
    */
    lineClicked = false;
    lineHori = false;
    lineVerti = false;
    lineStarted = false;
    lineClicked = false;
    resistorClicked = false;
    andgateClicked = false;
    notgateClicked = false;
    orgateClicked = false;
    nearPoint = false;

    componentSelected = false;
    selectedComponent = (Component*)NULL;

    line1Started = false;
    line1Hori = false;
    line1Verti = false;

    line2Started = false;
    line2Hori = false;
    line2Verti = false;

    line3Started = false;
    line3Hori = false;
    line3Verti = false;

    rightSelectComponent = false;
    rightSelectLine = false;

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
        DrawGrid(gc, 10);


        //画直线
        if (lineStarted)
        {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            if (lineHori) {//先绘制横向
                gc->StrokeLine(lineStartPos.x, lineStartPos.y, lineEndPos.x, lineStartPos.y);
                gc->StrokeLine(lineEndPos.x, lineStartPos.y, lineEndPos.x, lineEndPos.y);
            }
            else if (lineVerti) {//先绘制纵向
                gc->StrokeLine(lineStartPos.x, lineStartPos.y, lineStartPos.x, lineEndPos.y);
                gc->StrokeLine(lineStartPos.x, lineEndPos.y, lineEndPos.x, lineEndPos.y);
            }
        }

        if (line1Started)
        {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            if (line1Hori) {//先绘制横向
                gc->StrokeLine(line1StartPos.x, line1StartPos.y, line1EndPos.x, line1StartPos.y);
                gc->StrokeLine(line1EndPos.x, line1StartPos.y, line1EndPos.x, line1EndPos.y);
            }
            else if (line1Verti) {//先绘制纵向
                gc->StrokeLine(line1StartPos.x, line1StartPos.y, line1StartPos.x, line1EndPos.y);
                gc->StrokeLine(line1StartPos.x, line1EndPos.y, line1EndPos.x, line1EndPos.y);
            }
        }

        if (line2Started)
        {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            if (line2Hori) {//先绘制横向
                gc->StrokeLine(line2StartPos.x, line2StartPos.y, line2EndPos.x, line2StartPos.y);
                gc->StrokeLine(line2EndPos.x, line2StartPos.y, line2EndPos.x, line2EndPos.y);
            }
            else if (line2Verti) {//先绘制纵向
                gc->StrokeLine(line2StartPos.x, line2StartPos.y, line2StartPos.x, line2EndPos.y);
                gc->StrokeLine(line2StartPos.x, line2EndPos.y, line2EndPos.x, line2EndPos.y);
            }
        }

        if (line3Started)
        {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            if (line3Hori) {//先绘制横向
                gc->StrokeLine(line3StartPos.x, line3StartPos.y, line3EndPos.x, line3StartPos.y);
                gc->StrokeLine(line3EndPos.x, line3StartPos.y, line3EndPos.x, line3EndPos.y);
            }
            else if (line3Verti) {//先绘制纵向
                gc->StrokeLine(line3StartPos.x, line3StartPos.y, line3StartPos.x, line3EndPos.y);
                gc->StrokeLine(line3StartPos.x, line3EndPos.y, line3EndPos.x, line3EndPos.y);
            }
        }

        //将已绘制的直线显示出来
        for (const auto& line : lines) {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            gc->StrokeLine(line.first.x, line.first.y, line.second.x, line.second.y);
        }

        //鼠标靠近连接点时强调该点
        if (nearPoint) {
            gc->SetPen(wxPen(wxColor(0, 255, 255), 1));
            gc->DrawEllipse(circlePos.x - 4, circlePos.y - 4, 8, 8);
        }
       
        /*
        //画圆
        if (circleClicked || circleSelected)
        {
            gc->SetPen(wxPen(wxColor(0, 0, 0), 1));
            gc->DrawEllipse(circlePos.x, circlePos.y, 50, 50);
        }
        
        //将已绘制的圆显示出来
        for (const auto& circle : circles) {
            gc->SetPen(wxPen(wxColor(0,255,255), 1));
            gc->DrawEllipse(std::get<0>(circle).x, std::get<0>(circle).y, std::get<1>(circle), std::get<2>(circle));
        }
        */
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

//返回离输入点最近的x、y均为10倍数的点
wxPoint Board:: findNearestTen(const wxPoint& point) {
    wxPoint nearestPoint;
    nearestPoint.x = round(static_cast<double>(point.x) / 10) * 10;
    nearestPoint.y = round(static_cast<double>(point.y) / 10) * 10;

    return nearestPoint;
}

void Board::onLeftClick(wxMouseEvent& event)
{
    wxPoint mousePosition = findNearestTen(event.GetPosition());

    //直线绘制状态
    if (lineClicked)
    {
        if (!lineStarted)//选择线开始位置
        {
            lineStartPos = mousePosition;
            lineEndPos = lineStartPos;
            lineStarted = true;
            lineClicked = false;
            resistorClicked = false;
            andgateClicked = false;
            notgateClicked = false;
            orgateClicked = false;
        }
    }
    /*
    //圆绘制状态
    if (circleClicked)
    {
        circlePos = mousePosition;
        circles.push_back(std::make_tuple(circlePos, 50, 50));
        circleClicked = false;
    }
    */
    //电阻器绘制状态
    if (resistorClicked)
    {
        componentPos = mousePosition;
        AddComponent("000", componentPos);
        lineClicked = false;
        resistorClicked = false;
        andgateClicked = false;
        notgateClicked = false;
        orgateClicked = false;


    }

    //与门绘制状态
    if (andgateClicked)
    {
        componentPos = mousePosition;
        AddComponent("001", componentPos);
        lineClicked = false;
        resistorClicked = false;
        andgateClicked = false;
        notgateClicked = false;
        orgateClicked = false;

    }

    //非门绘制状态
    if (notgateClicked)
    {
        componentPos = mousePosition;
        AddComponent("002", componentPos);
        lineClicked = false;
        resistorClicked = false;
        andgateClicked = false;
        notgateClicked = false;
        orgateClicked = false;

    }

    //或门绘制状态
    if (orgateClicked)
    {
        componentPos = mousePosition;
        AddComponent("003", componentPos);
        lineClicked = false;
        resistorClicked = false;
        andgateClicked = false;
        notgateClicked = false;
        orgateClicked = false;

    }

    //判断是否点击到了可移动元素或连接点
    /*
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
        */

    //移动元器件状态
    Component* component = FindComponent(mousePosition);
    if (component) {
        nearPoint = false;
        selectedComponent = component;
        componentSelected = true;

        //寻找与该元器件连接的直线，并存储连接点
        vector<int> connectPoints = selectedComponent->GetPoint();
        wxPoint point1;
        point1.x = connectPoints[0];
        point1.y = connectPoints[1];
        wxPoint point2;
        point2.x = connectPoints[2];
        point2.y = connectPoints[3];
        wxPoint point3;
        point3.x = connectPoints[4];
        point3.y = connectPoints[5];
      
        bool isLineHori = false;

        for (auto it = lines.begin(); it != lines.end();)
        {
            wxPoint startPos = std::get<0>(*it);
            wxPoint endPos = std::get<1>(*it);
            
            if (startPos.x == endPos.x) {
                isLineHori = false;
            }
            else if(startPos.y == endPos.y){
                isLineHori = true;
            }

            if (startPos == point1) {
                it = lines.erase(it);
                line1StartPos = endPos;
                line1Started = true;
                line1EndPos = point1;
                if (isLineHori) {
                    line1Hori = true;
                    line1Verti = false;
                }
                else {
                    line1Verti = true;
                    line1Hori = false;
                }
            }
            else if (startPos == point2) {
                it = lines.erase(it);
                line2StartPos = endPos;
                line2Started = true;
                line2EndPos = point2;
                if (isLineHori) {
                    line2Hori = true;
                    line2Verti = false;
                }
                else {
                    line2Verti = true;
                    line2Hori = false;
                }
            }
            else if (startPos == point3) {
                it = lines.erase(it);
                line3StartPos = endPos;
                line3Started = true;
                line3EndPos = point3;
                if (isLineHori) {
                    line3Hori = true;
                    line3Verti = false;
                }
                else {
                    line3Verti = true;
                    line3Hori = false;
                }
            }
            else if (endPos == point1) {
                it = lines.erase(it);
                line1StartPos = startPos;
                line1Started = true;
                line1EndPos = point1;
                if (isLineHori) {
                    line1Hori = true;
                    line1Verti = false;
                }
                else {
                    line1Verti = true;
                    line1Hori = false;
                }
            }
            else if (endPos == point2) {
                it = lines.erase(it);
                line2StartPos = startPos;
                line2Started = true;
                line2EndPos = point2;
                if (isLineHori) {
                    line2Hori = true;
                    line2Verti = false;
                }
                else {
                    line2Verti = true;
                    line2Hori = false;
                }
            }
            else if (endPos == point3) {
                it = lines.erase(it);
                line3StartPos = startPos;
                line3Started = true;
                line3EndPos = point3;
                if (isLineHori) {
                    line3Hori = true;
                    line3Verti = false;
                }
                else {
                    line3Verti = true;
                    line3Hori = false;
                }
            }
            else {
                ++it;
            }
        }
    }

    //连接点/直线中取点画线状态
    if (nearPoint) {
        lineStartPos = circlePos;
        lineEndPos = lineStartPos;
        lineStarted = true;
    }

    Refresh();
}

void Board::onLeftUp(wxMouseEvent& event)
{
    wxPoint mousePosition = findNearestTen(event.GetPosition());
    /*
    //圆移动状态
    if (circleSelected)
    {
        circlePos = mousePosition;
        circles.push_back(std::make_tuple(circlePos, 50, 50));
        circleSelected = false;
    }
    */
    //直线绘制状态
    if (lineStarted)
    {
        //直线绘制到连接点/直线中取点
        if (nearPoint) {
            lineEndPos = circlePos;
        }
        else {
            lineEndPos = mousePosition;
        }
        
        wxPoint lineMidPos;
        if (lineVerti) {//先绘制纵向
            lineMidPos.x = lineStartPos.x;
            lineMidPos.y = lineEndPos.y;
            lines.push_back(std::make_pair(lineStartPos, lineMidPos));
            if (lineMidPos != lineEndPos) {
                lines.push_back(std::make_pair(lineMidPos, lineEndPos));
            }
        }
        else if (lineHori) {//先绘制横向
            lineMidPos.y = lineStartPos.y;
            lineMidPos.x = lineEndPos.x;
            lines.push_back(std::make_pair(lineStartPos, lineMidPos));
            if (lineMidPos != lineEndPos) {
                lines.push_back(std::make_pair(lineMidPos, lineEndPos));
            }
        }

        lineVerti = false;
        lineHori = false;
        lineStarted = false;  
    }

    //元器件移动状态
    if (componentSelected)
    {
        componentPos = mousePosition;
        if (selectedComponent) {
            nearPoint = false;
            selectedComponent->Move(componentPos);

            vector<int> connectPoints = selectedComponent->GetPoint();
            wxPoint point1;
            point1.x = connectPoints[0];
            point1.y = connectPoints[1];
            wxPoint point2;
            point2.x = connectPoints[2];
            point2.y = connectPoints[3];
            wxPoint point3;
            point3.x = connectPoints[4];
            point3.y = connectPoints[5];

            //第一连接点
            if (line1Started)
            {
                line1EndPos = point1;

                wxPoint lineMidPos;
                if (line1Verti) {//先绘制纵向
                    lineMidPos.x = line1StartPos.x;
                    lineMidPos.y = line1EndPos.y;
                    line1Verti = false;
                    line1Hori = false;
                }
                else if (line1Hori) {//先绘制横向
                    lineMidPos.y = line1StartPos.y;
                    lineMidPos.x = line1EndPos.x;
                    line1Hori = false;
                    line1Verti = false;
                }

                lines.push_back(std::make_pair(line1StartPos, lineMidPos));
                if (lineMidPos != line1EndPos) {
                    lines.push_back(std::make_pair(lineMidPos, line1EndPos));
                }
                line1Started = false;
            }
            //第二连接点
            if (line2Started)
            {
                line2EndPos = point2;

                wxPoint lineMidPos;
                if (line2Verti) {//先绘制纵向
                    lineMidPos.x = line2StartPos.x;
                    lineMidPos.y = line2EndPos.y;
                    line2Verti = false;
                    line2Hori = false;
                }
                else if (line2Hori) {//先绘制横向
                    lineMidPos.y = line2StartPos.y;
                    lineMidPos.x = line2EndPos.x;
                    line2Hori = false;
                    line2Verti = false;
                }

                lines.push_back(std::make_pair(line2StartPos, lineMidPos));
                if (lineMidPos != line2EndPos) {
                    lines.push_back(std::make_pair(lineMidPos, line2EndPos));
                }
                line2Started = false;
            }
            //第三连接点
            if (line3Started)
            {
                line3EndPos = point3;

                wxPoint lineMidPos;
                if (line3Verti) {//先绘制纵向
                    lineMidPos.x = line3StartPos.x;
                    lineMidPos.y = line3EndPos.y;
                    line3Verti = false;
                    line3Hori = false;
                }
                else if (line3Hori) {//先绘制横向
                    lineMidPos.y = line3StartPos.y;
                    lineMidPos.x = line3EndPos.x;
                    line3Hori = false;
                    line3Verti = false;
                }

                lines.push_back(std::make_pair(line3StartPos, lineMidPos));
                if (lineMidPos != line3EndPos) {
                    lines.push_back(std::make_pair(lineMidPos, line3EndPos));
                }
                line3Started = false;
            }

            selectedComponent = (Component*)NULL;
        }
        componentSelected = false;

    }

    Refresh();
}

void Board::onRightClick(wxMouseEvent& event)
{
    wxPoint mousePosition = findNearestTen(event.GetPosition());

    //遍历已绘制的直线
    for (auto it = lines.begin(); it != lines.end(); ++it)
    {
        wxPoint startPos = std::get<0>(*it);
        wxPoint endPos = std::get<1>(*it);

        int minX = (startPos.x < endPos.x) ? startPos.x : endPos.x;
        int maxX = (startPos.x > endPos.x) ? startPos.x : endPos.x;
        int minY = (startPos.y < endPos.y) ? startPos.y : endPos.y;
        int maxY = (startPos.y > endPos.y) ? startPos.y : endPos.y;

        //判断鼠标点击位置是否在线附近
        if (mousePosition.x >= minX - 5 && mousePosition.x <= maxX + 5 && mousePosition.y >= minY - 5 && mousePosition.y <= maxY + 5)
        {
            rightSelectedLine = *it;
            rightSelectLine = true;
            showButtons(mousePosition);
            break;
        }
    }

    //遍历已绘制的元器件
    Component* component = FindComponent(mousePosition);
    if (component) {
        rightSelectComponent = true;
        rightSelectedComponent = component;
        showButtons(mousePosition);
    }
}

void Board::onMouseMove(wxMouseEvent& event)
{
    wxPoint mousePosition = findNearestTen(event.GetPosition());

    //直线绘制状态
    if (lineStarted)
    {
        lineEndPos = mousePosition;

        int minX = (lineStartPos.x < lineEndPos.x) ? lineStartPos.x : lineEndPos.x;
        int maxX = (lineStartPos.x > lineEndPos.x) ? lineStartPos.x : lineEndPos.x;
        int minY = (lineStartPos.y < lineEndPos.y) ? lineStartPos.y : lineEndPos.y;
        int maxY = (lineStartPos.y > lineEndPos.y) ? lineStartPos.y : lineEndPos.y;
        int deltaX = maxX - minX;
        int deltaY = maxY - minY;
        if (deltaX <= 20 && deltaY <= 20) {//x、y位移都小于20时，哪个位移更大决定先绘制哪个
            if (deltaX >= deltaY) {
                lineHori = true;
                lineVerti = false;
            }
            else {
                lineVerti = true;
                lineHori = false;
            }
        }

    }
    /*
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
    */
    //元器件移动状态
    if (componentSelected)
    {
        componentPos = mousePosition;
        selectedComponent->Move(componentPos);

        vector<int> connectPoints = selectedComponent->GetPoint();
        wxPoint point1;
        point1.x = connectPoints[0];
        point1.y = connectPoints[1];
        wxPoint point2;
        point2.x = connectPoints[2];
        point2.y = connectPoints[3];
        wxPoint point3;
        point3.x = connectPoints[4];
        point3.y = connectPoints[5];

        //第一连接点
        if (line1Started)
        {
            line1EndPos = point1;

            int minX = (line1StartPos.x < line1EndPos.x) ? line1StartPos.x : line1EndPos.x;
            int maxX = (line1StartPos.x > line1EndPos.x) ? line1StartPos.x : line1EndPos.x;
            int minY = (line1StartPos.y < line1EndPos.y) ? line1StartPos.y : line1EndPos.y;
            int maxY = (line1StartPos.y > line1EndPos.y) ? line1StartPos.y : line1EndPos.y;
            int deltaX = maxX - minX;
            int deltaY = maxY - minY;
            if (deltaX < 20 && deltaY < 20) {
                if (deltaX >= deltaY) {
                    line1Hori = true;
                    line1Verti = false;
                }
                else {
                    line1Verti = true;
                    line1Hori = false;
                }
            }

        }
        //第二连接点
        if (line2Started)
        {
            line2EndPos = point2;

            int minX = (line2StartPos.x < line2EndPos.x) ? line2StartPos.x : line2EndPos.x;
            int maxX = (line2StartPos.x > line2EndPos.x) ? line2StartPos.x : line2EndPos.x;
            int minY = (line2StartPos.y < line2EndPos.y) ? line2StartPos.y : line2EndPos.y;
            int maxY = (line2StartPos.y > line2EndPos.y) ? line2StartPos.y : line2EndPos.y;
            int deltaX = maxX - minX;
            int deltaY = maxY - minY;
            if (deltaX < 20 && deltaY < 20) {
                if (deltaX >= deltaY) {
                    line2Hori = true;
                    line2Verti = false;
                }
                else {
                    line2Verti = true;
                    line2Hori = false;
                }
            }

        }
        //第三连接点
        if (line3Started)
        {
            line3EndPos = point3;

            int minX = (line3StartPos.x < line3EndPos.x) ? line3StartPos.x : line3EndPos.x;
            int maxX = (line3StartPos.x > line3EndPos.x) ? line3StartPos.x : line3EndPos.x;
            int minY = (line3StartPos.y < line3EndPos.y) ? line3StartPos.y : line3EndPos.y;
            int maxY = (line3StartPos.y > line3EndPos.y) ? line3StartPos.y : line3EndPos.y;
            int deltaX = maxX - minX;
            int deltaY = maxY - minY;
            if (deltaX < 20 && deltaY < 20) {
                if (deltaX >= deltaY) {
                    line3Hori = true;
                    line3Verti = false;
                }
                else {
                    line3Verti = true;
                    line3Hori = false;
                }
            }

        }

    }
    
    nearPoint = false;

    //判断鼠标是否在已绘制的元器件的连接点附近
    wxPoint connectPoint = FindConnectPoint(mousePosition);
    if (connectPoint.x != -1) {
        circlePos = connectPoint;
        nearPoint = true;
    }
    else {
        //判断鼠标是否在已绘制的直线附近
        //遍历已绘制的直线
        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            wxPoint startPos = std::get<0>(*it);
            wxPoint endPos = std::get<1>(*it);

            int minX = (startPos.x < endPos.x) ? startPos.x : endPos.x;
            int maxX = (startPos.x > endPos.x) ? startPos.x : endPos.x;
            int minY = (startPos.y < endPos.y) ? startPos.y : endPos.y;
            int maxY = (startPos.y > endPos.y) ? startPos.y : endPos.y;

            wxPoint connectPoint;

            //判断鼠标点击位置是否在线附近
            if (mousePosition.x >= minX && mousePosition.x <= maxX && mousePosition.y >= minY - 5 && mousePosition.y <= maxY + 5 && minY == maxY)
            {
                connectPoint.x = mousePosition.x;
                connectPoint.y = minY;
                circlePos = connectPoint;
                nearPoint = true;
                break;
            }
            else if (mousePosition.x >= minX - 5 && mousePosition.x <= maxX + 5 && mousePosition.y >= minY && mousePosition.y <= maxY && minX == maxX)
            {
                connectPoint.y = mousePosition.y;
                connectPoint.x = minX;
                circlePos = connectPoint;
                nearPoint = true;
                break;
            }
            else {
                nearPoint = false;
            }
        }
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

//在componentList中鼠标附近连接点并返回
wxPoint Board::FindConnectPoint(const wxPoint& pos)
{
    wxList::compatibility_iterator node = componentList.GetFirst();
    wxPoint connectPoint;
    while (node) {
        Component* component = (Component*)node->GetData();
        connectPoint = component->FindPoint(pos);
        if (connectPoint.x != -1) {
            return connectPoint;
        }
        node = node->GetNext();
    }
    connectPoint.x = -1;
    connectPoint.y = -1;
    return connectPoint;
}

//右键点击线或元器件出现选项
void Board::showButtons(wxPoint mousePosition) {
    for (auto button : buttonList) {
        delete button;
    }
    buttonList.clear();

    wxButton* deleteButton = new wxButton(this, wxID_ANY, "delete",mousePosition);
    //wxButton* highlightButton = new wxButton(this, wxID_ANY, "highlight",wxPoint(mousePosition.x, mousePosition.y + 30));
    buttonList.push_back(deleteButton);
    //buttonList.push_back(highlightButton);
    
    deleteButton->SetSize(70, 30);
    //highlightButton->SetSize(70, 30);
    
    deleteButton->SetPosition(mousePosition);
    deleteButton->Bind(wxEVT_BUTTON, &Board::onDeleteButtonClick, this);

    //highlightButton->SetPosition(wxPoint(mousePosition.x, mousePosition.y + 30));


}

//点击删除按钮
void Board::onDeleteButtonClick(wxCommandEvent& event) {
    if (rightSelectLine) {
        for (auto it = lines.begin(); it != lines.end(); ++it) {
            if (*it == rightSelectedLine) {
                lines.erase(it);
                break;
            }
        }
    }
    else if (rightSelectComponent) {
        for (auto it = componentList.begin(); it != componentList.end(); ++it) {
            if (*it == rightSelectedComponent) {
                componentList.erase(it);
                break;
            }
        }
    }

    rightSelectComponent = false;
    rightSelectLine = false;
    for (auto button : buttonList) {
        delete button;
    }
    buttonList.clear();
}