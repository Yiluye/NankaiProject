#pragma once
#ifndef _BULLET_H
#define _BULLET_H

#include"Gameobject.h"
#include"Defines.h" 

enum class BulletShape {
    CIRCLE,     // 圆形
    RECT,       // 长方形（梭形）
};

enum BulletColor {
    BTRED=0,
    BTBLUE,
    BTYELLOW
};


class Bullet : public Gameobject {
private:
    Vector velocity;
    int radius;
    int width;               // 长方形宽度
    int height;              // 长方形高度
    double rotation;
    bool active;
    Camp camp;
    BulletShape shape;
    BulletColor color;
public:
    // 圆形构造（使用贴图）
    Bullet(double x, double y, double vx, double vy, int r, Camp c,BulletColor col);

    // 矩形构造（无旋转）
    Bullet(double x, double y, double vx, double vy, int w, int h, Camp c);

    // 矩形构造（带旋转）
    Bullet(double x, double y, double vx, double vy, int w, int h, double rot, Camp c);

    ~Bullet() = default;

    void Update();
    void Draw() override;

    bool IsActive() const;
    void Deactivate();
    Camp GetCamp() const;
    int GetRadius() const;
};

#endif // !_BULLET_H
