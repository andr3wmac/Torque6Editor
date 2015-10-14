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
	
	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu2, MENU_PROJECT, wxString( wxT("Project") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem5 );
	
	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_menu2, MENU_CONSOLE, wxString( wxT("Console") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem6 );
	
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu2, MENU_SCENE, wxString( wxT("Scene") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem7 );
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu2, MENU_SCRIPTS, wxString( wxT("Scripts") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem8 );
	
	wxMenuItem* m_menuItem9;
	m_menuItem9 = new wxMenuItem( m_menu2, MENU_MATERIALS, wxString( wxT("Materials") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem9 );
	
	wxMenuItem* m_menuItem10;
	m_menuItem10 = new wxMenuItem( m_menu2, MENU_PROFILER, wxString( wxT("Profiler") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem10 );
	
	mainMenuBar->Append( m_menu2, wxT("Tools") ); 
	
	this->SetMenuBar( mainMenuBar );
	
	mainToolbar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL ); 
	mainToolbar->Realize();
	m_mgr.AddPane( mainToolbar, wxAuiPaneInfo() .Top() .CaptionVisible( false ).CloseButton( false ).Gripper().Dock().Resizable().FloatingSize( wxSize( 37,57 ) ).MinSize( wxSize( -1,39 ) ).Layer( 1 ) );
	
	
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
	
	m_button21 = new wxButton( this, SCENE_NEW, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button21, 1, wxALL, 5 );
	
	m_button2 = new wxButton( this, SCENE_OPEN, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button2, 1, wxALL|wxEXPAND, 5 );
	
	m_button4 = new wxButton( this, SCENE_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button4, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel11 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bpButton1 = new wxBitmapButton( m_panel11, ADD_ENTITY_BUTTON, wxBitmap( wxT("images/entityIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
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
	
	entityList = new wxTreeCtrl( m_panel11, ENTITY_LIST, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	bSizer5->Add( entityList, 1, wxALL|wxEXPAND, 1 );
	
	propertyGrid = new wxPropertyGrid(m_panel11, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer5->Add( propertyGrid, 1, wxALL|wxEXPAND, 2 );
	
	
	m_panel11->SetSizer( bSizer5 );
	m_panel11->Layout();
	bSizer5->Fit( m_panel11 );
	m_notebook2->AddPage( m_panel11, wxT("Objects"), false );
	m_panel12 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bpButton11 = new wxBitmapButton( m_panel12, ADD_FEATURE_BUTTON, wxBitmap( wxT("images/featureIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer61->Add( m_bpButton11, 0, wxALL, 2 );
	
	
	bSizer51->Add( bSizer61, 0, wxEXPAND, 0 );
	
	featureList = new wxTreeCtrl( m_panel12, FEATURE_LIST, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	bSizer51->Add( featureList, 1, wxALL|wxEXPAND, 1 );
	
	featurePropGrid = new wxPropertyGrid(m_panel12, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer51->Add( featurePropGrid, 1, wxALL|wxEXPAND, 2 );
	
	
	m_panel12->SetSizer( bSizer51 );
	m_panel12->Layout();
	bSizer51->Fit( m_panel12 );
	addFeatureMenu = new wxMenu();
	wxMenuItem* m_menuItem14;
	m_menuItem14 = new wxMenuItem( addFeatureMenu, ADD_FEATURE_DLAA, wxString( wxT("DLAA") ) , wxEmptyString, wxITEM_NORMAL );
	addFeatureMenu->Append( m_menuItem14 );
	
	wxMenuItem* m_menuItem15;
	m_menuItem15 = new wxMenuItem( addFeatureMenu, ADD_FEATURE_SSAO, wxString( wxT("SSAO") ) , wxEmptyString, wxITEM_NORMAL );
	addFeatureMenu->Append( m_menuItem15 );
	
	wxMenuItem* m_menuItem16;
	m_menuItem16 = new wxMenuItem( addFeatureMenu, ADD_FEATURE_HDR, wxString( wxT("HDR") ) , wxEmptyString, wxITEM_NORMAL );
	addFeatureMenu->Append( m_menuItem16 );
	
	wxMenuItem* m_menuItem17;
	m_menuItem17 = new wxMenuItem( addFeatureMenu, ADD_FEATURE_SKYBOX, wxString( wxT("Simple Skybox") ) , wxEmptyString, wxITEM_NORMAL );
	addFeatureMenu->Append( m_menuItem17 );
	
	wxMenuItem* m_menuItem18;
	m_menuItem18 = new wxMenuItem( addFeatureMenu, ADD_FEATURE_DIRLIGHT, wxString( wxT("Directional Light") ) , wxEmptyString, wxITEM_NORMAL );
	addFeatureMenu->Append( m_menuItem18 );
	
	m_panel12->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ScenePanel::m_panel12OnContextMenu ), NULL, this ); 
	
	m_notebook2->AddPage( m_panel12, wxT("Features"), true );
	
	bSizer3->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
	translateMenu = new wxMenu();
	wxMenuItem* m_menuItem114;
	m_menuItem114 = new wxMenuItem( translateMenu, TRANSLATE_SNAP_NONE, wxString( wxT("Snap: None") ) , wxEmptyString, wxITEM_RADIO );
	translateMenu->Append( m_menuItem114 );
	
	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( translateMenu, TRANSLATE_SNAP_1, wxString( wxT("Snap: 0.1") ) , wxEmptyString, wxITEM_RADIO );
	translateMenu->Append( m_menuItem11 );
	
	wxMenuItem* m_menuItem111;
	m_menuItem111 = new wxMenuItem( translateMenu, TRANSLATE_SNAP_2, wxString( wxT("Snap: 0.25") ) , wxEmptyString, wxITEM_RADIO );
	translateMenu->Append( m_menuItem111 );
	
	wxMenuItem* m_menuItem112;
	m_menuItem112 = new wxMenuItem( translateMenu, TRANSLATE_SNAP_3, wxString( wxT("Snap: 0.5") ) , wxEmptyString, wxITEM_RADIO );
	translateMenu->Append( m_menuItem112 );
	
	wxMenuItem* m_menuItem113;
	m_menuItem113 = new wxMenuItem( translateMenu, TRANSLATE_SNAP_4, wxString( wxT("Snap: 1.0") ) , wxEmptyString, wxITEM_RADIO );
	translateMenu->Append( m_menuItem113 );
	
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ScenePanel::ScenePanelOnContextMenu ), NULL, this ); 
	
}

ScenePanel::~ScenePanel()
{
	delete addFeatureMenu; 
	delete translateMenu; 
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
	
	m_bpButton1 = new wxBitmapButton( m_panel, MATERIAL_SAVE, wxBitmap( wxT("images/saveIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer2->Add( m_bpButton1, 0, wxALL, 2 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	m_materialTree = new wxTreeCtrl( m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	bSizer1->Add( m_materialTree, 1, wxALL|wxEXPAND, 2 );
	
	propertyGrid = new wxPropertyGrid(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer1->Add( propertyGrid, 1, wxALL|wxEXPAND, 2 );
	
	
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

ProjectPanel::ProjectPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel11 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	
	m_panel11->SetSizer( bSizer5 );
	m_panel11->Layout();
	bSizer5->Fit( m_panel11 );
	m_notebook2->AddPage( m_panel11, wxT("Project"), false );
	m_panel12 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bpButton11 = new wxBitmapButton( m_panel12, wxID_ANY, wxBitmap( wxT("images/featureIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer61->Add( m_bpButton11, 0, wxALL, 2 );
	
	
	bSizer51->Add( bSizer61, 0, wxEXPAND, 0 );
	
	assetList = new wxTreeCtrl( m_panel12, ASSET_LIST, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	bSizer51->Add( assetList, 1, wxALL|wxEXPAND, 1 );
	
	assetPropGrid = new wxPropertyGrid(m_panel12, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer51->Add( assetPropGrid, 1, wxALL|wxEXPAND, 2 );
	
	
	m_panel12->SetSizer( bSizer51 );
	m_panel12->Layout();
	bSizer51->Fit( m_panel12 );
	m_notebook2->AddPage( m_panel12, wxT("Assets"), true );
	
	bSizer3->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
	moduleMenu = new wxMenu();
	wxMenuItem* m_menuItem12;
	m_menuItem12 = new wxMenuItem( moduleMenu, MENU_IMPORT_MESH, wxString( wxT("Import Mesh") ) , wxEmptyString, wxITEM_NORMAL );
	moduleMenu->Append( m_menuItem12 );
	
	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem( moduleMenu, MENU_IMPORT_TEXTURE, wxString( wxT("Import Texture") ) , wxEmptyString, wxITEM_NORMAL );
	moduleMenu->Append( m_menuItem13 );
	
	wxMenuItem* m_menuItem121;
	m_menuItem121 = new wxMenuItem( moduleMenu, MENU_NEW_MATERIAL, wxString( wxT("New Material") ) , wxEmptyString, wxITEM_NORMAL );
	moduleMenu->Append( m_menuItem121 );
	
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ProjectPanel::ProjectPanelOnContextMenu ), NULL, this ); 
	
}

ProjectPanel::~ProjectPanel()
{
	delete moduleMenu; 
}

ImportMeshWizard::ImportMeshWizard( wxWindow* parent, wxWindowID id, const wxString& title, const wxBitmap& bitmap, const wxPoint& pos, long style ) 
{
	this->Create( parent, id, title, bitmap, pos, style );
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxWizardPageSimple* m_wizPage2 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage2 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1 = new wxStaticText( m_wizPage2, wxID_ANY, wxT("Mesh File:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer16->Add( m_staticText1, 0, wxALL, 5 );
	
	meshFilePath = new wxFilePickerCtrl( m_wizPage2, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer16->Add( meshFilePath, 0, wxALL|wxEXPAND, 5 );
	
	m_checkBox2 = new wxCheckBox( m_wizPage2, wxID_ANY, wxT("Import Animations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox2->SetValue(true); 
	bSizer16->Add( m_checkBox2, 0, wxALL, 5 );
	
	m_checkBox3 = new wxCheckBox( m_wizPage2, wxID_ANY, wxT("Y-Up"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_checkBox3, 0, wxALL, 5 );
	
	
	m_wizPage2->SetSizer( bSizer16 );
	m_wizPage2->Layout();
	bSizer16->Fit( m_wizPage2 );
	wxWizardPageSimple* m_wizPage21 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage21 );
	
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText111 = new wxStaticText( m_wizPage21, wxID_ANY, wxT("Asset ID (no spaces):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	bSizer161->Add( m_staticText111, 0, wxALL, 5 );
	
	assetID = new wxTextCtrl( m_wizPage21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( assetID, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText121 = new wxStaticText( m_wizPage21, wxID_ANY, wxT("Import To Path:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	bSizer161->Add( m_staticText121, 0, wxALL, 5 );
	
	importPath = new wxDirPickerCtrl( m_wizPage21, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	bSizer161->Add( importPath, 0, wxALL|wxEXPAND, 5 );
	
	copyMeshCheck = new wxCheckBox( m_wizPage21, wxID_ANY, wxT("Copy Mesh File"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( copyMeshCheck, 0, wxALL, 5 );
	
	
	m_wizPage21->SetSizer( bSizer161 );
	m_wizPage21->Layout();
	bSizer161->Fit( m_wizPage21 );
	
	this->Centre( wxBOTH );
	
	for ( unsigned int i = 1; i < m_pages.GetCount(); i++ )
	{
		m_pages.Item( i )->SetPrev( m_pages.Item( i - 1 ) );
		m_pages.Item( i - 1 )->SetNext( m_pages.Item( i ) );
	}
}

ImportMeshWizard::~ImportMeshWizard()
{
	m_pages.Clear();
}

ImportTextureWizard::ImportTextureWizard( wxWindow* parent, wxWindowID id, const wxString& title, const wxBitmap& bitmap, const wxPoint& pos, long style ) 
{
	this->Create( parent, id, title, bitmap, pos, style );
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxWizardPageSimple* m_wizPage2 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage2 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1 = new wxStaticText( m_wizPage2, wxID_ANY, wxT("Texture File:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer16->Add( m_staticText1, 0, wxALL, 5 );
	
	textureFilePath = new wxFilePickerCtrl( m_wizPage2, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer16->Add( textureFilePath, 0, wxALL|wxEXPAND, 5 );
	
	generateMipsCheck = new wxCheckBox( m_wizPage2, wxID_ANY, wxT("Generate Mip-Maps"), wxDefaultPosition, wxDefaultSize, 0 );
	generateMipsCheck->SetValue(true); 
	bSizer16->Add( generateMipsCheck, 0, wxALL, 5 );
	
	
	m_wizPage2->SetSizer( bSizer16 );
	m_wizPage2->Layout();
	bSizer16->Fit( m_wizPage2 );
	wxWizardPageSimple* m_wizPage21 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage21 );
	
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText111 = new wxStaticText( m_wizPage21, wxID_ANY, wxT("Asset ID (no spaces):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	bSizer161->Add( m_staticText111, 0, wxALL, 5 );
	
	assetID = new wxTextCtrl( m_wizPage21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( assetID, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText121 = new wxStaticText( m_wizPage21, wxID_ANY, wxT("Import To Path:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	bSizer161->Add( m_staticText121, 0, wxALL, 5 );
	
	importPath = new wxDirPickerCtrl( m_wizPage21, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	bSizer161->Add( importPath, 0, wxALL|wxEXPAND, 5 );
	
	copyTextureCheck = new wxCheckBox( m_wizPage21, wxID_ANY, wxT("Copy Texture File"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( copyTextureCheck, 0, wxALL, 5 );
	
	
	m_wizPage21->SetSizer( bSizer161 );
	m_wizPage21->Layout();
	bSizer161->Fit( m_wizPage21 );
	
	this->Centre( wxBOTH );
	
	for ( unsigned int i = 1; i < m_pages.GetCount(); i++ )
	{
		m_pages.Item( i )->SetPrev( m_pages.Item( i - 1 ) );
		m_pages.Item( i - 1 )->SetNext( m_pages.Item( i ) );
	}
}

ImportTextureWizard::~ImportTextureWizard()
{
	m_pages.Clear();
}

NewMaterialWizard::NewMaterialWizard( wxWindow* parent, wxWindowID id, const wxString& title, const wxBitmap& bitmap, const wxPoint& pos, long style ) 
{
	this->Create( parent, id, title, bitmap, pos, style );
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxWizardPageSimple* m_wizPage21 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage21 );
	
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText111 = new wxStaticText( m_wizPage21, wxID_ANY, wxT("Asset ID (no spaces):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	bSizer161->Add( m_staticText111, 0, wxALL, 5 );
	
	assetID = new wxTextCtrl( m_wizPage21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( assetID, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText121 = new wxStaticText( m_wizPage21, wxID_ANY, wxT("Save To Path:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	bSizer161->Add( m_staticText121, 0, wxALL, 5 );
	
	savePath = new wxDirPickerCtrl( m_wizPage21, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	bSizer161->Add( savePath, 0, wxALL|wxEXPAND, 5 );
	
	
	m_wizPage21->SetSizer( bSizer161 );
	m_wizPage21->Layout();
	bSizer161->Fit( m_wizPage21 );
	
	this->Centre( wxBOTH );
	
	for ( unsigned int i = 1; i < m_pages.GetCount(); i++ )
	{
		m_pages.Item( i )->SetPrev( m_pages.Item( i - 1 ) );
		m_pages.Item( i - 1 )->SetNext( m_pages.Item( i ) );
	}
}

NewMaterialWizard::~NewMaterialWizard()
{
	m_pages.Clear();
}
