#include "Boss.h"
#include "Bullet.h"
#include "Global.h"
#include <cmath>

Boss::Boss(double x, double y)
    : Gameobject(60, 60), hp(1500), maxHp(300), radius(30), shootTimer(0), Pattern(0), phase(0) {
    pos.x = x;
    pos.y = y;
    Alive = 0;
    Duration = 100;
    Pattern = 2;
    totaltime = 0;

    Basexuanzhuansanshe = 0;
}

void Boss::Update() {
    if (shootTimer > 0) shootTimer--;

    Alive++;
    if (Alive > Duration)
    {
        Pattern = (Pattern + 1) % 7;
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
    case 0: 
    {
        //for (int i = 0; i < 10; i++) {
          //  baseangle += pai / 10.0;
            for (int i =0; i <=6; ++i) {
                double angle = Basexuanzhuansanshe + i * 2 * pai /6;
                double vx = sin(angle) * 4;
                double vy = cos(angle) * 4;
                bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y, vx, vy, 5, Camp::ENEMY));
            }
            shootTimer = 20;
            Basexuanzhuansanshe += 0.3141592;
       // }
        break;
    }
    case 1:
    {
        const int numBullets = 20;
        static double rotation = 0.0;
        rotation += 0.2;  // 每轮增加约11.5度，非36°的倍数
        for (int i = 0; i < numBullets; ++i) {
            double angle = rotation + i * 2 * pai / numBullets;
            double vx = sin(angle) * 5;
            double vy = cos(angle) * 5;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y , vx, vy, 5, Camp::ENEMY));
        }
        shootTimer = 5;
        break;
    }
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
            double vx = dx / len * 10;
            double vy = dy / len * 10;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y , vx, vy, 5, Camp::ENEMY));
            bullets.push_back(std::make_shared<Bullet>(150, 200, vx >= 0 ? -vx : vx, vy, 5, Camp::ENEMY));
            bullets.push_back(std::make_shared<Bullet>(600, 200, vx < 0 ? -vx : vx, vy, 5, Camp::ENEMY));
        }
        else {
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y + radius, 0, 6, 5, Camp::ENEMY));
        }
        shootTimer = 1;
        break;
    }
    case 4: // 快速摆动散射
    {
        double dx = playerX - pos.x;
        double dy = playerY - pos.y;
        double baseangle=dx/dy;
        // 摆动频率：每帧变化 0.1 弧度（原为 0.02，提高5倍）
        double angularSpeed = 0.1;
        // 摆动幅度：±0.8 弧度 (约 ±45°)
        double amplitude = 1.5;
        double angle = sin(totaltime * angularSpeed) * amplitude;

            double vx = sin(angle) * 8;
            double vy = cos(angle) * 8;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y + radius, vx, vy, 8, Camp::ENEMY));
       
        shootTimer = 1;   // 缩短冷却，使弹幕更密集
        break;
    }
    case 5: // 旋转扇形射击（明显可见整体旋转）
    {
        static double baseAngle = 0.0;  // 或者用成员变量 BaseRotation
        baseAngle += 0.2618;

        int numDirections = 4;
        for (int i = 0; i < numDirections; ++i) {
            double angle = baseAngle + i * (2 * 3.14159 / numDirections);
            double vx = sin(angle) * 5;
            double vy = cos(angle) * 5;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y + radius, vx, vy, 5, Camp::ENEMY));
        }
        shootTimer = 5;
        break;
    }

    case 6: // 多方向梭形弹幕（8 方向，梭形指向径向）
    {
        int numDirections = 8;          // 8 个方向
        double speed = 5.0;
        double bulletWidth = 16.0;      // 长边（尖头方向）
        double bulletHeight = 6.0;      // 短边

        for (int i = 0; i < numDirections; ++i) {
            double angle = 2 * pai * i / numDirections;   // 发射方向（弧度）
            double vx = cos(angle) * speed;
            double vy = sin(angle) * speed;
            // 梭形长边指向径向（即旋转角 = 发射方向）
            bullets.push_back(std::make_shared<Bullet>(
                pos.x, pos.y, vx, vy,
                (int)bulletWidth, (int)bulletHeight,
                angle,                          // 旋转角
                Camp::ENEMY
            ));
        }
        shootTimer = 30;        // 冷却时间
        break;
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