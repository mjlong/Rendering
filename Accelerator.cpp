// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <vector>
#include <optix_world.h>
#include "AccObj.h"
#include "Object3D.h"
#include "Plane.h"
#include "HitInfo.h"
#include "Accelerator.h"

using namespace std;
using namespace optix;

Accelerator::~Accelerator()
{
  for(unsigned int i = 0; i < primitives.size(); ++i)
    delete primitives[i];
}

void Accelerator::init(const vector<Object3D*>& geometry, const vector<const Plane*>& scene_planes)
{
  for(unsigned int i = 0; i < geometry.size(); ++i)
  {
    Object3D* obj = geometry[i];
    unsigned int no_of_prims = primitives.size();
    primitives.resize(no_of_prims + obj->get_no_of_primitives());
    for(unsigned int j = 0; j < obj->get_no_of_primitives(); ++j)
      primitives[j + no_of_prims] = new AccObj(obj, j);
  }
  planes = scene_planes;
}

bool Accelerator::closest_hit(optix::Ray& r, HitInfo& hit) const
{
    closest_plane(r, hit);
    
    bool hasHit = false;
    
    for (unsigned int i = 0; i < primitives.size(); i++) {
        Object3D *obj = primitives[i]->geometry;
        if (obj->intersect(r, hit, i)) {
            hasHit = true;
            r.tmax = hit.dist;
            hit.position = r.origin + hit.dist * r.direction;
        }
    }
    
    return hasHit;
}

bool Accelerator::any_hit(optix::Ray& r, HitInfo& hit) const
{
  if(!any_plane(r, hit))
  {
      
      for (unsigned int i = 0; i < primitives.size(); i++) {
          Object3D *obj = primitives[i]->geometry;
          if(obj->intersect(r, hit, i)) {
              return true;
          }
      }
      return false;
  }
  return hit.has_hit;
}

void Accelerator::closest_plane(Ray& r, HitInfo& hit) const
{
    for (unsigned int i = 0; i < planes.size(); i++) {
        const Plane *plane = planes[i];
        if(plane->intersect(r, hit, 0)){
            r.tmax = hit.dist;
            hit.position = r.origin + hit.dist * r.direction;
        }
    }
}

bool Accelerator::any_plane(Ray& r, HitInfo& hit) const
{
    for (unsigned int i = 0; i < planes.size(); i++) {
        const Plane *plane = planes[i];
        if(plane->intersect(r, hit, i)) {
            return true;
        }
    }
    return false; 
}
