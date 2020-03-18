#ifndef SPHEREH
#define SPHEREH
#include "hitable.h"

class sphere : public hitable
{
public:
    sphere() {}
    sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m){};
    virtual bool hit(const ray &r, float tMin, float tMax, hitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, aabb &box) const;
    vec3 center;
    float radius;
    material *mat_ptr;
};

bool sphere::hit(const ray &r, float tMin, float tMax, hitRecord &rec) const
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < tMax && temp > tMin)
        {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < tMax && temp > tMin)
        {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}

bool sphere::boundingBox(float t0, float t1, aabb &box) const
{
    box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}

class movingSphere : public hitable
{
public:
    movingSphere() {}
    movingSphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m)
        : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m){};

    virtual bool hit(const ray &r, float tmin, float tmax, hitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, aabb &box) const;
    vec3 center(float time) const;
    vec3 center0, center1;
    float time0, time1;
    float radius;
    material *mat_ptr;
};

vec3 movingSphere::center(float time) const
{
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool movingSphere::hit(const ray &r, float tMin, float tMax, hitRecord &rec) const
{
    vec3 oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < tMax && temp > tMin)
        {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < tMax && temp > tMin)
        {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}

bool movingSphere::boundingBox(float t0, float t1, aabb &box) const
{
    aabb box0(center(t0) - vec3(radius, radius, radius),
              center(t0) + vec3(radius, radius, radius));

    aabb box1(center(t1) - vec3(radius, radius, radius),
              center(t1) + vec3(radius, radius, radius));

    box = surroundingBox(box0, box1);
    return true;
}

#endif