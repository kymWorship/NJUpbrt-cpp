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
            // TODO: Add more alg
        geometry_alg(string): alg used to simulate blocking relationship
            input "" to use default
            "Smith"(defualt): TODO:
            "implicit"：TODO:
            "Cook-Torrance": TODO:
            // TODO: Add more alg
        #color(vec3/string/shared_ptr<texture>): [0, 1) rgb
        #index(float/vec3): [1, infin] vec3 for rgb channel
*/

/* #region fake hash macro */
// mask
#define MFNormalMask(alg) (alg>>4)
#define MFGeometryMask(alg) (alg&0b00001111)
// normal codes: 0 - 15
#define MFNormalBlinn_PhongCode 0 // (default)
// geometry codes: 0 - 15
#define MFGeometrySmithCode 0 // (default)
#define MFGeometryimplicitCode 1
#define MFGeometryCook_TorranceCode 2
/* #endregion */

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
        virtual double scatterign_pdf(
            const ray& r_in, const hit_rec& h_rec, const ray& r_out
        ) const;
        void updateAlg(string normalAlg, string geometryAlg) {
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
        string get_normal_alg() {
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
        string get_geometry_alg() {
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
    private:
        // algorithm 0b 0000(normal_alg) 0000(geometry_alg)
        unsigned char algorithm;
        float m; // roughness
        // UE4Roughness = m^2
        shared_ptr<texture> attenuation;
        vec3 n; // index for rgb channel
        float shininess, BPNormalize;
        double normalFactor(vec3 surfaceNorm, vec3 currentNorm) {
            switch (MFNormalMask(algorithm)) {
                case MFNormalBlinn_PhongCode:
                    if (shininess!=0) {
                        return BPNormalize*pow(dot(surfaceNorm, currentNorm), shininess);
                    } else {    // specular
                        assert(0 && "should check and design how to handle this case");
                        /* 
                            it's possible to handle by adding sampler to mixed_sampler,
                            but it's better not to use monte carlo if such (i.e. handle 
                            in specular case instead)
                        */
                       if (dot(surfaceNorm, currentNorm) < 0.99999999) {
                           return 0;
                       } else {
                           return 1;
                       }
                    }
                default:
                    assert(0 && "found unvalid alg");
                    break;
            }
            assert(0 && "should not reach here");
            return 1;
        }
};

#endif