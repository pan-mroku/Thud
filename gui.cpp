#include "gui.hpp"

#include <wx/xrc/xmlres.h>
#include <iostream>

Gui::Gui()
{
  Bind(wxEVT_IDLE, &Gui::OnIdle, this);
  Timer.reset();
}

void Gui::InitAfterXRC()
{
  FindWindow("ButtonQuit")->Bind(wxEVT_BUTTON, &Gui::onQuitButton, this);
}

void Gui::onQuitButton(wxCommandEvent& event)
{
  Close(true);
}

void Gui::SetViewer(osgViewer::Viewer *viewer)
{
  osgViewer = viewer;
}

void Gui::OnIdle(wxIdleEvent &event)
{
  if (!osgViewer->isRealized())
    return;

  physicsEngine.CheckCollision(*CurrentScene);
  physicsEngine.Tick(*CurrentScene, Timer.elapsedTime_m());
  Timer.reset();

  osgViewer->frame();
    
  event.RequestMore();
}
