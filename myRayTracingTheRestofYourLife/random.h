#ifndef RANDOMH
#define RANDOMH
#include <math.h>
#include <stdlib.h>


//#define RELEASE

#ifdef RELEASE
#define drand() ((float)rand() / RAND_MAX)
#else
#define drand() (debug_drand())
#endif
static int SEED = 96;

const int RMAX = 32768;
const float FRMAX = (float)RMAX;
float debug_drand()
{
	int r = ((SEED = SEED * 214013L + 2531011L) >> 16) & 0x7fff;
	return (float)(r) / FRMAX;
}
inline void SetSeed(int seed)
{
#ifdef RELEASE
	srand(seed);
#else
	SEED = seed;
#endif
}
vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(drand(), drand(), drand()) - vec3(1, 1, 1);
	} while (dot(p, p) >= 1.0);
	return p;
}
//p落在xoy平面上一个原点在o的半径为1的圆中
vec3 random_in_unit_disk()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(drand(), drand(), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}
//pdf(theta) = sin(theta)/2
//pdf(phi) = 1/2pi(均匀分布)
inline vec3 random_cosine_direction()
{
	float r1 = drand();
	float r2 = drand();
	float z = sqrt(1 - r2);
	float phi = 2 * 3.14159 * r1;
	float x = cos(phi) * 2 * sqrt(r2);
	float y = sin(phi) * 2 * sqrt(r2);
	return vec3(x, y, z);
}
#endif // !RANDOM
