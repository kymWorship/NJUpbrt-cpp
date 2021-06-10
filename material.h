#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"
#include "mylib/myrand.h"
#include "mylib/settings.h"
#include "bits/stdc++.h"
#include "material/texture.h"

#define ENV_IND 1.0

struct sca_rec {
    ray scattered;
    vec3 ratio;
    bool is_specular;
    #if IS_DISP
    bool is_dispersing;
    ray rscattered;
    ray gscattered;
    ray bscattered;
    #endif
};

class material {
    public:
        virtual bool scatter(
            const ray& r, const hit_rec& h_rec, sca_rec& s_rec 
        ) const = 0;
        virtual vec3 emitted(const ray& r, const hit_rec& h_rec) const {
            return vec3(0, 0, 0);
        }
        virtual double scattering_pdf(
            const ray& r_in, const hit_rec& h_rec, const ray& r_out
        ) const {
            assert(0&&"warning");
            return 0.0;
        }

        inline virtual bool is_sampling() const{
            return false;
        }
};



// incident light and normal should be normalized
bool reflect(const vec3& inc, const vec3& nor, float dif, ray& reflected) {
    //assert( ( fabs(inc.length()-1) < 0.01 ) && "incident light not normalized" );
    //assert( ( fabs(nor.length()-1) < 0.01 ) && "normal not normalized" );
    // NOTE: should be annotated after testing
    reflected.mod_direction( inc - 2*dot(inc, nor)*nor + dif*rand_in_unit_sphere() );
    return (dot(inc, nor)*dot(reflected.direction(), nor) < 0);
    // incident light at same side of the surface as reflected light
}

// incident light and normal should be normalized
bool refract(const vec3& inc, const vec3& nor, float ni_over_nt, ray& refracted) {
    //assert( ( fabs(inc.length()-1) < 0.01 ) && "incident light not normalized" );
    //assert( ( fabs(nor.length()-1) < 0.01 ) && "normal not normalized" );
    // NOTE: should be annotated after testing
    vec3 ref_perp = ( inc - dot(inc, nor)*nor ) * ni_over_nt;
    float determination = ref_perp.squared_length();
    if (determination>1.0) return false;

    vec3 ref_para = sqrt(1-determination)*nor;
    if ( dot(inc, nor)<0 ) ref_para = -ref_para;
    refracted.mod_direction(ref_para+ref_perp);
    return true; 
}

#include "material/source.h"
#include "material/metal.h"
#include "material/lambertian.h"
#include "material/glass.h"

#endif