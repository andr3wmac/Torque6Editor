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

#ifndef _GIZMO_H_
#define _GIZMO_H_

#ifndef EDITORMANAGER_H
#include "editorManager.h"
#endif

#ifndef _SCENE_OBJECT_H_
#include <scene/object.h>
#endif

class Gizmo
{
   protected:
      bool     mHovering;

      bool     mSelectRed;
      bool     mSelectGreen;
      bool     mSelectBlue;
      Point3F  mSelectRedPoint;
      Point3F  mSelectGreenPoint;
      Point3F  mSelectBluePoint;

      bool     mDragging;
      bool     mDragRed;
      bool     mDragBlue;
      bool     mDragGreen;
      Point3F  mDownPoint;
      F32      mDownAngle;

      Point3F  mSelectedPosition;
      Point3F  mSelectedRotation;
      Point3F  mSelectedScale;

   public:
      EditorManager*         mEditorManager;
      Scene::SceneObject*     mSelectedObject;
      Scene::BaseComponent*   mSelectedComponent;

      F32 mTranslateSnap;
      F32 mScaleSnap;
      F32 mRotateSnap;

      Gizmo();
      ~Gizmo();

      void selectObject(Scene::SceneObject* obj);
      void selectComponent(Scene::BaseComponent* component);
      void render();

      bool onMouseLeftDown(int x, int y);
      bool onMouseLeftUp(int x, int y);
      bool onMouseMove(int x, int y);

      void dragTranslate(int x, int y);
      void dragRotate(int x, int y);
      void dragScale(int x, int y);
};

#endif // _SCENE_TOOL_H_
