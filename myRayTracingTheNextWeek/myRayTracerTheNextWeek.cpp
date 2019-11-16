#include <iostream>
#include <stdlib.h>
#include "sphere.h"
#include "float.h"
#include "hitable_list.h"
#include "hitable.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include <fstream>
#include <sstream>
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
hitable_list* two_perlin_spheres()
{
	int nx, ny;
#if _WIN32
	std::string texpos = "./img5-01.imgtexture";
	int* tex_data = load_image_texture_file(texpos, nx, ny);
#else
	std::string texpos = "./img5-01.imgtexture";
	unsigned char* tex_data = load_image_texture_file(texpos, nx, ny);
#endif
	material* mat = new lambertian(new image_texture(tex_data, nx, ny));
	hitable** list = new hitable * [1];
	list[0] = new sphere(vec3(0, 0, 0), 1, mat);
	return new hitable_list(list, 1);
}
int main(int argc, char** argv)
{
	int nx = 192;
	int ny = 108;
	int ns = 1;
	int worldseed = 96;
	SetSeed(worldseed);
	hitable_list* hlworld = two_perlin_spheres();
	bvh_node world(hlworld->list, hlworld->list_size, 0.001, FLT_MAX);
	int seed = 0;
	std::cin >> seed;
	SetSeed(seed);
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = (lookfrom - lookat).length();
	float aperture = 0.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny),
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
