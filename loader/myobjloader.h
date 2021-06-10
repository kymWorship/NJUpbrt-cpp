#ifndef MYOBJLOADERH
#define MYOBJLOADERH

#include "../geometry/geometry.h"
#include "../hitable.h"
#include "../material.h"
#include <cstdio>
#include <cassert>

#define REL_MODEL_PATH "model/"


struct texture_dict {
    shared_ptr<texture> dict_text;
    char name[128];
};

bool myobjloader(string filename, vec3 translation,
    float gain, vector<shared_ptr<hitable>>& hit_list
) { // filename shouldn't have suffix!
    // init buf
    vector<vec3> vertices;
    vector<float> vtu;
    vector<float> vtv;
    vertices.clear();
    vtu.clear(); vtv.clear();
    vector<texture_dict> mtldict;
    mtldict.clear();

    // init var 
    shared_ptr<material> cur_mat = make_shared<lambertian>(vec3(0.9, 0.1, 0.6));
    shared_ptr<material> temp_mat = make_shared<lambertian>(vec3(0.9, 0.1, 0.6));

    // open obj file
    string objpath = REL_MODEL_PATH + filename + ".obj";
    const char * objpath_char = objpath.c_str();
    FILE* objfile = fopen(objpath_char, "r");
    assert( objfile != NULL && "open obj file failed");
    // TODO: delete this
    cout<<"obj file opened\n";

    // open mtlfile
    string mtlpath = REL_MODEL_PATH + filename + ".mtl";
    const char* mtlpath_char = mtlpath.c_str();
    FILE* mtlfile = fopen(mtlpath_char, "r");
    assert( mtlfile != NULL && "open mtl file failed" );

    // load mtl file first
    while (true) {
        char lineprefix[128];
        int res = fscanf(mtlfile, "%s", lineprefix);
        if ( res == EOF ) break;
        if ( strcmp(lineprefix, "newmtl") == 0 ) {
            texture_dict td;
            fscanf(mtlfile, "%s\n", td.name);
            mtldict.push_back(td);
        }
        if ( strcmp(lineprefix, "map_Kd") == 0 ) {
            char rel_path[128];
            fscanf(mtlfile, "%s\n", rel_path);
            mtldict.back().dict_text = make_shared<png_texture>(string(rel_path));
        }
    }
    cout<<mtldict.size()<<" textures loaded from "<<filename<<endl;
    clog<<mtldict.size()<<" textures loaded from "<<filename<<endl;

    // load obj file
    int trianglenum =  0;
    while (true) {
        char lineprefix[128];
        int res = fscanf(objfile, "%s", lineprefix);
        if ( res == EOF ) break;
        if ( strcmp(lineprefix, "v") == 0 ) {
            // push vertex
            float x0, y0, z0;
            fscanf(objfile, "%f %f %f\n", &x0, &y0, &z0);
            vertices.push_back(vec3(x0, y0, z0)*gain + translation);
            continue;
        }
        if (strcmp(lineprefix, "vt") == 0 ) {
            // push uv
            float u0, v0;
            fscanf(objfile, "%f %f\n", &u0, &v0);
            vtu.push_back(u0);
            vtv.push_back(v0);
        }
        if (strcmp(lineprefix, "usemtl") == 0 ) {
            char cur_name[128];
            fscanf(objfile, "%s\n", cur_name);
            bool find_texture = false;
            for (auto dict: mtldict) {
                if ( strcmp(cur_name, dict.name) == 0 ) {
                    cur_mat = make_shared<lambertian>(dict.dict_text);
                    find_texture = true;
                    break;
                }
            }
            assert(find_texture && "texture not found!");
        }
        if ( strcasecmp(lineprefix, "f") == 0 ) {
            unsigned int verind[3], uvind[3], norind[3];
            int matches = fscanf(objfile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                &verind[0], &uvind[0], &norind[0],
                &verind[1], &uvind[1], &norind[1],
                &verind[2], &uvind[2], &norind[2]);
            assert( matches == 9 && "load obj failed");
            // TODO: temp_mat -> cur_mat
            hit_list.push_back(make_shared<triangle>(
                vertices[verind[0] - 1],
                vertices[verind[1] - 1],
                vertices[verind[2] - 1],
                cur_mat,
                vec3(vtu[uvind[0]-1], vtu[uvind[1]-1], vtu[uvind[2]-1]),
                vec3(vtv[uvind[0]-1], vtv[uvind[1]-1], vtv[uvind[2]-1])
            ));
            trianglenum ++;
        }
    }
    clog<<objpath<<"loaded, "<<trianglenum<<"triangles added\n";
    // TODO: delete this
    cout<<"02loaded, "<<trianglenum<<"triangles added\n";
    return true;
}

#endif