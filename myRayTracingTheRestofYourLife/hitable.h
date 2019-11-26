#pragma once
#ifndef HITABLEH
#define HITABLEH
#include "ray.h"
#include "random.h"
#include "aabb.h"
#include "pdf.h"
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
	virtual float pdf_value(const vec3& o, const vec3& v) const 
	{
		return 0.0;
	}
	virtual vec3 random(const vec3& o) const
	{
		return vec3(1, 0, 0);
	}
};
class hitable_pdf : public pdf 
{
public:
	vec3 o;
	hitable* ptr;
	hitable_pdf(hitable* p, const vec3& origin) : ptr(p), o(origin)	{}
	virtual float value(const vec3& direction) const
	{
		return ptr->pdf_value(o, direction);
	}
	virtual vec3 generate() const
	{
		return ptr->random(o);
	}
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