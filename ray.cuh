#ifndef RAYH
#define RAYH
#include "vec3.cuh"

class ray{
    public:
        __device__ ray() {is_dispersed = false;}
        __device__ ray(const vec3& a, const vec3& b) { A = a; B = b; is_dispersed = false; }
        __device__ vec3 origin() const     { return A; }
        __device__ vec3 direction() const  { return B; }
        __device__ void mod_origin(vec3 moded_o) { A = moded_o; }
        __device__ void mod_direction(vec3 moded_d) { B = moded_d; }
        __device__ vec3 point_at_parameter(float t) const { return A + t*B; }
        bool is_dispersed;

    private:
        vec3 A;
        vec3 B;
};

#endif