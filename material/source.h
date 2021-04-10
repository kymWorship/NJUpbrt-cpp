#ifndef SOURCEH
#define SOURCEH

#include "../material.h"

class source: public material {
    public:
        source() {}
        source(vec3 color) {
            src_col = make_shared<const_texture>(color);
            back_src_col = src_col;
        }
        source(vec3 color, vec3 back_color) {
            src_col = make_shared<const_texture>(color);
            back_src_col = make_shared<const_texture>(back_color);
        }
        source(string filename) {
            src_col = make_shared<png_texture>(filename);
            back_src_col = src_col;
        }
        source(string frontfile, string backfile) {
            src_col = make_shared<png_texture>(frontfile);
            back_src_col = make_shared<png_texture>(backfile);
        }
        source(shared_ptr<texture> text) {
            src_col = text;
            back_src_col = text;
        }
        source(shared_ptr<texture> fronttext,
               shared_ptr<texture> backtext) {
            src_col = fronttext;
            back_src_col = backtext;
        }
        virtual bool scatter(const ray& r, const hit_rec& h_rec, sca_rec& s_rec) const;
        virtual vec3 emitted(const hit_rec& hrec) const;
        inline virtual bool is_sampling() const {
            return true;
        }
    private:
        shared_ptr<texture> src_col;
        shared_ptr<texture> back_src_col;
};

bool source::scatter(
    const ray& r, const hit_rec& h_rec, sca_rec& s_rec
) const{
    #if IS_DISP
    s_rec.is_dispersing = false;
    #endif

    //scattered = ray(record.hit_p, -r.direction());
    s_rec.is_specular = false;
    return false;
}

vec3 source::emitted(const hit_rec& h_rec) const {
    return src_col->get_texture(h_rec.u, h_rec.v);
}


#endif