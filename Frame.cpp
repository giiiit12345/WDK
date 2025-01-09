#include "Frame.h"
#include <wx/artprov.h>

//用宏定义创建事件表
BEGIN_EVENT_TABLE(Frame, wxFrame)
EVT_MENU(wxID_NEW, Frame::onNew)
EVT_MENU(wxID_EXIT, Frame::onQuit)
EVT_MENU(wxID_SAVE, Frame::onSave)
EVT_TOOL(wxID_HELP, Frame::onHelp)
EVT_TOOL(wxID_OPEN, Frame::OnOpenButtonClicked)
//EVT_TOOL(wxID_ANY + 1, Frame::onCircle)
EVT_TOOL(wxID_ANY + 2, Frame::onLine)
EVT_TOOL(wxID_ANY + 3, Frame::onResistor)
EVT_TOOL(wxID_ANY + 4, Frame::onAndGate)
EVT_TOOL(wxID_ANY + 5, Frame::onNotGate)
EVT_TOOL(wxID_ANY + 6, Frame::onOrGate)
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

    fileMenu->Append(wxID_SAVE, "Save\tCtrl-S", "Save the current file");

    wxMenuItem* openItem = fileMenu->Append(wxID_OPEN, _("&Open\tCtrl+O"));
    Bind(wxEVT_MENU, &Frame::OnOpenButtonClicked, this, wxID_OPEN);



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

    wxBitmap Bitmaps[5];

    Bitmaps[0] = wxBitmap(line_xpm);
    wxImage image = Bitmaps[0].ConvertToImage();
    image.Rescale(image.GetWidth() * 0.08, image.GetHeight() * 0.08);
    wxBitmap lineBitmap = wxBitmap(image);

    Bitmaps[1] = wxBitmap(resistor_xpm);
    image = Bitmaps[1].ConvertToImage();
    image.Rescale(image.GetWidth() * 0.08, image.GetHeight() * 0.08);
    wxBitmap resistorBitmap = wxBitmap(image);

    Bitmaps[2] = wxBitmap(andgate_xpm);
    image = Bitmaps[2].ConvertToImage();
    image.Rescale(image.GetWidth() * 0.08, image.GetHeight() * 0.08);
    wxBitmap andgateBitmap = wxBitmap(image);

    Bitmaps[3] = wxBitmap(notgate_xpm);
    image = Bitmaps[3].ConvertToImage();
    image.Rescale(image.GetWidth() * 0.08, image.GetHeight() * 0.08);
    wxBitmap notgateBitmap = wxBitmap(image);

    Bitmaps[4] = wxBitmap(orgate_xpm);
    image = Bitmaps[4].ConvertToImage();
    image.Rescale(image.GetWidth() * 0.08, image.GetHeight() * 0.08);
    wxBitmap orgateBitmap = wxBitmap(image);

    //toolBar->AddTool(wxID_ANY + 1, _("Draw circle"), wxArtProvider::GetBitmap("wxART_WX_LOGO"));
    toolBar->AddTool(wxID_ANY + 2, _("Draw line"), lineBitmap);
    toolBar->AddTool(wxID_ANY + 3, _("Draw resistor"), resistorBitmap);
    toolBar->AddTool(wxID_ANY + 4, _("Draw andgate"), andgateBitmap);
    toolBar->AddTool(wxID_ANY + 5, _("Draw notgate"), notgateBitmap);
    toolBar->AddTool(wxID_ANY + 6, _("Draw orgate"), orgateBitmap);

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

void Frame::onSave(wxCommandEvent& event) {
    // 显示保存文件对话框
    wxFileDialog saveFileDialog(this, "Save File", "", "", "JSON Files (*.json)|*.json", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return; // 用户取消
    }

    wxString filePath = saveFileDialog.GetPath();
    std::string path(filePath.mb_str());

    // 调用保存函数
    board->SaveToJson(path);

    // 模拟保存内容（你可以替换为实际文件写入逻辑）
    wxMessageBox("File saved to: " + filePath, "Save Successful", wxOK | wxICON_INFORMATION);
}


void Frame::OnOpenButtonClicked(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("Open JSON file"), "", "",
        "JSON files (*.json)|*.json", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return; // 用户取消了文件选择
    }

    wxString filePath = openFileDialog.GetPath();
    std::string path = std::string(filePath.mb_str());

    // 读取 JSON 文件
    Json::Value root = ReadJson(path);
    if (root.isNull()) {
        wxLogError("无法读取或解析 JSON 文件: %s", path);
        return;
    }

    // 将 JSON 数据传递给 Board 类的 DrawPicture 方法进行绘制
    board->DrawPicture(root);
}


void Frame::onTest(wxCommandEvent& event)
{
    wxMessageBox("Frame::onTest");
}

void Frame::onHelp(wxCommandEvent& event)
{
    wxMessageBox("Frame::onHelp");
}

/*
void Frame::onCircle(wxCommandEvent& event)
{
    board->circleClicked = true;
}
*/
void Frame::onLine(wxCommandEvent& event)
{
    board->lineClicked = true;
}

void Frame::onResistor(wxCommandEvent& event)
{
    board->resistorClicked = true;
}

void Frame::onAndGate(wxCommandEvent& event)
{
    board->andgateClicked = true;
}

void Frame::onNotGate(wxCommandEvent& event)
{
    board->notgateClicked = true;
}

void Frame::onOrGate(wxCommandEvent& event)
{
    board->orgateClicked = true;
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