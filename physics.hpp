#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "scene.hpp"

class Physics
{
public:
  Physics(){};
  
  void Tick(Scene& scene, const double& miliseconds);
protected:
private:
};

#endif
