#include "gui.h"

GUI::GUI( wxWindow* parent )
:
MainWindow( parent )
{

}

void GUI::m_button1OnButtonClick( wxCommandEvent& event )
{
  Close(true);
}
