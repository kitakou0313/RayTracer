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
    if (world->hit(r, 0.0, FLT_MAX, rec))
    {
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
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
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());
    std::uniform_real_distribution<> dist(0.0, 1.0);
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
                float u = float(i + dist(engine)) / float(nx);
                float v = float(j + dist(engine)) / float(ny);

                ray r = cam.getRay(u, v);

                vec3 p = r.pointAtParameter(2.0);

                col += color(r, world);
            }

            col /= float(ns);

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}