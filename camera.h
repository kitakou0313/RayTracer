#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera
{
public:
    camera(vec3 lookFrom, vec3 lookAt, vec3 vup, float vfov, float aspect, float aperture, float focusDist,
           float t0, float t1)
    {
        time0 = t0;
        time1 = t1;
        lensRadius = aperture / 2;

        float theta = vfov * M_PI / 180;
        float halfHeight = tan(theta / 2);
        float halfWidth = aspect * halfHeight;

        origin = lookFrom;
        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(vup, w));
        v = cross(w, u);

        lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
        horizontal = 2 * halfWidth * focusDist * u;
        vertical = 2 * halfHeight * focusDist * v;
    }

    ray getRay(float s, float t)
    {
        vec3 rd = lensRadius * randomInUnitDisk();
        vec3 offset = u * rd.x() + v * rd.y();
        float time = time0 + drand() * (time1 - time0);
        return ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset,
                   time);
    }

    vec3 origin;
    vec3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float time0, time1;
    float lensRadius;
};

#endif