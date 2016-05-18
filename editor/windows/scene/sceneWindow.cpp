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
   Bind(wxTORQUE_SELECT_OBJECT, &SceneWindow::OnObjectSelected, this);
}

SceneWindow::~SceneWindow()
{
   mRefreshing = true;
}

void SceneWindow::initWindow()
{
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

void SceneWindow::OnObjectSelected(wxTorqueObjectEvent& evt)
{
   Scene::SceneObject* obj = evt.GetSceneObject();
   if (obj)
   {
      selectObject(obj, true);
      return;
   }

   Scene::BaseComponent* component = evt.GetComponent();
   if (component)
   {
      selectComponent(component, true);
      return;
   }
}