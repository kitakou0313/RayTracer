#include <iostream>
#include <random>
#include <bits/stdc++.h>
#include "sphere.h"
#include "hitablelist.h"
#include "float.h"
#include "camera.h"

using namespace std;

vec3 color(const ray &r, hitable *world)
{
    hitRecord rec;
    if (world->hit(r, 0.001, FLT_MAX, rec))
    {
        vec3 target = rec.p + rec.normal + randomInUnitsphere();
        return 0.5 * color(ray(rec.p, target - rec.p), world);
    }
    else
    {
        vec3 unitDirection = unitVector(r.direction());
        float t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main()
{

    ofstream out("output.ppm");
    cout.rdbuf(out.rdbuf());

    int nx = 200;
    int ny = 100;
    int ns = 100;

    cout << "P3\n"
         << nx << " " << ny << "\n255\n";

    hitable *list[2];

    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitableList(list, 2);

    camera cam;

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + drand()) / float(nx);
                float v = float(j + drand()) / float(ny);

                ray r = cam.getRay(u, v);

                vec3 p = r.pointAtParameter(2.0);

                col += color(r, world);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}