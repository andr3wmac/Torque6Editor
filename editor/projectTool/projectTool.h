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

#ifndef _PROJECT_TOOL_H_
#define _PROJECT_TOOL_H_

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
#include <3d/scene/object/object.h>
#endif

struct AssetCategory
{
   const char* categoryName;
   Vector<const AssetDefinition*> assets;
   wxTreeItemId treeItemID;
};

struct Module
{
   const char* moduleID;
   U32 moduleVersion;
   Vector<AssetCategory> assets;
   wxTreeItemId treeItemID;
};

class ModuleTreeItemData : public wxTreeItemData
{
public:
   Module obj;

   ModuleTreeItemData(Module _obj)
      : obj(_obj)
   {
   }
};

class AssetCategoryTreeItemData : public wxTreeItemData
{
public:
   AssetCategory obj;
   
   AssetCategoryTreeItemData(AssetCategory _obj)
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

class ProjectTool : public wxEvtHandler, public EditorTool
{
   typedef EditorTool Parent;

   protected:
      ProjectPanel*  mProjectPanel;
      wxImageList*   mAssetIconList;
      wxTreeItemId   mAssetListRoot;

      ModuleDefinition*       mSelectedModule;
      const AssetDefinition*  mSelectedAssetDef;
      AssetBase*              mSelectedAsset;
      MaterialAsset*          mSelectedMaterialAsset;
      wxPGChoices             mTextureAssetChoices;

   public:
      ProjectTool(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager);
      ~ProjectTool();

      const char* getAssetCategoryName(const char* _name);

      void refresh();
      void refreshAssetList();
      void refreshChoices();

      void OnTreeDrag(wxTreeEvent& evt);
      void OnTreeEvent(wxTreeEvent& evt);
      void OnTreeMenu(wxTreeEvent& evt);
      void OnMenuEvent(wxCommandEvent& evt);
      void OnPropertyChanged(wxPropertyGridEvent& evt);

      virtual void initTool();
      virtual void openTool();
      virtual void closeTool();

      virtual void onProjectLoaded(const wxString& projectName, const wxString& projectPath);
      virtual void onProjectClosed();

      void loadAssetDefinitionProperties(wxPropertyGrid* propertyGrid, const AssetDefinition* assetDef);
};

#endif // _PROJECT_TOOL_H_
