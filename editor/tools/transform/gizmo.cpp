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

// UI generated from wxFormBuilder
#include "Torque6EditorUI.h"

#include "gizmo.h"
#include <bx/bx.h>
#include <bx/fpumath.h>
#include "bounds.h"

Box3F getObjectsBoundingBox(Vector<SimObject*> objects);
F32 getCircleDistance(Point2F screenPos, const float* _normal, const float* _center, float _radius, float _weight = 0.0f);

Point2F closestPointToLine(Point2F A, Point2F B, Point2F P)
{
   Point2F AP = P - A;  //Vector from A to P   
   Point2F AB = B - A;  //Vector from A to B  

   float magnitudeAB = AB.lenSquared();  //Magnitude of AB vector (it's length squared)     
   float ABAPproduct = mDot(AP, AB);               //The DOT product of a_to_p and a_to_b     
   float distance = ABAPproduct / magnitudeAB;     //The normalized "distance" from a to your closest point  

   if (distance < 0) //Check if P projection is over vectorAB     
   {
      return A;

   }
   else if (distance > 1) {
      return B;
   }
   else
   {
      return A + AB * distance;
   }
}

bool LinePlaneIntersection(Point3F& intersection, Point3F linePoint, Point3F lineVec, Point3F planeNormal, Point3F planePoint) {

   F32 length;
   F32 dotNumerator;
   F32 dotDenominator;
   Point3F vector;
   intersection = Point3F::Zero;

   //calculate the distance between the linePoint and the line-plane intersection point
   dotNumerator = mDot((planePoint - linePoint), planeNormal);
   dotDenominator = mDot(lineVec, planeNormal);

   //line and plane are not parallel
   if (dotDenominator != 0.0f) {
      length = dotNumerator / dotDenominator;

      //create a vector from the linePoint to the intersection point
      vector = lineVec;
      vector.normalize();
      vector *= length;

      //get the coordinates of the line-plane intersection point
      intersection = linePoint + vector;
      return true;
   }

   //output not valid
   else {
      return false;
   }
}

Point3F getPointOnXPlane(F32 x, Point3F p1, Point3F p2)
{
   F32 a = (x - p1.x) / (p2.x - p1.x);
   return Point3F(x, p1.y + a*(p2.y - p1.y), p1.z + a*(p2.z - p1.z));
}

Point3F getPointOnYPlane(F32 y, Point3F p1, Point3F p2)
{
   F32 a = (y - p1.y) / (p2.y - p1.y);
   return Point3F(p1.x + a*(p2.x - p1.x), y, p1.z + a*(p2.z - p1.z));
}

Point3F getPointOnZPlane(F32 z, Point3F p1, Point3F p2)
{
   F32 a = (z - p1.z) / (p2.z - p1.z);
   return Point3F(p1.x + a*(p2.x - p1.x), p1.y + a*(p2.y - p1.y), z);
}

Gizmo::Gizmo()
   : mEditorManager(NULL),
   mMode(0),
   mMousePosition(0.0f, 0.0f),
   mMultiselect(false),
   mSelectedObject(NULL),
   mSelectedComponent(NULL),
   mSelectRed(false),
   mSelectGreen(false),
   mSelectBlue(false),
   mSelectRedPoint(Point3F::Zero),
   mSelectGreenPoint(Point3F::Zero),
   mSelectBluePoint(Point3F::Zero),
   mDownPoint(Point3F::Zero),
   mDragging(false),
   mDragRed(false),
   mDragGreen(false),
   mDragBlue(false),
   mDownAngle(0.0f),
   mHovering(false),
   mTranslateSnap(0.0f),
   mTranslateLocal(true),
   mScaleSnap(0.0f),
   mRotateSnap(0.0f),
   mRotateLocal(true)
{
   //
}

Gizmo::~Gizmo()
{
   //
}

void Gizmo::selectObject(Scene::SceneObject* Object)
{
   mMultiselect         = false;
   mSelectedObject      = Object;
   mSelectedComponent   = NULL;
   mSelectedPosition    = Object->mTransform.getPosition();
   mSelectedRotation    = Object->mTransform.getRotationEuler();
   mSelectedScale       = Object->mTransform.getScale();
}

void Gizmo::selectObjects(Vector<SimObject*> objects)
{
   mMultiselect         = true;
   mSelectedObject      = NULL;
   mSelectedObjects     = objects;
   mSelectedComponent   = NULL;

   Box3F bounds      = getObjectsBoundingBox(mSelectedObjects);
   mSelectedPosition = bounds.getCenter();
}

void Gizmo::selectComponent(Scene::BaseComponent* component)
{
   mMultiselect         = false;
   mSelectedObject      = component->mOwnerObject;
   mSelectedComponent   = component;
   mSelectedPosition    = mSelectedObject->mTransform.getPosition() + component->mTransform.getPosition();
   mSelectedRotation    = component->mTransform.getRotationEuler();
   mSelectedScale       = component->mTransform.getScale();
}

void Gizmo::render()
{
   if (mEditorManager == NULL || !(mSelectedObject != NULL || mMultiselect))
      return;

   Point3F position = Point3F::Zero;
   Point3F rotation = Point3F::Zero;
   Transform transform;

   // Selected Object
   if (mSelectedObject != NULL && mSelectedComponent == NULL)
   {
      position    = mSelectedObject->mTransform.getPosition();
      rotation    = mSelectedObject->mTransform.getRotationEuler();
      transform   = mSelectedObject->mTransform;
   }

   // Multiple Selected Objected
   if (mMultiselect)
   {
      position = mSelectedPosition;
   }

   // Selected Component
   if (mSelectedObject != NULL && mSelectedComponent != NULL)
   {
      position    = mSelectedObject->mTransform.getPosition() + mSelectedComponent->mTransform.getPosition();
      rotation    = mSelectedComponent->mTransform.getRotationEuler();
      transform   = mSelectedObject->mTransform;
   }

   Point3F editorPos = mEditorManager->mEditorCamera.getWorldPosition();
   Point3F camToObject = position - editorPos;

   // Highlight selected axis
   ColorI redColor   = mSelectRed ? ColorI(255, 0, 0, 255) : ColorI(128, 0, 0, 255);
   ColorI greenColor = mSelectGreen ? ColorI(0, 255, 0, 255) : ColorI(0, 128, 0, 255);
   ColorI blueColor  = mSelectBlue ? ColorI(0, 0, 255, 255) : ColorI(0, 0, 128, 255);

   // Translate
   if (mMode == 0)
   {
      const F32 size = camToObject.len() / 5.0f;
      const F32 coneLength = size / 1.2f;
      const F32 coneRadius = size / 10.0f;

      Torque::Debug.ddPush();
      Torque::Debug.ddSetState(false, false, true);
      Torque::Debug.ddSetWireframe(false);

      Torque::Debug.ddSetColor(mSelectRed ? 0xff00ffff : 0xff0000ff);
      Torque::Debug.ddMoveTo(position.x, position.y, position.z);
      Torque::Debug.ddLineTo(position.x + size, position.y, position.z);
      Torque::Debug.ddMoveTo(position.x + (size / 5.0f), position.y, position.z);
      Torque::Debug.ddLineTo(position.x + (size / 5.0f), position.y, position.z + (size / 5.0f));
      Torque::Debug.ddMoveTo(position.x + (size / 5.0f), position.y, position.z);
      Torque::Debug.ddLineTo(position.x + (size / 5.0f), position.y + (size / 5.0f), position.z);
 
      Point3F xConeStart(position.x + coneLength, position.y, position.z);
      Point3F xConeEnd(position.x + size, position.y, position.z);
      Torque::Debug.ddDrawCone(xConeStart, xConeEnd, coneRadius, 0.0f);

      Torque::Debug.ddSetColor(mSelectGreen ? 0xff00ffff : 0xff00ff00);
      Torque::Debug.ddMoveTo(position.x, position.y, position.z);
      Torque::Debug.ddLineTo(position.x, position.y + size, position.z);
      Torque::Debug.ddMoveTo(position.x, position.y + (size / 5.0f), position.z);
      Torque::Debug.ddLineTo(position.x, position.y + (size / 5.0f), position.z + (size / 5.0f));
      Torque::Debug.ddMoveTo(position.x, position.y + (size / 5.0f), position.z);
      Torque::Debug.ddLineTo(position.x + (size / 5.0f), position.y + (size / 5.0f), position.z);

      Point3F yConeStart(position.x, position.y + coneLength, position.z);
      Point3F yConeEnd(position.x, position.y + size, position.z);
      Torque::Debug.ddDrawCone(yConeStart, yConeEnd, coneRadius, 0.0f);

      Torque::Debug.ddSetColor(mSelectBlue ? 0xff00ffff : 0xffff0000);
      Torque::Debug.ddMoveTo(position.x, position.y, position.z);
      Torque::Debug.ddLineTo(position.x, position.y, position.z + size);
      Torque::Debug.ddMoveTo(position.x, position.y, position.z + (size / 5.0f));
      Torque::Debug.ddLineTo(position.x, position.y + (size / 5.0f), position.z + (size / 5.0f));
      Torque::Debug.ddMoveTo(position.x, position.y, position.z + (size / 5.0f));
      Torque::Debug.ddLineTo(position.x + (size / 5.0f), position.y, position.z + (size / 5.0f));

      Point3F zConeStart(position.x, position.y, position.z + coneLength);
      Point3F zConeEnd(position.x, position.y, position.z + size);
      Torque::Debug.ddDrawCone(zConeStart, zConeEnd, coneRadius, 0.0f);

      Sphere panSphere;
      panSphere.m_center[0] = position.x;
      panSphere.m_center[1] = position.y;
      panSphere.m_center[2] = position.z;
      panSphere.m_radius = size / 20.0f;
      Torque::Debug.ddSetState(false, false, false);
      Torque::Debug.ddSetColor(0xffffffff);
      Torque::Debug.ddDrawSphere(panSphere);

      //Torque::Debug.ddSetState(true, false, true);
      Torque::Debug.ddPop();
   }

   // Rotate
   if (mMode == 1)
   {
      F32 size = (camToObject.len() / 4.0f);
      Torque::Debug.ddPush();
      Torque::Debug.ddSetState(false, false, true);
      
      F32 xNormal[3] = { 1.0f, 0.0f, 0.0f };
      F32 yNormal[3] = { 0.0f, 1.0f, 0.0f };
      F32 zNormal[3] = { 0.0f, 0.0f, 1.0f };

      if (mRotateLocal)
      {
         transform.setPosition(Point3F(0.0f, 0.0f, 0.0f));
         transform.setScale(VectorF(1.0f, 1.0f, 1.0f));

         bx::vec3MulMtx(xNormal, xNormal, transform.matrix);
         bx::vec3MulMtx(yNormal, yNormal, transform.matrix);
         bx::vec3MulMtx(zNormal, zNormal, transform.matrix);
      }

      Torque::Debug.ddSetColor(mSelectRed ? 0xff00ffff : 0xff0000ff);
      Torque::Debug.ddDrawCircle(xNormal, position, size / 2.0f, 0.0f);
      Torque::Debug.ddSetColor(mSelectGreen ? 0xff00ffff : 0xff00ff00);
      Torque::Debug.ddDrawCircle(yNormal, position, size / 2.0f, 0.0f);
      Torque::Debug.ddSetColor(mSelectBlue ? 0xff00ffff : 0xffff0000);
      Torque::Debug.ddDrawCircle(zNormal, position, size / 2.0f, 0.0f);

      Torque::Debug.ddPop();
   }

   // Scale
   if (mMode == 2)
   {
      F32 size = camToObject.len() / 4.0f;
      Torque::Debug.ddPush();
      Torque::Debug.ddSetState(false, false, true);

      Torque::Debug.ddSetColor(mSelectRed ? 0xff00ffff : 0xff0000ff);
      Torque::Debug.ddMoveTo(position.x, position.y, position.z);
      Torque::Debug.ddLineTo(position.x + size, position.y, position.z);

      Torque::Debug.ddSetColor(mSelectGreen ? 0xff00ffff : 0xff00ff00);
      Torque::Debug.ddMoveTo(position.x, position.y, position.z);
      Torque::Debug.ddLineTo(position.x, position.y + size, position.z);

      Torque::Debug.ddSetColor(mSelectBlue ? 0xff00ffff : 0xffff0000);
      Torque::Debug.ddMoveTo(position.x, position.y, position.z);
      Torque::Debug.ddLineTo(position.x, position.y, position.z + size);

      //Torque::Debug.ddSetState(true, false, true);
      Torque::Debug.ddPop();
   }

}

bool Gizmo::onMouseLeftDown(int x, int y)
{
   mDragRed    = false;
   mDragGreen  = false;
   mDragBlue   = false;
   mDragging   = false;

   Point3F position = Point3F::Zero;
   Point3F rayBegin;
   Point3F rayEnd;
   Torque::Rendering.screenToWorld(Point2I(x, y), rayBegin, rayEnd);
   Point3F editorPos = mEditorManager->mEditorCamera.getWorldPosition();

   // Selected Object
   if (mSelectedObject != NULL && mSelectedComponent == NULL)
      position = mSelectedObject->mTransform.getPosition();

   // Multiselect
   if ( mMultiselect )
   {
      Box3F bounds = getObjectsBoundingBox(mSelectedObjects);
      position = bounds.getCenter();
   }

   // Selected Component
   if (mSelectedObject != NULL && mSelectedComponent != NULL)
      position = mSelectedComponent->mTransform.getPosition();

   if (mSelectRed)
   {
      mDownPoint     = mSelectRedPoint;
      Point3F xPoint = getPointOnXPlane(position.x, rayBegin, rayEnd);
      mDownAngle     = mAtan(xPoint.z, xPoint.y);
      mDragRed       = true;
      mDragging      = true;
      return true;
   }

   if (mSelectGreen)
   {
      mDownPoint     = mSelectGreenPoint;
      Point3F yPoint = getPointOnYPlane(position.y, rayBegin, rayEnd);
      mDownAngle     = mAtan(yPoint.z, yPoint.x);
      mDragGreen     = true;
      mDragging      = true;
      return true;
   }

   if (mSelectBlue)
   {
      mDownPoint     = mSelectBluePoint;
      Point3F zPoint = getPointOnZPlane(position.z, rayBegin, rayEnd);
      mDownAngle     = mAtan(zPoint.y, zPoint.x);
      mDragBlue      = true;
      mDragging      = true;
      return true;
   }

   return false;
}

bool Gizmo::onMouseLeftUp(int x, int y)
{
   mDragging   = false;
   mDragRed    = false;
   mDragGreen  = false;
   mDragBlue   = false;

   return (mSelectRed || mSelectGreen || mSelectBlue);
}


bool Gizmo::onMouseMove(int x, int y)
{
   if (mSelectedObject == NULL && !mMultiselect)
      return false;

   mMousePosition = Point2F(x, y);
   Point3F position = Point3F::Zero;
   Transform transform;

   // Selected Object
   if (mSelectedObject != NULL && mSelectedComponent == NULL)
   {
      position    = mSelectedObject->mTransform.getPosition();
      transform   = mSelectedObject->mTransform;
   }

   // Multiselect
   if (mMultiselect)
   {
      Box3F bounds = getObjectsBoundingBox(mSelectedObjects);
      position = bounds.getCenter();
   }

   // Selected Component
   if (mSelectedObject != NULL && mSelectedComponent != NULL)
   {
      position    = mSelectedObject->mTransform.getPosition() + mSelectedComponent->mTransform.getPosition();
      transform   = mSelectedObject->mTransform;
   }

   // Determine the worldspace points we're closest to.
   Point3F dummyPoint;
   Point3F rayBegin;
   Point3F rayEnd;
   Torque::Rendering.screenToWorld(Point2I(x, y), rayBegin, rayEnd);
   Point3F editorPos = mEditorManager->mEditorCamera.getWorldPosition();
   Torque::Rendering.closestPointsOnTwoLines(mSelectRedPoint, dummyPoint, position, Point3F(1000.0f, 0.0f, 0.0f), rayBegin, rayEnd);
   Torque::Rendering.closestPointsOnTwoLines(mSelectGreenPoint, dummyPoint, position, Point3F(0.0, 1000.0f, 0.0f), rayBegin, rayEnd);
   Torque::Rendering.closestPointsOnTwoLines(mSelectBluePoint, dummyPoint, position, Point3F(0.0f, 0.0f, 1000.0f), rayBegin, rayEnd);

   // Dragging
   if (mDragging)
   {
      // Translate
      if (mMode == 0)
         dragTranslate(x, y);

      // Rotate
      if (mMode == 1)
         dragRotate(x, y);

      // Scale
      if (mMode == 2)
         dragScale(x, y);

      return true;
   }

   // Reset Mouse Over
   mSelectRed     = false;
   mSelectGreen   = false;
   mSelectBlue    = false;

   // Determine if the mouse is close enough to be within the gizmo.
   Point3F camToObject = position - editorPos;
   F32 size = camToObject.len() / 4.0f;
   F32 dist = getMax((position - mSelectRedPoint).len(), (position - mSelectGreenPoint).len());
   dist = getMax(dist, (position - mSelectBluePoint).len());
   if (dist > size)
      return false;

   // Translate
   if (mMode == 0)
   {
      Point2I redPointScreen     = Torque::Rendering.worldToScreen(mSelectRedPoint);
      Point2I greenPointScreen   = Torque::Rendering.worldToScreen(mSelectGreenPoint);
      Point2I bluePointScreen    = Torque::Rendering.worldToScreen(mSelectBluePoint);

      F32 redDist    = Point2F(x - redPointScreen.x, y - redPointScreen.y).len();
      F32 greenDist  = Point2F(x - greenPointScreen.x, y - greenPointScreen.y).len();
      F32 blueDist   = Point2F(x - bluePointScreen.x, y - bluePointScreen.y).len();

      mSelectRed     = (redDist < greenDist && redDist < blueDist);
      mSelectGreen   = (greenDist < redDist && greenDist < blueDist);
      mSelectBlue    = (blueDist < greenDist && blueDist < redDist);
      return true;
   }

   // Rotate
   if (mMode == 1)
   {
      F32 xNormal[3] = { 1.0f, 0.0f, 0.0f };
      F32 yNormal[3] = { 0.0f, 1.0f, 0.0f };
      F32 zNormal[3] = { 0.0f, 0.0f, 1.0f };

      if (mRotateLocal)
      {
         transform.setPosition(Point3F(0.0f, 0.0f, 0.0f));
         transform.setScale(VectorF(1.0f, 1.0f, 1.0f));

         bx::vec3MulMtx(xNormal, xNormal, transform.matrix);
         bx::vec3MulMtx(yNormal, yNormal, transform.matrix);
         bx::vec3MulMtx(zNormal, zNormal, transform.matrix);
      }

      F32 xDist = getCircleDistance(mMousePosition, xNormal, position, size / 2.0f, 0.0f);
      F32 yDist = getCircleDistance(mMousePosition, yNormal, position, size / 2.0f, 0.0f);
      F32 zDist = getCircleDistance(mMousePosition, zNormal, position, size / 2.0f, 0.0f);

      mSelectRed     = (xDist < yDist && xDist < zDist);
      mSelectGreen   = (yDist < xDist && yDist < zDist);
      mSelectBlue    = (zDist < xDist && zDist < yDist);
      return true;
   }

   // Scale
   if (mMode == 2)
   {
      Point2I redPointScreen     = Torque::Rendering.worldToScreen(mSelectRedPoint);
      Point2I greenPointScreen   = Torque::Rendering.worldToScreen(mSelectGreenPoint);
      Point2I bluePointScreen    = Torque::Rendering.worldToScreen(mSelectBluePoint);

      F32 redDist    = Point2F(x - redPointScreen.x, y - redPointScreen.y).len();
      F32 greenDist  = Point2F(x - greenPointScreen.x, y - greenPointScreen.y).len();
      F32 blueDist   = Point2F(x - bluePointScreen.x, y - bluePointScreen.y).len();

      mSelectRed     = (redDist < greenDist && redDist < blueDist);
      mSelectGreen   = (greenDist < redDist && greenDist < blueDist);
      mSelectBlue    = (blueDist < greenDist && blueDist < redDist);
      return true;
   }

   return false;
}

Point3F snap(const Point3F& input, F32 snapTo)
{
   Point3F snapped = input;
   snapped.x = mFloor(snapped.x / snapTo) * snapTo;
   snapped.y = mFloor(snapped.y / snapTo) * snapTo;
   snapped.z = mFloor(snapped.z / snapTo) * snapTo;
   return snapped;
}

void Gizmo::dragTranslate(int x, int y)
{
   Point3F dragVector = Point3F::Zero;
   F32 dragAngle = 0.0f;

   if (mDragRed)
      dragVector = mSelectRedPoint - mDownPoint;

   if (mDragGreen)
      dragVector = mSelectGreenPoint - mDownPoint;

   if (mDragBlue)
      dragVector = mSelectBluePoint - mDownPoint;

   // Move mode.
   mDownPoint += dragVector;
   mSelectedPosition += dragVector;

   Point3F newPosition = mSelectedPosition;

   // Snapping
   if (mTranslateSnap > 0.0f)
      newPosition = snap(mSelectedPosition, mTranslateSnap);

   // Selected Object
   if (mSelectedObject != NULL && mSelectedComponent == NULL)
   {
      mSelectedObject->mTransform.setPosition(newPosition);
      mSelectedObject->refresh();
   }

   if (mMultiselect)
   {
      for (S32 n = 0; n < mSelectedObjects.size(); ++n)
      {
         Scene::SceneObject* obj = dynamic_cast<Scene::SceneObject*>(mSelectedObjects[n]);
         if (obj)
         {
            Point3F pos = obj->mTransform.getPosition();
            pos += dragVector;
            obj->mTransform.setPosition(pos);
            obj->refresh();
         }

         Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(mSelectedObjects[n]);
         if (component)
         {
            Point3F pos = component->mTransform.getPosition();
            pos += dragVector;
            component->mTransform.setPosition(pos);
            component->mOwnerObject->refresh();
         }
      }
   }

   // Selected Component
   if (mSelectedObject != NULL && mSelectedComponent != NULL)
   {
      mSelectedComponent->mTransform.setPosition(newPosition - mSelectedObject->mTransform.getPosition());
      mSelectedObject->refresh();
   }
}

void Gizmo::dragRotate(int x, int y)
{
   F32 dragAngle = 0.0f;
   Point3F rayBegin;
   Point3F rayEnd;
   Torque::Rendering.screenToWorld(Point2I(x, y), rayBegin, rayEnd);
   Point3F editorPos = mEditorManager->mEditorCamera.getWorldPosition();

   Point3F position = mSelectedPosition;
   rayBegin -= position;
   rayEnd -= position;

   Point3F delta;
   if (mDragRed)
   {
      Point3F xPoint = getPointOnXPlane(0, rayBegin, rayEnd);
      dragAngle = mAtan(xPoint.z, xPoint.y) - mDownAngle;
      mSelectedRotation.x += dragAngle;
      delta.x = dragAngle;
      delta.y = 0.0f;
      delta.z = 0.0f;
   }

   if (mDragGreen)
   {
      Point3F yPoint = getPointOnYPlane(0, rayBegin, rayEnd);
      dragAngle = mAtan(yPoint.z, yPoint.x) - mDownAngle;
      mSelectedRotation.y += dragAngle;
      delta.x = 0.0f;
      delta.y = dragAngle;
      delta.z = 0.0f;
   }

   if (mDragBlue)
   {
      Point3F zPoint = getPointOnZPlane(0, rayBegin, rayEnd);
      dragAngle = mAtan(zPoint.y, zPoint.x) - mDownAngle;
      mSelectedRotation.z += dragAngle;
      delta.x = 0.0f;
      delta.y = 0.0f;
      delta.z = dragAngle;
   }

   Point3F newRotation = mSelectedRotation;

   // Snapping
   if (mRotateSnap > 0.0f)
      newRotation = snap(mSelectedRotation, mRotateSnap);

   // Selected Object
   if (mSelectedObject != NULL && mSelectedComponent == NULL)
   {
      mSelectedObject->mTransform.setRotation(newRotation);
      mSelectedObject->refresh();
   }

   // Selected Objects
   if (mMultiselect)
   {
      F32 rotateMtx[16];
      bx::mtxSRT(rotateMtx, 1.0f, 1.0f, 1.0f, delta.x, delta.y, delta.z, 0.0f, 0.0f, 0.0f);

      for (S32 n = 0; n < mSelectedObjects.size(); ++n)
      {
         Scene::SceneObject* obj = dynamic_cast<Scene::SceneObject*>(mSelectedObjects[n]);
         if (obj)
         {
            Point3F pos = obj->mTransform.getPosition() - mSelectedPosition;
            Point3F newPos;
            bx::vec3MulMtx(newPos, pos, rotateMtx);
            obj->mTransform.setPosition(newPos + mSelectedPosition);

            Point3F rot = obj->mTransform.getRotationEuler();
            rot += delta;
            obj->mTransform.setRotation(rot);

            obj->refresh();
         }

         Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(mSelectedObjects[n]);
         if (component)
         {
            Point3F pos = component->mTransform.getPosition() - mSelectedPosition;
            Point3F newPos;
            bx::vec3MulMtx(newPos, pos, rotateMtx);
            component->mTransform.setPosition(newPos + mSelectedPosition);

            Point3F rot = component->mTransform.getRotationEuler();
            rot += delta;
            component->mTransform.setRotation(rot);

            component->mOwnerObject->refresh();
         }
      }
   }

   // Selected Component
   if (mSelectedObject != NULL && mSelectedComponent != NULL)
   {
      mSelectedComponent->mTransform.setRotation(newRotation);
      mSelectedObject->refresh();
   }

   mDownAngle += dragAngle;
}

void Gizmo::dragScale(int x, int y)
{
   Point3F dragVector = Point3F::Zero;
   F32 dragAngle = 0.0f;

   if (mDragRed)
      dragVector = mSelectRedPoint - mDownPoint;

   if (mDragGreen)
      dragVector = mSelectGreenPoint - mDownPoint;

   if (mDragBlue)
      dragVector = mSelectBluePoint - mDownPoint;

   mDownPoint += dragVector;
   mSelectedScale += dragVector;

   Point3F newScale = mSelectedScale;

   // Snapping
   if (mScaleSnap > 0.0f)
      mSelectedObject->mTransform.setScale(snap(mSelectedScale, mScaleSnap));

   // Selected Object
   if (mSelectedObject != NULL && mSelectedComponent == NULL)
   {
      mSelectedObject->mTransform.setScale(newScale);
      mSelectedObject->refresh();
   }

   // Selected Objects
   if (mMultiselect)
   {
      F32 rotateMtx[16];
      bx::mtxSRT(rotateMtx, 1.0f + dragVector.x, 1.0f + dragVector.y, 1.0f + dragVector.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

      for (S32 n = 0; n < mSelectedObjects.size(); ++n)
      {
         Scene::SceneObject* obj = dynamic_cast<Scene::SceneObject*>(mSelectedObjects[n]);
         if (obj)
         {
            Point3F pos = obj->mTransform.getPosition() - mSelectedPosition;
            Point3F newPos;
            bx::vec3MulMtx(newPos, pos, rotateMtx);
            obj->mTransform.setPosition(newPos + mSelectedPosition);

            Point3F scale = obj->mTransform.getScale();
            scale += dragVector;
            obj->mTransform.setScale(scale);

            obj->refresh();
         }

         Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(mSelectedObjects[n]);
         if (component)
         {
            Point3F pos = component->mTransform.getPosition() - mSelectedPosition;
            Point3F newPos;
            bx::vec3MulMtx(newPos, pos, rotateMtx);
            component->mTransform.setPosition(newPos + mSelectedPosition);

            Point3F scale = component->mTransform.getScale();
            scale += dragVector;
            component->mTransform.setScale(scale);

            component->mOwnerObject->refresh();
         }
      }
   }

   // Selected Component
   if (mSelectedObject != NULL && mSelectedComponent != NULL)
   {
      mSelectedComponent->mTransform.setScale(newScale);
      mSelectedObject->refresh();
   }
}

static void circle(float* _out, float _angle)
{
   float sa = bx::fsin(_angle);
   float ca = bx::fcos(_angle);
   _out[0] = sa;
   _out[1] = ca;
}

static void squircle(float* _out, float _angle)
{
   float sa = bx::fsin(_angle);
   float ca = bx::fcos(_angle);
   _out[0] = bx::fsqrt(bx::fabsolute(sa)) * bx::fsign(sa);
   _out[1] = bx::fsqrt(bx::fabsolute(ca)) * bx::fsign(ca);
}

void _calcPlaneUv(const Plane& _plane, float* _udir, float* _vdir)
{
   const uint8_t axis =
      bx::fabsolute(_plane.m_normal[0]) > 0.6f ? 0
      : (bx::fabsolute(_plane.m_normal[1]) > 0.6f ? 1
         : 2
         );
   const uint8_t* index = (uint8_t*)&"\x1\x2\x0\x2\x0\x1"[axis * 2];
   const uint8_t idx0 = *(index);
   const uint8_t idx1 = *(index + 1);

   _udir[0] = 0.0f;
   _udir[1] = 0.0f;
   _udir[2] = 0.0f;
   _udir[idx0] = 1.0f;

   _vdir[0] = 0.0f;
   _vdir[1] = 0.0f;
   _vdir[2] = 0.0f;
   _vdir[idx1] = 1.0f;

   const float invPlaneAxis = 1.0f / _plane.m_normal[axis];

   _udir[axis] -= bx::vec3Dot(_udir, _plane.m_normal) * invPlaneAxis;
   bx::vec3Norm(_udir, _udir);

   _vdir[axis] -= bx::vec3Dot(_vdir, _plane.m_normal) * invPlaneAxis;
   bx::vec3Norm(_vdir, _vdir);
}

F32 getCircleDistance(Point2F screenPos, const float* _normal, const float* _center, float _radius, float _weight)
{
   const uint32_t num = 37;
   const float step = bx::pi * 2.0f / num;
   _weight = bx::fclamp(_weight, 0.0f, 2.0f);

   Plane plane = { { _normal[0], _normal[1], _normal[2] }, 0.0f };
   float udir[3];
   float vdir[3];
   _calcPlaneUv(plane, udir, vdir);

   float pos[3];
   float tmp0[3];
   float tmp1[3];

   float xy0[2];
   float xy1[2];
   circle(xy0, 0.0f);
   squircle(xy1, 0.0f);

   bx::vec3Mul(pos, udir, bx::flerp(xy0[0], xy1[0], _weight)*_radius);
   bx::vec3Mul(tmp0, vdir, bx::flerp(xy0[1], xy1[1], _weight)*_radius);
   bx::vec3Add(tmp1, pos, tmp0);
   bx::vec3Add(pos, tmp1, _center);

   Point2I startPoint;
   Point2I endPoint = Torque::Rendering.worldToScreen(Point3F(pos[0], pos[1], pos[2]));

   F32 distanceToCircle = F32_MAX;

   for (uint32_t ii = 1; ii < num; ++ii)
   {
      startPoint = endPoint;

      float angle = step * ii;
      circle(xy0, angle);
      squircle(xy1, angle);

      bx::vec3Mul(pos, udir, bx::flerp(xy0[0], xy1[0], _weight)*_radius);
      bx::vec3Mul(tmp0, vdir, bx::flerp(xy0[1], xy1[1], _weight)*_radius);
      bx::vec3Add(tmp1, pos, tmp0);
      bx::vec3Add(pos, tmp1, _center);
      endPoint = Torque::Rendering.worldToScreen(Point3F(pos[0], pos[1], pos[2]));

      Point2F pt = closestPointToLine(Point2F(startPoint.x, startPoint.y), Point2F(endPoint.x, endPoint.y), screenPos);
      Point2F distVec = pt - screenPos;
      F32 dist = distVec.len();
      if (dist < distanceToCircle)
         distanceToCircle = dist;
   }

   return distanceToCircle;
}

Box3F getObjectsBoundingBox(Vector<SimObject*> objects)
{
   Box3F multiSelectBox;
   multiSelectBox.set(Point3F(0, 0, 0));

   for (S32 n = 0; n < objects.size(); ++n)
   {
      Scene::SceneObject* obj = dynamic_cast<Scene::SceneObject*>(objects[n]);
      if (obj)
      {
         if (n == 0)
            multiSelectBox = obj->mBoundingBox;
         else
            multiSelectBox.intersect(obj->mBoundingBox);
      }

      Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(objects[n]);
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

   return multiSelectBox;
}