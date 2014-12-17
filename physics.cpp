#include "physics.hpp"

#include <iostream>

void Physics::Tick(Scene& scene, const double& miliseconds)
{
  scene.ObjectA.PAT->setPosition(scene.ObjectA.PAT->getPosition()+scene.ObjectA.MoveVector*(miliseconds/1000));
  if(scene.ObjectA.PAT->getPosition().x()<0 || scene.ObjectA.PAT->getPosition().x()>2)
    scene.ObjectA.MoveVector*=-1;
 
  scene.ObjectB.PAT->setPosition(scene.ObjectB.PAT->getPosition()+scene.ObjectB.MoveVector*(miliseconds/1000));
  if(scene.ObjectB.PAT->getPosition().x()>0 || scene.ObjectB.PAT->getPosition().x()<-2)
    scene.ObjectB.MoveVector*=-1;
}
