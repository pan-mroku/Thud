#include "object.hpp"

#include <osgDB/ReadFile>

Object::Object(const std::string& filename)
{
  Model=osgDB::readNodeFile(filename);
  PAT=new osg::PositionAttitudeTransform;
  PAT->setPosition(osg::Vec3(0,0,0));
  PAT->addChild(Model);
}
