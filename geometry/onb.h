#ifndef ONBH
#define ONBH

#include "vec3.h"

class onb {
    public:
        onb(vec3 norm) {
            bases[2] = unit_vector(norm);
            if ( fabs(bases[2].x()) < 0.9 ) 
                bases[0] = unit_vector( cross(bases[2], vec3(1,0,0)) );
            else
                bases[0] = unit_vector( cross(bases[2], vec3(0,1,0)) );
            bases[1] = cross(bases[2], bases[0]);
        }

        vec3 local(const vec3& a) const{
            return a.x()*bases[0] + a.y()*bases[1] + a.z()*bases[2];
        }

        vec3 local(double u, double v, double w) const {
            return u*bases[0] + v*bases[1] + w*bases[2];
        }

    private:
        vec3 bases[3];
};

#endif