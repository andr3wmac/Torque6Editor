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

#include "main.h"

#include <wx/dir.h>
#include <wx/utils.h> 
#include <wx/stdpaths.h>
#include "wx/treectrl.h"
#include "wx/aui/aui.h"
#include <wx/tokenzr.h>
 
IMPLEMENT_APP(Torque6Launcher)

// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool Torque6Launcher::OnInit()
{
   wxInitAllImageHandlers();
   mProjectPath = "";

   mRunPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
#ifdef TORQUE_DEBUG
   mRunPath.Append("/Torque6App_DEBUG.exe");
#else
   mRunPath.Append("/Torque6App.exe");
#endif

   mEditorPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
#ifdef TORQUE_DEBUG
   mEditorPath.Append("/Torque6Editor_DEBUG.exe");
#else
   mEditorPath.Append("/Torque6Editor.exe");
#endif

	mFrame = new MainFrame((wxFrame*) NULL, -1, _T("Torque 6 Launcher"));

   mProjectBrowser = wxWebView::New(mFrame->projectsPanel, wxID_ANY);
   mFrame->projectSizer->Add(mProjectBrowser, 1, wxEXPAND | wxALL, 0);
   mFrame->projectsPanel->SetSizerAndFit(mFrame->projectSizer);

   // Frame Events
   mFrame->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Torque6Launcher::OnMenuEvent), NULL, this);
   mFrame->Connect(wxID_ANY, wxEVT_WEBVIEW_NAVIGATING, wxWebViewEventHandler(Torque6Launcher::OnNavigationRequest), NULL, this);

   mFrame->Show(true);
   SetTopWindow(mFrame);

   loadProjects("engine/projects");
	return true;
}

Torque6Launcher::~Torque6Launcher()
{

}

void Torque6Launcher::OnMenuEvent(wxCommandEvent& evt)
{

}

void Torque6Launcher::OnNavigationRequest(wxWebViewEvent& evt)
{
   wxString url = evt.GetURL();

   // Run
   if (url.StartsWith("run:"))
   {
      mProjectPath = url.Mid(4);
      runProject();
      evt.Veto();
      return;
   }

   // Editor
   if (url.StartsWith("editor:"))
   {
      mProjectPath = url.Mid(7);
      openEditor();
      evt.Veto();
      return;
   }
}

void Torque6Launcher::loadProjects(wxString projectsPath)
{
   wxString defaultScreenshotPath = wxGetCwd();
   defaultScreenshotPath.Append("/images/screenshot.png");

   wxString htmlBody("");
   htmlBody.Append("<head><style>");
   htmlBody.Append("body { background-color: #323232; color: #bfc9d3;  }");
   htmlBody.Append("a { color: #bfc9d3; text-decoration: none; }");
   htmlBody.Append("a:hover { color: #ffffff; text-decoration: none; }");
   htmlBody.Append("table { margin-bottom: 10px; font-family: 'Lato', sans-serif; font-weight: 400; font-size: 14px; width:100%; background-color: #1C1C1C; \
                     border: 1px solid #000000; box-shadow: 5px 5px 2px #000000; filter: progid:DXImageTransform.Microsoft.Shadow(color='#000000', Direction=135, Strength=5); }");
   htmlBody.Append("h2 { font-family: 'Raleway', sans-serif; margin-top: 10px; color: #FFFFFF; border-bottom: 2px solid #ffffff; }");
   htmlBody.Append(".description {  }");
   htmlBody.Append(".actionbar { float: left; margin-top: 90px; }");
   htmlBody.Append("</style></head>");
   htmlBody.Append("<body>");

   wxDir projectDir(projectsPath);
   if (!projectDir.IsOpened())
      return;

   wxString filename;
   bool cont = projectDir.GetFirst(&filename);
   while (cont)
   {
      wxString path = "";
      path.Append(projectsPath);
      path.Append("/");
      path.Append(filename);

      // Check if this is a directory.
      if (wxFileName::DirExists(path))
      {
         wxString mainCSPath = "";
         mainCSPath.Append(path);
         mainCSPath.Append("/main.cs");

         // If it has a main.cs then we consider it a project.
         if ( wxFileName::FileExists(mainCSPath) )
         {
            // Check if project has a screenshot.
            wxString screenshotPath = wxGetCwd();
            screenshotPath.Append("/");
            screenshotPath.Append(path);
            screenshotPath.Append("/screenshot.png");
            if (!wxFileName::FileExists(screenshotPath))
               screenshotPath = defaultScreenshotPath;

            // Start Table
            htmlBody.Append("<table><tr>");

            // Screenshot
            htmlBody.Append("<td width=260><img width=250 height=200 src='");
            htmlBody.Append(screenshotPath);
            htmlBody.Append("'/></td>");

            // Title
            htmlBody.Append("<td valign='top'><h2>");
            htmlBody.Append(filename);
            htmlBody.Append("</h2>");

            // Description
            htmlBody.Append("<div class='description'>Project Description</ddiv>");

            wxString projectPath = wxGetCwd();
            projectPath.Append("/");
            projectPath.Append(path);

            // Run Project Button
            htmlBody.Append("<div class='actionbar'><a href='run:");
            htmlBody.Append(projectPath);
            htmlBody.Append("'>Run</a> | ");

            // Open Editor Button
            htmlBody.Append("<a href='editor:");
            htmlBody.Append(projectPath);
            htmlBody.Append("'>Open Editor</a> | ");
            
            // Copy Project Button
            htmlBody.Append("<a href='copy:");
            htmlBody.Append(projectPath);
            htmlBody.Append("'>Copy</a> | ");

            // Delete Project Button
            htmlBody.Append("<a href='delete:");
            htmlBody.Append(projectPath);
            htmlBody.Append("'>Delete</a></div></td>");

            // End Table
            htmlBody.Append("</tr></table>");
         }
      }
      cont = projectDir.GetNext(&filename);
   }

   htmlBody.Append("</body>");
   mProjectBrowser->SetPage(htmlBody, "http://127.0.0.1");
}

void Torque6Launcher::runProject()
{
   wxString command = "";
   command.Append(mRunPath);
   command.Append(" -project ");
   command.Append(mProjectPath);
   wxExecute(command);
}

void Torque6Launcher::openEditor()
{
   wxString command = "";
   command.Append(mEditorPath);
   command.Append(" -project ");
   command.Append(mProjectPath);
   wxExecute(command);
}