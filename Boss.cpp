#include "Boss.h"
#include "Bullet.h"
#include "Global.h"
#include <cmath>

Boss::Boss(double x, double y)
    : Gameobject(60, 60), hp(300), maxHp(300), radius(30), shootTimer(0), Pattern(0), phase(0) {
    pos.x = x;
    pos.y = y;
    Alive = 0;
    Duration = 900;
    Pattern = 2;
    totaltime = 0;
}

void Boss::Update() {
    if (shootTimer > 0) shootTimer--;

    Alive++;
    if (Alive > Duration)
    {
        Pattern = (Pattern + 1) % 4;
        Alive = 0;
    }
    totaltime++;

    // 根据血量改变阶段和弹幕模式
    //if (hp < maxHp * 0.5 && phase == 0) {
    //    phase = 1;
    //    shootPattern = 1;   // 第一阶段用散射，第二阶段用自机狙
    //}
}

void Boss::Draw() {
    setfillcolor(RGB(200, 0, 200));  // 紫色
    fillcircle((int)pos.x, (int)pos.y, radius);
    // 血条
    setfillcolor(RED);
    fillrectangle((int)(pos.x - 60), (int)(pos.y - radius - 10),
        (int)(pos.x - 60 + 120 * hp / maxHp), (int)(pos.y - radius - 5));
}


void Boss::Shoot(std::vector<std::shared_ptr<Bullet>>& bullets, double playerX, double playerY) {
    if (shootTimer > 0) return;

    switch (Pattern) {
    case 0: // 八角散射
        for (int i = -4; i <= 4; ++i) {
            double angle = i * 0.78539816;
            double vx = sin(angle) * 4;
            double vy = cos(angle) * 4;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y , vx, vy, 5, Camp::ENEMY));
        }
        shootTimer = 10;
        break;
    case 1: // 16角散射
        for (int i = -8; i <= 8; ++i) {
            double angle = i * 0.3926990;
            double vx = sin(angle) * 4;
            double vy = cos(angle) * 4;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y, vx, vy, 5, Camp::ENEMY));
        }
        shootTimer = 10;
        break;
    case 2: //角度变化散射
    {
        double offset =totaltime * 0.5;
        //int numBullets = 1;   // 5条弹道
        //for (int i = -2; i <= 2; ++i) {
            double baseAngle = 0;          // 基础间隔角度
            double angle = baseAngle + offset;    // 加上摆动偏移
            double vx = sin(angle) * 4;           // x方向速度
            double vy = cos(angle) * 4;           // y方向速度（正向下）
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y + radius, vx, vy, 5, Camp::ENEMY));
       // }
        // 散射模式冷却：20帧
        shootTimer = 1;
        break;
    }
    case 3: // 狙击炮
    {
        double dx = playerX - pos.x;
        double dy = playerY - pos.y;
        double len = sqrt(dx * dx + dy * dy);
        if (len > 0.1) {
            double vx = dx / len * 6;
            double vy = dy / len * 6;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y + radius, vx, vy, 6, Camp::ENEMY));
        }
        else {
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y + radius, 0, 6, 6, Camp::ENEMY));
        }
        shootTimer = 5;
    }
    break;
    default:
        shootTimer = 20;
        break;
    }
}
void Boss::TakeDamage(int damage)
{
    hp -= damage;
    if (hp < 0) hp = 0;
}