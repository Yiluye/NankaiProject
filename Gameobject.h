#pragma once
#include "Defines.h"
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

class Gameobject {
protected:
	Coordinate pos;
	double width;
	double height;
public:
	Gameobject(double _width = 0.0, double _height = 0.0);
	virtual ~Gameobject() {};

	Gameobject* Setcoordinate(Coordinate& coord);
	Gameobject* Setx(double _x);
	Gameobject* Sety(double _y);
	Gameobject* Setwidth(double _width);
	Gameobject* Setheight(double _height);

	Coordinate Getcoordinate()const;
	double Getx()const;
	double Gety()const;
	double Getwidth()const;
	double Getheight()const;

	Rect Getborder()const;

	virtual void Draw() {};
	virtual void Update() {};
};


#endif // !_GAMEOBJECT_H
