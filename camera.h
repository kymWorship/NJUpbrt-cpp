#ifndef CAMERAH
#define CAMERAH

#include "mylib/common.h"
#include "geometry/geometry.h"

class camera {
    public:
        camera() {};
        /* 
        look_from: point of camera; 
        look_at: center of view; 
        hfov: in degree; 
        ratio: nx/ny
        */
        camera(vec3 look_from, vec3 look_at, vec3 vup, float hfov, float ratio):
            lens_radius(0)
        {
            cam_origin = look_from;
            vec3 center_ray = look_at - look_from;
            double focus_dist = center_ray.length();
            double w = focus_dist*2*tan(M_PI*hfov/360);
            double h = w/ratio;
            unit_horizen  = unit_vector( cross(center_ray, vup) );
            unit_vertical = unit_vector( cross(center_ray, unit_horizen) );
            horizen =  w*unit_horizen;
            vertical = h*unit_vertical;
            up_left_corner = cam_origin + center_ray - 0.5*horizen - 0.5*vertical; 
        }
        camera(vec3 look_from, vec3 look_at, vec3 vup,
            float hfov, float ratio, float radius
        ): lens_radius(radius) 
        {
            cam_origin = look_from;
            vec3 center_ray = look_at - look_from;
            double focus_dist = center_ray.length();
            double w = focus_dist*2*tan(M_PI*hfov/360);
            double h = w/ratio;
            unit_horizen  = unit_vector( cross(center_ray, vup) );
            unit_vertical = unit_vector( cross(center_ray, unit_horizen) );
            horizen =  w*unit_horizen;
            vertical = h*unit_vertical;
            up_left_corner = cam_origin + center_ray - 0.5*horizen - 0.5*vertical; 
            #if !DepthOfField
            if( lens_radius != 0 ) {
                cout<<"Please set DepthOfField to 'true' first in order to use depth of field"<<endl;
            }
            #endif
        }

        ray get_ray(float rh, float rv) {
            #if DepthOfField
            auto rand_vec2_on_lens = lens_radius*rand_in_unit_circle();
            vec3 lens_offset = unit_horizen  * rand_vec2_on_lens.x()
                             + unit_vertical * rand_vec2_on_lens.y();
            return ray(
                cam_origin + lens_offset,
                up_left_corner + rh*horizen + rv*vertical - cam_origin - lens_offset
            );
            #else
            return ray(cam_origin, up_left_corner + rh*horizen + rv*vertical - cam_origin );
            #endif
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
        float lens_radius;
        vec3 unit_horizen, unit_vertical;
};

#endif