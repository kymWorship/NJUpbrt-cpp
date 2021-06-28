#ifndef MYHDRLOADERH
#define MYHDRLOADERH

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"
#include <string>
#include "../mylib/common.h"
#include "../geometry/geometry.h"

using std::string;

#define ABS_BACKGROUND_PATH string(WorkspaceDir)+"import/background/pics/"
#define HDR_BYTES_PER_CHANNEL 4
#define HDR_CHANNELS_PER_PIXEL 3
#define HDR_BYTES_PER_PIXEL HDR_BYTES_PER_CHANNEL*HDR_CHANNELS_PER_PIXEL

float* mybackgroundhdtloader(string filename,
    int& width, int& height
) {
    string filepath = ABS_BACKGROUND_PATH + filename;
    const char* filechar = filepath.c_str();
    int comp_per_pixel;
    auto image = stbi_loadf(filechar, &width, &height, &comp_per_pixel, 0);
    std::cout<<filepath<<": "<<comp_per_pixel<<std::endl;
    //assert(comp_per_pixel == HDR_BYTES_PER_PIXEL && "not RGB HDR");
    assert(image && "fail to load png\n");
    return image;
}

inline vec3 read_pixel(float* data, int width, int i, int j) {
    auto pixel = data + (j*width+i)*HDR_CHANNELS_PER_PIXEL;
    return vec3(pixel[0], pixel[1], pixel[2]);
}

#endif