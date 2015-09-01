#include "app.hpp"
#include "gui.hpp"
#include "osgcanvas.hpp"

#include <wx/xrc/xmlres.h>

#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/Node>

#include <tinyxml2.h>

#include <cstdlib> //strtod

IMPLEMENT_APP(App)

bool App::OnInit()
{
  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->Load("gui.xrc");
  Gui* gui=new Gui();
  wxXmlResource::Get()->LoadFrame(gui, NULL, "MainWindow");
  gui->InitAfterXRC();
  
  int attributes[7];
  attributes[0] = int(WX_GL_DOUBLEBUFFER);
  attributes[1] = WX_GL_RGBA;
  attributes[2] = WX_GL_DEPTH_SIZE;
  attributes[3] = 8;
  attributes[4] = WX_GL_STENCIL_SIZE;
  attributes[5] = 8;
  attributes[6] = 0;
  
  osgCanvas* canvas=new osgCanvas(gui->GetViewerWindow(), wxID_ANY, attributes, wxDefaultPosition, wxSize(800, 600), wxSUNKEN_BORDER, wxT("osgviewerWX"));

  osgViewer::Viewer* viewer=new osgViewer::Viewer;
  viewer->getCamera()->setGraphicsContext(canvas);
  viewer->getCamera()->setViewport(0,0,800,600);
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
    double squaredDistanceWhenCollision=std::strtod(sceneElement->Attribute("squared_distance"), NULL);
    gui->Scenes.push_back(Scene(modelA, modelB, squaredDistanceWhenCollision));
    gui->SceneChoice->Append(sceneElement->Attribute("title"));
  }
	
  viewer->setSceneData(gui->Scenes[0].GroupNode);
  gui->CurrentScene=&(gui->Scenes[0]);
  gui->SceneChoice->SetSelection(0);
  viewer->setCameraManipulator(new osgGA::TrackballManipulator);

  gui->SetViewer(viewer);

  gui->Show(true);
  gui->PhysicsEngine.CreateContext();

  for(int i=0; i<=gui->PhysicsEngine.maxWorkGroup; i++)
	  gui->WorkGroupCombo->Append(wxString::Format(wxT("%i"), i));
  gui->WorkGroupCombo->SetSelection(0);

  gui->MaxWorkGroupText->SetLabel(wxString::Format(wxT("%i"), (int)gui->PhysicsEngine.maxWorkGroup));
  
  canvas->SetFocus();

  return true;
}

