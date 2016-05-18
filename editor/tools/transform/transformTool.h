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

#ifndef TRANSFORM_TOOL_H
#define TRANSFORM_TOOL_H

#ifndef EDITORMANAGER_H
#include "editorManager.h"
#endif

#ifndef __TORQUE6EDITORUI_H__
#include "Torque6EditorUI.h"
#endif

#ifndef _WX_TREECTRL_H_BASE_
#include <wx/treectrl.h>
#endif

#ifndef _SCENE_OBJECT_H_
#include <scene/object.h>
#endif

#ifndef _GIZMO_H_
#include "gizmo.h"
#endif

#ifndef WXFLATNOTEBOOK_H
#include "widgets/wxFlatNotebook/wxFlatNotebook.h"
#endif

class TransformTool : public EditorTool
{
   typedef EditorTool Parent;

   protected:
      Scene::SceneObject*     mSelectedObject;
      Scene::BaseComponent*   mSelectedComponent;

      Scene::SceneObject*     mMenuObject;
      Scene::BaseComponent*   mMenuComponent;

      // Translate
      wxBitmapButton*   mTranslateBtn;
      wxBitmap*         mTranslateIcon;
      wxBitmap*         mTranslateHighlightIcon;
      wxMenu*           mTranslateMenu;

      // Rotate
      wxBitmapButton*   mRotateBtn;
      wxBitmap*         mRotateIcon;
      wxBitmap*         mRotateHighlightIcon;
      wxMenu*           mRotateMenu;

      // Scale
      wxBitmapButton*   mScaleBtn;
      wxBitmap*         mScaleIcon;
      wxBitmap*         mScaleHighlightIcon;
      wxMenu*           mScaleMenu;

      bool mRefreshing;
      Gizmo mGizmo;

      TextureObject* mLightIcon;

   public:
      TransformTool(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager);
      ~TransformTool();

      void OnObjectSelected(wxTorqueObjectEvent& evt);
      void OnToolbarEvent(wxCommandEvent& evt);
      void OnToolbarDropdownEvent(wxCommandEvent& evt);
      void OnTranslateMenuEvent(wxCommandEvent& evt);
      void OnRotateMenuEvent(wxCommandEvent& evt);
      void OnScaleMenuEvent(wxCommandEvent& evt);

      virtual void initTool();
      virtual void onActivateTool(S32 index = -1);
      virtual void onDeactivateTool();
      virtual void renderTool();

      virtual bool onMouseLeftDown(int x, int y);
      virtual bool onMouseLeftUp(int x, int y);
      virtual bool onMouseMove(int x, int y);

      virtual void onSceneChanged();
      virtual void onProjectLoaded(const wxString& projectName, const wxString& projectPath);
      virtual void onProjectClosed();
};

#endif // TRANSFORM_TOOL_H
