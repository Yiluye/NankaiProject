#pragma once
#ifndef _BULLET_H
#define _BULLET_H

#include"Gameobject.h"

enum class BulletShape {
    CIRCLE,     // 圆形
    RECT,       // 长方形（梭形）
    SPRITE
};


class Bullet :public Gameobject {
private:
    Vector velocity;
    int radius;
    int width;               // 长方形宽度（圆形时等于 2*radius）
    int height;              // 长方形高度（圆形时等于 2*radius）
    double rotation;

    bool active;
    Camp camp;
    BulletShape shape;       // 形状
    int srcX, srcY;
public:
    Bullet(double x, double y, double vx, double vy, int r, Camp c);
    Bullet(double x, double y, double vx, double vy, int w, int h, Camp c);
    Bullet(double x, double y, double vx, double vy, int w, int h, double rot, Camp c);
    Bullet(double x, double y, double vx, double vy, int r, Camp c, int srcX, int srcY);

    ~Bullet() = default;

    void Update();
    void Draw() override;

    bool IsActive() const;
    void Deactivate();
    Camp GetCamp() const;
    int GetRadius() const;
};
#endif // !_BULLET_H
