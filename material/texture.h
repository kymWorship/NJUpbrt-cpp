#ifndef TEXTUREH
#define TEXTUREH

#include "../loader/myloaders.h"

class texture {
    public:
        virtual vec3 get_texture(float u, float v) const = 0;
};

class const_texture: public texture {
    public:
        ~const_texture() {}
        const_texture(vec3 ct): texture_color(ct){}
        virtual vec3 get_texture(float u, float v) const {
            return texture_color;
        }
    private:
        vec3 texture_color;
};

class png_texture: public texture {
    public:
        ~png_texture() {stbi_image_free(pixels);}
        png_texture(string filename) {
            pixels = mymodelpngloader(filename, width, height);
        }
        virtual vec3 get_texture(float u, float v) const {
            int i(u*width), j((1-v)*height);
            if(i==width) i--;
            if(j==height) j--;
            DASSERT(i<width&&j<height, "wrong uv");
            auto temp =  read_pixel(pixels, width, i, j);
            //cout<< temp<<endl;
            return temp;
        }
    private:
        unsigned char* pixels;
        int width;
        int height;
};


#endif