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

#include <wx/propgrid/propgrid.h>
#include <wx/dir.h>
#include <wx/treectrl.h>

// UI generated from wxFormBuilder
#include "Torque6EditorUI.h"
#include "theme.h"

#include "sceneWindow.h"
#include <bx/bx.h>
#include <bx/fpumath.h>
#include <debugdraw/debugdraw.h>

#include "lighting/lighting.h"
#include "scene/components/animationComponent.h"
#include "scene/components/controllerComponent.h"
#include "scene/components/lighting/lightComponent.h"
#include "scene/components/meshComponent.h"
#include "scene/components/physics/physicsBoxComponent.h"
#include "scene/components/physics/physicsSphereComponent.h"
#include "scene/components/textComponent.h"
#include "widgets/wxTorqueInspector/wxTorqueInspector.h"

SceneWindow::SceneWindow(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager)
   : Parent(_EditorManager, _frame, _manager),
     mScenePanel(NULL),
     mSelectedObject(NULL),
     mSelectedComponent(NULL),
     mMenuObject(NULL),
     mMenuComponent(NULL),
     mLightIcon(NULL),
     mRefreshing(false)
{
   mObjectIconList = new wxImageList(16, 16);

   // Translate Menu
   mTranslateMenu = new wxMenu;
   mTranslateMenu->Append(0, wxT("Snap: None"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(1, wxT("Snap: 0.1"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(2, wxT("Snap: 0.5"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(3, wxT("Snap: 1.0"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(4, wxT("Snap: 5.0"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneWindow::OnTranslateMenuEvent), NULL, this);

   // Rotate Menu
   mRotateMenu = new wxMenu;
   mRotateMenu->Append(0, wxT("Snap: None"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(1, wxT("Snap: 5 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(2, wxT("Snap: 15 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(3, wxT("Snap: 45 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(4, wxT("Snap: 90 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneWindow::OnRotateMenuEvent), NULL, this);

   // Scale Menu
   mScaleMenu = new wxMenu;
   mScaleMenu->Append(0, wxT("Snap: None"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(1, wxT("Snap: 0.1"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(2, wxT("Snap: 0.5"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(3, wxT("Snap: 1.0"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(4, wxT("Snap: 5.0"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneWindow::OnScaleMenuEvent), NULL, this);
}

SceneWindow::~SceneWindow()
{
   mRefreshing = true;
   delete mTranslateMenu;
   delete mRotateMenu;
   delete mScaleMenu;
}

void SceneWindow::initWindow()
{
   mGizmo.mEditorManager = mEditorManager;
   mScenePanel = new ScenePanel(mFrame, wxID_ANY);

   mScenePanel->Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SceneWindow::OnMenuEvent), NULL, this);

   // Object Icons.
   mObjectIconList->Add(wxBitmap("images/objectIcon.png", wxBITMAP_TYPE_PNG));
   mObjectIconList->Add(wxBitmap("images/componentIcon.png", wxBITMAP_TYPE_PNG));

   // Load Tabs
   mTabs = new wxFlatNotebook(mScenePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFNB_NO_X_BUTTON | wxFNB_NO_NAV_BUTTONS);
   mTabs->SetForegroundColour(Theme::lightBackgroundColor);
   mTabs->SetBackgroundColour(Theme::lightBackgroundColor);
   mTabs->SetGradientColors(Theme::lightBackgroundColor, Theme::lightBackgroundColor, Theme::darkBackgroundColor);
   mTabs->SetActiveTabColour(Theme::darkBackgroundColor);
   mTabs->SetActiveTabTextColour(wxColor(255, 255, 255));
   mTabs->SetNonActiveTabTextColour(wxColor(255, 255, 255));
   mTabs->SetTabAreaColour(Theme::lightBackgroundColor);
   
   // Objects tab
   mScenePanelObjects = new ScenePanel_Objects(mTabs, wxID_ANY, wxDefaultPosition, wxDefaultSize);

   // Add TorqueInspector to scene panel objects.
   mScenePanelInspector = new wxTorqueInspector(mEditorManager, mScenePanelObjects->InspectorWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize);
   mScenePanelObjects->InspectorContents->Add(mScenePanelInspector, 1, wxALL | wxEXPAND, 1);

   mTabs->AddPage(mScenePanelObjects, "Objects ", true);

   // Add Tabs to ScenePanel
   mScenePanel->ScenePanelContent->Add(mTabs, 1, wxEXPAND | wxALL, 5);

   // Object Icons
   mScenePanelObjects->objectList->AssignImageList(mObjectIconList);

   // Object Events
   mScenePanelObjects->objectList->Connect(wxID_ANY, wxEVT_TREE_SEL_CHANGED, wxTreeEventHandler(SceneWindow::OnTreeEvent), NULL, this);
   mScenePanelObjects->objectList->Connect(wxID_ANY, wxEVT_TREE_ITEM_MENU, wxTreeEventHandler(SceneWindow::OnTreeMenu), NULL, this);
   /*mScenePanelObjects->propertyGrid->Connect(wxID_ANY, wxEVT_PG_CHANGED, wxPropertyGridEventHandler(SceneWindow::OnObjectPropChanged), NULL, this);
   mScenePanelObjects->propertyGrid->SetEmptySpaceColour(wxColor(51, 51, 51));
   mScenePanelObjects->propertyGrid->SetMarginColour(wxColor(51, 51, 51));
   mScenePanelObjects->propertyGrid->SetCellBackgroundColour(wxColor(75, 75, 75));
   mScenePanelObjects->propertyGrid->SetCellTextColour(wxColor(255, 255, 255));
   mScenePanelObjects->propertyGrid->SetCaptionBackgroundColour(wxColor(51, 51, 51));
   mScenePanelObjects->propertyGrid->SetCaptionTextColour(wxColor(255, 255, 255));*/

   mObjectListRoot = mScenePanelObjects->objectList->AddRoot("ROOT");

   mManager->AddPane(mScenePanel, wxAuiPaneInfo().Caption("Scene")
                                                  .CaptionVisible( true )
                                                  .CloseButton( true )
                                                  .PinButton( true )
                                                  .MaximizeButton(true)
                                                  .Dock()
                                                  .Resizable()
                                                  .FloatingSize( wxDefaultSize )
                                                  .Left()
                                                  .Hide());
   mManager->Update();

   // Add Tools to toolabr
   mFrame->mainToolbar->AddTool(1, wxT("Move"), wxBitmap(wxT("images/translate.png"), wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_DROPDOWN, wxT("Move"), wxEmptyString, NULL);
   mFrame->mainToolbar->AddTool(2, wxT("Rotate"), wxBitmap(wxT("images/rotate.png"), wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_DROPDOWN, wxT("Rotate"), wxEmptyString, NULL);
   mFrame->mainToolbar->AddTool(3, wxT("Scale"), wxBitmap(wxT("images/scale.png"), wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_DROPDOWN, wxT("Scale"), wxEmptyString, NULL);
   mFrame->mainToolbar->AddSeparator();
   mFrame->mainToolbar->Realize();

   // Toolbar Dropdown Events
   mFrame->mainToolbar->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_DROPDOWN_CLICKED, wxCommandEventHandler(SceneWindow::OnToolbarDropdownEvent), NULL, this);

   // Refresh Mesh and Material Choices
   refreshChoices();
}

void SceneWindow::openWindow()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mScenePanel);
   paneInfo.Show();
   mManager->Update();

   if (mEditorManager->mProjectLoaded)
   {
      refreshObjectList();
      refreshChoices();
   }
}

void SceneWindow::closeWindow()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mScenePanel);
   paneInfo.Hide();
   mManager->Update();
}

void SceneWindow::renderWindow()
{
   Point3F editorPos = mEditorManager->mEditorCamera.getWorldPosition();
   editorPos = editorPos / 10.0f;
   editorPos.x = mFloor(editorPos.x);
   editorPos.y = mFloor(editorPos.y);
   editorPos = editorPos * 10.0f;

   Torque::Debug.ddPush();
   Torque::Debug.ddSetState(true, false, true);

   Torque::Debug.ddSetWireframe(true);
   Torque::Debug.ddSetColor(BGFXCOLOR_RGBA(255, 255, 255, 255));
   F32 gridNormal[3] = { 0.0f, 0.0f, 1.0f };
   F32 gridPos[3]    = { editorPos.x, editorPos.y, -0.01f };
   Torque::Debug.ddDrawGrid(gridNormal, gridPos, 100, 10.0f);

   // Draw Light Icons
   /*if (mLightIcon != NULL)
   {
      Vector<Lighting::LightData*> lightList = Torque::Lighting.getLightList();
      for (S32 n = 0; n < lightList.size(); ++n)
      {
         Lighting::LightData* light = lightList[n];

         Torque::Graphics.drawBillboard(mEditorManager->mRenderLayer4View->id,
                                              mLightIcon,
                                              light->position,
                                              1.0f, 1.0f,
                                              ColorI(light->color[0] * 255, light->color[1] * 255, light->color[2] * 255, 255),
                                              NULL);
      }
   }*/

   // Object Selected
   if (mSelectedObject != NULL && mSelectedComponent == NULL)
   {
      // Bounding Box
      //Torque::Graphics.drawBox3D(mEditorManager->mRenderLayer4View->id, mSelectedObject->mBoundingBox, ColorI(255, 255, 255, 255), NULL);

      Aabb debugBox;
      debugBox.m_min[0] = mSelectedObject->mBoundingBox.minExtents.x;
      debugBox.m_min[1] = mSelectedObject->mBoundingBox.minExtents.y;
      debugBox.m_min[2] = mSelectedObject->mBoundingBox.minExtents.z;
      debugBox.m_max[0] = mSelectedObject->mBoundingBox.maxExtents.x;
      debugBox.m_max[1] = mSelectedObject->mBoundingBox.maxExtents.y;
      debugBox.m_max[2] = mSelectedObject->mBoundingBox.maxExtents.z;

      Torque::Debug.ddSetWireframe(true);
      Torque::Debug.ddSetColor(BGFXCOLOR_RGBA(255, 255, 255, 255));
      Torque::Debug.ddDrawAabb(debugBox);

      Torque::Debug.ddPop();

      // Render Gizmo
      mGizmo.render();

      return;
   }

   // Component Selected
   if (mSelectedObject != NULL && mSelectedComponent != NULL)
   {
      Box3F boundingBox = mSelectedComponent->getBoundingBox();
      boundingBox.transform(mSelectedObject->mTransform.matrix);

      // Bounding Box
      //Torque::Graphics.drawBox3D(mEditorManager->mRenderLayer4View->id, boundingBox, ColorI(0, 255, 0, 255), NULL);

      Aabb debugBox;
      debugBox.m_min[0] = boundingBox.minExtents.x;
      debugBox.m_min[1] = boundingBox.minExtents.y;
      debugBox.m_min[2] = boundingBox.minExtents.z;
      debugBox.m_max[0] = boundingBox.maxExtents.x;
      debugBox.m_max[1] = boundingBox.maxExtents.y;
      debugBox.m_max[2] = boundingBox.maxExtents.z;

      Torque::Debug.ddSetWireframe(true);
      Torque::Debug.ddSetColor(BGFXCOLOR_RGBA(255, 255, 0, 255));
      Torque::Debug.ddDrawAabb(debugBox);

      Torque::Debug.ddPop();

      // Render Gizmo
      mGizmo.render();
   }
}

bool SceneWindow::onMouseLeftDown(int x, int y)
{
   Point3F worldRay = Torque::Rendering.screenToWorld(Point2I(x, y));
   Point3F editorPos = mEditorManager->mEditorCamera.getWorldPosition();

   if (!mGizmo.onMouseLeftDown(x, y))
   {
      Scene::SceneObject* hit = Torque::Scene.raycast(editorPos, editorPos + (worldRay * 1000.0f));
      if (mSelectedObject != hit)
      {
         if (hit)
            selectObject(hit, true);
      }
   }

   return false;
}

bool SceneWindow::onMouseLeftUp(int x, int y)
{
   mGizmo.onMouseLeftUp(x, y);

   if ( mSelectedComponent != NULL )
      mScenePanelInspector->Inspect(mSelectedComponent);
   else if ( mSelectedObject != NULL )
      mScenePanelInspector->Inspect(mSelectedObject);

   return false;
}

bool SceneWindow::onMouseMove(int x, int y)
{
   mGizmo.onMouseMove(x, y);
   return false;
}

void SceneWindow::onSceneChanged()
{
   refreshObjectList();
   refreshChoices();
}

void SceneWindow::onProjectLoaded(const wxString& projectName, const wxString& projectPath)
{
   refreshObjectList();
   refreshChoices();

   if ( mLightIcon == NULL )
      mLightIcon = Torque::Graphics.loadTexture("light.png", TextureHandle::BitmapKeepTexture, BGFX_TEXTURE_NONE, false, false);
}

void SceneWindow::onProjectClosed()
{

}

void SceneWindow::OnMenuEvent(wxCommandEvent& evt)
{
   if (evt.GetId() == SCENE_NEW)
      Torque::Scene.clear();

   if (evt.GetId() == SCENE_OPEN)
   {
      wxFileDialog openFile(mFrame, wxT("Open Scene File"), "", "", "taml files (*.taml)|*.taml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
      if (openFile.ShowModal() == wxID_OK)
         Torque::Scene.load(openFile.GetPath(), false);
   }

   if (evt.GetId() == SCENE_SAVE)
   {
      wxFileDialog saveFile(mFrame, wxT("Save Scene File"), "", "", "taml files (*.taml)|*.taml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
      if (saveFile.ShowModal() == wxID_OK)
         Torque::Scene.save(saveFile.GetPath());
   }

   if (evt.GetId() == ADD_OBJECT_BUTTON)
      openAddObjectMenu();

   if (evt.GetId() == ADD_COMPONENT_BUTTON)
      openAddComponentMenu();

   refreshObjectList();
   refreshChoices();
}

void SceneWindow::openAddObjectMenu()
{
   wxMenu* menu = new wxMenu;
   menu->Append(0, wxT("Empty Object"));

   // From Asset
   wxMenu* fromAssetMenu         = new wxMenu;
   Vector<ModuleInfo>* modules   = mEditorManager->getModuleList();
   const char* currentModuleID   = "";
   wxMenu* currentModuleMenu     = NULL;
   U32 menuItemID                = 1;
   for (Vector<ModuleInfo>::iterator modulesItr = modules->begin(); modulesItr != modules->end(); ++modulesItr)
   {
      if (dStrcmp(modulesItr->moduleID, currentModuleID) != 0)
      {
         currentModuleID = modulesItr->moduleID;
         currentModuleMenu = new wxMenu;
      }

      for (Vector<AssetCategoryInfo>::iterator assetCatItr = modulesItr->assets.begin(); assetCatItr != modulesItr->assets.end(); ++assetCatItr)
      {
         wxMenu* currentCategoryMenu = new wxMenu;
         for (Vector<const AssetDefinition*>::iterator assetItr = assetCatItr->assets.begin(); assetItr != assetCatItr->assets.end(); ++assetItr)
         {
            // Fetch asset definition.
            const AssetDefinition* pAssetDefinition = *assetItr;
            char buf[256];
            dStrcpy(buf, pAssetDefinition->mAssetId);
            const char* moduleName  = dStrtok(buf, ":");
            const char* assetName   = dStrtok(NULL, ":");
            currentCategoryMenu->Append(menuItemID, assetName);
            menuItemID++;
         }
         currentModuleMenu->AppendSubMenu(currentCategoryMenu, assetCatItr->categoryName);
      }

      fromAssetMenu->AppendSubMenu(currentModuleMenu, modulesItr->moduleID);
   }
   menu->AppendSubMenu(fromAssetMenu, wxT("From Asset"));

   menu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneWindow::OnAddObjectMenuEvent), NULL, this);
   mFrame->PopupMenu(menu, wxDefaultPosition);
   delete menu;
}

void SceneWindow::OnAddObjectMenuEvent(wxCommandEvent& evt)
{
   if (evt.GetId() == 0)
   {
      Scene::SceneObject* newObject = new Scene::SceneObject();
      newObject->registerObject("NewSceneObject");
      Torque::Scene.addObject(newObject, "NewSceneObject");
      refreshObjectList();
   }

   if (evt.GetId() > 0)
   {
      Vector<ModuleInfo>* modules = mEditorManager->getModuleList();
      U32 menuItemID = 1;
      for (Vector<ModuleInfo>::iterator modulesItr = modules->begin(); modulesItr != modules->end(); ++modulesItr)
      {
         for (Vector<AssetCategoryInfo>::iterator assetCatItr = modulesItr->assets.begin(); assetCatItr != modulesItr->assets.end(); ++assetCatItr)
         {
            for (Vector<const AssetDefinition*>::iterator assetItr = assetCatItr->assets.begin(); assetItr != assetCatItr->assets.end(); ++assetItr)
            {
               // Fetch asset definition.
               const AssetDefinition* pAssetDefinition = *assetItr;
               if (evt.GetId() == menuItemID)
               {
                  Point3F editorPos;// = Torque::Scene.getActiveCamera()->getPosition();
                  if ( dStrcmp(pAssetDefinition->mAssetType, "ObjectTemplateAsset") == 0 )
                     mEditorManager->addObjectTemplateAsset(pAssetDefinition->mAssetId, editorPos);
                  else if (dStrcmp(pAssetDefinition->mAssetType, "MeshAsset") == 0)
                     mEditorManager->addMeshAsset(pAssetDefinition->mAssetId, editorPos);
                  return;
               }
               menuItemID++;
            }
         }
      }
   }
}

void SceneWindow::openAddComponentMenu()
{
   wxMenu* menu = new wxMenu;

   refreshClassLists();
   for (S32 i = 0; i < mComponentClassList.size(); ++i)
      menu->Append(i, mComponentClassList[i]);

   menu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneWindow::OnAddComponentMenuEvent), NULL, this);
   mFrame->PopupMenu(menu, wxDefaultPosition);
   delete menu;
}

void SceneWindow::OnAddComponentMenuEvent(wxCommandEvent& evt)
{
   addComponent(mSelectedObject, mComponentClassList[evt.GetId()]);
}

void SceneWindow::OnTreeEvent( wxTreeEvent& evt )
{
   if (mRefreshing)
      return;

   if (evt.GetId() == OBJECT_LIST)
   {
      ObjectTreeItemData* data = dynamic_cast<ObjectTreeItemData*>(mScenePanelObjects->objectList->GetItemData(evt.GetItem()));
      if (data)
      {
         // Did we select an object?
         Scene::SceneObject* Object = dynamic_cast<Scene::SceneObject*>(data->objPtr);
         if (Object)
         {
            selectObject(Object);
            return;
         }

         // or a component?
         Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(data->objPtr);
         if (component)
         {
            selectComponent(component);
            return;
         }
      }
   }
}

void SceneWindow::OnTreeMenu( wxTreeEvent& evt ) 
{ 
   if (evt.GetId() == OBJECT_LIST)
   {
      ObjectTreeItemData* data = dynamic_cast<ObjectTreeItemData*>(mScenePanelObjects->objectList->GetItemData(evt.GetItem()));

      Scene::SceneObject* Object = dynamic_cast<Scene::SceneObject*>(data->objPtr);
      if (Object)
      {
         mMenuObject = Object;

         wxMenu* menu = new wxMenu;
         menu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneWindow::OnObjectMenuEvent), NULL, this);
         menu->Append(0, wxT("Delete Object"));

         // Find all components in console namespace list then make a menu.
         wxMenu* compMenu = new wxMenu;
         refreshClassLists();
         for (S32 i = 0; i < mComponentClassList.size(); ++i)
            compMenu->Append(1 + i, mComponentClassList[i]);
         menu->AppendSubMenu(compMenu, wxT("Add Component"));

         mFrame->PopupMenu(menu, wxDefaultPosition);
         delete menu;

         mMenuObject = NULL;
         return;
      }

      Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(data->objPtr);
      if (component)
      {
         mMenuComponent = component;

         wxMenu* menu = new wxMenu;
         menu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneWindow::OnComponentMenuEvent), NULL, this);
         menu->Append(0, wxT("Delete Component"));
         mFrame->PopupMenu(menu, wxDefaultPosition);
         delete menu;

         mMenuComponent = NULL;
         return;
      }
   }
} 

void SceneWindow::OnObjectMenuEvent(wxCommandEvent& evt)
{
   // Delete Object
   if (evt.GetId() == 0)
   {
      if (mSelectedObject == mMenuObject)
      {
         mSelectedObject = NULL;
         mSelectedComponent = NULL;
         mScenePanelObjects->addComponentButton->Enable(false);
      }

      Torque::Scene.deleteObject(mMenuObject);
      refreshObjectList();
      return;
   }

   // Add New Component
   if (evt.GetId() >= 1)
      addComponent(mMenuObject, mComponentClassList[evt.GetId() - 1]);
}

void SceneWindow::OnComponentMenuEvent(wxCommandEvent& evt)
{
   // Delete Object
   if (evt.GetId() == 0)
   {
      // Sanity check. mOwnerObject should never be NULL.
      if (mMenuComponent->mOwnerObject != NULL)
      {
         mMenuComponent->mOwnerObject->removeComponent(mMenuComponent);
         mMenuComponent = NULL;
         refreshObjectList();
         return;
      }
   }
}

void SceneWindow::OnObjectPropChanged(wxPropertyGridEvent& evt)
{
   wxString name = evt.GetPropertyName();
   wxVariant val = evt.GetPropertyValue();
   wxString strVal = val.GetString();

   SimObject* selected = mSelectedObject;
   if (mSelectedComponent != NULL)
      selected = mSelectedComponent;

   // Special Name Handling.
   if (name == "Name" && mSelectedObject)
   {
      mSelectedObject->assignUniqueName(strVal);
      refreshObjectList();
      return;
   }
   else if (name == "InternalName" && mSelectedComponent)
   {
      mSelectedComponent->setInternalName(strVal);
      refreshObjectList();
      return;
   }

   // Special Field Handling by Name
   if (name == "MeshAsset")
   {
      long intVal = val.GetInteger();
      strVal = mMeshChoices.GetLabel(intVal);
   }
   else if (name.StartsWith("MaterialAsset"))
   {
      long intVal = val.GetInteger();
      strVal = mMaterialChoices.GetLabel(intVal);
   }
   else if (name.StartsWith("SubMesh"))
   {
      strVal = val.GetBool() ? "true" : "false";
   }

   // Special Field Handling by Type
   U32 type = selected->getDataFieldType(Torque::StringTableLink->insert(name), NULL);
   if (type == Torque::Con.TypeColorF)
   {
      wxColour color;
      color << val;
      strVal.Printf("%f %f %f 1.0", color.Red() / 255.0f, color.Green() / 255.0f, color.Blue() / 255.0f);
   }

   // Assign the value and refresh the Object. 
   // Note: No need to refresh a selected component, better to refresh the whole Object.
   selected->setDataField(Torque::StringTableLink->insert(name), NULL, strVal);
   mSelectedObject->refresh();
}

void SceneWindow::refreshObjectList()
{
   if (!mEditorManager->isProjectLoaded())
      return;

   mRefreshing = true;

   // Clear list.
   mScenePanelObjects->objectList->DeleteAllItems();
   mObjectListRoot = mScenePanelObjects->objectList->AddRoot("ROOT");

   wxTreeItemId selectItem = mObjectListRoot;
   SimGroup* sceneGroup = Torque::Scene.getSceneGroup();
   if ( sceneGroup != NULL )
   {
      for(S32 n = 0; n < sceneGroup->size(); ++n)
      {
         Scene::SceneObject* Object = dynamic_cast<Scene::SceneObject*>(sceneGroup->at(n));
         if ( !Object ) continue;
         wxTreeItemId ObjectItem = mScenePanelObjects->objectList->AppendItem(mObjectListRoot, Object->getName(), 0, 0, new ObjectTreeItemData(Object));
         if (mSelectedObject == Object)
            selectItem = ObjectItem;

         // Iterate components
         for(S32 n = 0; n < Object->mComponents.size(); ++n)
         {
            Scene::BaseComponent* component = Object->mComponents[n];
            if (!component) continue;

            wxString compName(component->getClassName());
            const char* internalName = component->getInternalName();
            if (internalName)
            {
               compName = internalName;
               compName.Append(" (");
               compName.Append(component->getClassName());
               compName.Append(")");
            }

            wxTreeItemId componentItem = mScenePanelObjects->objectList->AppendItem(ObjectItem, compName, 1, 1, new ObjectTreeItemData(component));
            if (mSelectedComponent == component)
               selectItem = componentItem;
         }
      }
   }

   // Retain selection.
   if (selectItem != mObjectListRoot)
      mScenePanelObjects->objectList->SelectItem(selectItem);

   mScenePanelObjects->objectList->Refresh();
   mRefreshing = false;
}

void SceneWindow::refreshChoices()
{
   if (!mEditorManager->isProjectLoaded())
      return;

   mRefreshing = true;
   mMaterialChoices.Clear();
   mMeshChoices.Clear();
   mObjectTemplateChoices.Clear();
   
   Vector<const AssetDefinition*> assetDefinitions = Torque::AssetDatabaseLink.getDeclaredAssets();

   // Iterate sorted asset definitions.
   for (Vector<const AssetDefinition*>::iterator assetItr = assetDefinitions.begin(); assetItr != assetDefinitions.end(); ++assetItr)
   {
      // Fetch asset definition.
      const AssetDefinition* pAssetDefinition = *assetItr;

      //char buf[256];
      //dStrcpy(buf, pAssetDefinition->mAssetId);
      //const char* moduleName = dStrtok(buf, ":");
      //const char* assetName = dStrtok(NULL, ":");

      // Populate Material choices menu.
      if (dStrcmp(pAssetDefinition->mAssetType, "MaterialAsset") == 0)
         mMaterialChoices.Add(pAssetDefinition->mAssetId, mMaterialChoices.GetCount());

      // Populate Mesh choices menu.
      if (dStrcmp(pAssetDefinition->mAssetType, "MeshAsset") == 0)
         mMeshChoices.Add(pAssetDefinition->mAssetId, mMeshChoices.GetCount());

      // Object Templates
      if (dStrcmp(pAssetDefinition->mAssetType, "ObjectTemplateAsset") == 0)
         mObjectTemplateChoices.Add(pAssetDefinition->mAssetId, mMeshChoices.GetCount());
   }

   mRefreshing = false;
}

void SceneWindow::refreshClassLists()
{
   mComponentClassList.clear();

   for (Namespace *walk = Torque::Con.getNamespaceList(); walk; walk = walk->mNext)
   {
      if (walk->mParent == NULL)
         continue;

      Namespace* parent = walk->mParent;
      while (parent != NULL)
      {
         if (dStrcmp(parent->mName, "BaseComponent") == 0)
         {
            mComponentClassList.push_back(walk->mName);
            break;
         }

         parent = parent->mParent;
      }
   }
}

void SceneWindow::selectObject(Scene::SceneObject* obj, bool updateTree)
{
   mSelectedObject = obj;
   mSelectedComponent = NULL;
   mScenePanelObjects->addComponentButton->Enable(true);

   // Update the gizmo.
   mGizmo.selectObject(obj);

   // Update the tree selection.
   if (updateTree)
   {
      wxTreeItemIdValue cookie;
      wxTreeItemId item = mScenePanelObjects->objectList->GetFirstChild(mObjectListRoot, cookie);
      wxTreeItemId child;
      while (item.IsOk())
      {
         ObjectTreeItemData* data = dynamic_cast<ObjectTreeItemData*>(mScenePanelObjects->objectList->GetItemData(item));
         if (data)
         {
            Scene::SceneObject* dataObject = dynamic_cast<Scene::SceneObject*>(data->objPtr);
            if (dataObject == obj)
            {
               mScenePanelObjects->objectList->SelectItem(item);
               break;
            }
         }
         item = mScenePanelObjects->objectList->GetNextChild(mObjectListRoot, cookie);
      }
   }

   mScenePanelInspector->Inspect(obj);
}

void SceneWindow::addComponent(Scene::SceneObject* Object, StringTableEntry componentClassName)
{
   Scene::BaseComponent* newComponent = dynamic_cast<Scene::BaseComponent*>(Torque::Con.createObject(componentClassName));
   if (newComponent)
   {
      newComponent->registerObject();
      Object->addComponent(newComponent);
      Object->refresh();
      refreshObjectList();
      selectComponent(newComponent, true);
   }
}

void SceneWindow::selectComponent(Scene::BaseComponent* component, bool updateTree)
{
   mSelectedObject = component->mOwnerObject;
   mSelectedComponent = component;

   // Update the gizmo.
   mGizmo.selectComponent(component);

   // Update the tree selection.
   if (updateTree)
   {
      wxTreeItemIdValue cookie;
      wxTreeItemId item = mScenePanelObjects->objectList->GetFirstChild(mObjectListRoot, cookie);
      wxTreeItemId child;
      while (item.IsOk())
      {
         ObjectTreeItemData* data = dynamic_cast<ObjectTreeItemData*>(mScenePanelObjects->objectList->GetItemData(item));
         if (data)
         {
            Scene::BaseComponent* dataComponent = dynamic_cast<Scene::BaseComponent*>(data->objPtr);
            if (dataComponent == component)
            {
               mScenePanelObjects->objectList->SelectItem(item);
               break;
            }
         }
         item = mScenePanelObjects->objectList->GetNextChild(mObjectListRoot, cookie);
      }
   }

   mScenePanelInspector->Inspect(component);
   
}

void SceneWindow::OnToolbarDropdownEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 1:
         mFrame->PopupMenu(mTranslateMenu, wxDefaultPosition);
         break;

      case 2:
         mFrame->PopupMenu(mRotateMenu, wxDefaultPosition);
         break;

      case 3:
         mFrame->PopupMenu(mScaleMenu, wxDefaultPosition);
         break;

      default:
         break;
   }
}

void SceneWindow::OnTranslateMenuEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 0:
         mGizmo.mTranslateSnap = 0.0f;
         break;

      case 1:
         mGizmo.mTranslateSnap = 0.1f;
         break;

      case 2:
         mGizmo.mTranslateSnap = 0.5f;
         break;

      case 3:
         mGizmo.mTranslateSnap = 1.0f;
         break;

      case 4:
         mGizmo.mTranslateSnap = 5.0f;
         break;
   }
}

void SceneWindow::OnRotateMenuEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 0:
         mGizmo.mRotateSnap = 0.0f;
         break;

      case 1:
         mGizmo.mRotateSnap = M_PI_F / 36.0f;
         break;

      case 2:
         mGizmo.mRotateSnap = M_PI_F / 12.0f;
         break;

      case 3:
         mGizmo.mRotateSnap = M_PI_F / 4.0f;
         break;

      case 4:
         mGizmo.mRotateSnap = M_PI_F / 2.0f;
         break;
   }
}

void SceneWindow::OnScaleMenuEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 0:
         mGizmo.mScaleSnap = 0.0f;
         break;

      case 1:
         mGizmo.mScaleSnap = 0.1f;
         break;

      case 2:
         mGizmo.mScaleSnap = 0.5f;
         break;

      case 3:
         mGizmo.mScaleSnap = 1.0f;
         break;

      case 4:
         mGizmo.mScaleSnap = 5.0f;
         break;   
   }
}