#include "osgcontrol.hpp"

osgControl::osgControl()
{
  Viewer=new osgViewer::Viewer();

  WXWidget handle =GetHandle();

  osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
  traits->inheritedWindowData = new osgViewer::GraphicsWindowX11::WindowData(handle);

osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext( traits.get() );
viewer->getCamera()->setGraphicsContext( gc.get() );
viewer->getCamera()->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );
viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f);
}


osgControl::~osgControl()
{
  delete Viewer;
}

void osgControl::OnPaint(wxPaintEvent& paintEvent)
{

}
