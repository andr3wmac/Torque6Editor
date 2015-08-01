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

SceneTool::SceneTool()
   : mScenePanel(NULL),
     mSelectedObject(NULL)
{
   mIconList = new wxImageList( 16, 16 );
}

SceneTool::~SceneTool()
{

}

void SceneTool::initTool()
{
   mScenePanel = new ScenePanel(mFrame, wxID_ANY);

   // Icons.
   mIconList->Add(wxBitmap("images/entityIcon.png", wxBITMAP_TYPE_PNG));
   mIconList->Add(wxBitmap("images/componentIcon.png", wxBITMAP_TYPE_PNG));
   mScenePanel->entityList->AssignImageList(mIconList);

   // Events
   mScenePanel->entityList->Connect(wxID_ANY, wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(SceneTool::OnTreeEvent), NULL, this);
   mScenePanel->entityList->Connect(wxID_ANY, wxEVT_TREE_ITEM_MENU, wxTreeEventHandler(SceneTool::OnTreeMenu), NULL, this);
   mScenePanel->propertyGrid->Connect(wxID_ANY, wxEVT_PG_CHANGED, wxPropertyGridEventHandler(SceneTool::OnPropertyChanged), NULL, this);

   mEntityListRoot = mScenePanel->entityList->AddRoot("ROOT");

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
}

void SceneTool::openTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mScenePanel);
   paneInfo.Show();
   mManager->Update();

   if ( mProjectManager->mProjectLoaded )
      refreshEntityList();
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
      Plugins::Link.Graphics.drawBox3D(mSelectedBoundingBox, ColorI(255, 255, 255, 255), 2.0f);
   }
}

void SceneTool::onProjectLoaded(wxString projectName, wxString projectPath)
{
   refreshEntityList();
}

void SceneTool::onProjectClosed()
{
   //
}

void SceneTool::OnTreeEvent( wxTreeEvent& evt )
{
   EntityTreeItemData* data = dynamic_cast<EntityTreeItemData*>(mScenePanel->entityList->GetItemData(evt.GetItem()));
   if ( data )
   {
      // Did we select an entity ..
      Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(data->objPtr);
      if ( entity )
      {
         mSelectedObject = entity;
         mSelectedBoundingBox = entity->mBoundingBox;
         loadObjectProperties(entity);
         return;
      }

      // .. or a component?
      Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(data->objPtr);
      if ( component )
      {
         mSelectedObject = component;
         mSelectedBoundingBox = component->getBoundingBox();
         loadObjectProperties(component);
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

void SceneTool::OnPropertyChanged( wxPropertyGridEvent& evt ) 
{ 
   wxString name = evt.GetPropertyName();
   wxVariant val = evt.GetPropertyValue();
   wxString strVal = val.GetString();

   mSelectedObject->setField(name, strVal);
   Plugins::Link.Scene.refresh();
}

void SceneTool::refreshEntityList()
{
   SimGroup* sceneGroup = Plugins::Link.Scene.getEntityGroup();
   if ( sceneGroup != NULL )
   {
      for(U32 n = 0; n < sceneGroup->size(); ++n)
      {
         Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(sceneGroup->at(n));
         if ( !entity ) continue;
         wxTreeItemId entityItem = mScenePanel->entityList->AppendItem(mEntityListRoot, entity->mTemplateAssetID, 0, -1, new EntityTreeItemData(entity));

         // Iterate components
         Scene::EntityTemplate* entityTemplate = entity->mTemplate;
         for(S32 n = 0; n < entityTemplate->size(); ++n)
         {
            Scene::BaseComponent* component = static_cast<Scene::BaseComponent*>(entityTemplate->at(n));

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

void SceneTool::loadObjectProperties(SimObject* obj)
{
   mScenePanel->propertyGrid->Clear();

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
            mScenePanel->propertyGrid->Append( new wxPropertyCategory(fieldGroup) );
            addFieldGroup = false;
         }

         if ( f->type ==  Plugins::Link.Con.TypeBool )
            mScenePanel->propertyGrid->Append( new wxBoolProperty(f->pFieldname, f->pFieldname, val) );
         else 
            mScenePanel->propertyGrid->Append( new wxStringProperty(f->pFieldname, f->pFieldname, val) );
      }
   }
}