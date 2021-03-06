#ifndef MYPNGLOADERH
#define MYPNGLOADERH

#include "myloaders.h"
#include <string>
#include "../mylib/common.h"
#include "../geometry/geometry.h"

using std::string;
// TODO: commend this
using std::cout;
using std::endl;



#define ABS_MODEL_PATH string(WorkspaceDir)+"import/model/"
#define ABS_TEXTURE_PATH string(WorkspaceDir)+"import/texture/png/"
#define PNG_BYTES_PER_CHANNEL 1
#define PNG_CHANNELS_PER_PIXEL 3
#define PNG_BYTES_PER_PIXEL PNG_BYTES_PER_CHANNEL*PNG_CHANNELS_PER_PIXEL

// TODO: using CLASS!

unsigned char* mymodelpngloader(string filename, 
    int& width, int& height
) {
    string filepath = ABS_MODEL_PATH + filename;
    const char* filechar = filepath.c_str();
    int comp_per_pixel;
    auto image = stbi_load(filechar, &width, &height, &comp_per_pixel, 0);
    // DEBUG: commend this
    // cout<<filepath<<':'<<comp_per_pixel<<endl;
    assert(comp_per_pixel == PNG_BYTES_PER_PIXEL && "not RGB png");
    assert(image && "fail to load png\n");
    return image;
}

unsigned char* mytexturepngloader(string filename,
    int& width, int& height
) {
    string filepath = ABS_TEXTURE_PATH + filename;
    const char* filechar = filepath.c_str();
    int comp_per_pixel;
    auto image = stbi_load(filechar, &width, &height, &comp_per_pixel, 0);
    // DEBUG: commend this
    // cout<<filepath<<':'<<comp_per_pixel<<endl;
    assert(comp_per_pixel == PNG_BYTES_PER_PIXEL && "not RGB png");
    assert(image && "fail to load png\n");
    return image;
}

inline vec3 read_pixel(unsigned char* data, int width, int i, int j) {
    auto pixel = data + (j*width+i)*PNG_CHANNELS_PER_PIXEL;
    return vec3(pixel[0] , pixel[1], pixel[2]) / 255.0;
}

#endif