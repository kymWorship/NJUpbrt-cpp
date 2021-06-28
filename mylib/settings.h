#ifndef SETTINGSH
#define SETTINGSH

// environment
//#define WorkspaceDir "E:/WorkspaceZXZQ/private/NJUpbrt-cpp/"
#define WorkspaceDir "D:/NJU/cs/PhysicsBasedRaytracing/workspace/"

// pic para
#define MAX_DEP 40
#define NX 400
#define NY 400
#define NS 10000
#define HFOV 65 //104 //deg

// dispersion
#define IS_DISP false

// background
#define bg_light true
#define dark_fac 1
#define USING_ENVIRONMENT_LIGHT true

// camera
#define DepthOfField false

// sampler
#define USING_MONTE_CARLO true

// accelerator
#define BVH_direct_split_num 4
#define USING_BVH_ACCEL true

// multi threads 
#define MY_THREAD_NUM 8

// degub
#define ISDEBUGGING true


#endif