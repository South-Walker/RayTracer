#pragma once
#ifndef HITABLEH
#define HITABLEH
#include "ray.h"
#include "random.h"
#include "aabb.h"
class material;
struct hit_record
{
	float t;
	float u, v;
	vec3 p;
	vec3 normal;
	material* mat_ptr;
};
class hitable
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};
class flip_normals : public hitable
{
public:
	hitable* ptr;
	flip_normals(hitable* p) :ptr(p) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
	{
		if (ptr->hit(r, t_min, t_max, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		return  ptr->bounding_box(t0, t1, box);
	}
};

#endif