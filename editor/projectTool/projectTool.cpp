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

#include "../widgets/wxTorqueInspector/wxTorqueInspector.h"
#include "../theme.h"

#include "projectTool.h"
#include "module/moduleManager.h"
#include "materials/materialAsset.h"
#include <bx/bx.h>
#include <bx/fpumath.h>

ProjectTool::ProjectTool(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager)
   : Parent(_projectManager, _frame, _manager),
     mProjectPanel(NULL),
     mSelectedModule(NULL),
     mSelectedAssetDef(NULL),
     mSelectedAsset(NULL),
     mSelectedMaterialAsset(NULL)
{
   mAssetIconList = new wxImageList(16, 16);
}

ProjectTool::~ProjectTool()
{

}

void ProjectTool::initTool()
{
   mProjectPanel = new ProjectPanel(mFrame, wxID_ANY);

   // Load Tabs
   mTabs = new wxFlatNotebook(mProjectPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFNB_NO_X_BUTTON | wxFNB_NO_NAV_BUTTONS);
   mTabs->SetForegroundColour(Theme::lightBackgroundColor);
   mTabs->SetBackgroundColour(Theme::lightBackgroundColor);
   mTabs->SetGradientColors(Theme::lightBackgroundColor, Theme::lightBackgroundColor, Theme::darkBackgroundColor);
   mTabs->SetActiveTabColour(Theme::darkBackgroundColor);
   mTabs->SetActiveTabTextColour(wxColor(255, 255, 255));
   mTabs->SetNonActiveTabTextColour(wxColor(255, 255, 255));
   mTabs->SetTabAreaColour(Theme::lightBackgroundColor);

   // Project Tab
   mProjectTab = new ProjectPanel_Project(mTabs);
   mTabs->AddPage(mProjectTab, "Project ");

   // Assets Tab
   mAssetsTab = new ProjectPanel_Assets(mTabs);
   mTabs->AddPage(mAssetsTab, "Assets ");

   // Add Tabs to ProjectPanel
   mProjectPanel->ProjectPanelContent->Add(mTabs, 1, wxEXPAND | wxALL, 5);

   // Add TorqueInspector to scene panel objects.
   mAssetsInspector = new wxTorqueInspector(mProjectManager, mAssetsTab->InspectorWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize);
   mAssetsTab->InspectorContents->Add(mAssetsInspector, 1, wxALL | wxEXPAND, 1);

   // Assets Icons
   mAssetIconList->Add(wxBitmap("images/moduleIcon.png", wxBITMAP_TYPE_PNG));
   mAssetIconList->Add(wxBitmap("images/iconFolderGrey.png", wxBITMAP_TYPE_PNG));
   mAssetIconList->Add(wxBitmap("images/assetIcon.png", wxBITMAP_TYPE_PNG));
   mAssetsTab->assetList->AssignImageList(mAssetIconList);

   // Assets Events
   mAssetsTab->assetList->Connect(wxID_ANY, wxEVT_TREE_BEGIN_DRAG, wxTreeEventHandler(ProjectTool::OnTreeDrag), NULL, this);
   mAssetsTab->assetList->Connect(wxID_ANY, wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(ProjectTool::OnTreeEvent), NULL, this);
   mAssetsTab->assetList->Connect(wxID_ANY, wxEVT_TREE_ITEM_MENU, wxTreeEventHandler(ProjectTool::OnTreeMenu), NULL, this);
   
   // Assets Menu Events
   mProjectPanel->moduleMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ProjectTool::OnMenuEvent), NULL, this);

   // Assets List
   mAssetListRoot = mAssetsTab->assetList->AddRoot("ROOT");

   mManager->AddPane(mProjectPanel, wxAuiPaneInfo().Caption("Project")
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

void ProjectTool::openTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mProjectPanel);
   paneInfo.Show();
   mManager->Update();

   if (mProjectManager->mProjectLoaded)
      refresh();
}

void ProjectTool::closeTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mProjectPanel);
   paneInfo.Hide();
   mManager->Update();
}

void ProjectTool::onProjectLoaded(const wxString& projectName, const wxString& projectPath)
{
   refresh();
}

void ProjectTool::onProjectClosed()
{
   //
}

void ProjectTool::OnTreeDrag(wxTreeEvent& evt)
{
   if (evt.GetId() == ASSET_LIST)
   {
      AssetTreeItemData* data = dynamic_cast<AssetTreeItemData*>(mAssetsTab->assetList->GetItemData(evt.GetItem()));
      if (data)
      {
         const AssetDefinition* asset = data->objPtr;

         wxString command("Asset->");
         command.Append(asset->mAssetType);
         command.Append("->");
         command.Append(asset->mAssetId);

         wxTextDataObject dragData(command);
         wxDropSource dragSource(mProjectPanel);
         dragSource.SetData(dragData);
         wxDragResult result = dragSource.DoDragDrop(TRUE);
         return;
      }
   }
}

void ProjectTool::OnTreeEvent( wxTreeEvent& evt )
{
   if (evt.GetId() == ASSET_LIST)
   {
      AssetTreeItemData* data = dynamic_cast<AssetTreeItemData*>(mAssetsTab->assetList->GetItemData(evt.GetItem()));
      if (data)
      {
         mAssetsInspector->Inspect(data->objPtr);
         return;
      }
   }
}

void ProjectTool::OnTreeMenu( wxTreeEvent& evt )
{ 
   mSelectedModule = NULL;

   if (evt.GetId() == ASSET_LIST)
   {
      ModuleTreeItemData* module_data = dynamic_cast<ModuleTreeItemData*>(mAssetsTab->assetList->GetItemData(evt.GetItem()));
      if (module_data)
      {
         ModuleInfo mod = module_data->obj;
         mSelectedModule = Torque::ModuleDatabaseLink->findLoadedModule(mod.moduleID);
         if (mSelectedModule != NULL)
            mFrame->PopupMenu(mProjectPanel->moduleMenu, wxDefaultPosition);
         return;
      }
   }
} 

void ProjectTool::OnMenuEvent(wxCommandEvent& evt)
{
   if (evt.GetId() == MENU_IMPORT_MESH)
   {
      ImportMeshWizard* wizard = new ImportMeshWizard(mFrame);

      // Set initial import path, the user can change it.
      wxString importPath = mSelectedModule->getModulePath();
      importPath.Append("/meshes");
      wizard->importPath->SetPath(importPath);

      if (wizard->RunWizard(wizard->m_pages[0]))
      {
         wxString assetID     = wizard->assetID->GetValue();
         wxString meshPath    = wizard->meshFilePath->GetFileName().GetFullPath();
         wxString meshFile    = wizard->meshFilePath->GetFileName().GetFullName();
         wxString importPath  = wizard->importPath->GetPath();

         // Copy file (optional)
         if (wizard->copyMeshCheck->GetValue())
         {
            wxString moduleMeshPath(importPath);
            moduleMeshPath.Append("/");
            moduleMeshPath.Append(meshFile);

            Torque::Platform.createPath(moduleMeshPath.c_str());
            Torque::Platform.pathCopy(meshPath.c_str(), moduleMeshPath.c_str(), false);
            meshPath = moduleMeshPath;
         }

         // Make path relative to module directory.
         char buf[1024];
         const char* fullPath = Torque::Platform.makeFullPathName(meshPath.c_str(), buf, sizeof(buf), NULL);
         StringTableEntry relativePath = Torque::Platform.makeRelativePathName(fullPath, importPath);

         // Create full import path.
         importPath.Append("/");
         importPath.Append(assetID);
         importPath.Append(".asset.taml");

         // Create asset definition.
         Torque::Scene.createMeshAsset(assetID.c_str(), relativePath, importPath.c_str());
         Torque::AssetDatabaseLink.addDeclaredAsset(mSelectedModule, importPath.c_str());
         refresh();
      }

      wizard->Destroy();
   }

   if (evt.GetId() == MENU_IMPORT_TEXTURE)
   {
      ImportTextureWizard* wizard = new ImportTextureWizard(mFrame);
      
      // Set initial import path, the user can change it.
      wxString importPath = mSelectedModule->getModulePath();
      importPath.Append("/textures");
      wizard->importPath->SetPath(importPath);

      if (wizard->RunWizard(wizard->m_pages[0]))
      {
         wxString assetID = wizard->assetID->GetValue();
         wxString texturePath = wizard->textureFilePath->GetFileName().GetFullPath();
         wxString textureFile = wizard->textureFilePath->GetFileName().GetFullName();
         wxString importPath = wizard->importPath->GetPath();

         // Copy file (optional)
         if (wizard->copyTextureCheck->GetValue())
         {
            wxString moduleTexturePath(importPath);
            moduleTexturePath.Append("/");
            moduleTexturePath.Append(textureFile);

            Torque::Platform.createPath(moduleTexturePath.c_str());
            Torque::Platform.pathCopy(texturePath.c_str(), moduleTexturePath.c_str(), false);
            texturePath = moduleTexturePath;
         }

         // Make path relative to module directory.
         char buf[1024];
         const char* fullPath = Torque::Platform.makeFullPathName(texturePath.c_str(), buf, sizeof(buf), NULL);
         StringTableEntry relativePath = Torque::Platform.makeRelativePathName(fullPath, importPath);

         // Create full import path.
         importPath.Append("/");
         importPath.Append(assetID);
         importPath.Append(".asset.taml");

         // Create asset definition.
         Torque::Scene.createTextureAsset(assetID.c_str(), relativePath, importPath.c_str());
         Torque::AssetDatabaseLink.addDeclaredAsset(mSelectedModule, importPath.c_str());
         refresh();
      }

      wizard->Destroy();
   }

   if (evt.GetId() == MENU_NEW_MATERIAL)
   {
      NewMaterialWizard* wizard = new NewMaterialWizard(mFrame);

      // Set initial import path, the user can change it.
      wxString defaultSavePath = mSelectedModule->getModulePath();
      defaultSavePath.Append("/materials");
      wizard->savePath->SetPath(defaultSavePath);

      if (wizard->RunWizard(wizard->m_pages[0]))
      {
         wxString assetID = wizard->assetID->GetValue();
         wxString savePath = wizard->savePath->GetPath();

         wxString assetPath("");
         assetPath.Append(savePath);
         assetPath.Append("/");
         assetPath.Append(assetID);
         assetPath.Append(".asset.taml");

         wxString templateFileName("");
         templateFileName.Append(assetID);
         templateFileName.Append(".taml");

         wxString templatePath("");
         templatePath.Append(savePath);
         templatePath.Append("/");
         templatePath.Append(templateFileName);

         // Create material template and then asset.
         Torque::Scene.createMaterialTemplate(templatePath.c_str());
         Torque::Scene.createMaterialAsset(assetID.c_str(), templateFileName.c_str(), assetPath.c_str());
         Torque::AssetDatabaseLink.addDeclaredAsset(mSelectedModule, assetPath.c_str());
         refresh();
      }
   }
}

void ProjectTool::OnPropertyChanged( wxPropertyGridEvent& evt )
{ 
   if (!mSelectedAssetDef || !mSelectedAsset)
      return;

   wxString name = evt.GetPropertyName();
   wxVariant val = evt.GetPropertyValue();
   wxString strVal = val.GetString();

   if (name.StartsWith("TextureAsset"))
   {
      long intVal = val.GetInteger();
      //strVal = mTextureAssetChoices.GetLabel(intVal);
   }

   // Set Value.
   mSelectedAsset->setDataField(Torque::StringTableLink->insert(name), NULL, strVal);

   // Save the asset
   mSelectedAsset->saveAsset();

   // Refresh textures if it's a MaterialAsset.
   if (mSelectedMaterialAsset)
      mSelectedMaterialAsset->loadTextures();

   // Refresh Scene.
   Torque::Scene.refresh();

   // Reload object properties.
   mAssetsInspector->Inspect(mSelectedAssetDef);
}

const char* ProjectTool::getAssetCategoryName(const char* _name)
{
   if (dStrcmp(_name, "ObjectTemplateAsset") == 0)
      return "Object Templates";

   if (dStrcmp(_name, "MaterialAsset") == 0)
      return "Materials";

   if (dStrcmp(_name, "MeshAsset") == 0)
      return "Meshes";

   if (dStrcmp(_name, "ImageAsset") == 0)
      return "Images";

   if (dStrcmp(_name, "ShaderAsset") == 0)
      return "Shaders";

   if (dStrcmp(_name, "TextureAsset") == 0)
      return "Textures";

   return _name;
}

void ProjectTool::refresh()
{
   refreshAssetList();
}

void ProjectTool::refreshAssetList()
{
   // Clear list.
   mAssetsTab->assetList->DeleteAllItems();
   mAssetListRoot = mAssetsTab->assetList->AddRoot("ROOT");

   Vector<ModuleInfo>* modules = mProjectManager->getModuleList();

   const char* currentModuleID = "";
   wxTreeItemId currentModuleTreeID = mAssetListRoot;
   for (Vector<ModuleInfo>::iterator modulesItr = modules->begin(); modulesItr != modules->end(); ++modulesItr)
   {
      if (dStrcmp(modulesItr->moduleID, currentModuleID) != 0)
      {
         currentModuleID = modulesItr->moduleID;
         currentModuleTreeID = mAssetsTab->assetList->AppendItem(mAssetListRoot, modulesItr->moduleID, 0, -1, new ModuleTreeItemData(*modulesItr));
      }
      
      for (Vector<AssetCategoryInfo>::iterator assetCatItr = modulesItr->assets.begin(); assetCatItr != modulesItr->assets.end(); ++assetCatItr)
      {
         wxTreeItemId categoryTreeID = mAssetsTab->assetList->AppendItem(currentModuleTreeID, assetCatItr->categoryName, 1, -1, new AssetCategoryTreeItemData(*assetCatItr));

         for (Vector<const AssetDefinition*>::iterator assetItr = assetCatItr->assets.begin(); assetItr != assetCatItr->assets.end(); ++assetItr)
         {
            // Fetch asset definition.
            const AssetDefinition* pAssetDefinition = *assetItr;
            char buf[256];
            dStrcpy(buf, pAssetDefinition->mAssetId);
            const char* moduleName = dStrtok(buf, ":");
            const char* assetName = dStrtok(NULL, ":");
            mAssetsTab->assetList->AppendItem(categoryTreeID, assetName, 2, -1, new AssetTreeItemData(pAssetDefinition));
         }
      }
   }

   // Sort Modules by Name
   mAssetsTab->assetList->SortChildren(mAssetListRoot);
}