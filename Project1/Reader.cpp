#include "Reader.h"

using namespace std;

void Reader::add_to_map(Triangle* t)
{
	int y_min_now = (int)ceil(t->get_y_min());
	map<int, vector<Triangle*>>::iterator ite;
	ite = y_min_to_list.find(y_min_now);
	if (ite != y_min_to_list.end())
	{
		ite->second.push_back(t);
	}
	else
	{
		vector<Triangle*> new_vector;
		new_vector.clear();
		new_vector.push_back(t);
		y_min_to_list.insert(
			pair<int, vector<Triangle*>>(y_min_now, new_vector));
	}
	return;
}

void Reader::read_in_vertex()
{
	double x[3];
	char now;
	string tmp;
	int i;
	for (i = 0; i < 3; i++)
	{
		now = input_file->get();//read' '
		tmp.clear();
		while (input_file->peek() != ' ' 
			&& input_file->peek() != '\n' && input_file->peek() != '\0')
		{
			tmp += input_file->get();
		}
		x[i] = stod(tmp);//read a double
	}
	//x[0] = x[0] * zoom_rate + screen_width / 2;
	x[0] = x[0] * zoom_rate;
	//x[1] = x[1] * zoom_rate + screen_height / 2;
	x[1] = x[1] * zoom_rate;
	x[2] = x[2] * zoom_rate;
	Vertex* new_one = new Vertex(x[0], x[1], x[2]);
	//update y_all min and max
	if (total_vertex == 0)
	{
		y_all_min = y_all_max = x[1];
	}
	else
	{
		if (y_all_min > x[1])
			y_all_min = x[1];
		if (y_all_max < x[1])
			y_all_max = x[1];
	}
	total_vertex++;
	printf("%d:\t %lf %lf %lf\n", total_vertex, x[0], x[1], x[2]);
	vertexs.push_back(new_one);
}

void Reader::read_in_triangle()
{
	vector<int> contains;
	char now;
	string tmp;
	while (input_file->peek() == ' ')
	{
		now = input_file->get();//read' '
		tmp.clear();
		while (input_file->peek() != ' ' && input_file->peek() != '/')
		{
			tmp += input_file->get();
		}
		int x = stoi(tmp);//read the vertex number
		contains.push_back(x);
		while (input_file->peek() != ' ' 
			&& input_file->peek() != '\n' && input_file->peek() != '\0')
		{
			now = input_file->get();//skip vt and vn
		}
	}
	unsigned int i;
	for (i = 0; i < contains.size(); i++)
		printf("%d ", contains[i]);
	printf("\n");
	for (i = 1; i < contains.size() - 1; i++)//seperate polygon and save by triangle
	{
		Triangle* t = new Triangle
		(vertexs[contains[0]], vertexs[contains[i]], vertexs[contains[i + 1]]);
		add_to_map(t);
	}
}

Reader::Reader(string file_name, double rate, int w, int h)
	: zoom_rate(rate), screen_width(w), screen_height(h)
{
	input_file = new ifstream(file_name);
	vertexs.clear();
	vertexs.push_back(new Vertex(0, 0, 0));//id begin with 1
	y_min_to_list.clear();
	total_vertex = 0;
	y_all_max = 0;
	y_all_min = 0;
}

bool Reader::begin_read_file()
{
	//TODO
	char now;
	if (!input_file->is_open())
	{
		printf("Wrong:Unable to open the input file\n");
		return false;
	}
	int debug = 0;
	while (!input_file->eof())
	{
		now = input_file->get();
		if (now == 'v')
		{
			now = input_file->peek();
			if (now == ' ')					//is a vertex
			{
				debug++;
				printf("v count:%d\n", debug);
				read_in_vertex();
			}
		}
		else if (now == 'f')				//is a face
		{
			read_in_triangle();
		}
	}
	return true;
}

bool Reader::have_triangle_of_y(int y)
{
	map<int, vector<Triangle*>>::iterator ite;
	ite = y_min_to_list.find(y);
	if (ite != y_min_to_list.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

vector<Triangle*> Reader::get_triangle_by_y(int y)
{
	// Need pre_check, using have_triangle_of_y
	map<int, vector<Triangle*>>::iterator ite;
	ite = y_min_to_list.find(y);
	return ite->second;
}

int Reader::get_y_all_max()
{
	return (int)ceil(y_all_max);
}

int Reader::get_y_all_min()
{
	return (int)ceil(y_all_min);
}

int Reader::get_width()
{
	return screen_width;
}

int Reader::get_height()
{
	return screen_height;
}
