#include "Render.h"

Render::Render(Reader* r):reader(r)
{
	active_triangle_table = new TriangleList;
	active_triangle_table->intial();
	z_all_min = 0;
	z_all_max = 0;
	z_buffer = new vector<vector<double>* >();
	color_per_pixel = new vector<vector<tuple<int, int, int> >* >();
	int i, j;
	for (i = 0; i < r->get_height(); i++)
	{
		vector<double>* tmp = new vector<double>();
		vector<tuple<int, int, int> >* tmp_c = new vector<tuple<int, int, int> >();
		for (j = 0; j < r->get_width(); j++)
		{
			tmp->push_back(Z_MIN_DOUBLE);
			tmp_c->push_back(tuple<int, int, int>(0, 0, 0));
		}
		z_buffer->push_back(tmp);
		color_per_pixel->push_back(tmp_c);
	}
}

void Render::compute_render()
{
	int iy;
	int min_y = reader->get_y_all_min();
	int max_y = reader->get_y_all_max();
	if (min_y < 0)
		min_y = 0;
	if (max_y > reader->get_height())
		max_y = reader->get_height();
	int min_x = 0;
	int max_x = reader->get_width();
	for (iy = min_y; iy < max_y; iy++)
	{
		active_triangle_table->remove_if_y(iy);
		if (reader->have_triangle_of_y(iy))
			active_triangle_table->add_all(reader->get_triangle_by_y(iy));
		list<Triangle*>::iterator ite;
		list<Triangle*> att_now = active_triangle_table->get_all();
		for (ite = att_now.begin(); ite != att_now.end(); ite++)
		{
			pair<int, int> x_range = (*ite)->get_x_range(iy);
			int ix;
			if (x_range.first < min_x)
				x_range.first = min_x;
			if (x_range.second > max_x)
				x_range.second = max_x;
			for (ix = x_range.first; ix < x_range.second; ix++)
			{
				double z_value = (*ite)->compute_z_value(ix, iy);
				//printf("%lf\t", z_value);
				if (z_value > (*(*z_buffer)[iy])[ix])
				{
					(*(*z_buffer)[iy])[ix] = z_value;
				}	
			}
		}
	}
	//printf("z_min: %lf, z_max: %lf\n", z_all_min, z_all_max);
}

void Render::depth_to_color()
{
	int ix, iy;
	bool flag = true;
	for (iy = 0; iy < reader->get_height(); iy++)
	{
		for (ix = 0; ix < reader->get_width(); ix++)
		{
			if ((*(*z_buffer)[iy])[ix] != Z_MIN_DOUBLE)
			{
				if (flag)
				{
					flag = false;
					z_all_max = z_all_min = (*(*z_buffer)[iy])[ix];
					continue;
				}
				if ((*(*z_buffer)[iy])[ix] > z_all_max)
				{
					z_all_max = (int)(*(*z_buffer)[iy])[ix];
					//printf("max update: %lf\n", (*(*z_buffer)[iy])[ix]);
				}
				if ((*(*z_buffer)[iy])[ix] < z_all_min)
				{
					z_all_min = (int)(*(*z_buffer)[iy])[ix];
					//printf("min update: %lf\n", (*(*z_buffer)[iy])[ix]);
				}
			}
		}
	}
	int z_range = z_all_max - z_all_min;
	int color_range = 255 - COLOR_MIN;
	for (iy = 0; iy < reader->get_height(); iy++)
	{
		for (ix = 0; ix < reader->get_width(); ix++)
		{		
			if ((*(*z_buffer)[iy])[ix] != Z_MIN_DOUBLE)
			{
				//printf("%lf\t", (*(*z_buffer)[iy])[ix]);
				int c = (int)ceil
					(((*(*z_buffer)[iy])[ix] - z_all_min) / z_range * color_range)
					+ COLOR_MIN;
				if (c > 255)
					c = 255;
				//printf("(x: %05d, y: %05d, c: %03d)\n", ix, iy, c);
				(*(*color_per_pixel)[iy])[ix] = tuple<int, int, int>(c, c, c);
			}
		}
	}
	//printf("z_min: %d, z_range: %d\n", z_all_min, z_range);
}

vector<vector<double>*>* Render::get_z_picture()
{
	return z_buffer;
}

vector<vector<tuple<int, int, int>>*>* Render::get_color_table()
{
	return color_per_pixel;
}

string Render::color_to_string()
{
	string for_return;
	for_return.clear();
	int ix, iy;
	unsigned char t;
	for (iy = 0; iy < reader->get_height(); iy++)
	{
		for (ix = 0; ix < reader->get_width(); ix++)
		{
			tuple<int, int, int> now = (*(*color_per_pixel)[iy])[ix];
			int r, g, b;
			tie(r, g, b) = now;
			//printf("(%d %d %d) ", r, g, b);
			t = (unsigned char)r;
			for_return += t;
			t = (unsigned char)g;
			for_return += t;
			t = (unsigned char)b;
			for_return += t;
		}
	}
	return for_return;
}
