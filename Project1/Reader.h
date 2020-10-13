#pragma once

#include "Triangle.h"
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <string>

using namespace std;

class Reader
{
private:
	ifstream* input_file;
	vector<Vertex*> vertexs;

	int total_vertex;
	double y_all_min;
	double y_all_max;

	map<int, vector<Triangle*>> y_min_to_list;
	void add_to_map(Triangle* t);

	double zoom_rate;
	int screen_width;
	int screen_height;

	void read_in_vertex();
	void read_in_triangle();

public:
	Reader(string file_name, double rate, int w, int h);
	bool begin_read_file();
	bool have_triangle_of_y(int y);
	vector<Triangle*> get_triangle_by_y(int y);
	int get_y_all_max();
	int get_y_all_min();
	int get_width();
	int get_height();
};

