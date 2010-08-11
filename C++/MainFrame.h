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

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/statline.h>
#include <wx/artprov.h>
#include <wx/aboutdlg.h>

#include "cv.h"
#include "highgui.h"

class MainFrame: public wxFrame {
 public:
  MainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

 private:
  wxString filename;
  cv::VideoCapture cap;
  bool fileokay;
  int threshold;
  int framenum;
  int baseleft;
  int baseright;
  double tilt;
  int basepointx, basepointy;
  int fwidth;
  int fheight;
  int numframes;

  enum {ID_sliderFramenum=wxID_HIGHEST + 1, ID_sliderThres, ID_sliderLeft, \
	ID_sliderRight};

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
  virtual void onAbout(wxCommandEvent &event);
  virtual void process(wxScrollEvent &event);
};

#endif // MAINFRAME_H
