#include "osgcanvas.hpp"

#include <wx/dcclient.h>
#include <iostream>

using namespace osgViewer;

osgCanvas::osgCanvas(wxWindow *parent, wxWindowID id, int *attributes, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
  wxGLCanvas(parent, id, attributes, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
  Context=new wxGLContext(this);
  wxGLCanvas::Bind(wxEVT_PAINT, &osgCanvas::OnPaint, this);
  wxGLCanvas::Bind(wxEVT_LEFT_UP, &osgCanvas::OnMouseUp, this);
  wxGLCanvas::Bind(wxEVT_MOTION, &osgCanvas::OnMouseMotion, this);
  wxGLCanvas::Bind(wxEVT_LEFT_DOWN, &osgCanvas::OnMouseDown, this);
  wxGLCanvas::Bind(wxEVT_CHAR, &osgCanvas::OnKeyDown, this);
  wxGLCanvas::Bind(wxEVT_KEY_UP, &osgCanvas::OnKeyUp, this);
  wxGLCanvas::Bind(wxEVT_MOUSEWHEEL, &osgCanvas::OnMouseWheel, this);
  parent->Bind(wxEVT_SIZE, &osgCanvas::OnSize, this);

  _traits = new GraphicsContext::Traits;

  _traits->x = wxGLCanvas::GetPosition().x;
  _traits->y = wxGLCanvas::GetPosition().y;
  _traits->width = wxGLCanvas::GetSize().x;
  _traits->height = wxGLCanvas::GetSize().y;

  GraphicsWindow::setState( new osg::State );
  GraphicsWindow::getState()->setGraphicsContext(this);

  if (_traits && _traits->sharedContext.valid())
    {
      GraphicsWindow::getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
      GraphicsWindow::incrementContextIDUsageCount( getState()->getContextID() );
    }
  else
    {
      GraphicsWindow::getState()->setContextID( osg::GraphicsContext::createNewContextID() );
    }
}

osgCanvas::~osgCanvas()
{
  delete Context;
}

void osgCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
  /*
   * A wxPaintDC must be constructed if an application wishes to paint on the client area of a window from within an EVT_PAINT() event handler.
   * This should normally be constructed as a temporary stack object; don't store a wxPaintDC object. If you have an EVT_PAINT() handler, you must create a wxPaintDC object within it even if you don't actually use it.
   */
  wxPaintDC dc(this);
}

void osgCanvas::OnMouseDown(wxMouseEvent &event)
{
  GraphicsWindow::getEventQueue()->mouseButtonPress(event.GetX(), event.GetY(), event.GetButton());
}

void osgCanvas::OnMouseUp(wxMouseEvent &event)
{
  GraphicsWindow::getEventQueue()->mouseButtonRelease(event.GetX(), event.GetY(), event.GetButton());
}

void osgCanvas::OnMouseMotion(wxMouseEvent &event)
{
  GraphicsWindow::getEventQueue()->mouseMotion(event.GetX(), event.GetY());
}

void osgCanvas::OnMouseWheel(wxMouseEvent& event)
{
  int delta = event.GetWheelRotation() / event.GetWheelDelta() * event.GetLinesPerAction();
  if(delta>0)
    GraphicsWindow::getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
  else
    GraphicsWindow::getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);
}

void osgCanvas::OnSize(wxSizeEvent& event)
{
  _traits->width = wxWindow::GetParent()->GetClientSize().x;
  _traits->height = wxWindow::GetParent()->GetClientSize().y;
  wxGLCanvas::SetSize(wxWindow::GetParent()->GetClientSize());
  GraphicsContext::_cameras.front()->setViewport(0, 0, _traits->width, _traits->height);
  wxWindow::Refresh();
}

void osgCanvas::OnKeyDown(wxKeyEvent& event)
{
#if wxUSE_UNICODE
  int key = event.GetUnicodeKey();
#else
  int key = event.GetKeyCode();
#endif
  GraphicsWindow::getEventQueue()->keyPress(key);
}

void osgCanvas::OnKeyUp(wxKeyEvent& event)
{
#if wxUSE_UNICODE
  int key = event.GetUnicodeKey();
#else
  int key = event.GetKeyCode();
#endif
  GraphicsWindow::getEventQueue()->keyRelease(key);
}

bool osgCanvas::makeCurrentImplementation()
{
  wxGLCanvas::SetCurrent(*Context);
  return true;
}

void osgCanvas::swapBuffersImplementation()
{
  wxGLCanvas::SwapBuffers();
}

void osgCanvas::grabFocusIfPointerInWindow()
{
  // focus this window, if the pointer is in the window
  wxPoint pos = wxGetMousePosition();
  if (wxFindWindowAtPoint(pos) == this)
    SetFocus();
}
