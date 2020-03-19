#ifndef PERLINH
#define PERLINH

#include "random.h"
#include "vec3.h"

inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w)
{
    float uu = u * u * (3 - 2 * u);
    float vv = v * v * (3 - 2 * v);
    float ww = w * w * (3 - 2 * w);
    float accum = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                vec3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
            }
    return accum;
}

class perlin
{
public:
    float noise(const vec3 &p) const
    {
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());

        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);
        w = w * w * (3 - 2 * w);

        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());

        vec3 c[2][2][2];
        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[permX[(i + di) & 255] ^
                                           permY[(j + dj) & 255] ^
                                           permZ[(k + dk) & 255]];
        return perlin_interp(c, u, v, w);
    }

    float turb(const vec3 &p, int depth = 7) const
    {
        float accum = 0;
        vec3 tmpP = p;
        float weight = 1.0;
        for (int i = 0; i < depth; i++)
        {
            accum += weight * noise(tmpP);
            weight *= 0.5;
            tmpP *= 2;
        }
        return fabs(accum);
    }

    static vec3 *ranvec;
    static int *permX;
    static int *permY;
    static int *permZ;
};

static vec3 *perlinGenerate()
{
    vec3 *p = new vec3[256];
    for (int i = 0; i < 256; i++)
    {
        double xRan = 2 * drand() - 1;
        double yRan = 2 * drand() - 1;
        double zRan = 2 * drand() - 1;
        p[i] = unitVector(vec3(xRan, yRan, zRan));
    }
    return p;
}

void permute(int *p, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int target = int(drand() * (i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
    return;
}

static int *perlinGeneratePerm()
{
    int *p = new int[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = i;
    }
    permute(p, 256);
    return p;
}

vec3 *perlin::ranvec = perlinGenerate();
int *perlin::permX = perlinGeneratePerm();
int *perlin::permY = perlinGeneratePerm();
int *perlin::permZ = perlinGeneratePerm();

#endif