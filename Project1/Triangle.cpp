#include "Triangle.h"
#include "Reader.h"
#include <cmath>

void Triangle::compute_normal()
{
	// check if z is static
	if (v1->z == v2->z && v2->z == v3->z)
	{
		is_static_z = true;
		return;
	}
	is_static_z = false;
	// (a1, a2, a3) = v2 - v1
	double a1 = v2->x - v1->x;
	double a2 = v2->y - v1->y;
	double a3 = v2->z - v2->z;
	// (b1, b2, b3) = v3 - v1
	double b1 = v3->x - v1->x;
	double b2 = v3->y - v3->y;
	double b3 = v3->z - v1->z;
	// Compute normal vector (n1, n2, n3)
	double n1 = a2 * b3 - a3 * b2;
	double n2 = a3 * b1 - a1 * b3;
	double n3 = a1 * b2 - a2 * b1;
	n = new Vertex(n1, n2, n3);
	// Compute k_delta
	k_delta_x = -n1 / n3;
	k_delta_y = -n2 / n3;
	return;
}

void Triangle::compute_x_list()
{
	//printf("(%04.4lf, %04.4lf, %04.4lf)\t(%04.4lf, %04.4lf, %04.4lf)\t(%04.4lf, %04.4lf, %04.4lf)\n",
	//	v1->x, v1->y, v1->z, v2->x, v2->y, v2->z, v3->x, v3->y, v3->z);
	// Flags for horizontal lines
	bool check_horizon[3] = { false, false, false };
	// 1/gradient of line (v1, v2) on XOY
	double d0 = 0;
	if (std::abs (v1->y - v2->y) < EXP)
		check_horizon[0] = true;
	else
		d0 = (v1->x - v2->x) / (v1->y - v2->y);
	// 1/gradient of line (v2, v3) on XOY
	double d1 = 0;
	if (std::abs(v3->y - v2->y) < EXP)
		check_horizon[1] = true;
	else
		d1 = (v3->x - v2->x) / (v3->y - v2->y);
	// 1/gradient of line (v1, v3) on XOY
	double d2 = 0;
	if (std::abs(v1->y - v3->y) < EXP)
		check_horizon[2] = true;
	else
		d2 = (v1->x - v3->x) / (v1->y - v3->y);
	//printf("k0: %lf\t k1: %lf\t k2: %lf\n", d0, d1, d2);
	// Compute
	int min = begin_y;
	int max = (int)ceil(y_max);
	int i_y;
	for (i_y = begin_y; i_y < max; i_y++)
	{
		int tmp[3] = { 0 };
		int i_tmp = 0;
		// Get
		if ((!check_horizon[0]) &&
			((i_y <= v1->y && i_y >= v2->y) || (i_y >= v1->y && i_y <= v2->y)))
		{
			tmp[i_tmp] = (int)(d0 * (i_y - v2->y) + v2->x);
			i_tmp++;
		}
		if ((!check_horizon[1]) && 
			((i_y <= v3->y && i_y >= v2->y) || (i_y >= v3->y && i_y <= v2->y)))
		{
			tmp[i_tmp] = (int)(d1 * (i_y - v2->y) + v2->x);
			i_tmp++;
		}
		if ((!check_horizon[2]) && 
			((i_y <= v1->y && i_y >= v3->y) || (i_y >= v1->y && i_y <= v3->y)))
		{
			tmp[i_tmp] = (int)(d2 * (i_y - v1->y) + v1->x);
			i_tmp++;
		}
		//printf("x_min to x_max: %d, %d\n", tmp[0], tmp[1]);
		// Save
		if (tmp[0] > tmp[1])
		{
			x_min.push_back(tmp[1]);
			x_max.push_back(tmp[0]);
		}
		else
		{
			x_min.push_back(tmp[0]);
			x_max.push_back(tmp[1]);
		}
	}
}

Triangle::Triangle(Vertex* a1, Vertex* a2, Vertex* a3)
	: v1(a1), v2(a2), v3(a3)
{
	// Get y_max and y_min
	y_max = a1->y > a2->y ? a1->y : a2->y;
	if (y_max < a3->y)
		y_max = a3->y;
	y_min = a1->y < a2->y ? a1->y : a2->y;
	if (y_min > a3->y)
		y_min = a3->y;
	// Get z_max and z_min
	z_max = a1->z > a2->z ? a1->z : a2->z;
	if (z_max < a3->z)
		z_max = a3->z;
	z_min = a1->z < a2->z ? a1->z : a2->z;
	if (z_min > a3->z)
		z_min = a3->z;
	// Get x_y_min and z_y_min
	if (y_min == a1->y)
	{
		x_y_min = a1->x;
		z_y_min = a1->z;
	}
	else if (y_min == a2->y)
	{
		x_y_min = a2->x;
		z_y_min = a2->z;
	}
	else if (y_min == a3->y)
	{
		x_y_min = a3->x;
		z_y_min = a3->z;
	}
	// Set begin_y;
	begin_y = (int)ceil(y_min);
	// Get k_delta_x and k_delta_y
	compute_normal();
	// Get x_min and x_max
	compute_x_list();
}

double Triangle::compute_z_value(double x, double y)
{
	if (is_static_z)
		return v1->z;
	double z_tmp = -(n->x * (x - v1->x) + n->y * (y - v1->y)) / n->z + v1->z;
	if (isinf(z_tmp))
	{
		z_tmp = z_y_min;
	}
	else if (z_tmp > z_max)
	{
		//printf("(%04.4lf, %04.4lf, %04.4lf)\t(%04.4lf, %04.4lf, %04.4lf)\t(%04.4lf, %04.4lf, %04.4lf)\n",
		//	v1->x, v1->y, v1->z, v2->x, v2->y, v2->z, v3->x, v3->y, v3->z);
		//printf("max: %lf\t %lf\n", z_max, z_tmp);
		z_tmp = z_max;
	}
	else if (z_tmp < z_min)
	{
		//printf("(%04.4lf, %04.4lf, %04.4lf)\t(%04.4lf, %04.4lf, %04.4lf)\t(%04.4lf, %04.4lf, %04.4lf)\n",
		//	v1->x, v1->y, v1->z, v2->x, v2->y, v2->z, v3->x, v3->y, v3->z);
		//printf("min: %lf\t %lf\n", z_min, z_tmp);
		z_tmp = z_min;
	}
	return z_tmp;
}

pair<int, int> Triangle::get_x_range(int y)
{
	if (y >= y_max || y <= y_min)
		return pair<int, int>(0, 0);
	return pair<int, int>(x_min[y - begin_y], x_max[y - begin_y]);
}
