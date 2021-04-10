#ifndef HITABLELISTH
#define HITABLELISTH

#include "../hitable.cuh"
#include "../material.h"

using namespace std;

class hitable_list: public hitable {
    public:
        ~hitable_list() {}
        hitable_list() {clear();}
        hitable_list(vector<shared_ptr<hitable>> l): list(l) {
            #if USING_MONTE_CARLO
                samp_index.clear();
                for (int i = 0; i < list.size(); i++) {
                    if ( list[i]->mat()->is_sampling() )  samp_index.push_back(i);
                }
                clog<<"sampling number: "<<samp_index.size()<<endl;
            #endif
        }
        void clear() {list.clear(); samp_index.clear();}
        void add(shared_ptr<hitable> obj) {
            list.push_back(obj);
            #if USING_MONTE_CARLO
                if ( obj->mat()->is_sampling() ) samp_index.push_back(list.size() - 1);
                clog<<"sampling number updated: "<<samp_index.size()<<endl;
            #endif
        }
        virtual bool hit(
            const ray & r, float t_min, float t_max, hit_rec & rec
        ) const;
        virtual bound bbox() const;
        virtual bool is_inside(const vec3& point) const;
        virtual ray ray_gen(const vec3& point) const;
        virtual double pdf_val(const ray& r) const;
        virtual bool has_source() const {return samp_index.size() != 0;}
    private:
        vector<shared_ptr<hitable>> list;
        vector<int> samp_index;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_rec& rec) const {
    float closest_so_far = t_max;
    hit_rec temp_rec;
    bool is_hit = false;
    for (auto obj: list) {
        if (obj->hit(r, t_min, closest_so_far, temp_rec)) {
            rec = temp_rec;
            closest_so_far = rec.hit_t;
            is_hit = true;
        }
    }
    return is_hit;
}

bound hitable_list::bbox() const {
    assert(list.size()&&"empty hitable list");
    bound box(list[0]->bbox());
    for (int i = 1; i < list.size(); i++) {
        box = Union(box, list[i]->bbox());
    }
    return box;
}

bool hitable_list::is_inside(const vec3& point) const {
    for (auto obj: list) {
        if ( obj->is_inside(point) ) return true;
    }
    return false;
}

ray hitable_list::ray_gen(const vec3& point) const {
    int ran_ind = (int)(samp_index.size()*random_double());
    assert(samp_index.size() && "no sampling object");
    return list[samp_index[ran_ind]]->ray_gen(point);
}

double hitable_list::pdf_val(const ray& r) const {
    double val = 0;
    for (auto index : samp_index) {
        val += list[index]->pdf_val(r);
    }
    assert(samp_index.size() && "no sampling object");
    return val/samp_index.size();
}

#endif