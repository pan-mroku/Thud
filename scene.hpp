#ifndef SCENE_HPP
#define SCENE_HPP

#include "object.hpp"

class Scene
{
public:
  Object ObjectA, ObjectB;
  osg::ref_ptr<osg::Group> GroupNode;

  Scene(const std::string& ModelA="", const std::string& ModelB="");
};

#endif
