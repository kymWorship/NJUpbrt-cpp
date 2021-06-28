#ifndef BACKGROUNDH
#define BACKGROUNDH

#include "geometry/geometry.h"
#include "mylib/settings.h"

class background {
    public:
        ~background() {stbi_image_free(pixels);}
        background(string imageFilename = "rnl_probe.hdr") {
            #if USING_ENVIRONMENT_LIGHT
            pixels = mybackgroundhdrloader(imageFilename, width, height);
            #endif
            std::cout<<"Background inited\n";
            std::clog<<"Background inited\n";

        }
        vec3 get_color(const vec3& direction) const;

    private:
        float* pixels;
        int width;
        int height;
        const vec3 forward_vec = vec3(1, 0, 0);
        const vec3 up_vec = vec3(0, 1, 0);
        const vec3 third_vec = cross(forward_vec, up_vec);
        vec3 environment_color(const vec3& direction) const;
        inline vec3 default_color(const vec3& direction) const;
};

vec3 background::get_color(const vec3& direction) const{
    #if bg_light
        #if USING_ENVIRONMENT_LIGHT
            return environment_color(direction);
        #else
            return default_color(direction);
        #endif
    #else
        #if USING_ENVIRONMENT_LIGHT
            std::cout<<"warning: turn on bg_light to use environment light"<<std::endl;
        #endif
        return vec3(0, 0, 0);
    #endif
}

inline vec3 background::default_color(const vec3& direction) const {
    vec3 unit_direction = unit_vector(direction);
    float t = 0.5*(unit_direction.y() + 1.0);
    return dark_fac*( (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0) );
}

vec3 background::environment_color(const vec3& direction) const {
    auto unit_direction = unit_vector(direction);
    auto cosine_theta = dot(forward_vec, unit_direction);
    auto rho = acos(cosine_theta) / M_PI;// [0, 1]
    auto dir_in_xy = unit_direction - cosine_theta*forward_vec;
    auto unit_dir_in_xy = (dir_in_xy.squared_length()) ? unit_vector(dir_in_xy) : dir_in_xy;
    auto u = (dot(unit_dir_in_xy, third_vec)*rho + 1) / 2;
    auto v = (dot(unit_dir_in_xy, up_vec)*rho + 1) / 2;
    int i(u*width), j((1-v)*height);
    if(i==width) i--;
    if(j==height) j--;
    DASSERT(i<width&&j<height, "wrong uv");
    return read_pixel(pixels, width, i, j);
}

#endif