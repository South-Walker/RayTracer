#ifndef NOISEH
#define NOISEH
#include "vec3.h"
#include "random.h"
#include "math.h"
inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w)
{
	//����Ken Perlin���˵������޸�
	float uu = u * u * u * (6 * u * u - 15 * u + 10);
	float vv = v * v * v * (6 * v * v - 15 * v + 10);
	float ww = w * w * w * (6 * w * w - 15 * w + 10);
	float accum = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				vec3 weight_v(u - i, v - j, w - k);
				//�ӺŶ�Ӧ������ֻ��һ�Ϊ��
				accum += (i * uu + (1 - i) * (1 - uu)) *
					(j * vv + (1 - j) * (1 - vv)) *
					(k * ww + (1 - k) * (1 - ww)) *
					dot(c[i][j][k], weight_v);
			}
		}
	}
	return accum;
}
class perlin
{
public:
	static vec3* ranvec;
	static int* perm_x;
	static int* perm_y;
	static int* perm_z;
	float noise(const vec3& p) const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());
		vec3 c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					//������������������õ�����������㷨
					c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^
					perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
		return perlin_interp(c, u, v, w);
	}
	float turb(const vec3& p, int depth = 7) const
	{
		float accum = 0;
		vec3 temp_p = p;
		float weight = 1.0;
		for (int i = 0; i < depth; i++)
		{
			accum += weight * noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}
		return fabs(accum);
	}
};
//����ÿ���ǵ��ݶ�Gard��
//�����Ĳ�ͬ�������ݶȵ�ÿ��������[-1,1]������ֵ
static vec3* perlin_generate()
{
	vec3* p = new vec3[256];
	for (int i = 0; i < 256; i++)
		p[i] = unit_vector(vec3(-1 + 2 * drand48(), -1 + 2 * drand48(),
			-1 + 2 * drand48()));
	return p;
}
//ϴ���㷨����n�����������
void permute(int* p, int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int target = int(drand48() * (i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
	return;
}
static int* perlin_generate_perm()
{
	int* p = new int[256];
	for (int i = 0; i < 256; i++)
		p[i] = i;
	permute(p, 256);
	return p;
}
//0-1
vec3* perlin::ranvec = perlin_generate();
//0-255
int* perlin::perm_x = perlin_generate_perm();
int* perlin::perm_y = perlin_generate_perm();
int* perlin::perm_z = perlin_generate_perm();
#endif