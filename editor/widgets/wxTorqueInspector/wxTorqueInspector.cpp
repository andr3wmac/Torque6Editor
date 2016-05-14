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

#include "wxTorqueInspector.h"

#include <plugins/plugins_shared.h>
#include "scene/components/animationComponent.h"
#include "scene/components/controllerComponent.h"
#include "scene/components/lighting/lightComponent.h"
#include "scene/components/meshComponent.h"
#include "scene/components/physics/physicsBoxComponent.h"
#include "scene/components/physics/physicsSphereComponent.h"
#include "scene/components/textComponent.h"

#include "../../editorManager.h"
#include "../../theme.h"

IMPLEMENT_DYNAMIC_CLASS(wxTorqueInspector, wxPanel)

BEGIN_EVENT_TABLE(wxTorqueInspector, wxPanel)

END_EVENT_TABLE()

void wxTorqueInspector::Init()
{
   mObject        = NULL;
   mSceneObject   = NULL;
   mComponent     = NULL;

   // Icons
   mIconList = new wxImageList(16, 16);
   mIconList->Add(wxBitmap("images/downArrow.png", wxBITMAP_TYPE_PNG));
   mIconList->Add(wxBitmap("images/rightArrow.png", wxBITMAP_TYPE_PNG));

   mContentsSizer = new wxBoxSizer(wxVERTICAL);
   this->SetSizer(mContentsSizer);
   this->Layout();
}

void wxTorqueInspector::Clear()
{
   if (mContentsSizer->GetChildren().size() > 0)
      mContentsSizer->Clear(true);
}

// -----------------------
//  Groups
// -----------------------

wxPanel* wxTorqueInspector::AddGroup(wxPanel* panel, const wxString& label)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxTorqueGroup* group = new wxTorqueGroup();

   group->headerPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
   group->headerSizer = new wxBoxSizer(wxHORIZONTAL);
   group->headerPanel->SetSizer(group->headerSizer);
   panelSizer->Add(group->headerPanel, 0, wxEXPAND, 0);

   group->headerButton = new wxButton(group->headerPanel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | wxBU_LEFT | wxBORDER_NONE);
   group->headerButton->SetBackgroundColour(Theme::darkBackgroundColor);
   group->headerButton->SetForegroundColour(wxColour(255, 255, 255));
   group->headerButton->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
   group->headerButton->SetBitmap(mIconList->GetBitmap(0));
   group->headerSizer->Add(group->headerButton, 1, wxEXPAND, 0);

   group->panel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
   group->sizer = new wxBoxSizer(wxVERTICAL);
   group->panel->SetSizer(group->sizer);
   panelSizer->Add(group->panel, 0, wxEXPAND, 10);
   mGroupPanel = group->panel;

   group->headerButton->Bind(wxEVT_BUTTON, &wxTorqueInspector::OnGroupClick, this, -1, -1, group);

   return mGroupPanel;
}

void wxTorqueInspector::OnGroupClick(wxCommandEvent& evt)
{
   wxTorqueGroup* group = dynamic_cast<wxTorqueGroup*>(evt.GetEventUserData());
   if (group)
   {
      if (group->panel->IsShown())
      {
         group->headerButton->SetBitmap(mIconList->GetBitmap(1));
         group->panel->Hide();
      }
      else
      {
         group->headerButton->SetBitmap(mIconList->GetBitmap(0));
         group->panel->Show();
      }

      wxSize size = this->GetParent()->GetBestVirtualSize();
      this->GetParent()->SetVirtualSize(size);
      this->SetVirtualSize(size);
   }
}

// -----------------------
//  String Field
// -----------------------

void wxTorqueInspector::AddStringField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxTorqueStringField* field = new wxTorqueStringField();
   field->fieldName = fieldName;
   field->sizer = new wxBoxSizer(wxHORIZONTAL);

   field->label = new wxStaticText(panel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
   field->label->Wrap(-1);
   field->label->SetForegroundColour(wxColour(255, 255, 255));
   field->sizer->Add(field->label, 1, wxALL, 5);

   field->value = new wxTextCtrl(panel, wxID_ANY, value, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER);
   field->value->SetBackgroundColour(Theme::darkBackgroundColor);
   field->value->SetForegroundColour(wxColour(255, 255, 255));
   field->value->SetMinSize(wxSize(10, 20));
   field->value->Bind(wxEVT_TEXT_ENTER, &wxTorqueInspector::OnStringFieldChanged, this, -1, -1, field);
   field->sizer->Add(field->value, 1, wxALL, 2);

   panelSizer->Add(field->sizer, 0, wxEXPAND, 5);
}

void wxTorqueInspector::OnStringFieldChanged(wxCommandEvent& evt)
{
   wxTorqueStringField* stringField = dynamic_cast<wxTorqueStringField*>(evt.GetEventUserData());
   if (stringField)
   {
      mObject->setDataField(Torque::StringTableLink->insert(stringField->fieldName), NULL, stringField->value->GetValue());

      if (mSceneObject != NULL)
         mSceneObject->refresh();

      if (mComponent != NULL)
         mComponent->mOwnerObject->refresh();
   }
}

// -----------------------
//  Bool Field
// -----------------------

void wxTorqueInspector::AddBoolField(wxPanel* panel, const char* fieldName, const wxString& label, bool value)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

   wxStaticText* labelCtrl = new wxStaticText(panel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
   labelCtrl->Wrap(-1);
   labelCtrl->SetForegroundColour(wxColour(255, 255, 255));
   sizer->Add(labelCtrl, 1, wxALL, 5);

   wxTorqueBoolField* field = new wxTorqueBoolField();
   field->fieldName = fieldName;
   field->value = new wxCheckBox(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
   field->value->SetBackgroundColour(Theme::lightBackgroundColor);
   field->value->SetForegroundColour(wxColour(255, 255, 255));
   field->value->SetValue(value);
   field->value->Bind(wxEVT_CHECKBOX, &wxTorqueInspector::OnBoolFieldChanged, this, -1, -1, field);
   sizer->Add(field->value, 1, wxALL, 5);

   panelSizer->Add(sizer, 0, wxEXPAND, 5);
}

void wxTorqueInspector::OnBoolFieldChanged(wxCommandEvent& evt)
{
   wxTorqueBoolField* field = dynamic_cast<wxTorqueBoolField*>(evt.GetEventUserData());
   if (field)
   {
      bool val = field->value->GetValue();
      mObject->setDataField(Torque::StringTableLink->insert(field->fieldName), NULL, val ? "1" : "0");

      if (mSceneObject != NULL)
         mSceneObject->refresh();

      if (mComponent != NULL)
         mComponent->mOwnerObject->refresh();
   }
}

// -----------------------
//  File Field
// -----------------------

void wxTorqueInspector::AddFileField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxTorqueFileField* field = new wxTorqueFileField();
   field->fieldName = fieldName;
   wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

   wxStaticText* labelCtrl = new wxStaticText(panel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
   labelCtrl->Wrap(-1);
   labelCtrl->SetForegroundColour(wxColour(255, 255, 255));
   sizer->Add(labelCtrl, 1, wxALL, 5);

   field->value = new wxFilePickerCtrl(panel, wxID_ANY, value, wxT("Choose File"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER);
   field->value->SetBackgroundColour(Theme::darkBackgroundColor);
   field->value->SetForegroundColour(wxColour(255, 255, 255));
   field->value->SetMinSize(wxSize(10, 20));
   field->value->Bind(wxEVT_TEXT_ENTER, &wxTorqueInspector::OnFileFieldChanged, this, -1, -1, field);
   sizer->Add(field->value, 1, wxALL, 2);

   panelSizer->Add(sizer, 0, wxEXPAND, 5);
}

void wxTorqueInspector::OnFileFieldChanged(wxCommandEvent& evt)
{
   wxTorqueFileField* field = dynamic_cast<wxTorqueFileField*>(evt.GetEventUserData());
   if (field)
   {
      mObject->setDataField(Torque::StringTableLink->insert(field->fieldName), NULL, field->value->GetPath());

      if (mSceneObject != NULL)
         mSceneObject->refresh();

      if (mComponent != NULL)
         mComponent->mOwnerObject->refresh();
   }
}

// -----------------------
//  Point3F Field
// -----------------------

void wxTorqueInspector::AddPoint3FField(wxPanel* panel, const char* fieldName, const wxString& label, const Point3F& value)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

   wxStaticText* labelCtrl = new wxStaticText(panel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
   labelCtrl->Wrap(-1);
   labelCtrl->SetForegroundColour(wxColour(255, 255, 255));
   sizer->Add(labelCtrl, 3, wxALL, 5);

   // X Value
   wxTextCtrl* xValueCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format(wxT("%g"), value.x), wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER);
   xValueCtrl->SetBackgroundColour(Theme::darkBackgroundColor);
   xValueCtrl->SetForegroundColour(wxColour(255, 255, 255));
   xValueCtrl->SetMinSize(wxSize(10, 20));
   sizer->Add(xValueCtrl, 1, wxALL, 2);

   // Y Value
   wxTextCtrl* yValueCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format(wxT("%g"), value.y), wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER);
   yValueCtrl->SetBackgroundColour(Theme::darkBackgroundColor);
   yValueCtrl->SetForegroundColour(wxColour(255, 255, 255));
   yValueCtrl->SetMinSize(wxSize(10, 20));
   sizer->Add(yValueCtrl, 1, wxALL, 2);

   // Z Value
   wxTextCtrl* zValueCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format(wxT("%g"), value.z), wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER);
   zValueCtrl->SetBackgroundColour(Theme::darkBackgroundColor);
   zValueCtrl->SetForegroundColour(wxColour(255, 255, 255));
   zValueCtrl->SetMinSize(wxSize(10, 20));
   sizer->Add(zValueCtrl, 1, wxALL, 2);

   // Events
   wxTorquePoint3FField* xField = new wxTorquePoint3FField();
   xField->fieldName = fieldName;
   xField->xValue = xValueCtrl;
   xField->yValue = yValueCtrl;
   xField->zValue = zValueCtrl;
   xValueCtrl->Bind(wxEVT_TEXT_ENTER, &wxTorqueInspector::OnPoint3FFieldChanged, this, -1, -1, xField);

   wxTorquePoint3FField* yField = new wxTorquePoint3FField();
   yField->fieldName = fieldName;
   yField->xValue = xValueCtrl;
   yField->yValue = yValueCtrl;
   yField->zValue = zValueCtrl;
   yValueCtrl->Bind(wxEVT_TEXT_ENTER, &wxTorqueInspector::OnPoint3FFieldChanged, this, -1, -1, yField);

   wxTorquePoint3FField* zField = new wxTorquePoint3FField();
   zField->fieldName = fieldName;
   zField->xValue = xValueCtrl;
   zField->yValue = yValueCtrl;
   zField->zValue = zValueCtrl;
   zValueCtrl->Bind(wxEVT_TEXT_ENTER, &wxTorqueInspector::OnPoint3FFieldChanged, this, -1, -1, zField);

   panelSizer->Add(sizer, 0, wxEXPAND, 5);
}

void wxTorqueInspector::OnPoint3FFieldChanged(wxCommandEvent& evt)
{
   wxTorquePoint3FField* field = dynamic_cast<wxTorquePoint3FField*>(evt.GetEventUserData());
   if (field)
   {
      wxString value = wxString::Format(wxT("%s %s %s"), field->xValue->GetValue(), field->yValue->GetValue(), field->zValue->GetValue());
      mObject->setDataField(Torque::StringTableLink->insert(field->fieldName), NULL, value);

      if (mSceneObject != NULL)
         mSceneObject->refresh();

      if (mComponent != NULL)
         mComponent->mOwnerObject->refresh();
   }
}

// -----------------------
//  Button Field
// -----------------------

void wxTorqueInspector::AddButtonField(wxPanel* panel, const char* fieldName, const wxString& label)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxBoxSizer* rowSizer;
   rowSizer = new wxBoxSizer(wxHORIZONTAL);

   wxButton* valueControl = new wxButton(panel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
   valueControl->SetBackgroundColour(Theme::darkBackgroundColor);
   valueControl->SetForegroundColour(wxColour(255, 255, 255));
   rowSizer->Add(valueControl, 1, wxALL, 5);

   panelSizer->Add(rowSizer, 0, wxEXPAND, 5);
}

// -----------------------
//  MeshAsset Field
// -----------------------

void wxTorqueInspector::AddMeshAssetField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxTorqueMeshAssetField* field = new wxTorqueMeshAssetField();
   field->fieldName = fieldName;
   field->sizer = new wxBoxSizer(wxHORIZONTAL);

   field->label = new wxStaticText(panel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
   field->label->Wrap(-1);
   field->label->SetForegroundColour(wxColour(255, 255, 255));
   field->sizer->Add(field->label, 1, wxALL, 5);

   wxBoxSizer* valueSizer = new wxBoxSizer(wxHORIZONTAL);

   field->value = new wxTextCtrl(panel, wxID_ANY, value, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER);
   field->value->SetBackgroundColour(Theme::darkBackgroundColor);
   field->value->SetForegroundColour(wxColour(255, 255, 255));
   field->value->SetMinSize(wxSize(1, 20));
   valueSizer->Add(field->value, 1, wxALL, 2);

   wxButton* button = new wxButton(panel, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   button->SetBackgroundColour(Theme::darkBackgroundColor);
   button->SetForegroundColour(wxColour(255, 255, 255));
   button->SetMinSize(wxSize(20, 20));
   button->SetMaxSize(wxSize(20, 20));
   button->Bind(wxEVT_BUTTON, &wxTorqueInspector::OnMeshAssetFieldSelect, this, -1, -1, field);
   valueSizer->Add(button, 0, wxALL, 2);

   field->sizer->Add(valueSizer, 1, wxEXPAND, 0);

   panelSizer->Add(field->sizer, 1, wxEXPAND, 5);
}

void wxTorqueInspector::OnMeshAssetFieldSelect(wxCommandEvent& evt)
{
   wxTorqueMeshAssetField* field = dynamic_cast<wxTorqueMeshAssetField*>(evt.GetEventUserData());
   if (field)
   {
      wxString returnValue;
      if (mEditorManager->selectAsset(returnValue, "MeshAsset"))
         field->value->SetValue(returnValue);

      mObject->setDataField(Torque::StringTableLink->insert(field->fieldName), NULL, field->value->GetValue());

      if (mSceneObject != NULL)
         mSceneObject->refresh();

      if (mComponent != NULL)
         mComponent->mOwnerObject->refresh();
   }
}

// -----------------------
//  ObjectTemplateAsset Field
// -----------------------

void wxTorqueInspector::AddObjectTemplateAssetField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxTorqueObjectTemplateAssetField* field = new wxTorqueObjectTemplateAssetField();
   field->fieldName = fieldName;
   field->sizer = new wxBoxSizer(wxHORIZONTAL);

   field->label = new wxStaticText(panel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
   field->label->Wrap(-1);
   field->label->SetForegroundColour(wxColour(255, 255, 255));
   field->sizer->Add(field->label, 1, wxALL, 5);

   wxBoxSizer* valueSizer = new wxBoxSizer(wxHORIZONTAL);

   field->value = new wxTextCtrl(panel, wxID_ANY, value, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER);
   field->value->SetBackgroundColour(Theme::darkBackgroundColor);
   field->value->SetForegroundColour(wxColour(255, 255, 255));
   field->value->SetMinSize(wxSize(1, 20));
   valueSizer->Add(field->value, 1, wxALL, 2);

   wxButton* button = new wxButton(panel, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   button->SetBackgroundColour(Theme::darkBackgroundColor);
   button->SetForegroundColour(wxColour(255, 255, 255));
   button->SetMinSize(wxSize(20, 20));
   button->SetMaxSize(wxSize(20, 20));
   button->Bind(wxEVT_BUTTON, &wxTorqueInspector::OnObjectTemplateAssetFieldSelect, this, -1, -1, field);
   valueSizer->Add(button, 0, wxALL, 2);

   field->sizer->Add(valueSizer, 1, wxEXPAND, 0);

   panelSizer->Add(field->sizer, 1, wxEXPAND, 5);
}

void wxTorqueInspector::OnObjectTemplateAssetFieldSelect(wxCommandEvent& evt)
{
   wxTorqueObjectTemplateAssetField* field = dynamic_cast<wxTorqueObjectTemplateAssetField*>(evt.GetEventUserData());
   if (field)
   {
      wxString returnValue;
      if (mEditorManager->selectAsset(returnValue, "ObjectTemplateAsset"))
         field->value->SetValue(returnValue);

      mObject->setDataField(Torque::StringTableLink->insert(field->fieldName), NULL, field->value->GetValue());

      if (mSceneObject != NULL)
         mSceneObject->refresh();

      if (mComponent != NULL)
         mComponent->mOwnerObject->refresh();
   }
}

// -----------------------
//  MaterialAsset Field
// -----------------------

void wxTorqueInspector::AddMaterialAssetField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxTorqueMaterialAssetField* field = new wxTorqueMaterialAssetField();
   field->fieldName = fieldName;
   field->sizer = new wxBoxSizer(wxHORIZONTAL);

   field->label = new wxStaticText(panel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
   field->label->Wrap(-1);
   field->label->SetForegroundColour(wxColour(255, 255, 255));
   field->sizer->Add(field->label, 1, wxALL, 5);

   wxBoxSizer* valueSizer = new wxBoxSizer(wxHORIZONTAL);

   field->value = new wxTextCtrl(panel, wxID_ANY, value, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER);
   field->value->SetBackgroundColour(Theme::darkBackgroundColor);
   field->value->SetForegroundColour(wxColour(255, 255, 255));
   field->value->SetMinSize(wxSize(1, 20));
   valueSizer->Add(field->value, 1, wxALL, 2);

   wxButton* button = new wxButton(panel, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   button->SetBackgroundColour(Theme::darkBackgroundColor);
   button->SetForegroundColour(wxColour(255, 255, 255));
   button->SetMinSize(wxSize(20, 20));
   button->SetMaxSize(wxSize(20, 20));
   button->Bind(wxEVT_BUTTON, &wxTorqueInspector::OnMaterialAssetFieldSelect, this, -1, -1, field);
   valueSizer->Add(button, 0, wxALL, 2);

   field->sizer->Add(valueSizer, 1, wxEXPAND, 0);

   panelSizer->Add(field->sizer, 1, wxEXPAND, 5);
}

void wxTorqueInspector::OnMaterialAssetFieldSelect(wxCommandEvent& evt)
{
   wxTorqueMaterialAssetField* field = dynamic_cast<wxTorqueMaterialAssetField*>(evt.GetEventUserData());
   if (field)
   {
      wxString returnValue;
      if (mEditorManager->selectMaterial(returnValue))
         field->value->SetValue(returnValue);

      mObject->setDataField(Torque::StringTableLink->insert(field->fieldName), NULL, field->value->GetValue());

      if (mSceneObject != NULL)
         mSceneObject->refresh();

      if (mComponent != NULL)
         mComponent->mOwnerObject->refresh();
   }
}

// -----------------------
//  TextureAsset Field
// -----------------------

void wxTorqueInspector::AddTextureAssetField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value)
{
   wxSizer* panelSizer = panel->GetSizer();

   wxTorqueTextureAssetField* field = new wxTorqueTextureAssetField();
   field->fieldName = fieldName;
   field->sizer = new wxBoxSizer(wxHORIZONTAL);

   field->label = new wxStaticText(panel, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
   field->label->Wrap(-1);
   field->label->SetForegroundColour(wxColour(255, 255, 255));
   field->sizer->Add(field->label, 1, wxALL, 5);

   wxBoxSizer* valueSizer = new wxBoxSizer(wxHORIZONTAL);

   field->value = new wxTextCtrl(panel, wxID_ANY, value, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER);
   field->value->SetBackgroundColour(Theme::darkBackgroundColor);
   field->value->SetForegroundColour(wxColour(255, 255, 255));
   field->value->SetMinSize(wxSize(1, 20));
   valueSizer->Add(field->value, 1, wxALL, 2);

   wxButton* button = new wxButton(panel, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   button->SetBackgroundColour(Theme::darkBackgroundColor);
   button->SetForegroundColour(wxColour(255, 255, 255));
   button->SetMinSize(wxSize(20, 20));
   button->SetMaxSize(wxSize(20, 20));
   button->Bind(wxEVT_BUTTON, &wxTorqueInspector::OnTextureAssetFieldSelect, this, -1, -1, field);
   valueSizer->Add(button, 0, wxALL, 2);

   field->sizer->Add(valueSizer, 1, wxEXPAND, 0);

   panelSizer->Add(field->sizer, 1, wxEXPAND, 5);
}

void wxTorqueInspector::OnTextureAssetFieldSelect(wxCommandEvent& evt)
{
   wxTorqueTextureAssetField* field = dynamic_cast<wxTorqueTextureAssetField*>(evt.GetEventUserData());
   if (field)
   {
      wxString returnValue;
      if (mEditorManager->selectAsset(returnValue, "TextureAsset"))
         field->value->SetValue(returnValue);

      mObject->setDataField(Torque::StringTableLink->insert(field->fieldName), NULL, field->value->GetValue());

      if (mSceneObject != NULL)
         mSceneObject->refresh();

      if (mComponent != NULL)
         mComponent->mOwnerObject->refresh();
   }
}

// -----------------------
//  Inspection Functions
// -----------------------

static S32 QSORT_CALLBACK compareEntries(const void* a, const void* b)
{
   SimFieldDictionary::Entry *fa = *((SimFieldDictionary::Entry **)a);
   SimFieldDictionary::Entry *fb = *((SimFieldDictionary::Entry **)b);
   return dStricmp(fa->slotName, fb->slotName);
}

void wxTorqueInspector::Inspect(SimObject* obj)
{
   if (!obj)
      return;

   Clear();

   mObject        = obj;
   mSceneObject   = NULL;
   mComponent     = NULL;

   wxPanel* group;

   Scene::SceneObject* object = dynamic_cast<Scene::SceneObject*>(obj);
   if (object)
   {
      group = AddGroup(this, wxT("SceneObject"));
      AddStringField(group, "name", wxT("Name"), object->getName());
   }

   Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(obj);
   if (component)
   {
      group = AddGroup(this, wxT("BaseComponent"));
      AddStringField(group, "internalName", wxT("InternalName"), component->getInternalName());
   }

   // Add static fields.
   wxString fieldGroup("");
   bool addFieldGroup = false;
   AbstractClassRep::FieldList fieldList = obj->getFieldList();
   for (Vector<AbstractClassRep::Field>::iterator itr = fieldList.begin(); itr != fieldList.end(); itr++)
   {
      const AbstractClassRep::Field* f = itr;
      if (f->type == AbstractClassRep::DepricatedFieldType || f->type == AbstractClassRep::EndGroupFieldType)
         continue;

      if (f->type == AbstractClassRep::StartGroupFieldType)
      {
         addFieldGroup = true;
         fieldGroup = f->pGroupname;
         continue;
      }

      for (U32 j = 0; S32(j) < f->elementCount; j++)
      {
         const char *val = (*f->getDataFn)(obj, Torque::Con.getData(f->type, (void *)(((const char *)obj) + f->offset), j, f->table, f->flag));

         if (!val)
            continue;

         if (addFieldGroup)
         {
            group = AddGroup(this, fieldGroup);
            addFieldGroup = false;
         }

         if (f->type == Torque::Con.TypeBool)
         {
            bool boolVal;
            Torque::Con.setData(Torque::Con.TypeBool, &boolVal, 0, 1, &val, NULL, 0);
            AddBoolField(group, f->pFieldname, wxString(f->pFieldname), boolVal);
         }

         // Point3F Field
         else if (f->type == Torque::Con.TypePoint3F)
         {
            Point3F pointVal;
            Torque::Con.setData(Torque::Con.TypePoint3F, &pointVal, 0, 1, &val, NULL, 0);
            AddPoint3FField(group, f->pFieldname, wxString(f->pFieldname), pointVal);
         } 

         // ColorF Field
         else if (f->type == Torque::Con.TypeColorF)
         {
            ColorF colorVal;
            Torque::Con.setData(Torque::Con.TypeColorF, &colorVal, 0, 1, &val, NULL, 0);
            wxColour color;
            color.Set(colorVal.red * 255, colorVal.green * 255, colorVal.blue * 255, 255);
            //propertyGrid->Append(new wxColourProperty(f->pFieldname, f->pFieldname, color));
         }
         
         // MeshAsset Field
         else if (f->flag.test(AbstractClassRep::MeshAssetField))
         {
            AddMeshAssetField(group, f->pFieldname, wxString(f->pFieldname), wxString(val));
         }

         // TextureAsset Field
         else if (f->flag.test(AbstractClassRep::TextureAssetField))
         {
            AddTextureAssetField(group, f->pFieldname, wxString(f->pFieldname), wxString(val));
         }

         // ObjectTemplateAsset Field
         else if (f->flag.test(AbstractClassRep::ObjectTemplateAssetField))
         {
            AddObjectTemplateAssetField(group, f->pFieldname, wxString(f->pFieldname), wxString(val));
         }

         // Default to String Field
         else 
         {
            AddStringField(group, f->pFieldname, wxString(f->pFieldname), wxString(val));
         }
      }
   }

   // Get list of dynamic fields and sort by name
   Vector<SimFieldDictionary::Entry *> flist;
   SimFieldDictionary* fieldDictionary = obj->getFieldDictionary();
   for (SimFieldDictionaryIterator ditr(fieldDictionary); *ditr; ++ditr)
      flist.push_back(*ditr);
   dQsort(flist.address(), flist.size(), sizeof(SimFieldDictionary::Entry *), compareEntries);

   // Add dynamic fields.
   wxPanel* materialsGroup = NULL;
   wxPanel* submeshGroup   = NULL;
   wxPanel* otherGroup     = NULL;
   for (U32 i = 0; i < (U32)flist.size(); i++)
   {
      SimFieldDictionary::Entry* entry = flist[i];

      if (dStrncmp(entry->slotName, "MaterialAsset", 13) == 0)
      {
         if (materialsGroup == NULL)
            materialsGroup = AddGroup(this, wxT("Materials"));
         AddMaterialAssetField(materialsGroup, entry->slotName, wxString(entry->slotName), wxString(entry->value));
      }
      else if (dStrncmp(entry->slotName, "SubMesh", 7) == 0)
      {
         if (submeshGroup == NULL)
            submeshGroup = AddGroup(this, wxT("SubMeshes"));

         bool boolVal;
         const char* val = entry->value;
         Torque::Con.setData(Torque::Con.TypeBool, &boolVal, 0, 1, &val, NULL, 0);
         AddBoolField(submeshGroup, entry->slotName, wxString(entry->slotName), boolVal);
      }
      else
      {
         if (otherGroup == NULL)
            otherGroup = AddGroup(this, wxT("Other"));
         AddStringField(otherGroup, entry->slotName, wxString(entry->slotName), wxString(entry->value));
      }
   }

   wxSize size = this->GetParent()->GetBestVirtualSize();
   this->GetParent()->SetVirtualSize(size);
   this->SetVirtualSize(size);
}

void wxTorqueInspector::Inspect(Scene::SceneObject* sceneObject)
{
   mObject = sceneObject;
   Inspect(mObject);

   mSceneObject   = sceneObject;
   mComponent     = NULL;
};

void wxTorqueInspector::Inspect(Scene::BaseComponent* component)
{
   mObject = component;
   Inspect(mObject);

   mSceneObject   = NULL;
   mComponent     = component;
};

void wxTorqueInspector::Inspect(const AssetDefinition* assetDef)
{
   // Fetch the asset.
   AssetBase* asset = Torque::AssetDatabaseLink.getAssetBase(assetDef->mAssetId);

   Clear();

   mObject        = asset;
   mSceneObject   = NULL;
   mComponent     = NULL;

   // Determine if this is a material asset.
   bool isMaterialAsset = (dStrcmp(assetDef->mAssetType, "MaterialAsset") == 0);
   wxPanel* group = NULL;
   wxPanel* texturesGroup = NULL;

   wxString fieldGroup("");
   bool addFieldGroup = false;

   // Static Fields
   AbstractClassRep::FieldList fieldList = asset->getFieldList();
   for (Vector<AbstractClassRep::Field>::iterator itr = fieldList.begin(); itr != fieldList.end(); itr++)
   {
      const AbstractClassRep::Field* f = itr;
      if (f->type == AbstractClassRep::DepricatedFieldType ||
         f->type == AbstractClassRep::EndGroupFieldType)
         continue;

      if (f->type == AbstractClassRep::StartGroupFieldType)
      {
         addFieldGroup = true;
         fieldGroup = f->pGroupname;
         continue;
      }

      for (U32 j = 0; S32(j) < f->elementCount; j++)
      {
         const char *val = (*f->getDataFn)(asset, Torque::Con.getData(f->type, (void *)(((const char *)asset) + f->offset), j, f->table, f->flag));

         if (!val)
            continue;

         if (addFieldGroup)
         {
            if (fieldGroup == "Textures")
               texturesGroup = AddGroup(this, fieldGroup);
            else
               group = AddGroup(this, fieldGroup);
            addFieldGroup = false;
         }

         if (f->type == Torque::Con.TypeBool)
         {
            bool boolVal;
            Torque::Con.setData(Torque::Con.TypeBool, &boolVal, 0, 1, &val, NULL, 0);
            AddBoolField(group, f->pFieldname, wxString(f->pFieldname), boolVal);
         }

         // Point3F Field
         else if (f->type == Torque::Con.TypePoint3F)
         {
            Point3F pointVal;
            Torque::Con.setData(Torque::Con.TypePoint3F, &pointVal, 0, 1, &val, NULL, 0);
            AddPoint3FField(group, f->pFieldname, wxString(f->pFieldname), pointVal);
         }

         // MeshAsset Field
         else if (f->flag.test(AbstractClassRep::MeshAssetField))
         {
            AddMeshAssetField(group, f->pFieldname, wxString(f->pFieldname), wxString(val));
         }

         // ObjectTemplateAsset Field
         else if (f->flag.test(AbstractClassRep::ObjectTemplateAssetField))
         {
            AddObjectTemplateAssetField(group, f->pFieldname, wxString(f->pFieldname), wxString(val));
         }

         // Default to String Field
         else
         {
            AddStringField(group, f->pFieldname, wxString(f->pFieldname), wxString(val));
         }
      }
   }

   // Get list of dynamic fields and sort by name
   Vector<SimFieldDictionary::Entry *> flist;
   SimFieldDictionary* fieldDictionary = asset->getFieldDictionary();
   for (SimFieldDictionaryIterator ditr(fieldDictionary); *ditr; ++ditr)
      flist.push_back(*ditr);
   dQsort(flist.address(), flist.size(), sizeof(SimFieldDictionary::Entry *), compareEntries);

   // Add dynamic fields.
   group = AddGroup(this, wxT("Other"));
   for (U32 i = 0; i < (U32)flist.size(); i++)
   {
      SimFieldDictionary::Entry* entry = flist[i];

      if (isMaterialAsset && dStrncmp(entry->slotName, "TextureAsset", 12) == 0)
         continue;
      else if (isMaterialAsset && dStrncmp(entry->slotName, "TextureFile", 11) == 0)
         continue;
      else 
         AddStringField(group, entry->slotName, wxString(entry->slotName), wxString(entry->value));
   }

   // Determine if this is a material asset.
   if (isMaterialAsset)
   {
      MaterialAsset* mSelectedMaterialAsset = dynamic_cast<MaterialAsset*>(asset);

      if (texturesGroup == NULL)
         texturesGroup = AddGroup(this, wxT("Textures"));

      for (S32 n = 0; n < mSelectedMaterialAsset->getTextureCount(); ++n)
      {
         char fieldName[32];

         // Texture Asset?
         dSprintf(fieldName, 32, "TextureAsset%d", n);
         const char* textureAssetId = mSelectedMaterialAsset->getDataField(Torque::StringTableLink->insert(fieldName), NULL);
         AddTextureAssetField(texturesGroup, fieldName, wxString(fieldName), wxString(textureAssetId));

         // Texture File?
         dSprintf(fieldName, 32, "TextureFile%d", n);
         const char* texturePath = mSelectedMaterialAsset->expandAssetFilePath(mSelectedMaterialAsset->getDataField(Torque::StringTableLink->insert(fieldName), NULL));
         if (texturePath)
            AddFileField(texturesGroup, fieldName, wxString(fieldName), wxString(texturePath));
         else
            AddFileField(texturesGroup, fieldName, wxString(fieldName), wxString(""));
      }
   }

   wxSize size = this->GetParent()->GetBestVirtualSize();
   this->GetParent()->SetVirtualSize(size);
   this->SetVirtualSize(size);
}