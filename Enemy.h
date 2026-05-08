#pragma once
#ifndef _ENEMY_H
#define _ENEMY_H

#include "Gameobject.h"
#include "Defines.h"

class Enemy : public Gameobject {
private:
    int hp;
    int radius;
    int shootCooldown;
    static constexpr int SHOOT_DELAY = 45;  // Éä»÷¼ä¸ô£¨Ö¡£©

public:
    Enemy(double x, double y);
    ~Enemy() = default;

    void Update();  // ¾²Ö¹£ºÖ»¸üÐÂÀäÈ´
    void Draw() override;

    int GetHp() const;
    void TakeDamage(int damage);
    bool IsAlive() const;
    bool CanShoot() const;
    void ResetShootCooldown();
    int GetRadius() const;
};

#endif
