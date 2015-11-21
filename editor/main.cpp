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
 
IMPLEMENT_APP(Torque6Editor)

// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool Torque6Editor::OnInit()
{
   wxInitAllImageHandlers();

	mFrame = new MainFrame((wxFrame*) NULL, -1, _T("Torque 6 Editor"));
   mFrame->Show(true);
	SetTopWindow(mFrame);

   // Preload Dialogs
   mAboutDialog = new AboutDialog(mFrame);

   // Setup Advanced UI Manager
   mManager = &mFrame->m_mgr;

   // Frame Events
   mFrame->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Torque6Editor::OnMenuEvent), NULL, this);

   wxString runPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
#ifdef TORQUE_DEBUG
   runPath.Append("/Torque6App_DEBUG.exe");
#else
   runPath.Append("/Torque6App.exe");
#endif

   // Torque 6 Project Manager
   mProjectManager.init(runPath, mManager, mFrame, mFrame->mainPanel);

   // Tools
   EditorTool::smEditorTools.push_back(new ProjectTool(&mProjectManager, mFrame, mManager));
   EditorTool::smEditorTools.push_back(new ConsoleTool(&mProjectManager, mFrame, mManager));
   EditorTool::smEditorTools.push_back(new MaterialsTool(&mProjectManager, mFrame, mManager));
   EditorTool::smEditorTools.push_back(new ProfilerTool(&mProjectManager, mFrame, mManager));
   EditorTool::smEditorTools.push_back(new SceneTool(&mProjectManager, mFrame, mManager));
   EditorTool::smEditorTools.push_back(new ScriptsTool(&mProjectManager, mFrame, mManager));

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->initTool();

   // Specified project.
   if (argc == 3)
   {
      wxString command = argv[1];
      if (command == "-project")
      {
         wxString path = argv[2];
         mProjectManager.openProject(path);
         mManager->Update();
      }
   }
   else {
      wxDirDialog openFolder(mFrame, wxT("Select Project Folder"), wxGetCwd());
      if (openFolder.ShowModal() == wxID_OK)
         mProjectManager.openProject(openFolder.GetPath());

      mManager->Update();
   }

	return true;
}

Torque6Editor::~Torque6Editor()
{
   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
   {
      EditorTool* tool = EditorTool::smEditorTools[i];
      tool->destroyTool();
      //delete tool;
   }

   // Dialogs
   //mAboutDialog->Destroy();

   mProjectManager.closeProject();
}

void Torque6Editor::OnMenuEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case MENU_EXIT:
         mFrame->Close();
         break;

      case MENU_ABOUT:
         mAboutDialog->ShowModal();
         break;

      case MENU_CAMERA_SLOW:
         mProjectManager.mCameraSpeed = 0.1f;
         break;

      case MENU_CAMERA_NORMAL:
         mProjectManager.mCameraSpeed = 0.5f;
         break;

      case MENU_CAMERA_FAST:
         mProjectManager.mCameraSpeed = 1.0f;
         break;

      case MENU_PROJECT:
         EditorTool::smEditorTools[0]->openTool();
         break;

      case MENU_CONSOLE:
         EditorTool::smEditorTools[1]->openTool();
         break;

      case MENU_MATERIALS:
         EditorTool::smEditorTools[2]->openTool();
         break;

      case MENU_SCENE:
         EditorTool::smEditorTools[4]->openTool();
         break;

      case MENU_SCRIPTS:
         EditorTool::smEditorTools[5]->openTool();
         break;

		case MENU_REMOTERY:
			wxLaunchDefaultBrowser("file:///C:/Users/Andrew/Documents/Projects/Torque6Editor/profiler/index.html");
			break;

      case MENU_WEBSITE:
         wxLaunchDefaultBrowser("http://www.torque6.com");
         break;

      case MENU_GITHUB:
         wxLaunchDefaultBrowser("http://www.github.com/andr3wmac/Torque6");
         break;

      case MENU_FORUMS:
         wxLaunchDefaultBrowser("http://forums.torque3d.org/viewforum.php?f=32");
         break;

      default:
         break;
   }
}