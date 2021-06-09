#ifndef MYPNGLOADERH
#define MYPNGLOADERH

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"
#include <string>
#include "../mylib/common.h"
#include "../vec3.h"

using std::string;
// TODO: commend this
using std::cout;
using std::endl;



#define ABS_MODEL_PATH string(WorkspaceDir)+"model/"
#define BYTES_PER_PIXEL 3


unsigned char* mymodelpngloader(string filename, 
    int& width, int& height
) {
    string filepath = ABS_MODEL_PATH + filename;
    const char* filechar = filepath.c_str();
    int comp_per_pixel;
    auto image = stbi_load(filechar, &width, &height, &comp_per_pixel, 0);
    // DEBUG: commend this
    // cout<<filepath<<':'<<comp_per_pixel<<endl;
    assert(comp_per_pixel == BYTES_PER_PIXEL && "not RGB png");
    assert(image && "fail to load png\n");
    return image;
}

inline vec3 read_pixel(unsigned char* data, int width, int i, int j) {
    auto pixel = data + (j*width+i)*BYTES_PER_PIXEL;
    return vec3(pixel[0] , pixel[1], pixel[2]) / 255.0;
}

#endif