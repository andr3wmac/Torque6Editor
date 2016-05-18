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

#ifndef EDITORTOOL_H
#define EDITORTOOL_H
 
#ifndef _PLUGINS_SHARED_H
#include <plugins/plugins_shared.h>
#endif

#ifndef _PLATFORM_LIBRARY_H_
#include "platform/platformLibrary.h"
#endif

#ifndef _WX_PROPGRID_PROPERTY_H_
#include "wx/propgrid/property.h"
#endif

class EditorManager;
class MainFrame;
class wxAuiManager;

class EditorTool : public wxEvtHandler
{
   public:
      EditorTool(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager);

      bool              mActive;
      EditorManager*    mEditorManager;
      MainFrame*        mFrame;
      wxAuiManager*     mManager;

      virtual void initTool() { }
      virtual void destroyTool() { }
      virtual void activateTool() { mActive = true; }
      virtual void deactivateTool() { mActive = false; }
      virtual void renderTool() { }

      virtual void onSceneChanged() { }
      virtual void onProjectLoaded(wxString projectName, wxString path) {}
      virtual void onProjectClosed() {}

      virtual bool onMouseLeftDown(int x, int y) { return false; }
      virtual bool onMouseLeftUp(int x, int y) { return false; }
      virtual bool onMouseRightDown(int x, int y) { return false; }
      virtual bool onMouseRightUp(int x, int y) { return false; }
      virtual bool onMouseMove(int x, int y) { return false; }
      
      static wxVector<EditorTool*> smEditorTools;
      template <typename T>
      static T* findTool()
      {
         for (S32 n = 0; n < smEditorTools.size(); ++n)
         {
            T* result = dynamic_cast<T*>(smEditorTools[n]);
            if (result)
               return result;
         }

         return NULL;
      }
};
 
#endif // EDITORTOOL_H