#include "interface.h"
#include "Application.h"
#include "Global.h"
#include "messege.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Random.h"
#include <algorithm>

DanmakuGameInterface::DanmakuGameInterface()
    : Interface(L"DanmakuGame"), score(0), enemySpawnCounter(0), gameRunning(true), shootCooldown(0) {
}

DanmakuGameInterface::~DanmakuGameInterface() {
}

void DanmakuGameInterface::Onenter() {
    player = std::make_shared<Player>();
    enemies.clear();
    bullets.clear();
    score = 0;
    enemySpawnCounter = 0;
    gameRunning = true;
    shootCooldown = 0;
}

void DanmakuGameInterface::Onexit() {
    player.reset();
    enemies.clear();
    bullets.clear();
}

void DanmakuGameInterface::Update() {
    // ESC 返回主菜单
    if (Iskeydown(VK_ESCAPE)) {
        Getapplication()->Changeinterface(L"Main");
        return;
    }

    if (!gameRunning) {
        // 游戏结束，按 R 重新开始
        if (Iskeydown('R')) {
            Onenter();
        }
        return;
    }

    // 1. 更新玩家
    player->Update();

    // 2. 玩家射击（Z 键或空格）
    if (shootCooldown > 0) shootCooldown--;
    if ((Iskeydown('Z') || Iskeydown(VK_SPACE)) && shootCooldown == 0) {
        bullets.push_back(std::make_shared<Bullet>(
            player->Getx(), player->Gety() - 15, 0, -9, 4, Camp::PLAYER));
        shootCooldown = SHOOT_DELAY;
    }

    // 3. 生成敌人（静止，位置随机）
    if (enemySpawnCounter <= 0) {
        double randX = Random(50.0, 750.0);
        double randY = Random(50.0, 300.0);  // 敌人生成在上半屏
        enemies.push_back(std::make_shared<Enemy>(randX, randY));
        enemySpawnCounter = 90;  // 约1.5秒生成一个（60帧）
    }
    else {
        enemySpawnCounter--;
    }

    // 4. 更新敌人（静止，只更新冷却）并射击
    for (auto& enemy : enemies) {
        enemy->Update();
        enemy->DecrementCooldown();

        // 敌人向玩家方向发射子弹
        if (enemy->CanShoot()) {
            double dx = player->Getx() - enemy->Getx();
            double dy = player->Gety() - enemy->Gety();
            double len = sqrt(dx * dx + dy * dy);
            if (len > 0.1) {
                double vx = dx / len * 5;
                double vy = dy / len * 5;
                bullets.push_back(std::make_shared<Bullet>(
                    enemy->Getx(), enemy->Gety(), vx, vy, 4, Camp::ENEMY));
            }
            else {
                bullets.push_back(std::make_shared<Bullet>(
                    enemy->Getx(), enemy->Gety(), 0, 5, 4, Camp::ENEMY));
            }
            enemy->ResetShootCooldown();
        }
    }

    // 5. 更新子弹
    updateBullets();

    // 6. 碰撞检测
    checkCollisions();

    // 7. 移除无效对象
    removeInactiveBullets();
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::shared_ptr<Enemy>& e) { return !e->IsAlive(); }), enemies.end());

    // 8. 检查玩家是否死亡
    if (!player->IsAlive()) {
        gameRunning = false;
    }
}

void DanmakuGameInterface::Draw() {
    // 绘制玩家
    player->Draw();

    // 绘制敌人
    for (const auto& enemy : enemies) {
        enemy->Draw();
    }

    // 绘制子弹
    for (const auto& bullet : bullets) {
        bullet->Draw();
    }

    // 绘制 UI
    settextcolor(WHITE);
    TCHAR str[128];
    _stprintf_s(str, _T("Score: %d  HP: %d"), score, player->GetHp());
    outtextxy(10, 10, str);

    // 绘制敌人数
    _stprintf_s(str, _T("Enemies: %d"), (int)enemies.size());
    outtextxy(10, 40, str);

    if (!gameRunning) {
        settextstyle(40, 0, _T("宋体"));
        outtextxy(420, 250, _T("GAME OVER"));
        settextstyle(20, 0, _T("宋体"));
        outtextxy(420, 320, _T("Press R to Restart"));
        outtextxy(420, 350, _T("Press ESC to Menu"));
    }
}

void DanmakuGameInterface::spawnEnemy() {
    // 已在 Update 中实现
}

void DanmakuGameInterface::updateBullets() {
    for (auto& bullet : bullets) {
        bullet->Update();
    }
}

void DanmakuGameInterface::checkCollisions() {
    // 玩家与敌方子弹碰撞
    for (auto& bullet : bullets) {
        if (!bullet->IsActive())
        {
            continue;
        }
        if (bullet->GetCamp() == Camp::ENEMY) {
            //比较二者中心距离和半径之和来判断是否碰撞
            double dx = player->Getx() - bullet->Getx();
            double dy = player->Gety() - bullet->Gety();
            double minDist = player->GetRadius() + bullet->GetRadius();
            //玩家受击反馈
            if (dx * dx + dy * dy < minDist * minDist) {
                player->TakeDamage(10);
                bullet->Deactivate();
            }
        }
    }

    // 敌人与玩家子弹碰撞
    for (auto& enemy : enemies) {
        for (auto& bullet : bullets) {
            if (!bullet->IsActive()) continue;
            if (bullet->GetCamp() == Camp::PLAYER) {
                double dx = enemy->Getx() - bullet->Getx();
                double dy = enemy->Gety() - bullet->Gety();
                double minDist = enemy->GetRadius() + bullet->GetRadius();
                if (dx * dx + dy * dy < minDist * minDist) {
                    enemy->TakeDamage(10);
                    //子弹失活
                    bullet->Deactivate();
                    if (!enemy->IsAlive()) {
                        score += 100;
                    }
                }
            }
        }
    }
}

void DanmakuGameInterface::removeInactiveBullets() {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const std::shared_ptr<Bullet>& b) { return !b->IsActive(); }), bullets.end());
}