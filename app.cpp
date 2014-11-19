#include "app.hpp"
#include "gui.h"
#include "osgcanvas.hpp"

#include<osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

IMPLEMENT_APP(App)

bool App::OnInit()
{
  GUI *gui=new GUI();

  int attributes[7];
  attributes[0] = int(WX_GL_DOUBLEBUFFER);
  attributes[1] = WX_GL_RGBA;
  attributes[2] = WX_GL_DEPTH_SIZE;
  attributes[3] = 8;
  attributes[4] = WX_GL_STENCIL_SIZE;
  attributes[5] = 8;
  attributes[6] = 0;
  
  osgCanvas* canvas=new osgCanvas(gui, wxID_ANY, wxDefaultPosition, wxSize(1024, 768), wxSUNKEN_BORDER, wxT("osgviewerWX"), attributes);

  osgViewer::Viewer* viewer=new osgViewer::Viewer;
  viewer->getCamera()->setGraphicsContext(canvas);
  viewer->getCamera()->setViewport(0,0,1024,768);
  viewer->addEventHandler(new osgViewer::StatsHandler);
  viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

  osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("models/monkey.obj");
  viewer->setSceneData(loadedModel.get());
  viewer->setCameraManipulator(new osgGA::TrackballManipulator);

  gui->SetViewer(viewer);

  gui->Show(true);
  canvas->SetFocus();

  return true;
}
