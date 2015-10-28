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

#ifndef _SCENE_TOOL_H_
#define _SCENE_TOOL_H_

#ifndef _PROJECTMANAGER_H_
#include "../project/projectManager.h"
#endif

#ifndef __TORQUE6EDITORUI_H__
#include "../Torque6EditorUI.h"
#endif

#ifndef _WX_TREECTRL_H_BASE_
#include <wx/treectrl.h>
#endif

#ifndef _SCENEENTITY_H_
#include <3d/entity/entity.h>
#endif

#ifndef _GIZMO_H_
#include "gizmo.h"
#endif

class EntityTreeItemData : public wxTreeItemData
{
public:
   SimObject* objPtr;
   
   EntityTreeItemData(SimObject* _objPtr)
      :  objPtr(_objPtr)
   {
   }
};

class FeatureTreeItemData : public wxTreeItemData
{
public:
   SimObject* objPtr;

   FeatureTreeItemData(SimObject* _objPtr)
      : objPtr(_objPtr)
   {
   }
};

class SceneTool : public wxEvtHandler, public EditorTool
{
   typedef EditorTool Parent;

   protected:
      ScenePanel*    mScenePanel;
      wxImageList*   mEntityIconList;
      wxTreeItemId   mEntityListRoot;

      wxImageList*   mFeatureIconList;
      wxTreeItemId   mFeatureListRoot;
      
      SimObject*              mSelectedObject;
      Scene::SceneEntity*     mSelectedEntity;
      Scene::BaseComponent*   mSelectedComponent;
      SimObject*              mSelectedFeature;

      Scene::SceneEntity*     mMenuEntity;
      Scene::BaseComponent*   mMenuComponent;

      wxMenu* mTranslateMenu;
      wxMenu* mRotateMenu;
      wxMenu* mScaleMenu;

      wxPGChoices mMeshChoices;
      wxPGChoices mMaterialChoices;
      wxPGChoices mEntityTemplateChoices;

      Gizmo mGizmo;

      TextureObject* mLightIcon;

   public:
      SceneTool(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager);
      ~SceneTool();

      void refreshEntityList();
      void refreshFeatureList();
      void refreshChoices();
      void loadObjectProperties(wxPropertyGrid* propertyGrid, SimObject* obj);
      void selectEntity(Scene::SceneEntity* entity);
      void selectComponent(Scene::BaseComponent* component);
      void openAddEntityMenu();

      void OnMenuEvent(wxCommandEvent& evt);
      void OnTreeEvent( wxTreeEvent& evt );
      void OnTreeMenu( wxTreeEvent& evt );
      void OnEntityPropChanged(wxPropertyGridEvent& evt);
      void OnFeaturePropChanged( wxPropertyGridEvent& evt );
      void OnToolbarDropdownEvent(wxCommandEvent& evt);
      void OnTranslateMenuEvent(wxCommandEvent& evt);
      void OnRotateMenuEvent(wxCommandEvent& evt);
      void OnScaleMenuEvent(wxCommandEvent& evt);
      void OnAddEntityMenuEvent(wxCommandEvent& evt);
      void OnEntityMenuEvent(wxCommandEvent& evt);
      void OnComponentMenuEvent(wxCommandEvent& evt);

      virtual void initTool();
      virtual void openTool();
      virtual void closeTool();
      virtual void renderTool();

      virtual bool onMouseLeftDown(int x, int y);
      virtual bool onMouseLeftUp(int x, int y);
      virtual bool onMouseMove(int x, int y);

      virtual void onSceneChanged();
      virtual void onProjectLoaded(const wxString& projectName, const wxString& projectPath);
      virtual void onProjectClosed();
};

#endif // _SCENE_TOOL_H_
