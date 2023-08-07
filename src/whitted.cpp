#include <nori/integrator.h>
#include <nori/emitter.h>
#include <nori/scene.h>
#include <nori/bsdf.h>

NORI_NAMESPACE_BEGIN

class WhittedIntegrator : public Integrator {
public:
    WhittedIntegrator(const PropertyList &propList) { }

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
        Intersection its;
        if (!scene->rayIntersect(ray, its)) {
            return Color3f(0.0f);
        }
        Color3f Le(0.0f), fr(0.0f);
        // self-luminous
        if (its.mesh->isEmitter()) {
            EmitterQueryRecord eRec(ray.o, its.p, its.shFrame.n);
            Le = its.mesh->getEmitter()->eval(eRec);
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
            float cosTheta = Frame::cosTheta(its.toLocal(-eRec.wi));
            BSDFQueryRecord bRec(its.toLocal(-ray.d), its.toLocal(-eRec.wi), ESolidAngle);
            fr = its.mesh->getBSDF()->eval(bRec);

            return Le + emission * fr * cosTheta / sampleEmitterPdf;
        }
        else {
            BSDFQueryRecord bRec(its.toLocal(-ray.d));
            Color3f color = its.mesh->getBSDF()->sample(bRec, sampler->next2D());
            if (sampler->next1D() < 0.95f && color.x() > 0.0f) {
                return color * Li(scene, sampler, Ray3f(its.p, its.toWorld(bRec.wo))) / 0.95f;
            }
            else return Color3f(0.0f);
        }
    }

    std::string toString() const {
        return "WhittedIntegrator[]";
    }

};

NORI_REGISTER_CLASS(WhittedIntegrator, "whitted")
NORI_NAMESPACE_END