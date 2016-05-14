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

#include <wx/dir.h>
#include <wx/treectrl.h>

// UI generated from wxFormBuilder
#include "../Torque6EditorUI.h"

#include "consoleTool.h"

ConsoleTool::ConsoleTool(EditorManager* _EditorManager, MainFrame* _frame, wxAuiManager* _manager)
   : Parent(_EditorManager, _frame, _manager),
     mConsolePanel(NULL)
{

}

ConsoleTool::~ConsoleTool()
{

}

void ConsoleTool::initTool()
{
   // Create panel.
   mConsolePanel = new ConsolePanel(mFrame, wxID_ANY);
   mManager->AddPane(mConsolePanel, wxAuiPaneInfo().Caption("Console")
                                                   .CaptionVisible( true )
                                                   .CloseButton( true )
                                                   .PinButton( true )
                                                   .MaximizeButton(true)
                                                   .Dock()
                                                   .Resizable()
                                                   .FloatingSize( wxDefaultSize )
                                                   .Bottom()
                                                   .Hide());
   mManager->Update();
}

void ConsoleTool::openTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mConsolePanel);
   paneInfo.Show();
   mManager->Update();
}

void ConsoleTool::closeTool()
{
   wxAuiPaneInfo& paneInfo = mManager->GetPane(mConsolePanel);
   paneInfo.Hide();
   mManager->Update();
}

void ConsoleTool::onProjectLoaded(const wxString& projectName, const wxString& projectPath)
{
   if ( !mOpen ) return;
}

void ConsoleTool::onProjectClosed()
{
   if ( !mOpen ) return;
}