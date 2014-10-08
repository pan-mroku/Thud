#include "osgcontrol.hpp"

#include <wx/dcclient.h>
#include <iostream>

osgCanvas::osgCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *attributes) : wxGLCanvas(parent, id, attributes, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
  Context=new wxGLContext(this);
}

osgCanvas::~osgCanvas()
{
  delete Context;
}

void osgCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
  /* must always be here */
  wxPaintDC dc(this);
}

/*void osgCanvas::OnChar(wxKeyEvent &event)
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
  }*/

//-------------------------------------------------------------------------------------

wxGraphicsWindow::wxGraphicsWindow(osgCanvas *canvas)
{
  Canvas = canvas;

  _traits = new GraphicsContext::Traits;

  wxPoint pos = Canvas->GetPosition();
  wxSize size = Canvas->GetSize();

  _traits->x = pos.x;
  _traits->y = pos.y;
  _traits->width = size.x;
  _traits->height = size.y;

  init();
}

wxGraphicsWindow::~wxGraphicsWindow()
{
}

void wxGraphicsWindow::init()
{
  if (valid())
    {
      setState( new osg::State );
      getState()->setGraphicsContext(this);

      if (_traits.valid() && _traits->sharedContext && _traits->sharedContext->valid())
        {
          getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
          incrementContextIDUsageCount( getState()->getContextID() );
        }
      else
        {
          getState()->setContextID( osg::GraphicsContext::createNewContextID() );
        }
    }
}

void wxGraphicsWindow::grabFocus()
{
  // focus the canvas
  Canvas->SetFocus();
}

void wxGraphicsWindow::grabFocusIfPointerInWindow()
{
  // focus this window, if the pointer is in the window
  wxPoint pos = wxGetMousePosition();
  if (wxFindWindowAtPoint(pos) == Canvas)
    Canvas->SetFocus();
}

void wxGraphicsWindow::useCursor(bool cursorOn)
{
//Canvas->UseCursor(cursorOn);

}

bool wxGraphicsWindow::makeCurrentImplementation()
{
Canvas->SetCurrent(*Canvas->Context);
return true;
}

void wxGraphicsWindow::swapBuffersImplementation()
{
  Canvas->SwapBuffers();
}

BEGIN_EVENT_TABLE(osgCanvas, wxGLCanvas)
EVT_PAINT               (osgCanvas::OnPaint)
END_EVENT_TABLE()
