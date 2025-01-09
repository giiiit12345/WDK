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
    /*
    circleClicked = false;
    circleSelected = false;
    */
    lineClicked = false;
    lineHori = false;
    lineVerti = false;
    lineStarted = false;
    lineSelected = false;

    resistorClicked = false;
    andgateClicked = false;
    notgateClicked = false;
    orgateClicked = false;

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
            if (lineHori) {//先绘制横向
                gc->StrokeLine(lineStartPos.x, lineStartPos.y, lineEndPos.x, lineStartPos.y);
                gc->StrokeLine(lineEndPos.x, lineStartPos.y, lineEndPos.x, lineEndPos.y);
            }
            else if (lineVerti) {//先绘制纵向
                gc->StrokeLine(lineStartPos.x, lineStartPos.y, lineStartPos.x, lineEndPos.y);
                gc->StrokeLine(lineStartPos.x, lineEndPos.y, lineEndPos.x, lineEndPos.y);
            }
        }

        //将已绘制的直线显示出来
        for (const auto& line : lines) {
            gc->SetPen(wxPen(wxColor(0, 0, 255), 1));
            gc->StrokeLine(line.first.x, line.first.y, line.second.x, line.second.y);
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
            gc->SetPen(wxPen(wxColor(0, 0, 0), 1));
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

void Board::onLeftClick(wxMouseEvent& event)
{
    wxPoint mousePosition = event.GetPosition();

    //直线绘制状态
    if (lineClicked)
    {
        if (!lineStarted)//选择线开始位置
        {
            lineStartPos = mousePosition;
            lineEndPos = lineStartPos;
            lineStarted = true;
        }
    }
    /*
    //圆绘制状态
    else if (circleClicked)
    {
        circlePos = mousePosition;
        circles.push_back(std::make_tuple(circlePos, 50, 50));
        circleClicked = false;
    }
    */
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

    //或门绘制状态
    else if (orgateClicked)
    {
        componentPos = mousePosition;
        AddComponent("003", componentPos);
        orgateClicked = false;

    }

    //判断是否点击到了可移动元素，使其选中后可移动
    else
    {
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
    if (lineClicked)
    {
        if (lineStarted)
        {
            lineEndPos = mousePosition;

            wxPoint lineMidPos;
            if (lineVerti) {//先绘制纵向
                lineMidPos.x = lineStartPos.x;
                lineMidPos.y = lineEndPos.y;
                lineVerti = false;
                lineHori = false;
            }
            else if (lineHori) {//先绘制横向
                lineMidPos.y = lineStartPos.y;
                lineMidPos.x = lineEndPos.x;
                lineHori = false;
                lineVerti = false;
            }

            lines.push_back(std::make_pair(lineStartPos, lineMidPos));
            lines.push_back(std::make_pair(lineMidPos, lineEndPos));
            lineStarted = false;
            lineClicked = false;
        }
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

void Board::onRightClick(wxMouseEvent& event)
{
    wxPoint mousePosition = event.GetPosition();

    //鼠标右击直线删除
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
            lineSelected = true;
            lines.erase(it);
            break;
        }
    }
}

void Board::onMouseMove(wxMouseEvent& event)
{
    wxPoint mousePosition = event.GetPosition();

    //直线绘制状态
    if (lineClicked && lineStarted)
    {
        lineEndPos = mousePosition;

        int minX = (lineStartPos.x < lineEndPos.x) ? lineStartPos.x : lineEndPos.x;
        int maxX = (lineStartPos.x > lineEndPos.x) ? lineStartPos.x : lineEndPos.x;
        int minY = (lineStartPos.y < lineEndPos.y) ? lineStartPos.y : lineEndPos.y;
        int maxY = (lineStartPos.y > lineEndPos.y) ? lineStartPos.y : lineEndPos.y;
        int deltaX = maxX - minX;
        int deltaY = maxY - minY;
        if (deltaX < 20 && deltaY < 20) {//x、y位移都小于20时，哪个位移更大决定先绘制哪个
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


// 将 JSON 数据传递给 json_ope.cpp 中的绘制方法
void Board::DrawPicture(const Json::Value& root)
{
    componentList.clear();
    lines.clear();


    // 解析并添加元器件
    const Json::Value& componentsData = root["components"];
    for (const auto& componentData : componentsData) {
        std::string id = componentData["id"].asString();
        std::string name = componentData["name"].asString();
        wxPoint position(componentData["centrePos"][0].asInt(), componentData["centrePos"][1].asInt());
        AddComponent(id, position);
    }

    // 解析并添加线
    const Json::Value& linesData = root["lines"];
    for (const auto& lineData : linesData) {
        std::string id = lineData["id"].asString();
        wxPoint start(lineData["start"][0].asInt(), lineData["start"][1].asInt());
        wxPoint end(lineData["end"][0].asInt(), lineData["end"][1].asInt());
        lines.push_back(std::make_pair(start, end));
    }

    // 刷新界面以绘制所有内容
    Refresh();
}

void Board::SaveToJson(const std::string& filePath) {
    Json::Value root;  // JSON 根节点

    // 1. 保存直线数据
    Json::Value linesJson(Json::arrayValue); // 初始化 lines 为数组
    int lineCounter = 1;  // 直线编号，从1开始
    for (const auto& line : lines) {
        Json::Value lineJson(Json::objectValue);

        // 给直线添加编号
        lineJson["id"] = "line" + std::to_string(lineCounter++);  // 直线编号：line1, line2, ...

        // 起点
        Json::Value startPos(Json::arrayValue);
        startPos.append(line.first.x);
        startPos.append(line.first.y);
        lineJson["start"] = startPos;

        // 终点
        Json::Value endPos(Json::arrayValue);
        endPos.append(line.second.x);
        endPos.append(line.second.y);
        lineJson["end"] = endPos;

        // 添加到 lines 数组
        linesJson.append(lineJson);
    }
    root["lines"] = linesJson;

    // 2. 保存元器件数据
    Json::Value componentsJson(Json::arrayValue); // 初始化 components 为数组
    std::map<std::string, int> componentCounter;  // 用于跟踪每个元器件类型的编号

    for (const auto& item : componentList) {
        Component* component = (Component*)item;  // 获取元器件
        Json::Value componentJson(Json::objectValue);

        // 获取元器件的 ID 
        int componentId = component->GetId();  // 获取元器件的唯一 ID

        // 获取并更新该 ID 的编号
        std::string componentIdStr = std::to_string(componentId);  // 转换为字符串
        componentCounter[componentIdStr]++;  // 使用字符串作为键
        std::string componentNum =std::to_string(componentCounter[componentIdStr]);


        // 元器件 ID
        componentJson["id"] = componentIdStr;
        componentJson["Num"] = componentNum;

        // 获取元器件的名称
        std::string componentName = component->GetType();
        componentJson["name"] = componentName; 


        // 中心位置
        Json::Value centrePos(Json::arrayValue);
        centrePos.append(component->centrePos.x);
        centrePos.append(component->centrePos.y);
        componentJson["centrePos"] = centrePos;

        // 是否显示
        componentJson["isShown"] = component->isShown;


        // 添加到 components 数组
        componentsJson.append(componentJson);
    }
    root["components"] = componentsJson;

    // 3. 将 JSON 数据写入文件
    std::ofstream file(filePath);  // 确保路径是 .json 格式
    if (file.is_open()) {
        std::cout << "Saving to file: " << filePath << std::endl;  // 输出文件路径
        file << root;  // 将 JSON 数据写入文件
        file.close();  // 完成写入后关闭文件
    }
    else {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
    }
}

