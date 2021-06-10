#ifndef PLANEH
#define PLANEH

#include "../hitable.h"

/*
    infinite plane defined with
        point: one point on the plane
        n: normal of the plane
    Note n also define inside outside of
    the plane!
*/

class plane: public hitable {
    public:
        plane(vec3 point, vec3 n, shared_ptr<material> m_p):
            p_on_plane(point), mat_ptr(m_p){
                normal = unit_vector(n);
        }
        virtual bool hit(
            const ray & r, float t_min, float t_max, hit_rec & rec
        ) const;
        virtual bound bbox() const {assert(0&&"not support bbox for plane"); return bound();}
        virtual bool is_inside (const vec3& point) const ;
        virtual ray ray_gen(const vec3& point) const {
            assert(0&&"warning");return ray(point, vec3(1, 0, 0));
        }
        virtual double pdf_val(const ray& r) const {
            assert(0&&"warning");return 0.0;
        }
        virtual shared_ptr<material> mat() const { 
            return mat_ptr;
        }
    private:
        vec3 p_on_plane;
        vec3 normal;
        shared_ptr<material> mat_ptr; 
};

bool plane::hit(
    const ray& r, float t_min, float t_max, hit_rec& rec
) const {
    double direc_n = dot(r.direction(), normal);
    if (direc_n == 0) return false;
    float temp_t = dot( p_on_plane - r.origin(), normal) / direc_n;
    if ( temp_t < t_max && temp_t > t_min ) {
        rec.hit_t = temp_t;
        rec.hit_p = r.point_at_parameter(temp_t);
        rec.mat_ptr = mat_ptr;
        rec.normal = normal;
        return true;
    }
    return false;
}

bool plane::is_inside(const vec3& point) const {
    return dot( p_on_plane - point, normal ) >= 0;
}



#endif