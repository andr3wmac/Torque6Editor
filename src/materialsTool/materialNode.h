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

#ifndef _MATERIAL_NODE_H_
#define _MATERIAL_NODE_H_

#ifndef _PLUGINS_SHARED_H
#include <plugins/plugins_shared.h>
#endif

struct InputPoint
{
   const char* name;

   bool mouseOver;
   wxPoint lastPosition;
};

struct OutputPoint
{
   ColorI color;

   bool mouseOver;
   wxPoint lastPosition;
};

class Node
{
   public:
      wxString type;
      wxString name;
      F32 x;
      F32 y;
      F32 width;
      F32 height;
      bool mouseOver;

      bool showColor;
      ColorF color;

      bool showTexture;
      bgfx::TextureHandle texture;
      S32 textureSlot;

      Scene::BaseNode* materialNode;

      Vector<InputPoint> inputs;
      Vector<OutputPoint> outputs;

      Node()
      {
         texture.idx = bgfx::invalidHandle;
         showTexture = false;
         showColor = false;
         name = "";
         type = "";
         materialNode = NULL;
         x = 500.0f;
         y = 300.0f;
         width = 100;
         height = 150;
         mouseOver = false;
         textureSlot = 0;
      }

      void addInput(const char* name)
      {
         InputPoint inp1;
         inp1.name = name;
         inputs.push_back(inp1);
      }

      void addOutput(ColorI color)
      {
         OutputPoint out1;
         out1.color = color;
         outputs.push_back(out1);
      }
};

struct Connection
{
   const char* outputNodeName;
   U32 outputIndex;
   const char* inputNodeName;
   U32 inputIndex;

   bool outputToMouse;
   bool inputToMouse;

   Connection()
   {
      outputToMouse = false;
      inputToMouse = false;
   }
};
 
#endif // _MATERIAL_NODE_H_