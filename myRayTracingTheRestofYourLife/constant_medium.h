#ifndef CONSTANTMEDIUMH
#define CONSTANTMEDIUMH
#include "hitable.h"
#include "texture.h"
#include "material.h"
#include <float.h>
class constant_medium : public hitable
{
public:
	hitable* boundary;
	float density;
	material* phase_function;
	constant_medium(hitable* b, float d, texture* a) : boundary(b), density(d)
	{
		phase_function = new isotropic(a);
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		return boundary->bounding_box(t0, t1, box);
	}
};
bool constant_medium::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	bool db = false;
	hit_record rec1, rec2;
	if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1))
	{
		if (boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2))
		{
			//射线现在在内部
			if (rec1.t < t_min)
				rec1.t = t_min;
			if (rec2.t > t_max)
				rec2.t = t_max;
			//若发生，则都位于t_min内或都位于t_max外
			if (rec1.t >= rec2.t)
				return false;
			//不可能两个都是负的，否则rec2不会命中,
			//这个是一开始就在内部的情况
			if (rec1.t < 0)
				rec1.t = 0;
			float distance_inside_boundary = (rec2.t - rec1.t)
				* r.direction().length();
			//距离阈值
			float hit_distance = -(1 / density) * log(drand48());
			if (hit_distance < distance_inside_boundary)
			{
				rec.t = rec1.t + hit_distance / r.direction().length();
				rec.p = r.point_at_parameter(rec.t);
				//这个无所谓
				rec.normal = vec3(1, 0, 0);
				rec.mat_ptr = phase_function;
				return true;
			}
		}
	}
	return false;
}
#endif