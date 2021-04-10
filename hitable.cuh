#ifndef HITABLEH
#define HITABLEH

#include "ray.cuh"
#include "vec3.cuh"
#include "bits/stdc++.h"
#include <memory>
#include "mylib/common.h"
#include "bound.h"


class material;

struct hit_rec
{
    vec3 hit_p;
    vec3 normal;    // NOTE: must return unit normal outward
    float hit_t;
    material mat_ptr;
    float u = 0;
    float v = 0;
};


class hitable {
    public:
        // NOTE! rec.normal should be unitized
        __device__ virtual bool hit(
            const ray & r, float t_min, float t_max, hit_rec & rec
        ) const = 0;
        __device__ virtual bound bbox() const = 0;
        __device__ virtual bool is_inside (const vec3& point) const = 0;
        __device__ virtual ray ray_gen(const vec3& point) const {
            assert(0&&"warning");return ray(point, vec3(1, 0, 0));
        }
        __device__ virtual double pdf_val(const ray& r) const {
            assert(0&&"warning");return 0.0;
        }
        __device__ virtual shared_ptr<material> mat() const {
            assert(0 && "warning"); return nullptr;
        }
        __device__ virtual bool has_source() const {assert(0 && "warning");return false;}
};

#include "hitable/sphere.h"
#include "hitable/hitable_list.h"
#include "hitable/cube.h"
#include "hitable/intersect.h"
#include "hitable/plane.h"
#include "hitable/triangle.h"


#endif