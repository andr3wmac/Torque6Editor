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

#ifndef _MATERIAL_WINDOW_H_
#define _MATERIAL_WINDOW_H_
 
#include "wx/wxprec.h"
 
#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#ifndef _WX_SOCKET_H_
#include <wx/socket.h>
#endif

#ifndef _WX_GRAPHICS_DC_H_
#include "wx/dcgraph.h"
#endif

#ifndef _WX_DCBUFFER_H_
#include "wx/dcbuffer.h"
#endif

#ifndef _MATERIAL_NODE_H_
#include "materialNode.h"
#endif

#ifndef _PLUGINS_SHARED_H
#include <plugins/plugins_shared.h>
#endif

#ifndef _SIM_OBJECT_H_
#include <sim/simObject.h>
#endif

#ifndef _DEFERRED_NODE_H_
#include "3d/material/nodes/deferredNode.h"
#endif

#ifndef _FLOAT_NODE_H_
#include "3d/material/nodes/floatNode.h"
#endif

#ifndef _FORWARD_NODE_H_
#include "3d/material/nodes/forwardNode.h"
#endif

#ifndef _MULTIPLY_NODE_H_
#include "3d/material/nodes/multiplyNode.h"
#endif

#ifndef _SIN_NODE_H_
#include "3d/material/nodes/sinNode.h"
#endif

#ifndef _TEXTURE_NODE_H_
#include "3d/material/nodes/textureNode.h"
#endif

#ifndef _TIME_NODE_H_
#include "3d/material/nodes/timeNode.h"
#endif

#ifndef _VEC3_NODE_H_
#include "3d/material/nodes/vec3Node.h"
#endif

class MaterialWindow: public wxScrolledWindow
{
   protected:
      wxPoint     mMouseDownPoint;
      wxPoint     mLastMousePoint;
      Connection* mActiveConnection;
      Node*       mSelectedNode;

   public:
      Vector<Node> nodeList;
      Vector<Connection> connectionList;

      MaterialWindow( wxWindow* parent );

      void loadMaterial(MaterialAsset* mat);

      Node* findNode(const char* name);
      wxString getUniqueNodeName(wxString name);
      Connection* findConnectionFromOutput(const char* name, U32 index = -1);
      Connection* findConnectionFromInput(const char* name, U32 index = -1);

      void  addConnection(const char* output, U32 outputIndex, const char* input, U32 inputIndex);
      Node* addDeferredNode(Scene::BaseNode* node = NULL);
      Node* addFloatNode(Scene::BaseNode* node = NULL);
      Node* addVec2Node(Scene::BaseNode* node = NULL);
      Node* addVec3Node(Scene::BaseNode* node = NULL);
      Node* addVec4Node(Scene::BaseNode* node = NULL);
      Node* addTextureNode(Scene::BaseNode* node = NULL);
      Node* addTimeNode(Scene::BaseNode* node = NULL);
      Node* addCosNode(Scene::BaseNode* node = NULL);
      Node* addSinNode(Scene::BaseNode* node = NULL);
      Node* addMultiplyNode(Scene::BaseNode* node = NULL);
      void  addNode(Scene::MaterialTemplate* matTemplate, const char* type, Scene::BaseNode* node = NULL);
      void  addNodeConnection(Node* node);
      void  saveConnection(Connection* connection);
      void  saveNode(Scene::MaterialTemplate* matTemplate, Node* node);
      void  drawNode(wxGCDC& gdc, Node* node);
      void  drawConnection(wxGCDC& gdc, wxGraphicsContext* context, Connection* conn);

      void OnPaint(wxPaintEvent &evt);
      void OnMouseMove(wxMouseEvent &evt);
      void OnMouseDown(wxMouseEvent &evt);
      void OnMouseUp(wxMouseEvent &evt);
      void OnRightMouseDown(wxMouseEvent &evt);
      void OnMenuEvent( wxCommandEvent& evt );

private:
    wxDECLARE_EVENT_TABLE();
};
 
#endif // _MATERIAL_WINDOW_H_