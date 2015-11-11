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
#include "../Torque6EditorUI.h"

#include "gizmo.h"
#include "3d/scene/camera.h"
#include <bx/bx.h>
#include <bx/fpumath.h>

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
   :  mProjectManager(NULL),
      mSelectedObject(NULL),
      mSelectedEntity(NULL),
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
      mScaleSnap(0.0f),
      mRotateSnap(0.0f)
{
   //
}

Gizmo::~Gizmo()
{
   //
}

void Gizmo::selectEntity(Scene::SceneEntity* entity)
{
   mSelectedObject      = entity;
   mSelectedEntity      = entity;
   mSelectedComponent   = NULL;
   mSelectedPosition    = entity->mPosition;
   mSelectedRotation    = entity->mRotation;
   mSelectedScale       = entity->mScale;
}

void Gizmo::selectComponent(Scene::BaseComponent* component)
{
   mSelectedObject      = component;
   mSelectedEntity      = component->mOwnerEntity;
   mSelectedComponent   = component;
   mSelectedPosition    = component->getPosition();
   mSelectedRotation    = component->getRotation();
   mSelectedScale       = component->getScale();
}

void Gizmo::render()
{
   if (mProjectManager == NULL || mSelectedEntity == NULL)
      return;

   Point3F position = Point3F::Zero;
   Point3F rotation = Point3F::Zero;

   // Selected Entity
   if (mSelectedEntity != NULL && mSelectedComponent == NULL)
   {
      position = mSelectedEntity->mPosition;
      rotation = mSelectedEntity->mRotation;
   }

   // Selected Component
   if (mSelectedEntity != NULL && mSelectedComponent != NULL)
   {
      position = mSelectedComponent->getPosition();
      rotation = mSelectedComponent->getRotation();
   }

   Point3F editorPos = Plugins::Link.Scene.getActiveCamera()->getPosition();
   Point3F camToEntity = mSelectedEntity->mPosition - editorPos;

   // Highlight selected axis
   ColorI redColor   = mSelectRed   ? ColorI(255, 0, 0, 255) : ColorI(128, 0, 0, 255);
   ColorI greenColor = mSelectGreen ? ColorI(0, 255, 0, 255) : ColorI(0, 128, 0, 255);
   ColorI blueColor  = mSelectBlue  ? ColorI(0, 0, 255, 255) : ColorI(0, 0, 128, 255);

   // Translate
   if (mProjectManager->mEditorMode == 0)
   {
      F32 size = camToEntity.len() / 4.0f;
      F32 length = size / 10.0f;

      Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, position, position + Point3F(size, 0.0f, 0.0f), redColor, NULL);
      Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, position, position + Point3F(0.0f, size, 0.0f), greenColor, NULL);
      Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, position, position + Point3F(0.0f, 0.0f, size), blueColor, NULL);

      // X Axis
      F32 xTransform[16];
      bx::mtxSRT(xTransform, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.57f, position.x + size, position.y, position.z);
      Plugins::Link.Graphics.drawCone3D(mProjectManager->mEditorOverlayView->id, Point3F(0.0, 0.0, 0.0), length, length / 2.0, 24, ColorI(128, 0, 0, 255), redColor, xTransform);

      // Y Axis
      F32 yTransform[16];
      bx::mtxSRT(yTransform, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, position.x, position.y + size, position.z);
      Plugins::Link.Graphics.drawCone3D(mProjectManager->mEditorOverlayView->id, Point3F(0.0, 0.0, 0.0), length, length / 2.0, 24, ColorI(0, 128, 0, 255), greenColor, yTransform);

      // Z Axis
      F32 zTransform[16];
      bx::mtxSRT(zTransform, 1.0f, 1.0f, 1.0f, -1.57f, 0.0f, 0.0f, position.x, position.y, position.z + size);
      Plugins::Link.Graphics.drawCone3D(mProjectManager->mEditorOverlayView->id, Point3F(0.0, 0.0, 0.0), length, length / 2.0, 24, ColorI(0, 0, 128, 255), blueColor, zTransform);
   }

   // Rotate
   if (mProjectManager->mEditorMode == 1)
   {
      F32 size = camToEntity.len() / 6.0f;

      // X Axis
      F32 xTransform[16];
      bx::mtxSRT(xTransform, 1.0f, 1.0f, 1.0f, 0.0f, 1.57f, 0.0f, position.x, position.y, position.z);
      Plugins::Link.Graphics.drawCircle3D(mProjectManager->mEditorOverlayView->id, Point3F(0.0, 0.0, 0.0), size, 24, redColor, xTransform);

      // Y Axis
      F32 yTransform[16];
      bx::mtxSRT(yTransform, 1.0f, 1.0f, 1.0f, 1.57f, 0.0f, 0.0f, position.x, position.y, position.z);
      Plugins::Link.Graphics.drawCircle3D(mProjectManager->mEditorOverlayView->id, Point3F(0.0, 0.0, 0.0), size, 24, greenColor, yTransform);

      // Z Axis
      F32 zTransform[16];
      bx::mtxSRT(zTransform, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, position.x, position.y, position.z);
      Plugins::Link.Graphics.drawCircle3D(mProjectManager->mEditorOverlayView->id, Point3F(0.0, 0.0, 0.0), size, 24, blueColor, zTransform);
   }

   // Scale
   if (mProjectManager->mEditorMode == 2)
   {
      F32 transform[16];
      bx::mtxSRT(transform,
         1.0, 1.0, 1.0,
         rotation.x, rotation.y, rotation.z,
         position.x, position.y, position.z);

      F32 size = camToEntity.len() / 4.0f;
      Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, Point3F(0.0, 0.0, 0.0), Point3F(size, 0.0f, 0.0f), redColor, transform);
      Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, Point3F(0.0, 0.0, 0.0), Point3F(0, size, 0.0f), greenColor, transform);
      Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, Point3F(0.0, 0.0, 0.0), Point3F(0.0f, 0.0f, size), blueColor, transform);
   }
}

bool Gizmo::onMouseLeftDown(int x, int y)
{
   mDragRed = false;
   mDragGreen = false;
   mDragBlue = false;
   mDragging = true;

   Point3F position = Point3F::Zero;
   Point3F worldRay = Plugins::Link.Rendering.screenToWorld(Point2I(x, y));
   Point3F editorPos = Plugins::Link.Scene.getActiveCamera()->getPosition();

   if (mSelectedEntity != NULL && mSelectedComponent == NULL)
      position = mSelectedEntity->mPosition;

   // Selected Component
   if (mSelectedEntity != NULL && mSelectedComponent != NULL)
      position = mSelectedComponent->getPosition();

   if (mSelectRed)
   {
      mDownPoint = mSelectRedPoint;
      Point3F xPoint = getPointOnXPlane(position.x, editorPos, editorPos + (worldRay * 1000.0f));
      mDownAngle = mAtan(xPoint.z, xPoint.y);
      mDragRed = true;
      return true;
   }

   if (mSelectGreen)
   {
      mDownPoint = mSelectGreenPoint;
      Point3F yPoint = getPointOnYPlane(position.y, editorPos, editorPos + (worldRay * 1000.0f));
      mDownAngle = mAtan(yPoint.z, yPoint.x);
      mDragGreen = true;
      return true;
   }

   if (mSelectBlue)
   {
      mDownPoint = mSelectBluePoint;
      Point3F zPoint = getPointOnZPlane(position.z, editorPos, editorPos + (worldRay * 1000.0f));
      mDownAngle = mAtan(zPoint.y, zPoint.x);
      mDragBlue = true;
      return true;
   }

   return false;
}

bool Gizmo::onMouseLeftUp(int x, int y)
{
   mDragging = false;
   mDragRed = false;
   mDragGreen = false;
   mDragBlue = false;

   return false;
}


bool Gizmo::onMouseMove(int x, int y)
{
   if (mSelectedEntity == NULL)
      return false;

   Point3F position = Point3F::Zero;

   // Selected Entity
   if (mSelectedEntity != NULL && mSelectedComponent == NULL)
      position = mSelectedEntity->mPosition;

   // Selected Component
   if (mSelectedEntity != NULL && mSelectedComponent != NULL)
      position = mSelectedComponent->getPosition();

   // Determine the worldspace points we're closest to.
   Point3F dummyPoint;
   Point3F worldRay = Plugins::Link.Rendering.screenToWorld(Point2I(x, y));
   Point3F editorPos = Plugins::Link.Scene.getActiveCamera()->getPosition();
   Plugins::Link.Rendering.closestPointsOnTwoLines(mSelectRedPoint, dummyPoint, position, Point3F(1000.0f, 0.0f, 0.0f), editorPos, worldRay * 1000.0f);
   Plugins::Link.Rendering.closestPointsOnTwoLines(mSelectGreenPoint, dummyPoint, position, Point3F(0.0, 1000.0f, 0.0f), editorPos, worldRay * 1000.0f);
   Plugins::Link.Rendering.closestPointsOnTwoLines(mSelectBluePoint, dummyPoint, position, Point3F(0.0f, 0.0f, 1000.0f), editorPos, worldRay * 1000.0f);

   // Dragging
   if (mDragging)
   {
      // Translate
      if (mProjectManager->mEditorMode == 0)
         dragTranslate(x, y);

      // Rotate
      if (mProjectManager->mEditorMode == 1)
         dragRotate(x, y);

      // Scale
      if (mProjectManager->mEditorMode == 2)
         dragScale(x, y);

      return true;
   }

   // Reset Mouse Over
   mSelectRed = false;
   mSelectGreen = false;
   mSelectBlue = false;

   // Determine if the mouse is close enough to be within the gizmo.
   Point3F camToEntity = position - editorPos;
   F32 size = camToEntity.len() / 4.0f;
   F32 dist = getMax((position - mSelectRedPoint).len(), (position - mSelectGreenPoint).len());
       dist = getMax(dist, (position - mSelectBluePoint).len());
   if (dist > size)
      return false;

   // Translate
   if (mProjectManager->mEditorMode == 0)
   {
      Point2I entityPointScreen = Plugins::Link.Rendering.worldToScreen(position);
      Point2I redPointScreen = Plugins::Link.Rendering.worldToScreen(mSelectRedPoint);
      Point2I greenPointScreen = Plugins::Link.Rendering.worldToScreen(mSelectGreenPoint);
      Point2I bluePointScreen = Plugins::Link.Rendering.worldToScreen(mSelectBluePoint);
      F32 redDist = Point2F(x - redPointScreen.x, y - redPointScreen.y).len();
      F32 greenDist = Point2F(x - greenPointScreen.x, y - greenPointScreen.y).len();
      F32 blueDist = Point2F(x - bluePointScreen.x, y - bluePointScreen.y).len();
      mSelectRed = (redDist < greenDist && redDist < blueDist);
      mSelectGreen = (greenDist < redDist && greenDist < blueDist);
      mSelectBlue = (blueDist < greenDist && blueDist < redDist);
      return true;
   }

   // Rotate
   if (mProjectManager->mEditorMode == 1)
   {
      Point3F xPoint = getPointOnXPlane(position.x, editorPos, editorPos + (worldRay * 1000.0f));
      Point3F yPoint = getPointOnYPlane(position.y, editorPos, editorPos + (worldRay * 1000.0f));
      Point3F zPoint = getPointOnZPlane(position.z, editorPos, editorPos + (worldRay * 1000.0f));
      F32 redDist = (xPoint - editorPos).len();
      F32 greenDist = (yPoint - editorPos).len();
      F32 blueDist = (zPoint - editorPos).len();
      mSelectRed = (redDist < greenDist && redDist < blueDist);
      mSelectGreen = (greenDist < redDist && greenDist < blueDist);
      mSelectBlue = (blueDist < greenDist && blueDist < redDist);
      return true;
   }

   // Scale
   if (mProjectManager->mEditorMode == 2)
   {
      Point2I entityPointScreen = Plugins::Link.Rendering.worldToScreen(position);
      Point2I redPointScreen = Plugins::Link.Rendering.worldToScreen(mSelectRedPoint);
      Point2I greenPointScreen = Plugins::Link.Rendering.worldToScreen(mSelectGreenPoint);
      Point2I bluePointScreen = Plugins::Link.Rendering.worldToScreen(mSelectBluePoint);
      F32 redDist = Point2F(x - redPointScreen.x, y - redPointScreen.y).len();
      F32 greenDist = Point2F(x - greenPointScreen.x, y - greenPointScreen.y).len();
      F32 blueDist = Point2F(x - bluePointScreen.x, y - bluePointScreen.y).len();
      mSelectRed = (redDist < greenDist && redDist < blueDist);
      mSelectGreen = (greenDist < redDist && greenDist < blueDist);
      mSelectBlue = (blueDist < greenDist && blueDist < redDist);
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

   // Selected Entity
   if (mSelectedEntity != NULL && mSelectedComponent == NULL)
   {
      mSelectedEntity->mPosition = newPosition;
      mSelectedEntity->refresh();
   }
      
   // Selected Component
   if (mSelectedEntity != NULL && mSelectedComponent != NULL)
   {
      mSelectedComponent->setPosition(newPosition);
      mSelectedEntity->refresh();
   }
}

void Gizmo::dragRotate(int x, int y)
{
   F32 dragAngle = 0.0f;
   Point3F worldRay = Plugins::Link.Rendering.screenToWorld(Point2I(x, y));
   Point3F editorPos = Plugins::Link.Scene.getActiveCamera()->getPosition();

   if (mDragRed)
   {
      Point3F xPoint = getPointOnXPlane(mSelectedEntity->mPosition.x, editorPos, editorPos + (worldRay * 1000.0f));
      dragAngle = mAtan(xPoint.z, xPoint.y) - mDownAngle;
      mSelectedRotation.x += dragAngle;
   }

   if (mDragGreen)
   {
      Point3F yPoint = getPointOnYPlane(mSelectedEntity->mPosition.y, editorPos, editorPos + (worldRay * 1000.0f));
      dragAngle = mAtan(yPoint.z, yPoint.x) - mDownAngle;
      mSelectedRotation.y += dragAngle;
   }

   if (mDragBlue)
   {
      Point3F zPoint = getPointOnZPlane(mSelectedEntity->mPosition.z, editorPos, editorPos + (worldRay * 1000.0f));
      dragAngle = mAtan(zPoint.y, zPoint.x) - mDownAngle;
      mSelectedRotation.z += dragAngle;
   }

   Point3F newRotation = mSelectedRotation;

   // Snapping
   if (mRotateSnap > 0.0f)
      newRotation = snap(mSelectedRotation, mRotateSnap);

   // Selected Entity
   if (mSelectedEntity != NULL && mSelectedComponent == NULL)
   {
      mSelectedEntity->mRotation = newRotation;
      mSelectedEntity->refresh();
   }

   // Selected Component
   if (mSelectedEntity != NULL && mSelectedComponent != NULL)
   {
      mSelectedComponent->setRotation(newRotation);
      mSelectedEntity->refresh();
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
      mSelectedEntity->mScale = snap(mSelectedScale, mScaleSnap);
   
   // Selected Entity
   if (mSelectedEntity != NULL && mSelectedComponent == NULL)
   {
      mSelectedEntity->mScale = newScale;
      mSelectedEntity->refresh();
   }

   // Selected Component
   if (mSelectedEntity != NULL && mSelectedComponent != NULL)
   {
      mSelectedComponent->setScale(newScale);
      mSelectedEntity->refresh();
   }
}