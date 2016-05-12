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

#ifndef WXTORQUEINSPECTOR_H
#define WXTORQUEINSPECTOR_H

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/dynarray.h>
#include <wx/collpane.h>
#include <wx/hyperlink.h>

#ifdef __WXMSW__
#ifdef _DEBUG
//#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif
#endif // __WXMSW__

#ifdef WXMAKINGDLL_FNB
#    define WXDLLIMPEXP_FNB WXEXPORT
#elif defined(WXUSINGDLL_FNB)
#    define WXDLLIMPEXP_FNB WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_FNB
#endif // WXMAKINGDLL_FNB

#include <wx/dcbuffer.h>
#include <wx/dataobj.h>
#include <wx/dnd.h>

#ifndef _SCENE_OBJECT_H_
#include <scene/object.h>
#endif

#ifndef __TORQUE6EDITORUI_H__
#include "../../Torque6EditorUI.h"
#endif

class ProjectManager;

class wxTorqueGroup : public wxObject
{
   public:
      wxPanel*    headerPanel;
      wxSizer*    headerSizer;
      wxButton*   headerButton;
      wxPanel*    panel;
      wxBoxSizer* sizer;
};

class wxTorqueStringField : public wxObject
{
   public:
      const char*    fieldName;

      wxBoxSizer*    sizer;
      wxStaticText*  label;
      wxTextCtrl*    value;
};

class wxTorqueBoolField : public wxObject
{
public:
   const char*    fieldName;
   wxCheckBox*    value;
};

class wxTorqueFileField : public wxObject
{
public:
   const char*       fieldName;
   wxFilePickerCtrl* value;
};

class wxTorquePoint3FField : public wxObject
{
public:
   const char*    fieldName;

   wxTextCtrl*    xValue;
   wxTextCtrl*    yValue;
   wxTextCtrl*    zValue;
};

class wxTorqueMeshAssetField : public wxObject
{
public:
   const char*    fieldName;

   wxBoxSizer*    sizer;
   wxStaticText*  label;
   wxTextCtrl*    value;
};

class wxTorqueObjectTemplateAssetField : public wxObject
{
public:
   const char*    fieldName;

   wxBoxSizer*    sizer;
   wxStaticText*  label;
   wxTextCtrl*    value;
};

class wxTorqueMaterialAssetField : public wxObject
{
public:
   const char*    fieldName;

   wxBoxSizer*    sizer;
   wxStaticText*  label;
   wxTextCtrl*    value;
};

class wxTorqueTextureAssetField : public wxObject
{
public:
   const char*    fieldName;

   wxBoxSizer*    sizer;
   wxStaticText*  label;
   wxTextCtrl*    value;
};

class wxTorqueInspector : public wxPanel
{
   protected:
      ProjectManager*   mProjectManager;
      wxPanel*          mGroupPanel;
      wxBoxSizer*       mContentsSizer;
      wxImageList*      mIconList;

      SimObject*              mObject;
      Scene::SceneObject*     mSceneObject;
      Scene::BaseComponent*   mComponent;

   public:
      wxTorqueInspector() : wxPanel()
      {
         Init();
      }

      wxTorqueInspector(ProjectManager* projectManager, 
         wxWindow *parent,
         wxWindowID winid = wxID_ANY,
         const wxPoint& pos = wxDefaultPosition,
         const wxSize& size = wxDefaultSize,
         long style = wxTAB_TRAVERSAL | wxNO_BORDER,
         const wxString& name = wxPanelNameStr)
         : wxPanel(parent, winid, pos, size, style, name)
      {
         mProjectManager = projectManager;
         Init();
      }

      void Init();
      void Clear();

      // Groups
      wxPanel* AddGroup(wxPanel* panel, const wxString& label);
      void OnGroupClick(wxCommandEvent& evt);

      // String Field
      void AddStringField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value);
      void OnStringFieldChanged(wxCommandEvent& evt);

      // Bool Field
      void AddBoolField(wxPanel* panel, const char* fieldName, const wxString& label, bool value);
      void OnBoolFieldChanged(wxCommandEvent& evt);

      // String Field
      void AddFileField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value);
      void OnFileFieldChanged(wxCommandEvent& evt);

      // Point3F Field
      void AddPoint3FField(wxPanel* panel, const char* fieldName, const wxString& label, const Point3F& value);
      void OnPoint3FFieldChanged(wxCommandEvent& evt);

      // MeshAsset Field
      void AddMeshAssetField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value);
      void OnMeshAssetFieldSelect(wxCommandEvent& evt);

      // ObjectTemplateAsset Field
      void AddObjectTemplateAssetField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value);
      void OnObjectTemplateAssetFieldSelect(wxCommandEvent& evt);

      // MaterialAsset Field
      void AddMaterialAssetField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value);
      void OnMaterialAssetFieldSelect(wxCommandEvent& evt);

      // TextureAsset Field
      void AddTextureAssetField(wxPanel* panel, const char* fieldName, const wxString& label, const wxString& value);
      void OnTextureAssetFieldSelect(wxCommandEvent& evt);

      // Button Field
      void AddButtonField(wxPanel* panel, const char* fieldName, const wxString& label);

      // Inspection Functions
      void Inspect(SimObject* object);
      void Inspect(Scene::SceneObject* sceneObject);
      void Inspect(Scene::BaseComponent* component);
      void Inspect(const AssetDefinition* assetDef);

   private:
      DECLARE_DYNAMIC_CLASS(wxTorqueInspector);
      DECLARE_EVENT_TABLE();
};

#endif // WXTORQUEINSPECTOR
