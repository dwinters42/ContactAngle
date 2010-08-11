
#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/statline.h>
#include <wx/artprov.h>

#include "cv.h"
#include "highgui.h"

class MyFrame: public wxFrame {
public:
    MyFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

protected:
    wxPanel* panel;
    wxMenuBar* menubar;
    wxStatusBar* statusbar;
    wxToolBar* toolbar;
    wxStaticBitmap* plotwindow;
    wxStaticLine* static_line_1;
    wxStaticText* label_1;
    wxSlider* sliderFramenum;
    wxStaticText* label_2;
    wxSlider* sliderThres;
    wxStaticText* label_3;
    wxSlider* sliderLeft;
    wxStaticText* label_4;
    wxSlider* sliderRight;

    DECLARE_EVENT_TABLE();

public:
    virtual void loadFile(wxCommandEvent &event);
    virtual void onExit(wxCommandEvent &event);
};

#endif // MYFRAME_H
