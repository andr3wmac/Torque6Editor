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
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/treectrl.h>
#include <wx/aui/auibook.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/dataview.h>
#include <wx/bmpbuttn.h>
#include <wx/srchctrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/notebook.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/wizard.h>
#include <wx/dynarray.h>
WX_DEFINE_ARRAY_PTR( wxWizardPageSimple*, WizardPages );

///////////////////////////////////////////////////////////////////////////

#define MENU_CAMERA_SLOW 1000
#define MENU_CAMERA_NORMAL 1001
#define MENU_CAMERA_FAST 1002
#define MENU_PROJECT 1003
#define MENU_CONSOLE 1004
#define MENU_SCENE 1005
#define MENU_SCRIPTS 1006
#define MENU_MATERIALS 1007
#define MENU_PROFILER 1008
#define MENU_WEBSITE 1009
#define MENU_GITHUB 1010
#define MENU_FORUMS 1011
#define MENU_ABOUT 1012
#define PROFILER_START 1013
#define SCENE_NEW 1014
#define SCENE_OPEN 1015
#define SCENE_SAVE 1016
#define ADD_ENTITY_BUTTON 1017
#define ADD_COMPONENT_BUTTON 1018
#define ENTITY_LIST 1019
#define ADD_FEATURE_BUTTON 1020
#define FEATURE_LIST 1021
#define TRANSLATE_SNAP_NONE 1022
#define TRANSLATE_SNAP_1 1023
#define TRANSLATE_SNAP_2 1024
#define TRANSLATE_SNAP_3 1025
#define TRANSLATE_SNAP_4 1026
#define MATERIAL_SAVE 1027
#define ASSET_LIST 1028
#define MENU_IMPORT_MESH 1029
#define MENU_IMPORT_TEXTURE 1030
#define MENU_NEW_MATERIAL 1031

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenu* m_menu1;
		wxMenu* m_menu7;
		wxMenu* m_menu2;
		wxMenu* m_menu6;
	
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
/// Class AboutDialog
///////////////////////////////////////////////////////////////////////////////
class AboutDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticBitmap* m_bitmap1;
		wxStaticText* m_staticText13;
		wxStaticText* m_staticText14;
		wxStaticText* m_staticText15;
		wxStaticText* m_staticText16;
	
	public:
		
		AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 464,265 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~AboutDialog();
	
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
		wxSearchCtrl* m_searchCtrl1;
		wxPanel* m_panel12;
		wxBitmapButton* m_bpButton11;
	
	public:
		wxBitmapButton* addEntityButton;
		wxBitmapButton* addComponentButton;
		wxTreeCtrl* entityList;
		wxPropertyGrid* propertyGrid;
		wxTreeCtrl* featureList;
		wxPropertyGrid* featurePropGrid;
		wxMenu* translateMenu;
		
		ScenePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 367,509 ), long style = wxTAB_TRAVERSAL ); 
		~ScenePanel();
		
		void ScenePanelOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( translateMenu, event.GetPosition() );
		}
	
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
	
	public:
		wxTreeCtrl* m_materialTree;
		wxPropertyGrid* propertyGrid;
		wxAuiNotebook* m_materialNotebook;
		
		MaterialsPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 498,301 ), long style = wxTAB_TRAVERSAL ); wxAuiManager m_mgr;
		
		~MaterialsPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ProjectPanel
///////////////////////////////////////////////////////////////////////////////
class ProjectPanel : public wxPanel 
{
	private:
	
	protected:
		wxNotebook* m_notebook2;
		wxPanel* m_panel11;
		wxPanel* m_panel12;
		wxBitmapButton* m_bpButton11;
	
	public:
		wxTreeCtrl* assetList;
		wxPropertyGrid* assetPropGrid;
		wxMenu* moduleMenu;
		
		ProjectPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 367,509 ), long style = wxTAB_TRAVERSAL ); 
		~ProjectPanel();
		
		void ProjectPanelOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( moduleMenu, event.GetPosition() );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ImportMeshWizard
///////////////////////////////////////////////////////////////////////////////
class ImportMeshWizard : public wxWizard 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxCheckBox* m_checkBox2;
		wxCheckBox* m_checkBox3;
		wxStaticText* m_staticText111;
		wxStaticText* m_staticText121;
	
	public:
		wxFilePickerCtrl* meshFilePath;
		wxTextCtrl* assetID;
		wxDirPickerCtrl* importPath;
		wxCheckBox* copyMeshCheck;
		
		ImportMeshWizard( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Import Mesh"), const wxBitmap& bitmap = wxNullBitmap, const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_DIALOG_STYLE );
		WizardPages m_pages;
		~ImportMeshWizard();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ImportTextureWizard
///////////////////////////////////////////////////////////////////////////////
class ImportTextureWizard : public wxWizard 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText111;
		wxStaticText* m_staticText121;
	
	public:
		wxFilePickerCtrl* textureFilePath;
		wxCheckBox* generateMipsCheck;
		wxTextCtrl* assetID;
		wxDirPickerCtrl* importPath;
		wxCheckBox* copyTextureCheck;
		
		ImportTextureWizard( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Import Texture"), const wxBitmap& bitmap = wxNullBitmap, const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_DIALOG_STYLE );
		WizardPages m_pages;
		~ImportTextureWizard();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class NewMaterialWizard
///////////////////////////////////////////////////////////////////////////////
class NewMaterialWizard : public wxWizard 
{
	private:
	
	protected:
		wxStaticText* m_staticText111;
		wxStaticText* m_staticText121;
	
	public:
		wxTextCtrl* assetID;
		wxDirPickerCtrl* savePath;
		
		NewMaterialWizard( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New Material"), const wxBitmap& bitmap = wxNullBitmap, const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_DIALOG_STYLE );
		WizardPages m_pages;
		~NewMaterialWizard();
	
};

#endif //__TORQUE6EDITORUI_H__
