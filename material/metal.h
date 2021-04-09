#ifndef METALH
#define METALH

#include "../material.h"

class metal: public material {
    // metal should reflect high freq light only
    // we haven't considerred this at the moment
    public:
        metal(vec3 alb) {
            albedo = make_shared<const_texture>(alb); 
        }
        metal(string filename) {
            albedo = make_shared<png_texture>(filename);
        }
        metal(shared_ptr<texture> alb): albedo(alb){}
        metal(vec3 alb, float dif): diffuse(dif) 
        {
            albedo = make_shared<const_texture>(alb); 
            assert( (0<=dif<=1) && "wrong range of diffuse"); 
            assert( (dif == 0) && "currently not support dif metal");
        }
        virtual bool scatter(const ray& r, const hit_rec& h_rec, sca_rec& s_rec) const;

    private:
        shared_ptr<texture> albedo;
        float diffuse;
};


bool metal::scatter(
    const ray& r, const hit_rec& h_rec, sca_rec& s_rec
) const {
    #if IS_DISP
    s_rec.is_dispersing = false;
    #endif
    s_rec.is_specular = true;
    s_rec.ratio = albedo->get_texture(h_rec.u, h_rec.v);
    s_rec.scattered.mod_origin(h_rec.hit_p);
    return reflect(unit_vector(r.direction()), h_rec.normal, diffuse, s_rec.scattered);
}


#endif

