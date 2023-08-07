/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#include <nori/bsdf.h>
#include <nori/frame.h>

NORI_NAMESPACE_BEGIN

static Vector3f refract(const Vector3f &wi, const Normal3f &N, float extIOR, float intIOR) {
    float cosTheta = wi.dot(N);
    float etaI = extIOR, etaT = intIOR;
    Normal3f n = N;
    if (cosTheta < 0.0f) {
        cosTheta = -cosTheta;
        std::swap(etaI, etaT);
        n = -N;
    }
    float eta = etaI / etaT;
    float k = 1 - eta * eta * (1.0f - cosTheta * cosTheta);
    if (k < 0) return Vector3f(0.0f);
    return -eta * wi + (eta * cosTheta - sqrtf(k)) * n; 
}


/// Ideal dielectric BSDF
class Dielectric : public BSDF {
public:
    Dielectric(const PropertyList &propList) {
        /* Interior IOR (default: BK7 borosilicate optical glass) */
        m_intIOR = propList.getFloat("intIOR", 1.5046f);

        /* Exterior IOR (default: air) */
        m_extIOR = propList.getFloat("extIOR", 1.000277f);
    }

    Color3f eval(const BSDFQueryRecord &) const {
        /* Discrete BRDFs always evaluate to zero in Nori */
        return Color3f(0.0f);
    }

    float pdf(const BSDFQueryRecord &) const {
        /* Discrete BRDFs always evaluate to zero in Nori */
        return 0.0f;
    }

    Color3f sample(BSDFQueryRecord &bRec, const Point2f &sample) const {
        bRec.measure = EDiscrete;
        float kr = fresnel(Frame::cosTheta(bRec.wi), m_extIOR, m_intIOR);
        if (sample.x() < kr) {
            bRec.wo = Vector3f(-bRec.wi.x(), -bRec.wi.y(), bRec.wi.z());
            bRec.eta = 1.0f;
        }
        else {
            float cosTheta = Frame::cosTheta(bRec.wi);
            bRec.wo = refract(bRec.wi, Normal3f(0, 0, 1), m_extIOR, m_intIOR);
            bRec.eta = cosTheta > 0 ? m_extIOR / m_intIOR : m_intIOR / m_extIOR;
        }
        return Color3f(1.0f);
    }

    std::string toString() const {
        return tfm::format(
            "Dielectric[\n"
            "  intIOR = %f,\n"
            "  extIOR = %f\n"
            "]",
            m_intIOR, m_extIOR);
    }
private:
    float m_intIOR, m_extIOR;
};

NORI_REGISTER_CLASS(Dielectric, "dielectric");
NORI_NAMESPACE_END
