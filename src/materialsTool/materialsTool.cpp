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

MaterialsTool::MaterialsTool()
   : mMaterialsPanel(NULL)
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
   mMaterialsPanel->Connect(wxID_ANY, wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(MaterialsTool::OnTreeEvent), NULL, this);

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

   MaterialWindow* materialWindow = new MaterialWindow(mFrame);
   mMaterialsPanel->m_materialNotebook->AddPage(materialWindow, "Test Window");

   mManager->Update();
}

void MaterialsTool::openTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mMaterialsPanel);
   paneInfo.Show();
   mManager->Update();
}

void MaterialsTool::closeTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mMaterialsPanel);
   paneInfo.Hide();
   mManager->Update();
}

void MaterialsTool::onProjectLoaded(wxString projectName, wxString projectPath)
{
   refreshMaterialList();
}

void MaterialsTool::onProjectClosed()
{
   //
}

void MaterialsTool::OnTreeEvent( wxTreeEvent& evt )
{
   MaterialsTreeItemData* data = dynamic_cast<MaterialsTreeItemData*>(mMaterialsPanel->m_materialTree->GetItemData(evt.GetItem()));
   if ( data )
   {
      MaterialAsset* mat = Plugins::Link.Scene.getMaterialAsset(data->assetId);
      if ( mat )
      {
         MaterialWindow* materialWindow = new MaterialWindow(mFrame);
         materialWindow->loadMaterial(mat);
         mMaterialsPanel->m_materialNotebook->AddPage(materialWindow, wxString(data->assetId));
      }
   }
}

void MaterialsTool::refreshMaterialList()
{
   bool inCategory = false;
   char last_mod_name[256];
   AssetQuery assQuery;
   Plugins::Link.AssetDatabaseLink.findAssetType(&assQuery, "MaterialAsset", false);
   for ( U32 n = 0; n < assQuery.size(); n++)
   {
      StringTableEntry assetID = assQuery[n];
      char buf[256];
      dStrcpy(buf, assetID);

      char* mod_name = dStrtok(buf, ":");
      if ( dStrcmp(last_mod_name, mod_name) != 0 )
      {
         dStrcpy(last_mod_name, mod_name);
         inCategory = true;
      }

      char* asset_name = dStrtok(NULL, ":");
      mMaterialsPanel->m_materialTree->AppendItem(mMaterialTreeRoot, asset_name, 0, -1, new MaterialsTreeItemData(assetID));
   }
}