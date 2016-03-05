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

#include "materialWindow.h"

void MaterialWindow::addConnection(const char* output, U32 outputIndex, const char* input, U32 inputIndex)
{
   if ( output == NULL || input == NULL ) 
      return;
   if ( dStrlen(output) < 1 || dStrlen(input) < 1 )
      return;

   Connection newConnection;
   newConnection.outputNodeName = output;
   newConnection.outputIndex = outputIndex;
   newConnection.inputNodeName = input;
   newConnection.inputIndex = inputIndex;
   connectionList.push_back(newConnection);
}

Node* MaterialWindow::addOpaqueNode(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("Opaque");
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.type = "Opaque";
   newNode.width = 180.0f;
   newNode.height = 170.0f;
   newNode.addInput("Base Color");
   newNode.addInput("Normals");
   newNode.addInput("Metallic");
   newNode.addInput("Roughness");
   newNode.addInput("Emissive");
   newNode.addInput("World Position Offset");

   if (node != NULL)
   {
      Materials::OpaqueNode* def_node = dynamic_cast<Materials::OpaqueNode*>(node);
      if (def_node)
         newNode.alphaThreshold = def_node->mAlphaThreshold;
   }
   
   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addFloatNode(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewFloat");
   newNode.type = "Float";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.height = 70.0f;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   
   newNode.color.set(0.0f, 0.0f, 0.0f, 0.0f);
   if ( node != NULL )
   {
      Materials::FloatNode* float_node = dynamic_cast<Materials::FloatNode*>(node);
      if ( float_node )
         newNode.color = ColorF(float_node->mValue, 0.0f, 0.0f, 0.0f);
   }

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addVec2Node(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewVec2");
   newNode.type = "Vec2";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.height = 110.0f;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   newNode.addOutput(ColorI(255, 50, 50, 255));
   newNode.addOutput(ColorI(50, 255, 50, 255));

   newNode.showColor = true;
   newNode.color.set(0.0, 0.0, 0.0, 1.0);
   if (node != NULL)
   {
      Materials::Vec2Node* vec2_node = dynamic_cast<Materials::Vec2Node*>(node);
      if (vec2_node)
         newNode.color = ColorF(vec2_node->mValue.x, vec2_node->mValue.y, 0.0, 1.0);
   }

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addVec3Node(Materials::BaseNode* node)
{
   Node newNode;
   newNode.height = 130.0f;
   newNode.name = getUniqueNodeName("NewVec3");
   newNode.type = "Vec3";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   newNode.addOutput(ColorI(255, 50, 50, 255));
   newNode.addOutput(ColorI(50, 255, 50, 255));
   newNode.addOutput(ColorI(50, 50, 255, 255));

   newNode.showColor = true;
   newNode.color.set(0, 0, 0, 255);
   if ( node != NULL )
   {
      Materials::Vec3Node* vec3_node = dynamic_cast<Materials::Vec3Node*>(node);
      if ( vec3_node )
         newNode.color = ColorF(vec3_node->mValue.x, vec3_node->mValue.y, vec3_node->mValue.z, 1.0);
   }
   
   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addVec4Node(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewVec4");
   newNode.type = "Vec4";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   newNode.addOutput(ColorI(255, 50, 50, 255));
   newNode.addOutput(ColorI(50, 255, 50, 255));
   newNode.addOutput(ColorI(50, 50, 255, 255));
   newNode.addOutput(ColorI(200, 200, 200, 255));

   newNode.showColor = true;
   newNode.color.set(0.0, 0.0, 0.0, 1.0);
   if (node != NULL)
   {
      Materials::Vec4Node* vec4_node = dynamic_cast<Materials::Vec4Node*>(node);
      if (vec4_node)
         newNode.color = ColorF(vec4_node->mValue.x, vec4_node->mValue.y, vec4_node->mValue.z, vec4_node->mValue.w);
   }

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addTextureNode(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewTexture");
   newNode.type = "Texture";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.width = 135.0f;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   newNode.addOutput(ColorI(255, 50, 50, 255));
   newNode.addOutput(ColorI(50, 255, 50, 255));
   newNode.addOutput(ColorI(50, 50, 255, 255));
   newNode.addOutput(ColorI(200, 200, 200, 255));

   if ( node != NULL )
   {
      Materials::TextureNode* tex_node = dynamic_cast<Materials::TextureNode*>(node);
      if ( tex_node )
      {
         newNode.textureSlot = tex_node->mSlot;
         //bgfx::TextureHandle* tex = getTextureHandle(tex_node->mSlot);
         //if ( tex != NULL )
         //{
         //   newNode.texture = *tex;
         //   newNode.showTexture = true;
         //}
      }
   }
   
   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addTimeNode(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewTime");
   newNode.type = "Time";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.height = 70.0f;
   newNode.addOutput(ColorI(255, 255, 255, 255));

   newNode.color.set(0.0f, 0.0f, 0.0f, 0.0f);
   if (node != NULL)
   {
      Materials::TimeNode* time_node = dynamic_cast<Materials::TimeNode*>(node);
      if (time_node)
         newNode.color = ColorF(time_node->mMultiplier, 0.0f, 0.0f, 0.0f);
   }

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addCosNode(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewCos");
   newNode.type = "Cos";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.height = 70.0f;
   newNode.addInput("x");
   newNode.addOutput(ColorI(255, 255, 255, 255));

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addSinNode(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewSin");
   newNode.type = "Sin";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.height = 70.0f;
   newNode.addInput("x");
   newNode.addOutput(ColorI(255, 255, 255, 255));

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addMultiplyNode(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewMultiply");
   newNode.type = "Multiply";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.width = 120.0f;
   newNode.height = 90.0f;
   newNode.addInput("a");
   newNode.addInput("b");
   newNode.addOutput(ColorI(255, 255, 255, 255));

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addLerpNode(Materials::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewLerp");
   newNode.type = "Lerp";
   newNode.x = -mWindowX + mLastMousePoint.x;
   newNode.y = -mWindowY + mLastMousePoint.y;
   newNode.width = 120.0f;
   newNode.height = 110.0f;
   newNode.addInput("a");
   newNode.addInput("b");
   newNode.addInput("amount");
   if (node != NULL)
   {
      Materials::LerpNode* lerp_node = dynamic_cast<Materials::LerpNode*>(node);
      if (lerp_node)
         newNode.color = ColorF(lerp_node->mAmount, 0.0f, 0.0f, 0.0f);
   }
   newNode.addOutput(ColorI(255, 255, 255, 255));

   nodeList.push_back(newNode);
   return &nodeList.back();
}

void MaterialWindow::addNode(Materials::MaterialTemplate* matTemplate, const char* type, Materials::BaseNode* node)
{
   if ( !matTemplate ) 
      return;

   Node* newNode = NULL;
   Materials::BaseNode* matNode = node;
   bool createMaterialNode = (matNode == NULL);

   // Outputs
   if ( dStrcmp(type, "Opaque") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Materials::OpaqueNode();
      newNode = addOpaqueNode(matNode);
   }

   // Inputs
   if ( dStrcmp(type, "Float") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Materials::FloatNode();
      newNode = addFloatNode(matNode);
   }
   if ( dStrcmp(type, "Vec2") == 0 )
   {
      if (createMaterialNode) matNode = new Materials::Vec2Node();
      newNode = addVec2Node(matNode);
   }
   if ( dStrcmp(type, "Vec3") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Materials::Vec3Node();
      newNode = addVec3Node(matNode);
   }
   if ( dStrcmp(type, "Vec4") == 0 ) 
   {
      if (createMaterialNode) matNode = new Materials::Vec4Node();
      newNode = addVec4Node(matNode);
   }
   if ( dStrcmp(type, "Texture") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Materials::TextureNode();
      newNode = addTextureNode(matNode);
   }
   if ( dStrcmp(type, "Time") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Materials::TimeNode();
      newNode = addTimeNode(matNode);
   }

   // Math
   if ( dStrcmp(type, "Multiply") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Materials::MultiplyNode();
      newNode = addMultiplyNode(matNode);
   }
   if (dStrcmp(type, "Lerp") == 0)
   {
      if (createMaterialNode) matNode = new Materials::LerpNode();
      newNode = addLerpNode(matNode);
   }
   if ( dStrcmp(type, "Sin") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Materials::SinNode();
      newNode = addSinNode(matNode);
   }
   if ( dStrcmp(type, "Cos") == 0 ) 
   {
      if (createMaterialNode) matNode = new Materials::CosNode();
      newNode = addCosNode(matNode);
   }

   // Additional Processing.
   if ( newNode != NULL )
   {
      if ( matNode != NULL )
      {
         // Set the name.
         StringTableEntry node_name = matNode->getInternalName();
         if ( node_name && dStrcmp(newNode->name, node_name) != 0 ) 
            newNode->name = getUniqueNodeName(node_name);

         // No internal name set? Use the one that comes with the new node.
         if (!node_name)
            matNode->setInternalName(newNode->name);

         // Link it to the material node and set position.
         newNode->materialNode = matNode;
      }

      // If we created a new material node we need to add it to the
      // template and name it.
      if ( createMaterialNode && newNode->materialNode != NULL )
      {
         newNode->materialNode->registerObject();
         newNode->materialNode->setInternalName(newNode->name);
         matTemplate->addObject(newNode->materialNode);
      }
      else {
         // Load Position.
         newNode->x = matNode->mPosition.x;
         newNode->y = matNode->mPosition.y;
      }
   }
}

void MaterialWindow::addNodeConnection(Node* node)
{
   if ( node->materialNode == NULL )
      return;

   // Opaque
   Materials::OpaqueNode* def_node = dynamic_cast<Materials::OpaqueNode*>(node->materialNode);
   if ( def_node )
   {
      addConnection(def_node->mColorSrc, 0, node->name, 0);
      addConnection(def_node->mNormalSrc, 0, node->name, 1);
      addConnection(def_node->mMetallicSrc, 0, node->name, 2);
      addConnection(def_node->mRoughnessSrc, 0, node->name, 3);
      addConnection(def_node->mEmissiveSrc, 0, node->name, 4);
      addConnection(def_node->mWorldPosOffsetSrc, 0, node->name, 5);
      return;
   }

   // Multiply
   Materials::MultiplyNode* mul_node = dynamic_cast<Materials::MultiplyNode*>(node->materialNode);
   if ( mul_node )
   {
      addConnection(mul_node->mInputASrc, 0, node->name, 0);
      addConnection(mul_node->mInputBSrc, 0, node->name, 1);
      return;
   }

   // Lerp
   Materials::LerpNode* lerp_node = dynamic_cast<Materials::LerpNode*>(node->materialNode);
   if (lerp_node)
   {
      addConnection(lerp_node->mInputASrc, 0, node->name, 0);
      addConnection(lerp_node->mInputBSrc, 0, node->name, 1);
      addConnection(lerp_node->mAmountSrc, 0, node->name, 2);
      return;
   }

   // Sin
   Materials::SinNode* sin_node = dynamic_cast<Materials::SinNode*>(node->materialNode);
   if ( sin_node )
   {
      addConnection(sin_node->mXSrc, 0, node->name, 0);
      return;
   }

   // Cos
   Materials::CosNode* cos_node = dynamic_cast<Materials::CosNode*>(node->materialNode);
   if (sin_node)
   {
      addConnection(cos_node->mXSrc, 0, node->name, 0);
      return;
   }
}

void MaterialWindow::saveConnection(Connection* connection)
{
   Node* outputNode = findNode(connection->outputNodeName);
   Node* inputNode = findNode(connection->inputNodeName);

   if ( outputNode == NULL || inputNode == NULL )
      return;

   if ( outputNode->materialNode == NULL )
      return;

   if ( inputNode->materialNode == NULL )
      return;

   // Opaque
   Materials::OpaqueNode* def_node = dynamic_cast<Materials::OpaqueNode*>(inputNode->materialNode);
   if ( def_node )
   {
      if ( connection->inputIndex == 0 )
         def_node->mColorSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 1 )
         def_node->mNormalSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 2 )
         def_node->mMetallicSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 3 )
         def_node->mRoughnessSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 4 )
         def_node->mEmissiveSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 5 )
         def_node->mWorldPosOffsetSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      return;
   }

   // Multiply
   Materials::MultiplyNode* mul_node = dynamic_cast<Materials::MultiplyNode*>(inputNode->materialNode);
   if ( mul_node )
   {
      if ( connection->inputIndex == 0 )
         mul_node->mInputASrc = Torque::StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 1 )
         mul_node->mInputBSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      return;
   }

   // Lerp
   Materials::LerpNode* lerp_node = dynamic_cast<Materials::LerpNode*>(inputNode->materialNode);
   if (lerp_node)
   {
      if (connection->inputIndex == 0)
         lerp_node->mInputASrc = Torque::StringTableLink->insert(connection->outputNodeName);
      if (connection->inputIndex == 1)
         lerp_node->mInputBSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      if (connection->inputIndex == 2)
         lerp_node->mAmountSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      return;
   }

   // Sin
   Materials::SinNode* sin_node = dynamic_cast<Materials::SinNode*>(inputNode->materialNode);
   if ( sin_node )
   {
      if ( connection->inputIndex == 0 )
         sin_node->mXSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      return;
   }

   // Cos
   Materials::CosNode* cos_node = dynamic_cast<Materials::CosNode*>(inputNode->materialNode);
   if (cos_node)
   {
      if (connection->inputIndex == 0)
         cos_node->mXSrc = Torque::StringTableLink->insert(connection->outputNodeName);
      return;
   }
}

void MaterialWindow::saveNode(Materials::MaterialTemplate* matTemplate, Node* node)
{
   // Opaque
   Materials::OpaqueNode* def_node = dynamic_cast<Materials::OpaqueNode*>(node->materialNode);
   if ( def_node )
   {
      def_node->mColorSrc           = Torque::StringTableLink->EmptyString;
      def_node->mNormalSrc          = Torque::StringTableLink->EmptyString;
      def_node->mMetallicSrc        = Torque::StringTableLink->EmptyString;
      def_node->mRoughnessSrc       = Torque::StringTableLink->EmptyString;
      def_node->mEmissiveSrc        = Torque::StringTableLink->EmptyString;
      def_node->mWorldPosOffsetSrc  = Torque::StringTableLink->EmptyString;
      def_node->mAlphaThreshold     = node->alphaThreshold;
      return;
   }

   // Texture
   Materials::TextureNode* texture_node = dynamic_cast<Materials::TextureNode*>(node->materialNode);
   if (texture_node)
   {
      texture_node->mSlot = node->textureSlot;
      return;
   }

   // Multiply
   Materials::MultiplyNode* mul_node = dynamic_cast<Materials::MultiplyNode*>(node->materialNode);
   if ( mul_node )
   {
      mul_node->mInputASrc = Torque::StringTableLink->EmptyString;
      mul_node->mInputBSrc = Torque::StringTableLink->EmptyString;
      return;
   }

   // Lerp
   Materials::LerpNode* lerp_node = dynamic_cast<Materials::LerpNode*>(node->materialNode);
   if (lerp_node)
   {
      lerp_node->mInputASrc = Torque::StringTableLink->EmptyString;
      lerp_node->mInputBSrc = Torque::StringTableLink->EmptyString;
      lerp_node->mAmountSrc = Torque::StringTableLink->EmptyString;
      return;
   }

   // Sin
   Materials::SinNode* sin_node = dynamic_cast<Materials::SinNode*>(node->materialNode);
   if ( sin_node )
   {
      sin_node->mXSrc = Torque::StringTableLink->EmptyString;
      return;
   }

   // Cos
   Materials::CosNode* cos_node = dynamic_cast<Materials::CosNode*>(node->materialNode);
   if (cos_node)
   {
      cos_node->mXSrc = Torque::StringTableLink->EmptyString;
      return;
   }

   // Float
   Materials::FloatNode* float_node = dynamic_cast<Materials::FloatNode*>(node->materialNode);
   if ( float_node )
   {
      float_node->mValue = node->color.red;
      return;
   }

   // Time
   Materials::TimeNode* time_node = dynamic_cast<Materials::TimeNode*>(node->materialNode);
   if (time_node)
   {
      time_node->mMultiplier = node->color.red;
      return;
   }

   // Vec2
   Materials::Vec2Node* vec2_node = dynamic_cast<Materials::Vec2Node*>(node->materialNode);
   if (vec2_node)
   {
      vec2_node->mValue.set(node->color.red, node->color.green);
      return;
   }

   // Vec3
   Materials::Vec3Node* vec3_node = dynamic_cast<Materials::Vec3Node*>(node->materialNode);
   if ( vec3_node )
   {
      vec3_node->mValue.set(node->color.red, node->color.green, node->color.blue);
      return;
   }

   // Vec4
   Materials::Vec4Node* vec4_node = dynamic_cast<Materials::Vec4Node*>(node->materialNode);
   if (vec4_node)
   {
      vec4_node->mValue.set(node->color.red, node->color.green, node->color.blue, node->color.alpha);
      return;
   }
}

void MaterialWindow::deleteNode(Materials::MaterialTemplate* matTemplate, Node* targetNode)
{
   // Erase any connections.
   Vector<Connection> newConnectionList;
   for (S32 n = 0; n < connectionList.size(); ++n)
   {
      Connection* connection = &connectionList[n];
      if (dStrcmp(connection->inputNodeName, targetNode->name) != 0 && dStrcmp(connection->outputNodeName, targetNode->name) != 0)
         newConnectionList.push_back(*connection);
   }
   connectionList.clear();
   connectionList.merge(newConnectionList);
   mActiveConnection = NULL;

   // Erase from Template
   if (matTemplate != NULL && targetNode->materialNode != NULL)
      matTemplate->removeObject(targetNode->materialNode);

   // Erase the Node
   Vector<Node> newNodeList;
   for (S32 n = 0; n < nodeList.size(); ++n)
   {
      Node* node = &nodeList[n];
      if (node != targetNode)
         newNodeList.push_back(*node);
   }
   nodeList.clear();
   nodeList.merge(newNodeList);
}