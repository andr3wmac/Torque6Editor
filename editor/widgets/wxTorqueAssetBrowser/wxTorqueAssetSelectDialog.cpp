// -------------------------------------------------------------------------- -
// Copyright (c) 2016 Andrew Mac
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

#include "wxTorqueAssetSelectDialog.h"

#include <plugins/plugins_shared.h>
#include "scene/components/animationComponent.h"
#include "scene/components/controllerComponent.h"
#include "scene/components/lighting/lightComponent.h"
#include "scene/components/meshComponent.h"
#include "scene/components/physics/physicsBoxComponent.h"
#include "scene/components/physics/physicsSphereComponent.h"
#include "scene/components/textComponent.h"
#include "../../theme.h"

wxTorqueAssetSelectDialog::wxTorqueAssetSelectDialog(EditorManager* EditorManager, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
   mEditorManager    = EditorManager;
   SelectedAsset     = NULL;

   this->SetSizeHints(wxDefaultSize, wxDefaultSize);
   this->SetBackgroundColour(Theme::lightBackgroundColor);

   ContentSizer = new wxBoxSizer(wxVERTICAL);

   // Top Bar
   TopBarSizer = new wxBoxSizer(wxHORIZONTAL);
   ContentSizer->Add(TopBarSizer, 0, wxEXPAND, 5);

   // Asset Tree
   mAssetTree = new wxTorqueAssetTree(mEditorManager, this, ASSET_LIST, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
   mAssetTree->SetForegroundColour(wxColour(255, 255, 255));
   mAssetTree->SetBackgroundColour(Theme::darkBackgroundColor);
   mAssetTree->Connect(wxID_ANY, wxEVT_TREE_SEL_CHANGED, wxTreeEventHandler(wxTorqueAssetSelectDialog::OnAssetTreeEvent), NULL, this);
   ContentSizer->Add(mAssetTree, 1, wxALL | wxEXPAND, 1);

   // Bottom Bar
   BottomBarSizer = new wxBoxSizer(wxHORIZONTAL);
   BottomBarSizer->Add(0, 0, 1, wxEXPAND, 5);

   // Select Button
   mSelectButton = new wxButton(this, wxID_ANY, wxT("Select"), wxDefaultPosition, wxDefaultSize, 0 | wxNO_BORDER);
   mSelectButton->SetForegroundColour(wxColour(255, 255, 255));
   mSelectButton->SetBackgroundColour(Theme::darkBackgroundColor);
   mSelectButton->Bind(wxEVT_BUTTON, &wxTorqueAssetSelectDialog::OnSelectButton, this, -1, -1, NULL);
   BottomBarSizer->Add(mSelectButton, 0, wxALL, 5);

   // Cancel Button
   mCancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 | wxNO_BORDER);
   mCancelButton->SetForegroundColour(wxColour(255, 255, 255));
   mCancelButton->SetBackgroundColour(Theme::darkBackgroundColor);
   mCancelButton->Bind(wxEVT_BUTTON, &wxTorqueAssetSelectDialog::OnCancelButton, this, -1, -1, NULL);
   BottomBarSizer->Add(mCancelButton, 0, wxALL, 5);
   BottomBarSizer->Add(0, 0, 1, wxEXPAND, 5);

   ContentSizer->Add(BottomBarSizer, 0, wxEXPAND, 5);

   this->SetSizer(ContentSizer);
   this->Layout();
   this->Centre(wxCENTER_ON_SCREEN);
}

wxTorqueAssetSelectDialog::~wxTorqueAssetSelectDialog()
{

}

void wxTorqueAssetSelectDialog::OnAssetTreeEvent(wxTreeEvent& evt)
{
   AssetTreeItemData* data = dynamic_cast<AssetTreeItemData*>(mAssetTree->GetItemData(evt.GetItem()));
   if (data)
   {
      SelectedAsset = data->objPtr;
      return;
   }
}

void wxTorqueAssetSelectDialog::OnSelectButton(wxCommandEvent& evt)
{
   EndModal(1);
}

void wxTorqueAssetSelectDialog::OnCancelButton(wxCommandEvent& evt)
{
   SelectedAsset = NULL;
   EndModal(0);
}

bool wxTorqueAssetSelectDialog::SelectAsset(wxString& returnValue, const char* filter, const char* defaultAsset)
{
   mAssetTree->ShowAssets(filter, defaultAsset);
   ShowModal();

   if (SelectedAsset != NULL)
   {
      returnValue = wxString(SelectedAsset->mAssetId);
      return true;
   }

   return false;
}

void wxTorqueAssetSelectDialog::SetSelectedAsset(wxString assetId)
{
   SelectedAsset = Torque::AssetDatabaseLink.getAssetDefinition(assetId.c_str());
}

void wxTorqueAssetSelectDialog::RefreshAssetList(const char* filter, const char* defaultAsset)
{
   mAssetTree->ShowAssets(filter, defaultAsset);
}