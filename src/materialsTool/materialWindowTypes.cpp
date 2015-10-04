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

using namespace Plugins;

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

Node* MaterialWindow::addDeferredNode(Scene::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("Deferred");
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.type = "Deferred";
   newNode.width = 180.0f;
   newNode.height = 170.0f;
   newNode.addInput("Base Color");
   newNode.addInput("Normals");
   newNode.addInput("Metallic");
   newNode.addInput("Roughness");
   newNode.addInput("Emissive");
   newNode.addInput("World Position Offset");
   
   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addFloatNode(Scene::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewFloat");
   newNode.type = "Float";
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.height = 70.0f;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   
   newNode.color.set(0.0f, 0.0f, 0.0f, 0.0f);
   if ( node != NULL )
   {
      Scene::FloatNode* float_node = dynamic_cast<Scene::FloatNode*>(node);
      if ( float_node )
         newNode.color = ColorF(float_node->mValue, 0.0f, 0.0f, 0.0f);
   }

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addVec2Node(Scene::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewVec2");
   newNode.type = "Vec2";
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.height = 110.0f;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   newNode.addOutput(ColorI(255, 50, 50, 255));
   newNode.addOutput(ColorI(50, 255, 50, 255));

   newNode.showColor = true;
   newNode.color.set(0.0, 0.0, 0.0, 1.0);

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addVec3Node(Scene::BaseNode* node)
{
   Node newNode;
   newNode.height = 130.0f;
   newNode.name = getUniqueNodeName("NewVec3");
   newNode.type = "Vec3";
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   newNode.addOutput(ColorI(255, 50, 50, 255));
   newNode.addOutput(ColorI(50, 255, 50, 255));
   newNode.addOutput(ColorI(50, 50, 255, 255));

   newNode.showColor = true;
   newNode.color.set(0, 0, 0, 255);
   if ( node != NULL )
   {
      Scene::Vec3Node* vec3_node = dynamic_cast<Scene::Vec3Node*>(node);
      if ( vec3_node )
         newNode.color = ColorF(vec3_node->mValue.x, vec3_node->mValue.y, vec3_node->mValue.z, 1.0);
   }
   
   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addVec4Node(Scene::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewVec4");
   newNode.type = "Vec4";
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   newNode.addOutput(ColorI(255, 50, 50, 255));
   newNode.addOutput(ColorI(50, 255, 50, 255));
   newNode.addOutput(ColorI(50, 50, 255, 255));
   newNode.addOutput(ColorI(200, 200, 200, 255));

   newNode.showColor = true;
   newNode.color.set(0.0, 0.0, 0.0, 1.0);

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addTextureNode(Scene::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewTexture");
   newNode.type = "Texture";
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.width = 135.0f;
   newNode.addOutput(ColorI(255, 255, 255, 255));
   newNode.addOutput(ColorI(255, 50, 50, 255));
   newNode.addOutput(ColorI(50, 255, 50, 255));
   newNode.addOutput(ColorI(50, 50, 255, 255));
   newNode.addOutput(ColorI(200, 200, 200, 255));

   if ( node != NULL )
   {
      Scene::TextureNode* tex_node = dynamic_cast<Scene::TextureNode*>(node);
      if ( tex_node )
      {
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

Node* MaterialWindow::addTimeNode(Scene::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewTime");
   newNode.type = "Time";
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.height = 70.0f;
   newNode.addOutput(ColorI(255, 255, 255, 255));

   newNode.color.set(0.0f, 0.0f, 0.0f, 0.0f);
   if (node != NULL)
   {
      Scene::TimeNode* time_node = dynamic_cast<Scene::TimeNode*>(node);
      if (time_node)
         newNode.color = ColorF(time_node->mMultiplier, 0.0f, 0.0f, 0.0f);
   }

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addCosNode(Scene::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewCos");
   newNode.type = "Cos";
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.height = 70.0f;
   newNode.addInput("x");
   newNode.addOutput(ColorI(255, 255, 255, 255));

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addSinNode(Scene::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewSin");
   newNode.type = "Sin";
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.height = 70.0f;
   newNode.addInput("x");
   newNode.addOutput(ColorI(255, 255, 255, 255));

   nodeList.push_back(newNode);
   return &nodeList.back();
}

Node* MaterialWindow::addMultiplyNode(Scene::BaseNode* node)
{
   Node newNode;
   newNode.name = getUniqueNodeName("NewMultiply");
   newNode.type = "Multiply";
   newNode.x = mLastMousePoint.x;
   newNode.y = mLastMousePoint.y;
   newNode.width = 120.0f;
   newNode.height = 90.0f;
   newNode.addInput("a");
   newNode.addInput("b");
   newNode.addOutput(ColorI(255, 255, 255, 255));

   nodeList.push_back(newNode);
   return &nodeList.back();
}

void MaterialWindow::addNode(Scene::MaterialTemplate* matTemplate, const char* type, Scene::BaseNode* node)
{
   if ( !matTemplate ) 
      return;

   Node* newNode = NULL;
   Scene::BaseNode* matNode = node;
   bool createMaterialNode = (matNode == NULL);

   // Outputs
   if ( dStrcmp(type, "Deferred") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Scene::DeferredNode();
      newNode = addDeferredNode(matNode);
   }

   // Inputs
   if ( dStrcmp(type, "Float") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Scene::FloatNode();
      newNode = addFloatNode(matNode);
   }
   if ( dStrcmp(type, "Vec2") == 0 )
   {
      newNode = addVec2Node(matNode);
   }
   if ( dStrcmp(type, "Vec3") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Scene::Vec3Node();
      newNode = addVec3Node(matNode);
   }
   if ( dStrcmp(type, "Vec4") == 0 ) 
   {
      newNode = addVec4Node(matNode);
   }
   if ( dStrcmp(type, "Texture") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Scene::TextureNode();
      newNode = addTextureNode(matNode);
   }
   if ( dStrcmp(type, "Time") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Scene::TimeNode();
      newNode = addTimeNode(matNode);
   }

   // Math
   if ( dStrcmp(type, "Multiply") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Scene::MultiplyNode();
      newNode = addMultiplyNode(matNode);
   }
   if ( dStrcmp(type, "Sin") == 0 ) 
   {
      if ( createMaterialNode ) matNode = new Scene::SinNode();
      newNode = addSinNode(matNode);
   }
   if ( dStrcmp(type, "Cos") == 0 ) 
   {
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
         newNode->x = matNode->mPosition.x;
         newNode->y = matNode->mPosition.y;
      }

      // If we created a new material node we need to add it to the
      // template and name it.
      if ( createMaterialNode && newNode->materialNode != NULL )
      {
         newNode->materialNode->registerObject();
         newNode->materialNode->setInternalName(newNode->name);
         matTemplate->addObject(newNode->materialNode);
      }
   }
}

void MaterialWindow::addNodeConnection(Node* node)
{
   if ( node->materialNode == NULL )
      return;

   // Deferred
   Scene::DeferredNode* def_node = dynamic_cast<Scene::DeferredNode*>(node->materialNode);
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
   Scene::MultiplyNode* mul_node = dynamic_cast<Scene::MultiplyNode*>(node->materialNode);
   if ( mul_node )
   {
      addConnection(mul_node->mInputASrc, 0, node->name, 0);
      addConnection(mul_node->mInputBSrc, 0, node->name, 1);
      return;
   }

   // Sin
   Scene::SinNode* sin_node = dynamic_cast<Scene::SinNode*>(node->materialNode);
   if ( sin_node )
   {
      addConnection(sin_node->mXSrc, 0, node->name, 0);
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

   // Deferred
   Scene::DeferredNode* def_node = dynamic_cast<Scene::DeferredNode*>(inputNode->materialNode);
   if ( def_node )
   {
      if ( connection->inputIndex == 0 )
         def_node->mColorSrc = Link.StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 1 )
         def_node->mNormalSrc = Link.StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 2 )
         def_node->mMetallicSrc = Link.StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 3 )
         def_node->mRoughnessSrc = Link.StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 4 )
         def_node->mEmissiveSrc = Link.StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 5 )
         def_node->mWorldPosOffsetSrc = Link.StringTableLink->insert(connection->outputNodeName);
      return;
   }

   // Multiply
   Scene::MultiplyNode* mul_node = dynamic_cast<Scene::MultiplyNode*>(inputNode->materialNode);
   if ( mul_node )
   {
      if ( connection->inputIndex == 0 )
         mul_node->mInputASrc = Link.StringTableLink->insert(connection->outputNodeName);
      if ( connection->inputIndex == 1 )
         mul_node->mInputBSrc = Link.StringTableLink->insert(connection->outputNodeName);
      return;
   }

   // Sin
   Scene::SinNode* sin_node = dynamic_cast<Scene::SinNode*>(inputNode->materialNode);
   if ( sin_node )
   {
      if ( connection->inputIndex == 0 )
         sin_node->mXSrc = Link.StringTableLink->insert(connection->outputNodeName);
      return;
   }
}

void MaterialWindow::saveNode(Scene::MaterialTemplate* matTemplate, Node* node)
{
   // Deferred
   Scene::DeferredNode* def_node = dynamic_cast<Scene::DeferredNode*>(node->materialNode);
   if ( def_node )
   {
      def_node->mColorSrc = Link.StringTableLink->EmptyString;
      def_node->mNormalSrc = Link.StringTableLink->EmptyString;
      def_node->mMetallicSrc = Link.StringTableLink->EmptyString;
      def_node->mRoughnessSrc = Link.StringTableLink->EmptyString;
      def_node->mEmissiveSrc = Link.StringTableLink->EmptyString;
      def_node->mWorldPosOffsetSrc = Link.StringTableLink->EmptyString;
      return;
   }

   // Texture
   Scene::TextureNode* texture_node = dynamic_cast<Scene::TextureNode*>(node->materialNode);
   if (texture_node)
   {
      texture_node->mSlot = node->textureSlot;
      return;
   }

   // Multiply
   Scene::MultiplyNode* mul_node = dynamic_cast<Scene::MultiplyNode*>(node->materialNode);
   if ( mul_node )
   {
      mul_node->mInputASrc = Link.StringTableLink->EmptyString;
      mul_node->mInputBSrc = Link.StringTableLink->EmptyString;
      return;
   }

   // Sin
   Scene::SinNode* sin_node = dynamic_cast<Scene::SinNode*>(node->materialNode);
   if ( sin_node )
   {
      sin_node->mXSrc = Link.StringTableLink->EmptyString;
      return;
   }

   // Float
   Scene::FloatNode* float_node = dynamic_cast<Scene::FloatNode*>(node->materialNode);
   if ( float_node )
   {
      float_node->mValue = node->color.red;
      return;
   }

   // Time
   Scene::TimeNode* time_node = dynamic_cast<Scene::TimeNode*>(node->materialNode);
   if (time_node)
   {
      time_node->mMultiplier = node->color.red;
      return;
   }

   // Vec3
   Scene::Vec3Node* vec3_node = dynamic_cast<Scene::Vec3Node*>(node->materialNode);
   if ( vec3_node )
   {
      vec3_node->mValue.set(node->color.red, node->color.green, node->color.blue);
      return;
   }
}