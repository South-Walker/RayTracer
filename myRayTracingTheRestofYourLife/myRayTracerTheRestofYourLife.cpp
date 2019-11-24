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
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		float pdf;
		vec3 albedo;
		if (depth < 30 && rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf))
		{
			return emitted + albedo * rec.mat_ptr->scattering_pdf(r, rec, scattered)
				* color(scattered, world, depth + 1) / pdf;
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return vec3(0, 0, 0);
		return vec3(0.1, 0.1, 0.1);
		/*
		//环境光
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
		*/
	}
}
void cornell_box(hitable_list** scene, camera** cam, float aspect)
{
	int i = 0;
	hitable** list = new hitable * [8];
	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(
		new rotate_y(
			new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18)
		, vec3(130, 0, 65)
	);
	list[i++] = new translate(
		new rotate_y(
			new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15)
		, vec3(265, 0, 295)
	);
	*scene = new hitable_list(list, i);
	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;
	*cam = new camera(lookfrom, lookat, vec3(0, 1, 0),
		vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);
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
hitable_list* final()
{
	int nb = 20;
	hitable** list = new hitable * [30];
	hitable** boxlist = new hitable * [10000];
	hitable** boxlist2 = new hitable * [10000];
	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* ground = new lambertian(new constant_texture(vec3(0.48, 0.83, 0.53)));
	int b = 0;
	for (int i = 0; i < nb; i++)
	{
		for (int j = 0; j < nb; j++)
		{
			float w = 100;
			float x0 = -1000 + i * w;
			float z0 = -1000 + j * w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (drand48() + 0.01);
			float z1 = z0 + w;
			boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new bvh_node(boxlist, b, 0, 1);
	material* light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
	list[l++] = new xz_rect(123, 423, 147, 412, 504, light);
	vec3 center(400, 400, 200);
	list[l++] = new moving_sphere(center, center + vec3(30, 0, 0), 0, 1, 50,
		new lambertian(new constant_texture(vec3(0.7, 0.3, 0.1))));
	list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
	list[l++] = new sphere(vec3(0, 200, 145), 75, 
		new metal(vec3(0.8, 0.8, 0.9), 10.0)
	);
	hitable* boundary = new sphere(vec3(360, 175, 145), 70, new dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary, 0.2, new constant_texture(
		vec3(0.2, 0.4, 0.9)));
	boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
	list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(vec3(1.0, 1.0, 1.0)));
	int nx, ny;
	float* tex_data = load_image_texture_file("./earth.imgtexture", nx, ny);
	material* emat = new lambertian(new image_texture(tex_data, nx, ny));
	list[l++] = new sphere(vec3(400, 200, 400), 100, emat);
	texture* pertext = new noise_texture(2);
	list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(pertext));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxlist2[j] = new sphere(
			vec3(165 * drand48(), 165 * drand48(), 165 * drand48())
			, 10, white);
	}
	list[l++] = new translate(new rotate_y(new bvh_node(boxlist2, ns, 0.0, 1.0), 15),
		vec3(-100, 270, 395));
	return new hitable_list(list, l);
}
int main(int argc, char** argv)
{
	int nx = 1920;
	int ny = 1080;
	int ns = 1000;
	nx = 500;
	ny = 500;
	ns = 100;

	int worldseed = 96;
	SetSeed(worldseed);

	/*
	hitable_list* hlworld = final();
	bvh_node world(hlworld->list, hlworld->list_size, 0.001, FLT_MAX);
	vec3 lookfrom(478, 278, -600);
	vec3 lookat(278, 278, 0);
	float dist_to_focus = 10;
	float aperture = 0.0;
	float fov = 33;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), fov, float(nx) / float(ny),
		aperture, dist_to_focus, 0.0, 1.0);
	*/
	int seed = 0;
	std::cin >> seed;
	SetSeed(seed);

	hitable_list** phlworld = new hitable_list*;
	camera** ppcam = new camera*;
	cornell_box(phlworld, ppcam, float(nx) / float(ny));
	hitable_list* hlworld = *phlworld;
	camera cam = **ppcam;
	bvh_node world(hlworld->list, hlworld->list_size, 0.001, FLT_MAX);


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
