#ifndef PDFH
#define PDFH
#include "vec3.h"
#include "onb.h"
#include "random.h"
class pdf
{
public:
	virtual float value(const vec3& direction) const = 0;
	virtual vec3 generate() const = 0;
};
class zero_pdf : public pdf
{
public:
	zero_pdf() {}
	virtual float value(const vec3& direction) const
	{
		return 0;
	}
	virtual vec3 generate() const
	{
		return vec3(0, 0, 0);
	}
};
class cosine_pdf : public pdf
{
public:
	onb uvw;
	cosine_pdf(const vec3& w)
	{
		uvw.build_from_w(w);
	}
	virtual float value(const vec3& direction) const
	{
		float cosine = dot(unit_vector(direction), uvw.w());
		if (cosine > 0)
			return cosine / 3.14159;
		else
			return 0;
	}
	virtual vec3 generate() const
	{
		return uvw.local(random_cosine_direction());
	}
};
class mixture_pdf : public pdf
{
public:
	pdf* p[2];
	mixture_pdf(pdf* p0, pdf* p1) 
	{
		p[0] = p0; p[1] = p1;
	}
	virtual float value(const vec3& direction) const
	{
		return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
	}
	virtual vec3 generate() const
	{
		if (drand() < 0.5)
			return p[0]->generate();
		else
			return p[1]->generate();
	}
};
#endif