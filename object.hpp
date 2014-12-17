#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>

class Object
{
public:
  osg::ref_ptr<osg::Node> Model;
  osg::ref_ptr<osg::PositionAttitudeTransform> PAT;
  osg::Vec3d MoveVector;

  Object(const std::string& filename);
};

#endif
