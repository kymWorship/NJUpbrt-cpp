// TODO: should add ifndef?
#include "../hitable.h"
#include "../mylib/onb.h"
#include "../mylib/myrand.h"

class sphere: public hitable {
    public:
        sphere() {}
        sphere(vec3 c, float r, shared_ptr<material> m_p): center(c), radius(r), mat_ptr(m_p) {}
        virtual bool hit(const ray& r, float t_min, float t_max, hit_rec & rec) const;
        virtual bound bbox() const ;
        virtual bool is_inside(const vec3& point) const;
        virtual ray ray_gen(const vec3& point) const;
        virtual double pdf_val(const ray& r) const;
        virtual shared_ptr<material> mat() const {return mat_ptr;}
    private:
        vec3 center;
        float radius;
        shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_rec& rec) const {
    // TODO: add uv
    vec3 CO = r.origin() - center;
    double a = r.direction().squared_length();
    double b = 2*dot(CO, r.direction());
    double c = CO.squared_length() - radius*radius;
    double determination = b*b - 4*a*c;
    if ( determination > 0 ) {
        double t_temp = ( - b - sqrt(determination) ) / (2*a);
        if ( t_min<t_temp && t_max>t_temp ) { // come in light
            rec.hit_t = t_temp;
            rec.hit_p = r.point_at_parameter(t_temp);
            rec.normal = (rec.hit_p - center)/radius;// outward normal
            rec.mat_ptr = mat_ptr;
            return true;
        }
        t_temp = ( - b + sqrt(determination) ) / (2*a);
        if ( t_min<t_temp && t_max>t_temp ) { // come out light
            rec.hit_t = t_temp;
            rec.hit_p = r.point_at_parameter(t_temp);
            rec.normal = (rec.hit_p - center)/radius;// outward normal
            rec.mat_ptr = mat_ptr;
            return true;
        } 
    }
    return false;
}

bound sphere::bbox() const {
    vec3 sphereshift(radius, radius, radius);
    return bound(center-sphereshift, center+sphereshift);
}

bool sphere::is_inside(const vec3& point) const {
    vec3 CP = point - center;
    return CP.squared_length() <= radius*radius;
}

ray sphere::ray_gen(const vec3& point) const {
    vec3 OC = center - point;
    onb uvw(OC);
    double cosine_max = sqrt( 1 - radius*radius/OC.squared_length() );
    double r1 = random_double(), r2 = random_double();
    float gz = 1 - r2*(1-cosine_max);
    float grho = sqrt(1-gz*gz);
    float phi = 2*M_PI*r1;
    float gx = grho*cos(phi);
    float gy = grho*sin(phi);
    ray generated(point, uvw.local(gx, gy, gz));
    return generated;
}

double sphere::pdf_val(const ray& r) const {
    vec3 OC = center - r.origin();
    double cosine = dot(OC, r.direction()) / (OC.length()*r.direction().length());
    double cosine_max = sqrt( 1 - radius*radius/OC.squared_length() );
    if ( cosine < cosine_max ) return 0;
    return 1 / (2*M_PI*(1-cosine_max));
}