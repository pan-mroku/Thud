#include "physics.hpp"
#include "opttritri.hpp" //NoDivTriTriIsect

#include <iostream>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/io_utils> //cout<<mat

//----------------------------------------------------------------------

OSGTriangler::OSGTriangler(const Object& object):
	elementIndex(0),
	triangleIndex(-1),
	AnyTrianglesLeft(true)
{
	vertexArray=object.GetVertexArray();
	drawElements=object.GetElementsList();
	/*	osg::NodeVisitor nodeVisitor;
			object.PAT->accept(nodeVisitor);*/
	transform.set(object.PAT->getWorldMatrices(object.Model)[0]);
	GetNextTriangle();
}

bool OSGTriangler::GetNextTriangle()
{
	while(true)
	{
		if(drawElements[elementIndex]->getMode() == osg::PrimitiveSet::TRIANGLES)
		{
			if(triangleIndex+3>drawElements[elementIndex]->getNumIndices()-3)
			{
				if(elementIndex+1>=drawElements.size())
				{
					AnyTrianglesLeft=false;
					return false;
				}
				else
				{
					++elementIndex;
					triangleIndex=-1;
					continue;
				}
			}

			if(triangleIndex<0)
				triangleIndex=0;
			else
				triangleIndex+=3;

			v0=transform*vertexArray->at(drawElements[elementIndex]->index(triangleIndex));
			v1=transform*vertexArray->at(drawElements[elementIndex]->index(triangleIndex+1));
			v2=transform*vertexArray->at(drawElements[elementIndex]->index(triangleIndex+2));

			AnyTrianglesLeft=true;
				
			return true;
		}
			
		else if(drawElements[elementIndex]->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP)
		{
			if(triangleIndex+1>=drawElements[elementIndex]->getNumIndices())
			{
				if(elementIndex+1>=drawElements.size())
				{
					AnyTrianglesLeft=false;
					return false;
				}
				else
				{
					++elementIndex;
					triangleIndex=-1;
					continue;
				}
			}

			if(triangleIndex<0)
			{
				v0=transform*vertexArray->at(drawElements[elementIndex]->index(0));
				v1=transform*vertexArray->at(drawElements[elementIndex]->index(1));
				v2=transform*vertexArray->at(drawElements[elementIndex]->index(2));
				triangleIndex=3;
			}
			else
			{
				v0=v1;
				v1=v2;
				++triangleIndex;
				v2=transform*vertexArray->at(drawElements[elementIndex]->index(triangleIndex));
			}
			AnyTrianglesLeft=true;
			return true;
		}
		else
		{
			std::cerr<<"Wrong	 element mode"<<std::endl;
			++elementIndex;
			triangleIndex=-1;
			continue;
		}
	}
}

//------------------------------------------------------------------------

Physics::Physics():
	ActiveAlgorithm(COLLISION_ALGORITHM_TRIANGLE)
{
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
			{
				std::cerr<<trianglerA.elementIndex<<","<<trianglerB.elementIndex<<"!"<<"Polygon[{{"<<trianglerA.v0<<"},{"<<trianglerA.v1<<"},{"<<trianglerA.v2<<"}},{{"<<trianglerB.v0<<"},{"<<trianglerB.v1<<"},{"<<trianglerB.v2<<"}}]"<<std::endl;
				return true;
			}
	
	return false;
}

bool Physics::OpenCLCollisionAlgorithm(const Scene& scene)
{
	return false;
}
