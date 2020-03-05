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

#endif