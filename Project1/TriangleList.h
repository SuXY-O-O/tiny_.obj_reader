#pragma once

#include "Triangle.h"
#include <list>
#include <vector>

using namespace std;

class TriangleList
{
private:
	list<Triangle*> t_list;
	int for_remove = 0;
public:
	void intial();
	void add(Triangle* t);
	list<Triangle*> get_all();
	void add_all(vector<Triangle*> vt);
	void remove_if_y(int y);
};

class Remove
{
private:
	int y_r;
public:
	bool operator() (Triangle* &obj)
	{
		return obj->get_y_max() <= y_r;
	};
	Remove(int y) :y_r(y) {};
};