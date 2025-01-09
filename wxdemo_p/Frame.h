#pragma once
#include <wx/wxprec.h>
#include <cstdlib>
#include "Board.h"

#include "Bitmap/line.xpm"
#include "Bitmap/resistor.xpm"
#include "Bitmap/andgate.xpm"
#include "Bitmap/notgate.xpm"
#include "Bitmap/orgate.xpm"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class Frame :public wxFrame
{
public:
    Frame(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));
    ~Frame();

    Board* board;

    void onNew(wxCommandEvent& event);
    void onTest(wxCommandEvent& event);
    void onSave(wxCommandEvent& event);
    void onQuit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);
    void onHelp(wxCommandEvent& event);
    //void onCircle(wxCommandEvent& event);
    void onLine(wxCommandEvent& event);
    void onResistor(wxCommandEvent& event);
    void onAndGate(wxCommandEvent& event);
    void onNotGate(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()

private:
    wxStaticText* st1;
    wxStaticText* st2;

};
