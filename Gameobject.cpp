#include"Gameobject.h"
Gameobject::Gameobject(double _width, double _height)
	:width(_width), height(_height)
{

}
Gameobject* Gameobject::Setcoordinate(Coordinate& coord)
{
	pos = coord;
	return this;
}
Gameobject* Gameobject::Setx(double _x)
{
	pos.x = _x;
	return this;
}
Gameobject* Gameobject::Sety(double _y)
{
	pos.y = _y;
	return this;
}
Gameobject* Gameobject::Setwidth(double _width)
{
	width = _width;
	return this;
}
Gameobject* Gameobject::Setheight(double _height)
{
	height = _height;
	return this;
}
Coordinate Gameobject::Getcoordinate()const
{
	return pos;
}

double Gameobject::Getx()const
{
	return pos.x;
}
double Gameobject::Gety()const
{
	return pos.y;
}
double Gameobject::Getwidth()const
{
	return width;
}
double Gameobject::Getheight()const
{
	return height;
}

Rect Gameobject::Getborder()const
{
	return { pos.x,pos.y,width,height };
}
