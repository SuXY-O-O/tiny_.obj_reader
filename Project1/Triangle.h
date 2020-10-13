#pragma once

#include "Vertex.h"
#include <vector>

using namespace std;

const double EXP = 0.000001;

class Triangle
{
private:
	// The three vertexs of the triangle
	Vertex* v1;
	Vertex* v2;
	Vertex* v3;
	// Max and Min of vertex.y
	double y_max;
	double y_min;
	// The x and z of the vertex containing the y_min
	double x_y_min;
	double z_y_min;
	// The range of x inside the triangle when y = index
	int begin_y;
	vector<int> x_min;
	vector<int> x_max;
	// Parameters for computing delta z
	bool is_static_z;
	double k_delta_x;
	double k_delta_y;
	// Compute normal vector and initial k_delta
	void compute_normal();
	// Initial x_min and x_max
	void compute_x_list();
public:
	// Initial
	Triangle(Vertex* a1, Vertex* a2, Vertex* a3);
	// Compute z at (x,y) on the triangle
	double compute_z_value(double x, double y);
	// Get x_min and x_max at y
	pair<int, int> get_x_range(int y);
	// Get y_max
	double get_y_max()
	{
		return y_max;
	};
	// Get y_min
	double get_y_min()
	{
		return y_min;
	};
};

