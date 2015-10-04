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

#include <wx/dir.h>
#include <wx/treectrl.h>

// UI generated from wxFormBuilder
#include "../Torque6EditorUI.h"

#include "materialsTool.h"

MaterialsTool::MaterialsTool(ProjectManager* _projectManager, MainFrame* _frame, wxAuiManager* _manager)
   : Parent(_projectManager, _frame, _manager),
     mMaterialsPanel(NULL),
     mSelectedNode(NULL),
     mSelectedNodeParent(NULL)
{
   mIconList = new wxImageList( 16, 16 );
      
}

MaterialsTool::~MaterialsTool()
{

}

void MaterialsTool::initTool()
{
   // Create panel.
   mMaterialsPanel = new MaterialsPanel(mFrame);

   // Icons
   mIconList->Add(wxBitmap("images/materialIcon.png", wxBITMAP_TYPE_PNG));
   mMaterialsPanel->m_materialTree->AssignImageList(mIconList);

   // Events
   mMaterialsPanel->Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MaterialsTool::OnMenuEvent), NULL, this);
   mMaterialsPanel->Connect(wxID_ANY, wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(MaterialsTool::OnTreeEvent), NULL, this);
   mMaterialsPanel->propertyGrid->Connect(wxID_ANY, wxEVT_PG_CHANGED, wxPropertyGridEventHandler(MaterialsTool::OnPropertyChanged), NULL, this);

   // Root for material tree
   mMaterialTreeRoot = mMaterialsPanel->m_materialTree->AddRoot("ROOT");

   mManager->AddPane(mMaterialsPanel, wxAuiPaneInfo().Caption("Materials")
                                                .CaptionVisible( true )
                                                .CloseButton( true )
                                                .PinButton( true )
                                                .MaximizeButton(true)
                                                .Dock()
                                                .Resizable()
                                                .FloatingSize( wxDefaultSize )
                                                .Bottom()
                                                .Hide());

   //MaterialWindow* materialWindow = new MaterialWindow(mFrame);
   //mMaterialsPanel->m_materialNotebook->AddPage(materialWindow, "Test Window");

   mManager->Update();
}

void MaterialsTool::openTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mMaterialsPanel);
   paneInfo.Show();
   mManager->Update();
   refreshMaterialList();
}

void MaterialsTool::closeTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mMaterialsPanel);
   paneInfo.Hide();
   mManager->Update();
}

void MaterialsTool::onProjectLoaded(const wxString& projectName, const wxString& projectPath)
{
   refreshMaterialList();
}

void MaterialsTool::onProjectClosed()
{
   //
}

void MaterialsTool::OnMenuEvent(wxCommandEvent& evt)
{
   if (evt.GetId() == MATERIAL_SAVE)
   {
      MaterialWindow* window = dynamic_cast<MaterialWindow*>(mMaterialsPanel->m_materialNotebook->GetCurrentPage());
      if (window)
         window->saveMaterial();
   }
}

void MaterialsTool::OnTreeEvent(wxTreeEvent& evt)
{
   MaterialsTreeItemData* data = dynamic_cast<MaterialsTreeItemData*>(mMaterialsPanel->m_materialTree->GetItemData(evt.GetItem()));
   if ( data )
   {
      MaterialAsset* mat = Plugins::Link.Scene.getMaterialAsset(data->assetId);
      if ( mat )
      {
         // See if we've already opened a tab for this one, if so then set focus to it.
         for (unsigned int i = 0; i < mMaterialsPanel->m_materialNotebook->GetPageCount(); ++i)
         {
            MaterialWindow* window = dynamic_cast<MaterialWindow*>(mMaterialsPanel->m_materialNotebook->GetPage(i));
            MaterialAsset* asset = window->getMaterialAsset();
            if (asset == mat)
            {
               mMaterialsPanel->m_materialNotebook->SetSelection(i);
               return;
            }
         }

         // If not then create a new MaterialWindow and add it to the collection.
         MaterialWindow* materialWindow = new MaterialWindow(mFrame, this);
         materialWindow->loadMaterial(mat);
         mMaterialsPanel->m_materialNotebook->AddPage(materialWindow, wxString(data->assetId));
         mMaterialsPanel->m_materialNotebook->SetSelection(mMaterialsPanel->m_materialNotebook->GetPageCount() - 1);
      }
   }
}

void MaterialsTool::refreshMaterialList()
{
   if (!mProjectManager->isProjectLoaded())
      return;

   // Clear list.
   mMaterialsPanel->m_materialTree->DeleteAllItems();
   mMaterialTreeRoot = mMaterialsPanel->m_materialTree->AddRoot("ROOT");

   // Find all material assets.
   AssetQuery assQuery;
   Plugins::Link.AssetDatabaseLink.findAssetType(&assQuery, "MaterialAsset", false);

   bool inCategory = false;
   char last_mod_name[256];
   wxTreeItemId itemParent = mMaterialTreeRoot;
   for ( U32 n = 0; n < assQuery.size(); n++)
   {
      StringTableEntry assetID = assQuery[n];
      char buf[256];
      dStrcpy(buf, assetID);

      char* mod_name = dStrtok(buf, ":");
      if ( dStrcmp(last_mod_name, mod_name) != 0 )
      {
         itemParent = mMaterialsPanel->m_materialTree->AppendItem(mMaterialTreeRoot, mod_name, 0, -1);
         dStrcpy(last_mod_name, mod_name);
         inCategory = true;
      }

      char* asset_name = dStrtok(NULL, ":");
      mMaterialsPanel->m_materialTree->AppendItem(itemParent, asset_name, 0, -1, new MaterialsTreeItemData(assetID));
   }
}

void MaterialsTool::selectNode(MaterialWindow* parent, Node* node)
{
   mSelectedNode = node;
   mSelectedNodeParent = parent;
   wxPropertyGrid* grid = mMaterialsPanel->propertyGrid;

   grid->Clear();

   // Standard for all nodes.
   grid->Append(new wxPropertyCategory("Material Node"));
   grid->Append(new wxStringProperty("Name", "Name", node->name));
   grid->Append(new wxPropertyCategory(node->type));

   // Texture
   if (node->type == "Texture")
      grid->Append(new wxIntProperty("Slot", "Slot", node->textureSlot));

   // Float
   if (node->type == "Float")
      grid->Append(new wxFloatProperty("Value", "Value", node->color.red));

   // Time
   if (node->type == "Time")
      grid->Append(new wxFloatProperty("Multiplier", "Multiplier", node->color.red));

   // Vec2
   if (node->type == "Vec2")
   {
      grid->Append(new wxFloatProperty("X", "X", node->color.red));
      grid->Append(new wxFloatProperty("Y", "Y", node->color.green));
   }

   // Vec3, Vec4
   if (node->type == "Vec3" || node->type == "Vec4")
   {
      wxColour color;
      color.Set(node->color.red * 255, node->color.green * 255, node->color.blue * 255, 255);
      grid->Append(new wxColourProperty("Color", "Color", color));
   }
   if (node->type == "Vec4")
   {
      grid->Append(new wxFloatProperty("Alpha", "Alpha", node->color.alpha));
   }
}

void MaterialsTool::OnPropertyChanged(wxPropertyGridEvent& evt)
{
   if (mSelectedNode == NULL)
      return;

   wxString name = evt.GetPropertyName();
   wxVariant val = evt.GetPropertyValue();

   // Name
   if (name == "Name")
   {
      mSelectedNode->name = val.GetString();
      if (mSelectedNode->materialNode != NULL)
         mSelectedNode->materialNode->setInternalName(mSelectedNode->name);
   }

   // Slot is for Textures
   if (name == "Slot")
      mSelectedNode->textureSlot = val.GetInteger();

   // Value is for Float
   if (name == "Value")
      mSelectedNode->color.red = val.GetDouble();

   // Multiplier is for Time
   if (name == "Multiplier")
      mSelectedNode->color.red = val.GetDouble();

   // X, Y are for Vec2
   if (name == "X")
      mSelectedNode->color.red = val.GetDouble();
   if (name == "Y")
      mSelectedNode->color.green = val.GetDouble();

   // Color covers Vec3 and Vec4.
   if (name == "Color")
   {
      wxColour color;
      color << val;
      mSelectedNode->color.red   = color.Red() / 255.0f;
      mSelectedNode->color.green = color.Green() / 255.0f;
      mSelectedNode->color.blue  = color.Blue() / 255.0f;
   }

   // Alpha for Vec4
   if (name == "Alpha")
      mSelectedNode->color.alpha = val.GetDouble();

   if (mSelectedNodeParent != NULL)
      mSelectedNodeParent->Refresh(false);
}