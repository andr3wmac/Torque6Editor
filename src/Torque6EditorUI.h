///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TORQUE6EDITORUI_H__
#define __TORQUE6EDITORUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/treectrl.h>
#include <wx/aui/auibook.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/dataview.h>
#include <wx/bmpbuttn.h>
#include <wx/srchctrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/stattext.h>
#include <wx/clrpicker.h>
#include <wx/notebook.h>
#include <wx/scrolwin.h>

///////////////////////////////////////////////////////////////////////////

#define TOOLBAR_PROJECT 1000
#define TOOLBAR_CONSOLE 1001
#define TOOLBAR_SCENE 1002
#define TOOLBAR_SCRIPTS 1003
#define TOOLBAR_MATERIALS 1004
#define TOOLBAR_PROFILER 1005
#define PROFILER_START 1006

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenu* m_menu1;
		wxToolBarToolBase* m_tool1; 
		wxToolBarToolBase* m_tool2; 
		wxToolBarToolBase* m_tool3; 
		wxToolBarToolBase* m_tool4; 
		wxToolBarToolBase* m_tool5; 
		wxToolBarToolBase* m_tool7; 
	
	public:
		wxMenuBar* mainMenuBar;
		wxMenuItem* m_menuItem1;
		wxToolBar* mainToolbar;
		wxPanel* mainPanel;
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Torque 6 Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,768 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;
		
		~MainFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ScriptsPanel
///////////////////////////////////////////////////////////////////////////////
class ScriptsPanel : public wxPanel 
{
	private:
	
	protected:
	
	public:
		wxTreeCtrl* m_scriptsTree;
		wxAuiNotebook* m_scriptsNotebook;
		
		ScriptsPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); wxAuiManager m_mgr;
		
		~ScriptsPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ProfilerPanel
///////////////////////////////////////////////////////////////////////////////
class ProfilerPanel : public wxPanel 
{
	private:
	
	protected:
		wxPanel* m_profilerMenu;
		wxButton* m_button1;
		wxChoice* m_choice1;
		wxPanel* m_mainPanel;
	
	public:
		wxDataViewCtrl* ProfilerDataView;
		
		ProfilerPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); wxAuiManager m_mgr;
		
		~ProfilerPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ScenePanel
///////////////////////////////////////////////////////////////////////////////
class ScenePanel : public wxPanel 
{
	private:
	
	protected:
		wxButton* m_button21;
		wxButton* m_button2;
		wxButton* m_button4;
		wxNotebook* m_notebook2;
		wxPanel* m_panel11;
		wxBitmapButton* m_bpButton1;
		wxBitmapButton* m_bpButton2;
		wxSearchCtrl* m_searchCtrl1;
		wxPanel* m_panel12;
		wxStaticText* m_staticText1;
		wxColourPickerCtrl* m_colourPicker1;
	
	public:
		wxTreeCtrl* entityList;
		wxPropertyGrid* propertyGrid;
		
		ScenePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 367,509 ), long style = wxTAB_TRAVERSAL ); 
		~ScenePanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MaterialsPanel
///////////////////////////////////////////////////////////////////////////////
class MaterialsPanel : public wxPanel 
{
	private:
	
	protected:
		wxPanel* m_panel;
		wxBitmapButton* m_bpButton1;
		wxScrolledWindow* m_scrolledWindow1;
		wxButton* m_button1;
		wxButton* m_button2;
		wxButton* m_button3;
		wxButton* m_button4;
		wxButton* m_button5;
		wxButton* m_button6;
		wxButton* m_button7;
		wxButton* m_button8;
		wxButton* m_button9;
		wxButton* m_button10;
	
	public:
		wxTreeCtrl* m_materialTree;
		wxAuiNotebook* m_materialNotebook;
		
		MaterialsPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 498,301 ), long style = wxTAB_TRAVERSAL ); wxAuiManager m_mgr;
		
		~MaterialsPanel();
	
};

#endif //__TORQUE6EDITORUI_H__
