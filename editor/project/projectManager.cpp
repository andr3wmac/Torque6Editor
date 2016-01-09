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
#include <wx/utils.h> 
#include "wx/treectrl.h"
#include "wx/aui/aui.h"
#include "wx/dnd.h"

#include "platform/event.h"

// UI generated from wxFormBuilder
#include "../Torque6EditorUI.h"

#include "projectManager.h"
#include "module/moduleManager.h"
#include "scene/components/meshComponent.h"
#include "rendering/renderCamera.h"

class TextDropTarget : public wxTextDropTarget
{
   virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text);
};

ProjectManager::ProjectManager()
   :  mManager(NULL),
      mFrame(NULL),
      mWindow(NULL),
      mProjectLoaded( false ), 
      mProjectName(""),
      mProjectPath(""),
      mTorque6Library(NULL),
      mTorque6Init(NULL),
      mTorque6Shutdown(NULL),
      mEditorOverlayView(NULL),
      mRenderLayer4View(NULL),
      mEditorMode(0)
{
   mEditorCameraPanVelocity = Point3F::Zero;
   mEditorCameraSpeed = 0.5f;
}

ProjectManager::~ProjectManager()
{
   closeProject();
   if (mTorque6Library != NULL)
      FreeLibrary(mTorque6Library);
}

void ProjectManager::init(wxString runPath, wxAuiManager* manager, MainFrame* frame, wxWindow* window)
{
   mRunPath = runPath;
   mManager = manager;
   mFrame   = frame;
   mWindow  = window;

   TextDropTarget* dropTarget = new TextDropTarget();
   mWindow->SetDropTarget(dropTarget);

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

   // Add Tools to toolabr
   mFrame->mainToolbar->AddTool(0, wxT("Run"), wxBitmap(wxT("images/run.png"), wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, wxT("Run"), wxEmptyString, NULL);
   mFrame->mainToolbar->AddSeparator();
   mFrame->mainToolbar->Realize();

   // Toolbar Events
   mFrame->mainToolbar->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(ProjectManager::OnToolbarEvent), NULL, this);
}

bool ProjectManager::openProject(wxString projectPath)
{
   // Load Torque 6 DLL
   if ( mTorque6Library == NULL )
   {
#ifdef TORQUE_DEBUG
      mTorque6Library = openLibrary("Torque6_DEBUG");
#else
      mTorque6Library = openLibrary("Torque6");
#endif

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

      // Editor Overlay
      mRenderLayer4View = Torque::Graphics.getView("RenderLayer4", 2000);
      mEditorOverlayView = Torque::Graphics.getView("EditorOverlay", 6100);

      // Editor Camera
      //Scene::SceneCamera* activeCam = Torque::Scene.getActiveCamera();
      //mEditorCamera.copy(activeCam);
      //mEditorCamera.setBindMouse(true, false, true);
      //Torque::Scene.addCamera("EditorCamera", &mEditorCamera);
      //Torque::Scene.pushActiveCamera("EditorCamera");

      onProjectLoaded(mProjectName, projectPath);
      return true;
   }

   return false;
}

void ProjectManager::closeProject()
{
   if (!mProjectLoaded) 
      return;

   mProjectLoaded = false;
   mProjectPath = "";
   mProjectName = "";

   mTorque6Shutdown();
   //FreeLibrary(T6_lib);

   onProjectClosed();
}

void ProjectManager::runProject()
{
   wxString command = "";
   command.Append(mRunPath);
   command.Append(" -project ");
   command.Append(mProjectPath);
   wxExecute(command);
}

void ProjectManager::OnToolbarEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 0:
         runProject();
         break;

      case 1:
         mEditorMode = 0;
         break;

      case 2:
         mEditorMode = 1;
         break;

      case 3:
         mEditorMode = 2;
         break;

      default:
         break;
   }
}

void ProjectManager::OnIdle(wxIdleEvent& evt)
{
   if (mProjectLoaded)
   {
      Torque::Engine.mainLoop();
      evt.RequestMore();
   }
}

void ProjectManager::OnSize(wxSizeEvent& evt)
{
   if (!mProjectLoaded)
      return;

   Torque::Engine.resizeWindow(evt.GetSize().GetX(), evt.GetSize().GetY());
}

void ProjectManager::OnMouseMove(wxMouseEvent& evt)
{
   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onMouseMove(evt.GetPosition().x, evt.GetPosition().y);

   Torque::Engine.mouseMove(evt.GetPosition().x, evt.GetPosition().y);
}
void ProjectManager::OnMouseLeftDown(wxMouseEvent& evt)
{
   mWindow->SetFocus();

   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onMouseLeftDown(evt.GetPosition().x, evt.GetPosition().y);

   Torque::Engine.mouseButton(true, true);
}
void ProjectManager::OnMouseLeftUp(wxMouseEvent& evt)
{
   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onMouseLeftUp(evt.GetPosition().x, evt.GetPosition().y);

   Torque::Engine.mouseButton(false, true);
}
void ProjectManager::OnMouseRightDown(wxMouseEvent& evt)
{
   mWindow->SetFocus();

   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onMouseRightDown(evt.GetPosition().x, evt.GetPosition().y);

   Torque::Engine.mouseButton(true, false);
}
void ProjectManager::OnMouseRightUp(wxMouseEvent& evt)
{
   if (!mProjectLoaded)
      return;

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onMouseRightUp(evt.GetPosition().x, evt.GetPosition().y);

   Torque::Engine.mouseButton(false, false);
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
         mEditorCameraPanVelocity.y = -1.0 * mEditorCameraSpeed;
         break;

      case 65: // A
         mEditorCameraPanVelocity.x = 1.0 * mEditorCameraSpeed;
         break;

      case 83: // S
         mEditorCameraPanVelocity.y = 1.0 * mEditorCameraSpeed;
         break;

      case 68: // D
         mEditorCameraPanVelocity.x = -1.0 * mEditorCameraSpeed;
         break;
   }
   mEditorCamera.setPanVelocity(mEditorCameraPanVelocity);

   KeyCodes torqueKey = getTorqueKeyCode(evt.GetKeyCode());
   Torque::Engine.keyDown(torqueKey);
}

void ProjectManager::OnKeyUp(wxKeyEvent& evt)
{
   if (!mProjectLoaded)
      return;

   switch (evt.GetKeyCode())
   {
      case 87: // W
         mEditorCameraPanVelocity.y = 0.0;
         break;

      case 65: // A
         mEditorCameraPanVelocity.x = 0.0;
         break;

      case 83: // S
         mEditorCameraPanVelocity.y = 0.0;
         break;

      case 68: // D
         mEditorCameraPanVelocity.x = 0.0;
         break;
   }
   mEditorCamera.setPanVelocity(mEditorCameraPanVelocity);

   KeyCodes torqueKey = getTorqueKeyCode(evt.GetKeyCode());
   Torque::Engine.keyUp(torqueKey);
}

void ProjectManager::onAddToCamera()
{

}

void ProjectManager::onRemoveFromCamera()
{

}

void ProjectManager::preRender()
{

}

void ProjectManager::render()
{
   Torque::bgfx.setViewRect(mEditorOverlayView->id, 0, 0, *Torque::Rendering.canvasWidth, *Torque::Rendering.canvasHeight);
   Torque::bgfx.setViewTransform(mEditorOverlayView->id, mCamera->viewMatrix, mCamera->projectionMatrix, BGFX_VIEW_STEREO, NULL);

   for(unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->renderTool();
}

void ProjectManager::postRender()
{

}

// Project Tool Management
wxVector<EditorTool*> EditorTool::smEditorTools;

EditorTool::EditorTool(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager)
   :  mOpen(false),
      mProjectManager(_projectManager),
      mFrame(_frame),
      mManager(_manager)
{
   //
}

void ProjectManager::onProjectLoaded(const wxString& projectName, const wxString& projectPath)
{
   for(unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onProjectLoaded(projectName, projectPath);
}

void ProjectManager::onProjectClosed()
{
   for(unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onProjectClosed();
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

void _addObjectTemplateAsset(wxString assetID, Point3F position)
{
   Scene::SceneObject* newObject = new Scene::SceneObject();
   newObject->setTemplateAsset(assetID);
   newObject->mPosition.set(position);
   Torque::Scene.addObject(newObject, "NewSceneObject");
   newObject->registerObject();
}

void _addMeshAsset(wxString assetID, Point3F position)
{
   Scene::SceneObject* newObject = new Scene::SceneObject();
   Scene::MeshComponent* meshComponent = new Scene::MeshComponent();
   meshComponent->setMesh(assetID.c_str());
   newObject->addComponent(meshComponent);
   newObject->mPosition.set(position);
   Torque::Scene.addObject(newObject, "NewSceneObject");
   newObject->registerObject();
   meshComponent->registerObject();
}

bool TextDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
   // Debug:
   Torque::Con.printf("DROP: x: %d y: %d text: %s", x, y, static_cast<const char*>(text));

   // Parse Commands
   wxStringTokenizer tokenizer(text, "->");
   wxString command = tokenizer.GetNextToken();

   // Asset->AssetType->AssetID
   if (command == "Asset" && tokenizer.HasMoreTokens())
   {
      tokenizer.GetNextToken();
      wxString assetType = tokenizer.GetNextToken();
      tokenizer.GetNextToken();
      wxString assetID = tokenizer.GetNextToken();

      // ObjectTemplateAsset gets added straight to the scene.
      if (assetType == "ObjectTemplateAsset")
      {
         Point3F worldRay = Torque::Rendering.screenToWorld(Point2I(x, y));
         //Point3F editorPos = Torque::Scene.getActiveCamera()->getPosition();
         //_addObjectTemplateAsset(assetID, editorPos + (worldRay * 10.0f));
      }

      // MeshAsset
      if (assetType == "MeshAsset")
      {
         Point3F worldRay = Torque::Rendering.screenToWorld(Point2I(x, y));
         //Point3F editorPos = Torque::Scene.getActiveCamera()->getPosition();
         //_addMeshAsset(assetID, editorPos + (worldRay * 10.0f));
      }

      // Inform the tools the scene has changed.
      for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
         EditorTool::smEditorTools[i]->onSceneChanged();
   }

   return true;
}

void ProjectManager::addObjectTemplateAsset(wxString assetID, Point3F position)
{
   _addObjectTemplateAsset(assetID, position);
}

void ProjectManager::addMeshAsset(wxString assetID, Point3F position)
{
   _addMeshAsset(assetID, position);
}

void ProjectManager::refreshChoices()
{
   if (!isProjectLoaded())
      return;

   mTextureAssetChoices.Clear();
   mTextureAssetChoices.Add("", 0);

   Vector<const AssetDefinition*> assetDefinitions = Torque::AssetDatabaseLink.getDeclaredAssets();

   // Iterate sorted asset definitions.
   for (Vector<const AssetDefinition*>::iterator assetItr = assetDefinitions.begin(); assetItr != assetDefinitions.end(); ++assetItr)
   {
      // Fetch asset definition.
      const AssetDefinition* pAssetDefinition = *assetItr;

      // Populate TextureAsset choices menu.
      if (dStrcmp(pAssetDefinition->mAssetType, "TextureAsset") == 0)
         mTextureAssetChoices.Add(pAssetDefinition->mAssetId, mTextureAssetChoices.GetCount());
   }
}

void ProjectManager::refreshModuleList()
{
   mModuleList.clear();
   Vector<const AssetDefinition*> assetDefinitions = Torque::AssetDatabaseLink.getDeclaredAssets();

   // Fetch all loaded module definitions.
   ModuleManager::typeConstModuleDefinitionVector loadedModules;
   Torque::ModuleDatabaseLink->findModules(true, loadedModules);

   // Iterate found loaded module definitions.
   for (ModuleManager::typeConstModuleDefinitionVector::const_iterator loadedModuleItr = loadedModules.begin(); loadedModuleItr != loadedModules.end(); ++loadedModuleItr)
   {
      // Fetch module definition.
      const ModuleDefinition* module = *loadedModuleItr;

      // Add to module list.
      ModuleInfo newModule;
      newModule.moduleID = module->getModuleId();
      newModule.moduleVersion = module->getVersionId();
      mModuleList.push_back(newModule);
   }

   // Iterate asset definitions.
   for (Vector<const AssetDefinition*>::iterator assetItr = assetDefinitions.begin(); assetItr != assetDefinitions.end(); ++assetItr)
   {
      // Fetch asset definition.
      const AssetDefinition* pAssetDefinition = *assetItr;

      char buf[256];
      dStrcpy(buf, pAssetDefinition->mAssetId);
      const char* moduleName = dStrtok(buf, ":");
      const char* assetName = dStrtok(NULL, ":");

      // Try to find module
      bool foundModule = false;
      for (Vector<ModuleInfo>::iterator modulesItr = mModuleList.begin(); modulesItr != mModuleList.end(); ++modulesItr)
      {
         const char* moduleID = pAssetDefinition->mpModuleDefinition->getModuleId();
         if (dStrcmp(modulesItr->moduleID, moduleID) == 0)
         {
            // Try to find category
            bool foundCategory = false;
            for (Vector<AssetCategoryInfo>::iterator categoriesItr = modulesItr->assets.begin(); categoriesItr != modulesItr->assets.end(); ++categoriesItr)
            {
               const char* moduleID = pAssetDefinition->mpModuleDefinition->getModuleId();
               if (dStrcmp(categoriesItr->categoryName, pAssetDefinition->mAssetType) == 0)
               {
                  categoriesItr->assets.push_back(pAssetDefinition);
                  foundCategory = true;
                  break;
               }
            }

            // Can't find module? Create one.
            if (!foundCategory)
            {
               AssetCategoryInfo newCategory;
               newCategory.categoryName = pAssetDefinition->mAssetType;
               newCategory.assets.push_back(pAssetDefinition);
               modulesItr->assets.push_back(newCategory);
            }

            foundModule = true;
            break;
         }
      }

      // Can't find module? Create one.
      if (!foundModule)
      {
         ModuleInfo newModule;
         newModule.moduleID = pAssetDefinition->mpModuleDefinition->getModuleId();
         newModule.moduleVersion = pAssetDefinition->mpModuleDefinition->getVersionId();

         AssetCategoryInfo newCategory;
         newCategory.categoryName = pAssetDefinition->mAssetType;
         newCategory.assets.push_back(pAssetDefinition);
         newModule.assets.push_back(newCategory);
         mModuleList.push_back(newModule);
      }
   }
}

Vector<ModuleInfo>* ProjectManager::getModuleList()
{
   refreshModuleList();
   return &mModuleList;
}

wxPGChoices* ProjectManager::getTextureAssetChoices()
{
   refreshChoices();
   return &mTextureAssetChoices;
}