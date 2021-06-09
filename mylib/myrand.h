/* #ifndef MYRANDH
#define MYRANDH

#include <cstdlib>
#include "../geometry/geometry.h"

// float
#ifndef MAXFLOAT
#define MAXFLOAT std::numeric_limits<float>::max()
#endif

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}



#endif */

#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "common.h"

#include <random>
static double random_double()
{
	static std::random_device seed_gen;
	static std::mt19937 engine(seed_gen());
	static std::uniform_real_distribution<> dist(0.0, 1.0);
	return dist(engine);
}

vec3 rand_in_unit_sphere() {
    vec3 rand_unit_vec3;
    do {
        rand_unit_vec3 = vec3( random_double(), random_double(), random_double() );
    } while (rand_unit_vec3.squared_length()>=1.0);
    return rand_unit_vec3;
}

vec2 rand_in_unit_circle() {
    vec2 rand_unit_vec2;
    do {
        rand_unit_vec2 = vec2( random_double(), random_double());
    } while (rand_unit_vec2.squared_length() >= 1.0);
    return rand_unit_vec2;
}

