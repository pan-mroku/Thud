#ifndef OSGCANVAS_HPP
#define OSGCANVAS_HPP

#include <wx/glcanvas.h>
#include <osgViewer/Viewer>

class osgCanvas : public wxGLCanvas, public osgViewer::GraphicsWindow
{
public:
  osgCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0,
            const wxString& name = wxT("TestGLCanvas"),
            int *attributes = 0);

  virtual ~osgCanvas();
  void OnPaint(wxPaintEvent& event);

  void grabFocusIfPointerInWindow();

  void OnMouseDown(wxMouseEvent &event);
  void OnMouseUp(wxMouseEvent &event);
  void OnMouseMotion(wxMouseEvent &event);

  wxGLContext* Context;

  bool makeCurrentImplementation();
  void swapBuffersImplementation();

  virtual bool valid() const { return true; }
  virtual bool realizeImplementation() { return true; }
  virtual bool isRealizedImplementation() const { return IsShownOnScreen(); }
  virtual void closeImplementation() {}
  virtual bool releaseContextImplementation() { return true; }
  
};

#endif
