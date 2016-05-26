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

#include "editorCamera.h"
#include "module/moduleManager.h"
#include "scene/components/meshComponent.h"
#include "rendering/renderCamera.h"

// ---------------------------------------------------------------

EditorCamera::EditorCamera()
{
   mRenderCamera     = NULL;
   mForwardVelocity  = Point3F::Zero;
   mWorldPosition    = Point3F(0.0f, 0.0f, 0.0f);
   mMouseDown        = false;
   mMouseStart       = Point2I(0, 0);
   mHorizontalAngle  = 0.0f;
   mVerticalAngle    = 0.0f;

   mTransform.set(Point3F(0.0f, 0.0f, 0.0f), VectorF(0.0f, 0.0f, 0.0f), VectorF(1.0f, 1.0f, 1.0f));
}

void EditorCamera::initialize(EditorManager* EditorManager)
{
   mEditorManager    = EditorManager;
   mRenderCamera     = Torque::Rendering.getPriorityRenderCamera();

   // Render Camera settings.
   mRenderCamera->farPlane = 1000.0f;
}

void EditorCamera::mainLoop()
{
   if (mRenderCamera == NULL)
      return;

   mVerticalAngle = mClampF(mVerticalAngle, -4.7f, -1.7f);
   VectorF rotation = mTransform.getRotationEuler();
   rotation.y = mVerticalAngle;
   rotation.z = mHorizontalAngle;
   mTransform.setRotation(rotation);

   VectorF up(0.0f, 0.0f, 1.0f);
   Point3F look;
   Point3F cameraForward(1.0f, 0.0f, 0.0f);

   bx::vec3MulMtx(look, cameraForward, mTransform.matrix);

   if (mForwardVelocity.len() > 0.01f)
   {
      MatrixF lookMatrix;
      bx::mtxLookAt(lookMatrix, mWorldPosition, look, up);

      F32 speedMod = mShiftKey ? 2.0f : 1.0f;
      mWorldPosition += (lookMatrix.getForwardVector() * mForwardVelocity.x * speedMod);
      mWorldPosition -= (lookMatrix.getRightVector() * mForwardVelocity.y * speedMod);

      mTransform.setPosition(mWorldPosition);
   }

   bx::vec3MulMtx(look, cameraForward, mTransform.matrix);
   bx::mtxLookAt(mRenderCamera->viewMatrix, mWorldPosition, look, up);
   mRenderCamera->position = mWorldPosition;
}

void EditorCamera::setForwardVelocity(Point3F velocity)
{
   mForwardVelocity = velocity;
}

bool EditorCamera::onMouseLeftDown(int x, int y)
{
   return false;
}

bool EditorCamera::onMouseLeftUp(int x, int y)
{
   return false;
}

bool EditorCamera::onMouseRightDown(int x, int y)
{
   mMouseDown = true;
   mMouseStart = Point2I(x, y);
   return false;
}

bool EditorCamera::onMouseRightUp(int x, int y)
{
   mMouseDown = false;
   return false;
}

bool EditorCamera::onMouseMove(int x, int y)
{
   if (!mMouseDown)
      return false;

   Point2I pos = Point2I(x, y);
   Point2I delta = mMouseStart - pos;
   mMouseStart = pos;

   if (delta.isZero()) return false;

   mHorizontalAngle += delta.x * 0.01f;
   mVerticalAngle -= delta.y * 0.01f;
   mVerticalAngle = mClampF(mVerticalAngle, -4.7f, -1.7f);
   return false;
}