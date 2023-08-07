#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/warp.h>

NORI_NAMESPACE_BEGIN

class AOIntegrator : public Integrator {
public:
    AOIntegrator(const PropertyList &props) {}

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
        /**
         * Ambient occlusion
        */
        Intersection its;
        if (!scene->rayIntersect(ray, its))
            return Color3f(0.0f);
        Point3f p = its.p;
        Point2f sample = sampler->next2D();
        Vector3f wi = Warp::squareToCosineHemisphere(sample);
        wi = its.shFrame.toWorld(wi);
        
        float visible = 1.0f;
        if (scene->rayIntersect(Ray3f(p, wi)))
            visible = 0.0f;
        /**
         * In fact, Li = visible * (cos\theta / pi) / pdf, 
         * but here squareToCosineHemispherePdf == (cos/theta),
         * so, Li = visible.
        */
        Color3f Li(visible);
        return Li;
    }

    std::string toString() const {
        return "AOIntegrator[]";
    }
};

NORI_REGISTER_CLASS(AOIntegrator, "ao");
NORI_NAMESPACE_END