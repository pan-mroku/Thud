#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "scene.hpp"

class Physics
{
public:
  Physics();

  typedef enum
    {
      COLLISION_ALGORITHM_NONE=0,
      COLLISION_ALGORITHM_TRIANGLE,
      COLLISION_ALGORITHM_OPENCL
    } CollisionAlgorithmEnum;
  CollisionAlgorithmEnum ActiveAlgorithm;
  
  void Tick(Scene& scene, const double& miliseconds);

  bool CheckCollision(const Scene& scene);
  bool TriangleCollisionAlgorithm(const Scene& scene);
  bool OpenCLCollisionAlgorithm(const Scene& scene);

protected:
private:
};

class OSGTriangler
{
public:
  OSGTriangler(const Object& object);

  //Zwraca false, jeśli skończyły się trójkąty. Ustawia też AnyTrianglesLeft.
  bool GetNextTriangle();

  bool AnyTrianglesLeft;
  int elementIndex, triangleIndex;
  osg::Vec3d v0,v1,v2;
  const osg::Vec3Array* vertexArray;
  osg::Geometry::DrawElementsList drawElements;
  osg::Matrix transform;
};

#endif
