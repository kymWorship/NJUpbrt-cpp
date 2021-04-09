#ifndef INTERSECTH
#define INTERSECTH

#include "../hitable.h"

class intersect:public hitable{
    public:
        intersect() {}
        intersect(vector<shared_ptr<hitable>> l)
            : list(l), list_size(l.size()) {}
        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_rec& rec
        ) const;
        virtual bound bbox() const;
        virtual bool is_inside(const vec3& point) const;
    private:
        vector<shared_ptr<hitable>> list;
        int list_size;
};

bool intersect::hit(
    const ray& r, float t_min, float t_max, hit_rec& rec
) const {
    float closest_so_far = t_max;
    hit_rec temp_rec;
    bool is_hit = false;
    for (int i = 0; i < list_size; i++) {
        if ( list[i]->hit(r, t_min, closest_so_far, temp_rec) ) {
            bool inside = true;
            for (int j = 0; j < list_size; j++) {
                if ( (j!=i) && (!list[j]->is_inside(temp_rec.hit_p)) ) inside = false;
            }
            if ( inside ) {
                rec = temp_rec;
                closest_so_far = rec.hit_t;
                is_hit = true;
            }
        }
    }
    return is_hit;
}

bound intersect::bbox() const {
    assert(list.size()&&"empty intersection");
    bound box(list[0]->bbox());
    for (int i = 1; i < list.size(); i++) {
        box = Intersection(box, list[i]->bbox());
    }
    return box;
}

bool intersect::is_inside(const vec3& point) const {
    for (auto obj: list) {
        if ( obj->is_inside(point) ) return false;
    }
    return true;
}


#endif