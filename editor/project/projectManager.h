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

#ifndef _PROJECTMANAGER_H_
#define _PROJECTMANAGER_H_
 
#ifndef _PLUGINS_SHARED_H
#include <plugins/plugins_shared.h>
#endif

#ifndef _PLATFORM_LIBRARY_H_
#include "platform/platformLibrary.h"
#endif

#ifndef _WX_PROPGRID_PROPERTY_H_
#include "wx/propgrid/property.h"
#endif

#ifndef _RENDER_CAMERA_H_
#include "rendering/renderCamera.h"
#endif

#ifndef _CAMERA_COMPONENT_H_
#include "scene/components/cameraComponent.h"
#endif

typedef int (*initFunc)(int argc, const char **argv, HWND windowHWND);
typedef void (*shutdownFunc)();

class ProjectManager;
class MainFrame;
class wxAuiManager;

// Editor Camera
class EditorCamera : public Scene::CameraComponent
{
   private:
      typedef Scene::CameraComponent Parent;

   public:
      EditorCamera();

      void onMouseMoveEvent(const GuiEvent &event);
      void onMouseDownEvent(const GuiEvent &event);
      void onMouseDraggedEvent(const GuiEvent &event);

      DECLARE_PLUGIN_CONOBJECT(EditorCamera);
};

struct AssetCategoryInfo
{
   const char* categoryName;
   Vector<const AssetDefinition*> assets;
};

struct ModuleInfo
{
   const char* moduleID;
   U32 moduleVersion;
   Vector<AssetCategoryInfo> assets;
};

class EditorTool
{
   public:
      EditorTool(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager);

      bool              mOpen;
      ProjectManager*   mProjectManager;
      MainFrame*        mFrame;
      wxAuiManager*     mManager;

      virtual void initTool() { }
      virtual void destroyTool() { }
      virtual void openTool() { mOpen = true; }
      virtual void closeTool() { mOpen = false; }
      virtual void renderTool() { }
      virtual void onSceneChanged() { }
      virtual void onProjectLoaded(wxString projectName, wxString path) {}
      virtual void onProjectClosed() {}

      virtual bool onMouseLeftDown(int x, int y) { return false; }
      virtual bool onMouseLeftUp(int x, int y) { return false; }
      virtual bool onMouseRightDown(int x, int y) { return false; }
      virtual bool onMouseRightUp(int x, int y) { return false; }
      virtual bool onMouseMove(int x, int y) { return false; }
      
      static wxVector<EditorTool*> smEditorTools;
};

class ProjectManager : public wxEvtHandler, public Rendering::RenderHook
{
   public:
      ProjectManager();
      ~ProjectManager();

      void init(wxString runPath, wxAuiManager* manager, MainFrame* frame, wxWindow* window);
      bool isProjectLoaded() { return mProjectLoaded; }

      LIBRARY_HANDLE    mTorque6Library;
      initFunc          mTorque6Init;
      shutdownFunc      mTorque6Shutdown;

      wxString          mRunPath;

      bool              mProjectLoaded;
      wxString          mProjectName;
      wxString          mProjectPath;
      wxAuiManager*     mManager;
      MainFrame*        mFrame;
      wxWindow*         mWindow;
      EditorCamera      mEditorCamera;
      Point3F           mEditorCameraPanVelocity;
      F32               mEditorCameraSpeed;

      S32                  mEditorMode;
      Vector<ModuleInfo>   mModuleList;
      wxPGChoices          mTextureAssetChoices;

      Graphics::ViewTableEntry* mRenderLayer4View;
      Graphics::ViewTableEntry* mEditorOverlayView;

      bool openProject(wxString projectPath);
      void closeProject();
      void runProject();
      void addObjectTemplateAsset(wxString assetID, Point3F position);
      void addMeshAsset(wxString assetID, Point3F position);

      void refreshChoices();
      void refreshModuleList();
      Vector<ModuleInfo>* getModuleList();
      wxPGChoices* getTextureAssetChoices();

      virtual void onAddToCamera();
      virtual void onRemoveFromCamera();
      virtual void preRender();
      virtual void render();
      virtual void postRender();

      virtual void OnToolbarEvent(wxCommandEvent& evt);
      virtual void OnIdle(wxIdleEvent& evt);
      virtual void OnSize(wxSizeEvent& evt);
      virtual void OnMouseMove(wxMouseEvent& evt);
      virtual void OnMouseLeftDown(wxMouseEvent& evt);
      virtual void OnMouseLeftUp(wxMouseEvent& evt);
      virtual void OnMouseRightDown(wxMouseEvent& evt);
      virtual void OnMouseRightUp(wxMouseEvent& evt);
      virtual void OnKeyDown(wxKeyEvent& evt);
      virtual void OnKeyUp(wxKeyEvent& evt);

      static void onProjectLoaded(const wxString& projectName, const wxString& projectPath);
      static void onProjectClosed();
};
 
#endif // _PROJECTMANAGER_H_