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

void GUI::SetViewer(osgViewer::Viewer *viewer)
{
  osgViewer = viewer;
}

void GUI::OnIdle(wxIdleEvent &event)
{
    if (!osgViewer->isRealized())
        return;

    osgViewer->frame();

    event.RequestMore();
}
