#include "Boss.h"
#include "Bullet.h"
#include "Global.h"
#include <cmath>
#include <windows.h>      
#pragma comment(lib, "Msimg32.lib") 

Boss::Boss(double x, double y)
    : Gameobject(60, 60), hp(1500), maxHp(300), radius(30), shootTimer(0), Pattern(0), phase(0) {
    pos.x = x;
    pos.y = y;
    Alive = 0;
    Duration = 200;
    Pattern = 0;
}

void Boss::Update() {
    if (shootTimer > 0)
    {
        shootTimer--;
    }
}

// Boss.cpp 的 Draw() 函数
void Boss::Draw() {
    // 使用透明绘制（抠掉黑色背景）
        TransparentBlt(GetImageHDC(NULL),
            (int)(pos.x - 33), (int)(pos.y - 75), 66, 150,
            GetImageHDC(&imgBoss), 0, 0, 66, 150,
            RGB(255,255,255));  // 黑色背景变透明

    // 血条（根据图片大小调整位置）
    setfillcolor(RED);
    fillrectangle((int)(pos.x - 50), (int)(pos.y - 85),
        (int)(pos.x - 50 + 120 * hp / maxHp), (int)(pos.y - 78));
}


void Boss::Shoot(std::vector<std::shared_ptr<Bullet>>& bullets, double playerX, double playerY) {
    if (shootTimer > 0) return;

    switch (Pattern) {
    case 0: 
    {
        static double basexuan = 0;
        for (int i = 0; i <= 6; ++i) {
            double angle = basexuan + i * 2 * pai / 6;
            double vx = sin(angle) * 8;
            double vy = cos(angle) * 8;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y, vx, vy, 8,
                Camp::ENEMY, BulletColor::BTBLUE));
        }
        shootTimer = 1;
        basexuan += 0.3141592;
     
        break;
    }
    case 1:
    {
        const int numBullets = 20;
        static double rotation = 0.0;
        rotation += 0.2;  // 每轮增加约11.5度，非36°的倍数
        for (int i = 0; i < numBullets; ++i) {
            double angle = rotation - i * 2 * pai / numBullets;
            double vx = sin(angle) * 5;
            double vy = cos(angle) * 5;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y , vx, vy, 8, 
                Camp::ENEMY,BulletColor::BTRED));
        }
        shootTimer = 2;
        break;
    }
    case 2: //角度变化散射
    {
        double offset =gametime * 0.5;
       
        double baseAngle = 0; 
        double angle = baseAngle + offset; 
        double vx = sin(angle) * 4; 
        double vy = cos(angle) * 4; 
         bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y, vx, vy, 8,
             Camp::ENEMY, BulletColor::BTCYAN));
        shootTimer = 1;
        break;
    }
    case 3: // 狙击炮
    {
        double dx = playerX - pos.x;
        double dy = playerY - pos.y;
        double len = sqrt(dx * dx + dy * dy);
        if (len > 0.1) {
            double vx = dx / len * 18;
            double vy = dy / len * 18;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y , vx, vy, 8,
                Camp::ENEMY, BulletColor::BTYELLOW));
            bullets.push_back(std::make_shared<Bullet>(150, 200, vx >= 0 ? -vx : vx, vy, 8,
                Camp::ENEMY, BulletColor::BTRED));
            bullets.push_back(std::make_shared<Bullet>(600, 200, vx < 0 ? -vx : vx, vy, 8,
                Camp::ENEMY, BulletColor::BTBLUE));
        }
        else {
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y, 0, 6, 8,
                Camp::ENEMY, BulletColor::BTRED));
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
        double angle = sin(gametime * angularSpeed) * amplitude;

            double vx = sin(angle) * 8;
            double vy = cos(angle) * 8;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y + radius, vx, vy, 8,
                Camp::ENEMY, BulletColor::BTPINK));
       
        shootTimer = 1;
        break;
    }
    case 5: // 旋转扇形射击
    {
        static double baseAngle = 0.0; 
        baseAngle += 0.2618;

        int numDirections = 4;
        for (int i = 0; i < numDirections; ++i) {
            double angle = baseAngle + i * (1 * 3.14159 / numDirections);
            double vx = sin(angle) * 5;
            double vy = cos(angle) * 5;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y, vx, vy, 8,
                Camp::ENEMY, BulletColor::BTCYAN));
        }
        shootTimer = 1;
        break;
    }
    // 旋转扇形射击
    case 6: 
    {
        static double baseAngle = 0.0;  // 或者用成员变量 BaseRotation
        baseAngle += 0.2618;

        int numDirections = 4;
        for (int i = 0; i < numDirections; ++i) {
            double angle = baseAngle + i * (2 * 3.14159 / numDirections);
            double vx = sin(angle) * 5;
            double vy = cos(angle) * 5;
            bullets.push_back(std::make_shared<Bullet>(pos.x, pos.y + radius, vx, vy, 8,
                Camp::ENEMY,BulletColor::BTRED));
        }
        shootTimer = 5;
        break;
    }
    // 多方向梭形弹幕（8 方向，梭形指向径向）
    case 7: 
    {
        int numDirections = 25;          // 8 个方向
        double speed = 6.0;
        double bulletWidth = 6.0;      // 长边（尖头方向）
        double bulletHeight = 2.0;      // 短边
        static double baseangle = 0.0;
        baseangle += 0.31415;

        for (int i = 0; i < numDirections; ++i) {
            double angle = baseangle+2 * pai * i / numDirections;   // 发射方向（弧度）
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
        shootTimer = 3;        // 冷却时间
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