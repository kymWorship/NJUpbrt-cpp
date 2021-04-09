#ifndef MIX_SAMPLERH
#define MIX_SAMPLERH

#include "../sampler.h"

class mix_sampler: public sampler{
    public:
        ~mix_sampler() {}
        mix_sampler() {}
        mix_sampler(vector<shared_ptr<sampler>> s_l, vector<float> w):
            sampler_list(s_l), weight(w), length(s_l.size()) {
            w_sum = 0;
            for (auto wi: weight) {
                w_sum += wi;
                w_cdf.push_back ( w_sum );
            }
            assert(s_l.size() == w.size());
            // TODO: commend following test after testing
            assert(w_cdf.size() == sampler_list.size());
        }
        virtual ray ray_gen(const vec3& point) const;
        virtual double pdf_val(const ray& r) const;
    private:
        vector<shared_ptr<sampler>> sampler_list;
        vector<float> weight;
        vector<float> w_cdf;
        float w_sum;
        int length;
    
};

ray mix_sampler::ray_gen(const vec3& point) const {
    double rand_w = w_sum*random_double();
    for (int i = 0; i < length; i++) {
        if ( rand_w < w_cdf[i]) return sampler_list[i]->ray_gen(point);
    }
    return sampler_list[length-1]->ray_gen(point);
}

double mix_sampler::pdf_val(const ray& r) const {
    double pdf = 0;
    for (int i = 0; i < length; i++) {
        pdf += weight[i] * sampler_list[i]->pdf_val(r);
    }
    return pdf / w_sum;
}


#endif