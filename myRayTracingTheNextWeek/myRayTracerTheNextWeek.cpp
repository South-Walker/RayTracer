#include <iostream>
#include <stdlib.h>
#include "sphere.h"
#include "rectangle.h"
#include "box.h"
#include "hitable_list.h"
#include "hitable.h"
#include "translate.h"
#include "rotate.h"
#include "constant_medium.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include <float.h>
#include <fstream>
#include <sstream>
vec3 color(const ray& r, hitable* world, int depth = 0)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 30 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return emitted + attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return vec3(0.1, 0.1, 0.1);
		/*
		//环境光
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
		*/
	}
}
hitable_list* cornell_box()
{
	hitable** list = new hitable * [100];
	int i = 0;
	material* red = new lambertian(
		new constant_texture(vec3(0.65, 0.05, 0.05))
	);
	material* white = new lambertian(
		new constant_texture(vec3(0.73, 0.73, 0.73))
	);
	material* green = new lambertian(
		new constant_texture(vec3(0.12, 0.45, 0.15))
	);
	material* light = new diffuse_light(
		new constant_texture(vec3(15, 15, 15))
	);
	list[i++] = new flip_normals(new yz_rect(-50, 605, -50, 605, 805, green));
	list[i++] = new yz_rect(-50, 605, -50, 605, -250, red);
	list[i++] = new xz_rect(188, 368, 202, 357, 605, light);
	list[i++] = new flip_normals(new xz_rect(-250, 805, -50, 605, 605, white));
	list[i++] = new xz_rect(-250, 805, -50, 605, 0, white);
	list[i++] = new flip_normals(new xy_rect(-250, 805, -50, 605, 605, white));
	hitable* b1 = new translate(
		new rotate_y(
			new box(vec3(-60, 0, -25), vec3(140, 215, 190), white), -18
		)
		, vec3(130, 0, 65));
	hitable* b2 = new translate(
		new rotate_y(
			new box(vec3(-25, 0, -25), vec3(190, 380, 165), white), 15
		)
		, vec3(265, 0, 295));
	list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
	list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0, 0, 0)));
	return new hitable_list(list, i);
}
int main(int argc, char** argv)
{
	int nx = 1920;
	int ny = 1080;
	int ns = 1;
	int worldseed = 96;
	SetSeed(worldseed);
	hitable_list* hlworld = cornell_box();
	bvh_node world(hlworld->list, hlworld->list_size, 0.001, FLT_MAX);
	int seed = 0;
	std::cin >> seed;
	SetSeed(seed);
	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float dist_to_focus = 10;
	float aperture = 0.0;
	float fov = 40;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), fov, float(nx) / float(ny),
		aperture, dist_to_focus, 0.0, 1.0);
	std::ofstream fout;
	std::ostringstream filepath;

#if _WIN32
	filepath << "C:\\Users\\lenovo\\Desktop\\RayTracer\\myRayTracingTheNextWeek\\" << seed << ".ppm";
#else
	filepath << "./output/" << seed << ".ppm";
#endif
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
				col += color(r, &world);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(col[0] * 255.99f);
			int ig = int(col[1] * 255.99f);
			int ib = int(col[2] * 255.99f);
			int base = i * 500 * 3 + j * 3;
			fout << ir << " " << ig << " " << ib << std::endl;
		}
	}
	fout.close();
	return 0;
}
