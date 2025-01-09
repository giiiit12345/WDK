#pragma once
#pragma once
#include <wx/wxprec.h>
#include <cstdlib>
#include "Component.h"
#include <fstream>


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class Board :public wxPanel
{
public:
    Board(wxFrame* parent);
public:
    void onPaint(wxPaintEvent& event);
    void DrawGrid(wxGraphicsContext* gc, int gridSize);
    void onLeftClick(wxMouseEvent& event);
    void onLeftUp(wxMouseEvent& event);
    void onRightClick(wxMouseEvent& event);
    void onMouseMove(wxMouseEvent& event);

    wxPoint lineStartPos;
    wxPoint lineEndPos;
    wxPoint circlePos;
    /*
    bool circleClicked;
    bool circleSelected;
    */
    bool lineClicked;
    bool lineHori;
    bool lineVerti;
    bool lineStarted;
    bool lineSelected;

    std::vector<std::pair<wxPoint, wxPoint>> lines;//存储直线的列表
    std::vector<std::tuple<wxPoint, int, int>> circles;//存储圆的列表


    void DrawComponents(wxMemoryDC& memDC);
    void AddComponent(wxString id, wxPoint& pt);
    wxList* GetComponentList() { return &componentList; }
    Component* FindComponent(const wxPoint& point);

    wxPoint componentPos;

    bool resistorClicked;
    bool andgateClicked;
    bool notgateClicked;

    bool componentSelected;
    Component* selectedComponent;

    wxList componentList; //存储元器件的列表

    void SaveToJson(const std::string& filePath); // 保存函数声明

    wxDECLARE_EVENT_TABLE();
};


