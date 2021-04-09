#ifndef COS_SAMPLERH
#define COS_SAMPLERH

#include "../sampler.h"

class cos_sampler:public sampler {
    public:
        ~cos_sampler() {}
        cos_sampler(vec3 n): normal(n) {}
        virtual ray ray_gen(const vec3& point) const ;
        virtual double pdf_val(const ray& r) const ;
    private:
        vec3 normal;
};

ray cos_sampler::ray_gen(const vec3& point) const {
    return ray(point, normal+rand_in_unit_sphere());
}

double cos_sampler::pdf_val(const ray& r) const {
    double cosine =  dot(normal, unit_vector(r.direction()));
    return (cosine<=0) ? 0 : cosine / M_PI;
}

#endif