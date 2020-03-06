#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class material;
struct hitRecord
{
    float t;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
};

class hitable
{
public:
    virtual bool hit(const ray &r, float tMin, float tMax, hitRecord &rec) const = 0;
};

class material
{
public:
    virtual bool scatter(const ray &rIn, const hitRecord &rec, vec3 &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const vec3 &a) : albedo(a) {}
    virtual bool scatter(const ray &r_in, const hitRecord &rec, vec3 &attenuation, ray &scattered) const
    {
        vec3 target = rec.p + rec.normal + randomInUnitsphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }
    vec3 albedo;
};

class metal : public material
{
public:
    metal(const vec3 &a) : albedo(a) {}
    virtual bool scatter(const ray &rayIn, const hitRecord &rec, vec3 &attenuation, ray &scattered) const
    {
        vec3 reflected = reflect(unitVector(rayIn.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vec3 albedo;
};

float schlick(float cosine, float refIdx)
{
    float r0 = (1 - refIdx) / (1 + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class dielectric : public material
{
public:
    dielectric(float ri) : refIdx(ri) {}
    virtual bool scatter(const ray &rIn, const hitRecord &rec, vec3 &attenuation, ray &scattered) const
    {
        vec3 outwardNormal;
        vec3 reflected = reflect(rIn.direction(), rec.normal);
        float niOverNt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflectProb;
        float cosine;
        if (dot(rIn.direction(), rec.normal) > 0)
        {
            outwardNormal = -rec.normal;
            niOverNt = refIdx;
            cosine = refIdx * dot(rIn.direction(), rec.normal) / rIn.direction().length();
        }
        else
        {
            outwardNormal = rec.normal;
            niOverNt = 1.0 / refIdx;
            cosine = -dot(rIn.direction(), rec.normal) / rIn.direction().length();
        }
        if (refract(rIn.direction(), outwardNormal, niOverNt, refracted))
        {
            reflectProb = schlick(cosine, refIdx);
        }
        else
        {
            scattered = ray(rec.p, reflected);
            reflectProb = 1.0;
        }
        if (drand() < reflectProb)
        {
            scattered = ray(rec.p, reflected);
        }
        else
        {
            scattered = ray(rec.p, refracted);
        }

        return true;
    }
    float refIdx;
};

#endif