#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera
{
public:
    camera(vec3 lookFrom, vec3 lookAt, vec3 vup, float vfov, float aspect)
    {
        vec3 u, v, w;
        float theta = vfov * M_PI / 180;
        float halfHeight = tan(theta / 2);
        float halfWidth = aspect * halfHeight;

        origin = lookFrom;
        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(vup, w));
        v = cross(w, u);

        lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
        horizontal = 2 * halfWidth * u;
        vertical = 2 * halfHeight * v;
    }

    ray getRay(float u, float v)
    {
        return ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
    }

    vec3 origin;
    vec3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
};

#endif