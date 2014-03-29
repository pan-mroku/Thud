#ifndef OSGCONTROL_HPP
#define OSGCONTROL_HPP

#include <wx/control.h>
#include <osgViewer/Viewer>

//@TODO: przerobić na wxGLCanvas https://github.com/openscenegraph/osg/tree/master/examples/osgviewerWX
class osgControl : public wxControl
{
public:
  osgControl();
  ~osgControl();

  osgViewer::Viewer* Viewer;

private:
  void OnPaint(wxPaintEvent&);
};

#endif
