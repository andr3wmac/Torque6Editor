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

#ifndef WXTORQUEASSETTREE_H
#define WXTORQUEASSETTREE_H

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

#ifndef _PROJECTMANAGER_H_
#include "../../project/projectManager.h"
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
      : obj(_obj)
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

class wxTorqueAssetTree : public wxTreeCtrl
{
   protected:
      ProjectManager*   mProjectManager;

   public:
      wxTorqueAssetTree() : wxTreeCtrl()
      {
         Init();
      }

      wxTorqueAssetTree(ProjectManager* projectManager,
         wxWindow *parent,
         wxWindowID winid = wxID_ANY,
         const wxPoint& pos = wxDefaultPosition,
         const wxSize& size = wxDefaultSize,
         long style = wxTAB_TRAVERSAL | wxNO_BORDER,
         const wxValidator& validator = wxDefaultValidator,
         const wxString& name = wxPanelNameStr)
         : wxTreeCtrl(parent, winid, pos, size, style, validator, name)
      {
         mProjectManager = projectManager;
         Init();
      }

      void Init();
      void ShowAssets(const char* filter = NULL);

   private:
      DECLARE_DYNAMIC_CLASS(wxTorqueAssetTree);
      DECLARE_EVENT_TABLE();
};

#endif // WXTORQUEASSETTREE
