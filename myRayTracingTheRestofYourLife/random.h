#ifndef RANDOMH
#define RANDOMH
#include <math.h>
#include <stdlib.h>

#if _WIN32
#define drand48() ((float)rand() / RAND_MAX) 
#endif

void SetSeed(long long seed)
{
#if _WIN32
	srand(seed);
#else
	srand48(seed);
#endif
}
vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
	} while (dot(p, p) >= 1.0);
	return p;
}
//p落在xoy平面上一个原点在o的半径为1的圆中
vec3 random_in_unit_disk()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(drand48(), (float)rand() / RAND_MAX, 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}
//pdf(theta) = sin(theta)/2
//pdf(phi) = 1/2pi(均匀分布)
inline vec3 random_cosine_direction()
{
	float r1 = drand48();
	float r2 = drand48();
	float z = sqrt(1 - r2);
	float phi = 2 * 3.14159 * r1;
	float x = cos(phi) * 2 * sqrt(r2);
	float y = sin(phi) * 2 * sqrt(r2);
	return vec3(x, y, z);
}
#endif // !RANDOM
