//-----------------------------------------------------------------------------
// Copyright (c) 2016 Andrew Mac
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

#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H
 
#ifndef _PLUGINS_SHARED_H
#include <plugins/plugins_shared.h>
#endif

#ifndef _PLATFORM_LIBRARY_H_
#include "platform/platformLibrary.h"
#endif

#ifndef _WX_PROPGRID_PROPERTY_H_
#include "wx/propgrid/property.h"
#endif

#ifndef EDITORCAMERA_H
#include "editorCamera.h"
#endif

#ifndef EDITORTOOL_H
#include "editorTool.h"
#endif

#ifndef EDITORWINDOW_H
#include "editorWindow.h"
#endif

#ifndef _DEBUG_MODE_H_
#include "debug/debugMode.h"
#endif

typedef int (*initFunc)(int argc, const char **argv, HWND windowHWND);
typedef void (*shutdownFunc)();

class SelectMaterialDialog;
class wxTorqueAssetSelectDialog;
class wxTorqueAssetTree;
class wxWizard;

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

class wxWizardCallback : public wxObject
{
   public:
      wxWizard*   wizard;
};

class wxTorqueObjectEvent;
wxDECLARE_EVENT(wxTORQUE_SELECT_OBJECT, wxTorqueObjectEvent);

class wxTorqueObjectEvent : public wxEvent
{
   public:
      wxTorqueObjectEvent(int id = 0, wxEventType commandType = wxEVT_NULL)
         : wxEvent(id, commandType) 
      { 
         mSimObject     = NULL;
         mSceneObject   = NULL;
         mComponent     = NULL;
      }

      // You *must* copy here the data to be transported
      wxTorqueObjectEvent(const wxTorqueObjectEvent& event)
         : wxEvent(event) 
      {
         this->SetSimObject(event.GetSimObject());
         this->SetSceneObject(event.GetSceneObject());
         this->SetComponent(event.GetComponent());
      }

      // Required for sending with wxPostEvent()
      wxEvent* Clone() const { return new wxTorqueObjectEvent(*this); }

      SimObject* GetSimObject() const { return mSimObject; }
      void SetSimObject(SimObject* obj) { mSimObject = obj; }
      Scene::SceneObject* GetSceneObject() const { return mSceneObject; }
      void SetSceneObject(Scene::SceneObject* obj) { mSceneObject = obj; }
      Scene::BaseComponent* GetComponent() const { return mComponent; }
      void SetComponent(Scene::BaseComponent* comp) { mComponent = comp; }

   private:
      SimObject* mSimObject;
      Scene::SceneObject* mSceneObject;
      Scene::BaseComponent* mComponent;
};

typedef void (wxEvtHandler::*wxTorqueObjectEventFunction)(wxTorqueObjectEvent &);
#define wxTorqueObjectEventHandler(func) wxEVENT_HANDLER_CAST(wxTorqueObjectEventFunction, func)  

class EditorManager : public wxEvtHandler, public Debug::DebugMode
{
   public:
      EditorManager();
      ~EditorManager();

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
      Point3F           mEditorCameraForwardVelocity;
      F32               mEditorCameraSpeed;

      wxImageList*         mCommonIcons;
      Vector<ModuleInfo>   mModuleList;
      wxPGChoices          mTextureAssetChoices;

      // Dialogs
      wxTorqueAssetSelectDialog* mAssetSelectDialog;
      wxTorqueAssetSelectDialog* mMaterialSelectDialog;

      Graphics::ViewTableEntry* mRenderLayer4View;
      Graphics::ViewTableEntry* mEditorOverlayView;

      bool openProject(wxString projectPath);
      void closeProject();
      void runProject();
      void addObjectTemplateAsset(wxString assetID, Point3F position);
      void addMeshAsset(wxString assetID, Point3F position);

      void OnObjectSelected(wxTorqueObjectEvent& evt);

      // New Material Wizard
      bool newMaterialWizard(wxString& returnMaterialName, const char* moduleId = NULL);
      void OnNewMaterialSelectModule(wxCommandEvent& evt);

      // Material selection dialog
      bool selectMaterial(wxString& returnMaterialName, const char* defaultAsset = NULL);
      void OnNewMaterialButton(wxCommandEvent& evt);
      void OnOpenInMaterialEditorButton(wxCommandEvent& evt);

      bool selectAsset(wxString& returnValue, const char* filter = NULL, const char* defaultAsset = NULL);

      void refreshChoices();
      void refreshModuleList();
      Vector<ModuleInfo>* getModuleList();
      wxPGChoices* getTextureAssetChoices();

      virtual void render(Rendering::RenderCamera*);

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
 
#endif // EDITORMANAGER_H