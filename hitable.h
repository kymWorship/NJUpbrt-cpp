#ifndef HITABLEH
#define HITABLEH

#include "geometry/geometry.h"
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
    shared_ptr<material> mat_ptr;
    float u = 0;
    float v = 0;
};


class hitable {
    public:
        // NOTE! rec.normal should be unitized
        virtual bool hit(
            const ray & r, float t_min, float t_max, hit_rec & rec
        ) const = 0;
        virtual bound bbox() const = 0;
        virtual bool is_inside (const vec3& point) const = 0;
        virtual ray ray_gen(const vec3& point) const {
            assert(0&&"warning");return ray(point, vec3(1, 0, 0));
        }
        virtual double pdf_val(const ray& r) const {
            assert(0&&"warning");return 0.0;
        }
        virtual shared_ptr<material> mat() const {
            assert(0 && "warning"); return nullptr;
        }
        virtual bool has_source() const {assert(0 && "warning");return false;}
};

#include "hitable/sphere.h"
#include "hitable/hitable_list.h"
#include "hitable/cube.h"
#include "hitable/intersect.h"
#include "hitable/plane.h"
#include "hitable/triangle.h"
#include "hitable/cylinder.h"
#include "hitable/polygon.h"
#include "hitable/disc.h"


#endif