#ifndef OSGTRIANGLER_HPP
#define OSGTRIANGLER_HPP

#include "object.hpp"

class OSGTriangler
{
public:
  OSGTriangler(const Object& object);

  //Zwraca false, jeśli skończyły się trójkąty. Ustawia też AnyTrianglesLeft.
  bool GetNextTriangle();

  bool AnyTrianglesLeft;
	int elementIndex, triangleIndex;
  osg::Vec3d v0,v1,v2;
	int index0, index1, index2;
  const osg::Vec3Array* vertexArray;
  osg::Geometry::DrawElementsList drawElements;
  osg::Matrix transform;
};

#endif
