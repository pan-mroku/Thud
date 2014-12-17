#include "app.hpp"
#include "gui.h"
#include "osgcanvas.hpp"


#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/Node>

#include <tinyxml2.h>

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

  osgDB::ReaderWriter::Options* options = new osgDB::ReaderWriter::Options();
  options->setObjectCacheHint(osgDB::ReaderWriter::Options::CACHE_ALL);
  osgDB::Registry::instance()->setOptions(options);

  tinyxml2::XMLDocument configDocument;
  configDocument.LoadFile("./scenes.xml");
  tinyxml2::XMLNode*  configRoot = configDocument.RootElement();
  tinyxml2::XMLElement* sceneElement = configRoot->FirstChildElement("scene");
  for(tinyxml2::XMLElement* sceneElement = configRoot->FirstChildElement("scene"); sceneElement!=NULL; sceneElement=sceneElement->NextSiblingElement())
    {
      std::string modelA=sceneElement->Attribute("objecta");
      std::string modelB=sceneElement->Attribute("objectb");
      gui->Scenes.push_back(Scene(modelA, modelB));
    }
	
  viewer->setSceneData(gui->Scenes[0].GroupNode);
  gui->CurrentScene=&(gui->Scenes[0]);
  viewer->setCameraManipulator(new osgGA::TrackballManipulator);

  gui->SetViewer(viewer);

  gui->Show(true);
  canvas->SetFocus();

  return true;
}

