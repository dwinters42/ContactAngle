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

#include "config.h"

#include <wx/wxprec.h>
#include <wx/cmdline.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/image.h>
#endif

#include "MainFrame.h"

class ContactAngleApp: public wxApp {

public:
  bool OnInit();
private:
  bool loadFile, verbose;
  wxString infile;
};

IMPLEMENT_APP(ContactAngleApp)

bool ContactAngleApp::OnInit()
{
  /* parse the command line */
  wxCmdLineEntryDesc desc[] =   {
#ifdef _WIN32
    {wxCMD_LINE_SWITCH, "h", "help", "Show this help message",
     wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    {wxCMD_LINE_SWITCH, "v", "verbose", "Verbose output"},
    {wxCMD_LINE_OPTION, "f", "file", "Input file to process",
     wxCMD_LINE_VAL_STRING},
    {wxCMD_LINE_NONE}
#else
    {wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("Show this help message"),
     wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    {wxCMD_LINE_SWITCH, wxT("v"), wxT("verbose"), wxT("Verbose output")},
    {wxCMD_LINE_OPTION, wxT("f"), wxT("file"), wxT("Input file to process"),
     wxCMD_LINE_VAL_STRING},
    {wxCMD_LINE_NONE}
#endif
  };
  wxCmdLineParser parser (desc, argc, argv);
  if (parser.Parse() !=0)
    return false;

  verbose = parser.Found(wxT("v"));
  loadFile = parser.Found(wxT("f"),&infile);

  /* display main frame and run main loop */
  wxInitAllImageHandlers();
  MainFrame* frame = new MainFrame(NULL, wxID_ANY, wxEmptyString);
  SetTopWindow(frame);
  frame->Show();
  if (loadFile)
    frame->_loadFile(infile);
  return true;
}
