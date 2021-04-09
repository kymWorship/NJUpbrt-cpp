#ifndef GLASSH
#define GLASSH

#include "../material.h"


class glass:public material {
    public:
        glass() {}
        glass(float indf, float dif):
            ind(vec3(indf, indf, indf)),
            ave_ind(indf), diffuze(dif)
            {}
        glass(vec3 indv, float dif): 
            ind(indv), diffuze(dif),
            ave_ind( (indv.r()+indv.g()+indv.b())/3 ) 
            {}
        virtual bool scatter(const ray& r, const hit_rec& h_rec, sca_rec& s_rec) const;

    private:
        vec3 ind;
        float ave_ind;
        float diffuze;
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
            
        }
        ray disp_sca(
            const ray& r, const hit_rec& h_rec, float cur_ind
        ) const {
            float ni_over_nt = (dot(r.direction(), h_rec.normal)>0) ? (cur_ind/ENV_IND) : (ENV_IND/cur_ind);
            vec3 unit_incident = unit_vector(r.direction());
            ray scattered;
            scattered.mod_origin(h_rec.hit_p);
            float cos_theta = fabs(dot(unit_incident, h_rec.normal));
            float prob = reflectance(cos_theta, ni_over_nt);
            
            if ( random_double()>prob && refract(unit_incident, h_rec.normal, ni_over_nt, scattered) ) {
                return scattered;
            }
            else {
                reflect(unit_incident, h_rec.normal, diffuze, scattered);
                return scattered;
            }
        }
};

bool glass::scatter(
    const ray& r, const hit_rec& h_rec, sca_rec& s_rec
)const {
    s_rec.ratio = vec3(1.0, 1.0, 1.0);
    #if IS_DISP
    if ( !r.is_dispersed ) {
        s_rec.is_dispersing = true;
        s_rec.rscattered = disp_sca(r, h_rec, ind.r());
        s_rec.gscattered = disp_sca(r, h_rec, ind.g());
        s_rec.bscattered = disp_sca(r, h_rec, ind.b());
        return true;
    }
    s_rec.is_dispersing = false;
    #endif
    s_rec.is_specular = true;
    s_rec.scattered = disp_sca(r, h_rec, ave_ind);
    return true;
}

#endif