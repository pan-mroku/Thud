#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "scene.hpp"

class Physics
{
public:
  Physics();

  typedef enum
    {
      COLLISION_ALGORITHM_NONE,
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

#endif
