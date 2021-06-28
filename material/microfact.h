#ifndef MICROFACTH
#define MICROFACTH

#include "../material.h"

/*
    tunable diffuse material using microfact
    args:
        roughness(float): [0, 1] define the roughness of the material
        normal_alg(string): alg used to define the distribution of normal
            input "" to use default
            "Blinn-Phong"(default): ( (e+2)/(2pi) ) (wn dot wm)^e
            "Beckmann": (1/(pi*m^2*ndotm^4)) exp((ndotm^2-1)/(m^2*ndotm^2))
            "GGX": m^2 / ( pi*(ndotm^2 * (m^2-1) + 1)^2 )
            "GGXA": TODO:
            // TODO: Add more alg
        geometry_alg(string): alg used to simulate blocking relationship
            input "" to use default
            "Smith"(defualt): G(l, v, m) = G1(l)*G1(v)
                depend on normal alg:
                "Blinn-Phong": no closed solution, used same equation
                    as "Beckmann" as suggested
                "Beckmann": c(v) = ndotv / (m*sqrt(1-ndotv^2))
                    G(v) = (3.535c(v)+2.181c^2(v)) / (1+2.276c(v)+2.577c^2(v)) if c(v)<1.6 else 1
                "GGX": 2 ndotv / (ndotv + sqrt(m^2 + (1-m^2)*ndotv))
            "Schlick": Similar to Smith G1(v) = ndotv / (ndotv(1-k) + k)
                depend on normal alg:
                "Beckmann": k = m*sqrt(2/pi) (warning: wrong approx)
                "GGX": k = m/2
            "implicit"：ndotl * ndotv
            "Cook-Torrance": min[1, 2*ndotm*ndotv/vdotm, 2*ndotm*ndotl/vdotm]
            "Kelemen": ndotl*ndotv/ndotm^2
            "Neumann": TODO: awkward, needs to check:
                http://citeseer.ist.psu.edu/viewdoc/download;jsessionid=3D3407B845B0B7F0314B4D2694501DEE?doi=10.1.1.41.3489&rep=rep1&type=pdf
            // TODO: Add more alg
        #color(vec3/string/shared_ptr<texture>): [0, 1) rgb
        #index(float/vec3): [1, infin] vec3 for rgb channel
    refs:
        http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
    myNotes:
        BRDF = DFG/(4*ndotl*ndotv)
        Lo = integral Li*BRDF*ndotl*dOmega(l)
        BRDF(Lambertian) = 1/pi
        4: dOmega(wm) = dOmega(l)/4
        integral D*vdotm dOmega(wm) = ndotv
*/

/* #region fake hash macro */
// mask
#define MFNormalMask(alg) (alg>>4)
#define MFGeometryMask(alg) (alg&0b00001111)
// normal codes: 0 - 15
#define MFNormalBlinn_PhongCode 0 // (default)
#define MFNormalBeckmannCode 1
#define MFNormalGGXCode 2
#define MFNormalGGXACode 3
// geometry codes: 0 - 15
#define MFGeometrySmithCode 0 // (default)
#define MFGeometrySchlickCode 1
#define MFGeometryimplicitCode 2
#define MFGeometryCook_TorranceCode 3
#define MFGeometryKelemenCode 4
#define MFGeometryNeumannCode 5
/* #endregion */

class microfact;
typedef double (microfact::*normFunc)(double) const;
typedef double (microfact::*geoFunc)(double, double, double, double) const;

class microfact: public material {
    public:
        /* #region definition */
        microfact() {}
        microfact(
            float roughness, string normal_alg,
            string geometry_alg, vec3 a, float index = 1.5
        ): m(roughness), n(vec3(index, index, index)) {
            attenuation = make_shared<const_texture>(a);
            updateAlg(normal_alg, geometry_alg);
        }
        microfact(
            float roughness, string normal_alg,
            string geometry_alg, string textureFilename,
            float index = 1.5
        ): m(roughness), n(vec3(index, index, index)) {
            attenuation = make_shared<png_texture>(textureFilename);
            updateAlg(normal_alg, geometry_alg);
        }
        microfact(
            float roughness, string normal_alg,
            string geometry_alg, shared_ptr<texture> text,
            float index = 1.5
        ): m(roughness), n(vec3(index, index, index)) {
            attenuation = text;
            updateAlg(normal_alg, geometry_alg);
        }
        microfact(
            float roughness, string normal_alg,
            string geometry_alg, vec3 a, vec3 index
        ): m(roughness), n(index) {
            attenuation = make_shared<const_texture>(a);
            updateAlg(normal_alg, geometry_alg);
        }
        microfact(
            float roughness, string normal_alg,
            string geometry_alg, string textureFilename,
            vec3 index
        ): m(roughness), n(index) {
            attenuation = make_shared<png_texture>(textureFilename);
            updateAlg(normal_alg, geometry_alg);
        }
        microfact(
            float roughness, string normal_alg,
            string geometry_alg, shared_ptr<texture> text,
            vec3 index
        ): m(roughness), n(index) {
            attenuation = text;
            updateAlg(normal_alg, geometry_alg);
        }
        /* #endregion */
        virtual bool scatter(
            const ray& r, const hit_rec& h_rec, sca_rec& s_rec
        ) const;
        virtual vec3 scattering_pdf(
            const ray& r_in, const hit_rec& h_rec, const ray& r_out
        ) const;
        void updateAlg(string normalAlg, string geometryAlg);
        string get_normal_alg() const;
        string get_geometry_alg() const;
    private:
        // algorithm 0b 0000(normal_alg) 0000(geometry_alg)
        unsigned char algorithm;
        float m; // roughness
        // UE4Roughness = m^2
        shared_ptr<texture> attenuation;
        vec3 n; // index for rgb channel
        double normalCache[2];
        double geometryCache[1];
        normFunc normalFactor;
        geoFunc geometryFactor;
        double Fernel_Schlick(double cosine, double ref_idx) const;
        /*
            wn: surface normal, wm: current normal,
            wl: light direction, wv: view direction
            all should be unit length
        */
        double Norm_Blinn_Phong(double ndotm) const;
        double Norm_Beckmann(double ndotm) const;
        double Norm_GGX(double ndotm) const;
        // TODO: add alg
        double Geo_Smith_Beckmann(double ndotl, double ndotv, double mdotv, double ndotm) const;
        double Geo_Smith_GGX(double ndotl, double ndotv, double mdotv, double ndotm) const;
        double Geo_Schlick(double ndotl, double ndotv, double mdotv, double ndotm) const;
        double Geo_implicit(double ndotl, double ndotv, double mdotv, double ndotm) const;
        double Geo_CT(double ndotl, double ndotv, double mdotv, double ndotm) const;
        double Geo_Kelemen(double ndotl, double ndotv, double mdotv, double ndotm) const;
        // TODO: add alg
};

bool microfact::scatter(
    const ray& r, const hit_rec& h_rec, sca_rec& s_rec
) const {
    #if IS_DISP
    s_rec.is_dispersing = false;
    #endif
    s_rec.ratio = attenuation->get_texture(h_rec.u, h_rec.v);
    if ( m!= 0) { // diffuse: should handled by Monte Carlo
        #if !USING_MONTE_CARLO 
            assert(0 && "microfact can't be handled properly without Monte Carlo, turn on Monte Carlo in settings.h");
        #endif
        s_rec.is_specular = false;
        return true;
    }
    // specular
    s_rec.is_specular = true;
    s_rec.scattered.mod_origin(h_rec.hit_p);
    auto unit_inRay = unit_vector(r.direction());
    bool flag = reflect(unit_inRay, h_rec.normal, 0, s_rec.scattered);
    double cosine = dot(-unit_inRay, h_rec.normal);
    s_rec.ratio *= vec3(Fernel_Schlick(cosine, n.r()), Fernel_Schlick(cosine, n.g()), Fernel_Schlick(cosine, n.b()));
    return flag;
}

vec3 microfact::scattering_pdf(
    const ray& r_in, const hit_rec& h_rec, const ray& r_out
) const {
    vec3 wl = unit_vector(r_out.direction());
    double ndotl = dot(h_rec.normal, wl);
    if (ndotl <= 0) return vec3(0, 0, 0); // unvalid light
    vec3 wv = -unit_vector(r_in.direction());
    #if ISDEBUGGING
    if(dot(h_rec.normal, wv) <= 0) std::clog<<"found light insideout: "<<dot(h_rec.normal, wv)<<std::endl;
    #endif
    // DASSERT(dot(h_rec.normal, wv) > 0, "need check");
    vec3 wm = unit_vector(wl + wv);
    double ndotv = dot(h_rec.normal, wv);
    double mdotv = dot(wm, wv);
    double ndotm = dot(h_rec.normal, wm);
    // ndotl ndotv ndotm mdotv(=mdotl)
    // return D*G*F / (4*ndotl*ndotv) * ndotl (i.e.: BRDF * cosine)
    return ((this->*normalFactor)(ndotm)) * ((this->*geometryFactor)(ndotl, ndotv, mdotv, ndotm))
        * vec3(Fernel_Schlick(mdotv, n.r()), Fernel_Schlick(mdotv, n.g()), Fernel_Schlick(mdotv, n.b()))
        / (4*ndotv);
}

/* #Region assist funcs*/
void microfact::updateAlg(string normalAlg, string geometryAlg) {
    // can apply hash?
    algorithm = 0;
    assert(0<=m && m<=1 && "Roughness out of range!");
    if (normalAlg == "" || normalAlg == "Blinn-Phong"){
        normalFactor = &microfact::Norm_Blinn_Phong;
        algorithm |= MFNormalBlinn_PhongCode;
        if ( m == 0 ) {
            normalCache[1] = 0;// shininess
        } else {
            normalCache[1] = 2/(m*m) - 2;
            normalCache[0] = (normalCache[1] + 2) / (2*M_PI);// nomalize factor
        }
    }
    else if (normalAlg == "Beckmann") {
        normalFactor = &microfact::Norm_Beckmann;
        algorithm |= MFNormalBeckmannCode;
        normalCache[1] = 1/(m*m);
        normalCache[0] = normalCache[1] / M_PI;
        // TODO: init args
    }
    else if (normalAlg == "GGX") {
        normalFactor = &microfact::Norm_GGX;
        algorithm |= MFNormalGGXCode;
        normalCache[0] = m*m/M_PI;
        normalCache[1] = m*m - 1;
        // TODO: init args
    }
    // TODO: Add more alg
    else {
        std::clog<<"warning: Unknown normal algorithm: "<<normalAlg<<endl;
        assert(0);
    }
    if (geometryAlg == "" || geometryAlg == "Smith") {
        algorithm |= (MFGeometrySmithCode<<4);
        switch (MFNormalMask(algorithm)) {
            case MFNormalBlinn_PhongCode:
                // no closed solution, use Beckmann as suggested
                geometryFactor = &microfact::Geo_Smith_Beckmann;
                break;
            case MFNormalBeckmannCode:
                geometryFactor = &microfact::Geo_Smith_Beckmann;
                break;
            case MFNormalGGXCode:
                geometryFactor = &microfact::Geo_Smith_GGX;
                geometryCache[0] = m*m;
                break;
            default:
                std::clog<<"Warning: Smith alg doesn't support normal alg: "<<get_normal_alg()<<endl;
                assert(0);
                break;
        }
    }
    else if (geometryAlg == "Schlick") {
        algorithm |= (MFGeometrySchlickCode<<4);
        switch (MFNormalMask(algorithm)){
            case MFNormalBeckmannCode:
                std::clog<<"Warning: Schlick-Beckmann use wrong approx, should check before use!";
                geometryFactor = &microfact::Geo_Schlick;
                geometryCache[0] = m*sqrt(2/M_PI);//k
                break;
            case MFNormalGGXCode:
                geometryFactor = &microfact::Geo_Schlick;
                geometryCache[0] = m/2; //k
                break;
            default:
                std::clog<<"Warning: Schlick alg doesn't support normal alg: "<<get_normal_alg()<<endl;
                assert(0);
                break;
        }
    }
    else if (geometryAlg == "implicit") {
        algorithm |= MFGeometryimplicitCode;
        geometryFactor = &microfact::Geo_implicit;
    } else if (geometryAlg == "Cook-Torrance") {
        algorithm |= MFGeometryCook_TorranceCode;
        geometryFactor = &microfact::Geo_CT;
    } else if (geometryAlg == "Kelemen") {
        algorithm |= MFGeometryKelemenCode;
        geometryFactor = &microfact::Geo_Kelemen;
    }
    // TODO: Add more alg
    else {
        std::clog<<"warning: Unknown geometry algorithm: "<<geometryAlg<<endl;
        assert(0);
    }
}

string microfact::get_normal_alg() const {
    switch (MFNormalMask(algorithm)) {
        case MFNormalBlinn_PhongCode:
            return "Blinn-Phong";
        case MFNormalBeckmannCode:
            return "Beckmann";
        case MFNormalGGXCode:
            return "GGX";
        // TODO: Add more alg
        default:
            assert(0 && "found unvalid algorithm");
            break;
    }
    assert(0 && "should not reach here");
    return "";
}
string microfact::get_geometry_alg() const {
    switch (MFGeometryMask(algorithm)) {
        case MFGeometrySmithCode:
            return "Smith";
        case MFGeometrySchlickCode:
            return "Schlick";
        case MFGeometryimplicitCode:
            return "implicit";
        case MFGeometryCook_TorranceCode:
            return "Cook-Torrance";
        case MFGeometryKelemenCode:
            return "Kelemen";
        // TODO: Add more alg
        default:
            assert(0 && "found unvalid algorithm");
            break;
    }
    assert(0 && "should not reach here");
    return "";
}
double microfact::Fernel_Schlick(double cosine, double ref_idx) const {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}
/* #endRegion */

/* #Region Normal Factors */
double microfact::Norm_Blinn_Phong(double ndotm) const {
    // TODO: Test
    // assert(0 && "not tested!");
    // Code here
    return normalCache[0]*pow(ndotm, normalCache[1]);
}

double microfact::Norm_Beckmann(double ndotm) const {
    // TODO: Test
    // assert(0 && "not tested!");
    // Code here
    double inv_ndotm_sq = 1 / (ndotm*ndotm);
    return normalCache[0]*inv_ndotm_sq*inv_ndotm_sq*exp( normalCache[1]*(1 - inv_ndotm_sq) );
}

double microfact::Norm_GGX(double ndotm) const {
    // TODO: Test
    // assert(0 && "not tested!");
    // Code here
    return normalCache[0]/pow(ndotm*ndotm*normalCache[1] + 1, 2);
}
// TODO: add alg
/* #endRegion */

/* #Region Geometry Factors */
double microfact::Geo_Smith_Beckmann(double ndotl, double ndotv, double mdotv, double ndotm) const {
    // TODO: Test
    // assert(0 && "not tested!");
    // Code here
    double cv = ndotv/(m*sqrt(1-ndotv*ndotv));
    double g1v;
    if (cv < 1.6) {
        double cv_sq = cv*cv;
        g1v = (3.535*cv + 2.181*cv_sq) / (1 + 2.276*cv + 2.577*cv_sq);
    } else {
        g1v = 1;
    }
    double cl = ndotl/(m*sqrt(1-ndotl*ndotl));
    double g1l;
    if (cl < 1.6) {
        double cl_sq = cl*cl;
        g1l = (3.535*cl + 2.181*cl_sq) / (1 + 2.276*cl + 2.577*cl_sq);
    } else {
        g1l = 1;
    }
    return g1v*g1l;
}
double microfact::Geo_Smith_GGX(double ndotl, double ndotv, double mdotv, double ndotm) const{
    // TODO: Test
    // assert(0 && "not tested!");
    // Code here
    double g1v = 2*ndotv / (ndotv + sqrt(geometryCache[0] + (1-geometryCache[0])*ndotv*ndotv));
    double g1l = 2*ndotl / (ndotl + sqrt(geometryCache[0] + (1-geometryCache[0])*ndotl*ndotl));
    return g1v*g1l;
}
double microfact::Geo_Schlick(double ndotl, double ndotv, double mdotv, double ndotm) const{
    // TODO: Test
    // assert(0 && "not tested!");
    // Code here
    return (ndotv / (geometryCache[0] + (1-geometryCache[0])*ndotv))
        *  (ndotl / (geometryCache[0] + (1-geometryCache[0])*ndotl));
}
double microfact::Geo_implicit(double ndotl, double ndotv, double mdotv, double ndotm) const{
    // TODO: Test
    // assert(0 && "not tested!");
    // Code here
    return ndotl*ndotv;
}
double microfact::Geo_CT(double ndotl, double ndotv, double mdotv, double ndotm) const{
    // TODO: Test
    // assert(0 && "not tested!");
    // Code here
    double common = 2*ndotm/mdotv;
    return Fmin(1, common*ndotv, common*ndotl);
}
double microfact::Geo_Kelemen(double ndotl, double ndotv, double mdotv, double ndotm) const{
    // TODO: Test
    // assert(0 && "not tested!");
    // Code here
    return ndotl*ndotv/(mdotv*mdotv);
}
// TODO: add alg
/* #endRegion */ 
#endif