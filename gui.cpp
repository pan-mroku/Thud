#include "gui.h"
#include <iostream>

GUI::GUI( wxWindow* parent ):
  MainWindow( parent )
{
  Bind(wxEVT_IDLE, &GUI::OnIdle, this);
  Timer.reset();
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

    physicsEngine.Tick(*CurrentScene, Timer.elapsedTime_m());
    Timer.reset();

    osgViewer->frame();
    
    event.RequestMore();
}
