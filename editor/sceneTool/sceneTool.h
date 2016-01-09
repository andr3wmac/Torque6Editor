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

#ifndef _SCENE_OBJECT_H_
#include <scene/object.h>
#endif

#ifndef _GIZMO_H_
#include "gizmo.h"
#endif

class ObjectTreeItemData : public wxTreeItemData
{
public:
   SimObject* objPtr;
   
   ObjectTreeItemData(SimObject* _objPtr)
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
      wxImageList*   mObjectIconList;
      wxTreeItemId   mObjectListRoot;

      wxImageList*   mFeatureIconList;
      wxTreeItemId   mFeatureListRoot;
      
      Scene::SceneObject*     mSelectedObject;
      Scene::BaseComponent*   mSelectedComponent;

      Scene::SceneObject*     mMenuObject;
      Scene::BaseComponent*   mMenuComponent;

      Vector<StringTableEntry> mComponentClassList;
      Vector<StringTableEntry> mFeatureClassList;

      wxMenu* mTranslateMenu;
      wxMenu* mRotateMenu;
      wxMenu* mScaleMenu;

      wxPGChoices mMeshChoices;
      wxPGChoices mMaterialChoices;
      wxPGChoices mObjectTemplateChoices;

      bool mRefreshing;
      Gizmo mGizmo;

      TextureObject* mLightIcon;

   public:
      SceneTool(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager);
      ~SceneTool();

      void refreshClassLists();
      void refreshObjectList();
      void refreshFeatureList();
      void refreshChoices();
      void loadObjectProperties(wxPropertyGrid* propertyGrid, SimObject* obj);
      void selectObject(Scene::SceneObject* obj, bool updateTree = false);
      void addComponent(Scene::SceneObject* obj, StringTableEntry componentClassName);
      void selectComponent(Scene::BaseComponent* componenty, bool updateTree = false);
      void openAddObjectMenu();
      void openAddComponentMenu();
      void openAddFeatureMenu();

      void OnMenuEvent(wxCommandEvent& evt);
      void OnTreeEvent( wxTreeEvent& evt );
      void OnTreeMenu( wxTreeEvent& evt );
      void OnObjectPropChanged(wxPropertyGridEvent& evt);
      void OnFeaturePropChanged( wxPropertyGridEvent& evt );
      void OnToolbarDropdownEvent(wxCommandEvent& evt);
      void OnTranslateMenuEvent(wxCommandEvent& evt);
      void OnRotateMenuEvent(wxCommandEvent& evt);
      void OnScaleMenuEvent(wxCommandEvent& evt);
      void OnAddObjectMenuEvent(wxCommandEvent& evt);
      void OnAddComponentMenuEvent(wxCommandEvent& evt);
      void OnAddFeatureMenuEvent(wxCommandEvent& evt);
      void OnObjectMenuEvent(wxCommandEvent& evt);
      void OnComponentMenuEvent(wxCommandEvent& evt);
      void OnFeatureMenuEvent(wxCommandEvent& evt);

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
