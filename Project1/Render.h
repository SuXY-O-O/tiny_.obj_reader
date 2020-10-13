#pragma once

#include "Reader.h"
#include "TriangleList.h"

constexpr auto Z_MIN_DOUBLE = -99999;
constexpr auto COLOR_MIN = 55;

class Render
{
private:
	TriangleList* active_triangle_table;
	Reader* reader;

	vector<vector<double>* > *z_buffer;
	int z_all_min;
	int z_all_max;

	vector<vector<tuple<int, int, int> >* >* color_per_pixel;

public:
	Render(Reader* r);
	void compute_render();
	void depth_to_color();
	vector<vector<double>* >* get_z_picture();
	vector<vector<tuple<int, int, int> >* >* get_color_table();
	string color_to_string();
};

