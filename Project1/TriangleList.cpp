#include "TriangleList.h"

void TriangleList::intial()
{
    t_list.clear();
}

void TriangleList::add(Triangle* t)
{
    t_list.push_back(t);
}

list<Triangle*> TriangleList::get_all()
{
    return t_list;
}

void TriangleList::add_all(vector<Triangle*> vt)
{
    unsigned int i;
    for (i = 0; i < vt.size(); i++)
        t_list.push_back(vt[i]);
}

void TriangleList::remove_if_y(int y)
{
    for_remove = y;
    if (t_list.size() == 0)
        return;
    t_list.remove_if(Remove(y));
}
