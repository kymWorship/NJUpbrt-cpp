#ifndef DISCH
#define DISCH

#include "../hitable.h"
#include "../mylib/mycode.h"

class disc: public hitable {
    public:
        disc(vec3 disc_center, vec3 arbitary_len_normal,
        double disc_radius, shared_ptr<material> m_p):
            center(disc_center), mat_ptr(m_p), radius(disc_radius),
            normal(unit_vector(arbitary_len_normal)) 
            {
                radius_sq = radius*radius;
                area = M_PI*radius_sq;
                uvw = onb(normal);
            }
        virtual bool hit (
            const ray& r, float t_min, float t_max,
            hit_rec& rec
        ) const;
        virtual bound bbox() const;
        virtual bool is_inside(const vec3& point) const;
        virtual ray ray_gen (const vec3& point) const;
        virtual double pdf_val (const ray& r) const;
        virtual shared_ptr<material> mat() const {return mat_ptr;}

    private:
        vec3 center, normal;
        double radius, radius_sq;
        double area;
        onb uvw;
        shared_ptr<material> mat_ptr;
};

bool disc::hit(
    const ray& r, float t_min, float t_max, hit_rec& rec
) const {
    hit_rec temp_rec;
    vec3 OC = center - r.origin();
    double direct_n = dot(r.direction(), normal);
    if (direct_n == 0) return false;
    temp_rec.hit_t = dot(OC, normal) / direct_n;
    temp_rec.hit_p = r.point_at_parameter(temp_rec.hit_t);
    if (temp_rec.hit_t < t_max && temp_rec.hit_t > t_min
        && (temp_rec.hit_p - center).squared_length() <= radius_sq
    ) { // hit the disc
        rec = temp_rec;
        rec.normal = normal;
        rec.mat_ptr = mat_ptr;
        return true;
    }
    return false;
}

bound disc::bbox() const {
    double delta_x = radius * sqrt(1-normal.x()*normal.x());
    double delta_y = radius * sqrt(1-normal.y()*normal.y());
    double delta_z = radius * sqrt(1-normal.z()*normal.z());
    return bound (
        center.x() - delta_x, center.y() - delta_y, center.z() - delta_z,
        center.x() + delta_x, center.y() + delta_y, center.z() + delta_z
    );
}

bool disc::is_inside(const vec3& point) const {
    assert(0 && "warning");
    return dot(center - point, normal) >= 0;
}

ray disc::ray_gen(const vec3& point) const {
    vec2 rand_cir = radius * rand_in_unit_circle();
    vec3 rand_p = center + uvw.local(rand_cir.x(), rand_cir.y(), 0);
    return ray(point, rand_p - point);
}

double disc::pdf_val(const ray& r) const {
    hit_rec temp_hrec;
    if (hit(r, 0.001, MAXFLOAT, temp_hrec)) {
        vec3 dist = temp_hrec.hit_p - r.origin();
        double cosine = fabs( dot(unit_vector(dist), normal) );
        return dist.squared_length() / (area*cosine);
    }
    return 0;
}

#endif