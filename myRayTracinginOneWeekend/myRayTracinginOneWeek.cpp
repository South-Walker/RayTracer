﻿#include <iostream>
#include <stdlib.h>
#include "sphere.h"
#include "float.h"
#include "hitable_list.h"
#include "hitable.h"
#include "camera.h"
#include "material.h"
#include <fstream>
#include <sstream>
float hit_sphere(const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		return -1.0;
	}
	else
	{
		return (-b - sqrt(discriminant)) / 2.0 / a;
	}
	return discriminant > 0;
}
vec3 color(const ray& r, hitable* world, int depth = 0)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 30 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
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
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}
hitable* random_scene()
{
	int n = 500;
	hitable** list = new hitable * [n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = drand48();
			vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
			if ((center - vec3(4, 0, 2.0)).length() > 0.9);
			{
				if (choose_mat < 0.8)//diffuse
				{
					list[i++] = new sphere(center, 0.2, 
						new lambertian(vec3(drand48() * drand48(),
							drand48() * drand48(), drand48() * drand48())));
				}
				else if (choose_mat < 0.95)//metal
				{
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()),
							0.5 * (1 + drand48())), 0));
				}
				else//glass
				{
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}

		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0));
	return new hitable_list(list, i);
}
int main(int argc, char** argv)
{
	int nx = 1920;
	int ny = 1080;
	int ns = 100;
	int worldseed = 96;
	SetSeed(worldseed);
	hitable* world = random_scene();
	int seed = 0;
	std::cin >> seed;
	SetSeed(seed);
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = (lookfrom - lookat).length();
	float aperture = 0.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);
	std::ofstream fout;
	std::ostringstream filepath;
	filepath << "./output/" << seed << ".ppm";
	fout.open(filepath.str());
	fout << "P3\n" << nx << " " << ny << "\n255" << std::endl;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(col[0] * 255.99f);
			int ig = int(col[1] * 255.99f);
			int ib = int(col[2] * 255.99f);
			fout << ir << " " << ig << " " << ib << std::endl;
		}
	}
	fout.close();
	return 0;
}
