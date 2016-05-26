//-----------------------------------------------------------------------------
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

#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H
 
#ifndef _PLUGINS_SHARED_H
#include <plugins/plugins_shared.h>
#endif

#ifndef _RENDER_CAMERA_H_
#include "rendering/renderCamera.h"
#endif

#ifndef _CAMERA_COMPONENT_H_
#include "scene/components/cameraComponent.h"
#endif

class EditorManager;

// Editor Camera
class EditorCamera
{
   protected:
      EditorManager*             mEditorManager;
      Rendering::RenderCamera*   mRenderCamera;
      Transform                  mTransform;
      Point3F                    mWorldPosition;
      Point3F                    mForwardVelocity;

      bool                       mShiftKey;
      bool                       mMouseDown;
      Point2I                    mMouseStart;
      F32                        mHorizontalAngle;
      F32                        mVerticalAngle;

   public:
      EditorCamera();

      void initialize(EditorManager* EditorManager);
      void mainLoop();
      Point3F getForwardVelocity() { return mForwardVelocity; }
      void setForwardVelocity(Point3F velocity);

      Rendering::RenderCamera* getRenderCamera() { return mRenderCamera; }
      Point3F getWorldPosition() { return mWorldPosition; }

      void setShiftKey(bool val) { mShiftKey = val; }
      bool onMouseLeftDown(int x, int y);
      bool onMouseLeftUp(int x, int y);
      bool onMouseRightDown(int x, int y);
      bool onMouseRightUp(int x, int y);
      bool onMouseMove(int x, int y);
};
 
#endif // EDITORCAMERA_H