// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "mt_random.h"
#include "PointLight.h"

using namespace optix;

bool PointLight::sample(const float3& pos, float3& dir, float3& L) const
{
  // Compute output and return value given the following information.
  //
  // Input:  pos (the position of the geometry in the scene)
  //
  // Output: dir (the direction toward the light)
  //         L   (the radiance received from the direction dir)
  //
  // Return: true if not in shadow
  //
  // Relevant data fields that are available (see Directional.h and Light.h):
  // shadows    (on/off flag for shadows)
  // tracer     (pointer to ray tracer)
  // light_pos  (position of the point light)
  // intensity  (intensity of the emitted light)
  //
  // Hint: Construct a shadow ray using the Ray datatype. Trace it using the
  //       pointer to the ray tracer.
    
    dir = (light_pos - pos);
    float dist = optix::length(dir);
    dir = optix::normalize(dir);
    
    Ray ray = make_Ray(pos, dir, 0, 1e-3, dist);    
    HitInfo hit;
    tracer->trace_to_any(ray, hit);

    if (hit.has_hit) {
        return false;
    }

    L = intensity / powf(dist, 2);
    
    return true;
}

bool PointLight::emit(Ray& r, HitInfo& hit, float3& Phi) const
{
  // Sample ray direction and create ray

  // Trace ray
  
  // If a surface was hit, compute Phi and return true

  return false;
}
