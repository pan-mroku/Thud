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
      COLLISION_ALGORITHM_OPENCL,
      COLLISION_ALGORITHM_TRIANGLE_ALL,
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
	int workGroupSize;

  void Tick(Scene& scene, const double& miliseconds);

	void CreateContext();
	
  bool CheckCollision(const Scene& scene);
  bool TriangleCollisionAlgorithm(const Scene& scene);
  bool OpenCLCollisionAlgorithm(const Scene& scene);
	bool TriangleAllCollisionAlgorithm(const Scene& scene);

	bool CheckSceneCollision(const Scene& scene);

protected:
private:
};

#endif
