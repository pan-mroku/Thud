#include "object.hpp"

#include <osgDB/ReadFile>

Object::Object(const std::string& filename)
{
  Model=osgDB::readNodeFile(filename);
  PAT=new osg::PositionAttitudeTransform;
  PAT->setPosition(osg::Vec3(0,0,0));
  PAT->addChild(Model);
  MoveVector.set(0,0,0);
}

osg::Geometry::DrawElementsList Object::GetElementsList() const
{
  osg::Geometry::DrawElementsList out;
  Model->asGroup()->getChild(0)->asGeode()->getDrawable(0)->asGeometry()->getDrawElementsList(out);
  return out; 
}

const osg::Vec3Array* Object::GetVertexArray() const
{
  return (osg::Vec3Array*)Model->asGroup()->getChild(0)->asGeode()->getDrawable(0)->asGeometry()->getVertexArray();
}
