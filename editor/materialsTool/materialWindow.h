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

#ifndef _MATERIALS_H_
#include "materials/materials.h"
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

#ifndef _OPAQUE_NODE_H_
#include "materials/nodes/opaqueNode.h"
#endif

#ifndef _FLOAT_NODE_H_
#include "materials/nodes/floatNode.h"
#endif

#ifndef _FORWARD_NODE_H_
#include "materials/nodes/forwardNode.h"
#endif

#ifndef _MULTIPLY_NODE_H_
#include "materials/nodes/multiplyNode.h"
#endif

#ifndef _LERP_NODE_H_
#include "materials/nodes/lerpNode.h"
#endif

#ifndef _SIN_NODE_H_
#include "materials/nodes/sinNode.h"
#endif

#ifndef _COS_NODE_H_
#include "materials/nodes/cosNode.h"
#endif

#ifndef _TEXTURE_NODE_H_
#include "materials/nodes/textureNode.h"
#endif

#ifndef _TIME_NODE_H_
#include "materials/nodes/timeNode.h"
#endif

#ifndef _VEC2_NODE_H_
#include "materials/nodes/vec2Node.h"
#endif

#ifndef _VEC3_NODE_H_
#include "materials/nodes/vec3Node.h"
#endif

#ifndef _VEC4_NODE_H_
#include "materials/nodes/vec4Node.h"
#endif

class MaterialsTool;

class MaterialWindow: public wxScrolledWindow
{
   protected:
      wxPoint        mMouseDownPoint;
      wxPoint        mLastMousePoint;
      Connection*    mActiveConnection;
      Node*          mHoverNode;
      Node*          mSelectedNode;
      MaterialsTool* mMaterialsTool;
      MaterialAsset* mMaterialAsset;
      bool           mRightMouseDown;
      bool           mRightMouseDrag;

      float          mWindowX;
      float          mWindowY;

   public:
      Vector<Node> nodeList;
      Vector<Connection> connectionList;

      MaterialWindow(wxWindow* parent, MaterialsTool* matTool);

      void loadMaterial(MaterialAsset* mat);
      void saveMaterial();

      MaterialAsset* getMaterialAsset() { return mMaterialAsset; }
      Node* findNode(const char* name);
      wxString getUniqueNodeName(wxString name);
      Connection* findConnectionFromOutput(const char* name, U32 index = -1);
      Connection* findConnectionFromInput(const char* name, U32 index = -1);

      void  addConnection(const char* output, U32 outputIndex, const char* input, U32 inputIndex);
      Node* addOpaqueNode(Materials::BaseNode* node = NULL);
      Node* addFloatNode(Materials::BaseNode* node = NULL);
      Node* addVec2Node(Materials::BaseNode* node = NULL);
      Node* addVec3Node(Materials::BaseNode* node = NULL);
      Node* addVec4Node(Materials::BaseNode* node = NULL);
      Node* addTextureNode(Materials::BaseNode* node = NULL);
      Node* addTimeNode(Materials::BaseNode* node = NULL);
      Node* addCosNode(Materials::BaseNode* node = NULL);
      Node* addSinNode(Materials::BaseNode* node = NULL);
      Node* addMultiplyNode(Materials::BaseNode* node = NULL);
      Node* addLerpNode(Materials::BaseNode* node = NULL);
      void  addNode(Materials::MaterialTemplate* matTemplate, const char* type, Materials::BaseNode* node = NULL);
      void  addNodeConnection(Node* node);
      void  deleteNode(Materials::MaterialTemplate* matTemplate, Node* node);
      void  saveConnection(Connection* connection);
      void  saveNode(Materials::MaterialTemplate* matTemplate, Node* node);
      void  drawNode(wxGCDC& gdc, Node* node);
      void  drawConnection(wxGCDC& gdc, wxGraphicsContext* context, Connection* conn);

      void OnPaint(wxPaintEvent &evt);
      void OnMouseMove(wxMouseEvent &evt);
      void OnMouseDown(wxMouseEvent &evt);
      void OnMouseUp(wxMouseEvent &evt);
      void OnRightMouseDown(wxMouseEvent &evt);
      void OnRightMouseUp(wxMouseEvent &evt);
      void OnMenuEvent( wxCommandEvent& evt );

private:
    wxDECLARE_EVENT_TABLE();
};
 
#endif // _MATERIAL_WINDOW_H_