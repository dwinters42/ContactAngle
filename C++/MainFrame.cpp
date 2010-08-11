// Copyright (c) 2010 Daniel Gruber <daniel@tydirium.org>

// Permission to use, copy, modify, and distribute this software for
// any purpose with or without fee is hereby granted, provided that the
// above copyright notice and this permission notice appear in all
// copies.

// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
// DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
// OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.


#include "MainFrame.h"

MainFrame::MainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
    SetTitle(wxT("ContactAngle"));
    panel = new wxPanel(this, wxID_ANY);

    //menubar
    menubar = new wxMenuBar();
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(wxID_OPEN, wxEmptyString, wxEmptyString, wxITEM_NORMAL);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, wxEmptyString, wxEmptyString, wxITEM_NORMAL);
    menubar->Append(menuFile, wxT("&File"));
    wxMenu* menuInfo = new wxMenu();
    menuInfo->Append(wxID_ABOUT, wxEmptyString, wxEmptyString, wxITEM_NORMAL);
    menubar->Append(menuInfo, wxT("&About"));
    SetMenuBar(menubar);

    // statusbar
    statusbar = CreateStatusBar(1, 0);

    // toolbar
    toolbar = new wxToolBar(this, -1, wxDefaultPosition, wxDefaultSize,	\
			    wxTB_HORIZONTAL|wxTB_TEXT);
    SetToolBar(toolbar);
    toolbar->AddTool(wxID_OPEN, wxEmptyString, \
		     wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR),\
		     wxNullBitmap, wxITEM_NORMAL, wxT("Load video or image"),\
		     wxEmptyString);
    toolbar->AddTool(wxID_ANY, wxT("Analyze"),\
		     wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR),\
		     wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    toolbar->Realize();

    // plotwindow
    plotwindow = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap);
    plotwindow->SetMinSize(wxSize(640,480));

    static_line_1 = new wxStaticLine(panel, wxID_ANY);

    // sliders
    label_1 = new wxStaticText(panel, wxID_ANY, wxT("Frame number:"));
    sliderFramenum = new wxSlider(panel, wxID_ANY, 0, 0, 256, \
				  wxDefaultPosition, wxDefaultSize,\
				  wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
    label_2 = new wxStaticText(panel, wxID_ANY, wxT("Threshold:"));
    sliderThres = new wxSlider(panel, wxID_ANY, 0, 0, 10,\
				       wxDefaultPosition, wxDefaultSize,\
				       wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
    label_3 = new wxStaticText(panel, wxID_ANY, wxT("Base left:"));
    sliderLeft = new wxSlider(panel, wxID_ANY, 0, 0, 10,\
					 wxDefaultPosition, wxDefaultSize,\
					 wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
    label_4 = new wxStaticText(panel, wxID_ANY, wxT("Base right:"));
    sliderRight = new wxSlider(panel, wxID_ANY, 0, 0, 10,\
					 wxDefaultPosition, wxDefaultSize,\
					 wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);


    // put all stuff in sizers
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    sizer_2->Add(plotwindow, 1, wxALL|wxEXPAND, 0);
    sizer_2->Add(static_line_1, 0, wxEXPAND, 0);
    sizer_3->Add(label_1, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_3->Add(sliderFramenum, 5, wxALL|wxEXPAND, 0);
    sizer_2->Add(sizer_3, 0, wxEXPAND, 0);
    sizer_4->Add(label_2, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_4->Add(sliderThres, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    sizer_2->Add(sizer_4, 0, wxEXPAND, 0);
    sizer_5->Add(label_3, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_5->Add(sliderLeft, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    sizer_2->Add(sizer_5, 0, wxEXPAND, 0);
    sizer_6->Add(label_4, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_6->Add(sliderRight, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    sizer_2->Add(sizer_6, 0, wxEXPAND, 0);
    panel->SetSizer(sizer_2);
    sizer_1->Add(panel, 1, wxEXPAND, 0);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
    Layout();


    cv::Mat img = cv::imread("lena.jpg");
    wxImage plot(img.cols, img.rows, (unsigned char*) img.data, true);
    wxBitmap bm(plot);

    plotwindow->SetClientSize(img.cols,img.rows);
    plotwindow->SetBitmap(bm);
}


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MainFrame::loadFile)
    EVT_MENU(wxID_EXIT, MainFrame::onExit)
    EVT_MENU(wxID_ABOUT, MainFrame::onAbout)
END_EVENT_TABLE();


void MainFrame::loadFile(wxCommandEvent &event)
{
  event.Skip();
  wxLogDebug(wxT("Event handler (MainFrame::loadFile) not implemented yet"));
}

void MainFrame::onExit(wxCommandEvent &event)
{
  Destroy();
}

void MainFrame::onAbout(wxCommandEvent &event)
{
  wxAboutDialogInfo info;
  info.SetDescription(wxT("measures contact angles"));
  info.SetCopyright(wxT("(C) 2010 Daniel Gruber <daniel.gruber@tydirium.org>"));

  wxAboutBox(info);
}


