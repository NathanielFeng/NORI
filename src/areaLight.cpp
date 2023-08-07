#include <nori/emitter.h>

NORI_NAMESPACE_BEGIN

class AreaLight : public Emitter {
public:
    AreaLight(const PropertyList &propList) {
        m_radiance = propList.getColor("radiance");
    }

    Color3f sample(EmitterQueryRecord &eRec, Sampler *sampler) const {
        m_mesh->sampleSurface(sampler, eRec.lightP, eRec.n);
        eRec.wi = (eRec.p - eRec.lightP).normalized();
        eRec.shadowRay = Ray3f(eRec.p, -eRec.wi, Epsilon, (eRec.p - eRec.lightP).norm() - Epsilon);
        float p = pdf(eRec);
        if (p <= 0.0f)
            return Color3f(0.0f);
        return eval(eRec) / p;
    }

    Color3f eval(const EmitterQueryRecord &eRec) const {
        float cosTheta = eRec.n.dot(eRec.wi);
        if (cosTheta <= 0.0f)
            return Color3f(0.0f);
        return m_radiance;
    }

    float pdf(const EmitterQueryRecord &eRec) const {
        float cosTheta = eRec.n.dot(eRec.wi);
        if (cosTheta <= 0.0f)
            return 0.0f;
        return m_mesh->getPDF() * (eRec.p - eRec.lightP).squaredNorm() / cosTheta;
    }

    // Return a human-readable summary
    std::string toString() const {
        return tfm::format(
            "AreaLight[\n"
            "  m_radiance = %f, %f, %f\n"
            "]", m_radiance.x(), m_radiance.y(), m_radiance.z());
    }

};

NORI_REGISTER_CLASS(AreaLight, "area");
NORI_NAMESPACE_END
