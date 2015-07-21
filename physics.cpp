#include "physics.hpp"
#include "opttritri.hpp" //NoDivTriTriIsect

#include <iostream>
#include <fstream>
#include <sstream>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/io_utils> //cout<<mat

#include <cstdlib> //exit

Physics::Physics():
	ActiveAlgorithm(COLLISION_ALGORITHM_TRIANGLE),
	kernelCode("")
{

}

void Physics::CreateContext(osgCanvas& canvas)
{
	cl::Platform::get(&platforms);

	platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
	bool av=0;
	
	int err=CL_SUCCESS;
	context = cl::Context(devices, NULL, NULL, NULL, &err);

	std::string tmp;
	for(std::fstream file("CollisionWithJumps.cl"); std::getline(file, tmp);)
		kernelCode+=tmp;
	
	sources.push_back({kernelCode.c_str(), kernelCode.length()});

	program = cl::Program(context, sources, &err);
	if(err!=CL_SUCCESS)
		std::cerr<<"Program creation error: "<<err<<std::endl;			
	if(program.build({devices[0]})!=CL_SUCCESS)
	{
		std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0])<<"\n";
		exit(1);
	}

	kernel = cl::Kernel(program, "CollisionWithJumps", &err);
	if(err!=CL_SUCCESS)
		std::cerr<<"kernel creation error: "<<err<<std::endl;			

	//http://stackoverflow.com/questions/23017005/determine-max-global-work-group-size-based-on-device-memory-in-opencl
	std::string extensions="";
	unsigned long memSize=0, maxMemAlloc=0;
	size_t computeUnits=0;
	devices[0].getInfo(CL_DEVICE_EXTENSIONS, &extensions);
	devices[0].getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &computeUnits);
	devices[0].getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &maxMemAlloc);
	devices[0].getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &memSize);
	kernel.getWorkGroupInfo(devices[0], CL_KERNEL_WORK_GROUP_SIZE, &maxWorkGroup);
	std::cerr<<"ext: "<<extensions<<std::endl<<"max units: "<<computeUnits<<" max work group: "<<maxWorkGroup<<" err: "<<err<<" devices: "<<devices.size()<<" max mem alloc: "<<maxMemAlloc<<" global mem size: "<<memSize<<std::endl;
	
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
	std::vector<float> verticesA, verticesB;
	std::vector<int> indicesA, indicesB;
	for(; trianglerA.AnyTrianglesLeft; trianglerA.GetNextTriangle())
	{
		indicesA.push_back(trianglerA.index0);
		indicesA.push_back(trianglerA.index1);
		indicesA.push_back(trianglerA.index2);
	}
	for(; trianglerB.AnyTrianglesLeft; trianglerB.GetNextTriangle())
	{
		indicesB.push_back(trianglerB.index0);
		indicesB.push_back(trianglerB.index1);
		indicesB.push_back(trianglerB.index2);
	}
	std::vector<int> triangleCountVector;
	triangleCountVector.push_back(indicesA.size()/3);
	triangleCountVector.push_back(indicesB.size()/3);
	triangleCountVector.push_back(indicesA.size()/3 * indicesB.size()/3);

	cl_int err;

	cl::CommandQueue queue(context, devices[0]);
	//http://stackoverflow.com/questions/9565253/benchmark-of-cl-mem-use-host-ptr-and-cl-mem-copy-host-ptr-in-opencl
	//http://www.cs.virginia.edu/~mwb7w/cuda_support/pinned_tradeoff.html
	cl::Buffer transformBufferA(context,	CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*4*4, trianglerA.transform.ptr(), &err);
	if(err)
		std::cerr<<err<<std::endl;
	cl::Buffer triangleCountBuffer(context, CL_MEM_READ_ONLY| CL_MEM_COPY_HOST_PTR, triangleCountVector.size()*sizeof(int), triangleCountVector.data(), &err);
	if(err)
		std::cerr<<err<<std::endl;

	cl::Buffer vertexBufferA(context,	CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*scene.ObjectA.GetVertexArray()->getNumElements(), (void*)scene.ObjectA.GetVertexArray()->getDataPointer(), &err);
	if(err)
		std::cerr<<err<<std::endl;			
	cl::Buffer indexBufferA(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*indicesA.size(), indicesA.data(), &err);
	if(err)
		std::cerr<<err<<std::endl;

	cl::Buffer vertexBufferB(context,	CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*scene.ObjectB.GetVertexArray()->getNumElements(), (void*)scene.ObjectB.GetVertexArray()->getDataPointer(), &err);
	if(err)
		std::cerr<<err<<std::endl;			
	cl::Buffer indexBufferB(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*indicesB.size(), indicesB.data(), &err);
	if(err)
		std::cerr<<err<<std::endl;
	
	cl::Buffer bufferC(context, CL_MEM_WRITE_ONLY, sizeof(bool)*triangleCountVector[2], NULL, &err);
	if(err)
		std::cerr<<err<<std::endl;

	std::cerr<<"Needed memory: "<<
		triangleCountVector.size()*sizeof(int)
		+sizeof(float)*scene.ObjectA.GetVertexArray()->getNumElements()
		+sizeof(int)*indicesA.size()
		+sizeof(float)*scene.ObjectB.GetVertexArray()->getNumElements()
		+sizeof(int)*indicesB.size()
		+sizeof(bool)*triangleCountVector[2]
	         <<std::endl;
	
	//global się musi dzielić przez local, jeśli nie zostawiamy tego do ogarnęcia opencl
	//0 być nie może. musi być wtedy nullrange
	std::clog<<"Number of triangle pairs: "<<triangleCountVector[2]<<std::endl;
	kernel.setArg(0, triangleCountBuffer);
	kernel.setArg(1, vertexBufferA);
	kernel.setArg(2, indexBufferA);
	kernel.setArg(3, vertexBufferB);
	kernel.setArg(4, indexBufferB);
	kernel.setArg(5, bufferC);
	err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(triangleCountVector[2]), cl::NullRange);
	if (err != CL_SUCCESS)
	{
		std::cerr<<"Queue error: "<<err<<std::endl;
		return false;
	}
	err = queue.finish();
	if (err != CL_SUCCESS)
	{
		std::cerr<<"Kernel error: "<<err<<std::endl;
		return false;
	}

	bool C[triangleCountVector[2]]; //Dla 4k*4k rzuca segfaultem (nic dziwnego w sumie)
	for(int i=0; i<triangleCountVector[2]; i++)
		C[i]=false;
	//read result C from the device to array C
	err = queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, sizeof(bool)*triangleCountVector[2], C);
	if (err != CL_SUCCESS)
	{
		std::cerr<<"Read error: "<<err<<std::endl;
		return false;
	}

	std::cout<<"result: \n";
	for(int x=0; x<triangleCountVector[0]; x++)
	{
		for(int y=0; y<triangleCountVector[1]; y++)
			std::cout<<C[x*triangleCountVector[1]+y]<<" ";
		std::cout<<std::endl;
	}
	std::cout<<"---------------------------"<<std::endl;

	return false;
}

bool Physics::CheckSceneCollision(const Scene& scene)
{
	auto squaredDistanceVector = scene.ObjectA.PAT->getPosition() - scene.ObjectB.PAT->getPosition();
	return (squaredDistanceVector.length2()-scene.SquaredDistanceWhenCollision)<0.1;
}
