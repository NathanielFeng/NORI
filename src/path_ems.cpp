#include <nori/integrator.h>
#include <nori/emitter.h>
#include <nori/scene.h>
#include <nori/bsdf.h>

NORI_NAMESPACE_BEGIN

class PathEmsIntegrator : public Integrator {
public:
    PathEmsIntegrator(const PropertyList &propList) { }

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
        Color3f color(0.0f), t(1.0f);
        Ray3f r = ray;
        float p = 0.0f;
        int depth = 1, isDelta = 1;
        while (true) {
            Intersection its;
            if (!scene->rayIntersect(r, its)) {
                break;
            }
            if (its.mesh->isEmitter()) {
                EmitterQueryRecord eRec(r.o, its.p, its.shFrame.n);
                color += t * its.mesh->getEmitter()->eval(eRec) * isDelta;
            }
            if (its.mesh->getBSDF()->isDiffuse()) {
                // direct light (sample light)
                Emitter *emitter = scene->sampleEmitter(sampler);
                float sampleEmitterPdf = scene->sampleEmitterPdf();
                EmitterQueryRecord eRec(its.p);
                Color3f emission = emitter->sample(eRec, sampler);
                if (scene->rayIntersect(eRec.shadowRay)) {
                    emission = 0.0f;
                }
                float cosTheta = fabs(Frame::cosTheta(its.toLocal(-eRec.wi)));
                BSDFQueryRecord bRec(its.toLocal(-r.d), its.toLocal(-eRec.wi), ESolidAngle);
                Color3f fr = its.mesh->getBSDF()->eval(bRec);

                color += emission * fr * cosTheta * t / sampleEmitterPdf;
                isDelta = 0;
            }
            else isDelta = 1;
            // Russian Roulette
            if (depth >= 3) {
                p = std::min(t.maxCoeff(), 0.99f);
                if (sampler->next1D() > p) {
                    break;
                }
                t /= p;
            }
            BSDFQueryRecord bRec(its.toLocal(-r.d));
            Color3f fr = its.mesh->getBSDF()->sample(bRec, sampler->next2D());
            r = Ray3f(its.p, its.toWorld(bRec.wo));
            t *= fr;
            ++depth;
        }
        return color;
    }

    std::string toString() const {
        return "PathEmsIntegrator[]";
    }

};

NORI_REGISTER_CLASS(PathEmsIntegrator, "path_ems")
NORI_NAMESPACE_END