#include<bits/stdc++.h>
#include "geometry/geometry.h"
#include "hitable.h"
#include "material.h"
#include "camera.h"
#include "sampler.h"
#include "mylib/myrand.h"
#include "mylib/settings.h"
#include "mylib/mycode.h"
#include "mylib/common.h"
#include "mylib/mylogo.h"
#include "scene.h"
#include "background.h"
#include "accelerator/bvh.h"

#include <omp.h>



using namespace std;

inline vec3 de_nan(const vec3& c) {
    vec3 temp = c;
    if (!(temp[0] == temp[0])) temp[0] = 0;
    if (!(temp[1] == temp[1])) temp[1] = 0;
    if (!(temp[2] == temp[2])) temp[2] = 0;
    return temp;
}


vec3 color(const ray & r, shared_ptr<hitable> scene, int depth, shared_ptr<hitable> naivesource, shared_ptr<background> bg) {
    hit_rec hrec;
    // MAX DEP  ->  dark
    if ( depth >= MAX_DEP ) return vec3(0, 0, 0);
    // hit_nothing -> background
    if ( !scene->hit(r, 0.001, MAXFLOAT, hrec) ) return bg->get_color(r.direction());

    vec3 emitted = hrec.mat_ptr->emitted(r, hrec);
    sca_rec srec;
    // hit sth only emitting -> return emitted light
    if ( !hrec.mat_ptr->scatter(r, hrec, srec) ) return emitted;
    // hit sth specular (currently all except lambertian) -> no Monte-Carlo
    if ( srec.is_specular ) {
        
        // dispersion
        #if IS_DISP
        if ( srec.is_dispersing ) {
            assert(!r.is_dispersed);
            srec.rscattered.is_dispersed = true;
            srec.gscattered.is_dispersed = true;
            srec.bscattered.is_dispersed = true;
            return  emitted
                    +vec3(srec.ratio.r(), 0, 0)*color(srec.rscattered, scene, depth+1, naivesource, bg)
                    +vec3(0, srec.ratio.g(), 0)*color(srec.gscattered, scene, depth+1, naivesource, bg)
                    +vec3(0, 0, srec.ratio.b())*color(srec.bscattered, scene, depth+1, naivesource, bg);
        }
        srec.scattered.is_dispersed = r.is_dispersed;
        #endif
        // onther light
        return emitted + srec.ratio*color(srec.scattered, scene, depth+1, naivesource, bg);
    }
    // Lambertian / diffused microfact
    #if USING_MONTE_CARLO   // Monte-Carlo
        if ( naivesource->has_source() ) { 
            vector<shared_ptr<sampler>> samp_list {
                make_shared<cos_sampler>(hrec.normal),
                make_shared<hit_sampler>(naivesource)
            };
            vector<float> weight {0.5, 0.5};
            auto mixpdf = make_shared<mix_sampler>(samp_list, weight);
            ray scattered =  mixpdf->ray_gen(hrec.hit_p);
            auto scattering_pdf = hrec.mat_ptr->scattering_pdf(r, hrec, scattered);
            // invalid scatering, put forward to avoid extra cal
                if ( scattering_pdf.is_zero() ) {return emitted;}
            double pdf_val = mixpdf->pdf_val(scattered);
            #if ISDEBUGGING
            assert(pdf_val && "pdf_val = 0, may cause error");
            #endif
            return emitted + 
                srec.ratio*scattering_pdf*color(scattered, scene, depth+1, naivesource, bg)/pdf_val;
        }
    #endif
    // traditional ray tracing
    return emitted + srec.ratio*color(srec.scattered, scene, depth+1, naivesource, bg);
}



int main() {
    // using multi threads
	omp_set_num_threads(MY_THREAD_NUM);
    
    // initiate
    outputMYLOGO();
    init_log("log.txt");    // redirect clog to "filename" & log time
    // vec3 look_a(0, 0, 0);
    vec3 look_a(0, 0, -2);// 02 position
    vec3 look_f(0, 3, -6);
    camera cam(look_f, look_a, vec3(0,1,0), HFOV, float(NX)/float(NY), 0);
    // init the background
    auto environment = make_shared<background>();
    // build BVH tree

    auto scenelist = scene02_1();//testEnvironmentLight(0.3, "", "", 8);
    auto time0 = clock();
    auto scene = make_shared<BVHAccel>(scenelist);
    // naive source
    vector<shared_ptr<hitable>> sourcelist;
    for (auto hptr: scenelist) {
        if (hptr->mat()->is_sampling()) sourcelist.push_back(hptr);
    }
    #if ISDEBUGGING
    cout<<"total number of source: "<<sourcelist.size()<<endl;
    #endif
    auto naivesource = make_shared<hitable_list>(sourcelist);
    auto time1 = clock();
    cout<<"BVH Tree built in "<<(time1 - time0) / CLOCKS_PER_SEC<<"secs\n";
    clog<<"BVH Tree built in "<<(time1 - time0) / CLOCKS_PER_SEC<<"secs\n";
    // prepare the output file
    ofstream outfile;
    outfile.open("picture.ppm", ios::out | ios::trunc );
    outfile<<"P3\n"<<NX<<" "<<NY<<"\n255\n";

    
    // scan the scene
    cout<<"start renderring\n";
    int progress_step = NY / 20;
    for(int j = 0; j < NY; j++) {
        // show progress indicator
        if ( j%progress_step == 0 ) {
            cout<<"\rRenderring progress: "<<(int)j*100/NY
                <<"%[";
            for (int indicator = 0; indicator < 20; indicator ++) {
                if (indicator<(int)j*20/NY) cout<<"=";
                else cout<<' ';
            }
            cout<<']';
        }
        for (int i = 0; i < NX; i++) {
            vec3 col(0, 0, 0);
            vec3 mp_col[NS];
            
            #pragma omp parallel for
            for (int k = 0; k < NS; k++) {
                float ratioh = float(i + random_double()) / float(NX);
                float ratiov = float(j + random_double()) / float(NY);
                ray r = cam.get_ray(ratioh, ratiov);
                mp_col[k] = color(r, scene, 0, naivesource, environment);
            }
            for (int k = 0; k < NS; k++ ) {
                col += de_nan(mp_col[k]);
            }
            col /= NS;
            outfile<< int( 255*sqrt(col.r()) ) << " "
                   << int( 255*sqrt(col.g()) ) << " "
                   << int( 255*sqrt(col.b()) ) << "\n";
        }
    }
    auto time2 = clock();
    cout<<"\rRenderring progress: 100%[====================]"<<endl;
    cout<<"Rendering using "
        << (time2-time1) / CLOCKS_PER_SEC<< "secs\n";
    clog<<"Rendering using "
        << (time2-time1) / CLOCKS_PER_SEC<< "secs\n";
    // close the file
    outfile.close();
}