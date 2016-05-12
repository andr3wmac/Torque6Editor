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

wxTorqueAssetSelectDialog::wxTorqueAssetSelectDialog(ProjectManager* projectManager, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
   mProjectManager   = projectManager;
   mSelectedAsset    = NULL;

   this->SetSizeHints(wxDefaultSize, wxDefaultSize);
   this->SetBackgroundColour(Theme::lightBackgroundColor);

   wxBoxSizer* bSizer51;
   bSizer51 = new wxBoxSizer(wxVERTICAL);

   assetList = new wxTorqueAssetTree(mProjectManager, this, ASSET_LIST, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
   assetList->SetForegroundColour(wxColour(255, 255, 255));
   assetList->SetBackgroundColour(Theme::darkBackgroundColor);
   assetList->Connect(wxID_ANY, wxEVT_TREE_SEL_CHANGED, wxTreeEventHandler(wxTorqueAssetSelectDialog::OnAssetTreeEvent), NULL, this);

   bSizer51->Add(assetList, 1, wxALL | wxEXPAND, 1);

   wxBoxSizer* bSizer611;
   bSizer611 = new wxBoxSizer(wxHORIZONTAL);


   bSizer611->Add(0, 0, 1, wxEXPAND, 5);

   m_button6 = new wxButton(this, wxID_ANY, wxT("Select"), wxDefaultPosition, wxDefaultSize, 0 | wxNO_BORDER);
   m_button6->SetForegroundColour(wxColour(255, 255, 255));
   m_button6->SetBackgroundColour(Theme::darkBackgroundColor);
   m_button6->Bind(wxEVT_BUTTON, &wxTorqueAssetSelectDialog::OnSelectButton, this, -1, -1, NULL);

   bSizer611->Add(m_button6, 0, wxALL, 5);

   m_button61 = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 | wxNO_BORDER);
   m_button61->SetForegroundColour(wxColour(255, 255, 255));
   m_button61->SetBackgroundColour(Theme::darkBackgroundColor);
   m_button61->Bind(wxEVT_BUTTON, &wxTorqueAssetSelectDialog::OnCancelButton, this, -1, -1, NULL);

   bSizer611->Add(m_button61, 0, wxALL, 5);


   bSizer611->Add(0, 0, 1, wxEXPAND, 5);


   bSizer51->Add(bSizer611, 0, wxEXPAND, 5);


   this->SetSizer(bSizer51);
   this->Layout();

   this->Centre(wxCENTER_ON_SCREEN);
}

wxTorqueAssetSelectDialog::~wxTorqueAssetSelectDialog()
{

}

void wxTorqueAssetSelectDialog::OnAssetTreeEvent(wxTreeEvent& evt)
{
   AssetTreeItemData* data = dynamic_cast<AssetTreeItemData*>(assetList->GetItemData(evt.GetItem()));
   if (data)
   {
      mSelectedAsset = data->objPtr;
      return;
   }
}

void wxTorqueAssetSelectDialog::OnSelectButton(wxCommandEvent& evt)
{
   EndModal(1);
}

void wxTorqueAssetSelectDialog::OnCancelButton(wxCommandEvent& evt)
{
   mSelectedAsset = NULL;
   EndModal(0);
}

bool wxTorqueAssetSelectDialog::SelectAsset(wxString& returnValue, const char* filter)
{
   assetList->ShowAssets(filter);
   ShowModal();

   if (mSelectedAsset != NULL)
   {
      returnValue = wxString(mSelectedAsset->mAssetId);
      return true;
   }

   return false;
}