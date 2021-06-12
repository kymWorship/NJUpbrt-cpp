#ifndef SCENEH
#define SCENEH

#include "hitable.h"
#include "material.h"
#include "loader/myobjloader.h"

shared_ptr<hitable> myfirstscene() {
    vector<shared_ptr<hitable>> h_list;
    h_list.clear();
    //hitable* h_list[4];
    //lambertian sphere
    h_list.push_back( make_shared<sphere>( vec3(1, -1.5, -4), 0.5, make_shared<lambertian>(vec3(0.7,0.2,0.7)) ) );
    // metal sphere
    h_list.push_back( make_shared<sphere>( vec3(0.5, -0.5, -9), 1.5, make_shared<metal>(vec3(1.0, 0.9, 0.2), 0.1)) );
    // source sphere
    h_list.push_back( make_shared<sphere>( vec3(-1, -1, -5), 1.0, make_shared<source>(vec3(1,0.5,1)) ) );
    // mirror plate
    h_list.push_back( make_shared<sphere>( vec3(-10004, 2, -5), 10000, make_shared<metal>(vec3(1.0, 1.0, 1.0), 0)) );
    // the ground
    h_list.push_back( make_shared<sphere>( vec3(0, -1002, -5), 1000, make_shared<metal>(vec3(0.9, 0.9, 0.9), 0.9)) );
    return make_shared<hitable_list>(h_list);
}


vector<shared_ptr<hitable>> balls1() {
    vector<shared_ptr<hitable>> list;
    list.clear();
    list.push_back( make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))) );
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_double();
            vec3 center(a+0.9*random_double(),0.2,b+0.9*random_double());
            if ((center-vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list.push_back( make_shared<sphere>(
                        center, 0.2,
                        make_shared<lambertian>(vec3(random_double()*random_double(),
                                            random_double()*random_double(),
                                            random_double()*random_double()))
                    ));
                }
                else if (choose_mat < 0.95) { // metal
                    list.push_back( make_shared<sphere>(
                        center, 0.2,
                        make_shared<metal>(vec3(0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
                                       0.5*(1 + random_double())),
                                  0)
                    ));
                }
                else {  // glass
                    list.push_back( make_shared<sphere>(center, 0.2, make_shared<glass>(vec3(1.4, 1.45, 1.5), 0)));
                }
            }
        }
    }

    list.push_back( make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<glass>(vec3(1.4, 1.45, 1.5), 0)));
    list.push_back( make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
    list.push_back( make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));
    //list[i++] = make_shared sphere(vec3(-10013, 0, 0), 10000, make_shared metal(vec3(0.9, 0.9, 0.9), 0.0));
    //return make_shared<hitable_list>(list);
    return list;
}

/*
TODO: debug scene
shared_ptr<hitable> testmycube() {
    vector<shared_ptr<hitable>> h_list;
    h_list.clear();
    h_list.push_back( make_shared<sphere>(vec3(4, 1, 2), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));
    h_list.push_back( make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));
    h_list.push_back( make_shared<cube>(
        vec3(-2,0,0), vec3(2,0,0), vec3(0,4,0), vec3(0,0,4),
        make_shared<glass>(vec3(1.40, 1.45, 1.5), 0.0)
    ));
    //h_list.push_back( make_shared<sphere>(vec3(-2.1, 0.1, -0.1), 0.1, make_shared<source>(vec3(0.8, 0.0, 0.2))));
    //h_list.push_back( make_shared<sphere>(vec3( 0.1, 0.1, -0.1), 0.1, make_shared<source>(vec3(0.8, 0.0, 0.2))));
    //h_list.push_back( make_shared<sphere>(vec3(-2.1, 0.1,  4.1), 0.1, make_shared<source>(vec3(0.8, 0.0, 0.2))));
    //h_list.push_back( make_shared<sphere>(vec3( 0.1, 0.1,  4.1), 0.1, make_shared<source>(vec3(0.8, 0.0, 0.2))));
    //h_list.push_back( make_shared<sphere>(vec3(-2.1, 4.0, -0.1), 0.1, make_shared<source>(vec3(0.8, 0.0, 0.2))));
    //h_list.push_back( make_shared<sphere>(vec3( 0.1, 4.0, -0.1), 0.1, make_shared<source>(vec3(0.8, 0.0, 0.2))));
    //h_list.push_back( make_shared<sphere>(vec3(-2.1, 4.0,  4.1), 0.1, make_shared<source>(vec3(0.8, 0.0, 0.2))));
    //h_list.push_back( make_shared<sphere>(vec3(0.1, 4.0,  4.1), 0.1, make_shared<source>(vec3(0.8, 0.0, 0.2))));
    h_list.push_back( make_shared<sphere>(vec3(4, 2.0, -1.5), 1.8, make_shared<lambertian>(vec3(0.7, 0.9, 0.8))));
    h_list.push_back( make_shared<sphere>(vec3(4, 2.0, 5), 2.0, make_shared<lambertian>(vec3(0, 0.2, 0.8))));
    
    return make_shared<hitable_list>(h_list);
}
*/

/*
hitable* test_intersect() {
    hitable** scene = make_shared hitable*[2];
    hitable** balls = make_shared hitable*[2];
    balls[0] = make_shared sphere(vec3(1, 1.5, 0), 1.5, make_shared lambertian(vec3(0.8, 0.6,0.3)));
    balls[1] = make_shared sphere(vec3(-1,1.5, 0), 2.5, make_shared metal(vec3(0.3, 0.5, 0.7), 0.4));
    scene[0] = make_shared sphere(vec3(0,-1000,0), 1000, make_shared lambertian(vec3(0.5, 0.5, 0.5)));
    scene[1] = make_shared intersect(balls, 2);
    return make_shared hitable_list(scene, 2);
}

*/


shared_ptr<hitable> test_monte_carlo() {
    vector<shared_ptr<hitable>> list;
    list.push_back( make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(vec3(0.7, 0.7, 0.7))));
    int i = 1;
    int source_num = 0; //testing
    for (int a = -2; a <= 2; a++) {
        for (int b = -2; b <= 2; b++) {
            float choose_mat = random_double();
            vec3 center(3*a+2.7*random_double(),0.5,3*b+2.7*random_double());
            if ((center-vec3(4,0.5,0)).length() > 0.9) {
                if (choose_mat < 0.4) {  // diffuse
                    list.push_back( make_shared<sphere>(
                        center, 0.5,
                        make_shared<lambertian>(vec3(random_double()*random_double(),
                                            random_double()*random_double(),
                                            random_double()*random_double()))
                    ));
                }
                else if (choose_mat < 0.50) { // metal
                    list.push_back( make_shared<sphere>(
                        center, 0.5,
                        make_shared<metal>(vec3(0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
                                       0.5*(1 + random_double())),
                                  0)
                    ));
                }
                else if (choose_mat < 0.60) {  // glass
                    list.push_back( make_shared<sphere>(center, 0.5, make_shared<glass>(vec3(1.4, 1.45, 1.5), 0)));
                }
                else {  // source
                    list.push_back( make_shared<sphere>(
                        center, 0.5, 
                        make_shared<source>(vec3(0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
                                       0.5*(1 + random_double()))
                                )
                    ));
                    source_num ++;
                }
            }
        }
    }
    list.push_back( make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
    list.push_back( make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<source>(vec3(0.7, 0.9, 0.5))));
    source_num++;
    //list[i++] = make_shared sphere(vec3(10010, 0, 0), 10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    //list[i++] = make_shared sphere(vec3(-10010, 0, 0),10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    //list[i++] = make_shared sphere(vec3(0, 10010, 0), 10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    //list[i++] = make_shared sphere(vec3(0, 0, 10010), 10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    //list[i++] = make_shared sphere(vec3(0, 0, -10010),10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    list.push_back(make_shared<plane>(vec3(10, 0, 0), vec3(-1, 0, 0), make_shared<lambertian>(vec3(0.7, 0.7, 0.7))));
    list.push_back(make_shared<plane>(vec3(-10, 0, 0),vec3( 1, 0, 0), make_shared<lambertian>(vec3(0.7, 0.7, 0.7))));
    list.push_back(make_shared<plane>(vec3(0, 10, 0), vec3(0, -1, 0), make_shared<lambertian>(vec3(0.7, 0.7, 0.7))));
    list.push_back(make_shared<plane>(vec3(0, 0, 10), vec3(0, 0, -1), make_shared<lambertian>(vec3(0.7, 0.7, 0.7))));
    list.push_back(make_shared<plane>(vec3(0, 0, -10),vec3(0, 0,  1), make_shared<lambertian>(vec3(0.7, 0.7, 0.7))));
    cout<<source_num<<" source(balls) in this scene in all"<<endl;
    return make_shared<hitable_list>(list);
}

/*

hitable* test_triangle() {
    int n = 35;
    hitable **list = make_shared hitable*[n+1];
    list[0] =  make_shared sphere(vec3(0,-1000,0), 1000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    int i = 1;
    list[i++] = make_shared sphere(vec3(-4, 1, -8), 1.0, make_shared lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = make_shared sphere(vec3(4, 1, -8), 1.0, make_shared source(vec3(0.7, 0.9, 0.5)));
    list[i++] = make_shared plane(vec3(10, 0, 0), vec3(-1, 0, 0), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared plane(vec3(-10, 0, 0),vec3( 1, 0, 0), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared plane(vec3(0, 10, 0), vec3(0, -1, 0), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared plane(vec3(0, 0, 10), vec3(0, 0, -1), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared plane(vec3(0, 0, -10),vec3(0, 0,  1), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(vec3(1, 1, 2), vec3(3, 1, 2), vec3(4, 3, 1), make_shared lambertian(vec3(0.8, 0.3, 0.1), vec3(0.1, 0.3, 0.8)));
    list[i++] = make_shared triangle(vec3(-3, 0, 0), vec3(-4, 2, 1), vec3(-6, 0, 0), make_shared lambertian(vec3(0.9, 0.2, 0.1), vec3(0.1, 0.2, 0.9)));
    list[i++] = make_shared triangle(vec3(1, 3.7, -1), vec3(0.2, 5.9, -1.9), vec3(-1.5, 2.8, -1.1), make_shared source(vec3(0.9, 0.5, 0.3), vec3(0.2, 0.3, 0.9)));
    return make_shared hitable_list(list, i);
}

hitable* test_triangle2() {
    int n = 50;
    hitable **list = make_shared hitable*[n+1];
    list[0] =  make_shared sphere(vec3(0,-1000,0), 1000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    int i = 1;
    int source_num = 0; //testing
    for (int a = -2; a <= 2; a++) {
        for (int b = -2; b <= 2; b++) {
            float choose_mat = random_double();
            vec3 center(3*a+2.7*random_double(),0.5,3*b+2.7*random_double());
            if ((center-vec3(4,0.5,0)).length() > 0.9) {
                if (choose_mat < 0.4) {  // diffuse
                    list[i++] = make_shared sphere(
                        center, 0.5,
                        make_shared lambertian(vec3(random_double()*random_double(),
                                            random_double()*random_double(),
                                            random_double()*random_double()))
                    );
                }
                else if (choose_mat < 0.50) { // metal
                    list[i++] = make_shared sphere(
                        center, 0.5,
                        make_shared metal(vec3(0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
                                       0.5*(1 + random_double())),
                                  0)
                    );
                }
                else if (choose_mat < 0.60) {  // glass
                    list[i++] = make_shared sphere(center, 0.5, make_shared glass(vec3(1.4, 1.45, 1.5), 0));
                }
                else {  // source
                    list[i++] = make_shared sphere(
                        center, 0.5, 
                        make_shared source(vec3(0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
                                       0.5*(1 + random_double()))
                                )
                    );
                    source_num ++;
                }
            }
        }
    }
    list[i++] = make_shared sphere(vec3(-4, 1, 0), 1.0, make_shared lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = make_shared sphere(vec3(4, 1, 0), 1.0, make_shared source(vec3(0.7, 0.9, 0.5)));

    list[i++] = make_shared plane(vec3(10, 0, 0), vec3(-1, 0, 0), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared plane(vec3(-10, 0, 0),vec3( 1, 0, 0), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared plane(vec3(0, 10, 0), vec3(0, -1, 0), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared plane(vec3(0, 0, 10), vec3(0, 0, -1), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared plane(vec3(0, 0, -10),vec3(0, 0,  1), make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    vec3 V1(1-2, 0, 2+4);
    vec3 V2(2-2, 0, 2+4);
    vec3 V3(1-2, 0, 1+4);
    vec3 V4(2-2, 0, 1+4);
    vec3 V5(1-2, 1, 2+4);
    vec3 V6(2-2, 1, 2+4);
    vec3 V7(1-2, 1, 1+4);
    vec3 V8(2-2, 1, 1+4);
    list[i++] = make_shared triangle(V1, V2, V6, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V1, V6, V5, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V2, V4, V8, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V2, V8, V6, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V5, V6, V8, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V5, V8, V7, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V7, V8, V4, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V7, V4, V3, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V1, V3, V4, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V1, V4, V2, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V5, V7, V3, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    list[i++] = make_shared triangle(V5, V3, V1, make_shared lambertian(vec3(random_double(), random_double(), random_double())));
    return make_shared hitable_list(list, i);
}

hitable* test_green() {
    int n = 35;
    hitable **list = make_shared hitable*[n+1];
    list[0] =  make_shared sphere(vec3(0,-1000,0), 1000, make_shared lambertian(vec3(0.0, 0.0, 0.0)));
    int i = 1;

    //list[i++] = make_shared sphere(vec3(-4, 1, 0), 1.0, make_shared lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = make_shared sphere(vec3(4, 1, -5), 1.0, make_shared source(vec3(0.7, 0.9, 0.5)));
    //list[i++] = make_shared sphere(vec3(10010, 0, 0), 10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    //list[i++] = make_shared sphere(vec3(-10010, 0, 0),10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    //list[i++] = make_shared sphere(vec3(0, 10010, 0), 10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    //list[i++] = make_shared sphere(vec3(0, 0, 10010), 10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    //list[i++] = make_shared sphere(vec3(0, 0, -10010),10000, make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    list[i++] = make_shared plane(vec3(10, 0, 0), vec3(-1, 0, 0), make_shared lambertian(vec3(0.9, 0.4, 0.3)));
    list[i++] = make_shared plane(vec3(-10, 0, 0),vec3( 1, 0, 0), make_shared lambertian(vec3(0.3, 0.4, 0.9)));
    list[i++] = make_shared plane(vec3(0, 10, 0), vec3(0, -1, 0), make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    list[i++] = make_shared plane(vec3(0, 0, 10), vec3(0, 0, -1), make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    list[i++] = make_shared plane(vec3(0, 0, -10),vec3(0, 0,  1), make_shared lambertian(vec3(0.7, 0.7, 0.7)));
    return make_shared hitable_list(list, i);
}
*/

vector<shared_ptr<hitable>> scene02_1() {
    vector<shared_ptr<hitable>> list;
    list.clear();
    list.push_back( make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))) );
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            if (a == 4 && b == 4) {
                myobjloader("02-3", vec3(4.5, 0, 4.5), 2, list);
                continue;
            }
            float choose_mat = random_double();
            vec3 center(a+0.9*random_double(),0.2,b+0.9*random_double());
            if ((center-vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list.push_back( make_shared<sphere>(
                        center, 0.2,
                        make_shared<lambertian>(vec3(random_double()*random_double(),
                                            random_double()*random_double(),
                                            random_double()*random_double()))
                    ));
                }
                else if (choose_mat < 0.95) { // metal
                    list.push_back( make_shared<sphere>(
                        center, 0.2,
                        make_shared<metal>(vec3(0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
                                       0.5*(1 + random_double())),
                                  0)
                    ));
                }
                else {  // glass
                    list.push_back( make_shared<sphere>(center, 0.2, make_shared<glass>(vec3(1.4, 1.45, 1.5), 0)));
                }
            }
        }
    }

    list.push_back( make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<glass>(vec3(1.4, 1.45, 1.5), 0)));
    list.push_back( make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
    list.push_back( make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));
    return list;
}

vector<shared_ptr<hitable>> scene02indoor_1() {
    vector<shared_ptr<hitable>> list;
    for (int a = -4; a <= 4; a++) {
        for (int b = -4; b <= 4; b++) {
            float choose_mat = random_double();
            float radius = 0.25*(0.5*3 + 0.4*random_double());
            vec3 center(2*a+1.6*random_double(),radius,2*b+1.6*random_double());
            if ((center-vec3(4,radius,0)).length() > 0.9) {
                if (a == 2 && b == 2) { // 02
                    myobjloader("02-3", vec3(4, 0, 5), 2, list);
                }
                else if (choose_mat < 0.55) {  // diffuse
                    list.push_back( make_shared<sphere>(
                        center, radius,
                        make_shared<lambertian>(vec3(0.8*random_double(),
                                            0.6*random_double(),
                                            0.7*random_double()))
                    ));
                }
                else if (choose_mat < 0.70) { // metal
                    list.push_back( make_shared<sphere>(
                        center, radius,
                        make_shared<metal>(vec3(0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
                                       0.5*(1 + random_double())),
                                  0)
                    ));
                }
                else if (choose_mat < 0.85) {  // glass
                    list.push_back( make_shared<sphere>(center, radius, make_shared<glass>(vec3(1.4, 1.45, 1.5), 0)));
                }
                else {  // source
                    list.push_back( make_shared<sphere>(
                        center, radius, 
                        make_shared<source>(vec3(0.25*(3*0.9 + random_double()),
                                       0.25*(3*0.5 + random_double()),
                                       0.25*(3*0.8 + random_double()))
                                )
                    ));
                }
            }
        }
    }
    list.push_back( make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
    list.push_back( make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<source>(vec3(0.7, 0.9, 0.5))));
    // walls
    int minx = -10, maxx = 10, miny = 0, maxy = 10, minz = -10, maxz = 10;
    vec3 v1(maxx, miny, maxz), v2(maxx, miny, minz), v3(maxx, maxy, maxz), v4(maxx, maxy, minz),
         v5(minx, miny, maxz), v6(minx, miny, minz), v7(minx, maxy, maxz), v8(minx, maxy, minz);
    auto mat1 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat2 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat3 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat4 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat5 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat6 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    list.push_back(make_shared<triangle>(v1, v3, v4, mat1));
    list.push_back(make_shared<triangle>(v1, v4, v2, mat1));
    list.push_back(make_shared<triangle>(v6, v8, v7, mat2));
    list.push_back(make_shared<triangle>(v6, v7, v5, mat2));
    list.push_back(make_shared<triangle>(v1, v5, v7, mat3));
    list.push_back(make_shared<triangle>(v1, v7, v3, mat3));
    list.push_back(make_shared<triangle>(v2, v4, v8, mat4));
    list.push_back(make_shared<triangle>(v2, v8, v6, mat4));
    list.push_back(make_shared<triangle>(v3, v7, v8, mat5));
    list.push_back(make_shared<triangle>(v3, v8, v4, mat5));
    list.push_back(make_shared<triangle>(v2, v6, v5, mat6));
    list.push_back(make_shared<triangle>(v2, v5, v1, mat6));
    // upperlight
    list.push_back(make_shared<sphere>(vec3(-4, 5, 6), 1.5, make_shared<source>(vec3(0.9, 0.3, 0.75))));
    list.push_back(make_shared<sphere>(vec3(-7, 5, -3), 1, make_shared<source>(vec3(0.74, 0.4, 0.9))));
    return list;
}

vector<shared_ptr<hitable>> scene02indoor_2() {
    vector<shared_ptr<hitable>> list;
    for (int a = -2; a <= 2; a++) {
        for (int b = -2; b <= 2; b++) {
            float choose_mat = random_double();
            float radius = 0.1*(0.7*9 + 0.6*random_double());
            vec3 center(4*a+1.6*random_double(),radius,4*b+1.6*random_double());
            if ((center-vec3(4,radius,0)).length() > 0.9) {
                if (a == 1 && b == 1) { // 02
                    myobjloader("02-3", vec3(4, 0, 5), 2, list);
                }
                else if (choose_mat < 0.50) {  // diffuse
                    list.push_back( make_shared<sphere>(
                        center, radius,
                        make_shared<lambertian>(vec3(0.8*random_double(),
                                            0.6*random_double(),
                                            0.7*random_double()))
                    ));
                }
                else if (choose_mat < 0.60) { // metal
                    list.push_back( make_shared<sphere>(
                        center, radius,
                        make_shared<metal>(vec3(0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
                                       0.5*(1 + random_double())),
                                  0)
                    ));
                }
                else if (choose_mat < 0.70) {  // glass
                    list.push_back( make_shared<sphere>(center, radius, make_shared<glass>(vec3(1.4, 1.45, 1.5), 0)));
                }
                else {  // source
                    list.push_back( make_shared<sphere>(
                        center, radius, 
                        make_shared<source>(vec3(0.25*(3*0.9 + random_double()),
                                       0.25*(3*0.5 + random_double()),
                                       0.25*(3*0.8 + random_double()))
                                )
                    ));
                }
            }
        }
    }
    list.push_back( make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
    list.push_back( make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<source>(vec3(0.7, 0.9, 0.5))));
    // walls
    int minx = -10, maxx = 10, miny = 0, maxy = 10, minz = -10, maxz = 10;
    vec3 v1(maxx, miny, maxz), v2(maxx, miny, minz), v3(maxx, maxy, maxz), v4(maxx, maxy, minz),
         v5(minx, miny, maxz), v6(minx, miny, minz), v7(minx, maxy, maxz), v8(minx, maxy, minz);
    auto mat1 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat2 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat3 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat4 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat5 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat6 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    list.push_back(make_shared<triangle>(v1, v3, v4, mat1));
    list.push_back(make_shared<triangle>(v1, v4, v2, mat1));
    list.push_back(make_shared<triangle>(v6, v8, v7, mat2));
    list.push_back(make_shared<triangle>(v6, v7, v5, mat2));
    list.push_back(make_shared<triangle>(v1, v5, v7, mat3));
    list.push_back(make_shared<triangle>(v1, v7, v3, mat3));
    list.push_back(make_shared<triangle>(v2, v4, v8, mat4));
    list.push_back(make_shared<triangle>(v2, v8, v6, mat4));
    list.push_back(make_shared<triangle>(v3, v7, v8, mat5));
    list.push_back(make_shared<triangle>(v3, v8, v4, mat5));
    list.push_back(make_shared<triangle>(v2, v6, v5, mat6));
    list.push_back(make_shared<triangle>(v2, v5, v1, mat6));
    // upperlight
    list.push_back(make_shared<sphere>(vec3(-4, 5, 6), 1.5, make_shared<source>(vec3(0.9, 0.3, 0.75))));
    list.push_back(make_shared<sphere>(vec3(-7, 5, -3), 1, make_shared<source>(vec3(0.74, 0.4, 0.9))));
    return list;
}

vector<shared_ptr<hitable>> test_cylinder() {
    vector<shared_ptr<hitable>> list;
    list.clear();
    list.push_back( make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))) );
    list.push_back( make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<glass>(vec3(1.4, 1.45, 1.5), 0)));
    list.push_back( make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
    list.push_back( make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));
    list.push_back( make_shared<cylinder>(vec3(-2, 2, -5), vec3(2, 3, 5), 1.5, make_shared<lambertian>(vec3(0.3, 0.5, 0.9))) );
    return list;
}

vector<shared_ptr<hitable>> homework1() {
    vector<shared_ptr<hitable>> list;
    for (int a = -2; a <= 2; a++) {
        for (int b = -2; b <= 2; b++) {
            float choose_mat = random_double();
            float radius = 0.1*(0.7*9 + 0.6*random_double());
            vec3 center(4*a+1.6*random_double(),radius,4*b+1.6*random_double());
            if ((center-vec3(4,radius,0)).length() > 0.9) {
                if (a == 1 && b == 1) { // 02
                    myobjloader("02-3", vec3(4, 0, 5), 2, list);
                }
                else if (choose_mat < 0.50) {  // diffuse
                    list.push_back( make_shared<sphere>(
                        center, radius,
                        make_shared<lambertian>(vec3(0.8*random_double(),
                                            0.6*random_double(),
                                            0.7*random_double()))
                    ));
                }
                else if (choose_mat < 0.60) { // metal
                    list.push_back( make_shared<sphere>(
                        center, radius,
                        make_shared<metal>(vec3(0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
                                       0.5*(1 + random_double())),
                                  0)
                    ));
                }
                else if (choose_mat < 0.70) {  // glass
                    list.push_back( make_shared<sphere>(center, radius, make_shared<glass>(vec3(1.4, 1.45, 1.5), 0)));
                }
                else {  // source
                    list.push_back( make_shared<sphere>(
                        center, radius, 
                        make_shared<source>(vec3(0.25*(3*0.9 + random_double()),
                                       0.25*(3*0.5 + random_double()),
                                       0.25*(3*0.8 + random_double()))
                                )
                    ));
                }
            }
        }
    }
    list.push_back( make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
    list.push_back( make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<source>(vec3(0.7, 0.9, 0.5))));
    // walls
    int minx = -10, maxx = 10, miny = 0, maxy = 10, minz = -10, maxz = 10;
    vec3 v1(maxx, miny, maxz), v2(maxx, miny, minz), v3(maxx, maxy, maxz), v4(maxx, maxy, minz),
         v5(minx, miny, maxz), v6(minx, miny, minz), v7(minx, maxy, maxz), v8(minx, maxy, minz);
    auto mat1 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat2 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat3 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat4 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat5 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    auto mat6 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    list.push_back(make_shared<triangle>(v1, v3, v4, mat1));
    list.push_back(make_shared<triangle>(v1, v4, v2, mat1));
    list.push_back(make_shared<triangle>(v6, v8, v7, mat2));
    list.push_back(make_shared<triangle>(v6, v7, v5, mat2));
    list.push_back(make_shared<triangle>(v1, v5, v7, mat3));
    list.push_back(make_shared<triangle>(v1, v7, v3, mat3));
    list.push_back(make_shared<triangle>(v2, v4, v8, mat4));
    list.push_back(make_shared<triangle>(v2, v8, v6, mat4));
    list.push_back(make_shared<triangle>(v3, v7, v8, mat5));
    list.push_back(make_shared<triangle>(v3, v8, v4, mat5));
    list.push_back(make_shared<triangle>(v2, v6, v5, mat6));
    list.push_back(make_shared<triangle>(v2, v5, v1, mat6));
    // upperlight
    list.push_back(make_shared<sphere>(vec3(-4, 5, 6), 1.5, make_shared<source>(vec3(0.9, 0.3, 0.75))));
    list.push_back(make_shared<cylinder>(vec3(-7, 5, -3), vec3(1.5, 1, 2), 0.6, make_shared<lambertian>(vec3(0.35, 0.6, 1.0))));
    return list;
}

vector<shared_ptr<hitable>> testSurfaceSource() {
    vector<shared_ptr<hitable>> list;
    // scene
    // # load 02
    myobjloader("02-3", vec3(4, 0, 5), 4.5, list);
    // # use sphere
    // list.push_back(make_shared<sphere>(vec3(4, 1, 5), 1, make_shared<lambertian>(vec3(0.7, 0.4, 0.5))));
    // add connell box
    int minx = -10, maxx = 10, miny = 0, maxy = 10, minz = -10, maxz = 10;
    vec3 v1(maxx, miny, maxz), v2(maxx, miny, minz), v3(maxx, maxy, maxz), v4(maxx, maxy, minz),
         v5(minx, miny, maxz), v6(minx, miny, minz), v7(minx, maxy, maxz), v8(minx, maxy, minz);
    // ## pink style
    // auto mat1 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    // auto mat2 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    // auto mat3 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    // auto mat4 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    // auto mat5 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    // auto mat6 = make_shared<lambertian>(vec3(0.25*(3*0.9 + random_double()),0.25*(3*0.5 + random_double()),0.25*(3*0.8 + random_double())));
    // ## conell style
    auto mat1 = make_shared<lambertian>(vec3(0.9,   0,   0));
    auto mat2 = make_shared<lambertian>(vec3(  0, 0.9,   0));
    auto mat3 = make_shared<lambertian>(vec3(0.9, 0.9, 0.9));
    auto mat4 = make_shared<lambertian>(vec3(0.9, 0.9, 0.9));
    auto mat5 = make_shared<lambertian>(vec3(0.9, 0.9, 0.9));
    auto mat6 = make_shared<lambertian>(vec3(0.9, 0.9, 0.9));
    list.push_back(make_shared<triangle>(v1, v3, v4, mat1));
    list.push_back(make_shared<triangle>(v1, v4, v2, mat1));
    list.push_back(make_shared<triangle>(v6, v8, v7, mat2));
    list.push_back(make_shared<triangle>(v6, v7, v5, mat2));
    list.push_back(make_shared<triangle>(v1, v5, v7, mat3));
    list.push_back(make_shared<triangle>(v1, v7, v3, mat3));
    list.push_back(make_shared<triangle>(v2, v4, v8, mat4));
    list.push_back(make_shared<triangle>(v2, v8, v6, mat4));
    list.push_back(make_shared<triangle>(v3, v7, v8, mat5));
    list.push_back(make_shared<triangle>(v3, v8, v4, mat5));
    list.push_back(make_shared<triangle>(v2, v6, v5, mat6));
    list.push_back(make_shared<triangle>(v2, v5, v1, mat6));
    // add upperlight
    double delta = 0.01;
    auto sourceMat = make_shared<source>(vec3(0.95, 0.95, 0.95));
    // # triangle
    // vec3 vA(-3, 10-delta, -3), vB(-3, 10-delta, 3), vC(3, 10-delta, 0);
    // list.push_back(make_shared<triangle>(vC, vB, vA, sourceMat));
    // # sphere
    // list.push_back(make_shared<sphere>(vec3(0, 7, 0), 1, sourceMat));
    // # polygon
    // int n = 6;
    // vector<vec3> polyVerticeList;
    // double theta0 = 2*M_PI/n;
    // double r0 = 2;
    // polyVerticeList.clear();
    // for (int i = 0; i < n; i++) {
    //     polyVerticeList.push_back(vec3(r0*cos(i*theta0), 10-delta, r0*sin(i*theta0)));
    // }
    // list.push_back(make_shared<polygon>(polyVerticeList, sourceMat));
    // # disc
    list.push_back(make_shared<disc>(vec3(0, 10-delta, 0), vec3(0, -1, 0), 2, sourceMat));
    return list;
}

#endif