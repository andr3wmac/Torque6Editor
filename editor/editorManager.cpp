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
#include "Torque6EditorUI.h"

#include "widgets/wxTorqueAssetBrowser/wxTorqueAssetSelectDialog.h"
#include "widgets/wxTorqueAssetBrowser/wxTorqueAssetTree.h"
#include "theme.h"

#include "editorManager.h"
#include "module/moduleManager.h"
#include "scene/components/meshComponent.h"
#include "rendering/renderCamera.h"

class TextDropTarget : public wxTextDropTarget
{
   virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text);
};

EditorManager::EditorManager()
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
   mEditorCameraForwardVelocity  = Point3F::Zero;
   mEditorCameraSpeed            = 0.5f;

   mCommonIcons = new wxImageList(16, 16);
}

EditorManager::~EditorManager()
{
   closeProject();
   if (mTorque6Library != NULL)
      FreeLibrary(mTorque6Library);
}

void EditorManager::init(wxString runPath, wxAuiManager* manager, MainFrame* frame, wxWindow* window)
{
   mRunPath = runPath;
   mManager = manager;
   mFrame   = frame;
   mWindow  = window;

   // Common Icons
   mCommonIcons->Add(wxBitmap("images/moduleIcon.png", wxBITMAP_TYPE_PNG));
   mCommonIcons->Add(wxBitmap("images/iconFolderGrey.png", wxBITMAP_TYPE_PNG));
   mCommonIcons->Add(wxBitmap("images/assetIcon.png", wxBITMAP_TYPE_PNG));

   TextDropTarget* dropTarget = new TextDropTarget();
   mWindow->SetDropTarget(dropTarget);

   // Events
   mWindow->Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(EditorManager::OnIdle), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_SIZE, wxSizeEventHandler(EditorManager::OnSize), NULL, this);

   // Mouse Events
   mWindow->Connect(wxID_ANY, wxEVT_MOTION, wxMouseEventHandler(EditorManager::OnMouseMove), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_LEFT_DOWN, wxMouseEventHandler(EditorManager::OnMouseLeftDown), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(EditorManager::OnMouseLeftUp), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_RIGHT_DOWN, wxMouseEventHandler(EditorManager::OnMouseRightDown), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_RIGHT_UP, wxMouseEventHandler(EditorManager::OnMouseRightUp), NULL, this);

   // Keyboard Events
   mWindow->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(EditorManager::OnKeyDown), NULL, this);
   mWindow->Connect(wxID_ANY, wxEVT_KEY_UP, wxKeyEventHandler(EditorManager::OnKeyUp), NULL, this);

   // Add Tools to toolabr
   mFrame->mainToolbar->AddTool(0, wxT("Run"), wxBitmap(wxT("images/run.png"), wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, wxT("Run"), wxEmptyString, NULL);
   mFrame->mainToolbar->AddSeparator();
   mFrame->mainToolbar->Realize();

   // Toolbar Events
   mFrame->mainToolbar->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditorManager::OnToolbarEvent), NULL, this);

   // Dialogs
   mAssetSelectDialog = new wxTorqueAssetSelectDialog(this, mWindow);
   mMaterialSelectDialog = new wxTorqueAssetSelectDialog(this, mWindow);
}

bool EditorManager::openProject(wxString projectPath)
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

      // Run a frame.
      Torque::Engine.mainLoop();

      // Editor Overlay
      Torque::Scene.pause();
      Torque::Debug.registerDebugMode("Editor", this);
      Torque::Debug.setDebugMode("Editor", true);

      // Editor Camera
      mEditorCamera.initialize(this);
      mRenderLayer4View = Torque::Graphics.getView("DeferredFinal", 1750, mEditorCamera.getRenderCamera());
      mEditorOverlayView = Torque::Graphics.getView("EditorOverlay", 6100, mEditorCamera.getRenderCamera());

      onProjectLoaded(mProjectName, projectPath);
      return true;
   }

   return false;
}

void EditorManager::closeProject()
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

void EditorManager::runProject()
{
   wxString command = "";
   command.Append(mRunPath);
   command.Append(" -project ");
   command.Append(mProjectPath);
   wxExecute(command);
}

void EditorManager::OnToolbarEvent(wxCommandEvent& evt)
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

void EditorManager::OnIdle(wxIdleEvent& evt)
{
   if (mProjectLoaded)
   {
      mEditorCamera.mainLoop();
      Torque::Engine.mainLoop();
      evt.RequestMore();
   }
}

void EditorManager::OnSize(wxSizeEvent& evt)
{
   if (!mProjectLoaded)
      return;

   Torque::Engine.resizeWindow(evt.GetSize().GetX(), evt.GetSize().GetY());
}

void EditorManager::OnMouseMove(wxMouseEvent& evt)
{
   if (!mProjectLoaded)
      return;

   mEditorCamera.onMouseMove(evt.GetPosition().x, evt.GetPosition().y);

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onMouseMove(evt.GetPosition().x, evt.GetPosition().y);

   Torque::Engine.mouseMove(evt.GetPosition().x, evt.GetPosition().y);
}
void EditorManager::OnMouseLeftDown(wxMouseEvent& evt)
{
   mWindow->SetFocus();

   if (!mProjectLoaded)
      return;

   mEditorCamera.onMouseLeftDown(evt.GetPosition().x, evt.GetPosition().y);

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onMouseLeftDown(evt.GetPosition().x, evt.GetPosition().y);

   Torque::Engine.mouseButton(true, true);
}
void EditorManager::OnMouseLeftUp(wxMouseEvent& evt)
{
   if (!mProjectLoaded)
      return;

   mEditorCamera.onMouseLeftUp(evt.GetPosition().x, evt.GetPosition().y);

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onMouseLeftUp(evt.GetPosition().x, evt.GetPosition().y);

   Torque::Engine.mouseButton(false, true);
}
void EditorManager::OnMouseRightDown(wxMouseEvent& evt)
{
   mWindow->SetFocus();

   if (!mProjectLoaded)
      return;

   mEditorCamera.onMouseRightDown(evt.GetPosition().x, evt.GetPosition().y);

   for (unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onMouseRightDown(evt.GetPosition().x, evt.GetPosition().y);

   Torque::Engine.mouseButton(true, false);
}
void EditorManager::OnMouseRightUp(wxMouseEvent& evt)
{
   if (!mProjectLoaded)
      return;

   mEditorCamera.onMouseRightUp(evt.GetPosition().x, evt.GetPosition().y);

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

void EditorManager::OnKeyDown(wxKeyEvent& evt)
{
   if (!mProjectLoaded)
      return;

   switch (evt.GetKeyCode())
   {
      case 87: // W
         mEditorCameraForwardVelocity.x = 1.0 * mEditorCameraSpeed;
         break;

      case 65: // A
         mEditorCameraForwardVelocity.y = 1.0 * mEditorCameraSpeed;
         break;

      case 83: // S
         mEditorCameraForwardVelocity.x = -1.0 * mEditorCameraSpeed;
         break;

      case 68: // D
         mEditorCameraForwardVelocity.y = -1.0 * mEditorCameraSpeed;
         break;
   }
   mEditorCamera.setForwardVelocity(mEditorCameraForwardVelocity);

   KeyCodes torqueKey = getTorqueKeyCode(evt.GetKeyCode());
   Torque::Engine.keyDown(torqueKey);
}

void EditorManager::OnKeyUp(wxKeyEvent& evt)
{
   if (!mProjectLoaded)
      return;

   switch (evt.GetKeyCode())
   {
      case 87: // W
         mEditorCameraForwardVelocity.x = 0.0;
         break;

      case 65: // A
         mEditorCameraForwardVelocity.y = 0.0;
         break;

      case 83: // S
         mEditorCameraForwardVelocity.x = 0.0;
         break;

      case 68: // D
         mEditorCameraForwardVelocity.y = 0.0;
         break;
   }
   mEditorCamera.setForwardVelocity(mEditorCameraForwardVelocity);

   KeyCodes torqueKey = getTorqueKeyCode(evt.GetKeyCode());
   Torque::Engine.keyUp(torqueKey);
}

void EditorManager::render(Rendering::RenderCamera* camera)
{
   //Torque::bgfx.setViewRect(mEditorOverlayView->id, 0, 0, *Torque::Rendering.windowWidth, *Torque::Rendering.windowHeight);
   //Torque::bgfx.setViewTransform(mEditorOverlayView->id, camera->viewMatrix, camera->projectionMatrix, BGFX_VIEW_STEREO, NULL);

   for(unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->renderTool();
}

// Project Tool Management
wxVector<EditorTool*> EditorTool::smEditorTools;

EditorTool::EditorTool(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager)
   :  mOpen(false),
      mEditorManager(_EditorManager),
      mFrame(_frame),
      mManager(_manager)
{
   //
}

void EditorManager::onProjectLoaded(const wxString& projectName, const wxString& projectPath)
{
   for(unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onProjectLoaded(projectName, projectPath);
}

void EditorManager::onProjectClosed()
{
   for(unsigned int i = 0; i < EditorTool::smEditorTools.size(); ++i)
      EditorTool::smEditorTools[i]->onProjectClosed();
}

// ---------------------------------------------------------------

EditorCamera::EditorCamera()
{
   mRenderCamera     = NULL;
   mForwardVelocity  = Point3F::Zero;
   mWorldPosition    = Point3F(0.0f, 0.0f, 0.0f);
   mMouseDown        = false;
   mMouseStart       = Point2I(0, 0);
   mHorizontalAngle  = 0.0f;
   mVerticalAngle    = 0.0f;

   mTransform.set(Point3F(0.0f, 0.0f, 0.0f), VectorF(0.0f, 0.0f, 0.0f), VectorF(1.0f, 1.0f, 1.0f));
}

void EditorCamera::initialize(EditorManager* EditorManager)
{
   mEditorManager   = EditorManager;
   mRenderCamera     = Torque::Rendering.getPriorityRenderCamera();
}

void EditorCamera::mainLoop()
{
   if (mRenderCamera == NULL)
      return;

   mVerticalAngle = mClampF(mVerticalAngle, -4.7f, -1.7f);
   VectorF rotation = mTransform.getRotationEuler();
   rotation.y = mVerticalAngle;
   rotation.z = mHorizontalAngle;
   mTransform.setRotation(rotation);

   VectorF up(0.0f, 0.0f, 1.0f);
   Point3F look;
   Point3F cameraForward(1.0f, 0.0f, 0.0f);

   bx::vec3MulMtx(look, cameraForward, mTransform.matrix);

   if (mForwardVelocity.len() > 0.01f)
   {
      MatrixF lookMatrix;
      bx::mtxLookAt(lookMatrix, mWorldPosition, look, up);
      mWorldPosition += (lookMatrix.getForwardVector() * mForwardVelocity.x);
      mWorldPosition -= (lookMatrix.getRightVector() * mForwardVelocity.y);
      mTransform.setPosition(mWorldPosition);
   }

   bx::vec3MulMtx(look, cameraForward, mTransform.matrix);
   bx::mtxLookAt(mRenderCamera->viewMatrix, mWorldPosition, look, up);
   mRenderCamera->position = mWorldPosition;
}

void EditorCamera::setForwardVelocity(Point3F velocity)
{
   mForwardVelocity = velocity;
}

bool EditorCamera::onMouseLeftDown(int x, int y)
{
   return false;
}

bool EditorCamera::onMouseLeftUp(int x, int y)
{
   return false;
}

bool EditorCamera::onMouseRightDown(int x, int y)
{
   mMouseDown = true;
   mMouseStart = Point2I(x, y);
   return false;
}

bool EditorCamera::onMouseRightUp(int x, int y)
{
   mMouseDown = false;
   return false;
}

bool EditorCamera::onMouseMove(int x, int y)
{
   if (!mMouseDown)
      return false;

   Point2I pos = Point2I(x, y);
   Point2I delta = mMouseStart - pos;
   mMouseStart = pos;

   if (delta.isZero()) return false;

   mHorizontalAngle += delta.x * 0.01f;
   mVerticalAngle -= delta.y * 0.01f;
   mVerticalAngle = mClampF(mVerticalAngle, -4.7f, -1.7f);
   return false;
}

// ---------------------------------------------------------------

void _addObjectTemplateAsset(wxString assetID, Point3F position)
{
   Scene::SceneObject* newObject = new Scene::SceneObject();
   newObject->setTemplateAsset(assetID);
   newObject->mTransform.setPosition(position);
   Torque::Scene.addObject(newObject, "NewSceneObject");
   newObject->registerObject();
}

void _addMeshAsset(wxString assetID, Point3F position)
{
   Scene::SceneObject* newObject = new Scene::SceneObject();
   Scene::MeshComponent* meshComponent = new Scene::MeshComponent();
   meshComponent->setMesh(assetID.c_str());
   newObject->addComponent(meshComponent);
   newObject->mTransform.setPosition(position);
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

void EditorManager::addObjectTemplateAsset(wxString assetID, Point3F position)
{
   _addObjectTemplateAsset(assetID, position);
}

void EditorManager::addMeshAsset(wxString assetID, Point3F position)
{
   _addMeshAsset(assetID, position);
}

void EditorManager::refreshChoices()
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

void EditorManager::refreshModuleList()
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

Vector<ModuleInfo>* EditorManager::getModuleList()
{
   refreshModuleList();
   return &mModuleList;
}

wxPGChoices* EditorManager::getTextureAssetChoices()
{
   refreshChoices();
   return &mTextureAssetChoices;
}

bool EditorManager::selectMaterial(wxString& returnMaterialName)
{
   return mMaterialSelectDialog->SelectAsset(returnMaterialName, "MaterialAsset");
}

bool EditorManager::selectAsset(wxString& returnValue, const char* filter)
{
   return mAssetSelectDialog->SelectAsset(returnValue, filter);
}