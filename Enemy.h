#pragma once
#ifndef _ENEMY_H
#define _ENEMY_H

#include "Gameobject.h"
#include "Defines.h"

enum  EnemyMove {
    LEFT,     // 圆形
    RIGHT       // 长方形（梭形）
};

class Enemy : public Gameobject {
private:
    int hp;
    int radius;
    int shootCooldown;
    static constexpr int SHOOT_DELAY = 45;  // 射击间隔（帧）
    int velocity;
    EnemyMove enemymove;
public:
    Enemy(double x, double y, EnemyMove move);
    ~Enemy() = default;

    void Update();
    void Draw() override;

    int GetHp() const;
    void TakeDamage(int damage);
    bool IsAlive() const;
    bool CanShoot() const;
    void ResetShootCooldown();
    int GetRadius() const;
};

#endif
