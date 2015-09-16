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

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"
 
#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include <wx/dir.h>
#include "wx/treectrl.h"
#include "wx/aui/aui.h"

#include "platform/event.h"

// UI generated from wxFormBuilder
#include "../Torque6EditorUI.h"

#include "projectManager.h"

ProjectManager::ProjectManager()
   :  mManager(NULL),
      mWindow(NULL),
      mProjectLoaded( false ), 
      mProjectName(""),
      mProjectPath(""),
      mTorque6Library(NULL),
      mTorque6Init(NULL),
      mTorque6Shutdown(NULL),
      mEditorOverlayView(NULL),
      mEditorMode(0)
{
   mCameraPanVelocity = Point3F::Zero;
}

ProjectManager::~ProjectManager()
{

}

void ProjectManager::init(wxAuiManager* manager, wxWindow* window)
{
   mManager = manager;
   mWindow = window;

   // Events
   mWindow->Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(ProjectManager::OnIdle), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_SIZE, wxSizeEventHandler(ProjectManager::OnSize), NULL, this);

   // Mouse Events
   mWindow->Connect(wxID_ANY, wxEVT_MOTION, wxMouseEventHandler(ProjectManager::OnMouseMove), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_LEFT_DOWN, wxMouseEventHandler(ProjectManager::OnMouseLeftDown), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(ProjectManager::OnMouseLeftUp), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_RIGHT_DOWN, wxMouseEventHandler(ProjectManager::OnMouseRightDown), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_RIGHT_UP, wxMouseEventHandler(ProjectManager::OnMouseRightUp), NULL, this);

   // Keyboard Events
   mWindow->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(ProjectManager::OnKeyDown), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_KEY_UP, wxKeyEventHandler(ProjectManager::OnKeyUp), NULL, this);
}

bool ProjectManager::openProject(wxString projectPath)
{
   // Load Torque 6 DLL
   if ( mTorque6Library == NULL )
   {
      mTorque6Library = openLibrary("Torque6_DEBUG");

      // Load Nessicary Functions
      mTorque6Init      = (initFunc)getLibraryFunc(mTorque6Library, "winInit");
      mTorque6Shutdown  = (shutdownFunc)getLibraryFunc(mTorque6Library, "winDestroy");
   }

   // If successful, initialize.
   if ( mTorque6Library && mTorque6Init && mTorque6Shutdown )
   {
      const char* argv[3];
      argv[0] = "Torque6Editor.exe";
      argv[1] = "-project";
      argv[2] = projectPath;

      mTorque6Init(3, argv, (HWND)mWindow->GetHWND());

      mProjectLoaded = true;
      mProjectPath = projectPath;
      wxDir projectDir(mProjectPath);
      mProjectName = projectDir.GetName();
      setRendering(true);

      // Editor Overlay
      mEditorOverlayView = Plugins::Link.Graphics.getView("EditorOverlay", 6100);

      // Editor Camera
      Scene::SceneCamera* activeCam = Plugins::Link.Scene.getActiveCamera();
      mCamera.copy(activeCam);
      mCamera.setBindMouse(true, false, true);
      Plugins::Link.Scene.addCamera("EditorCamera", &mCamera);
      Plugins::Link.Scene.pushActiveCamera("EditorCamera");

      onProjectLoaded(mProjectName, projectPath);
      return true;
   }

   return false;
}

void ProjectManager::closeProject()
{
   setRendering(false);

   mProjectLoaded = false;
   mProjectPath = "";
   mProjectName = "";

   mTorque6Shutdown();
   //FreeLibrary(T6_lib);

   onProjectClosed();
}

void ProjectManager::OnIdle(wxIdleEvent& evt)
{
   if (mProjectLoaded)
   {
      Plugins::Link.Engine.mainLoop();
      evt.RequestMore();
   }
}

void ProjectManager::OnSize(wxSizeEvent& evt)
{
   if (!mProjectLoaded)
      return;

   Plugins::Link.Engine.resizeWindow(evt.GetSize().GetX(), evt.GetSize().GetY());
}

void ProjectManager::OnMouseMove(wxMouseEvent& evt)
{
   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < smProjectTools.size(); ++i)
      smProjectTools[i]->onMouseMove(evt.GetPosition().x, evt.GetPosition().y);

   Plugins::Link.Engine.mouseMove(evt.GetPosition().x, evt.GetPosition().y);
}
void ProjectManager::OnMouseLeftDown(wxMouseEvent& evt)
{
   mWindow->SetFocus();

   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < smProjectTools.size(); ++i)
      smProjectTools[i]->onMouseLeftDown(evt.GetPosition().x, evt.GetPosition().y);

   Plugins::Link.Engine.mouseButton(true, true);
}
void ProjectManager::OnMouseLeftUp(wxMouseEvent& evt)
{
   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < smProjectTools.size(); ++i)
      smProjectTools[i]->onMouseLeftUp(evt.GetPosition().x, evt.GetPosition().y);

   Plugins::Link.Engine.mouseButton(false, true);
}
void ProjectManager::OnMouseRightDown(wxMouseEvent& evt)
{
   mWindow->SetFocus();

   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < smProjectTools.size(); ++i)
      smProjectTools[i]->onMouseRightDown(evt.GetPosition().x, evt.GetPosition().y);

   Plugins::Link.Engine.mouseButton(true, false);
}
void ProjectManager::OnMouseRightUp(wxMouseEvent& evt)
{
   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < smProjectTools.size(); ++i)
      smProjectTools[i]->onMouseRightUp(evt.GetPosition().x, evt.GetPosition().y);

   Plugins::Link.Engine.mouseButton(false, false);
}

KeyCodes getTorqueKeyCode(int key)
{
   switch(key)
   {
      case 65 : return KEY_A;
      case 66 : return KEY_B;
      case 67 : return KEY_C;
      case 68 : return KEY_D;
      case 69 : return KEY_E;
      case 70 : return KEY_F;
      case 71 : return KEY_G;
      case 72 : return KEY_H;
      case 73 : return KEY_I;
      case 74 : return KEY_J;
      case 75 : return KEY_K;
      case 76 : return KEY_L;
      case 77 : return KEY_M;
      case 78 : return KEY_N;
      case 79 : return KEY_O;
      case 80 : return KEY_P;
      case 81 : return KEY_Q;
      case 82 : return KEY_R;
      case 83 : return KEY_S;
      case 84 : return KEY_T;
      case 85 : return KEY_U;
      case 86 : return KEY_V;
      case 87 : return KEY_W;
      case 88 : return KEY_X;
      case 89 : return KEY_Y;
      case 90 : return KEY_Z;
   }

   return KEY_NULL;
}

void ProjectManager::OnKeyDown(wxKeyEvent& evt)
{
   if (!mProjectLoaded)
      return;

   switch (evt.GetKeyCode())
   {
      case 87: // W
         mCameraPanVelocity.z = -1.0;
         break;

      case 65: // A
         mCameraPanVelocity.x = 1.0;
         break;

      case 83: // S
         mCameraPanVelocity.z = 1.0;
         break;

      case 68: // D
         mCameraPanVelocity.x = -1.0;
         break;
   }
   mCamera.setPanVelocity(mCameraPanVelocity);

   KeyCodes torqueKey = getTorqueKeyCode(evt.GetKeyCode());
   Plugins::Link.Engine.keyDown(torqueKey);
}
void ProjectManager::OnKeyUp(wxKeyEvent& evt)
{
   if (!mProjectLoaded)
      return;

   switch (evt.GetKeyCode())
   {
      case 87: // W
         mCameraPanVelocity.z = 0.0;
         break;

      case 65: // A
         mCameraPanVelocity.x = 0.0;
         break;

      case 83: // S
         mCameraPanVelocity.z = 0.0;
         break;

      case 68: // D
         mCameraPanVelocity.x = 0.0;
         break;
   }
   mCamera.setPanVelocity(mCameraPanVelocity);

   KeyCodes torqueKey = getTorqueKeyCode(evt.GetKeyCode());
   Plugins::Link.Engine.keyUp(torqueKey);
}

void ProjectManager::preRender()
{
   Plugins::Link.bgfx.setViewRect(mEditorOverlayView->id, 0, 0, *Plugins::Link.Rendering.canvasWidth, *Plugins::Link.Rendering.canvasHeight);
   Plugins::Link.bgfx.setViewTransform(mEditorOverlayView->id, Plugins::Link.Rendering.viewMatrix, Plugins::Link.Rendering.projectionMatrix, BGFX_VIEW_STEREO, NULL);
}

void ProjectManager::render()
{
   for(unsigned int i = 0; i < smProjectTools.size(); ++i)
      smProjectTools[i]->renderTool();
}

void ProjectManager::postRender()
{

}

// Project Tool Management
wxVector<ProjectTool*> ProjectManager::smProjectTools;

ProjectTool::ProjectTool()
   :  mOpen(false),
      mProjectManager(NULL),
      mFrame(NULL),
      mManager(NULL)
{
   ProjectManager::smProjectTools.push_back(this);
}

void ProjectTool::init(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager)
{
   mProjectManager   = _projectManager;
   mFrame            = _frame;
   mManager          = _manager;
   initTool();
}

void ProjectManager::onProjectLoaded(wxString projectName, wxString projectPath)
{
   for(unsigned int i = 0; i < smProjectTools.size(); ++i)
      smProjectTools[i]->onProjectLoaded(projectName, projectPath);
}

void ProjectManager::onProjectClosed()
{
   for(unsigned int i = 0; i < smProjectTools.size(); ++i)
      smProjectTools[i]->onProjectClosed();
}

// ---------------------------------------------------------------

IMPLEMENT_PLUGIN_CONOBJECT(EditorCamera);

EditorCamera::EditorCamera()
{

}

void EditorCamera::onMouseMoveEvent(const GuiEvent &event)
{
   //editorList[activeEditorIndex]->onMouseMoveEvent(event);
}

void EditorCamera::onMouseDownEvent(const GuiEvent &event)
{
   //editorList[activeEditorIndex]->onMouseDownEvent(event);
}

void EditorCamera::onMouseDraggedEvent(const GuiEvent &event)
{
   //editorList[activeEditorIndex]->onMouseDraggedEvent(event);
}