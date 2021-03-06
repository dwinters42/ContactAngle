// Copyright (c) 2010,2011 Daniel Winters <daniel@tydirium.org>

// Permission to use, copy, modify, and distribute this software for
// any purpose with or without fee is hereby granted, provided that
// the above copyright notice and this permission notice appear in all
// copies.

// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
// CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
// OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
// NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
// CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wxprec.h>
#include <wx/busyinfo.h>
#include <wx/artprov.h>
#include <wx/statline.h>
#include <wx/filename.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/dynarray.h>
#endif

#ifdef _WIN32
#include "opencv/cv.h"
#include "opencv/highgui.h"
#else
#include "cv.h"
#include "highgui.h"
#endif

class MainFrame: public wxFrame {
 public:
  MainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

 private:
  wxFileName filename;
  cv::VideoCapture cap;
  int threshold;
  int framenum;
  int baseleft;
  int baseright;
  double tilt;
  int basepointx, basepointy;
  int fwidth;
  int fheight;
  int numframes;
  double al, ar;

  bool dataloaded;
  enum {ID_sliderFramenum=wxID_HIGHEST + 1, ID_sliderThres, ID_sliderLeft, \
	ID_sliderRight, ID_processAll};

 protected:
  wxPanel* panel;
  wxMenuBar* menubar;
  wxStatusBar* statusbar;
  wxToolBar* toolbar;
  wxStaticBitmap* plotwindow;
  wxStaticBitmap* fitwindowleft;
  wxStaticBitmap* fitwindowright;
  wxStaticLine* static_line_1;
  wxStaticText* label_1;
  wxSlider* sliderFramenum;
  wxStaticText* label_2;
  wxSlider* sliderThres;
  wxStaticText* label_3;
  wxSlider* sliderFitpoints;
  wxStaticText* label_4;
  wxSlider* sliderLeft;
  wxStaticText* label_5;
  wxSlider* sliderRight;

  DECLARE_EVENT_TABLE();

 public:
  virtual void loadFile(wxCommandEvent &event);
  virtual void onExit(wxCommandEvent &event);
  virtual void onAbout(wxCommandEvent &event);
  virtual void processAll(wxCommandEvent &event);
  virtual void onScroll(wxScrollEvent &event);
  virtual void onSize(wxSizeEvent &event);
  int _loadFile(wxFileName filename);
  int _process(void);
};

#endif // MAINFRAME_H
