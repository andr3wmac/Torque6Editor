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

#include "tools/transform//transformTool.h"

#include "windows/project/projectWindow.h"
#include "windows/console/consoleWindow.h"
#include "windows/materials/materialsWindow.h"
#include "windows/profiler/profilerWindow.h"
#include "windows/scene/sceneWindow.h"
#include "windows/scripts/scriptsWindow.h"
 
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

   // Editor Manager
   mEditorManager.init(runPath, mManager, mFrame, mFrame->mainPanel);

   // Tools
   EditorTool::smEditorTools.push_back(new TransformTool(&mEditorManager, mFrame, mManager));

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->initTool();

   // Windows
   EditorWindow::smEditorWindows.push_back(new ProjectWindow(&mEditorManager, mFrame, mManager));
   EditorWindow::smEditorWindows.push_back(new ConsoleWindow(&mEditorManager, mFrame, mManager));
   EditorWindow::smEditorWindows.push_back(new MaterialsWindow(&mEditorManager, mFrame, mManager));
   EditorWindow::smEditorWindows.push_back(new ProfilerWindow(&mEditorManager, mFrame, mManager));
   EditorWindow::smEditorWindows.push_back(new SceneWindow(&mEditorManager, mFrame, mManager));
   EditorWindow::smEditorWindows.push_back(new ScriptsWindow(&mEditorManager, mFrame, mManager));

   for (unsigned int i = 0; i < EditorWindow::smEditorWindows.size(); ++i)
      EditorWindow::smEditorWindows[i]->initWindow();

   // Specified project.
   if (argc == 3)
   {
      wxString command = argv[1];
      if (command == "-project")
      {
         wxString path = argv[2];
         mEditorManager.openProject(path);
         mManager->Update();
      }
   }
   else {
      wxDirDialog openFolder(mFrame, wxT("Select Project Folder"), wxGetCwd());
      if (openFolder.ShowModal() == wxID_OK)
         mEditorManager.openProject(openFolder.GetPath());

      mManager->Update();
   }

	return true;
}

Torque6Editor::~Torque6Editor()
{
   for (unsigned int i = 0; i < EditorWindow::smEditorWindows.size(); ++i)
   {
      EditorWindow* window = EditorWindow::smEditorWindows[i];
      window->destroyWindow();
      //delete tool;
   }

   // Dialogs
   //mAboutDialog->Destroy();

   mEditorManager.closeProject();
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
         mEditorManager.mEditorCameraSpeed = 0.1f;
         break;

      case MENU_CAMERA_NORMAL:
         mEditorManager.mEditorCameraSpeed = 0.5f;
         break;

      case MENU_CAMERA_FAST:
         mEditorManager.mEditorCameraSpeed = 1.0f;
         break;

      case MENU_PROJECT:
         EditorWindow::smEditorWindows[0]->openWindow();
         break;

      case MENU_CONSOLE:
         EditorWindow::smEditorWindows[1]->openWindow();
         break;

      case MENU_MATERIALS:
         EditorWindow::smEditorWindows[2]->openWindow();
         break;

      case MENU_SCENE:
         EditorWindow::smEditorWindows[4]->openWindow();
         break;

      case MENU_SCRIPTS:
         EditorWindow::smEditorWindows[5]->openWindow();
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