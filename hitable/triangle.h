#ifndef TRIANGLEH
#define TRIANGLEH

#include "../hitable.h"
#include "../mylib/mycode.h"

/*
    Standard and elementary hitable type (Triangle) 
    in 3D CG, which is defined with
        point A, B, C: three vertice in right-handed 
            order
        uv: uv of three vertice
    - can be used as source (approximation)
    - can applied with texture
*/

class triangle: public hitable {
    public:
        triangle (vec3 pointA, vec3 pointB, vec3 pointC, shared_ptr<material> m_p):
            rA(pointA), rB(pointB), rC(pointC), mat_ptr(m_p) {
                P_lamb = rC - rA;
                P_mu = rC - rB;
                area_vec = cross(P_lamb, P_mu);
                area = area_vec.length() / 2;
                normal = unit_vector(area_vec);
                vecu = vec3(0, 0, 0);
                vecv = vec3(0, 0, 0);
                uvinited = false;
            }
        triangle (vec3 pointA, vec3 pointB, vec3 pointC, 
        shared_ptr<material> m_p, vec3 u, vec3 v)
            : rA(pointA), rB(pointB), rC(pointC),
              mat_ptr(m_p), vecu(u), vecv(v) {
                P_lamb = rC - rA;
                P_mu = rC - rB;
                area_vec = cross(P_lamb, P_mu);
                area = area_vec.length() / 2;
                normal = unit_vector(area_vec);
                uvinited = true;
            }
        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_rec& rec
        ) const ;
        virtual bound bbox() const;
        virtual bool is_inside(const vec3& point) const;
        virtual ray ray_gen (const vec3& point) const;
        virtual double pdf_val (const ray& r) const;
        virtual shared_ptr<material> mat() const { return mat_ptr; }

    private:
        vec3 rA, rB, rC, P_lamb, P_mu; // righthanded
        vec3 vecu, vecv; // 3 direct: ABC seperately
        bool uvinited;
        vec3 normal;    // normal is init as unit vec, right-handed
        vec3 area_vec;
        double area;
        shared_ptr<material> mat_ptr;
        inline vec3 point_at_para(double lamb, double mu) const {
            return lamb*rA + mu*rB + (1-lamb-mu)*rC;
        }
};


bool triangle::hit(
    const ray& r, float t_min, float t_max, hit_rec& rec
) const {
    // D*t + (C - B) mu + (C - A) lamb = C - O
    vec3 P_t = r.direction();
    vec3 P_0 = rC - r.origin();
    double determination = dot(P_t, area_vec);
    if ( determination == 0 ) return false;
    double det_t = dot(P_0, area_vec);
    vec3 temp_vec = cross(P_t, P_0);
    double det_lamb = dot(P_mu, temp_vec);
    double det_mu = - dot(P_lamb, temp_vec);
    double temp_t = det_t / determination;
    double lamb = det_lamb / determination;
    double mu = det_mu / determination;
    if (temp_t < t_max && temp_t > t_min 
        &&  mu   <= 1  && mu   >= 0
        &&  lamb <= 1  && lamb >= 0
        &&  (lamb + mu) <= 1
    ) { // hit the triangle
        rec.hit_t = temp_t;
        rec.hit_p = r.point_at_parameter(temp_t);
        rec.normal = normal;
        rec.mat_ptr = mat_ptr;
        if(uvinited) {
            rec.u = lamb*vecu.x() + mu*vecu.y() + (1-mu-lamb)*vecu.z();
            rec.v = lamb*vecv.x() + mu*vecv.y() + (1-mu-lamb)*vecv.z();
        }
        return true;
    }
    return false;
}

bound triangle::bbox() const {
    return bound( Fmin(rA.x(), rB.x(), rC.x()),
                  Fmin(rA.y(), rB.y(), rC.y()),
                  Fmin(rA.z(), rB.z(), rC.z()),
                  Fmax(rA.x(), rB.x(), rC.x()),
                  Fmax(rA.y(), rB.y(), rC.y()),
                  Fmax(rA.z(), rB.z(), rC.z())
    );
}

bool triangle::is_inside(const vec3& point) const {
    assert(0&&"warning");
    return dot(rA - point, normal) >= 0;
}

ray triangle::ray_gen(const vec3& point) const {
    double rand1 = random_double();
    double rand2 = random_double();
    double rand3 = random_double();
    double temp_rand;
    if (rand3 > rand2) in_place_switch(rand2, rand3);
    if (rand2 > rand1) in_place_switch(rand1, rand2);
    if (rand3 > rand2) in_place_switch(rand2, rand3);
    return ray(point, point_at_para(rand1-rand2, rand2-rand3) - point);
}

double triangle::pdf_val(const ray& r) const {
    hit_rec temp_hrec;
    if (hit(r, 0.001, MAXFLOAT, temp_hrec)) {
        vec3 dist = temp_hrec.hit_p - r.origin();
        double cosine = fabs( dot(unit_vector(dist), normal) );
        return dist.squared_length() / (area*cosine);
    }
    return 0;
}



#endif