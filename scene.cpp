#include "scene.hpp"

Scene::Scene(const std::string& ModelA, const std::string& ModelB, const double& squaredDistanceWhenCollision):
  ObjectA(ModelA),
  ObjectB(ModelB),
  SquaredDistanceWhenCollision(squaredDistanceWhenCollision)
{
  ObjectA.PAT->setPosition(osg::Vec3(2,0,0));
  ObjectA.MoveVector.x()=-0.5;
  ObjectB.PAT->setPosition(osg::Vec3(-2,0,0));
  ObjectB.MoveVector.x()=0.5;
  GroupNode=new osg::Group();
  GroupNode->addChild(ObjectA.PAT);
  GroupNode->addChild(ObjectB.PAT);
}

