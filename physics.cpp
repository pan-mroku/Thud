#include "physics.hpp"
#include "opttritri.hpp" //NoDivTriTriIsect

#include <iostream>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/io_utils> //cout<<mat

Physics::Physics():
	ActiveAlgorithm(COLLISION_ALGORITHM_TRIANGLE)
{

}

void Physics::CreateContext(osgCanvas& canvas)
{
	cl::Platform::get(&platforms);

	platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
	bool av=0;
	
	int err=CL_SUCCESS;
	context = cl::Context(devices, NULL, NULL, NULL, &err);

	std::string extensions;
	devices[0].getInfo(CL_DEVICE_EXTENSIONS, &extensions);
	std::cerr<<extensions<<" err: "<<err<<" devices: "<<devices.size()<<std::endl;

	//https://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/global.html
	kernelCode=
		"void kernel translate(global const float* vA, global const int* iA, global float* C){"
		"C[get_global_id(0)]=vA[iA[get_global_id(0)]];"
		"}";
	
	sources.push_back({kernelCode.c_str(),kernelCode.length()});

	program = cl::Program(context,sources);
  if(program.build({devices[0]})!=CL_SUCCESS)
    {
      std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0])<<"\n";
      exit(1);
    }

	//platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
	/*	std::string out;
		devices[0].getInfo(CL_DEVICE_NAME, &out);
		std::cerr<<out<<std::endl;*/ //combobox?	
}

void Physics::Tick(Scene& scene, const double& miliseconds)
{
	scene.ObjectA.PAT->setPosition(scene.ObjectA.PAT->getPosition()+scene.ObjectA.MoveVector*(miliseconds/1000));
	if(scene.ObjectA.PAT->getPosition().x()<0) 
	{
		osg::Vec3d position=scene.ObjectA.PAT->getPosition();
		position.x()=0;
		scene.ObjectA.PAT->setPosition(position);
		scene.ObjectA.MoveVector*=-1;
	}
	else if (scene.ObjectA.PAT->getPosition().x()>2)
	{
		osg::Vec3d position=scene.ObjectA.PAT->getPosition();
		position.x()=2;
		scene.ObjectA.PAT->setPosition(position);
		scene.ObjectA.MoveVector*=-1;
	}
 
	scene.ObjectB.PAT->setPosition(scene.ObjectB.PAT->getPosition()+scene.ObjectB.MoveVector*(miliseconds/1000));
	if(scene.ObjectB.PAT->getPosition().x()>0)
		{
			osg::Vec3d position=scene.ObjectB.PAT->getPosition();
			position.x()=0;
			scene.ObjectB.PAT->setPosition(position);
			scene.ObjectB.MoveVector*=-1;
		}
		else if(scene.ObjectB.PAT->getPosition().x()<-2)
		{
			osg::Vec3d position=scene.ObjectB.PAT->getPosition();
			position.x()=-2;
			scene.ObjectB.PAT->setPosition(position);
			scene.ObjectB.MoveVector*=-1;
		}
}

bool Physics::CheckCollision(const Scene& scene)
{
	switch(ActiveAlgorithm)
	{
	case COLLISION_ALGORITHM_NONE:
		return false;
		break;
	case COLLISION_ALGORITHM_TRIANGLE:
		return TriangleCollisionAlgorithm(scene);
		break;
	case COLLISION_ALGORITHM_OPENCL:
		return OpenCLCollisionAlgorithm(scene);
		break;
	}
}

bool Physics::TriangleCollisionAlgorithm(const Scene& scene)
{
	for(OSGTriangler trianglerA(scene.ObjectA); trianglerA.AnyTrianglesLeft; trianglerA.GetNextTriangle())
		for(OSGTriangler trianglerB(scene.ObjectB); trianglerB.AnyTrianglesLeft; trianglerB.GetNextTriangle())
			if(NoDivTriTriIsect(trianglerA.v0._v, trianglerA.v1._v, trianglerA.v2._v,
			                    trianglerB.v0._v, trianglerB.v1._v, trianglerB.v2._v))
				return true;
	return false;
}

bool Physics::OpenCLCollisionAlgorithm(const Scene& scene)
{
	OSGTriangler trianglerA(scene.ObjectA), trianglerB(scene.ObjectB);
	std::vector<int> indicesA, indicesB;
	for(; trianglerA.AnyTrianglesLeft; trianglerA.GetNextTriangle())
	{
		indicesA.push_back(trianglerA.index);
		indicesA.push_back(trianglerA.index+1);
		indicesA.push_back(trianglerA.index+2);
	}
		//https://developer.apple.com/library/mac/documentation/Performance/Conceptual/OpenCL_MacProgGuide/CreatingandManagingBufferObjectsInOpenCL/CreatingandManagingBufferObjectsInOpenCL.html
	//http://stackoverflow.com/questions/9565253/benchmark-of-cl-mem-use-host-ptr-and-cl-mem-copy-host-ptr-in-opencl
	cl_int err;

	cl::Buffer vertexBufferA(context,	CL_MEM_USE_HOST_PTR, (GLuint)scene.ObjectA.GetVertexArray()->getNumElements(), (void*)scene.ObjectA.GetVertexArray()->getDataPointer(), &err);
	if(err)
		std::cerr<<err<<std::endl;			
	cl::Buffer indexBufferA(context,	CL_MEM_USE_HOST_PTR, (GLuint)indicesA.size(), indicesA.data(), &err);
	if(err)
		std::cerr<<err<<std::endl;			
	//cl::Buffer bufferB(context,	CL_MEM_READ_ONLY, (GLuint)1, nullptr, &err);


	 cl::Buffer buffer_C(context,CL_MEM_READ_WRITE,sizeof(int)*10);
	
	//create queue to which we will push commands for the device.
  cl::CommandQueue queue(context, devices[0]);

  //write arrays A and B to the device
  queue.enqueueWriteBuffer(vertexBufferA,CL_TRUE,0,sizeof(float)*scene.ObjectA.GetVertexArray()->getNumElements(), scene.ObjectA.GetVertexArray()->getDataPointer());
  queue.enqueueWriteBuffer(indexBufferA,CL_TRUE,0,sizeof(int)*indicesA.size(),indicesA.data());
  cl::KernelFunctor translate(cl::Kernel(program,"translate"),queue,cl::NullRange,cl::NullRange,cl::NullRange);
  translate(vertexBufferA, indexBufferA, buffer_C);

  float C[20];
  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*20,C);

  std::cout<<" result: \n";
  for(int i=0;i<20;i++)
    std::cout<<C[i]<<" ";
  std::cout<<std::endl;
	
	return false;
}

bool Physics::CheckSceneCollision(const Scene& scene)
{
	auto squaredDistanceVector = scene.ObjectA.PAT->getPosition() - scene.ObjectB.PAT->getPosition();
	return (squaredDistanceVector.length2()-scene.SquaredDistanceWhenCollision)<0.1;
}
