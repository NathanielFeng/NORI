/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#include <nori/warp.h>
#include <nori/vector.h>
#include <nori/frame.h>

NORI_NAMESPACE_BEGIN

Point2f Warp::squareToUniformSquare(const Point2f &sample) {
    return sample;
}

float Warp::squareToUniformSquarePdf(const Point2f &sample) {
    return ((sample.array() >= 0).all() && (sample.array() <= 1).all()) ? 1.0f : 0.0f;
}

Point2f Warp::squareToTent(const Point2f &sample) {
    auto tent = [](const float& x) -> float {
        if(x < 0.5f) return std::sqrt(2 * x) - 1;
        else return 1 - std::sqrt(2 - 2 * x);
    };
    return Point2f(tent(sample.x()), tent(sample.y()));
}

float Warp::squareToTentPdf(const Point2f &p) {
    float x = 0.0f, y = 0.0f;
    if(-1 <= p.x() && p.x() <= 1) x = 1 - std::abs(p.x());
    if(-1 <= p.y() && p.y() <= 1) y = 1 - std::abs(p.y());
    return x * y;
}

Point2f Warp::squareToUniformDisk(const Point2f &sample) {
    float x = sqrt(sample.x()) * cos(2 * M_PI * sample.y());
    float y = sqrt(sample.x()) * sin(2 * M_PI * sample.y());
    return Point2f(x, y);
}

float Warp::squareToUniformDiskPdf(const Point2f &p) {
    if(p.x() * p.x() + p.y() * p.y() > 1) return 0.0f;
    else return INV_PI;
}

Vector3f Warp::squareToUniformSphere(const Point2f &sample) {
    float temp = 1 - 2 * sample.x();
    float x = sqrt(1 - temp * temp) * cos(2 * M_PI * sample.y());
    float y = sqrt(1 - temp * temp) * sin(2 * M_PI * sample.y());
    float z = temp;
    return Vector3f(x, y, z);
}

float Warp::squareToUniformSpherePdf(const Vector3f &v) {
    if(v.x() * v.x() + v.y() * v.y() + v.z() * v.z() > 1) return 0.0f;
    else return 0.25f * INV_PI;
}

Vector3f Warp::squareToUniformHemisphere(const Point2f &sample) {
    float temp = 1 - sample.x();
    float x = sqrt(1 - temp * temp) * cos(2 * M_PI * sample.y());
    float y = sqrt(1 - temp * temp) * sin(2 * M_PI * sample.y());
    float z = temp;
    return Vector3f(x, y, z);
}

float Warp::squareToUniformHemispherePdf(const Vector3f &v) {
    if(v.x() * v.x() + v.y() * v.y() + v.z() * v.z() > 1 || v.z() < 0) return 0.0f;
    else return 0.5f * INV_PI;
}

Vector3f Warp::squareToCosineHemisphere(const Point2f &sample) {
    float x = sqrt(sample.x()) * cos(2 * M_PI * sample.y());
    float y = sqrt(sample.x()) * sin(2 * M_PI * sample.y());
    float z = sqrt(1 - sample.x());
    return Vector3f(x, y, z);
}

float Warp::squareToCosineHemispherePdf(const Vector3f &v) {
    if(v.x() * v.x() + v.y() * v.y() + v.z() * v.z() > 1 || v.z() < 0) return 0.0f;
    else return v.z() * INV_PI;
}

Vector3f Warp::squareToBeckmann(const Point2f &sample, float alpha) {
    float temp = - alpha * alpha * log(1 - sample.x());
    float x = cos(2 * M_PI * sample.y()) / sqrt(1 + 1 / temp);
    float y = sin(2 * M_PI * sample.y()) / sqrt(1 + 1 / temp);
    float z = 1 / sqrt(1 + temp);
    return Vector3f(x, y, z);
}

float Warp::squareToBeckmannPdf(const Vector3f &m, float alpha) {
    if(m.z() <= 1e-5) return 0.0f;
    float alpha2 = alpha * alpha;
    float cos3 = m.z() * m.z() * m.z();
    return exp((m.z() * m.z() - 1) / (m.z() * m.z() * alpha2)) / (alpha2 * cos3) * INV_PI;

}

NORI_NAMESPACE_END
