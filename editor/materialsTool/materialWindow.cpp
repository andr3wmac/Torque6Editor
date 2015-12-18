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

#include "wx/graphics.h"
#include <wx/socket.h>
#include "materialWindow.h"
#include "materialsTool.h"

wxBEGIN_EVENT_TABLE(MaterialWindow, wxScrolledWindow)
    EVT_PAINT                    (MaterialWindow::OnPaint)
    EVT_MOTION                   (MaterialWindow::OnMouseMove)
    EVT_LEFT_DOWN                (MaterialWindow::OnMouseDown)
    EVT_RIGHT_DOWN               (MaterialWindow::OnRightMouseDown)
    EVT_RIGHT_UP                 (MaterialWindow::OnRightMouseUp)
    EVT_LEFT_UP                  (MaterialWindow::OnMouseUp)
wxEND_EVENT_TABLE()

MaterialWindow::MaterialWindow(wxWindow* parent, MaterialsTool* matTool)
   : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE),
     mMaterialsTool(matTool),
     mMaterialAsset(NULL),
     mRightMouseDown(false),
     mRightMouseDrag(false),
     mWindowX(0.0f),
     mWindowY(0.0f)
{
   mActiveConnection = NULL;
   mSelectedNode     = NULL;
   mHoverNode        = NULL;

   Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MaterialWindow::OnMenuEvent), NULL, this);
   SetVirtualSize(1000, 1000);
}

void MaterialWindow::OnPaint(wxPaintEvent &WXUNUSED(event))
{
   wxBufferedPaintDC bpdc(this);
   wxGCDC gdc;
   wxGraphicsRenderer* const renderer = wxGraphicsRenderer::GetDefaultRenderer();
   wxGraphicsContext* context;
   if ( wxMemoryDC *memdc = wxDynamicCast(&bpdc, wxMemoryDC) )
   {
      context = renderer->CreateContext(*memdc);
   }
   gdc.SetGraphicsContext(context);

   // Draw Background
   gdc.SetBackground(wxBrush(wxColour(38, 38, 38)));
   gdc.Clear();

   // Draw Background Grid
   int gridOffsetX = (int)mWindowX % 20;
   int gridOffsetY = (int)mWindowY % 20;
   gdc.SetPen(wxPen( wxColour(128, 128, 128, 128), 1) );
   wxSize windowSize = GetSize();
   // Vertical Lines
   for(int x = 0; x < (windowSize.x + 40); x += 20)
      gdc.DrawLine(gridOffsetX + x - 20, gridOffsetY - 20, gridOffsetX + x - 20, gridOffsetY + windowSize.y + 40);
   // Horizontal Lines
   for(int y = 0; y < (windowSize.y + 40); y += 20)
      gdc.DrawLine(gridOffsetX - 20, gridOffsetY + y - 20, gridOffsetX + windowSize.x + 40, gridOffsetY + y - 20);

   // Draw Nodes
   for(S32 n = 0; n < nodeList.size(); ++n )
   {
      Node* node = &nodeList[n];
      drawNode(gdc, node);
   }

   // Draw Connections
   for(S32 n = 0; n < connectionList.size(); ++n )
   {
      Connection* connection = &connectionList[n];
      drawConnection(gdc, context, connection);
   }
}

void MaterialWindow::OnMouseMove(wxMouseEvent &evt)
{
   if (mRightMouseDown)
   {
      mRightMouseDrag = true;
      wxPoint mouseDelta = evt.GetPosition() - mLastMousePoint;
      mLastMousePoint = evt.GetPosition();
      mWindowX += mouseDelta.x;
      mWindowY += mouseDelta.y;
      Refresh(false);
      return;
   }

   mHoverNode = NULL;
   mLastMousePoint = evt.GetPosition();
   wxPoint mousePoint = evt.GetPosition();

   // Drag Event.
   if ( evt.LeftIsDown() && mSelectedNode != NULL )
   {
      mSelectedNode->x += mousePoint.x - mMouseDownPoint.x;
      mSelectedNode->y += mousePoint.y - mMouseDownPoint.y;
      mMouseDownPoint = mousePoint;

      if ( mSelectedNode->materialNode != NULL )
         mSelectedNode->materialNode->mPosition.set(mSelectedNode->x,  mSelectedNode->y);

      Refresh(false);
      return;
   }

   // Just regular click event.
   for(S32 n = 0; n < nodeList.size(); ++n )
   {
      Node* node = &nodeList[n];
      node->mouseOver = false;

      if ( mousePoint.x >= (mWindowX + node->x) && mousePoint.x <= (mWindowX + node->x + node->width)
         && mousePoint.y >= (mWindowY + node->y) && mousePoint.y <= (mWindowY + node->y + node->height) )
      {
         node->mouseOver = true;
         mHoverNode = node;

         for ( S32 i = 0; i < node->inputs.size(); ++i )
         {
            InputPoint* input = &node->inputs[i];
            input->mouseOver = false;

            wxPoint dist = input->lastPosition - mLastMousePoint;
            if ( abs(dist.x) <= 5 && abs(dist.y) <= 5 )
               input->mouseOver = true;
         }

         for ( S32 i = 0; i < node->outputs.size(); ++i )
         {
            OutputPoint* output = &node->outputs[i];
            output->mouseOver = false;

            wxPoint dist = output->lastPosition - mLastMousePoint;
            if ( abs(dist.x) <= 5 && abs(dist.y) <= 5 )
               output->mouseOver = true;
         }
      }
   }

   Refresh(false);
}

void MaterialWindow::OnMouseDown(wxMouseEvent &evt)
{
   mLastMousePoint = evt.GetPosition();
   mSelectedNode = NULL;
   mMouseDownPoint = evt.GetPosition();

   for(S32 n = 0; n < nodeList.size(); ++n )
   {
      Node* node = &nodeList[n];

      if ( node->mouseOver )
      {
         for ( S32 i = 0; i < node->inputs.size(); ++i )
         {
            InputPoint* input = &node->inputs[i];
            if ( input->mouseOver ) 
            {
               if ( mActiveConnection != NULL )
               {
                  if ( mActiveConnection->outputToMouse )
                  {
                     mActiveConnection->inputNodeName = node->name;
                     mActiveConnection->inputIndex = i;
                     mActiveConnection->outputToMouse = false;
                     mActiveConnection = NULL;
                     return;
                  }
               } else {
                  Connection* conn = findConnectionFromInput(node->name, i);
                  if ( conn )
                  {
                     conn->outputToMouse = true;
                     mActiveConnection = conn;
                     return;
                  } else {
                     Connection newConn;
                     newConn.inputNodeName = node->name;
                     newConn.inputIndex = i;
                     newConn.inputToMouse = true;
                     connectionList.push_back(newConn);
                     mActiveConnection = &connectionList.back();
                     return;
                  }
               }
            }
         }

         for ( S32 i = 0; i < node->outputs.size(); ++i )
         {
            OutputPoint* output = &node->outputs[i];
            if ( output->mouseOver ) 
            {
               if ( mActiveConnection != NULL )
               {
                  if ( mActiveConnection->inputToMouse )
                  {
                     mActiveConnection->outputNodeName = node->name;
                     mActiveConnection->outputIndex = i;
                     mActiveConnection->inputToMouse = false;
                     mActiveConnection = NULL;
                     return;
                  }
               } else {
                  Connection* conn = findConnectionFromOutput(node->name, i);
                  if ( conn )
                  {
                     conn->inputToMouse = true;
                     mActiveConnection = conn;
                     return;
                  } else {
                     Connection newConn;
                     newConn.outputNodeName = node->name;
                     newConn.outputIndex = i;
                     newConn.outputToMouse = true;
                     connectionList.push_back(newConn);
                     mActiveConnection = &connectionList.back();
                     return;
                  }
               }
            }
         }

         mMaterialsTool->selectNode(this, node);
         mSelectedNode = node;
         return;
      }
   }

   // Clicked on nothing, delete connection.
   if ( mActiveConnection != NULL )
   {
      Vector<Connection> newConnectionList;

      for(S32 n = 0; n < connectionList.size(); ++n )
      {
         Connection* connection = &connectionList[n];
         if ( connection != mActiveConnection )
            newConnectionList.push_back(*connection);
      }

      connectionList.clear();
      connectionList.merge(newConnectionList);
      mActiveConnection = NULL;
   }
}

void MaterialWindow::OnMouseUp(wxMouseEvent &event)
{

}

void MaterialWindow::OnRightMouseDown(wxMouseEvent &evt)
{
   mRightMouseDown = true;
   mRightMouseDrag = false;
   mMouseDownPoint = evt.GetPosition();
   mLastMousePoint = evt.GetPosition();
}

void MaterialWindow::OnRightMouseUp(wxMouseEvent &evt)
{
   // If right mouse wasn't dragged we open context menu.
   if (!mRightMouseDrag)
   {
      mLastMousePoint = evt.GetPosition();

      wxMenu* menu = new wxMenu;

      if (mHoverNode != NULL)
      {
         menu->Append(100, wxT("Delete Node"));
         menu->AppendSeparator();
      }

      menu->Append(0, wxT("Opaque"));
      menu->Append(1, wxT("Float"));
      menu->Append(2, wxT("Vec2"));
      menu->Append(3, wxT("Vec3"));
      menu->Append(4, wxT("Vec4"));
      menu->Append(5, wxT("Texture"));
      menu->Append(6, wxT("Time"));
      menu->Append(7, wxT("Cos"));
      menu->Append(8, wxT("Sin"));
      menu->Append(9, wxT("Multiply"));
      menu->Append(10, wxT("Lerp"));
      PopupMenu(menu, wxDefaultPosition);
      delete menu;
   }

   // Reset
   mRightMouseDown = false;
   mRightMouseDrag = false;
}

void MaterialWindow::OnMenuEvent( wxCommandEvent& evt )
{
   switch( evt.GetId() )
   {
      // Add Nodes
      case 0: addNode(mMaterialAsset->getTemplate(), "Opaque"); break;
      case 1: addNode(mMaterialAsset->getTemplate(), "Float"); break;
      case 2: addNode(mMaterialAsset->getTemplate(), "Vec2"); break;
      case 3: addNode(mMaterialAsset->getTemplate(), "Vec3"); break;
      case 4: addNode(mMaterialAsset->getTemplate(), "Vec4"); break;
      case 5: addNode(mMaterialAsset->getTemplate(), "Texture"); break;
      case 6: addNode(mMaterialAsset->getTemplate(), "Time"); break;
      case 7: addNode(mMaterialAsset->getTemplate(), "Cos"); break;
      case 8: addNode(mMaterialAsset->getTemplate(), "Sin"); break;
      case 9: addNode(mMaterialAsset->getTemplate(), "Multiply"); break;
      case 10: addNode(mMaterialAsset->getTemplate(), "Lerp"); break;

      // Delete Node
      case 100:
         deleteNode(mMaterialAsset->getTemplate(), mHoverNode);
         break;
   }
}

void MaterialWindow::drawNode(wxGCDC& gdc, Node* node)
{
   // Background
   gdc.SetBrush(wxBrush(wxColour(75, 75, 75), wxBRUSHSTYLE_SOLID));
   gdc.DrawRoundedRectangle(mWindowX + node->x, mWindowY + node->y, node->width, node->height, 10);

   // Title Bar Background
   if ( node->mouseOver || mSelectedNode == node )
      gdc.SetBrush(wxBrush(wxColour(50, 50, 50), wxBRUSHSTYLE_SOLID));
   else
      gdc.SetBrush(wxBrush(wxColour(30, 30, 30), wxBRUSHSTYLE_SOLID));
   gdc.DrawRoundedRectangle(mWindowX + node->x + 5, mWindowY + node->y + 5, node->width - 10, 30, 10);

   // Title Bar Text
   wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
   gdc.SetFont(font);
   gdc.SetTextForeground(wxColour(200, 200, 190));
   wxSize textSize = gdc.GetTextExtent(node->name);
   gdc.DrawText(node->name, mWindowX + node->x + (node->width / 2) - (textSize.GetWidth() / 2), mWindowY + node->y + 18 - (textSize.GetHeight() / 2));

   // Input Points
   for(S32 n = 0; n < node->inputs.size(); ++n )
   {
      InputPoint* input = &node->inputs[n];
      input->lastPosition = wxPoint(mWindowX + node->x + 12.0f, mWindowY + node->y + 49.0f + (n * 20.0f));

      U8 alpha_val = input->mouseOver ? 255 : 175;

      gdc.SetBrush(wxBrush(wxColour(255, 255, 255, alpha_val), wxBRUSHSTYLE_SOLID));
      gdc.DrawCircle(mWindowX + node->x + 12.0f, mWindowY + node->y + 49.0f + (n * 20.0f), 5);

      wxSize inputTextSize = gdc.GetTextExtent(input->name);
      wxFont inputfont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
      gdc.SetFont(inputfont);
      gdc.SetTextForeground(wxColour(255, 255, 255, alpha_val));
      gdc.DrawText(input->name, mWindowX + node->x + 22, mWindowY + node->y + 41 + (n * 20));
   }

   // Output Points
   for(S32 n = 0; n < node->outputs.size(); ++n )
   {
      OutputPoint* output = &node->outputs[n];
      output->lastPosition = wxPoint(mWindowX + node->x + node->width - 12.0f, mWindowY + node->y + 49.0f + (n * 20.0f));

      U8 alpha_val = output->mouseOver ? 255 : 175;

      gdc.SetBrush(wxBrush(wxColour(output->color.red, output->color.green, output->color.blue, alpha_val), wxBRUSHSTYLE_SOLID));
      gdc.DrawCircle(mWindowX + node->x + node->width - 12.0f, mWindowY + node->y + 49.0f + (n * 20.0f), 5);
   }

   // Float can be shown in a single line of text.
   if (node->type == "Float")
   {
      wxString floatValue = wxString::Format("%f", node->color.red);
      wxFont inputfont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
      gdc.SetFont(inputfont);
      gdc.SetTextForeground(wxColour(255, 255, 255, 255));
      gdc.DrawText(floatValue, mWindowX + node->x + 12.0f, mWindowY + node->y + 41);
   }

   // Vec3 and Vec4 draw a color preview.
   if (node->type == "Vec3" || node->type == "Vec4")
   {
      gdc.SetBrush(wxBrush(wxColour(node->color.red * 255, node->color.green * 255, node->color.blue * 255, node->color.alpha * 255), wxBRUSHSTYLE_SOLID));
      gdc.DrawRectangle(mWindowX + node->x + 12.0f, mWindowY + node->y + 16.0f + (node->height / 2.0f) - 32.0f, 64.0f, 64.0f);
   }
}

void MaterialWindow::drawConnection(wxGCDC& gdc, wxGraphicsContext* context, Connection* conn)
{
   wxPoint start;
   wxPoint end;

   if ( conn->outputToMouse )
      end = mLastMousePoint;
   else {
      Node* endNode = findNode(conn->inputNodeName);
      if ( !endNode || conn->inputIndex >= endNode->inputs.size() ) 
         return;
      end = endNode->inputs[conn->inputIndex].lastPosition;
   }

   if ( conn->inputToMouse )
      start = mLastMousePoint;
   else {
      Node* startNode = findNode(conn->outputNodeName);
      if ( !startNode || conn->outputIndex >= startNode->outputs.size() ) 
         return;
      start = startNode->outputs[conn->outputIndex].lastPosition;
   }

   // Create Bezier Curve
   wxGraphicsPath path = context->CreatePath();
   path.MoveToPoint(start.x, start.y);
   wxPoint ctrlPointA(start);
   ctrlPointA.x += 50;
   ctrlPointA.y -= (end.y - start.y) / 4;
   wxPoint ctrlPointB(end);
   ctrlPointB.x -= 50;
   ctrlPointB.y += (end.y - start.y) / 4;
   path.AddCurveToPoint(ctrlPointA.x, ctrlPointA.y, ctrlPointB.x, ctrlPointB.y, end.x, end.y);

   // Draw Bezier Curve
   context->SetPen( wxPen( *wxWHITE, 4) );
   context->StrokePath(path);
}

wxString MaterialWindow::getUniqueNodeName(wxString name)
{
   if ( !findNode(name) )
   {
      return name;
   }

   U32 extension = 0;
   while(true)
   {
      wxString newName = wxString::Format("%s%d", name, extension);

      if ( !findNode(newName) )
         return newName;

      extension++;
   }

   return name;
}

Node* MaterialWindow::findNode(const char* name)
{
   for(S32 n = 0; n < nodeList.size(); ++n )
   {
      Node* node = &nodeList[n];
      if ( dStrcmp(node->name, name) == 0 )
      {
         return node;
      }
   }
   return NULL;
}

Connection* MaterialWindow::findConnectionFromOutput(const char* name, U32 index)
{

   for(S32 n = 0; n < connectionList.size(); ++n )
   {
      Connection* connection = &connectionList[n];
      if ( dStrcmp(connection->outputNodeName, name) == 0 )
      {
         if ( index == -1 || connection->outputIndex == index )
            return connection;
      }
   }
   return NULL;
}

Connection* MaterialWindow::findConnectionFromInput(const char* name, U32 index)
{
   for(S32 n = 0; n < connectionList.size(); ++n )
   {
      Connection* connection = &connectionList[n];
      if ( dStrcmp(connection->inputNodeName, name) == 0 )
      {
         if ( index == -1 || connection->inputIndex == index )
            return connection;
      }
   }
   return NULL;
}

void MaterialWindow::loadMaterial(MaterialAsset* mat)
{
   mMaterialAsset = mat;
   Scene::MaterialTemplate* mMaterialTemplate = mat->getTemplate();
   
   for(S32 n = 0; n < mMaterialTemplate->size(); ++n)
   {
      Scene::BaseNode* node = dynamic_cast<Scene::BaseNode*>(mMaterialTemplate->at(n));
      addNode(mMaterialTemplate, node->type, node);
   }

   for(S32 n = 0; n < nodeList.size(); ++n)
   {
      Node* node = &nodeList[n];
      addNodeConnection(node);
   }
}

void MaterialWindow::saveMaterial()
{
   if (mMaterialAsset == NULL)
      return;

   for (S32 n = 0; n < nodeList.size(); ++n)
   {
      Node* node = &nodeList[n];
      saveNode(mMaterialAsset->getTemplate(), node);
   }

   for (S32 n = 0; n < connectionList.size(); ++n)
   {
      Connection* connection = &connectionList[n];
      saveConnection(connection);
   }

   mMaterialAsset->saveMaterial();
   mMaterialAsset->reloadMaterial();
}