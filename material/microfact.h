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
typedef double (microfact::*normFunc)(vec3, vec3) const;
typedef double (microfact::*geoFunc)(vec3, vec3, vec3, vec3) const;

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
        float shininess, BPNormalize;
        normFunc normalFactor;
        geoFunc geometryFactor;
        /*
            wn: surface normal, wm: current normal,
            wl: light direction, wv: view direction
            all should be unit length
        */
        double Norm_Blinn_Phong(vec3 wn, vec3 wm) const;
        double Norm_Beckmann(vec3 wn, vec3 wm) const;
        double Norm_GGX(vec3 wn, vec3 wm) const;
        double Geo_Smith_Beckmann(vec3 wn, vec3 wm, vec3 wl, vec3 wv) const;
        double Geo_Smith_GGX(vec3 wn, vec3 wm, vec3 wl, vec3 wv) const;
        double Geo_Schlick_Beckmann(vec3 wn, vec3 wm, vec3 wl, vec3 wv) const;
        double Geo_Schlick_GGX(vec3 wn, vec3 wm, vec3 wl, vec3 wv) const;
        double Geo_implicit(vec3 wn, vec3 wm, vec3 wl, vec3 wv) const;
        double Geo_CT(vec3 wn, vec3 wm, vec3 wl, vec3 wv) const;
        double Geo_Kelemen(vec3 wn, vec3 wm, vec3 wl, vec3 wv) const;
};


/* #Region assist funcs*/
void microfact::updateAlg(string normalAlg, string geometryAlg) {
    // can apply hash?
    algorithm = 0;
    assert(0<=m && m<=1 && "Roughness out of range!");
    if (normalAlg == "" || normalAlg == "Blinn-Phong"){
        if ( m == 0 ) {
            shininess = 0;
        } else {
            shininess = 2/(m*m) - 2;
            BPNormalize = (shininess + 2) / (2*M_PI);
        }
    }
    // TODO: Add more alg
    else {
        clog<<"warning: Unknown normal algorithm: "<<normalAlg<<endl;
    }
    if (geometryAlg == "" || geometryAlg == "Smith") {}
    else if (geometryAlg == "implicit") {
        algorithm |= MFGeometryimplicitCode;
    } else if (geometryAlg == "Cook-Torrance") {
        algorithm |= MFGeometryCook_TorranceCode;
    }
    // TODO: Add more alg
    else {
        clog<<"warning: Unknown geometry algorithm: "<<geometryAlg<<endl;
    }
}

string microfact::get_normal_alg() const {
    switch (MFNormalMask(algorithm)) {
        case MFNormalBlinn_PhongCode:
            return "Blinn-Phong";
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
        case MFGeometryimplicitCode:
            return "implicit";
        case MFGeometryCook_TorranceCode:
            return "Cook-Torrance";
        // TODO: Add more alg
        default:
            assert(0 && "found unvalid algorithm");
            break;
    }
    assert(0 && "should not reach here");
    return "";
}
/* #endRegion */

// # Low efficiency -> use function varable instead
// double microfact::normalFactor(vec3 surfaceNorm, vec3 currentNorm) const {
//     // NOTE: input norm should be unit length!
//     switch (MFNormalMask(algorithm)) {
//         case MFNormalBlinn_PhongCode:
//             if (shininess!=0) {
//                 return BPNormalize*pow(dot(surfaceNorm, currentNorm), shininess);
//             } else {    // specular
//                 assert(0 && "should check and design how to handle this case");
//                 /*
//                     it's possible to handle by adding sampler to mixed_sampler,
//                     but it's better not to use monte carlo if such (i.e. handle 
//                     in specular case instead)
//                 */
//                 if (dot(surfaceNorm, currentNorm) < 0.99999999) {
//                     return 0;
//                 } else {
//                     return 1;
//                 }
//             }
//         // TODO: Add more alg
//         default:
//             assert(0 && "found unvalid alg");
//             break;
//     }
//     assert(0 && "should not reach here");
//     return 1;
// }
#endif