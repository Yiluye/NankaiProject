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

#include <mmsystem.h>      // 多媒体函数
#pragma comment(lib, "winmm.lib")  // 链接多媒体库

//构造函数
DanmakuGameInterface::DanmakuGameInterface()
    : Interface(L"DanmakuGame"), score(0), gameRunning(true), shootCooldown(0),
      minionSpawnTimer(0), minionSpawnDelay(90), currentEventIndex(0) {
    initEvents();   // 初始化事件列表
}

void DanmakuGameInterface::initEvents() {
    // 第 0 帧（游戏开始）
    eventList.push_back({ 0, [this]() {
        // 设置初始 Boss 模式
        if (boss) {
            boss->SetPattern(0);
        }
    } });

    // 10 秒（600 帧）后
    eventList.push_back({ 5*zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(1);           // 切换 Boss 弹幕模式
            minionSpawnDelay = 60;          // 小怪生成更快
        }
    } });

    // 20 秒（1200 帧）后
    eventList.push_back({ 10*zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(2);
            minionSpawnDelay = 45;
        }
    } });

    // 30 秒（1800 帧）后
    eventList.push_back({ 15*zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(3);
            minionSpawnDelay = 30;
        }
    } });

    // 40 秒（2400 帧）后 - Boss 狂暴
    eventList.push_back({ 20*zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(6);           // 梭形弹幕模式
            boss->SetShootTimer(5);         // 缩短冷却
        }
    } });

    // 60 秒（3600 帧）后 - 游戏胜利或进入二阶段
    eventList.push_back({ 25*zhenpermiao, [this]() {
        // 可以显示胜利文字，或者在下一个事件中结束游戏
        gameRunning = false;  // 胜利结束
    } });
}

//由智能指针管理没必要手动释放
DanmakuGameInterface::~DanmakuGameInterface() {
}
//进入初始化
//1、设置玩家和怪物位置
//2、清楚上局残留
void DanmakuGameInterface::Onenter() {
    gametime = 0;
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
    mciSendString(_T("open \"res/bgm.mp3\" alias game_bgm"), NULL, 0, NULL);
    mciSendString(_T("play game_bgm repeat"), NULL, 0, NULL);
}
void DanmakuGameInterface::Onexit() {
    mciSendString(_T("stop game_bgm"), NULL, 0, NULL);
    mciSendString(_T("close game_bgm"), NULL, 0, NULL);
    player.reset();
    boss.reset();
    minions.clear();
    bullets.clear();
    gametime = 0;
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
//判断事件
void DanmakuGameInterface::checkEvents() {
    while (currentEventIndex < eventList.size() &&
        eventList[currentEventIndex].first <= gametime) {
        // 执行对应的事件函数
        eventList[currentEventIndex].second();
        currentEventIndex++;
    }
}
//更新
void DanmakuGameInterface::Update() {
    //增加游戏时间
    gametime++;

    // ESC 返回主菜单
    if (Iskeydown(VK_ESCAPE)) {
        Getapplication()->Changeinterface(L"Main");
        gametime = 0;
        return;
    }

    if (!gameRunning) {
        // 游戏结束，按 R 重新开始
        if (Iskeydown('R')) {
            gametime = 0;
            Onenter();
        }
        return;
    }

    checkEvents();

    //玩家位置更新
    player->Update();

    //玩家射击更新
    if (shootCooldown > 0) shootCooldown--;
    //射击
    if ((Iskeydown('Z') || Iskeydown(VK_SPACE)) && shootCooldown == 0)
    {
        //双发子弹
        bullets.push_back(std::make_shared<Bullet>(
            player->Getx()-player->GetRadius(), player->Gety() - 15, 0, -9, 4,
            Camp::PLAYER,BulletColor::BTRED));
        bullets.push_back(std::make_shared< Bullet>(
            player->Getx()+player->GetRadius(), player->Gety() - 15, 0, -9, 4,
            Camp::PLAYER,BulletColor::BTRED));
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

        // 根据游戏时间动态设置小怪弹幕类型
        if (::gametime > 5*zhenpermiao && ::gametime < 10*zhenpermiao) {
            minion->SetBulletType(EnemyBulletType::SCATTER);
        }
        else if (::gametime >= 15*zhenpermiao && ::gametime < 20*zhenpermiao) {
            minion->SetBulletType(EnemyBulletType::RING);
        }
        else if (::gametime >= 20*zhenpermiao) {
            minion->SetBulletType(EnemyBulletType::LASER);
        }
        else {
            minion->SetBulletType(EnemyBulletType::NORMAL);
        }

        if (minion->CanShoot()) {
            double dx = player->Getx() - minion->Getx();
            double dy = player->Gety() - minion->Gety();
            double len = sqrt(dx * dx + dy * dy);

            switch (minion->GetBulletType()) {
            case EnemyBulletType::NORMAL:  // 普通自机狙
                if (len > 0.1) {
                    double vx = dx / len * 4;
                    double vy = dy / len * 4;
                    bullets.push_back(std::make_shared<Bullet>(
                        minion->Getx(), minion->Gety(), vx, vy, 4, Camp::ENEMY, BulletColor::BTRED));
                }
                break;

            case EnemyBulletType::SCATTER:  // 散射（3 方向）
                for (int i = -1; i <= 1; ++i) {
                    double angle = atan2(dy, dx) + i * 0.3;
                    double vx = cos(angle) * 4;
                    double vy = sin(angle) * 4;
                    bullets.push_back(std::make_shared<Bullet>(
                        minion->Getx(), minion->Gety(), vx, vy, 4, Camp::ENEMY, BulletColor::BTYELLOW));
                }
                break;

            case EnemyBulletType::RING:     // 环形（8 方向）
                for (int i = 0; i < 8; ++i) {
                    double angle = 2 * 3.14159 * i / 8;
                    double vx = cos(angle) * 3;
                    double vy = sin(angle) * 3;
                    bullets.push_back(std::make_shared<Bullet>(
                        minion->Getx(), minion->Gety(), vx, vy, 3, Camp::ENEMY, BulletColor::BTBLUE));
                }
                break;

            case EnemyBulletType::LASER:    // 直线快速激光
                if (len > 0.1) {
                    double vx = dx / len * 8;  // 速度更快
                    double vy = dy / len * 8;
                    bullets.push_back(std::make_shared<Bullet>(
                        minion->Getx(), minion->Gety(), vx, vy, 5, Camp::ENEMY, BulletColor::BTYELLOW));
                }
                break;
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
//绘画
void DanmakuGameInterface::Draw() {
    // 清屏（若有背景图可在此绘制）
    cleardevice();
    
    //绘制背景
    putimage(0, 0, &imgGame);

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


