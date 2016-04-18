///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "editor/Torque6EditorUI.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetForegroundColour( wxColour( 45, 45, 45 ) );
	this->SetBackgroundColour( wxColour( 45, 45, 45 ) );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	mainMenuBar = new wxMenuBar( 0 );
	mainMenuBar->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DDKSHADOW ) );
	mainMenuBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DDKSHADOW ) );
	
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu1, MENU_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem4 );
	
	mainMenuBar->Append( m_menu1, wxT("File") ); 
	
	m_menu7 = new wxMenu();
	wxMenuItem* m_menuItem291;
	m_menuItem291 = new wxMenuItem( m_menu7, wxID_ANY, wxString( wxT("Return to (0, 0, 0)") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu7->Append( m_menuItem291 );
	
	m_menu7->AppendSeparator();
	
	wxMenuItem* m_menuItem30;
	m_menuItem30 = new wxMenuItem( m_menu7, MENU_CAMERA_SLOW, wxString( wxT("Speed: Slow") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu7->Append( m_menuItem30 );
	
	wxMenuItem* m_menuItem31;
	m_menuItem31 = new wxMenuItem( m_menu7, MENU_CAMERA_NORMAL, wxString( wxT("Speed: Normal") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu7->Append( m_menuItem31 );
	
	wxMenuItem* m_menuItem32;
	m_menuItem32 = new wxMenuItem( m_menu7, MENU_CAMERA_FAST, wxString( wxT("Speed: Fast") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu7->Append( m_menuItem32 );
	
	mainMenuBar->Append( m_menu7, wxT("Camera") ); 
	
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
	
	mainMenuBar->Append( m_menu2, wxT("Tools") ); 
	
	m_menu71 = new wxMenu();
	wxMenuItem* m_menuItem24;
	m_menuItem24 = new wxMenuItem( m_menu71, MENU_REMOTERY, wxString( wxT("Remotery Profiler") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu71->Append( m_menuItem24 );
	
	mainMenuBar->Append( m_menu71, wxT("Debug") ); 
	
	m_menu6 = new wxMenu();
	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( m_menu6, MENU_WEBSITE, wxString( wxT("Website") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem26 );
	
	wxMenuItem* m_menuItem29;
	m_menuItem29 = new wxMenuItem( m_menu6, MENU_GITHUB, wxString( wxT("GitHub") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem29 );
	
	wxMenuItem* m_menuItem27;
	m_menuItem27 = new wxMenuItem( m_menu6, MENU_FORUMS, wxString( wxT("Forums") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem27 );
	
	m_menu6->AppendSeparator();
	
	wxMenuItem* m_menuItem28;
	m_menuItem28 = new wxMenuItem( m_menu6, MENU_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem28 );
	
	mainMenuBar->Append( m_menu6, wxT("Help") ); 
	
	this->SetMenuBar( mainMenuBar );
	
	mainToolbar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL ); 
	mainToolbar->SetForegroundColour( wxColour( 30, 30, 30 ) );
	mainToolbar->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	
	mainToolbar->Realize();
	m_mgr.AddPane( mainToolbar, wxAuiPaneInfo() .Top() .CaptionVisible( false ).CloseButton( false ).PaneBorder( false ).Dock().Resizable().FloatingSize( wxSize( 37,57 ) ).MinSize( wxSize( -1,39 ) ).Layer( 1 ) );
	
	
	mainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( mainPanel, wxAuiPaneInfo() .Left() .CaptionVisible( false ).CloseButton( false ).PinButton( true ).PaneBorder( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).CentrePane() );
	
	
	m_mgr.Update();
	this->Centre( wxBOTH );
}

MainFrame::~MainFrame()
{
	m_mgr.UnInit();
	
}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 464,265 ), wxSize( 464,265 ) );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap1 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("images/logo.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_bitmap1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Created By:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	m_staticText13->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer23->Add( m_staticText13, 0, wxALL, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("     Andrew Mac ( http://www.andrewmac.ca )"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText14->Wrap( -1 );
	bSizer23->Add( m_staticText14, 0, wxALL, 5 );
	
	m_staticText15 = new wxStaticText( this, wxID_ANY, wxT("Contributors:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	m_staticText15->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer23->Add( m_staticText15, 0, wxALL, 5 );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("     Lukas Jorgensen"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer23->Add( m_staticText16, 0, wxALL, 5 );
	
	
	this->SetSizer( bSizer23 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

AboutDialog::~AboutDialog()
{
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
	this->SetForegroundColour( wxColour( 45, 45, 45 ) );
	this->SetBackgroundColour( wxColour( 45, 45, 45 ) );
	
	ScenePanelContent = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_button21 = new wxButton( this, SCENE_NEW, wxT("New"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_button21->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	m_button21->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	
	bSizer->Add( m_button21, 1, wxALL, 5 );
	
	m_button2 = new wxButton( this, SCENE_OPEN, wxT("Open"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_button2->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	m_button2->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	
	bSizer->Add( m_button2, 1, wxALL|wxEXPAND, 5 );
	
	m_button4 = new wxButton( this, SCENE_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_button4->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNHIGHLIGHT ) );
	m_button4->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	
	bSizer->Add( m_button4, 1, wxALL|wxEXPAND, 5 );
	
	
	ScenePanelContent->Add( bSizer, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( ScenePanelContent );
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
	delete translateMenu; 
}

ScenePanel_Objects::ScenePanel_Objects( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 45, 45, 45 ) );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	addObjectButton = new wxBitmapButton( this, ADD_OBJECT_BUTTON, wxBitmap( wxT("images/objectIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 28,28 ), wxBU_AUTODRAW|wxNO_BORDER );
	addObjectButton->SetForegroundColour( wxColour( 30, 30, 30 ) );
	addObjectButton->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	
	bSizer6->Add( addObjectButton, 0, wxALL, 2 );
	
	addComponentButton = new wxBitmapButton( this, ADD_COMPONENT_BUTTON, wxBitmap( wxT("images/componentIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 28,28 ), wxBU_AUTODRAW|wxNO_BORDER );
	addComponentButton->SetForegroundColour( wxColour( 30, 30, 30 ) );
	addComponentButton->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	addComponentButton->Enable( false );
	
	bSizer6->Add( addComponentButton, 0, wxALL, 2 );
	
	m_searchCtrl1 = new wxSearchCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifndef __WXMAC__
	m_searchCtrl1->ShowSearchButton( true );
	#endif
	m_searchCtrl1->ShowCancelButton( true );
	m_searchCtrl1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 90, false, wxEmptyString ) );
	
	bSizer6->Add( m_searchCtrl1, 1, wxALL, 3 );
	
	
	bSizer5->Add( bSizer6, 0, wxEXPAND, 0 );
	
	objectList = new wxTreeCtrl( this, OBJECT_LIST, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	objectList->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	objectList->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	
	bSizer5->Add( objectList, 1, wxALL|wxEXPAND, 1 );
	
	propertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE|wxNO_BORDER);
	propertyGrid->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNHIGHLIGHT ) );
	propertyGrid->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNHIGHLIGHT ) );
	
	bSizer5->Add( propertyGrid, 1, wxALL|wxEXPAND, 0 );
	
	
	this->SetSizer( bSizer5 );
	this->Layout();
}

ScenePanel_Objects::~ScenePanel_Objects()
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
	ProjectPanelContent = new wxBoxSizer( wxVERTICAL );
	
	
	this->SetSizer( ProjectPanelContent );
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

ProjectPanel_Project::ProjectPanel_Project( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 45, 45, 45 ) );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	
	this->SetSizer( bSizer5 );
	this->Layout();
}

ProjectPanel_Project::~ProjectPanel_Project()
{
}

ProjectPanel_Assets::ProjectPanel_Assets( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetBackgroundColour( wxColour( 45, 45, 45 ) );
	
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bpButton11 = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("images/featureIcon.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 28,28 ), wxBU_AUTODRAW|wxNO_BORDER );
	m_bpButton11->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	
	bSizer61->Add( m_bpButton11, 0, wxALL, 2 );
	
	
	bSizer51->Add( bSizer61, 0, wxEXPAND, 0 );
	
	assetList = new wxTreeCtrl( this, ASSET_LIST, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT );
	assetList->SetForegroundColour( wxColour( 255, 255, 255 ) );
	assetList->SetBackgroundColour( wxColour( 30, 30, 30 ) );
	
	bSizer51->Add( assetList, 1, wxALL|wxEXPAND, 1 );
	
	assetPropGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE|wxNO_BORDER);
	bSizer51->Add( assetPropGrid, 1, wxALL|wxEXPAND, 0 );
	
	
	this->SetSizer( bSizer51 );
	this->Layout();
}

ProjectPanel_Assets::~ProjectPanel_Assets()
{
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
