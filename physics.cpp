#include "physics.hpp"
#include "opttritri.hpp"

#include <iostream>

#include <osg/Geode>
#include <osg/Geometry>

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

//std::cout<<scene.ObjectA.Model->asGroup()->getChild(0)->asGeode()->getDrawableList()[0]->asGeometry()->getVertexArray()->getNumElements()<<std::endl;
std::cout<<scene.ObjectA.Model->asGroup()->getChild(0)->asGeode()->getDrawableList()[0]->asGeometry()->getNumPrimitiveSets()<<std::endl;
  /*
  //Position
  Ogre::Vector3 positionA=objectA.Node->_getDerivedPosition();
  Ogre::Vector3 positionB=objectB.Node->_getDerivedPosition();
  //Orientation
  Ogre::Quaternion orientationA=objectA.Node->_getDerivedOrientation();
  Ogre::Quaternion orientationB=objectB.Node->_getDerivedOrientation();
  //FullTransform
  Ogre::Matrix4 transformA=objectA.Node->_getFullTransform();
  Ogre::Matrix4 transformB=objectB.Node->_getFullTransform();
  //trzyprzej¶ciowe pêtle bardzo wszystko spowalnia³y, wiêc trzeba sobie radziæ inaczej
  Ogre::Vector3 a0,a1,a2, b0,b1,b2;
  //dla ka¿dego trójk±ta A
  for(int faceIndexA=0;faceIndexA<objectA.IndicesBuffer.size()/3;faceIndexA++)
  {
  //trójk±t A
  a0=transformA*objectA.VerticesBuffer[objectA.IndicesBuffer[3*faceIndexA]];
  //std::cout<<a0<<" "<<objectA.VerticesBuffer[objectA.IndicesBuffer[3*faceIndexA]]<<" "<<positionA<<std::endl;
  a1=transformA*objectA.VerticesBuffer[objectA.IndicesBuffer[3*faceIndexA+1]];
  a2=transformA*objectA.VerticesBuffer[objectA.IndicesBuffer[3*faceIndexA+2]];
  //sprawdzamy ka¿dy trójk±t B
  for(int faceIndexB=0;faceIndexB<objectB.IndicesBuffer.size()/3;faceIndexB++)
  {
  //trójk±t B
  b0=transformB*objectB.VerticesBuffer[objectB.IndicesBuffer[3*faceIndexB]];
  b1=transformB*objectB.VerticesBuffer[objectB.IndicesBuffer[3*faceIndexB+1]];
  b2=transformB*objectB.VerticesBuffer[objectB.IndicesBuffer[3*faceIndexB+2]];
  //if(Moller(a0, a1, a2, b0, b1, b2))
  if(NoDivTriTriIsect(&a0.x,&a1.x,&a2.x,&b0.x,&b1.x,&b2.x))
  return true;
}
}
  */
  return false;
}

bool Physics::OpenCLCollisionAlgorithm(const Scene& scene)
{
  return false;
}
