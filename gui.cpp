#include "gui.hpp"

#include "opttritri.hpp" //test

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
  viewerWindow=(wxWindow*)FindWindow("osgViewer");
  algorithmChoice=(wxChoice*)FindWindow("AlgorithmChoice");
  algorithmChoice->Append("asd");
  algorithmChoice->Append("asd");
}

void Gui::onQuitButton(wxCommandEvent& event)
{
  Close(true);
}

void Gui::SetViewer(osgViewer::Viewer *viewer)
{
  osgViewer = viewer;
}

wxWindow* Gui::GetViewerWindow()
{
  return viewerWindow;
}

wxChoice* Gui::GetAlgorithmChoice()
{
  return algorithmChoice;
}

void Gui::OnIdle(wxIdleEvent &event)
{
  if (!osgViewer->isRealized())
    return;

  //std::cerr<<physicsEngine.CheckCollision(*CurrentScene)<<std::endl;
  /*  osg::Vec3f asd[6];
  asd[0]=osg::Vec3f(-1.23625, -0.864516, 0.908743);
  asd[1]=osg::Vec3f(-1.33038, -0.918059, 0.938894);
  asd[2]=osg::Vec3f(-1.46115, -0.928695, 0.970334);
  asd[3]=osg::Vec3f(-1884.64, -1467.39, -896.76);
  asd[4]=osg::Vec3f(-39.1307, -30.047, -21.9818);
  asd[5]=osg::Vec3f(402.94, 312.441, 220.892);
  std::cout<<NoDivTriTriIsect(asd[0]._v, asd[1]._v, asd[2]._v, asd[3]._v, asd[4]._v, asd[5]._v)<<std::endl;*/
  physicsEngine.Tick(*CurrentScene, Timer.elapsedTime_m());
  Timer.reset();

  osgViewer->frame();
    
  event.RequestMore();
}
