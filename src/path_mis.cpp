#include <nori/integrator.h>
#include <nori/emitter.h>
#include <nori/scene.h>
#include <nori/bsdf.h>

NORI_NAMESPACE_BEGIN

class PathMisIntegrator : public Integrator {
public:
    PathMisIntegrator(const PropertyList &propList) { }

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
        Color3f color(0.0f), t(1.0f);
        Ray3f r = ray;
        float p = 0.0f, w_mats = 1.0f;
        int depth = 1;
        while (true) {
            Intersection its;
            if (!scene->rayIntersect(r, its)) {
                break;
            }
            if (its.mesh->isEmitter()) {
                EmitterQueryRecord eRec(r.o, its.p, its.shFrame.n);
                color += t * w_mats * its.mesh->getEmitter()->eval(eRec);
            }
            // direct light (sample light)
            Emitter *emitter = scene->sampleEmitter(sampler);
            float sampleEmitterPdf = scene->sampleEmitterPdf();
            EmitterQueryRecord eRec(its.p);
            Color3f emission = emitter->sample(eRec, sampler);
            float pdf_ems = emitter->pdf(eRec);
            if (!scene->rayIntersect(eRec.shadowRay)) {
                float cosTheta = fabs(Frame::cosTheta(its.toLocal(-eRec.wi)));
                BSDFQueryRecord bRec(its.toLocal(-r.d), its.toLocal(-eRec.wi), ESolidAngle);
                Color3f fr = its.mesh->getBSDF()->eval(bRec);
                float pdf_mats = its.mesh->getBSDF()->pdf(bRec);
                float w_ems = pdf_ems;
                if (pdf_ems + pdf_mats > 0.0f) 
                    w_ems = pdf_ems / (pdf_ems + pdf_mats);
                // if (std::isnan(w_ems)) {
                //     std::cerr << "w_ems = " << w_ems << "\n"
                //             << "pdf_ems = " << pdf_ems << "\n"
                //             << "pdf_mats = " << pdf_mats << "\n----------------------\n";
                //}
                color += emission * fr * cosTheta * t * w_ems / sampleEmitterPdf;
            }
            
            // Russian Roulette
            if (depth >= 3) {
                p = std::min(t.maxCoeff(), 0.99f);
                if (sampler->next1D() > p) {
                    break;
                }
                t /= p;
            }

            // indirect light (sample BSDF)
            BSDFQueryRecord bRec(its.toLocal(-r.d));
            Color3f fr = its.mesh->getBSDF()->sample(bRec, sampler->next2D());
            float pdf_mats = its.mesh->getBSDF()->pdf(bRec);
            r = Ray3f(its.p, its.toWorld(bRec.wo));
            Intersection its2;
            if (scene->rayIntersect(r, its2)) {
                if (its2.mesh->isEmitter()) {
                    EmitterQueryRecord eRec2(its.p, its2.p, its2.shFrame.n);
                    float pdf_ems2 = its2.mesh->getEmitter()->pdf(eRec2);
                    w_mats = pdf_mats;
                    if (pdf_ems2 + pdf_mats > 0.0f)
                        w_mats = pdf_mats / (pdf_ems2 + pdf_mats);
                    // std::cerr << "w_mats = " << w_mats << "\n------------------\n";
                }
            }
            if (bRec.measure == EDiscrete) {
                w_mats = 1.0f;
            }
            t *= fr;
            ++depth;
        }
        return color;
    }

    std::string toString() const {
        return "PathMisIntegrator[]";
    }

};

NORI_REGISTER_CLASS(PathMisIntegrator, "path_mis")
NORI_NAMESPACE_END