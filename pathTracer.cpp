#include <iostream>
#include <random>
#include <bits/stdc++.h>
#include "sphere.h"
#include "hitablelist.h"
#include "float.h"
#include "camera.h"
#include "bvh.h"

#include <chrono>

using namespace std;

hitable *randomScene()
{
    int n = 50000;
    hitable **list = new hitable *[n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float chooseMat = drand();
            vec3 center(a + 0.9 * drand(), 0.2, b + 0.9 * drand());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (chooseMat < 0.8)
                {
                    list[i++] = new movingSphere(
                        center,
                        center + vec3(0, 0.5 * drand(), 0),
                        0.0, 1.0, 0.2,
                        new lambertian(vec3(drand() * drand(), drand() * drand(), drand() * drand())));
                }
                else if (chooseMat < 0.95)
                {
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + drand()), 0.5 * (1 + drand()), 0.5 * (1 + drand())), 0.5 * drand()));
                }
                else
                {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    /*return new hitableList(list, i); */
    return new bvhNode(list, i, 0.0, 1.0);
}

vec3 color(const ray &r, hitable *world, int depth)
{
    hitRecord rec;
    if (world->hit(r, 0.001, FLT_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
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
    std::chrono::system_clock::time_point start, end;
    ofstream out("output.ppm");
    cout.rdbuf(out.rdbuf());

    int nx = 600;
    int ny = 400;
    int ns = 10;

    cout << "P3\n"
         << nx << " " << ny << "\n255\n";
    /*
    hitable *list[5];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
*/
    hitable *world = randomScene();
    /*
    vec3 lookFrom = vec3(11, 0.5, 8);
    vec3 lookAt = vec3(0, 0, -1);
    float distToFocus = (lookFrom - lookAt).length();
    float aperture = 2.0;*/

    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;

    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

    start = std::chrono::system_clock::now();
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

                col += color(r, world, 0);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    end = std::chrono::system_clock::now();

    double elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    printf("With BVH, %lf sec.\n", elapsed);

    return 0;
}