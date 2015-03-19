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

void Physics::Tick(Scene& scene, const double& miliseconds)
{
  scene.ObjectA.PAT->setPosition(scene.ObjectA.PAT->getPosition()+scene.ObjectA.MoveVector*(miliseconds/1000));
  if(scene.ObjectA.PAT->getPosition().x()<0 || scene.ObjectA.PAT->getPosition().x()>2)
    scene.ObjectA.MoveVector*=-1;
 
  scene.ObjectB.PAT->setPosition(scene.ObjectB.PAT->getPosition()+scene.ObjectB.MoveVector*(miliseconds/1000));
  if(scene.ObjectB.PAT->getPosition().x()>0 || scene.ObjectB.PAT->getPosition().x()<-2)
    scene.ObjectB.MoveVector*=-1;
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
  const osg::Vec3Array* vertexArrayA=scene.ObjectA.GetVertexArray();
  const osg::Vec3Array* vertexArrayB=scene.ObjectB.GetVertexArray();
  
  osg::Geometry::DrawElementsList drawElementsA=scene.ObjectA.GetElementsList();
  osg::Geometry::DrawElementsList drawElementsB=scene.ObjectB.GetElementsList();

  osg::NodeVisitor nodeVisitor;
  scene.GroupNode->accept(nodeVisitor);
  osg::Matrix transformA, transformB;
  scene.ObjectA.PAT->computeLocalToWorldMatrix(transformA, &nodeVisitor);
  scene.ObjectB.PAT->computeLocalToWorldMatrix(transformB, &nodeVisitor);

  //trzyprzejściowe pętle z wektorem bardzo wszystko spowalniały, więc trzeba sobie radzić inaczej
  osg::Vec3d a0,a1,a2, b0,b1,b2;
  
  //int i=0;
  for(int i=0; i<drawElementsA.size(); i++)
    {
      switch(drawElementsA[i]->getMode())
        {
        case osg::PrimitiveSet::TRIANGLES:
          {
            for(int j=0; j<drawElementsA[i]->getNumIndices()-3; j+=3)
              {
                a0=transformA*vertexArrayA->at(drawElementsA[i]->index(j));
                a1=transformA*vertexArrayA->at(drawElementsA[i]->index(j+1));
                a2=transformA*vertexArrayA->at(drawElementsA[i]->index(j+2));
                //a teraz B
              }
            break;
          }
        case osg::PrimitiveSet::TRIANGLE_STRIP:
          {
            for(int j=0; j<drawElementsA[i]->getNumIndices(); j++)
              std::cout<<vertexArrayA->at(drawElementsA[i]->index(j))<<std::endl;
            std::cout<<std::endl;
            break;
          }
        default:
          std::cerr<<"Wrong  element mode"<<std::endl;
        }
      
    }

  /*  const osg::Vec3Array* vertexArrayB=scene.ObjectB.GetVertexArray();


  

  for(int vertexIndexA=0; vertexIndexA<vertexArrayA->size(); vertexIndexA++)
    {
      a0=transformA*vertexArrayA->at(vertexIndexA)[0];
      a1=transformA*vertexArrayA->at(vertexIndexA)[1];
      a2=transformA*vertexArrayA->at(vertexIndexA)[2];
      for(int vertexIndexB=0; vertexIndexB<vertexArrayB->size(); vertexIndexB++)
        {
          b0=transformB*vertexArrayB->at(vertexIndexB)[0];
          b1=transformB*vertexArrayB->at(vertexIndexB)[1];
          b1=transformB*vertexArrayB->at(vertexIndexB)[2];
          if(NoDivTriTriIsect(&a0.x,&a1.x,&a2.x,&b0.x,&b1.x,&b2.x))
            return true;
        }
        }*/
  
  return false;
}

bool Physics::OpenCLCollisionAlgorithm(const Scene& scene)
{
  return false;
}
