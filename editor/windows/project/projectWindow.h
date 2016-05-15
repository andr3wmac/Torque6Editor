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

#ifndef PROJECT_WINDOW_H
#define PROJECT_WINDOW_H

#ifndef EDITORMANAGER_H
#include "editorManager.h"
#endif

#ifndef __TORQUE6EDITORUI_H__
#include "Torque6EditorUI.h"
#endif

#ifndef _WX_TREECTRL_H_BASE_
#include <wx/treectrl.h>
#endif

#ifndef _SCENE_OBJECT_H_
#include <scene/object.h>
#endif

#ifndef WXFLATNOTEBOOK_H
#include "widgets/wxFlatNotebook/wxFlatNotebook.h"
#endif

class ModuleTreeItemData : public wxTreeItemData
{
public:
   ModuleInfo obj;

   ModuleTreeItemData(ModuleInfo _obj)
      : obj(_obj)
   {
   }
};

class AssetCategoryTreeItemData : public wxTreeItemData
{
public:
   AssetCategoryInfo obj;
   
   AssetCategoryTreeItemData(AssetCategoryInfo _obj)
      :  obj(_obj)
   {
   }
};

class AssetTreeItemData : public wxTreeItemData
{
public:
   const AssetDefinition* objPtr;

   AssetTreeItemData(const AssetDefinition* _objPtr)
      : objPtr(_objPtr)
   {
   }
};

class ModuleDefinition;
class wxTorqueInspector;

class ProjectWindow : public wxEvtHandler, public EditorWindow
{
   typedef EditorWindow Parent;

   protected:
      wxFlatNotebook*         mTabs;
      ProjectPanel*           mProjectPanel;
      ProjectPanel_Project*   mProjectTab;
      ProjectPanel_Assets*    mAssetsTab;
      wxTorqueInspector*      mAssetsInspector;
      wxImageList*            mAssetIconList;
      wxTreeItemId            mAssetListRoot;

      ModuleDefinition*       mSelectedModule;
      const AssetDefinition*  mSelectedAssetDef;
      AssetBase*              mSelectedAsset;
      MaterialAsset*          mSelectedMaterialAsset;

   public:
      ProjectWindow(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager);
      ~ProjectWindow();

      const char* getAssetCategoryName(const char* _name);

      void refresh();
      void refreshAssetList();

      void OnTreeDrag(wxTreeEvent& evt);
      void OnTreeEvent(wxTreeEvent& evt);
      void OnTreeMenu(wxTreeEvent& evt);
      void OnMenuEvent(wxCommandEvent& evt);
      void OnPropertyChanged(wxPropertyGridEvent& evt);

      virtual void initWindow();
      virtual void openWindow();
      virtual void closeWindow();

      virtual void onProjectLoaded(const wxString& projectName, const wxString& projectPath);
      virtual void onProjectClosed();
};

#endif // PROJECT_WINDOW_H
