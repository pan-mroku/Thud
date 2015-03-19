#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>

class Object
{
public:
  osg::ref_ptr<osg::Node> Model;
  osg::ref_ptr<osg::PositionAttitudeTransform> PAT;
  osg::Vec3d MoveVector;

  Object(const std::string& filename);

  osg::Geometry::DrawElementsList GetElementsList() const;
  const osg::Vec3Array* GetVertexArray() const;
};

#endif
