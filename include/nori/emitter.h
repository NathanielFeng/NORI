/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#pragma once

#include <nori/object.h>
#include <nori/sampler.h>
#include <nori/mesh.h>

NORI_NAMESPACE_BEGIN

struct EmitterQueryRecord {
    Point3f p;
    Point3f lightP;
    Normal3f n;
    Vector3f wi;
    Ray3f shadowRay;

    EmitterQueryRecord(const Point3f &p) : p(p) { }

    EmitterQueryRecord(const Point3f &p, 
            const Point3f &lightP, const Normal3f &n)
        : p(p), lightP(lightP), n(n) { 
            wi = (p - lightP).normalized();
        }
};

/**
 * \brief Superclass of all emitters
 */
class Emitter : public NoriObject {
public:
    /**
     * \brief Uniformly sample the emitter
     * \param sampler
     *      Random generator  
    */
    virtual Color3f sample(EmitterQueryRecord &eRec, Sampler *sampler) const = 0;

    /**
     * \brief Evaluate the emitter
    */
    virtual Color3f eval(const EmitterQueryRecord &eRec) const = 0;

    /**
     * \brief Compute the probability of sampling
    */
    virtual float pdf(const EmitterQueryRecord &eRec) const = 0;

    void setMesh(Mesh *mesh) { m_mesh = mesh; }

    /**
     * \brief Return the type of object (i.e. Mesh/Emitter/etc.) 
     * provided by this instance
     * */
    EClassType getClassType() const { return EEmitter; }

protected:
    Color3f m_radiance;
    Mesh *m_mesh;
};

NORI_NAMESPACE_END
