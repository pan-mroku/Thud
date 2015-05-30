#include "osgtriangler.hpp"

#include <iostream>

OSGTriangler::OSGTriangler(const Object& object):
	elementIndex(0),
	triangleIndex(-1),
	AnyTrianglesLeft(true),
	index(-1)
{
	vertexArray=object.GetVertexArray();
	drawElements=object.GetElementsList();
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

			index=drawElements[elementIndex]->index(triangleIndex);
			v0=vertexArray->at(drawElements[elementIndex]->index(triangleIndex))*transform;
			v1=vertexArray->at(drawElements[elementIndex]->index(triangleIndex+1))*transform;
			v2=vertexArray->at(drawElements[elementIndex]->index(triangleIndex+2))*transform;

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
				index=drawElements[elementIndex]->index(0);
				v0=vertexArray->at(drawElements[elementIndex]->index(0))*transform;
				v1=vertexArray->at(drawElements[elementIndex]->index(1))*transform;
				v2=vertexArray->at(drawElements[elementIndex]->index(2))*transform;
				triangleIndex=3;
			}
			else
			{
				v0=v1;
				v1=v2;
				++triangleIndex;
				v2=vertexArray->at(drawElements[elementIndex]->index(triangleIndex))*transform;
				index=drawElements[elementIndex]->index(triangleIndex-3);
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
