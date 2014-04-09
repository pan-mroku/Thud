///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxGridSizer* SizerMain;
	SizerMain = new wxGridSizer( 2, 2, 0, 0 );
	
	
	SizerMain->Add( 800,600, 0, wxALL|wxEXPAND, 5 );
	
	ButtonQuit = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	SizerMain->Add( ButtonQuit, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	this->SetSizer( SizerMain );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	ButtonQuit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::m_button1OnButtonClick ), NULL, this );
}

MainWindow::~MainWindow()
{
	// Disconnect Events
	ButtonQuit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::m_button1OnButtonClick ), NULL, this );
	
}
