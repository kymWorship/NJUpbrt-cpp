#ifndef BACKGROUNDH
#define BACKGROUNDH

#include "vec3.h"
#include "mylib/settings.h"

inline vec3 background(const vec3& direction) {
    #if bg_light
        vec3 unit_direction = unit_vector(direction);
        float t = 0.5*(unit_direction.y() + 1.0);
        return dark_fac*( (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0) );
    #else
        return vec3(0, 0, 0);
    #endif
}

#endif