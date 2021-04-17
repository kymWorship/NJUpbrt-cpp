#ifndef CYLINDERH
#define CYLINDERH

#include "../hitable.h"

class cylinder: public hitable {
    public:
        cylinder() {}
        ~cylinder() {}
        cylinder(vec3 o, vec3 d, float r, shared_ptr<material> m_p)
            :cy_origin(o), cy_direct(d), mat_ptr(m_p),
             cy_unit_direct(unit_vector(d)), cy_rad(r){
                 cy_radsq = cy_rad*cy_rad;
             }
        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_rec& rec
        ) const;
        virtual bound bbox() const;
        virtual bool is_inside(const vec3& point)const {
            assert(0&&"intersect function is currently not used");
            return false;
        }
        virtual shared_ptr<material> mat() const {return mat_ptr;}

    private:
        vec3 cy_origin, cy_direct;
        vec3 cy_unit_direct;
        float cy_rad;
        double cy_radsq;
        shared_ptr<material> mat_ptr;
};

bool cylinder::hit(
    const ray& r, float t_min, float t_max, hit_rec& rec
) const {
    vector<double> t_pos;t_pos.clear();
    vector<vec3> n_pos;n_pos.clear();
    double direct_parall = dot(r.direction(), cy_unit_direct);
    // a = |D|^2 - D_parall^2
    double a = r.direction().squared_length() - direct_parall*direct_parall; 
    vec3 delta_o = r.origin() - cy_origin;
    double delta_o_parall = dot(delta_o, cy_unit_direct);
    double delta_o_sq = delta_o.squared_length();
    if ( a == 0 ) { // ray is parallel to cylinder
        if (delta_o_sq - delta_o_parall*delta_o_parall <= cy_radsq) {  // intersect at top/bottom
            double t_bot = - delta_o_parall / direct_parall;
            double t_top = t_bot + cy_direct.length() / direct_parall;
            t_pos.push_back(t_bot); n_pos.push_back(-cy_unit_direct);
            t_pos.push_back(t_top); n_pos.push_back(cy_unit_direct);
        }
        else return false;
    }
    else {  // not parallel
        // b = 2 ( delta_o dot D - D_parall * delta_o_parall )
        double b = 2*( dot(delta_o, r.direction()) - direct_parall*delta_o_parall );
        // c = delta_o_sq - delta_o_parall^2 - r^2
        double c = delta_o_sq - delta_o_parall*delta_o_parall - cy_radsq;
        double determination = b*b - 4*a*c;
        if ( determination < 0 ) return false; // no intersection
        double t_side_p = (-b+sqrt(determination)) / (2*a);
        double t_side_n = (-b-sqrt(determination)) / (2*a);
        assert(direct_parall&&"pendicular");
        double r_bot_parall = - delta_o_parall;
        double r_top_parall = r_bot_parall + cy_direct.length();
        double t_bot, t_top;
        if ( direct_parall == 0 ) {
            // pendicular
            t_bot = r_bot_parall>0 ? MAXFLOAT : MINFLOAT;
            t_top = r_top_parall>0 ? MAXFLOAT : MINFLOAT;
        }
        else {
            // normal case
            t_bot = - delta_o_parall / direct_parall;
            t_top = t_bot + cy_direct.length() / direct_parall;
        }
        vec3 o_bot = cy_origin;
        vec3 o_top = cy_origin+cy_direct;
        double t_slot_min = fmin(t_top, t_bot);
        double t_slot_max = fmax(t_top, t_bot);
        if ( t_side_p<=t_slot_max 
          && t_side_p>=t_slot_min ) { // t_side_p is possible
            t_pos.push_back(t_side_p);
            n_pos.push_back(unit_vector( cross(
                    cy_direct,
                    cross(r.point_at_parameter(t_side_p)-cy_origin, cy_direct)
                ) 
            ));
        }
        if ( t_side_n<=t_slot_max 
          && t_side_n>=t_slot_min ) { // t_side_n is possible
            t_pos.push_back(t_side_n);
            n_pos.push_back(unit_vector( cross(
                    cy_direct,
                    cross(r.point_at_parameter(t_side_n)-cy_origin, cy_direct)
                ) 
            ));
        }
        if ( (r.point_at_parameter(t_bot)-o_bot).squared_length() <= cy_radsq ) {
            // t_bot is possible
            t_pos.push_back(t_bot);
            n_pos.push_back(-cy_unit_direct);
        }
        if ( (r.point_at_parameter(t_top)-o_top).squared_length() <= cy_radsq ) {
            // t_top is possible
            t_pos.push_back(t_top);
            n_pos.push_back(cy_unit_direct);
        }
    }
    // search valid over all possible(rude implement)
    double t_temp;
    vec3 n_temp;
    bool find_valid=false;
    for (int i = 0; i < t_pos.size(); i++) {
        if ( t_pos[i]<t_max && t_pos[i]>t_min ) {   //valid
            if ( !find_valid || (find_valid && t_pos[i]<t_temp) ) {
                // replace
                t_temp = t_pos[i];
                n_temp = n_pos[i];
            }
            find_valid = true;
        }
    }
    if ( find_valid ) {
        rec.hit_t = t_temp;
        rec.hit_p = r.point_at_parameter(t_temp);
        rec.normal = n_temp;
        rec.mat_ptr = mat_ptr;
        return true;
    }
    return false;
}

bound cylinder::bbox() const {
    return bound(fmin(cy_origin.x(), cy_origin.x()+cy_direct.x()) - cy_rad,
                 fmin(cy_origin.y(), cy_origin.y()+cy_direct.y()) - cy_rad,
                 fmin(cy_origin.z(), cy_origin.z()+cy_direct.z()) - cy_rad,
                 fmax(cy_origin.x(), cy_origin.x()+cy_direct.x()) + cy_rad,
                 fmax(cy_origin.y(), cy_origin.y()+cy_direct.y()) + cy_rad,
                 fmax(cy_origin.z(), cy_origin.z()+cy_direct.z()) + cy_rad
    );
}

#endif