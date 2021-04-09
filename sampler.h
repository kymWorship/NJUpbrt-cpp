#ifndef SAMPLERH
#define SAMPLERH

#include "hitable.h"
#include "mylib/myrand.h"

class sampler {
    public:
        sampler() {}
        virtual ray ray_gen(const vec3& point) const = 0;
        virtual double pdf_val(const ray& r) const = 0;
};

#include "sampler/mix_sampler.h"
#include "sampler/cos_sampler.h"
#include "sampler/hit_sampler.h"

#endif