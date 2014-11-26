#include "scene.hpp"

Scene::Scene():
  ObjectA("models/monkey.obj"),
  ObjectB("models/monkey.obj")
{
  ObjectA.PAT->setPosition(osg::Vec3(2,0,0));
  ObjectB.PAT->setPosition(osg::Vec3(-2,0,0));
  GroupNode=new osg::Group();
  GroupNode->addChild(ObjectA.PAT);
  GroupNode->addChild(ObjectB.PAT);
}

