#ifndef CAMERAH
#define CAMERAH

#include "vec3.h"
#include "ray.h"

class camera {
    public:
        camera() {};
        /* 
        look_from: point of camera; 
        look_at: center of view; 
        hfov: in degree; 
        ratio: nx/ny
        */
        camera(vec3 look_from, vec3 look_at, vec3 vup, float hfov, float ratio) {
            cam_origin = look_from;
            vec3 center_ray = unit_vector( look_at - look_from );
            float w = 2*tan(M_PI*hfov/360);
            float h = w/ratio;
            horizen = w*unit_vector( cross(center_ray, vup) );
            vertical = h*unit_vector( cross(center_ray, horizen) );
            up_left_corner = cam_origin + center_ray - 0.5*horizen - 0.5*vertical; 
        }

        ray get_ray(float rh, float rv) {
            return ray(cam_origin, up_left_corner + rh*horizen + rv*vertical - cam_origin );
        }


    private:
        /*
         * ---------> horizen
         | \                 |
         |   up_left_corner  |
         |                   |
         v vertical----------
        */
        vec3 cam_origin;
        vec3 up_left_corner;
        vec3 horizen;
        vec3 vertical;
};

#endif