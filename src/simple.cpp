#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/warp.h>

NORI_NAMESPACE_BEGIN

class SimpleIntegrator : public Integrator {
public:
    SimpleIntegrator(const PropertyList &props) {
        position = props.getPoint("position");
        energy = props.getColor("energy");
    }

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
        /**
         * Point lights
        */
        Intersection its;
        if (!scene->rayIntersect(ray, its))
            return Color3f(0.0f);
        Point3f p = its.p;
        Point3f x = position;
        Normal3f n = its.shFrame.n;
        float visible = 1.0f;
        Vector3f wi = (x - p).normalized();
        /**
         * shadowRay to check whether an intersection exists 
         * rather than having to find the closest one.
        */
        if (scene->rayIntersect(Ray3f(p, wi)))  
            visible = 0.0f;
        float dis = (x - p).dot(x - p);
        Color3f Li = energy * INV_FOURPI * INV_PI * std::max(0.0f, n.dot(wi)) / dis * visible;
        return Li;
    }

    std::string toString() const {
        return "SimpleIntegrator[]";
    }

private:
    Point3f position;
    Color3f energy;
};

NORI_REGISTER_CLASS(SimpleIntegrator, "simple");
NORI_NAMESPACE_END