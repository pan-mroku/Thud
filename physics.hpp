#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "scene.hpp"
#include "osgcanvas.hpp"
#include "osgtriangler.hpp"

#include <CL/cl.hpp>

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

	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;
	cl::Context context;
	cl::Program::Sources sources;
	std::string kernelCode;
	cl::Program program;
	cl::Kernel kernel;
	size_t maxWorkGroup;

  void Tick(Scene& scene, const double& miliseconds);

	void CreateContext(osgCanvas& canvas);
	
  bool CheckCollision(const Scene& scene);
  bool TriangleCollisionAlgorithm(const Scene& scene);
  bool OpenCLCollisionAlgorithm(const Scene& scene);

	bool CheckSceneCollision(const Scene& scene);

protected:
private:
};

#endif
