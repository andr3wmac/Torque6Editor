//-----------------------------------------------------------------------------
// Copyright (c) 2015 Andrew Mac
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"
 
#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#ifndef _WX_WEBVIEW_H_
#include <wx/webview.h>
#endif

#ifndef __TORQUE6LAUNCHERUI_H__
#include "Torque6LauncherUI.h"
#endif

class Torque6Launcher : public wxApp
{
public:
   ~Torque6Launcher();

   MainFrame*  mFrame;
   wxWebView*  mProjectBrowser;
   wxString    mRunPath;
   wxString    mEditorPath;
   wxString    mProjectPath;

   void loadProjects(wxString projectsDir);
   void runProject();
   void openEditor();

   // Events
	virtual bool OnInit();
   virtual void OnMenuEvent( wxCommandEvent& evt );
   virtual void OnNavigationRequest(wxWebViewEvent& evt);
};
 
DECLARE_APP(Torque6Launcher)
 
#endif // _MAIN_H_