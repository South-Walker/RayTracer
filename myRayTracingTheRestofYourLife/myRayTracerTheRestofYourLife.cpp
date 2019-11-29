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
#include "pdf.h"
#include <float.h>
#include <fstream>
#include <sstream>
vec3 color(const ray& r, hitable* world, hitable* light_shape, int depth = 0)
{
	hit_record hrec;
	if (world->hit(r, 0.001, FLT_MAX, hrec))
	{
		scatter_record srec;
		vec3 emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
		ray scattered;
		float pdf_val;
		if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec))
		{
			if (srec.is_specular)
			{
				return srec.attenuation * color(srec.specular_ray, world, light_shape, depth + 1);
			}
			hitable_pdf plight(light_shape, hrec.p);
			mixture_pdf p(&plight, srec.pdf_ptr);
			scattered = ray(hrec.p, p.generate(), r.time());
			pdf_val = p.value(scattered.direction());
			delete srec.pdf_ptr;

			if (pdf_val < 0.00001)
			{
				return emitted;
			}
			return emitted + srec.attenuation * hrec.mat_ptr->scattering_pdf(r, hrec, scattered)
				* color(scattered, world, light_shape, depth + 1) / pdf_val;
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
	list[i++] = new flip_normals(new xz_rect(213, 343, 227, 332, 554, light));
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	//list[i++] = new translate(
		//new rotate_y(
			//new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18)
		//, vec3(130, 0, 65)
	//);
	material* aluminum = new dielectric(1.5);
	list[i++] = new translate(
		new rotate_y(
			new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15)
		, vec3(265, 0, 295)
	);
	list[i++] = new sphere(vec3(190, 90, 190), 90, aluminum);
	*scene = new hitable_list(list, i);
	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;
	*cam = new camera(lookfrom, lookat, vec3(0, 1, 0),
		vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);
}

int main(int argc, char** argv)
{
	int nx = 1920;
	int ny = 1080;
	int ns = 1000;
	nx = 500;
	ny = 500;
	ns = 10;

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

	hitable* plight = new xz_rect(213, 343, 227, 332, 554, 0);
	hitable* psphere = new sphere(vec3(190, 90, 190), 90, 0);
	hitable* a[2];
	a[0] = plight;
	a[1] = psphere;
	hitable_list* hlist = new hitable_list(a, 2);

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
				float u = float(i + drand()) / float(nx);
				float v = float(j + drand()) / float(ny);
				ray r = cam.get_ray(u, v);

				col += color(r, &world, hlist);
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
