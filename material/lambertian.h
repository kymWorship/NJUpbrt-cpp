#ifndef LAMBERTIANH
#define LAMBERTIANH

#include "../material.h"

class lambertian: public material {
    public:
        lambertian() {}
        lambertian(vec3 a) {
            attenuation = make_shared<const_texture>(a);
            back_attenuation = attenuation;
        }
        lambertian(vec3 a, vec3 ba) {
            attenuation = make_shared<const_texture>(a);
            back_attenuation = make_shared<const_texture>(ba);
        }
        lambertian(string filename) {
            attenuation = make_shared<png_texture>(filename);
            back_attenuation = attenuation;
        }
        lambertian(string frontfile, string backfile) {
            attenuation = make_shared<png_texture>(frontfile);
            back_attenuation = make_shared<png_texture>(backfile);
        }
        lambertian(shared_ptr<texture> text) {
            attenuation = text;
            back_attenuation = text;
        }
        lambertian(shared_ptr<texture> fronttext,
                   shared_ptr<texture> backtext) {
            attenuation = fronttext;
            back_attenuation = backtext;
        }
        virtual bool scatter(
            const ray& r, const hit_rec& h_rec, sca_rec& s_rec
        ) const;
        virtual vec3 scattering_pdf(
            const ray& r_in, const hit_rec& h_rec, const ray& r_out
        ) const;
    private:
        shared_ptr<texture> attenuation;
        shared_ptr<texture> back_attenuation;
};

bool lambertian::scatter(
    const ray& r, const hit_rec& h_rec, sca_rec& s_rec
) const {
    #if IS_DISP
    s_rec.is_dispersing = false;
    #endif
    bool flag = ( dot(r.direction(), h_rec.normal) > 0 );
    s_rec.is_specular = false;
    s_rec.ratio = flag ? back_attenuation->get_texture(h_rec.u, h_rec.v) : attenuation->get_texture(h_rec.u, h_rec.v);
    s_rec.scattered.mod_origin(h_rec.hit_p);
    vec3 incident = flag ? h_rec.normal : - h_rec.normal;
    return reflect(incident, h_rec.normal, 1.0, s_rec.scattered);
}

vec3 lambertian::scattering_pdf(
    const ray& r_in, const hit_rec& h_rec, const ray& r_out
) const {
    double cosine = dot(h_rec.normal, unit_vector(r_out.direction()));
    return (cosine<=0) ? vec3(0, 0, 0) : vec3(cosine / M_PI, cosine / M_PI, cosine / M_PI);
}

#endif