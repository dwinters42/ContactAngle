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

#include <cmath>
#define PI 3.14159265

#include "MainFrame.h"
#include "fit.h"

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
  toolbar->AddTool(wxID_OPEN, wxT("Load"),				\
		   wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR),\
		   wxNullBitmap, wxITEM_NORMAL, wxT("Load video or image"),\
		   wxEmptyString);
  toolbar->AddTool(wxID_ANY, wxT("Analyze"),\
		   wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR),\
		   wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
  toolbar->Realize();

  // plotting windows
  plotwindow = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap);
  plotwindow->SetMinSize(wxSize(640,480));
  fitwindowleft = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap);
  fitwindowleft->SetMinSize(wxSize(200,480));
  fitwindowright = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap);
  fitwindowright->SetMinSize(wxSize(200,480));

  static_line_1 = new wxStaticLine(panel, wxID_ANY);

  // sliders
  label_1 = new wxStaticText(panel, wxID_ANY, wxT("Frame number:"));
  sliderFramenum = new wxSlider(panel, wxID_ANY, 0, 0, 256, \
				wxDefaultPosition, wxDefaultSize,\
				wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
  label_2 = new wxStaticText(panel, wxID_ANY, wxT("Threshold:"));
  sliderThres = new wxSlider(panel, wxID_ANY, 0, 0, 255,\
			     wxDefaultPosition, wxDefaultSize,\
			     wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
  label_3 = new wxStaticText(panel, wxID_ANY, wxT("NumofPoints:"));
  sliderFitpoints = new wxSlider(panel, wxID_ANY, 0, 0, 10,\
			    wxDefaultPosition, wxDefaultSize,\
			    wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
  label_4 = new wxStaticText(panel, wxID_ANY, wxT("Base left:"));
  sliderLeft = new wxSlider(panel, wxID_ANY, 0, 0, 10,\
			    wxDefaultPosition, wxDefaultSize,\
			    wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
  label_5 = new wxStaticText(panel, wxID_ANY, wxT("Base right:"));
  sliderRight = new wxSlider(panel, wxID_ANY, 0, 0, 10,\
			     wxDefaultPosition, wxDefaultSize,\
			     wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);


  // put all stuff in sizers
  wxBoxSizer* MainMainsizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* MainSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* plotSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* sizerLeft = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* sizerFitpoints = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* sizerThres = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* sizerFramenum = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* sizerRight = new wxBoxSizer(wxHORIZONTAL);

  plotSizer->Add(plotwindow, 1, wxALL|wxEXPAND,5);
  plotSizer->Add(fitwindowleft, 0, wxALL|wxEXPAND,5);
  plotSizer->Add(fitwindowright, 0, wxALL|wxEXPAND,5);

  MainSizer->Add(plotSizer, 1, wxALL|wxEXPAND, 5);
  MainSizer->Add(static_line_1, 0, wxEXPAND, 5);

  sizerFramenum->Add(label_1, 0, wxALIGN_CENTER_VERTICAL, 5);
  sizerFramenum->Add(sliderFramenum, 5, wxALL|wxEXPAND, 5);
  MainSizer->Add(sizerFramenum, 0, wxEXPAND, 5);

  sizerThres->Add(label_2, 0, wxALIGN_CENTER_VERTICAL, 5);
  sizerThres->Add(sliderThres, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
  MainSizer->Add(sizerThres, 0, wxEXPAND, 5);

  sizerFitpoints->Add(label_3, 0, wxALIGN_CENTER_VERTICAL, 5);
  sizerFitpoints->Add(sliderFitpoints, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
  MainSizer->Add(sizerFitpoints, 0, wxEXPAND, 5);

  sizerLeft->Add(label_4, 0, wxALIGN_CENTER_VERTICAL, 5);
  sizerLeft->Add(sliderLeft, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
  MainSizer->Add(sizerLeft, 0, wxEXPAND, 5);

  sizerRight->Add(label_5, 0, wxALIGN_CENTER_VERTICAL, 5);
  sizerRight->Add(sliderRight, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
  MainSizer->Add(sizerRight, 0, wxEXPAND, 5);

  panel->SetSizer(MainSizer);
  MainMainsizer->Add(panel, 1, wxEXPAND, 5);
  SetSizer(MainMainsizer);
  MainMainsizer->Fit(this);
  Layout();

  dataloaded = false;

  // defaults
  threshold = 110;
  tilt=0.0;
}


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(wxID_OPEN, MainFrame::loadFile)
EVT_MENU(wxID_EXIT, MainFrame::onExit)
EVT_MENU(wxID_ABOUT, MainFrame::onAbout)
EVT_SCROLL(MainFrame::process)
END_EVENT_TABLE();


void MainFrame::loadFile(wxCommandEvent &event)
{
  wxFileDialog *dlg = new wxFileDialog(this);
  if (dlg->ShowModal() == wxID_OK) {
    filename=dlg->GetPath();

    wxWindowDisabler disabler;
    wxBusyInfo busy(wxT("loading data, please wait ..."));

    cap.open(std::string(filename.mb_str()));
    if(!cap.isOpened())
      wxMessageBox(wxT("Could not open file!"), wxT("Error"), wxICON_ERROR);
    else {
      SetTitle(wxT("ContactAngle: ")+filename);
      statusbar->SetStatusText(wxT("Loaded ")+filename);

      fileokay=true;
      fwidth=cap.get(CV_CAP_PROP_FRAME_WIDTH);
      fheight=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
      numframes=cap.get(CV_CAP_PROP_FRAME_COUNT);

      sliderFramenum->SetRange(0, numframes-1);
      sliderThres->SetValue(threshold);
      sliderFitpoints->SetRange(1,fheight);

      int defaultfitpoints = 80;
      if (defaultfitpoints>fheight)
	defaultfitpoints=fheight;
      sliderFitpoints->SetValue(defaultfitpoints);

      sliderLeft->SetRange(0,fheight-1);
      sliderLeft->SetValue(fheight/2);
      sliderRight->SetRange(0,fheight-1);
      sliderRight->SetValue(fheight/2);

      dataloaded=true;

      wxScrollEvent dummy;
      process(dummy);
    }
  }
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

void MainFrame::process(wxScrollEvent &event) {

  if (dataloaded) {
    cv::Mat frame;
    int bl, br;

    cap.set(CV_CAP_PROP_POS_FRAMES, sliderFramenum->GetValue());
    cap >> frame;

    cv::Mat edges(fheight,fwidth,CV_8UC1);
    cv::cvtColor(frame, edges, CV_BGR2GRAY);
    cv::equalizeHist(edges, edges);
    cv::threshold(edges, edges, sliderThres->GetValue(), 255, CV_THRESH_BINARY);
    
    if (tilt != 0.0) {
      cv::Mat temp(fheight, fwidth, CV_8UC1);
      cv::Mat rot_mat(2,3,CV_32FC1);
      rot_mat=getRotationMatrix2D(cv::Point(basepointx,basepointy), -1*tilt, 1.0);
      cv::warpAffine(edges, temp, rot_mat, edges.size());
      edges=temp;
      bl=baseleft;
      br=baseleft;
    }
    else {
      bl=baseleft;
      br=baseright;
    }

    cv::cvtColor(edges, frame, CV_GRAY2BGR);
    
    int numofpoints=sliderFitpoints->GetValue();
    int baseleft=sliderLeft->GetValue();
    int baseright=sliderRight->GetValue();

    dataleft = new double[numofpoints];
    dataright = new double[numofpoints];

    int row, col;

    // left points

    for (row=0; row<numofpoints; row++) {
      const uchar* Mi = edges.ptr<uchar>(baseleft-numofpoints+row);
      for (col=50; col<edges.cols-50; col++) {
	if (Mi[col] == 0) {
	  dataleft[row]=col;
	  cv::circle(frame, cv::Point(col,baseleft-numofpoints+row),	\
		     1, CV_RGB(0,0,255));
	  break;
	}
      }
    }

    // right points

    for (row=0; row<numofpoints; row++) {
      const uchar* Mi = edges.ptr<uchar>(baseright-numofpoints+row);
      for (col=edges.cols-50; col>50; col--) {
	if (Mi[col] == 0) {
	  dataright[row]=col;
	  cv::circle(frame, cv::Point(col,baseright-numofpoints+row),	\
		     1, CV_RGB(0,0,255));
	  break;
	}
      }
    }

    // plot the fitwindows

    cv::Mat fitleft = cv::Mat::zeros(480,200,CV_8UC3);
    fitleft=CV_RGB(255,255,255);
    cv::Mat fitright = cv::Mat::zeros(480,200,CV_8UC3);
    fitright=CV_RGB(255,255,255);

    // left
    int i, min=fwidth, max=0; 

    // find minimum and maximum value for scaling in x
    for(i=0;i<numofpoints;i++) {
      if (dataleft[i]<min)
	min=dataleft[i];
      if (dataleft[i]>max)
	max=dataleft[i];
    }

    // plot datapoints
    for (i=0;i<numofpoints;i++) {
      cv::circle(fitleft, cv::Point((dataleft[i]-min)*(200/(max-min)),	\
				    i*(480/numofpoints)),		\
		 3, CV_RGB(255,0,0),-1);

    }

    // make a parabolic fit on the points, see fit.[cpp,h]
    double *p = new double[3];
    double *x = new double[numofpoints];
    double offset;

    offset = dataleft[0];
    for (i=0;i<numofpoints;i++) {
      x[i]=numofpoints-i;
      dataleft[i]=dataleft[i]-offset;
      std::cout << dataleft[i] << std::endl;
    }
    polyfit(p,x,dataleft,2,numofpoints);
    std::cout << p[0] << " " << p[1] << " " << p[2] << std::endl;

    // plot fitted line
    for (i=0;i<numofpoints-1;i++) {
      cv::line(fitleft, cv::Point((p[0]*i*i+p[1]*i+p[2]-min)*(200/(max-min)), \
				  i*(480/numofpoints)),			\
	       cv::Point((p[0]*(i+1)*(i+1)+p[1]*(i+1)+p[2]-min)*(200/(max-min)), \
			 (i+1)*(480/numofpoints)), CV_RGB(0,0,255), 1, 8, 0);
    }

    wxImage imleft(fitleft.cols, fitleft.rows, (uchar*) fitleft.data, true);
    wxBitmap bmleft(imleft);
    fitwindowleft->SetBitmap(bmleft);

    double al;
    if (p[1]<0)
      al=180-(atan(-1/p[1])*180/PI);
    else
      al=atan(1/p[1])*180/PI;

    std::cout << al << std::endl;

    // right 
    min=fwidth;
    max=0;

    // find minimum and maximum value for scaling in x
    for(i=0;i<numofpoints;i++) {
      if (dataright[i]<min)
	min=dataright[i];
      if (dataright[i]>max)
	max=dataright[i];
    }

    // plot datapoints
    for (i=0;i<numofpoints;i++) {
      cv::circle(fitright, cv::Point(\
				    (dataright[i]-min)*(200/(max-min)),\
				    i*(480/numofpoints)),\
		 3, CV_RGB(255,0,0),-1);
    }

    wxImage imright(fitright.cols, fitright.rows, (uchar*) fitright.data, true);
    wxBitmap bmright(imright);
    fitwindowright->SetBitmap(bmright);


    delete[] dataleft;
    delete[] dataright;

    wxImage plot(fwidth, fheight, (uchar*) frame.data, true);
    wxBitmap bm(plot);
    
    plotwindow->SetClientSize(fwidth,fheight);
    plotwindow->SetBitmap(bm);
  }
}

