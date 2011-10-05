// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Sphere.h"

using namespace optix;

bool Sphere::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
    // Implement ray-sphere intersection here.
    //
    // Input:  r                    (the ray to be checked for intersection)
    //         prim_idx             (index of the primitive element in a collection, not used here)
    //
    // Output: hit.has_hit          (true if the ray intersects the sphere, false otherwise)
    //         hit.dist             (distance from the ray origin to the intersection point)
    //         hit.geometric_normal (the normalized normal of the sphere)
    //         hit.shading_normal   (the normalized normal of the sphere)
    //         hit.material         (pointer to the material of the sphere)
    //        (hit.texcoord)        (texture coordinates of intersection point, not needed for Week 1)
    //
    // Return: True if the ray intersects the sphere, false otherwise
    //
    // Relevant data fields that are available (see Sphere.h and OptiX math library reference)
    // r.origin                     (ray origin)
    // r.direction                  (ray direction)
    // r.tmin                       (minimum intersection distance allowed)
    // r.tmax                       (maximum intersection distance allowed)
    // center                       (sphere center)
    // radius                       (sphere radius)
    // material                     (material of the sphere)
    //
    // Hints: (a) The square root function is called sqrt(x).
    //        (b) There is no need to handle the case where the 
    //            discriminant is zero separately.
    
    float b = optix::dot((r.origin - center), r.direction);
    float c = optix::dot((r.origin - center),(r.origin - center)) - radius * radius;
    
    if (b * b - c < 0) {
        return false;
    }   
    
    float t1 = -b - sqrt(b*b - c);
    
    if (r.tmin < t1 && t1 < r.tmax) {
        hit.dist = t1;
    } else {
        float t2 = -b + sqrt(b*b - c);
        if (r.tmin < t2 && t2 < r.tmax) {
            hit.dist = t2;
        } else {
            return false;
        }
    }
    
    optix::float3 intersection = hit.dist * r.direction + r.origin;
    optix::float3 normal = (intersection - center)/radius;
    
    hit.geometric_normal = normal;
    hit.shading_normal = normal;
    hit.has_hit = true;
    hit.material = &material;
    
    return hit.has_hit;
}

void Sphere::transform(const Matrix4x4& m)
{
    float3 radius_vec = make_float3(radius, 0.0f, 0.0f) + center;
    radius_vec = make_float3(m*make_float4(radius_vec, 1.0f));
    center = make_float3(m*make_float4(center, 1.0f)); 
    // The radius is scaled by the X scaling factor.
    // Not ideal, but the best we can do without elipsoids
    radius_vec -= center;
    radius = length(radius_vec);  
}

Aabb Sphere::compute_bbox() const
{
    Aabb bbox;
    bbox.include(center - make_float3(radius, 0.0f, 0.0f));
    bbox.include(center + make_float3(radius, 0.0f, 0.0f));
    bbox.include(center - make_float3(0.0f, radius, 0.0f));
    bbox.include(center + make_float3(0.0f, radius, 0.0f));
    bbox.include(center - make_float3(0.0f, 0.0f, radius));
    bbox.include(center + make_float3(0.0f, 0.0f, radius));
    return bbox;
}
