#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "scene.hpp"

class Physics
{
public:
  Physics();

  typedef enum
    {
      None,
      Triangle,
      OpenCL
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
