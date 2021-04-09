#ifndef BOUNDH
#define BOUNDH
// currently stand for aabb class
#include "ray.h"
#include "mylib/common.h"

class bound {
    public:
        bound(vec3 vecmin, vec3 vecmax){
            rmin[0]=vecmin.x(); rmax[0]=vecmax.x();
            rmin[1]=vecmin.y(); rmax[1]=vecmax.y();
            rmin[2]=vecmin.z(); rmax[2]=vecmax.z();
        } 
        bound(float xmin, float ymin, float zmin,
            float xmax, float ymax, float zmax
        )  {rmin[0]=xmin; rmin[1]=ymin; rmin[2]=zmin;
            rmax[0]=xmax; rmax[1]=ymax; rmax[2]=zmax; }
        bound(vec3 point) {
            rmin[0]=point.x(); rmax[0]=point.x();
            rmin[1]=point.y(); rmax[1]=point.y();
            rmin[2]=point.z(); rmax[2]=point.z();
        }
        bound() {
            rmin[0] = MAXFLOAT; rmax[0] = MINFLOAT;
            rmin[1] = MAXFLOAT; rmax[1] = MINFLOAT;
            rmin[2] = MAXFLOAT; rmax[2] = MINFLOAT;
        }
        void check() {
            assert( rmin[0] <= rmax[0]
                &&  rmin[1] <= rmax[1]
                &&  rmin[2] <= rmax[2]
                &&  "wrong bounding box!");
        }
        bool hit(
            const ray& r, float t_min, float t_max,
            const vec3& inv_dir, const bool is_pos[3]
        ) const;
        bool fake_hit(
            const ray& r, float t_min, float t_max,
            const vec3& inv_dir, const bool is_pos[3]
        ) const;
        inline vec3 centroid() const;
        int maxextend() const;
        double half_area() const;
        inline const float* first_to_hit(bool ispos) const {
            return ispos ? rmin : rmax;
        }
        float rmin[3];
        float rmax[3];
};

bool bound::hit(
    const ray& r, float t_min, float t_max,
    const vec3& inv_dir, const bool is_pos[3]
) const {
    // TODO: timing 'early rejection'
    // TODO: check the need to enlarge temp_max
     float temp_min = (first_to_hit( is_pos[0])[0] - r.origin().x()) * inv_dir[0];
     float temp_max = (first_to_hit(!is_pos[0])[0] - r.origin().x()) * inv_dir[0];
     float yt_min = (first_to_hit( is_pos[1])[1] - r.origin().y()) * inv_dir[1];
     float yt_max = (first_to_hit(!is_pos[1])[1] - r.origin().y()) * inv_dir[1];

    // comp x & y
    if (yt_min > temp_max || yt_max < temp_min) return false;
    if ( yt_min > temp_min ) temp_min = yt_min;
    if ( yt_max < temp_max ) temp_max = yt_max;
    // TODO: testing: if(temp_min > temp_max) return false;

    float zt_min = (first_to_hit( is_pos[2])[2] - r.origin().z()) * inv_dir[2];
    float zt_max = (first_to_hit(!is_pos[2])[2] - r.origin().z()) * inv_dir[2];

    if (zt_min > temp_max || zt_max < temp_min) return false;
    if ( zt_min > temp_min ) temp_min = zt_min;
    if ( zt_max < temp_max ) temp_max = zt_max;

    return temp_max > t_min && temp_min < t_max;
}

bool bound::fake_hit(
    const ray& r, float t_min, float t_max,
    const vec3& inv_dir, const bool is_pos[3]
) const {
    // TODO: timing 'early rejection'
    // TODO: check the need to enlarge temp_max
    float temp_min, temp_max, yt_min, yt_max;
    if ( inv_dir[0] ) {
        temp_min = (first_to_hit( is_pos[0])[0] - r.origin().x()) * inv_dir[0];
        temp_max = (first_to_hit(!is_pos[0])[0] - r.origin().x()) * inv_dir[0];
    } else {
        if ( r.origin().x() < rmin[0] || r.origin().x() > rmax[0] ) return false;
        temp_min = MINFLOAT;
        temp_max = MAXFLOAT;
    }
    if ( inv_dir[1] ) {
        yt_min = (first_to_hit( is_pos[1])[1] - r.origin().y()) * inv_dir[1];
        yt_max = (first_to_hit(!is_pos[1])[1] - r.origin().y()) * inv_dir[1];
    } else {
        if ( r.origin().y() < rmin[1] || r.origin().y() > rmax[1] ) return false;
        yt_min = MINFLOAT;
        yt_max = MAXFLOAT;
    }

    // comp x & y
    if (yt_min > temp_max || yt_max < temp_min) return false;
    if ( yt_min > temp_min ) temp_min = yt_min;
    if ( yt_max < temp_max ) temp_max = yt_max;
    // TODO: testing: if(temp_min > temp_max) return false;

    float zt_min, zt_max;
    if ( inv_dir[2] ) {
        zt_min = (first_to_hit( is_pos[2])[2] - r.origin().z()) * inv_dir[2];
        zt_max = (first_to_hit(!is_pos[2])[2] - r.origin().z()) * inv_dir[2];
    } else {
        if ( r.origin().z() < rmin[2] || r.origin().z() > rmax[2] ) return false;
        zt_min = MINFLOAT;
        zt_max = MAXFLOAT;
    }

    if (zt_min > temp_max || zt_max < temp_min) return false;
    if ( zt_min > temp_min ) temp_min = zt_min;
    if ( zt_max < temp_max ) temp_max = zt_max;

    return temp_max > t_min && temp_min < t_max;
}

inline vec3 bound::centroid() const {
    return vec3( (rmin[0]+rmax[0]) / 2,
                 (rmin[1]+rmax[1]) / 2,
                 (rmin[2]+rmax[2]) / 2);
}

inline bound Union(bound b1, bound b2) {
    return bound(
        fmin(b1.rmin[0], b2.rmin[0]),
        fmin(b1.rmin[1], b2.rmin[1]),
        fmin(b1.rmin[2], b2.rmin[2]),
        fmax(b1.rmax[0], b2.rmax[0]),
        fmax(b1.rmax[1], b2.rmax[1]),
        fmax(b1.rmax[2], b2.rmax[2])
    );
}

inline bound Union(bound b, vec3 point) {
    return bound(
        fmin(b.rmin[0], point.x()),
        fmin(b.rmin[1], point.y()),
        fmin(b.rmin[2], point.z()),
        fmax(b.rmax[0], point.x()),
        fmax(b.rmax[1], point.y()),
        fmax(b.rmax[2], point.z())
    );
}

inline bound Intersection(bound b1, bound b2) {
    return bound(
        fmax(b1.rmin[0], b2.rmin[0]),
        fmax(b1.rmin[1], b2.rmin[1]),
        fmax(b1.rmin[2], b2.rmin[2]),
        fmin(b1.rmax[0], b2.rmax[0]),
        fmin(b1.rmax[1], b2.rmax[1]),
        fmin(b1.rmax[2], b2.rmax[2])
    );
}

int bound::maxextend() const {
    int dim;
    float ext;
    float ext0(rmax[0] - rmin[0]),
          ext1(rmax[1] - rmin[1]),
          ext2(rmax[2] - rmin[2]);
    if ( ext0 > ext1 ) { ext = ext0; dim = 0; }
    else { ext = ext1; dim = 1; }
    if ( ext2 > ext ) { dim = 2; /* ext = ext2 */ }
    return dim;
}

double bound::half_area() const {
    // 
    float a(rmax[0] - rmin[0]),
          b(rmax[1] - rmin[1]),
          c(rmax[2] - rmin[2]);
    return a*b + b*c + c*a;
}


#endif