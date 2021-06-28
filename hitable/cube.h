#ifndef CUBEH
#define CUBEH

#include "../hitable.h"

/*
    This is infact Parallelepiped as
        cor define one corner of it
        a, b, c define three direction
*/

class cube:public hitable {
    public:
        cube() {}
        cube(
            vec3 cor, vec3 a, vec3 b, vec3 c,
            shared_ptr<material> m_p
        ):
            corner(cor),
            edgea(a), edgeb(b), edgec(c) {
                normalab = cross(a, b);
                normalbc = cross(b, c);
                normalca = cross(c, a);
                lambab = dot(c, normalab);
                lambbc = dot(a, normalbc);
                lambca = dot(b, normalca);
                if ( lambab<0 ) {
                    lambab = -lambab;
                    normalab = -normalab;
                }
                if ( lambbc<0 ) {
                    lambbc = -lambbc;
                    normalbc = -normalbc;
                }
                if (lambca<0 ) {
                    lambca = -lambca;
                    normalca = -normalca;
                }
                mat_ptr.reserve(6);
                mat_ptr[0]=m_p;
                mat_ptr[1]=m_p;
                mat_ptr[2]=m_p;
                mat_ptr[3]=m_p;
                mat_ptr[4]=m_p;
                mat_ptr[5]=m_p;
            }
        cube(
            vec3 cor, vec3 a, vec3 b, vec3 c,
            shared_ptr<material> mab0, shared_ptr<material> mab1,
            shared_ptr<material> mbc0, shared_ptr<material> mbc1,
            shared_ptr<material> mca0, shared_ptr<material> mca1
        ):
            corner(cor),
            edgea(a), edgeb(b), edgec(c) {
                normalab = cross(a, b);
                normalbc = cross(b, c);
                normalca = cross(c, a);
                lambab = dot(c, normalab);
                lambbc = dot(a, normalbc);
                lambca = dot(b, normalca);
                if ( lambab<0 ) {
                    lambab = -lambab;
                    normalab = -normalab;
                }
                if ( lambbc<0 ) {
                    lambbc = -lambbc;
                    normalbc = -normalbc;
                }
                if (lambca<0 ) {
                    lambca = -lambca;
                    normalca = -normalca;
                }
                mat_ptr.reserve(6);
                mat_ptr[0]=mab0;
                mat_ptr[1]=mab1;
                mat_ptr[2]=mbc0;
                mat_ptr[3]=mbc1;
                mat_ptr[4]=mca0;
                mat_ptr[5]=mca1;
            }
        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_rec& rec
        ) const;
        virtual bound bbox() const;
        virtual bool is_inside(const vec3& point) const;
        virtual shared_ptr<material> mat() const;
        // TODO: pdf
    private:
        vec3 corner;
        vec3 edgea;
        vec3 edgeb;
        vec3 edgec;
        vec3 normalab;
        vec3 normalbc;
        vec3 normalca;
        double lambab;  // >0
        double lambbc;  // >0
        double lambca;  // >0
        vector<shared_ptr<material>> mat_ptr;
};

bool cube::hit(
    const ray& r, float t_min, float t_max, hit_rec& rec
) const {
    // TODO: add uv
    vec3 r_cor = corner - r.origin();
    float d_ab0 = dot(r_cor, normalab), d_ab1 = d_ab0 + lambab;
    float d_bc0 = dot(r_cor, normalbc), d_bc1 = d_bc0 + lambbc;
    float d_ca0 = dot(r_cor, normalca), d_ca1 = d_ca0 + lambca;
    float unitab = dot(r.direction(), normalab);
    float unitbc = dot(r.direction(), normalbc);
    float unitca = dot(r.direction(), normalca);
    float t_sur[6];
    //avoid the case that direction perp to the three normal
    if ( unitab == 0 ) {
        if ( d_ab0*d_ab1 <= 0 ) t_sur[0] = t_min, t_sur[1] = t_max;
        else return false;
    }
    if ( unitbc == 0 ) {
        if ( d_bc0*d_bc1 <= 0 ) t_sur[2] = t_min, t_sur[3] = t_max;
        else return false;
    }
    if ( unitca == 0 ) {
        if ( d_ca0*d_ca1 <= 0 ) t_sur[4] = t_min, t_sur[5] = t_max;
        else return false;
    }
    //assert( (unitab*unitbc*unitca) && "direction perp to normal" );
    t_sur[0] = d_ab0 / unitab;
    t_sur[1] = d_ab1 / unitab;
    t_sur[2] = d_bc0 / unitbc;
    t_sur[3] = d_bc1 / unitbc;
    t_sur[4] = d_ca0 / unitca;
    t_sur[5] = d_ca1 / unitca;
    hit_rec temp_rec;
    temp_rec.hit_t = t_max;
    bool is_hit = false;
    vec3 temp_nor_list[6] = {  
        -unit_vector(normalab), unit_vector(normalab),
        -unit_vector(normalbc), unit_vector(normalbc),
        -unit_vector(normalca), unit_vector(normalca)
    };
    for (int i = 0; i < 6; i++) {
        if (
                (t_sur[i] - t_sur[0])*(t_sur[i] - t_sur[1]) <= 0
            &&  (t_sur[i] - t_sur[2])*(t_sur[i] - t_sur[3]) <= 0
            &&  (t_sur[i] - t_sur[4])*(t_sur[i] - t_sur[5]) <= 0
            &&   t_sur[i] < temp_rec.hit_t
            &&   t_sur[i] > t_min
        ) {
            temp_rec.hit_t = t_sur[i];
            temp_rec.normal = temp_nor_list[i];
            temp_rec.mat_ptr = mat_ptr[i];
            is_hit = true;
        }
    }
    if (is_hit) {
        temp_rec.hit_p = r.point_at_parameter(temp_rec.hit_t);
        rec = temp_rec;
    }
    return is_hit;
}

bound cube::bbox() const {
    vec3 rmin(corner), rmax(corner), rtemp;
    for (uint8_t i = 1; i < 8; i++ ) {
        rtemp = corner + (i & 0x01)*edgea + ((i & 0x02)>>1)*edgeb + ((i & 0x04)>>2)*edgec;
        rmin.e[0] = fmin(rmin.x(), rtemp.x());
        rmin.e[1] = fmin(rmin.y(), rtemp.y());
        rmin.e[2] = fmin(rmin.z(), rtemp.z());
        rmax.e[0] = fmax(rmax.x(), rtemp.x());
        rmax.e[1] = fmax(rmax.y(), rtemp.y());
        rmax.e[2] = fmax(rmax.z(), rtemp.z());
    }
    return bound(rmin, rmax);
}


bool cube::is_inside(const vec3& point) const{
    vec3 cor_to_point = point - corner;
    return dot(cor_to_point, normalab)>=0 && dot(cor_to_point, normalab) <= lambab
        && dot(cor_to_point, normalbc)>=0 && dot(cor_to_point, normalbc) <= lambbc
        && dot(cor_to_point, normalca)>=0 && dot(cor_to_point, normalca) <= lambca;
}

// bad code
shared_ptr<material> cube::mat() const {
    for (auto m_p:mat_ptr) {
        if ( m_p->is_sampling() ) {
            assert(0 && "running bad code, should check");
            return m_p;
        }
    }
    return mat_ptr[0];
}


#endif