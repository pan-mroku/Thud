#include "osgcontrol.hpp"

osgCanvas::osgCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *attributes) : wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name, attributes)
{
}

osgCanvas::~osgCanvas()
{
}

void osgCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
  /* must always be here */
  wxPaintDC dc(this);
}

void osgCanvas::OnSize(wxSizeEvent& event)
{
  // this is also necessary to update the context on some platforms
  wxGLCanvas::OnSize(event);

  // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
  int width, height;
  GetClientSize(&width, &height);

  if (_graphics_window.valid())
    {
      // update the window dimensions, in case the window has been resized.
      _graphics_window->getEventQueue()->windowResize(0, 0, width, height);
      _graphics_window->resized(0,0,width,height);
    }
}

void osgCanvas::OnChar(wxKeyEvent &event)
{
#if wxUSE_UNICODE
  int key = event.GetUnicodeKey();
#else
  int key = event.GetKeyCode();
#endif

  if (_graphics_window.valid())
    _graphics_window->getEventQueue()->keyPress(key);

  // If this key event is not processed here, we should call
  // event.Skip() to allow processing to continue.
}

void osgCanvas::OnKeyUp(wxKeyEvent &event)
{
#if wxUSE_UNICODE
  int key = event.GetUnicodeKey();
#else
  int key = event.GetKeyCode();
#endif

  if (_graphics_window.valid())
    _graphics_window->getEventQueue()->keyRelease(key);

  // If this key event is not processed here, we should call
  // event.Skip() to allow processing to continue.
}

void osgCanvas::OnMouseEnter(wxMouseEvent &event)
{
  // Set focus to ourselves, so keyboard events get directed to us
  SetFocus();
}

void osgCanvas::OnMouseDown(wxMouseEvent &event)
{
  if (_graphics_window.valid())
    {
      _graphics_window->getEventQueue()->mouseButtonPress(event.GetX(), event.GetY(),
                                                          event.GetButton());
    }
}

void osgCanvas::OnMouseUp(wxMouseEvent &event)
{
  if (_graphics_window.valid())
    {
      _graphics_window->getEventQueue()->mouseButtonRelease(event.GetX(), event.GetY(),
                                                            event.GetButton());
    }
}

void osgCanvas::OnMouseMotion(wxMouseEvent &event)
{
  if (_graphics_window.valid())
    _graphics_window->getEventQueue()->mouseMotion(event.GetX(), event.GetY());
}

void osgCanvas::OnMouseWheel(wxMouseEvent &event)
{
  int delta = event.GetWheelRotation() / event.GetWheelDelta() * event.GetLinesPerAction();

  if (_graphics_window.valid()) {
    _graphics_window->getEventQueue()->mouseScroll(
                                                   delta>0 ?
                                                   osgGA::GUIEventAdapter::SCROLL_UP :
                                                   osgGA::GUIEventAdapter::SCROLL_DOWN);
  }
}

//-------------------------------------------------------------------------------------

