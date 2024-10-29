#include "Frame.h"
#include <wx/artprov.h>

//用宏定义创建事件表
BEGIN_EVENT_TABLE(Frame,wxFrame)
    EVT_MENU(wxID_NEW,Frame::onNew)
    EVT_MENU(wxID_EXIT,Frame::onQuit)
    EVT_TOOL(wxID_HELP,Frame::onHelp)
    EVT_TOOL(wxID_ANY+1, Frame::onCircle)
    EVT_TOOL(wxID_ANY+2, Frame::onLine)
END_EVENT_TABLE();

Frame::Frame(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxFrame(parent, id, title, pos, size, style, name)
{
    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu();

    //wxWidgets default wxMenuItem example
    fileMenu->Append(wxID_NEW);

    //Custom wxMenuItem 自定义选项
    wxMenuItem* testItem = fileMenu->Append(wxID_ANY, _("&Test\tCtrl+T"));
    Bind(wxEVT_MENU, &Frame::onTest, this, testItem->GetId());

    //Separator 分隔线
    fileMenu->AppendSeparator();

    //Submenu exampke 二级菜单
    wxMenu* subMenu = new wxMenu();
    subMenu->Append(wxID_CUT);
    subMenu->Append(wxID_COPY);
    subMenu->Append(wxID_PASTE);
    fileMenu->AppendSubMenu(subMenu, _("SubMenu"));

    fileMenu->AppendSeparator();

    //Create wxMenuItem example 创造选项并添加图标
    wxMenuItem* quitItem = new wxMenuItem(fileMenu, wxID_EXIT);
    quitItem->SetBitmap(wxArtProvider::GetBitmap("wxART_QUIT"));

    fileMenu->Append(quitItem);

    menuBar->Append(fileMenu, _("&File"));
    SetMenuBar(menuBar);




    wxToolBar* toolBar = CreateToolBar();

    //Create new tool
    toolBar->AddTool(wxID_NEW, _("New"), wxArtProvider::GetBitmap("wxART_NEW"));

    //Separator
    toolBar->AddSeparator();

    //Create quit tool
    toolBar->AddTool(wxID_EXIT, _("Quit"), wxArtProvider::GetBitmap("wxART_QUIT"));

    toolBar->AddTool(wxID_ANY+1, _("Draw circle"), wxArtProvider::GetBitmap("wxART_WX_LOGO"));
   
    toolBar->AddTool(wxID_ANY+2, _("Draw line"), wxArtProvider::GetBitmap("wxART_WX_LOGO"));
    
    //可伸展的空间
    toolBar->AddStretchableSpace();

    //Create help tool
    toolBar->AddTool(wxID_HELP, _("Help"), wxArtProvider::GetBitmap("wxART_HELP"));
    toolBar->SetToolShortHelp(wxID_HELP, _("This is the help button"));
    toolBar->SetToolLongHelp(wxID_HELP, _("This is the long help text"));

    toolBar->Realize();




    /*
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);//wxVERTICAL垂直  wxHORIZONTAL水平

    //Create first name box
    wxTextCtrl* firstNameBox = new wxTextCtrl(this, wxID_ANY);

    //第二个参数表示是否被拉伸   0：不被拉伸  非0：纵向拉伸（wxVERTICAL）/横向拉伸（wxHORIZONTAL)
    //第三个参数flag   wxEXPAND表示占满空间
    //第四个参数表示边框宽度(需要标签表示位置：wxALL/wxLEFT/wxRIGHT/wxTOP/wxBUTTOM）
    mainSizer->Add(firstNameBox,1,wxEXPAND | wxALL,10);

    SetSizer(mainSizer);
    */



    board = new Board(this);




    const int SIZE = 2;
    wxStatusBar* statusBar = CreateStatusBar(2);
    statusBar->SetStatusText(_("Ready!"));
    statusBar->SetStatusText(_("Hello World"), 1);
    //设置每个状态的宽度
    int widths[SIZE] = { -2,-1 };//负数表示可变宽度，仅告知第1个宽度为第2个的两倍，实际宽度由wx计算
    statusBar->SetStatusWidths(SIZE, widths);
}




//Event handling
void Frame::onNew(wxCommandEvent& event) 
{
    wxMessageBox("Frame::onNew");
    PushStatusText(_("Frame::onNew"));

    wxSleep(5);//维持5秒

    PopStatusText();//弹出
}

void Frame::onTest(wxCommandEvent& event)
{
    wxMessageBox("Frame::onTest");
}

void Frame::onHelp(wxCommandEvent& event)
{
    wxMessageBox("Frame::onHelp");
}

void Frame::onCircle(wxCommandEvent& event)
{
    board->circleClicked = true;
}

void Frame::onLine(wxCommandEvent& event)
{
    board->lineClicked = true;
}

void Frame::onQuit(wxCommandEvent& event)
{
    wxMessageBox("Frame::onQuit");
    bool veto = Close();
}

void Frame::onClose(wxCloseEvent& event)
{
    wxMessageBox("Frame::onClose");

    if (event.CanVeto())
    {
        int answer = wxMessageBox("Should we close?", _("Confirm?"), wxYES_NO);

        if (answer != wxYES)
        {
            event.Veto();
            return;
        }
    }
    Destroy();
}

Frame::~Frame()
{

}