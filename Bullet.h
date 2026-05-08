#pragma once
#ifndef _BULLET_H
#define _BULLET_H

#include"Gameobject.h"

class Bullet :public Gameobject {
private:
    Vector velocity;
    int radius;
    bool active;
    Camp camp;
public:
    Bullet(double x, double y, double vx, double vy, int r, Camp c);
    ~Bullet() = default;

    void Update();
    void Draw() override;

    bool IsActive() const;
    void Deactivate();
    Camp GetCamp() const;
    int GetRadius() const;
};
#endif // !_BULLET_H
