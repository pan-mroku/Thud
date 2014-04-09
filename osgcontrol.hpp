#ifndef OSGCONTROL_HPP
#define OSGCONTROL_HPP

#include <wx/glcanvas.h>
#include <osgViewer/Viewer>

class osgCanvas : public wxGLCanvas
{
public:
  osgCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0,
            const wxString& name = wxT("TestGLCanvas"),
            int *attributes = 0);

  virtual ~osgCanvas();

  void OnPaint(wxPaintEvent& event);
  /*void OnSize(wxSizeEvent& event);
  //void OnEraseBackground(wxEraseEvent& event); // Do nothing, to avoid flashing on MSW

  void OnChar(wxKeyEvent &event);
  void OnKeyUp(wxKeyEvent &event);

  void OnMouseEnter(wxMouseEvent &event);
  void OnMouseDown(wxMouseEvent &event);
  void OnMouseUp(wxMouseEvent &event);
  void OnMouseMotion(wxMouseEvent &event);
  void OnMouseWheel(wxMouseEvent &event);*/

  osg::ref_ptr<osgViewer::GraphicsWindow> _graphics_window;
  wxGLContext* Context;

};

//-----------------------------------------------------------

class wxGraphicsWindow : public osgViewer::GraphicsWindow
{
public:
  osgCanvas* Canvas;

  wxGraphicsWindow(osgCanvas* canvas=NULL);
  ~wxGraphicsWindow();

  void init();

  //
  // GraphicsWindow interface
  //
  void grabFocus();
  void grabFocusIfPointerInWindow();
  void useCursor(bool cursorOn);

  bool makeCurrentImplementation();
  void swapBuffersImplementation();

  virtual bool valid() const { return true; }
  virtual bool realizeImplementation() { return true; }
  virtual bool isRealizedImplementation() const { return Canvas->IsShownOnScreen(); }
  virtual void closeImplementation() {}
  virtual bool releaseContextImplementation() { return true; }
};

#endif
