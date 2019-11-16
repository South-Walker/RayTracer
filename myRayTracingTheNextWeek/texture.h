#ifndef TEXTUREH
#define TEXTUREH
#include "noise.h"
#include <fstream>
class texture
{
public:
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};
class constant_texture :public texture
{
public:
	vec3 color;
	constant_texture() { }
	constant_texture(vec3 c) : color(c) { }
	virtual vec3 value(float u, float v, const vec3& p) const
	{
		return color;
	}
};
class checker_texture :public texture
{
public:
	texture* odd;
	texture* even;
	checker_texture() {}
	checker_texture(texture* t0, texture* t1) :even(t0), odd(t1) {}
	virtual vec3 value(float u, float v, const vec3& p) const
	{
		float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
};
class noise_texture : public texture
{
public:
	float scale;
	perlin noise;
	noise_texture(float sc) :scale(sc) {}
	virtual vec3 value(float u, float v, const vec3& p) const
	{
		return vec3(1, 1, 1) * 0.5 * 
			(1 + sin(scale * p.z() + 10 * noise.turb(p)));
	}
};
int* load_image_texture_file(std::string filepath, int& width, int& height)
{
	std::ifstream fin;
	fin.open(filepath);
	fin >> width >> height;
	int len = width * height * 3;
	int temp;
	int* rc = new int[width * height * 3];
	for (int i = 0; i < len; i++)
	{
		fin >> rc[i];
	}
	return rc;
}
class image_texture : public texture
{
public:
	int* data;
	int nx, ny;
	image_texture() {}
	image_texture(int* pixels, int A, int B)
		: data(pixels), nx(A), ny(B) {}
	virtual vec3 value(float u, float v, const vec3& p)const;
};
vec3 image_texture::value(float u, float v, const vec3& p) const
{
	int i = (u)* nx;
	int j = (1 - v) * ny - 0.001;
	if (i < 0)i = 0;
	if (j < 0)j = 0;
	if (i > nx - 1)i = nx - 1;
	if (j > ny - 1)j = ny - 1;
	float r = int(data[3 * i + 3 * nx * j]);
	float g = int(data[3 * i + 3 * nx * j + 1]);
	float b = int(data[3 * i + 3 * nx * j + 2]);
	return vec3(r, g, b);
}
#endif // !texture
