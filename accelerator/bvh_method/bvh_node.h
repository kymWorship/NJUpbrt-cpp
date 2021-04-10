#ifndef BVH_NODEH
#define BVH_NODEH

#include "../../bound.h"

struct BVHBuildNode {
    bound b;
    shared_ptr<BVHBuildNode> child[2];
    int axis;   // 0,1,2(x,y,z) 3: leaf
    int startid, objnum, nodeid;
    void initleaf(int start, int num,
        bound leafb, int nodenum
    ) {
        b = leafb;
        startid = start;
        objnum = num;
        child[0] = child[1] = nullptr;
        axis = 3;
        nodeid = nodenum;
    }
    void initinterior ( shared_ptr<BVHBuildNode> Lchild,
        shared_ptr<BVHBuildNode> Rchild, int dim,
        int start, int num, int nodenum
    ) {
        child[0] = Lchild; child[1] = Rchild;
        b = Union(Lchild->b, Rchild->b);
        axis = dim;
        startid = start;
        objnum = num;
        nodeid = nodenum;
    }
    inline bool isleaf() { return axis == 3; }
};

#pragma align(32)
struct BVHNode {
    // first child is the one with smaller centroid (split dim) 
    // which should be visit first by pos direction ray
    bound b;    // 24 bytes
    union {     // 4  bytes
        int SecChildID; // interior
        int hitableID;  // leaf
    };
    //  4 bytes
    uint16_t hitablenum;
    uint8_t axis;   // split axis 0,1,2(x,y,z) 3: leaf
    uint8_t first_trav; // first child to traverse along the axis
};

struct precomputeINFO {
    int hitableID;
    bound b;
    vec3 centroid;
};

#endif

