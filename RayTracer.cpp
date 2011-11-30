// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "ObjMaterial.h"
#include "fresnel.h"
#include "RayTracer.h"

using namespace optix;

bool RayTracer::trace_reflected(const Ray& in, const HitInfo& in_hit, Ray& out, HitInfo& out_hit) const
{
    optix::float3 R = optix::reflect(in.direction, in_hit.shading_normal);
    out = make_Ray(in_hit.position, R, 0, 1e-3, RT_DEFAULT_MAX);
    out_hit.trace_depth = in_hit.trace_depth + 1;
    return trace_to_closest(out, out_hit);
}

bool RayTracer::trace_refracted(const Ray& in, const HitInfo& in_hit, Ray& out, HitInfo& out_hit) const
{
    float3 normal, direction;
    float cos_theta_in;
    out_hit.ray_ior = get_ior_out(in, in_hit, direction, normal, cos_theta_in);
    if(!refract(out.direction, -direction, normal, out_hit.ray_ior/in_hit.ray_ior)) {
        return false;
    }
    out = make_Ray(in_hit.position, out.direction, 0, 1e-3, RT_DEFAULT_MAX);
    out_hit.trace_depth = in_hit.trace_depth + 1;
    return trace_to_closest(out, out_hit);
    
    // Initialize the refracted ray and trace it. Set out_hit.ray_ior and out_hit.trace_depth.
    // Return true if the refracted ray hit anything.
    // Remember that the function must handle total internal reflection.
}

bool RayTracer::trace_refracted(const Ray& in, const HitInfo& in_hit, Ray& out, HitInfo& out_hit, float& R) const
{
    // Initialize the refracted ray and trace it. Set out_hit.ray_ior and out_hit.trace_depth.
    // Compute the Fresnel reflectance (see fresnel.h) and return it in R.
    // Return true if the refracted ray hit anything.
    // Remember that the function must handle total internal reflection.
    float3 normal, direction;
    float cos_theta_in;
    out_hit.ray_ior = get_ior_out(in, in_hit, direction, normal, cos_theta_in);

    if(!refract(out.direction, -direction, normal, out_hit.ray_ior/in_hit.ray_ior)) {
        R = 1.0;
        return false;
    }

    out = make_Ray(in_hit.position, out.direction, 0, 1e-3, RT_DEFAULT_MAX);
    out_hit.trace_depth = in_hit.trace_depth + 1;
    
    bool rtn = trace_to_closest(out, out_hit);

    float3 outNormal = -normal;
    float3 outDir = out.direction;
    float cos_theta_out = dot(outNormal, outDir);
    
    R = fresnel_R(cos_theta_in, cos_theta_out, in_hit.ray_ior, out_hit.ray_ior);
    
    return rtn;
}

float RayTracer::get_ior_out(const Ray& in, const HitInfo& in_hit, float3& dir, float3& normal, float& cos_theta_in) const
{
    normal = in_hit.shading_normal;
    dir = -in.direction;
    cos_theta_in = dot(normal, dir);
	if(cos_theta_in < 0.0)
	{
        normal = -normal;
		cos_theta_in = -cos_theta_in;
        return 1.0f;
    }
    const ObjMaterial* m = in_hit.material;
    return m ? m->ior : 1.0f;
}
