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
#include "Torque6EditorUI.h"
#include "theme.h"

#include "transformTool.h"
#include "math/mPlaneSet.h"
#include <bx/bx.h>
#include <bx/fpumath.h>
#include <debugdraw/debugdraw.h>

TransformTool::TransformTool(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager)
   : Parent(_EditorManager, _frame, _manager),
     mSelectedObject(NULL),
     mSelectedComponent(NULL),
     mMenuObject(NULL),
     mMenuComponent(NULL),
     mLightIcon(NULL),
     mRefreshing(false)
{
   mSelectionBounds     = false;
   mMouseDown           = false;
   mBoxSelection        = false;
   mDebugBoxSelection   = false;
   mDebugWorldRay       = false;
   mLastRayStart        = Point3F::Zero;
   mLastRayEnd          = Point3F::Zero;

   // Translate Menu
   mTranslateMenu = new wxMenu;
   mTranslateMenu->Append(0, wxT("Local"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(1, wxT("Global"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->AppendSeparator();
   mTranslateMenu->Append(2, wxT("Snap: None"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(3, wxT("Snap: 0.1"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(4, wxT("Snap: 0.5"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(5, wxT("Snap: 1.0"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Append(6, wxT("Snap: 5.0"), wxEmptyString, wxITEM_RADIO);
   mTranslateMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TransformTool::OnTranslateMenuEvent), NULL, this);

   // Rotate Menu
   mRotateMenu = new wxMenu;
   mRotateMenu->Append(0, wxT("Local"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(1, wxT("Global"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->AppendSeparator();
   mRotateMenu->Append(2, wxT("Snap: None"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(3, wxT("Snap: 5 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(4, wxT("Snap: 15 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(5, wxT("Snap: 45 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Append(6, wxT("Snap: 90 Degrees"), wxEmptyString, wxITEM_RADIO);
   mRotateMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TransformTool::OnRotateMenuEvent), NULL, this);

   // Scale Menu
   mScaleMenu = new wxMenu;
   mScaleMenu->Append(0, wxT("Snap: None"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(1, wxT("Snap: 0.1"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(2, wxT("Snap: 0.5"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(3, wxT("Snap: 1.0"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Append(4, wxT("Snap: 5.0"), wxEmptyString, wxITEM_RADIO);
   mScaleMenu->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TransformTool::OnScaleMenuEvent), NULL, this);

   // Events
   Bind(wxTORQUE_SELECT_OBJECT, &TransformTool::OnObjectSelected, this);

   // Icons
   mTranslateIcon          = new wxBitmap(wxT("images/translate.png"), wxBITMAP_TYPE_ANY);
   mTranslateHighlightIcon = new wxBitmap(wxT("images/translate_highlight.png"), wxBITMAP_TYPE_ANY);
   mRotateIcon             = new wxBitmap(wxT("images/rotate.png"), wxBITMAP_TYPE_ANY);
   mRotateHighlightIcon    = new wxBitmap(wxT("images/rotate_highlight.png"), wxBITMAP_TYPE_ANY);
   mScaleIcon              = new wxBitmap(wxT("images/scale.png"), wxBITMAP_TYPE_ANY);
   mScaleHighlightIcon     = new wxBitmap(wxT("images/scale_highlight.png"), wxBITMAP_TYPE_ANY);
}

TransformTool::~TransformTool()
{
   mRefreshing = true;
   delete mTranslateMenu;
   delete mRotateMenu;
   delete mScaleMenu;
}

void TransformTool::initTool()
{
   mGizmo.mEditorManager = mEditorManager;

   // Translate
   mTranslateBtn = new wxBitmapButton(mFrame->toolbar, 1, *mTranslateIcon, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   mTranslateBtn->SetBackgroundColour(Theme::darkBackgroundColor);
   mTranslateBtn->SetMinSize(wxSize(36, 36));
   mTranslateBtn->Bind(wxEVT_BUTTON, &TransformTool::OnToolbarEvent, this, -1, -1, NULL);
   mFrame->toolbarContents->Add(mTranslateBtn, 0, 0, 5);

   wxBitmapButton* translateDownBtn = new wxBitmapButton(mFrame->toolbar, 2, wxBitmap(wxT("images/smallDownArrow.png"), wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   translateDownBtn->SetBackgroundColour(Theme::darkBackgroundColor);
   translateDownBtn->SetMinSize(wxSize(16, 36));
   translateDownBtn->Bind(wxEVT_BUTTON, &TransformTool::OnToolbarDropdownEvent, this, -1, -1, NULL);
   mFrame->toolbarContents->Add(translateDownBtn, 0, 0, 5);

   // Rotate
   mRotateBtn = new wxBitmapButton(mFrame->toolbar, 3, *mRotateIcon, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   mRotateBtn->SetBackgroundColour(Theme::darkBackgroundColor);
   mRotateBtn->SetMinSize(wxSize(36, 36));
   mRotateBtn->Bind(wxEVT_BUTTON, &TransformTool::OnToolbarEvent, this, -1, -1, NULL);
   mFrame->toolbarContents->Add(mRotateBtn, 0, 0, 5);

   wxBitmapButton* rotateDownBtn = new wxBitmapButton(mFrame->toolbar, 4, wxBitmap(wxT("images/smallDownArrow.png"), wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   rotateDownBtn->SetBackgroundColour(Theme::darkBackgroundColor);
   rotateDownBtn->SetMinSize(wxSize(16, 36));
   rotateDownBtn->Bind(wxEVT_BUTTON, &TransformTool::OnToolbarDropdownEvent, this, -1, -1, NULL);
   mFrame->toolbarContents->Add(rotateDownBtn, 0, 0, 5);

   // Scale
   mScaleBtn = new wxBitmapButton(mFrame->toolbar, 5, *mScaleIcon, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   mScaleBtn->SetBackgroundColour(Theme::darkBackgroundColor);
   mScaleBtn->SetMinSize(wxSize(36, 36));
   mScaleBtn->Bind(wxEVT_BUTTON, &TransformTool::OnToolbarEvent, this, -1, -1, NULL);
   mFrame->toolbarContents->Add(mScaleBtn, 0, 0, 5);

   wxBitmapButton* scaleDownBtn = new wxBitmapButton(mFrame->toolbar, 6, wxBitmap(wxT("images/smallDownArrow.png"), wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
   scaleDownBtn->SetBackgroundColour(Theme::darkBackgroundColor);
   scaleDownBtn->SetMinSize(wxSize(16, 36));
   scaleDownBtn->Bind(wxEVT_BUTTON, &TransformTool::OnToolbarDropdownEvent, this, -1, -1, NULL);
   mFrame->toolbarContents->Add(scaleDownBtn, 0, 0, 5);
}

void TransformTool::onActivateTool(S32 index)
{
   Parent::onActivateTool(index);

   // Translate
   if (index == 0)
   {
      mGizmo.mMode = 0;
      mTranslateBtn->SetBitmap(*mTranslateHighlightIcon);
      mRotateBtn->SetBitmap(*mRotateIcon);
      mScaleBtn->SetBitmap(*mScaleIcon);
   }
   
   // Rotate
   if (index == 1)
   {
      mGizmo.mMode = 1;
      mTranslateBtn->SetBitmap(*mTranslateIcon);
      mRotateBtn->SetBitmap(*mRotateHighlightIcon);
      mScaleBtn->SetBitmap(*mScaleIcon);
   }

   // Scale
   if (index == 2)
   {
      mGizmo.mMode = 2;
      mTranslateBtn->SetBitmap(*mTranslateIcon);
      mRotateBtn->SetBitmap(*mRotateIcon);
      mScaleBtn->SetBitmap(*mScaleHighlightIcon);
   }
}

void TransformTool::onDeactivateTool()
{
   Parent::onDeactivateTool();

   // Reset icons.
   mTranslateBtn->SetBitmap(*mTranslateIcon);
   mRotateBtn->SetBitmap(*mRotateIcon);
   mScaleBtn->SetBitmap(*mScaleIcon);
}

void TransformTool::renderTool()
{
   if (!mActive)
      return;

   // Grid
   {
      Point3F editorPos = mEditorManager->mEditorCamera.getWorldPosition();
      editorPos = editorPos / 10.0f;
      editorPos.x = mFloor(editorPos.x);
      editorPos.y = mFloor(editorPos.y);
      editorPos = editorPos * 10.0f;

      Torque::Debug.ddPush();
      Torque::Debug.ddSetState(true, false, true);

      Torque::Debug.ddSetWireframe(true);
      Torque::Debug.ddSetColor(BGFXCOLOR_RGBA(255, 255, 255, 255));
      F32 gridNormal[3] = { 0.0f, 0.0f, 1.0f };
      F32 gridPos[3] = { editorPos.x, editorPos.y, -0.01f };
      Torque::Debug.ddDrawGrid(gridNormal, gridPos, 100, 10.0f);
   }

   // Draw Light Icons
   /*if (mLightIcon != NULL)
   {
      Vector<Lighting::LightData*> lightList = Torque::Lighting.getLightList();
      for (S32 n = 0; n < lightList.size(); ++n)
      {
         Lighting::LightData* light = lightList[n];

         Torque::Graphics.drawBillboard(mEditorManager->mRenderLayer4View->id,
                                              mLightIcon,
                                              light->position,
                                              1.0f, 1.0f,
                                              ColorI(light->color[0] * 255, light->color[1] * 255, light->color[2] * 255, 255),
                                              NULL);
      }
   }*/

   if (mMultiselect)
   {
      Box3F multiSelectBox;
      multiSelectBox.set(Point3F(0, 0, 0));
         
      for (S32 n = 0; n < mSelectedObjects.size(); ++n)
      {
         Scene::SceneObject* obj = dynamic_cast<Scene::SceneObject*>(mSelectedObjects[n]);
         if (obj)
         {
            if (n == 0)
               multiSelectBox = obj->mBoundingBox;
            else
               multiSelectBox.intersect(obj->mBoundingBox);
         }

         Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(mSelectedObjects[n]);
         if (component)
         {
            Box3F boundingBox = component->getBoundingBox();
            boundingBox.transform(component->mTransform.matrix);

            if (n == 0)
               multiSelectBox = boundingBox;
            else
               multiSelectBox.intersect(boundingBox);
         }
      }

      mSelectionBounds        = true;
      mSelectionBoundsStart   = multiSelectBox.minExtents;
      mSelectionBoundsEnd     = multiSelectBox.maxExtents;
   }

   // Object Selected
   if (mSelectedObject != NULL && mSelectedComponent == NULL)
   {
      mSelectionBounds        = true;
      mSelectionBoundsStart   = mSelectedObject->mBoundingBox.minExtents;
      mSelectionBoundsEnd     = mSelectedObject->mBoundingBox.maxExtents;
   }

   // Component Selected
   if (mSelectedObject != NULL && mSelectedComponent != NULL)
   {
      Box3F boundingBox = mSelectedComponent->getBoundingBox();
      boundingBox.transform(mSelectedObject->mTransform.matrix);

      mSelectionBounds        = true;
      mSelectionBoundsStart   = boundingBox.minExtents;
      mSelectionBoundsEnd     = boundingBox.maxExtents;
   }

   // Selection Bounding Box
   if (mSelectionBounds)
   {
      Aabb debugBox;
      debugBox.m_min[0] = mSelectionBoundsStart.x;
      debugBox.m_min[1] = mSelectionBoundsStart.y;
      debugBox.m_min[2] = mSelectionBoundsStart.z;
      debugBox.m_max[0] = mSelectionBoundsEnd.x;
      debugBox.m_max[1] = mSelectionBoundsEnd.y;
      debugBox.m_max[2] = mSelectionBoundsEnd.z;

      Torque::Debug.ddSetWireframe(true);
      Torque::Debug.ddSetColor(BGFXCOLOR_RGBA(mSelectionBoundsColor.red, mSelectionBoundsColor.green, mSelectionBoundsColor.blue, mSelectionBoundsColor.alpha));
      Torque::Debug.ddDrawAabb(debugBox);

      Torque::Debug.ddPop();
   }

   // Gizmo
   mGizmo.render();

   // Debug
   if (mDebugWorldRay)
   {
      Torque::Debug.ddMoveTo(mLastRayStart.x, mLastRayStart.y, mLastRayStart.z);
      Torque::Debug.ddLineTo(mLastRayEnd.x, mLastRayEnd.y, mLastRayEnd.z);
   }

   if (mDebugBoxSelection)
   {
      for (U32 i = 0; i < 4; ++i)
      {
         Torque::Debug.ddMoveTo(mBoxNearPoint.x, mBoxNearPoint.y, mBoxNearPoint.z);
         Torque::Debug.ddLineTo(mBoxFarPoint[i].x, mBoxFarPoint[i].y, mBoxFarPoint[i].z);
      }
   }
}

void TransformTool::dglRenderTool()
{
   if (!mActive)
      return;

   if (mBoxSelection)
   {
      Torque::Graphics.dglDrawRectFill(mMouseDownPosition, mMousePosition, ColorI(255, 255, 255, 64));
      Torque::Graphics.dglDrawRect(RectI(mMouseDownPosition, mMousePosition - mMouseDownPosition), ColorI(255, 255, 255, 255), 2.0f);
   }
}

bool TransformTool::onMouseLeftDown(int x, int y)
{
   if (!mActive)
      return false;

   mMouseDown = true;
   mMouseDownPosition.set(x, y);
   mGizmo.onMouseLeftDown(x, y);
   return false;
}

bool TransformTool::onMouseLeftUp(int x, int y)
{
   if (!mActive)
      return false;

   if (!mGizmo.onMouseLeftUp(x, y))
   {
      Point2I delta = mMousePosition - mMouseDownPosition;
      F32 dist = delta.len();

      if (mBoxSelection && mMousePosition.x != mMouseDownPosition.x && mMousePosition.y != mMouseDownPosition.y)
      {
         //mDebugBoxSelection = true;

         Torque::Rendering.screenToWorld(Point2I(mMouseDownPosition.x, mMouseDownPosition.y), mBoxNearPoint, mBoxFarPoint[0]);
         Torque::Rendering.screenToWorld(Point2I(mMousePosition.x, mMouseDownPosition.y), mBoxNearPoint, mBoxFarPoint[1]);
         Torque::Rendering.screenToWorld(Point2I(mMouseDownPosition.x, mMousePosition.y), mBoxNearPoint, mBoxFarPoint[2]);
         Torque::Rendering.screenToWorld(Point2I(mMousePosition.x, mMousePosition.y), mBoxNearPoint, mBoxFarPoint[3]);

         PlaneF planes[4];
         planes[0].set(mBoxNearPoint, mBoxFarPoint[2], mBoxFarPoint[0]);
         planes[1].set(mBoxNearPoint, mBoxFarPoint[1], mBoxFarPoint[3]);
         planes[2].set(mBoxNearPoint, mBoxFarPoint[0], mBoxFarPoint[1]);
         planes[3].set(mBoxNearPoint, mBoxFarPoint[3], mBoxFarPoint[2]);
         PlaneSetF planeSet(planes, 4);

         Vector<Scene::SceneObject*> objects = Torque::Scene.boxSearch(planeSet);
         if (objects.size() > 0)
         {
            wxTorqueObjectEvent evt(0, wxTORQUE_SELECT_OBJECT);

            for (S32 n = 0; n < objects.size(); ++n)
               evt.AddObject(objects[n]);

            if (mShiftDown)
            {
               if (mSelectedObject != NULL)
                  evt.AddObject(mSelectedObject);

               for (S32 n = 0; n < mSelectedObjects.size(); ++n)
                  evt.AddObject(mSelectedObjects[n]);

               if (mSelectedComponent != NULL)
                  evt.AddObject(mSelectedComponent);
            }

            // Broadcast the selections.
            mEditorManager->postEvent(evt);
         }
      }
      else 
      {
         Torque::Rendering.screenToWorld(Point2I(x, y), mLastRayStart, mLastRayEnd);
         Point3F editorPos = mEditorManager->mEditorCamera.getWorldPosition();

         //mDebugWorldRay = true;

         Scene::SceneObject* hit = Torque::Scene.raycast(mLastRayStart, mLastRayEnd);
         if (mSelectedObject != hit)
         {
            if (hit)
            {
               wxTorqueObjectEvent evt(0, wxTORQUE_SELECT_OBJECT);
               evt.AddObject(hit);

               if (mShiftDown)
               {
                  if ( mSelectedObject != NULL )
                     evt.AddObject(mSelectedObject);

                  for (S32 n = 0; n < mSelectedObjects.size(); ++n)
                     evt.AddObject(mSelectedObjects[n]);

                  if (mSelectedComponent != NULL)
                     evt.AddObject(mSelectedComponent);
               }

               // Broadcast the hit.
               mEditorManager->postEvent(evt);
            }
         }
      }
   }

   mMouseDown     = false;
   mBoxSelection  = false;

   return false;
}

bool TransformTool::onMouseMove(int x, int y)
{
   if (!mActive)
      return false;

   mMousePosition.set(x, y);
   if (!mGizmo.onMouseMove(x, y))
   {
      if (mMouseDown)
      {
         Point2I delta = mMousePosition - mMouseDownPosition;
         F32 dist = delta.len();
         if (dist > 5.0f)
            mBoxSelection = true;
      }
   }
   return false;
}

bool TransformTool::onKeyDown(wxKeyEvent& evt)
{
   mShiftDown = evt.ShiftDown();
   return false;
}

bool TransformTool::onKeyUp(wxKeyEvent& evt)
{
   mShiftDown = evt.ShiftDown();
   return false;
}

void TransformTool::onSceneChanged()
{

}

void TransformTool::onProjectLoaded(const wxString& projectName, const wxString& projectPath)
{
   if ( mLightIcon == NULL )
      mLightIcon = Torque::Graphics.loadTexture("light.png", TextureHandle::BitmapKeepTexture, BGFX_TEXTURE_NONE, false, false);
}

void TransformTool::onProjectClosed()
{

}

void TransformTool::OnObjectSelected(wxTorqueObjectEvent& evt)
{
   if (!mActive)
      return;

   mMultiselect         = false;
   mSelectedObject      = NULL;
   mSelectedComponent   = NULL;
   mSelectedObjects.clear();

   Vector<SimObject*> objects = evt.GetObjects();
   if (objects.size() < 1)
      return;

   if (objects.size() > 1)
   {
      mMultiselect      = true;
      mSelectedObjects  = objects;

      // Update the gizmo.
      mGizmo.selectObjects(mSelectedObjects);
      return;
   }

   Scene::SceneObject* obj = dynamic_cast<Scene::SceneObject*>(objects[0]);
   if (obj)
   {
      mSelectedObject    = obj;

      // Update the gizmo.
      mGizmo.selectObject(obj);
      return;
   }

   Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(objects[0]);
   if (component)
   {
      mSelectedObject      = component->mOwnerObject;
      mSelectedComponent   = component;

      // Update the gizmo.
      mGizmo.selectComponent(component);
      return;
   }
}

void TransformTool::OnToolbarEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 1:
         EditorTool::activateTool(this, 0);
         break;

      case 3:
         EditorTool::activateTool(this, 1);
         break;

      case 5:
         EditorTool::activateTool(this, 2);
         break;

      default:
         break;
   }
}

void TransformTool::OnToolbarDropdownEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 2:
         mFrame->PopupMenu(mTranslateMenu, wxDefaultPosition);
         break;

      case 4:
         mFrame->PopupMenu(mRotateMenu, wxDefaultPosition);
         break;

      case 6:
         mFrame->PopupMenu(mScaleMenu, wxDefaultPosition);
         break;

      default:
         break;
   }
}

void TransformTool::OnTranslateMenuEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 0:
         mGizmo.mTranslateLocal = true;
         break;

      case 1:
         mGizmo.mTranslateLocal = false;
         break;

      case 2:
         mGizmo.mTranslateSnap = 0.0f;
         break;

      case 3:
         mGizmo.mTranslateSnap = 0.1f;
         break;

      case 4:
         mGizmo.mTranslateSnap = 0.5f;
         break;

      case 5:
         mGizmo.mTranslateSnap = 1.0f;
         break;

      case 6:
         mGizmo.mTranslateSnap = 5.0f;
         break;
   }
}

void TransformTool::OnRotateMenuEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 0:
         mGizmo.mRotateLocal = true;
         break;

      case 1:
         mGizmo.mRotateLocal = false;
         break;

      case 2:
         mGizmo.mRotateSnap = 0.0f;
         break;

      case 3:
         mGizmo.mRotateSnap = M_PI_F / 36.0f;
         break;

      case 4:
         mGizmo.mRotateSnap = M_PI_F / 12.0f;
         break;

      case 5:
         mGizmo.mRotateSnap = M_PI_F / 4.0f;
         break;

      case 6:
         mGizmo.mRotateSnap = M_PI_F / 2.0f;
         break;
   }
}

void TransformTool::OnScaleMenuEvent(wxCommandEvent& evt)
{
   switch (evt.GetId())
   {
      case 0:
         mGizmo.mScaleSnap = 0.0f;
         break;

      case 1:
         mGizmo.mScaleSnap = 0.1f;
         break;

      case 2:
         mGizmo.mScaleSnap = 0.5f;
         break;

      case 3:
         mGizmo.mScaleSnap = 1.0f;
         break;

      case 4:
         mGizmo.mScaleSnap = 5.0f;
         break;   
   }
}