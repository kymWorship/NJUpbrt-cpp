#ifndef AABBH
#define AABBH

#include "../hitable.h"

/*
    not used currently
    using ../bound.h instead
*/

class aabb:public hitable {
    public:
        aabb(vec3 mincorner, vec3 maxcorner):
            rmin(mincorner), rmax(maxcorner) {}
        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_rec& rec
        ) const ;
        virtual bound bbox() const {
            assert(0&&"should not use this method?");
            return bound(rmin, rmax);
        }
    private:
        vec3 rmin, rmax;
};

#endif