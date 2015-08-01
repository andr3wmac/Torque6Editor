///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "src/Torque6EditorUI.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	mainMenuBar = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_OPEN, wxString( wxT("Open Project") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Save Project") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem2 );
	
	m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu1, wxID_CLOSE, wxString( wxT("Close Project") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem3 );
	
	m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem4 );
	
	mainMenuBar->Append( m_menu1, wxT("File") ); 
	
	this->SetMenuBar( mainMenuBar );
	
	mainToolbar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_TEXT ); 
	m_tool1 = mainToolbar->AddTool( TOOLBAR_PROJECT, wxT("Project"), wxBitmap( wxT("images/projectTool.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL ); 
	
	mainToolbar->AddSeparator(); 
	
	m_tool2 = mainToolbar->AddTool( TOOLBAR_CONSOLE, wxT("Console"), wxBitmap( wxT("images/consoleTool.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool3 = mainToolbar->AddTool( TOOLBAR_SCENE, wxT("Scene"), wxBitmap( wxT("images/sceneTool.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool4 = mainToolbar->AddTool( TOOLBAR_SCRIPTS, wxT("Scripts"), wxBitmap( wxT("images/scriptTool.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool5 = mainToolbar->AddTool( TOOLBAR_MATERIALS, wxT("Materials"), wxBitmap( wxT("images/materialsTool.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool7 = mainToolbar->AddTool( TOOLBAR_PROFILER, wxT("Profiler"), wxBitmap( wxT("images/profilerTool.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString, NULL ); 
	
	mainToolbar->Realize();
	m_mgr.AddPane( mainToolbar, wxAuiPaneInfo() .Top() .CaptionVisible( false ).CloseButton( false ).Gripper().Dock().Resizable().FloatingSize( wxSize( 37,57 ) ).Layer( 1 ) );
	
	
	mainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( mainPanel, wxAuiPaneInfo() .Left() .CaptionVisible( false ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).CentrePane() );
	
	
	m_mgr.Update();
	this->Centre( wxBOTH );
}

MainFrame::~MainFrame()
{
	m_mgr.UnInit();
	
}

ScriptsPanel::ScriptsPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	m_scriptsTree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	m_mgr.AddPane( m_scriptsTree, wxAuiPaneInfo() .Left() .Caption( wxT("Script Browser") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 95,93 ) ).Row( 1 ).Layer( 0 ) );
	
	m_scriptsNotebook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE );
	m_mgr.AddPane( m_scriptsNotebook, wxAuiPaneInfo() .Left() .PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).CentrePane() );
	
	
	
	m_mgr.Update();
}

ScriptsPanel::~ScriptsPanel()
{
	m_mgr.UnInit();
	
}

ProfilerPanel::ProfilerPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	m_profilerMenu = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_profilerMenu, wxAuiPaneInfo() .Top() .CaptionVisible( false ).PinButton( true ).Movable( false ).Dock().Resizable().FloatingSize( wxSize( 316,69 ) ).BottomDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ).Row( 2 ).MinSize( wxSize( -1,32 ) ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( m_profilerMenu, PROFILER_START, wxT("Profile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_button1, 0, wxALL, 5 );
	
	wxString m_choice1Choices[] = { wxT("Seconds"), wxT("Milliseconds"), wxT("Microseconds"), wxT("Nanoseconds") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( m_profilerMenu, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 1 );
	bSizer1->Add( m_choice1, 0, wxALL, 5 );
	
	
	m_profilerMenu->SetSizer( bSizer1 );
	m_profilerMenu->Layout();
	bSizer1->Fit( m_profilerMenu );
	m_mainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_mainPanel, wxAuiPaneInfo() .Left() .CaptionVisible( false ).CloseButton( false ).PaneBorder( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).CentrePane() );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	ProfilerDataView = new wxDataViewCtrl( m_mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( ProfilerDataView, 1, wxALL|wxEXPAND, 5 );
	
	
	m_mainPanel->SetSizer( bSizer2 );
	m_mainPanel->Layout();
	bSizer2->Fit( m_mainPanel );
	
	m_mgr.Update();
}

ProfilerPanel::~ProfilerPanel()
{
	m_mgr.UnInit();
	
}

ScenePanel::ScenePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button21 = new wxButton( this, wxID_ANY, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button21, 1, wxALL, 5 );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button2, 1, wxALL|wxEXPAND, 5 );
	
	m_button4 = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button4, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel11 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bpButton1 = new wxBitmapButton( m_panel11, wxID_ANY, wxBitmap( wxT("images/entityIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer6->Add( m_bpButton1, 0, wxALL, 2 );
	
	m_bpButton2 = new wxBitmapButton( m_panel11, wxID_ANY, wxBitmap( wxT("images/componentIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_bpButton2->Enable( false );
	
	bSizer6->Add( m_bpButton2, 0, wxALL, 2 );
	
	m_searchCtrl1 = new wxSearchCtrl( m_panel11, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifndef __WXMAC__
	m_searchCtrl1->ShowSearchButton( true );
	#endif
	m_searchCtrl1->ShowCancelButton( true );
	m_searchCtrl1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 90, false, wxEmptyString ) );
	
	bSizer6->Add( m_searchCtrl1, 1, wxALL, 1 );
	
	
	bSizer5->Add( bSizer6, 0, wxEXPAND, 0 );
	
	entityList = new wxTreeCtrl( m_panel11, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	bSizer5->Add( entityList, 1, wxALL|wxEXPAND, 1 );
	
	propertyGrid = new wxPropertyGrid(m_panel11, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer5->Add( propertyGrid, 1, wxALL|wxEXPAND, 2 );
	
	
	m_panel11->SetSizer( bSizer5 );
	m_panel11->Layout();
	bSizer5->Fit( m_panel11 );
	m_notebook2->AddPage( m_panel11, wxT("Entities"), true );
	m_panel12 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( m_panel12, wxID_ANY, wxT("Directional Light"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText1->Wrap( -1 );
	bSizer11->Add( m_staticText1, 1, wxALL, 5 );
	
	m_colourPicker1 = new wxColourPickerCtrl( m_panel12, wxID_ANY, wxColour( 0, 128, 128 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer11->Add( m_colourPicker1, 0, wxALL, 2 );
	
	
	bSizer10->Add( bSizer11, 1, wxEXPAND, 5 );
	
	
	m_panel12->SetSizer( bSizer10 );
	m_panel12->Layout();
	bSizer10->Fit( m_panel12 );
	m_notebook2->AddPage( m_panel12, wxT("Lighting"), false );
	
	bSizer3->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
}

ScenePanel::~ScenePanel()
{
}

MaterialsPanel::MaterialsPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	m_panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel, wxAuiPaneInfo() .Left() .PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).MinSize( wxSize( 200,-1 ) ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bpButton1 = new wxBitmapButton( m_panel, wxID_ANY, wxBitmap( wxT("images/saveIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer2->Add( m_bpButton1, 0, wxALL, 2 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	m_materialTree = new wxTreeCtrl( m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	bSizer1->Add( m_materialTree, 1, wxALL|wxEXPAND, 2 );
	
	m_scrolledWindow1 = new wxScrolledWindow( m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_button1 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Deferred"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button1, 0, wxALL|wxEXPAND, 2 );
	
	m_button2 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Float"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button2, 0, wxALL|wxEXPAND, 2 );
	
	m_button3 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Vec2"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button3, 0, wxALL|wxEXPAND, 2 );
	
	m_button4 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Vec3"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button4, 0, wxALL|wxEXPAND, 2 );
	
	m_button5 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Vec4"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button5, 0, wxALL|wxEXPAND, 2 );
	
	m_button6 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Texture"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button6, 0, wxALL|wxEXPAND, 2 );
	
	m_button7 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Time"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button7, 0, wxALL|wxEXPAND, 2 );
	
	m_button8 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Cos"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button8, 0, wxALL|wxEXPAND, 2 );
	
	m_button9 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Sin"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button9, 0, wxALL|wxEXPAND, 2 );
	
	m_button10 = new wxButton( m_scrolledWindow1, wxID_ANY, wxT("Multiply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button10, 0, wxALL|wxEXPAND, 2 );
	
	
	m_scrolledWindow1->SetSizer( bSizer3 );
	m_scrolledWindow1->Layout();
	bSizer3->Fit( m_scrolledWindow1 );
	bSizer1->Add( m_scrolledWindow1, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel->SetSizer( bSizer1 );
	m_panel->Layout();
	bSizer1->Fit( m_panel );
	m_materialNotebook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE );
	m_mgr.AddPane( m_materialNotebook, wxAuiPaneInfo() .Left() .PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).CentrePane() );
	
	
	
	m_mgr.Update();
}

MaterialsPanel::~MaterialsPanel()
{
	m_mgr.UnInit();
	
}
