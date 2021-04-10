/* #ifndef MYRANDH
#define MYRANDH

#include <cstdlib>
#include "../vec3.h"

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
    vec3 rand_unit_vec;
    do {
        rand_unit_vec = vec3( random_double(), random_double(), random_double() );
    } while (rand_unit_vec.squared_length()>=1.0);
    return rand_unit_vec;
}

