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

#ifndef CONSOLE_WINDOW_H
#define CONSOLE_WINDOW_H

#ifndef EDITORMANAGER_H
#include "editorManager.h"
#endif

#ifndef __TORQUE6EDITORUI_H__
#include "Torque6EditorUI.h"
#endif

#ifndef _CONSOLEPANEL_H_
#include "consolePanel.h"
#endif

#ifndef _WX_TREECTRL_H_BASE_
#include <wx/treectrl.h>
#endif

class ConsoleWindow : public wxEvtHandler, public EditorWindow
{
   typedef EditorWindow Parent;

   protected:
      ConsolePanel*  mConsolePanel;

   public:
      ConsoleWindow(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager);
      ~ConsoleWindow();

      virtual void initWindow();
      virtual void openWindow();
      virtual void closeWindow();

      virtual void onProjectLoaded(const wxString& projectName, const wxString& projectPath);
      virtual void onProjectClosed();
};

#endif // CONSOLE_WINDOW_H
