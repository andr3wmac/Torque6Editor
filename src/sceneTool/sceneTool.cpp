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

SceneTool::SceneTool(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager)
   : Parent(_projectManager, _frame, _manager),
     mScenePanel(NULL),
     mSelectedObject(NULL),
     mSelectedEntity(NULL),
     mSelectedFeature(NULL)
{
   mEntityIconList = new wxImageList(16, 16);
   mFeatureIconList = new wxImageList( 16, 16 );
}

SceneTool::~SceneTool()
{

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
   if ( mSelectedObject != NULL )
   {
      Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(mSelectedObject);
      if (entity)
      {
         F32 transform[16];
         bx::mtxSRT(transform,
            entity->mScale.x, entity->mScale.y, entity->mScale.z,
            entity->mRotation.x, entity->mRotation.y, entity->mRotation.z,
            entity->mPosition.x, entity->mPosition.y, entity->mPosition.z);

         // Bounding Box
         Plugins::Link.Graphics.drawBox3D(mProjectManager->mRenderLayer4View->id, entity->mBoundingBox, ColorI(255, 255, 255, 255), NULL);
      }

      mGizmo.render();
   }
}

bool SceneTool::onMouseLeftDown(int x, int y)
{
   Point3F worldRay = Plugins::Link.Rendering.screenToWorld(Point2I(x, y));
   Point3F editorPos = Plugins::Link.Scene.getActiveCamera()->getPosition();

   Scene::SceneEntity* hit = Plugins::Link.Scene.raycast(editorPos, editorPos + (worldRay * 1000.0f));
   if (mSelectedObject != hit)
   {
      if (hit)
         selectEntity(hit);
   }
   else 
   {
      mGizmo.onMouseLeftDown(x, y);
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
}

void SceneTool::onProjectClosed()
{
   //
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

   if (evt.GetId() == ADD_FEATURE_DLAA)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("DLAA"));
      Plugins::Link.Scene.addFeature(feature);
   }

   if (evt.GetId() == ADD_FEATURE_SSAO)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("SSAO"));
      Plugins::Link.Scene.addFeature(feature);
   }

   if (evt.GetId() == ADD_FEATURE_HDR)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("HDR"));
      Plugins::Link.Scene.addFeature(feature);
   }

   if (evt.GetId() == ADD_FEATURE_SKYBOX)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("SimpleSkybox"));
      Plugins::Link.Scene.addFeature(feature);
   }

   if (evt.GetId() == ADD_FEATURE_DIRLIGHT)
   {
      Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(Plugins::Link.Con.createObject("DirectionalLight"));
      Plugins::Link.Scene.addFeature(feature);
   }

   refreshEntityList();
   refreshFeatureList();
   refreshChoices();
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
   wxMenu* menu = new wxMenu; 
   menu->Append(wxID_OPEN, wxT("Add Empty Entity")); 
   menu->Append(wxID_OPEN, wxT("Add Entity from Template")); 
   menu->AppendSeparator();
   
   wxMenu* compMenu = new wxMenu;
   compMenu->Append(wxID_OPEN, wxT("AnimationComponent")); 
   compMenu->Append(wxID_OPEN, wxT("LightComponent")); 
   compMenu->Append(wxID_OPEN, wxT("MeshComponent")); 
   compMenu->Append(wxID_OPEN, wxT("PhysicsComponent")); 
   compMenu->Append(wxID_OPEN, wxT("TextComponent")); 
   menu->AppendSubMenu(compMenu, wxT("Add Component"));

   menu->AppendSeparator();

   menu->Append(wxID_OPEN, wxT("Delete")); 
   mFrame->PopupMenu(menu, wxDefaultPosition); 
   delete menu; 
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
   else if (name.StartsWith("Material"))
   {
      long intVal = val.GetInteger();
      strVal = mMaterialChoices.GetLabel(intVal);
   }

   mSelectedObject->setDataField(Plugins::Link.StringTableLink->insert(name), NULL, strVal);
   mSelectedEntity->refresh();
}

void SceneTool::OnFeaturePropChanged(wxPropertyGridEvent& evt)
{
   wxString name = evt.GetPropertyName();
   wxVariant val = evt.GetPropertyValue();
   wxString strVal = val.GetString();

   mSelectedFeature->setField(name, strVal);
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

      for(U32 j = 0; S32(j) < f->elementCount; j++)
      {                      
         const char *val = (*f->getDataFn)( obj, Plugins::Link.Con.getData(f->type, (void *) (((const char *)obj) + f->offset), j, f->table, f->flag) );

         if( !val )
            continue;

         if ( addFieldGroup )
         {
            propertyGrid->Append( new wxPropertyCategory(fieldGroup) );
            addFieldGroup = false;
         }

         if (dStrcmp(f->pFieldname, "MeshAsset") == 0)
            propertyGrid->Append(new wxEnumProperty("MeshAsset", wxPG_LABEL, mMeshChoices));
         else if (dStrncmp(f->pFieldname, "Material", 8) == 0)
            propertyGrid->Append(new wxEnumProperty(f->pFieldname, wxPG_LABEL, mMaterialChoices));
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
   propertyGrid->Append(new wxPropertyCategory("Other"));
   for (U32 i = 0; i < (U32)flist.size(); i++)
   {
      SimFieldDictionary::Entry* entry = flist[i];

      if (dStrncmp(entry->slotName, "Material", 8) == 0)
         propertyGrid->Append(new wxEnumProperty(entry->slotName, wxPG_LABEL, mMaterialChoices));
      else
         propertyGrid->Append(new wxStringProperty(entry->slotName, entry->slotName, entry->value));
   }
}

void SceneTool::selectEntity(Scene::SceneEntity* entity)
{
   mSelectedObject = entity;
   mSelectedEntity = entity;

   mGizmo.selectEntity(entity);

   loadObjectProperties(mScenePanel->propertyGrid, entity);
}

void SceneTool::selectComponent(Scene::BaseComponent* component)
{
   mSelectedObject = component;
   mSelectedEntity = component->mOwnerEntity;

   loadObjectProperties(mScenePanel->propertyGrid, component);
}