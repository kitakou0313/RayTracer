#ifndef PERLINH
#define PERLINH

#include "vec3.h"

class perlin
{
public:
    float noise(const vec3 &p) const
    {
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());

        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        return ranfloat[permX[i] ^ permY[j] ^ permZ[k]];
    }

    static float *ranfloat;
    static int *permX;
    static int *permY;
    static int *permZ;
};

static float *perlinGenerate()
{
    float *p = new float[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = drand();
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

float *perlin::ranfloat = perlinGenerate();

int *perlin::permX = perlinGeneratePerm();
int *perlin::permY = perlinGeneratePerm();
int *perlin::permZ = perlinGeneratePerm();

#endif