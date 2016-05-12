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

#ifndef WXTORQUEASSETSELECTDIALOG_H
#define WXTORQUEASSETSELECTDIALOG_H

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

#ifndef WXTORQUEASSETTREE_H
#include "wxTorqueAssetTree.h"
#endif

#ifndef _PROJECTMANAGER_H_
#include "../../project/projectManager.h"
#endif

class wxTorqueAssetSelectDialog : public wxDialog
{
   protected:
      ProjectManager*         mProjectManager;
      wxButton*               m_button6;
      wxButton*               m_button61;
      wxTorqueAssetTree*      assetList;
      const AssetDefinition*  mSelectedAsset;

   public:
      wxTorqueAssetSelectDialog(ProjectManager* projectManager,
         wxWindow* parent,
         wxWindowID id = wxID_ANY, 
         const wxString& title = wxT("Select Asset"), 
         const wxPoint& pos = wxDefaultPosition, 
         const wxSize& size = wxSize(442, 442), 
         long style = wxDEFAULT_DIALOG_STYLE);

      ~wxTorqueAssetSelectDialog();

      void OnSelectButton(wxCommandEvent& evt);
      void OnCancelButton(wxCommandEvent& evt);
      void OnAssetTreeEvent(wxTreeEvent& evt);
      bool SelectAsset(wxString &returnValue, const char* filter = NULL);
};

#endif // WXTORQUEASSETSELECTDIALOG_H
