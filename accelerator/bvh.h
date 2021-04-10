#ifndef BVHH
#define BVHH

#include "../hitable.h"
#include "bvh_method/bvh_node.h"
#include "../mylib/settings.h"
/*
    ref1: Pbrt, 
    ref2: Wald, I., Boulos, S., and Shirley, P. 2007. Ray tracing deformable scenes using dynamic bounding volume hierarchies. ACM Trans. Graph. 26, 1, Article 6
    (January 2007), 18 pages. DOI = 10.1145/1186644.1186650 http://doi.acm.org/ 10.1145/1186644.1186650
    written by 曾许曌秋(eliot) April 2020
*/

class BVHAccel: public hitable {
    public:
        BVHAccel (vector<shared_ptr<hitable>> hit_l);
        ~BVHAccel () {/* TODO: delete nodes or check memory*/}
        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_rec& rec
        ) const;
        virtual bound bbox() const {
            assert(0&&"BVH don't have bbox currently"); 
            return bound();
        }
        virtual bool is_inside (const vec3& point) const {
            assert(0&&"BVH don't have is_inside currently");
            return false;
        }
    private:
        void precompute(vector<precomputeINFO>& infos);
        shared_ptr<BVHBuildNode> recurbuild(
            int start, int end,
            vector<precomputeINFO> infos, int& nodenum, 
            vector<shared_ptr<hitable>>& orderedobj
        ) const ;
        int realign(
            shared_ptr<BVHBuildNode> root
        );
        vector<shared_ptr<hitable>> h_list;
        vector<BVHNode> nodes;
};

BVHAccel::BVHAccel(vector<shared_ptr<hitable>> hit_l)
    : h_list(hit_l) {
    // pre calculate bounding box & centroid
    vector<precomputeINFO> infos(h_list.size());
    precompute(infos);

    // recurancely built (with SAH)
    int nodenum = 0;
    vector<shared_ptr<hitable>> orderedobj;
    orderedobj.reserve(h_list.size());
    auto buildnodes = recurbuild(
        0, h_list.size(), infos, 
        nodenum, orderedobj
    );
    assert(h_list.size() == orderedobj.size() && "obj not ordered properly");
    h_list.swap(orderedobj);

    // alloc memory for nodes
    nodes.clear();
    nodes.reserve(nodenum);
    //  traverse to realign
    int nodeindex = realign(buildnodes);
    // TODO: commend this after testing
    assert( nodenum == nodeindex && "wrong realign func");
    // log BVH info
    clog<<"BVH successfully built with "<<nodenum
        <<" nodes";

}

#include "bvh_method/bvh_build.h"

#endif