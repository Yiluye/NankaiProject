#include "Interface.h"
#include "Application.h"
#include "Global.h"
#include "messege.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Bullet.h"
#include "Random.h"
#include <algorithm>

//构造函数
DanmakuGameInterface::DanmakuGameInterface()
    : Interface(L"DanmakuGame"), score(0), gameRunning(true), shootCooldown(0),
    minionSpawnTimer(0), minionSpawnDelay(90) {
}
//由智能指针管理没必要手动释放
DanmakuGameInterface::~DanmakuGameInterface() {
}
//进入初始化
//1、设置玩家和怪物位置
//2、清楚上局残留
void DanmakuGameInterface::Onenter() {
    player = std::make_shared<Player>();
    boss = std::make_shared<Boss>(SCREEN_WIDTH / 2.0, 80.0);
    //
    minions.clear();
    bullets.clear();
    score = 0;
    minionSpawnTimer = 0;
    minionSpawnDelay = 90;   // 初始生成间隔（帧）
    gameRunning = true;
    shootCooldown = 0;
}
void DanmakuGameInterface::Onexit() {
    player.reset();
    boss.reset();
    minions.clear();
    bullets.clear();
}
//子弹更新
void DanmakuGameInterface::updateBullets() {
    for (auto& bullet : bullets) {
        bullet->Update();
    }
}
//删除失活的游戏对象（敌人和子弹）
void DanmakuGameInterface::RemoveInactiveGameobject() {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const std::shared_ptr<Bullet>& b) { return !b->IsActive(); }), bullets.end());

    minions.erase(std::remove_if(minions.begin(), minions.end(),
        [](const std::shared_ptr<Enemy>& e) { return !e->IsAlive(); }), minions.end());
}
//碰撞检测
void DanmakuGameInterface::checkCollisions() {
    // 玩家与敌方子弹碰撞（所有敌方子弹）
    for (auto& bullet : bullets) {
        if (!bullet->IsActive()) continue;
        if (bullet->GetCamp() == Camp::ENEMY) {
            double dx = player->Getx() - bullet->Getx();
            double dy = player->Gety() - bullet->Gety();
            double minDist = player->GetRadius() + bullet->GetRadius();
            if (dx * dx + dy * dy < minDist * minDist) {
                player->TakeDamage(10);
                bullet->Deactivate();
                if (!player->IsAlive()) {
                    gameRunning = false;
                }
            }
        }
    }

    //Boss受击
    if (boss && boss->IsAlive()) {
        for (auto& bullet : bullets) {
            if (!bullet->IsActive()) continue;
            if (bullet->GetCamp() == Camp::PLAYER) {
                double dx = boss->Getx() - bullet->Getx();
                double dy = boss->Gety() - bullet->Gety();
                double minDist = boss->GetRadius() + bullet->GetRadius();
                if (dx * dx + dy * dy < minDist * minDist) {
                    boss->TakeDamage(10);
                    bullet->Deactivate();
                    if (!boss->IsAlive()) {
                        score += 2000;   // 击败 Boss 奖励
                    }
                }
            }
        }
    }

    //小怪受击
    for (auto& minion : minions) {
        for (auto& bullet : bullets) {
            if (!bullet->IsActive()) continue;
            if (bullet->GetCamp() == Camp::PLAYER) {
                double dx = minion->Getx() - bullet->Getx();
                double dy = minion->Gety() - bullet->Gety();
                double minDist = minion->GetRadius() + bullet->GetRadius();
                if (dx * dx + dy * dy < minDist * minDist) {
                    minion->TakeDamage(10);
                    bullet->Deactivate();
                    if (!minion->IsAlive()) {
                        score += 100;
                    }
                }
            }
        }
    }
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

    //玩家位置更新
    player->Update();

    //玩家射击更新
    if (shootCooldown > 0) shootCooldown--;
    //射击
    if ((Iskeydown('Z') || Iskeydown(VK_SPACE)) && shootCooldown == 0)
    {
        //双发子弹
        bullets.push_back(std::make_shared<Bullet>(
            player->Getx()-player->GetRadius(), player->Gety() - 15, 0, -9, 4, Camp::PLAYER));
        bullets.push_back(std::make_shared< Bullet>(
            player->Getx()+player->GetRadius(), player->Gety() - 15, 0, -9, 4, Camp::PLAYER));
        //重置射击冷却
        shootCooldown = SHOOT_DELAY;
    }

    //Boss射击更新
    if (boss && boss->IsAlive()) 
    {
        boss->Update();
        boss->Shoot(bullets, player->Getx(), player->Gety());
    }
    else if (boss && !boss->IsAlive()) 
    {
        //Boss死亡直接结束游戏
        gameRunning = false;
    }

    //生成小怪
    if (boss && boss->IsAlive()) 
    {
        if (minionSpawnTimer <= 0) 
        {
            // 随机位置（屏幕上半区）
            double randX = Random(40.0, SCREEN_WIDTH - 40.0);
            double randY = Random(30.0, 150.0);
            //minions.push_back(std::make_shared<Enemy>(randX, randY,0));
            EnemyMove move = (Random(0, 2) == 0) ? EnemyMove::LEFT : EnemyMove::RIGHT;
            minions.push_back(std::make_shared<Enemy>(randX, randY, move));
            //minions.push_back(std::make_shared<Enemy>(randX, randY, EnemyMove::LEFT));
            //更新小怪生成冷却
            minionSpawnTimer = minionSpawnDelay;
        }
        else 
        {
            minionSpawnTimer--;
        }
    }

    //更新小怪射击
    for (auto& minion : minions) {
        minion->Update();

        //狙击炮 
        if (minion->CanShoot()) {
            double dx = player->Getx() - minion->Getx();
            double dy = player->Gety() - minion->Gety();
            double len = sqrt(dx * dx + dy * dy);
            if (len > 0.1) {
                double vx = dx / len * 4;
                double vy = dy / len * 4;
                bullets.push_back(std::make_shared<Bullet>(
                    minion->Getx(), minion->Gety(), vx, vy, 4, Camp::ENEMY));
            }
            else {
                bullets.push_back(std::make_shared<Bullet>(
                    minion->Getx(), minion->Gety(), 0, 4, 4, Camp::ENEMY));
            }
            minion->ResetShootCooldown();
        }
    }

    //更新所有子弹
    updateBullets();

    // 碰撞检测
    checkCollisions();

    //删除垃圾
    RemoveInactiveGameobject();

    //检查玩家是否死亡
    if (!player->IsAlive()) {
        gameRunning = false;
    }
}

void DanmakuGameInterface::Draw() {
    // 清屏（若有背景图可在此绘制）
    cleardevice();

    // 绘制 Boss（如果存活）
    if (boss && boss->IsAlive()) {
        boss->Draw();
    }

    // 绘制小怪
    for (const auto& minion : minions) {
        minion->Draw();
    }

    // 绘制玩家
    player->Draw();

    // 绘制子弹
    for (const auto& bullet : bullets) {
        bullet->Draw();
    }

    // 绘制 UI
    settextcolor(WHITE);
    TCHAR str[128];
    _stprintf_s(str, _T("Score: %d  HP: %d"), score, player->GetHp());
    outtextxy(10, 10, str);
    _stprintf_s(str, _T("Minions: %d"), (int)minions.size());
    outtextxy(10, 40, str);
    if (boss) {
        _stprintf_s(str, _T("Boss HP: %d/%d"), boss->GetHp(), boss->GetMaxHp());
        outtextxy(10, 70, str);
    }

    // 游戏结束画面
    if (!gameRunning) {
        settextstyle(40, 0, _T("宋体"));
        outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, _T("GAME OVER"));
        settextstyle(20, 0, _T("宋体"));
        outtextxy(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 30, _T("Press R to Restart"));
        outtextxy(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 60, _T("Press ESC to Menu"));
    }
}


