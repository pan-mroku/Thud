#include "scene.hpp"

Scene::Scene(const std::string& ModelA, const std::string& ModelB):
  ObjectA(ModelA),
  ObjectB(ModelB)
{
  ObjectA.PAT->setPosition(osg::Vec3(2,0,0));
  ObjectB.PAT->setPosition(osg::Vec3(-2,0,0));
  GroupNode=new osg::Group();
  GroupNode->addChild(ObjectA.PAT);
  GroupNode->addChild(ObjectB.PAT);
}

