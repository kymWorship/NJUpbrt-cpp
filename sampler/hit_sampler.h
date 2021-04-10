#ifndef HIT_SAMPLERH
#define HIT_SAMPLERH

#include "../sampler.h"

class hit_sampler: public sampler {
    public:
        ~hit_sampler() {}
        hit_sampler(shared_ptr<hitable> obj): samp_obj(obj) {}
        virtual ray ray_gen(const vec3& point) const;
        virtual double pdf_val(const ray& r) const;
    private:
        shared_ptr<hitable> samp_obj;
};

ray hit_sampler::ray_gen(const vec3& point) const {
    return samp_obj->ray_gen(point);
}

double hit_sampler::pdf_val(const ray& r) const {
    return samp_obj->pdf_val(r);
}

#endif