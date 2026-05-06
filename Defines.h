#pragma once
#pragma once

#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <easyx.h>

struct Vector
{
    double x;
    double y;

    Vector() : x(0.0), y(0.0)
    {
    }

    Vector(double _x, double _y) : x(_x), y(_y)
    {
    }

    bool operator==(const Vector& obj) const
    {
        return x == obj.x && y == obj.y;
    }

    bool operator!=(const Vector& obj) const
    {
        return !(*this == obj);
    }

    Vector operator+(const Vector& obj) const
    {
        return { x + obj.x, y + obj.y };
    }

    Vector operator-(const Vector& obj) const
    {
        return { x + obj.x, y + obj.y };
    }

    Vector operator-() const
    {
        return { -x, -y };
    }

    friend Vector operator*(const Vector& lhs, double rhs)
    {
        return { lhs.x * rhs, lhs.y * rhs };
    }

    friend Vector operator*(double lhs, const Vector& rhs)
    {
        return rhs * lhs;
    }

    friend Vector operator/(const Vector& lhs, double rhs)
    {
        return { lhs.x / rhs, lhs.y / rhs };
    }

    friend Vector operator/(double lhs, const Vector& rhs)
    {
        return rhs / lhs;
    }
};

using Coordinate = Vector;


class Rect :public Vector {
public:
    double height;
    double width;
    Rect() :Vector(0, 0), height(0), width(0) {}

    Rect(double _x, double _y, double _width, double _height) :Vector(_x, _y), width(_width), height(_height) {}

    Coordinate Getcenter()const {
        return Coordinate(x + width * 0.5, y + height * 0.5);
    }

    Coordinate Intersect(const Rect& rect) const {
        Coordinate overlap;
        if (this->x < rect.x)
        {
            overlap.x = this->x + this->width - rect.x;
        }
        else
        {
            overlap.x = rect.x + rect.width - this->x;
        }
        if (this->y < rect.y)
        {
            overlap.y = this->y + this->height - rect.y;
        }
        else
        {
            overlap.y = rect.y + rect.height - this->y;
        }
        return overlap;
    }

    bool Contains(const Coordinate& coord) const {
        return ((coord.x > x && coord.x < x + width) &&
            (coord.y > y && coord.y < y + height));
    }

    RECT Toeasyrect() const
    {
        return {
            static_cast<LONG>(x), static_cast<LONG>(y), static_cast<LONG>(x + width), static_cast<LONG>(y + height)
        };
    }
};

// ×Óµ¯ÕóÓª
enum class Camp {
    PLAYER = 0,
    ENEMY = 1
};

#endif;
