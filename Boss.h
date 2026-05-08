#pragma once
#ifndef _BOSS_H
#define _BOSS_H

#include "Gameobject.h"
#include "Defines.h"
#include <vector>
#include <memory>

class Bullet;

class Boss : public Gameobject {
private:
    int hp;
    int maxHp;
    int radius;

    int totaltime;
    int shootTimer;
    int phase;             // 0=第一阶段, 1=第二阶段
    int Alive;          // 存活帧计数
    int Duration;    // 每种模式持续帧数
    int Pattern;     // 当前生效的弹幕模式
public:
    Boss(double x, double y);
    ~Boss() = default;

    void Update() override;
    void Draw() override;
    void TakeDamage(int damage);
    bool IsAlive() const { return hp > 0; }
    int GetHp() const { return hp; }
    int GetMaxHp() const { return maxHp; }
    int GetRadius() const { return radius; }

    // 发射弹幕（由游戏界面调用）
    void Shoot(std::vector<std::shared_ptr<Bullet>>& bullets, double playerX, double playerY);
};

#endif