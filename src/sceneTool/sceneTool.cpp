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
#include "../Torque6EditorUI.h"

#include "sceneTool.h"
#include "3d/scene/camera.h"
#include <bx/bx.h>
#include <bx/fpumath.h>

SceneTool::SceneTool()
   : mScenePanel(NULL),
     mSelectedObject(NULL)
{
   mEntityIconList = new wxImageList(16, 16);
   mFeatureIconList = new wxImageList( 16, 16 );

   mSelectRed     = false;
   mSelectGreen   = false;
   mSelectBlue    = false;

   mSelectRedPoint   = Point3F::Zero;
   mSelectGreenPoint = Point3F::Zero;
   mSelectBluePoint  = Point3F::Zero;
   mDownPoint        = Point3F::Zero;
   mDragging         = false;
   mDragRed = false;
   mDragGreen = false;
   mDragBlue = false;
   mDownAngle = 0.0f;
}

SceneTool::~SceneTool()
{

}

void SceneTool::initTool()
{
   mScenePanel = new ScenePanel(mFrame, wxID_ANY);

   // Entity Icons.
   mEntityIconList->Add(wxBitmap("images/entityIcon.png", wxBITMAP_TYPE_PNG));
   mEntityIconList->Add(wxBitmap("images/componentIcon.png", wxBITMAP_TYPE_PNG));
   mScenePanel->entityList->AssignImageList(mEntityIconList);

   // Entity Events
   mScenePanel->entityList->Connect(wxID_ANY, wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(SceneTool::OnTreeEvent), NULL, this);
   mScenePanel->entityList->Connect(wxID_ANY, wxEVT_TREE_ITEM_MENU, wxTreeEventHandler(SceneTool::OnTreeMenu), NULL, this);
   mScenePanel->propertyGrid->Connect(wxID_ANY, wxEVT_PG_CHANGED, wxPropertyGridEventHandler(SceneTool::OnPropertyChanged), NULL, this);

   // Feature Icons.
   mFeatureIconList->Add(wxBitmap("images/featureIcon.png", wxBITMAP_TYPE_PNG));
   mScenePanel->featureList->AssignImageList(mFeatureIconList);

   // Feature Events
   mScenePanel->featureList->Connect(wxID_ANY, wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(SceneTool::OnTreeEvent), NULL, this);
   mScenePanel->featureList->Connect(wxID_ANY, wxEVT_TREE_ITEM_MENU, wxTreeEventHandler(SceneTool::OnTreeMenu), NULL, this);
   mScenePanel->featurePropGrid->Connect(wxID_ANY, wxEVT_PG_CHANGED, wxPropertyGridEventHandler(SceneTool::OnPropertyChanged), NULL, this);

   mEntityListRoot = mScenePanel->entityList->AddRoot("ROOT");
   mFeatureListRoot = mScenePanel->featureList->AddRoot("ROOT");

   mManager->AddPane(mScenePanel, wxAuiPaneInfo().Caption("Scene")
                                                  .CaptionVisible( true )
                                                  .CloseButton( true )
                                                  .PinButton( true )
                                                  .MaximizeButton(true)
                                                  .Dock()
                                                  .Resizable()
                                                  .FloatingSize( wxDefaultSize )
                                                  .Left()
                                                  .Hide());
   mManager->Update();
}

void SceneTool::openTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mScenePanel);
   paneInfo.Show();
   mManager->Update();

   if (mProjectManager->mProjectLoaded)
   {
      refreshEntityList();
      refreshFeatureList();
   }
}

void SceneTool::closeTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mScenePanel);
   paneInfo.Hide();
   mManager->Update();
}

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

void SceneTool::renderTool()
{
   if ( mSelectedObject != NULL )
   {
      Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(mSelectedObject);
      if (entity)
      {
         F32 boundingBox[16];
         bx::mtxSRT(boundingBox,
            entity->mScale.x, entity->mScale.y, entity->mScale.z,
            entity->mRotation.x, entity->mRotation.y, entity->mRotation.z,
            entity->mPosition.x, entity->mPosition.y, entity->mPosition.z);
         Plugins::Link.Graphics.drawBox3D(mProjectManager->mEditorOverlayView->id, mSelectedBoundingBox, ColorI(255, 255, 255, 255), boundingBox);

         if ( mSelectRed )
            Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, entity->mPosition, entity->mPosition + Point3F(50, 0, 0), ColorI(255, 20, 20, 255));
         else
            Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, entity->mPosition, entity->mPosition + Point3F(50, 0, 0), ColorI(200, 0, 0, 255));

         if ( mSelectGreen )
            Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, entity->mPosition, entity->mPosition + Point3F(0, 50, 0), ColorI(20, 255, 20, 255));
         else
            Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, entity->mPosition, entity->mPosition + Point3F(0, 50, 0), ColorI(0, 200, 0, 255));

         if ( mSelectBlue )
            Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, entity->mPosition, entity->mPosition + Point3F(0, 0, 50), ColorI(20, 20, 255, 255));
         else
            Plugins::Link.Graphics.drawLine3D(mProjectManager->mEditorOverlayView->id, entity->mPosition, entity->mPosition + Point3F(0, 0, 50), ColorI(0, 0, 200, 255));
      }
   }
}

Point3F getPointOnXPlane(Point3F p1, Point3F p2)
{
   F32 a = (0.0f - p1.x) / (p2.x - p1.x);
   return Point3F(0.0f, p1.y + a*(p2.y - p1.y), p1.z + a*(p2.z - p1.z));
}

Point3F getPointOnYPlane(Point3F p1, Point3F p2)
{
   F32 a = (0.0f - p1.y) / (p2.y - p1.y);
   return Point3F(p1.x + a*(p2.x - p1.x), 0.0f, p1.z + a*(p2.z - p1.z));
}

Point3F getPointOnZPlane(Point3F p1, Point3F p2)
{
   F32 a = (0.0f - p1.z) / (p2.z - p1.z);
   return Point3F(p1.x + a*(p2.x - p1.x), p1.y + a*(p2.y - p1.y), 0.0f);
}

bool SceneTool::onMouseLeftDown(int x, int y)
{
   mDragRed = false;
   mDragGreen = false;
   mDragBlue = false;
   mDragging = true;

   Point3F worldRay = Plugins::Link.Rendering.screenToWorld(Point2I(x, y));
   Point3F editorPos = Plugins::Link.Scene.getActiveCamera()->getPosition();

   Scene::SceneEntity* hit = Plugins::Link.Scene.raycast(editorPos, editorPos + (worldRay * 1000.0f));
   if (mSelectedObject != hit)
   {
      if (hit)
         selectEntity(hit);
   }
   else {

      if (mSelectRed)
      {
         mDownPoint = mSelectRedPoint;
         Point3F xPoint = getPointOnXPlane(editorPos, editorPos + (worldRay * 1000.0f));
         mDownAngle = mAtan(xPoint.z, xPoint.y);
         mDragRed = true;
      }

      if (mSelectGreen)
      {
         mDownPoint = mSelectGreenPoint;
         Point3F yPoint = getPointOnYPlane(editorPos, editorPos + (worldRay * 1000.0f));
         mDownAngle = mAtan(yPoint.z, yPoint.x);
         mDragGreen = true;
      }

      if (mSelectBlue)
      {
         mDownPoint = mSelectBluePoint;
         Point3F zPoint = getPointOnZPlane(editorPos, editorPos + (worldRay * 1000.0f));
         mDownAngle = mAtan(zPoint.y, zPoint.x);
         mDragBlue = true;
      }
   }

   return false;
}

bool SceneTool::onMouseLeftUp(int x, int y)
{
   mDragging = false;
   mDragRed = false;
   mDragGreen = false;
   mDragBlue = false;

   if ( mSelectedObject != NULL )
      loadObjectProperties(mScenePanel->propertyGrid, mSelectedObject);

   return false;
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

bool SceneTool::onMouseMove(int x, int y)
{
   if (mSelectedObject == NULL)
      return false;

   Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(mSelectedObject);
   if (!entity)
      return false;

   Point3F dummyPoint;
   Point3F worldRay = Plugins::Link.Rendering.screenToWorld(Point2I(x, y));
   Point3F editorPos = Plugins::Link.Scene.getActiveCamera()->getPosition();
   Plugins::Link.Rendering.closestPointsOnTwoLines(mSelectRedPoint, dummyPoint, entity->mPosition, Point3F(1000.0f, 0.0f, 0.0f), editorPos, worldRay * 1000.0f);
   Plugins::Link.Rendering.closestPointsOnTwoLines(mSelectGreenPoint, dummyPoint, entity->mPosition, Point3F(0.0, 1000.0f, 0.0f), editorPos, worldRay * 1000.0f);
   Plugins::Link.Rendering.closestPointsOnTwoLines(mSelectBluePoint, dummyPoint, entity->mPosition, Point3F(0.0f, 0.0f, 1000.0f), editorPos, worldRay * 1000.0f);

   Point2I redPointScreen = Plugins::Link.Rendering.worldToScreen(mSelectRedPoint);
   Point2I greenPointScreen = Plugins::Link.Rendering.worldToScreen(mSelectGreenPoint);
   Point2I bluePointScreen = Plugins::Link.Rendering.worldToScreen(mSelectBluePoint);
   F32 redDist = Point2F(x - redPointScreen.x, y - redPointScreen.y).len();
   F32 greenDist = Point2F(x - greenPointScreen.x, y - greenPointScreen.y).len();
   F32 blueDist = Point2F(x - bluePointScreen.x, y - bluePointScreen.y).len();
   mSelectRed = (redDist < greenDist && redDist < blueDist);
   mSelectGreen = (greenDist < redDist && greenDist < blueDist);
   mSelectBlue = (blueDist < greenDist && blueDist < redDist);

   if (mDragging)
   {
      Point3F dragVector = Point3F::Zero;
      F32 dragAngle = 0.0f;

      if (mDragRed)
      {
         dragVector = mSelectRedPoint - mDownPoint;
         if (mProjectManager->mEditorMode == 1)
         {
            Point3F xPoint = getPointOnXPlane(editorPos, editorPos + (worldRay * 1000.0f));
            dragAngle = mAtan(xPoint.z, xPoint.y) - mDownAngle;
            entity->mRotation.x += dragAngle;
            entity->refresh();
         }
      }

      if (mDragGreen)
      {
         dragVector = mSelectGreenPoint - mDownPoint;

         if (mProjectManager->mEditorMode == 1)
         {
            Point3F yPoint = getPointOnYPlane(editorPos, editorPos + (worldRay * 1000.0f));
            dragAngle = mAtan(yPoint.z, yPoint.x) - mDownAngle;
            entity->mRotation.y += dragAngle;
            entity->refresh();
         }
      }

      if (mDragBlue)
      {
         dragVector = mSelectBluePoint - mDownPoint;

         if (mProjectManager->mEditorMode == 1)
         {
            Point3F zPoint = getPointOnZPlane(editorPos, editorPos + (worldRay * 1000.0f));
            dragAngle = mAtan(zPoint.y, zPoint.x) - mDownAngle;
            entity->mRotation.z += dragAngle;
            entity->refresh();
         }
      }

      // Move mode.
      if (mProjectManager->mEditorMode == 0)
      {
         mDownPoint += dragVector;
         entity->mPosition += dragVector;
         entity->refresh();
      }

      // Rotate mode
      if (mProjectManager->mEditorMode == 1)
      {
         mDownAngle += dragAngle;
      }

      // Scale mode.
      if (mProjectManager->mEditorMode == 2)
      {
         mDownPoint += dragVector;
         entity->mScale += dragVector;
         entity->refresh();
      }
   }

   return false;
}


void SceneTool::onProjectLoaded(wxString projectName, wxString projectPath)
{
   refreshEntityList();
   refreshFeatureList();
}

void SceneTool::onProjectClosed()
{
   //
}

void SceneTool::OnTreeEvent( wxTreeEvent& evt )
{
   if (evt.GetId() == ENTITY_LIST)
   {
      EntityTreeItemData* data = dynamic_cast<EntityTreeItemData*>(mScenePanel->entityList->GetItemData(evt.GetItem()));
      if (data)
      {
         // Did we select an entity ..
         Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(data->objPtr);
         if (entity)
         {
            selectEntity(entity);
            return;
         }

         // .. or a component?
         Scene::BaseComponent* component = dynamic_cast<Scene::BaseComponent*>(data->objPtr);
         if (component)
         {
            selectComponent(component);
            return;
         }
      }
   }

   if (evt.GetId() == FEATURE_LIST)
   {
      FeatureTreeItemData* data = dynamic_cast<FeatureTreeItemData*>(mScenePanel->featureList->GetItemData(evt.GetItem()));
      if (data)
      {
         loadObjectProperties(mScenePanel->featurePropGrid, data->objPtr);
         return;
      }
   }
}

void SceneTool::OnTreeMenu( wxTreeEvent& evt ) 
{ 
   wxMenu* menu = new wxMenu; 
   menu->Append(wxID_OPEN, wxT("Add Empty Entity")); 
   menu->Append(wxID_OPEN, wxT("Add Entity from Template")); 
   menu->AppendSeparator();
   
   wxMenu* compMenu = new wxMenu;
   compMenu->Append(wxID_OPEN, wxT("AnimationComponent")); 
   compMenu->Append(wxID_OPEN, wxT("LightComponent")); 
   compMenu->Append(wxID_OPEN, wxT("MeshComponent")); 
   compMenu->Append(wxID_OPEN, wxT("PhysicsComponent")); 
   compMenu->Append(wxID_OPEN, wxT("TextComponent")); 
   menu->AppendSubMenu(compMenu, wxT("Add Component"));

   menu->AppendSeparator();

   menu->Append(wxID_OPEN, wxT("Delete")); 
   mFrame->PopupMenu(menu, wxDefaultPosition); 
   delete menu; 
} 

void SceneTool::OnPropertyChanged( wxPropertyGridEvent& evt ) 
{ 
   wxString name = evt.GetPropertyName();
   wxVariant val = evt.GetPropertyValue();
   wxString strVal = val.GetString();

   mSelectedObject->setField(name, strVal);
   Plugins::Link.Scene.refresh();
}

void SceneTool::refreshEntityList()
{
   // Clear list.
   mScenePanel->entityList->DeleteAllItems();
   mEntityListRoot = mScenePanel->entityList->AddRoot("ROOT");

   SimGroup* sceneGroup = Plugins::Link.Scene.getEntityGroup();
   if ( sceneGroup != NULL )
   {
      for(U32 n = 0; n < sceneGroup->size(); ++n)
      {
         Scene::SceneEntity* entity = dynamic_cast<Scene::SceneEntity*>(sceneGroup->at(n));
         if ( !entity ) continue;
         wxTreeItemId entityItem = mScenePanel->entityList->AppendItem(mEntityListRoot, entity->mTemplateAssetID, 0, -1, new EntityTreeItemData(entity));

         // Iterate components
         Scene::EntityTemplate* entityTemplate = entity->mTemplate;
         for(S32 n = 0; n < entityTemplate->size(); ++n)
         {
            Scene::BaseComponent* component = static_cast<Scene::BaseComponent*>(entityTemplate->at(n));

            wxString compName(component->getClassName());
            const char* internalName = component->getInternalName();
            if ( internalName )
            {
               compName = internalName;
               compName.Append(" (");
               compName.Append(component->getClassName());
               compName.Append(")");
            }

            if ( component )
               mScenePanel->entityList->AppendItem(entityItem, compName, 1, -1, new EntityTreeItemData(component));
         }
      }
   }
}

void SceneTool::refreshFeatureList()
{
   // Clear list.
   mScenePanel->featureList->DeleteAllItems();
   mEntityListRoot = mScenePanel->featureList->AddRoot("ROOT");

   SimGroup* featureGroup = Plugins::Link.Scene.getFeatureGroup();
   if (featureGroup != NULL)
   {
      for (U32 n = 0; n < featureGroup->size(); ++n)
      {
         Scene::SceneFeature* feature = dynamic_cast<Scene::SceneFeature*>(featureGroup->at(n));
         if (!feature) continue;
         wxTreeItemId entityItem = mScenePanel->featureList->AppendItem(mFeatureListRoot, feature->mName, 0, -1, new FeatureTreeItemData(feature));
      }
   }
}

void SceneTool::loadObjectProperties(wxPropertyGrid* propertyGrid, SimObject* obj)
{
   propertyGrid->Clear();

   wxString fieldGroup("");
   bool addFieldGroup = false;

   AbstractClassRep::FieldList fieldList = obj->getFieldList();
   for(Vector<AbstractClassRep::Field>::iterator itr = fieldList.begin(); itr != fieldList.end(); itr++)
   {
      const AbstractClassRep::Field* f = itr;
      if( f->type == AbstractClassRep::DepricatedFieldType ||
            f->type == AbstractClassRep::EndGroupFieldType) 
         continue;

      if ( f->type == AbstractClassRep::StartGroupFieldType )
      {
         addFieldGroup = true;
         fieldGroup = f->pGroupname;
         continue;
      }

      for(U32 j = 0; S32(j) < f->elementCount; j++)
      {                      
         const char *val = (*f->getDataFn)( obj, Plugins::Link.Con.getData(f->type, (void *) (((const char *)obj) + f->offset), j, f->table, f->flag) );

         if( !val )
            continue;

         if ( addFieldGroup )
         {
            propertyGrid->Append( new wxPropertyCategory(fieldGroup) );
            addFieldGroup = false;
         }

         if ( f->type ==  Plugins::Link.Con.TypeBool )
            propertyGrid->Append( new wxBoolProperty(f->pFieldname, f->pFieldname, val) );
         else 
            propertyGrid->Append( new wxStringProperty(f->pFieldname, f->pFieldname, val) );
      }
   }
}

void SceneTool::selectEntity(Scene::SceneEntity* entity)
{
   mSelectedObject = entity;
   mSelectedBoundingBox = entity->mBoundingBox;
   loadObjectProperties(mScenePanel->propertyGrid, entity);
}

void SceneTool::selectComponent(Scene::BaseComponent* component)
{
   mSelectedObject = component;
   mSelectedBoundingBox = component->getBoundingBox();
   loadObjectProperties(mScenePanel->propertyGrid, component);
}