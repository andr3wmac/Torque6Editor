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
#include "../Torque6EditorUI.h"

#include "sceneTool.h"
#include "3d/scene/camera.h"
#include <bx/bx.h>
#include <bx/fpumath.h>

#include "3d/entity/components/animationComponent.h"
#include "3d/entity/components/controllerComponent.h"
#include "3d/entity/components/lightComponent.h"
#include "3d/entity/components/meshComponent.h"
#include "3d/entity/components/physicsComponent.h"
#include "3d/entity/components/textComponent.h"

SceneTool::SceneTool(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager)
   : Parent(_projectManager, _frame, _manager),
     mScenePanel(NULL),
     mSelectedObject(NULL),
     mSelectedEntity(NULL),
     mSelectedComponent(NULL),
     mSelectedFeature(NULL),
     mMenuEntity(NULL),
     mMenuComponent(NULL),
     mLightIcon(NULL)
{
   mEntityIconList = new wxImageList(16, 16);
   mFeatureIconList = new wxImageList( 16, 16 );

   // Translate Menu
   mTranslateMenu = new wxMenu;
   mTranslateMenu->Append(0, wxT("Snap: None"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(1, wxT("Snap: 0.1"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(2, wxT("Snap: 0.5"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(3, wxT("Snap: 1.0"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(4, wxT("Snap: 5.0"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneTool::OnTranslateMenuEvent), NULL, this);

   // Rotate Menu
   mRotateMenu = new wxMenu;
   mRotateMenu->Append(0, wxT("Snap: None"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(1, wxT("Snap: 5 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(2, wxT("Snap: 15 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(3, wxT("Snap: 45 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(4, wxT("Snap: 90 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneTool::OnRotateMenuEvent), NULL, this);

   // Scale Menu
   mScaleMenu = new wxMenu;
   mScaleMenu->Append(0, wxT("Snap: None"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(1, wxT("Snap: 0.1"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(2, wxT("Snap: 0.5"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(3, wxT("Snap: 1.0"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(4, wxT("Snap: 5.0"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneTool::OnScaleMenuEvent), NULL, this);
}

SceneTool::~SceneTool()
{
   delete mTranslateMenu;
   delete mRotateMenu;
   delete mScaleMenu;
}

void SceneTool::initTool()
{
   mGizmo.mProjectManager = mProjectManager;
   mScenePanel = new ScenePanel(mFrame, wxID_ANY);

   mScenePanel->Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SceneTool::OnMenuEvent), NULL, this);

   // Entity Icons.
   mEntityIconList->Add(wxBitmap("images/entityIcon.png", wxBITMAP_TYPE_PNG));
   mEntityIconList->Add(wxBitmap("images/componentIcon.png", wxBITMAP_TYPE_PNG));
   mScenePanel->entityList->AssignImageList(mEntityIconList);

   // Entity Events
   mScenePanel->entityList->Connect(wxID_ANY, wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(SceneTool::OnTreeEvent), NULL, this);
   mScenePanel->entityList->Connect(wxID_ANY, wxEVT_TREE_ITEM_MENU, wxTreeEventHandler(SceneTool::OnTreeMenu), NULL, this);
   mScenePanel->propertyGrid->Connect(wxID_ANY, wxEVT_PG_CHANGED, wxPropertyGridEventHandler(SceneTool::OnEntityPropChanged), NULL, this);

   // Feature Icons.
   mFeatureIconList->Add(wxBitmap("images/featureIcon.png", wxBITMAP_TYPE_PNG));
   mScenePanel->featureList->AssignImageList(mFeatureIconList);

   // Feature Events
   mScenePanel->featureList->Connect(wxID_ANY, wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(SceneTool::OnTreeEvent), NULL, this);
   mScenePanel->featureList->Connect(wxID_ANY, wxEVT_TREE_ITEM_MENU, wxTreeEventHandler(SceneTool::OnTreeMenu), NULL, this);
   mScenePanel->featurePropGrid->Connect(wxID_ANY, wxEVT_PG_CHANGED, wxPropertyGridEventHandler(SceneTool::OnFeaturePropChanged), NULL, this);

   // Feature Menu Events
   mScenePanel->addFeatureMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneTool::OnMenuEvent), NULL, this);

   mEntityListRoot = mScenePanel->entityList->AddRoot("ROOT");
   mFeatureListRoot = mScenePanel->featureList->AddRoot("ROOT");

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
   //mFrame->mainToolbar->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_DROPDOWN_CLICKED, wxCommandEventHandler(SceneTool::OnToolbarDropdownEvent), NULL, this);

   // Refresh Mesh and Material Choices
   refreshChoices();
}

void SceneTool::openTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mScenePanel);
   paneInfo.Show();
   mManager->Update();

   if (mProjectManager->mProjectLoaded)
   {
      refreshEntityList();
      refreshFeatureList();
      refreshChoices();
   }
}

void SceneTool::closeTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mScenePanel);
   paneInfo.Hide();
   mManager->Update();
}

void SceneTool::renderTool()
{
   // Draw Light Icons
   if (mLightIcon != NULL)
   {
      Vector<Rendering::LightData*> lightList = Plugins::Link.Rendering.getLightList();
      for (U32 n = 0; n < lightList.size(); ++n)
      {
         Rendering::LightData* light = lightList[n];

         Plugins::Link.Graphics.drawBillboard(mProjectManager->mRenderLayer4View->id,
                                              mLightIcon,
                                              light->position,
                                              1.0f, 1.0f,
                                              ColorI(light->color[0] * 255, light->color[1] * 255, light->color[2] * 255, 255),
                                              NULL);
      }
   }

   // Entity Selected
   if (mSelectedEntity != NULL && mSelectedComponent == NULL)
   {
      // Bounding Box
      Plugins::Link.Graphics.drawBox3D(mProjectManager->mRenderLayer4View->id, mSelectedEntity->mBoundingBox, ColorI(255, 255, 255, 255), NULL);

      // Render Gizmo
      mGizmo.render();

      return;
   }

   // Component Selected
   if (mSelectedEntity != NULL && mSelectedComponent != NULL)
   {
      Box3F boundingBox = mSelectedComponent->getBoundingBox();
      boundingBox.transform(mSelectedEntity->mTransformMatrix);

      // Bounding Box
      Plugins::Link.Graphics.drawBox3D(mProjectManager->mRenderLayer4View->id, boundingBox, ColorI(0, 255, 0, 255), NULL);

      // Render Gizmo
      mGizmo.render();
   }
}

bool SceneTool::onMouseLeftDown(int x, int y)
{
   Point3F worldRay = Plugins::Link.Rendering.screenToWorld(Point2I(x, y));
   Point3F editorPos = Plugins::Link.Scene.getActiveCamera()->getPosition();

   if (!mGizmo.onMouseLeftDown(x, y))
   {
      Scene::SceneEntity* hit = Plugins::Link.Scene.raycast(editorPos, editorPos + (worldRay * 1000.0f));
      if (mSelectedObject != hit)
      {
         if (hit)
            selectEntity(hit);
      }
   }

   return false;
}

bool SceneTool::onMouseLeftUp(int x, int y)
{
   mGizmo.onMouseLeftUp(x, y);

   if ( mSelectedObject != NULL )
      loadObjectProperties(mScenePanel->propertyGrid, mSelectedObject);

   return false;
}

bool SceneTool::onMouseMove(int x, int y)
{
   mGizmo.onMouseMove(x, y);
   return false;
}

void SceneTool::onSceneChanged()
{
   refreshEntityList();
   refreshFeatureList();
   refreshChoices();
}

void SceneTool::onProjectLoaded(const wxString& projectName, const wxString& projectPath)
{
   refreshEntityList();
   refreshFeatureList();
   refreshChoices();

   if ( mLightIcon == NULL )
      mLightIcon = Plugins::Link.Graphics.loadTexture("light.png", TextureHandle::BitmapKeepTexture, BGFX_TEXTURE_NONE, false, false);
}

void SceneTool::onProjectClosed()
{

}

void SceneTool::OnMenuEvent(wxCommandEvent& evt)
{
   if (evt.GetId() == SCENE_NEW)
      Plugins::Link.Scene.clear();

   if (evt.GetId() == SCENE_OPEN)
   {
      wxFileDialog openFile(mFrame, wxT("Open Scene File"), "", "", "taml files (*.taml)|*.taml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
      if (openFile.ShowModal() == wxID_OK)
         Plugins::Link.Scene.load(openFile.GetPath());
   }

   if (evt.GetId() == SCENE_SAVE)
   {
      wxFileDialog saveFile(mFrame, wxT("Save Scene File"), "", "", "taml files (*.taml)|*.taml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
      if (saveFile.ShowModal() == wxID_OK)
         Plugins::Link.Scene.save(saveFile.GetPath());
   }

   if (evt.GetId() == ADD_FEATURE_BUTTON)
      mFrame->PopupMenu(mScenePanel->addFeatureMenu, wxDefaultPosition);

   if (evt.GetId() == ADD_ENTITY_BUTTON)
      openAddEntityMenu();

   if (evt.GetId() == ADD_FEATURE_DLAA)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("DLAA"));
      feature->registerObject();
      Plugins::Link.Scene.addFeature(feature);
   }

   if (evt.GetId() == ADD_FEATURE_SSAO)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("SSAO"));
      feature->registerObject();
      Plugins::Link.Scene.addFeature(feature);
   }

   if (evt.GetId() == ADD_FEATURE_HDR)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("HDR"));
      feature->registerObject();
      Plugins::Link.Scene.addFeature(feature);
   }

   if (evt.GetId() == ADD_FEATURE_SKYBOX)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("SimpleSkybox"));
      feature->registerObject();
      Plugins::Link.Scene.addFeature(feature);
   }

   if (evt.GetId() == ADD_FEATURE_DIRLIGHT)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("DirectionalLight"));
      feature->registerObject();
      Plugins::Link.Scene.addFeature(feature);
   }

   if (evt.GetId() == ADD_FEATURE_SKYLIGHT)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("SkyLight"));
      feature->registerObject();
      Plugins::Link.Scene.addFeature(feature);
   }

   refreshEntityList();
   refreshFeatureList();
   refreshChoices();
}

void SceneTool::openAddEntityMenu()
{
   wxMenu* menu = new wxMenu;
   menu->Append(0, wxT("New Entity"));
   menu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneTool::OnAddEntityMenuEvent), NULL, this);
   mFrame->PopupMenu(menu, wxDefaultPosition);
   delete menu;
}

void SceneTool::OnAddEntityMenuEvent(wxCommandEvent& evt)
{
   if (evt.GetId() == 0)
   {
      Scene::SceneEntity* newEntity = new Scene::SceneEntity();
      newEntity->registerObject("NewSceneEntity");
      Plugins::Link.Scene.addEntity(newEntity, "NewSceneEntity");
      refreshEntityList();
   }
}

void SceneTool::OnTreeEvent( wxTreeEvent& evt )
{
   if (evt.GetId() == ENTITY_LIST)
   {
      EntityTreeItemData* data = dynamic_cast<EntityTreeItemData*>(mScenePanel->entityList->GetItemData(evt.GetItem()));
      if (data)
      {
         // Did we select an entity ..
         Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(data->objPtr);
         if (entity)
         {
            selectEntity(entity);
            return;
         }

         // .. or a component?
         Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(data->objPtr);
         if (component)
         {
            selectComponent(component);
            return;
         }
      }
   }

   if (evt.GetId() == FEATURE_LIST)
   {
      FeatureTreeItemData* data = dynamic_cast<FeatureTreeItemData*>(mScenePanel->featureList->GetItemData(evt.GetItem()));
      if (data)
      {
         mSelectedFeature = data->objPtr;
         loadObjectProperties(mScenePanel->featurePropGrid, data->objPtr);
         return;
      }
   }
}

void SceneTool::OnTreeMenu( wxTreeEvent& evt ) 
{ 
   if (evt.GetId() == ENTITY_LIST)
   {
      EntityTreeItemData* data = dynamic_cast<EntityTreeItemData*>(mScenePanel->entityList->GetItemData(evt.GetItem()));

      Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(data->objPtr);
      if (entity)
      {
         mMenuEntity = entity;

         wxMenu* menu = new wxMenu;
         menu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneTool::OnEntityMenuEvent), NULL, this);
         menu->Append(0, wxT("Delete Entity"));

         wxMenu* compMenu = new wxMenu;
         compMenu->Append(1, wxT("AnimationComponent"));
         compMenu->Append(2, wxT("ControllerComponent"));
         compMenu->Append(3, wxT("LightComponent"));
         compMenu->Append(4, wxT("MeshComponent"));
         compMenu->Append(5, wxT("PhysicsComponent"));
         compMenu->Append(6, wxT("TextComponent"));
         menu->AppendSubMenu(compMenu, wxT("Add Component"));

         mFrame->PopupMenu(menu, wxDefaultPosition);
         delete menu;

         mMenuEntity = NULL;
         return;
      }

      Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(data->objPtr);
      if (component)
      {
         mMenuComponent = component;

         wxMenu* menu = new wxMenu;
         menu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SceneTool::OnComponentMenuEvent), NULL, this);
         menu->Append(0, wxT("Delete Component"));
         mFrame->PopupMenu(menu, wxDefaultPosition);
         delete menu;

         mMenuComponent = NULL;
         return;
      }
   }
} 

void SceneTool::OnEntityMenuEvent(wxCommandEvent& evt)
{
   // Delete Entity
   if (evt.GetId() == 0)
   {
      Plugins::Link.Scene.removeEntity(mMenuEntity);
      mMenuEntity = NULL;
      refreshEntityList();
      return;
   }

   // Add New Component
   if (evt.GetId() > 0)
   {
      Scene::BaseComponent* newComponent = NULL;

      switch (evt.GetId())
      {
         case 1:
            newComponent = new Scene::AnimationComponent();
            break;

         case 2:
            newComponent = new Scene::ControllerComponent();
            break;

         case 3:
            newComponent = new Scene::LightComponent();
            break;

         case 4:
            newComponent = new Scene::MeshComponent();
            break;

         case 5:
            newComponent = new Scene::PhysicsComponent();
            break;

         case 6:
            newComponent = new Scene::TextComponent();
            break;
      }

      newComponent->registerObject();
      mMenuEntity->addComponent(newComponent);
      mMenuEntity->refresh();
      refreshEntityList();
   }
}

void SceneTool::OnComponentMenuEvent(wxCommandEvent& evt)
{
   // Delete Entity
   if (evt.GetId() == 0)
   {
      // Sanity check. mOwnerEntity should never be NULL.
      if (mMenuComponent->mOwnerEntity != NULL)
      {
         mMenuComponent->mOwnerEntity->removeComponent(mMenuComponent);
         mMenuComponent = NULL;
         refreshEntityList();
         return;
      }
   }
}

void SceneTool::OnEntityPropChanged(wxPropertyGridEvent& evt)
{
   wxString name = evt.GetPropertyName();
   wxVariant val = evt.GetPropertyValue();
   wxString strVal = val.GetString();

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

   // Check field type.
   U32 type = mSelectedObject->getDataFieldType(Plugins::Link.StringTableLink->insert(name), NULL);
   if (type == Plugins::Link.Con.TypeColorF)
   {
      wxColour color;
      color << val;
      strVal.Printf("%f %f %f 1.0", color.Red() / 255.0f, color.Green() / 255.0f, color.Blue() / 255.0f);
   }

   mSelectedObject->setDataField(Plugins::Link.StringTableLink->insert(name), NULL, strVal);
   mSelectedEntity->refresh();
}

void SceneTool::OnFeaturePropChanged(wxPropertyGridEvent& evt)
{
   wxString name = evt.GetPropertyName();
   wxVariant val = evt.GetPropertyValue();
   wxString strVal = val.GetString();

   // Check field type.
   U32 type = mSelectedFeature->getDataFieldType(Plugins::Link.StringTableLink->insert(name), NULL);
   if (type == Plugins::Link.Con.TypeColorF)
   {
      wxColour color;
      color << val;
      strVal.Printf("%f %f %f 1.0", color.Red() / 255.0f, color.Green() / 255.0f, color.Blue() / 255.0f);
   }

   mSelectedFeature->setDataField(Plugins::Link.StringTableLink->insert(name), NULL, strVal);
}

void SceneTool::refreshEntityList()
{
   if (!mProjectManager->isProjectLoaded())
      return;

   // Clear list.
   mScenePanel->entityList->DeleteAllItems();
   mEntityListRoot = mScenePanel->entityList->AddRoot("ROOT");

   SimGroup* sceneGroup = Plugins::Link.Scene.getEntityGroup();
   if ( sceneGroup != NULL )
   {
      for(U32 n = 0; n < sceneGroup->size(); ++n)
      {
         Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(sceneGroup->at(n));
         if ( !entity ) continue;
         wxTreeItemId entityItem = mScenePanel->entityList->AppendItem(mEntityListRoot, entity->getName(), 0, -1, new EntityTreeItemData(entity));

         // Iterate components
         for(S32 n = 0; n < entity->mComponents.size(); ++n)
         {
            Scene::BaseComponent* component = entity->mComponents[n];

            wxString compName(component->getClassName());
            const char* internalName = component->getInternalName();
            if ( internalName )
            {
               compName = internalName;
               compName.Append(" (");
               compName.Append(component->getClassName());
               compName.Append(")");
            }

            if ( component )
               mScenePanel->entityList->AppendItem(entityItem, compName, 1, -1, new EntityTreeItemData(component));
         }
      }
   }
}

void SceneTool::refreshFeatureList()
{
   if (!mProjectManager->isProjectLoaded())
      return;

   // Clear list.
   mScenePanel->featureList->DeleteAllItems();
   mEntityListRoot = mScenePanel->featureList->AddRoot("ROOT");

   SimGroup* featureGroup = Plugins::Link.Scene.getFeatureGroup();
   if (featureGroup != NULL)
   {
      for (U32 n = 0; n < featureGroup->size(); ++n)
      {
         Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(featureGroup->at(n));
         if (!feature) continue;
         wxTreeItemId entityItem = mScenePanel->featureList->AppendItem(mFeatureListRoot, feature->mName, 0, -1, new FeatureTreeItemData(feature));
      }
   }
}

void SceneTool::refreshChoices()
{
   if (!mProjectManager->isProjectLoaded())
      return;

   mMaterialChoices.Clear();
   mMeshChoices.Clear();
   mEntityTemplateChoices.Clear();

   Vector<const AssetDefinition*> assetDefinitions = Plugins::Link.AssetDatabaseLink.getDeclaredAssets();

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

      // Entity Templates
      if (dStrcmp(pAssetDefinition->mAssetType, "EntityTemplateAsset") == 0)
         mEntityTemplateChoices.Add(pAssetDefinition->mAssetId, mMeshChoices.GetCount());
      
   }
}

static S32 QSORT_CALLBACK compareEntries(const void* a, const void* b)
{
   SimFieldDictionary::Entry *fa = *((SimFieldDictionary::Entry **)a);
   SimFieldDictionary::Entry *fb = *((SimFieldDictionary::Entry **)b);
   return dStricmp(fa->slotName, fb->slotName);
}

void SceneTool::loadObjectProperties(wxPropertyGrid* propertyGrid, SimObject* obj)
{
   propertyGrid->Clear();

   // Add static fields.
   wxString fieldGroup("");
   bool addFieldGroup = false;
   AbstractClassRep::FieldList fieldList = obj->getFieldList();
   for(Vector<AbstractClassRep::Field>::iterator itr = fieldList.begin(); itr != fieldList.end(); itr++)
   {
      const AbstractClassRep::Field* f = itr;
      if( f->type == AbstractClassRep::DepricatedFieldType ||
            f->type == AbstractClassRep::EndGroupFieldType) 
         continue;

      if ( f->type == AbstractClassRep::StartGroupFieldType )
      {
         addFieldGroup = true;
         fieldGroup = f->pGroupname;
         continue;
      }

      for (U32 j = 0; S32(j) < f->elementCount; j++)
      {
         const char *val = (*f->getDataFn)(obj, Plugins::Link.Con.getData(f->type, (void *)(((const char *)obj) + f->offset), j, f->table, f->flag));

         if (!val)
            continue;

         if (addFieldGroup)
         {
            propertyGrid->Append(new wxPropertyCategory(fieldGroup));
            addFieldGroup = false;
         }

         if (dStrcmp(f->pFieldname, "MeshAsset") == 0)
            propertyGrid->Append(new wxEnumProperty("MeshAsset", wxPG_LABEL, mMeshChoices));
         else if (f->type == Plugins::Link.Con.TypeColorF)
         {
            ColorF colorVal;
            Plugins::Link.Con.setData(Plugins::Link.Con.TypeColorF, &colorVal, 0, 1, &val, NULL, 0);
            wxColour color;
            color.Set(colorVal.red * 255, colorVal.green * 255, colorVal.blue * 255, 255);
            propertyGrid->Append(new wxColourProperty(f->pFieldname, f->pFieldname, color));
         }
         else if (f->type == Plugins::Link.Con.TypeBool)
            propertyGrid->Append(new wxBoolProperty(f->pFieldname, f->pFieldname, val));
         else
            propertyGrid->Append(new wxStringProperty(f->pFieldname, f->pFieldname, val));
      }
   }

   // Get list of dynamic fields and sort by name
   Vector<SimFieldDictionary::Entry *> flist;
   SimFieldDictionary* fieldDictionary = obj->getFieldDictionary();
   for (SimFieldDictionaryIterator ditr(fieldDictionary); *ditr; ++ditr)
      flist.push_back(*ditr);
   dQsort(flist.address(), flist.size(), sizeof(SimFieldDictionary::Entry *), compareEntries);

   // Add dynamic fields.
   wxPGProperty* materialsCategory = NULL;
   wxPGProperty* submeshCategory = NULL;
   wxPGProperty* otherCategory = NULL;
   for (U32 i = 0; i < (U32)flist.size(); i++)
   {
      SimFieldDictionary::Entry* entry = flist[i];

      if (dStrncmp(entry->slotName, "MaterialAsset", 13) == 0)
      {
         if (materialsCategory == NULL)
            materialsCategory = propertyGrid->Append(new wxPropertyCategory("Materials"));
         propertyGrid->Append(new wxEnumProperty(entry->slotName, wxPG_LABEL, mMaterialChoices));
      }
      else if (dStrncmp(entry->slotName, "SubMesh", 7) == 0)
      {
         if (submeshCategory == NULL)
            submeshCategory = propertyGrid->Append(new wxPropertyCategory("SubMeshes"));
         propertyGrid->AppendIn(submeshCategory, new wxBoolProperty(entry->slotName, wxPG_LABEL, dAtob(entry->value)));
      }
      else
      {
         if (otherCategory == NULL)
            otherCategory = propertyGrid->Append(new wxPropertyCategory("Other"));
         propertyGrid->AppendIn(otherCategory, new wxStringProperty(entry->slotName, entry->slotName, entry->value));
      }
   }
}

void SceneTool::selectEntity(Scene::SceneEntity* entity)
{
   mSelectedObject = entity;
   mSelectedEntity = entity; 
   mSelectedComponent = NULL;

   mGizmo.selectEntity(entity);

   loadObjectProperties(mScenePanel->propertyGrid, entity);
}

void SceneTool::selectComponent(Scene::BaseComponent* component)
{
   mSelectedObject = component;
   mSelectedEntity = component->mOwnerEntity;
   mSelectedComponent = component;

   mGizmo.selectComponent(component);

   loadObjectProperties(mScenePanel->propertyGrid, component);
}

void SceneTool::OnToolbarDropdownEvent(wxCommandEvent& evt)
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

void SceneTool::OnTranslateMenuEvent(wxCommandEvent& evt)
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

void SceneTool::OnRotateMenuEvent(wxCommandEvent& evt)
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

void SceneTool::OnScaleMenuEvent(wxCommandEvent& evt)
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