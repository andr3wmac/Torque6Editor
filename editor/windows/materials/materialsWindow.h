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

#ifndef MATERIALS_WINDOW_H
#define MATERIALS_WINDOW_H

#ifndef EDITORMANAGER_H
#include "editorManager.h"
#endif

#ifndef __TORQUE6EDITORUI_H__
#include "Torque6EditorUI.h"
#endif

#ifndef _MATERIAL_WINDOW_H_
#include "materialWindow.h"
#endif

#ifndef _WX_TREECTRL_H_BASE_
#include <wx/treectrl.h>
#endif

#ifndef WXTORQUEINSPECTOR_H
#include "widgets/wxTorqueInspector/wxTorqueInspector.h"
#endif

class MaterialsTreeItemData : public wxTreeItemData
{
public:
   StringTableEntry assetId;
   
   MaterialsTreeItemData(StringTableEntry _assetId)
      :  assetId(_assetId)
   {
   }
};

class MaterialsWindow : public wxEvtHandler, public EditorWindow, public wxTorqueInspectorDelegate
{
   typedef EditorWindow Parent;

   public:
      MaterialsPanel*         mMaterialsPanel;
      wxTreeItemId            mMaterialTreeRoot;
      wxImageList*            mIconList;
      Node*                   mSelectedNode;
      MaterialWindow*         mSelectedNodeParent;
      ModuleDefinition*       mSelectedModule;
      wxTorqueInspector*      mInspector;
      //Vector<MaterialWindow*> mMaterialWindows;

      MaterialsWindow(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager);
      ~MaterialsWindow();

      void refreshMaterialList();
      void selectNode(MaterialWindow* parent, Node* node);

      virtual void OnMenuEvent(wxCommandEvent& evt);
      virtual void OnTreeEvent(wxTreeEvent& evt);
      virtual void OnTreeMenu(wxTreeEvent& evt);
      virtual void OnModuleMenuEvent(wxCommandEvent& evt);

      // wxTorqueInspectorDelegate
      void OnFieldChanged(wxString name, wxVariant value);

      virtual void initWindow();
      virtual void openWindow();
      virtual void closeWindow();

      void openMaterial(StringTableEntry assetId);
      

      virtual void onProjectLoaded(const wxString& projectName, const wxString& projectPath);
      virtual void onProjectClosed();
};

#endif // MATERIALS_WINDOW_H
