#ifndef GUI_HPP
#define GUI_HPP

#include "osgcanvas.hpp"
#include "physics.hpp"

#include <wx/frame.h>
#include <wx/choice.h>
#include <osg/Timer>
#include <vector>

class Gui: public wxFrame
{
public:
  Gui();
  void InitAfterXRC();
  void SetViewer(osgViewer::Viewer *viewer);
  wxWindow* GetViewerWindow();
  wxChoice* GetAlgorithmChoice();
  void OnIdle(wxIdleEvent& event);

  osg::ElapsedTime Timer;
  std::vector<Scene> Scenes;
  Scene* CurrentScene;
protected:
  void onQuitButton(wxCommandEvent& event);
  
  osg::ref_ptr<osgViewer::Viewer> osgViewer;
  wxWindow* viewerWindow;
  wxChoice* algorithmChoice;
  Physics physicsEngine;
};

#endif
