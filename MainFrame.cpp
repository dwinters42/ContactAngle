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
#include <cstdio>

#include <wx/file.h>
#include <wx/utils.h>
#include <wx/progdlg.h>
#include <wx/log.h>
#include <wx/debug.h>

#include "MainFrame.h"
#include "fit.h"

#define PI 3.14159265

MainFrame::MainFrame(wxWindow* parent, int id, const wxString& title, \
		     const wxPoint& pos, const wxSize& size, long style):
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
  toolbar->AddTool(ID_dedup, wxT("Dedup"),				\
		   wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR),\
		   wxNullBitmap, wxITEM_NORMAL, wxT("Find non-duplicate frames"),\
		   wxEmptyString);
  toolbar->AddTool(ID_processAll, wxT("Analyze Movie"),\
		   wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR),\
		   wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
  toolbar->Realize();

  // plotting windows
  plotwindow = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap);
  plotwindow->SetMinSize(wxSize(320,240));
  fitwindowleft = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap);
  fitwindowleft->SetMinSize(wxSize(100,240));
  fitwindowright = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap);
  fitwindowright->SetMinSize(wxSize(100,240));

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

  sizerLeft->Add(label_4, 0, wxALIGN_CENTER_VERTICAL, 5);
  sizerLeft->Add(sliderLeft, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
  MainSizer->Add(sizerLeft, 0, wxEXPAND, 5);

  sizerRight->Add(label_5, 0, wxALIGN_CENTER_VERTICAL, 5);
  sizerRight->Add(sliderRight, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
  MainSizer->Add(sizerRight, 0, wxEXPAND, 5);

  sizerFitpoints->Add(label_3, 0, wxALIGN_CENTER_VERTICAL, 5);
  sizerFitpoints->Add(sliderFitpoints, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
  MainSizer->Add(sizerFitpoints, 0, wxEXPAND, 5);

  panel->SetSizer(MainSizer);
  MainMainsizer->Add(panel, 1, wxEXPAND, 5);
  SetSizer(MainMainsizer);
  MainMainsizer->SetSizeHints(this);
  MainMainsizer->Fit(this);
  Layout();

  dataloaded = false;
  framesToAnalyze.Clear();

  // defaults
  threshold = 110;
  tilt=0.0;
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(wxID_OPEN, MainFrame::loadFile)
EVT_MENU(wxID_EXIT, MainFrame::onExit)
EVT_MENU(wxID_ABOUT, MainFrame::onAbout)
EVT_MENU(ID_dedup, MainFrame::onDedup)
EVT_MENU(ID_processAll, MainFrame::processAll)
EVT_SCROLL(MainFrame::onScroll)
EVT_SIZE(MainFrame::onSize)
END_EVENT_TABLE();

void MainFrame::loadFile(wxCommandEvent &event)
{
  wxFileDialog *dlg = new wxFileDialog(this, wxT("Choose a video file"), \
				       wxT(""), wxT(""), wxT("*.*"), \
				       wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR);
  if (dlg->ShowModal() == wxID_OK) {
    filename=dlg->GetPath();
    _loadFile(filename);
  }
}

int MainFrame::_loadFile(wxFileName fn)
{

  filename = fn;
  wxWindowDisabler disabler;
  wxBusyInfo busy(wxT("loading data, please wait ..."));

  cap.open(std::string(filename.GetFullPath().mb_str()));
  if(!cap.isOpened()) {
    wxLogError(wxT("Could not open file!"));
    return -1;
  }

  SetTitle(wxT("ContactAngle: ")+filename.GetFullName());
  wxLogStatus(wxT("Loaded ")+filename.GetFullName());
  
  fwidth=cap.get(CV_CAP_PROP_FRAME_WIDTH);
  fheight=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  numframes=cap.get(CV_CAP_PROP_FRAME_COUNT);

  wxLogDebug(wxT("%s: %ix%i, %i frames"),filename.GetFullName().c_str(),fwidth,fheight,numframes);

  sliderFramenum->SetRange(1, numframes);
  sliderFramenum->SetValue(1);

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
  deduped=false;
  framesToAnalyze.Clear();
  
  _process();

  return 0;
}


void MainFrame::onDedup(wxCommandEvent &event)
{
  if (!dataloaded) {
    wxLogError(wxT("No data loaded!"));
    return;
  }
  
  _dedup();
  _process();
}

int MainFrame::_dedup()
{
  int i;
  cv::Mat temp;
  cv::Mat oldframe(fheight,fwidth,CV_8UC1);
  cv::Mat difframe(fheight,fwidth,CV_8UC1);
  cv::Mat nextframe(fheight,fwidth,CV_8UC1);

  wxProgressDialog *dlg = \
    new wxProgressDialog(wxT("Finding duplicate images"),		\
			 wxT("looking for duplicates in movie ..."),	\
			 numframes-1, this, wxPD_AUTO_HIDE | wxPD_APP_MODAL);

  framesToAnalyze.Clear();
  wxLogDebug(wxT("numframes = %i"),numframes);

  /* grab a first frame */
  cap.set(CV_CAP_PROP_POS_FRAMES, 0);
  cap >> temp;
  cv::cvtColor(temp, oldframe, CV_BGR2GRAY);
  framesToAnalyze.Add(0);

  /* run through all frames */
  for (i=1;i<numframes;i++) {
    cap >> temp;
    cv::cvtColor(temp, nextframe, CV_BGR2GRAY);
    cv::compare(oldframe, nextframe, difframe, cv::CMP_NE);
    if(cv::countNonZero(difframe) != 0) {
      /* this is a new frame */
      framesToAnalyze.Add(i);
      oldframe = nextframe.clone();
    }
    dlg->Update(i);
  }

  wxLogStatus(wxT("Found %i unique frames, using only these"),framesToAnalyze.GetCount());

  deduped = true;
  numframes=framesToAnalyze.GetCount();
  sliderFramenum->SetValue(1);
  sliderFramenum->SetRange(1, numframes);
 
  return 0;
}

void MainFrame::onExit(wxCommandEvent &event)
{
  Destroy();
}

void MainFrame::onAbout(wxCommandEvent &event)
{
#ifndef _WIN32 
  wxAboutDialogInfo info;
  info.SetDescription(wxT("measure contact angles in videos"));
  info.SetCopyright(wxT("(C) 2010 Daniel Gruber <daniel.gruber@tydirium.org>"));
  info.SetVersion(wxT("Version VERSION_MAJOR.VERSION_MINOR"));

  wxAboutBox(info);
#endif
}

void MainFrame::processAll(wxCommandEvent &event) {
  int i;

  if (!dataloaded) {
    wxLogError(wxT("No data loaded!"));
    return;
  }

  wxFileName outfilename = filename;
  outfilename.SetExt(wxT("txt"));
  
  if (outfilename.FileExists()) {
    if (wxMessageBox(wxT("Output file exists, overwrite?"), wxT("Confirm"),wxYES_NO)==wxNO)
      return;
  }
  
  wxFile outfile(outfilename.GetFullPath(),wxFile::write);
  wxString str;

  // print file header
  outfile.Write(wxT("# contact angle data for file ")+filename.GetFullName()+wxT("\n"));
  str.Printf(wxT("# thresh = %i\n"),sliderThres->GetValue());
  outfile.Write(str);
  str.Printf(wxT("# baseleft = %i\n"),sliderLeft->GetValue());
  outfile.Write(str);
  str.Printf(wxT("# baseright = %i\n"),sliderRight->GetValue());
  outfile.Write(str);
  str.Printf(wxT("# numoffitpoints = %i\n"),sliderFitpoints->GetValue());
  outfile.Write(str);

  // do the actual work
  wxBeginBusyCursor();
  for (i=1; i<=numframes; i++) {
    sliderFramenum->SetValue(i);
    _process();
    str.Printf(wxT("%i, %f, %f\n"),i,al,ar);
    outfile.Write(str);
    wxYield();
  }
  wxEndBusyCursor();

  sliderFramenum->SetValue(1);
  _process();

  wxLogMessage(wxT("Successfully wrote output file ") + outfilename.GetFullName() + wxT("."));
}

void MainFrame::onScroll(wxScrollEvent &event) {
  _process();
}

void MainFrame::onSize(wxSizeEvent &event) {
  Layout();
  _process();
}

int MainFrame::_process() {

  if (!dataloaded)
    return -1;

  cv::Mat frame;
  int bl, br;

  if (deduped)
    cap.set(CV_CAP_PROP_POS_FRAMES, framesToAnalyze.Item(sliderFramenum->GetValue()-1));
  else
    cap.set(CV_CAP_PROP_POS_FRAMES, sliderFramenum->GetValue()-1);
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

  int i, row, col;

  double *yvals = new double[numofpoints];
  for (i=0;i<numofpoints;i++)
    yvals[i]=i;

  double *dataleft = new double[numofpoints];
  double *dataright = new double[numofpoints];

  double min, max;

  // #### left contact angle ###

  // first, find the data points
  for (row=0; row<numofpoints; row++) {
    const uchar* Mi = edges.ptr<uchar>(baseleft-numofpoints+row);
    for (col=50; col<edges.cols-50; col++) {
      if (Mi[col] == 0) {
	dataleft[numofpoints-row-1]=col;

	// plot a dot in the image window
	cv::circle(frame, cv::Point(col,baseleft-numofpoints+row),	\
		   1, CV_RGB(0,0,255));
	break;
      }
    }
  }

  min=1e30;
  max=1e-30;

  for(i=0;i<numofpoints;i++) {
    min=MIN(min, dataleft[i]);
    max=MAX(max, dataleft[i]);
  }

  int width, height;
  fitwindowleft->GetClientSize(&width,&height);

  wxMemoryDC memDC;
  wxBitmap bmleft(width,height);

  memDC.SelectObject(bmleft);
  memDC.SetBackground(*wxWHITE_BRUSH);
  memDC.Clear();
  memDC.SetPen(*wxBLACK);
  memDC.SetBrush(*wxBLUE_BRUSH);

  double xstep=width/(max-min);
  double ystep=height/numofpoints;
  int xpos,ypos;

  // draw data points
  for (i=0;i<numofpoints;i++) {
    xpos=(dataleft[i]-min)*xstep;
    ypos=height-yvals[i]*ystep;
    memDC.DrawCircle(xpos,ypos,4);
  }      

  // make a parabolic fit on the points, see fit.[cpp,h]
  double *p = new double[3];
  double *x = new double[numofpoints];
  double offsetleft, offsetright;

  offsetleft=dataleft[0];
  for (i=0;i<numofpoints;i++) {
    x[i]=i;
    dataleft[i]=dataleft[i]-offsetleft;
  }
  polyfit(p,x,dataleft,2,numofpoints);

  // plot fitted line
  memDC.SetPen(*wxRED);
  memDC.SetBrush(*wxRED_BRUSH);

  double *fitdata = new double[numofpoints];
  fitdata[0]=p[2]+offsetleft;
  for (i=1;i<numofpoints;i++) {
    fitdata[i]=p[0]*x[i]*x[i]+p[1]*x[i]+p[2]+offsetleft;

    memDC.DrawLine((fitdata[i]-min)*xstep,height-yvals[i]*ystep,\
		   (fitdata[i-1]-min)*xstep,height-yvals[i-1]*ystep);
  }
  memDC.SelectObject(wxNullBitmap);
  fitwindowleft->SetBitmap(bmleft);

  // calculate contact angle
  if (p[1]<0)
    al=180-(atan(-1/p[1])*180/PI);
  else
    al=atan(1/p[1])*180/PI;

  // ### right contact angle ### 

  // right data points
  for (row=0; row<numofpoints; row++) {
    const uchar* Mi = edges.ptr<uchar>(baseright-numofpoints+row);
    for (col=edges.cols-50; col>50; col--) {
      if (Mi[col] == 0) {
	dataright[numofpoints-row-1]=col;
	cv::circle(frame, cv::Point(col,baseright-numofpoints+row),	\
		   1, CV_RGB(0,0,255));
	break;
      }
    }
  }

  min=1e30;
  max=1e-30;

  for(i=0;i<numofpoints;i++) {
    min=MIN(min, dataright[i]);
    max=MAX(max, dataright[i]);
  }

  fitwindowright->GetClientSize(&width,&height);

  wxBitmap bmright(width,height);

  memDC.SelectObject(bmright);
  memDC.SetBackground(*wxWHITE_BRUSH);
  memDC.Clear();
  memDC.SetPen(*wxBLACK);
  memDC.SetBrush(*wxBLUE_BRUSH);

  xstep=width/(max-min);
  ystep=height/numofpoints;

  // draw data points
  for (i=0;i<numofpoints;i++) {
    xpos=(dataright[i]-min)*xstep;
    ypos=height-yvals[i]*ystep;
    memDC.DrawCircle(xpos,ypos,4);
  }      

  // make a parabolic fit on the points, see fit.[cpp,h]
  offsetright=dataright[0];
  for (i=0;i<numofpoints;i++) {
    x[i]=i;
    dataright[i]=dataright[i]-offsetright;
  }
  polyfit(p,x,dataright,2,numofpoints);

  // plot fitted line
  memDC.SetPen(*wxRED);
  memDC.SetBrush(*wxRED_BRUSH);

  fitdata[0]=p[2]+offsetright;
  for (i=1;i<numofpoints;i++) {
    fitdata[i]=p[0]*x[i]*x[i]+p[1]*x[i]+p[2]+offsetright;

    memDC.DrawLine((fitdata[i]-min)*xstep,height-yvals[i]*ystep,\
		   (fitdata[i-1]-min)*xstep,height-yvals[i-1]*ystep);
  }
  memDC.SelectObject(wxNullBitmap);
  fitwindowright->SetBitmap(bmright);

  // calculate contact angle
  if (p[1]>0)
    ar=180-(atan(1/p[1])*180/PI);
  else
    ar=atan(-1/p[1])*180/PI;

  // cleanup
  delete[] dataleft;
  delete[] dataright;

  // ### plot lines showing the contact angles

  cv::Point2i basepointleft(offsetleft, baseleft);
  cv::Point2i basepointright(offsetright, baseright);
  cv::Point2i endpoint;

  cv::line(frame,basepointleft,basepointright,CV_RGB(255,255,0));

  // draw lines showing the contact angles
  if (al>90.0) {
    endpoint.x=basepointleft.x-100;
    endpoint.y=basepointleft.y-100*tan((180-al)/180*PI);
    cv::line(frame, basepointleft, endpoint, CV_RGB(0,255,0));
  }
  else if (al<90.0) {
    endpoint.x=basepointleft.x+100;
    endpoint.y=basepointleft.y-100*tan((al)/180*PI);
    cv::line(frame, basepointleft, endpoint, CV_RGB(0,255,0));
  }
  else {
    endpoint.x=basepointleft.x;
    endpoint.y=basepointleft.y+50;
    cv::line(frame, basepointleft, endpoint, CV_RGB(0,255,0));
  }

  if (ar>90.0) {
    endpoint.x=basepointright.x+100;
    endpoint.y=basepointright.y-100*tan((180-ar)/180*PI);
    cv::line(frame, basepointright, endpoint, CV_RGB(0,255,0));
  }
  else if (ar<90.0) {
    endpoint.x=basepointright.x-100;
    endpoint.y=basepointright.y-100*tan((ar)/180*PI);
    cv::line(frame, basepointright, endpoint, CV_RGB(0,255,0));
  }
  else {
    endpoint.x=basepointright.x;
    endpoint.y=basepointright.y+50;
    cv::line(frame, basepointright, endpoint, CV_RGB(0,255,0));
  }

  // finally, also print the contact angle as a number
  char posstring[7];
  sprintf(posstring,"%.2f", al);
  cv::Point labelposleft(basepointleft.x-70, basepointleft.y);
  cv::putText(frame, posstring, labelposleft, \
	      cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,255,0));
  sprintf(posstring,"%.2f", ar);
  cv::Point labelposright(basepointright.x+15, basepointright.y);
  cv::putText(frame, posstring, labelposright, \
	      cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,255,0));

  // show the droplet image
  wxImage plot(fwidth, fheight, (uchar*) frame.data, true);

  int plotwindowwidth, plotwindowheight;
  plotwindow->GetClientSize(&plotwindowwidth, &plotwindowheight);
  wxBitmap bm(plot.Scale(plotwindowwidth, plotwindowheight));
    
  plotwindow->SetBitmap(bm);
  plotwindow->Refresh();

  return 0;
}
