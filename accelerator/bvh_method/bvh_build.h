#ifndef BVHBUILDH
#define BVHBUILDH

#include "../bvh.h"

__host__ int BVHsahtraverse_split(vector<precomputeINFO>& infos, 
    int start, int end, int dim
) { // return splidID: start -> make leaf
    int num = end - start;
    float bestcost = num;
    int bestsplit = 0;
    // sort in dim
    sort(&infos[start], &infos[end], 
        [dim](const precomputeINFO& info1, const precomputeINFO& info2) {
            return info1.centroid.e[dim] < info2.centroid.e[dim];
        });
    // from left to right
    bound leftbound;
    vector<double> leftarea(num);
    for (int i = 0; i < num; i++) {
        leftbound = Union(leftbound, infos[i+start].b);
        leftarea[i] = leftbound.half_area();
    }
    double totalarea = leftarea[num-1];
    // from right to left and cal best case (n-1, 1) -> (1, n-1)
    bound rightbound;
    for (int i = num-1; i > 0; i--) {
        rightbound = Union(rightbound, infos[i+start].b);
        double temp_rightarea = rightbound.half_area();
        double cur_cost = ( i*leftarea[i-1] + (num-i)*temp_rightarea ) / totalarea;
        if ( cur_cost < bestcost ) {
            bestcost = cur_cost;
            bestsplit = i;
        }
    }
    return start + bestsplit;
}

__host__ void BVHAccel::precompute(vector<precomputeINFO>& infos) {
    for(int i = 0; i < h_list.size(); i++) {
        // calculate infos[i]
        infos[i].b = h_list[i]->bbox();
        infos[i].hitableID = i;
        infos[i].centroid = infos[i].b.centroid();
    }
}

__host__ shared_ptr<BVHBuildNode> BVHAccel::recurbuild(
    int start, int end, vector<precomputeINFO> infos,
    int& nodenum, vector<shared_ptr<hitable>>& orderedobj
) const {
    int objnum = end - start;
    assert(objnum>0 && "invalid split method(with 0 or less hitable child)");
    int cur_nodenum = nodenum;
    nodenum ++;
    shared_ptr<BVHBuildNode> newbuildnode = make_shared<BVHBuildNode>();
    int startid = orderedobj.size();
    // TODO: commend this
    // assert(startid == start && "wrong orderedobj");
    if ( objnum == 1 ) {    // build leaf
        int originid = infos[start].hitableID;
        orderedobj.push_back(h_list[originid]);
        newbuildnode->initleaf(startid, 1, infos[start].b, cur_nodenum);
        return newbuildnode;
    }
    else {
        // calulate maxextdim(centroid)
        bound centroidbound;
        for (int i = start; i < end; i++) {
            centroidbound = Union(centroidbound, infos[i].centroid);
        }
        int maxextdim = centroidbound.maxextend();

        if (centroidbound.rmax[maxextdim] == centroidbound.rmin[maxextdim]) {
            // build (possibly multi obj) leaf
            bound leafbound;
            int originid;
            for (int i = start; i < end; i++) {
                originid = infos[i].hitableID;
                orderedobj.push_back(h_list[originid]);
                leafbound = Union(leafbound, infos[i].b);
            }
            newbuildnode->initleaf(startid, objnum, leafbound, cur_nodenum);
            return newbuildnode;
        }
        else {  // split and build interior
            int splitID;
            if ( objnum <= BVH_direct_split_num ) {
                // TODO: directly split -> test special code
                nth_element(&infos[start], &infos[start] + objnum / 2, &infos[end-1]+1,
                    [maxextdim](const precomputeINFO& info1, const precomputeINFO& info2) {
                        return info1.centroid.e[maxextdim] < info2.centroid.e[maxextdim];
                    });
                splitID = start + objnum / 2;
            }
            else {  // TODO: test which method is better
                // CASE: SAH split using traverse
                splitID = BVHsahtraverse_split(infos, start, end, maxextdim);
                if (splitID == start) { 
                    // no split is the best case: build leaf with multi hitables
                    bound leafbound;
                    int originid;
                    for (int i = start; i < end; i++) {
                        originid = infos[i].hitableID;
                        orderedobj.push_back(h_list[originid]);
                        leafbound = Union(leafbound, infos[i].b);
                    }
                    newbuildnode->initleaf(startid, objnum, leafbound, cur_nodenum);
                    return newbuildnode;
                }
                // TODO: SAH split using bucket
                // splitID = BVHsahbucket_split()
                // TODO: partition according to splitID
            }
            // build interior
            // TODO: should be optimized
            bound bound1, bound2;
            for (int i = start; i < end; i++) {
                if ( i < splitID ) bound1 = Union(bound1, infos[i].b);
                else bound2 = Union(bound2, infos[i].b);
            }
            int startL, endL, startR, endR;
            // swap if not ordered
            if (bound1.centroid()[maxextdim] < bound2.centroid()[maxextdim])
                startL = start, endL = splitID, startR = splitID, endR = end;
            else
                startL = splitID, endL = end, startR = start, endR = splitID;
            auto nodeL = recurbuild(startL, endL, infos, nodenum, orderedobj);
            auto nodeR = recurbuild(startR, endR, infos, nodenum, orderedobj);
            newbuildnode->initinterior(nodeL, nodeR,
                maxextdim, startid, objnum, cur_nodenum);
        }
    }

    return newbuildnode;
}

__host__ int BVHAccel::realign(
    shared_ptr<BVHBuildNode> root
) {//  realign nodes
    // init a stack
    auto BuildNodeStack = stack<shared_ptr<BVHBuildNode>>();
    // TODO: commend this
    assert( BuildNodeStack.empty() && "stack not initialized properly" );
    BuildNodeStack.push(root);

    // dfs build nodes
    int index = 0;
    while( !BuildNodeStack.empty() ) {
        auto iptr = BuildNodeStack.top();
        BuildNodeStack.pop();
        // TODO: commend this
        assert( index == iptr->nodeid && "wrong nodeid");
        // init index_th node
        nodes[index].b = iptr->b;
        if ( iptr->isleaf() ) { nodes[index].hitableID = iptr->startid; }
        else { 
            nodes[index].SecChildID = iptr->child[1]->nodeid;
            BuildNodeStack.push(iptr->child[1]);
            BuildNodeStack.push(iptr->child[0]);
        }
        nodes[index].hitablenum = iptr->objnum;
        nodes[index].axis = iptr->axis;
        // first traverse?
        index++;
    }
    return index;
}

__device__ bool BVHAccel::hit(
    const ray& r, float t_min, float t_max, hit_rec& rec
) const {//  dfs nodes
    // init a stack
    auto dfsstack = stack<int>();
    dfsstack.push(0);
    float closest_so_far = t_max;
    hit_rec temp_rec;
    bool flag = false;

    // precalculation for ray
    bool (bound::*hit_ptr)(
        const ray& r, float t_min, float t_max,
        const vec3& inv_dir, const bool is_pos[3]
    ) const;
    vec3 inv_dir;
    if ( r.direction().x() && r.direction().y() && r.direction().z() ) {
        inv_dir = vec3( 1 / r.direction().x(), 1 / r.direction().y(), 1 / r.direction().z());
        hit_ptr = &bound::hit;
    } else {
        inv_dir = vec3( r.direction().x() ? 1 / r.direction().x() : 0, 
                        r.direction().y() ? 1 / r.direction().y() : 0, 
                        r.direction().z() ? 1 / r.direction().z() : 0);
        hit_ptr = &bound::fake_hit;
    }
    bool is_pos[3] = {r.direction().x() > 0, r.direction().y() > 0, r.direction().z() > 0};

    while ( !dfsstack.empty() ) {
        auto cur_index = dfsstack.top();
        dfsstack.pop(); 
        if ( (nodes[cur_index].b.*hit_ptr)(r, t_min, closest_so_far, inv_dir, is_pos) ) {
            // hit bound:
            int splitaxis = nodes[cur_index].axis;
            if ( splitaxis == 3 ) {
                // is leaf: tranverse all hitable
                int start = nodes[cur_index].hitableID;
                int end = start + nodes[cur_index].hitablenum;
                for (int i = start; i < end; i++) {
                    if (h_list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
                        rec = temp_rec;
                        flag = true;
                        closest_so_far = temp_rec.hit_t;
                    }
                }
            } else {
                // is interior: determing the order(ordered BVH)
                if (is_pos[splitaxis]) {
                    // visit first child first
                    dfsstack.push(nodes[cur_index].SecChildID);
                    dfsstack.push(cur_index + 1);
                } else {
                    // visit second child first
                    dfsstack.push(cur_index + 1);
                    dfsstack.push(nodes[cur_index].SecChildID);
                }
            }
        }
    }
    if (flag) rec = temp_rec;
    return flag;
}




#endif