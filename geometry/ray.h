#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray{
    public:
        ray() {is_dispersed = false;}
        ray(const vec3& a, const vec3& b) { A = a; B = b; is_dispersed = false; }
        vec3 origin() const     { return A; }
        vec3 direction() const  { return B; }
        void mod_origin(vec3 moded_o) { A = moded_o; }
        void mod_direction(vec3 moded_d) { B = moded_d; }
        vec3 point_at_parameter(float t) const { return A + t*B; }
        bool is_dispersed;

    private:
        vec3 A;
        vec3 B;
};

#endif