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
            boss->SetPattern(2);
        }
    } });

    // 10 秒后
    eventList.push_back({ 10*zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(6);           // 切换 Boss 弹幕模式
            minionSpawnDelay = 60;          // 小怪生成更快
        }
    } });
    // 20 秒后
    eventList.push_back({ 20 * zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(4);           // 切换 Boss 弹幕模式
            minionSpawnDelay = 60;          // 小怪生成更快
        }
    } });

    // 30 秒后
    eventList.push_back({ 30*zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(5);
            minionSpawnDelay = 45;
        }
    } });

    // 40 秒后
    eventList.push_back({ 40*zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(3);
            minionSpawnDelay = 30;
        }
    } });

    // 50 秒后 - Boss 狂暴
    eventList.push_back({ 50*zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(6);           // 梭形弹幕模式
            boss->SetShootTimer(0);         // 缩短冷却
        }
    } });
    // 60 秒后 - Boss 狂暴
    eventList.push_back({ 60 * zhenpermiao, [this]() {
        if (boss) {
            boss->SetPattern(7);           // 梭形弹幕模式
            boss->SetShootTimer(5);         // 缩短冷却
        }
    } });
    // 70 秒后 - Boss 狂暴
    eventList.push_back({ 70 * zhenpermiao, [this]() {
       if (boss) {
           boss->SetPattern(1);           // 梭形弹幕模式
           boss->SetShootTimer(5);         // 缩短冷却
       }
   } });

    // 70 秒后 - 游戏胜利或进入二阶段
    eventList.push_back({ 80*zhenpermiao, [this]() {
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
    boss = std::make_shared<Boss>(GAME_AREA_RIGHT / 2.0, 80.0);
    //
    minions.clear();
    bullets.clear();
    items.clear();
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
    items.clear();
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

    items.erase(std::remove_if(items.begin(), items.end(),
        [](const std::shared_ptr<Item>& i) { return !i->IsActive(); }), items.end());
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
                
                PlaySound(_T("res/baka.wav"), NULL, SND_ASYNC | SND_FILENAME);

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

                        // 随机掉落红色或蓝色 P 点
                        ItemType powerType;
                        if (Random(0, 2) == 0) {
                            powerType = ItemType::POWER_RED;
                        }
                        else {
                            powerType = ItemType::POWER_BLUE;
                        }
                        items.push_back(std::make_shared<Item>(
                            minion->Getx(), minion->Gety(), powerType, 1));
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
            player->Getx() - player->GetRadius() - 2, player->Gety() - 15, 0, -9, 8,
            Camp::PLAYER, BulletColor::BTRED));
        bullets.push_back(std::make_shared< Bullet>(
            player->Getx() + player->GetRadius() + 2, player->Gety() - 15, 0, -9, 8,
            Camp::PLAYER, BulletColor::BTRED));

        PlaySound(_T("res/shoot.wav"), NULL, SND_ASYNC | SND_FILENAME);

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
            double randX = Random(40.0, GAME_AREA_RIGHT - 40.0);
            double randY = Random(30.0, 250.0);
            EnemyMove move = (Random(0, 2) == 0) ? EnemyMove::LEFT : EnemyMove::RIGHT;
            minions.push_back(std::make_shared<Enemy>(randX, randY, move));
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

        int t = ::gametime / zhenpermiao;  // 当前秒数

        if (t < 5) {
            minion->SetBulletType(EnemyBulletType::RING);
        }
        else if (t < 15) {
            minion->SetBulletType(EnemyBulletType::SCATTER);
        }
        else if (t < 25) {
            minion->SetBulletType(EnemyBulletType::SHUTTLE);
        }
        else if (t < 35) {
            minion->SetBulletType(EnemyBulletType::LASER);
        }
        else if (t < 45) {
            minion->SetBulletType(EnemyBulletType::SHUTTLE);  // 梭形弹幕
        }
        else if (t < 55) {
            minion->SetBulletType(EnemyBulletType::RING);
        }
        else if (t < 55) {
            minion->SetBulletType(EnemyBulletType::SCATTER);
        }
        else if (t < 65) {
            minion->SetBulletType(EnemyBulletType::LASER);
        }
        else if (t < 75) {
            minion->SetBulletType(EnemyBulletType::SCATTER);
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
                        minion->Getx(), minion->Gety(), vx, vy, 8, Camp::ENEMY, BulletColor::BTRED));
                }
                break;

            case EnemyBulletType::SCATTER:  // 散射（3 方向）
                for (int i = -1; i <= 1; ++i) {
                    double angle = atan2(dy, dx) + i * 0.3;
                    double vx = cos(angle) * 4;
                    double vy = sin(angle) * 4;
                    bullets.push_back(std::make_shared<Bullet>(
                        minion->Getx(), minion->Gety(), vx, vy, 8, Camp::ENEMY, BulletColor::BTYELLOW));
                }
                break;

            case EnemyBulletType::RING:     // 环形（8 方向）
                for (int i = 0; i < 8; ++i) {
                    double angle = 2 * 3.14159 * i / 8;
                    double vx = cos(angle) * 3;
                    double vy = sin(angle) * 3;
                    bullets.push_back(std::make_shared<Bullet>(
                        minion->Getx(), minion->Gety(), vx, vy, 8, Camp::ENEMY, BulletColor::BTBLUE));
                }
                break;

            case EnemyBulletType::LASER:    // 直线快速激光
                if (len > 0.1) {
                    double vx = dx / len * 8;
                    double vy = dy / len * 8;
                    bullets.push_back(std::make_shared<Bullet>(
                        minion->Getx(), minion->Gety(), vx, vy, 8, Camp::ENEMY, BulletColor::BTYELLOW));
                }
                break;

            case EnemyBulletType::SHUTTLE:  // 梭形弹幕
            {
                if (len > 0.1) {
                    double angle = atan2(dy, dx);
                    double vx = cos(angle) * 5;
                    double vy = sin(angle) * 5;
                    bullets.push_back(std::make_shared<Bullet>(
                        minion->Getx(), minion->Gety(), vx, vy,
                        12, 4, angle, Camp::ENEMY));
                }
                break;
            }
            }  // switch 结束

            minion->ResetShootCooldown();
        }
    }  // for 循环结束

    //更新所有子弹
    updateBullets();

    // 更新道具
    for (auto& item : items) {
        item->Update();
    }

    // 玩家拾取道具
    for (auto& item : items) {
        if (!item->IsActive()) continue;
        double dx = player->Getx() - item->Getx();
        double dy = player->Gety() - item->Gety();
        double minDist = player->GetRadius() + 8;
        if (dx * dx + dy * dy < minDist * minDist) {
            if (item->GetType() == ItemType::POWER_RED) {
                player->AddHp(10);
            }
            else if (item->GetType() == ItemType::POWER_BLUE) {
                score += 10;
            }
            item->Collect();
        }
    }

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
    // 清屏
    cleardevice();

    // 绘制游戏背景（左侧游戏区域）
    putimage(0, 0, &imgGame);

    // ========== 绘制游戏对象 ==========
    if (boss && boss->IsAlive()) {
        boss->Draw();
    }

    for (const auto& minion : minions) {
        minion->Draw();
    }

    player->Draw();

    for (const auto& bullet : bullets) {
        bullet->Draw();
    }

    for (const auto& item : items) {
        item->Draw();
    }

    setbkmode(TRANSPARENT);  // 去掉文字黑底

    // ========== 绘制右侧 UI 面板 ==========
    int uiX = GAME_AREA_RIGHT;
    int uiWidth = SCREEN_WIDTH - GAME_AREA_RIGHT;

    // 1. 绘制 UI 背景（半透明或纯色面板）
    setfillcolor(RGB(30, 30, 50));      // 深蓝灰色背景
    setlinecolor(RGB(100, 100, 150));   // 边框颜色
    fillrectangle(uiX, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    rectangle(uiX, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // 2. 绘制标题装饰条
    setfillcolor(RGB(200, 100, 50));     // 橙色装饰条
    fillrectangle(uiX + 5, 5, SCREEN_WIDTH - 5, 35);

    // 3. 标题文字
    settextcolor(RGB(255, 255, 200));
    settextstyle(20, 0, _T("微软雅黑"));
    outtextxy(uiX + 15, 8, _T("=== 游戏状态 ==="));

    // 4. UI 内容区域起始位置
    int startY = 60;
    int lineHeight = 35;

    // 设置字体大小
    settextstyle(18, 0, _T("微软雅黑"));

    // 5. 分数（金色高亮）
    TCHAR str[128];
    settextcolor(RGB(255, 215, 0));      // 金色
    _stprintf_s(str, _T("★ 得分"));
    outtextxy(uiX + 15, startY, str);
    settextcolor(RGB(255, 255, 255));    // 白色
    _stprintf_s(str, _T("%d"), score);
    outtextxy(uiX + 120, startY, str);

    // 6. 玩家血量（红色）
    startY += lineHeight;
    settextcolor(RGB(255, 100, 100));    // 浅红色
    _stprintf_s(str, _T("❤ 生命"));
    outtextxy(uiX + 15, startY, str);
    settextcolor(RGB(255, 255, 255));
    _stprintf_s(str, _T("%d / 100"), player->GetHp());
    outtextxy(uiX + 120, startY, str);

    // 7. 小怪数量（绿色）
    startY += lineHeight;
    settextcolor(RGB(100, 255, 100));    // 绿色
    _stprintf_s(str, _T("👾 敌人数"));
    outtextxy(uiX + 15, startY, str);
    settextcolor(RGB(255, 255, 255));
    _stprintf_s(str, _T("%d"), (int)minions.size());
    outtextxy(uiX + 120, startY, str);

    // 8. Boss 血量（根据百分比变色）
    startY += lineHeight;
    if (boss && boss->IsAlive()) {
        int hpPercent = boss->GetHp() * 100 / boss->GetMaxHp();
        settextcolor(RGB(255, 150, 50));   // 橙色
        _stprintf_s(str, _T("👿 Boss"));
        outtextxy(uiX + 15, startY, str);

        // 血量文字颜色随百分比变化
        if (hpPercent > 60) {
            settextcolor(RGB(0, 255, 0));      // 绿色
        }
        else if (hpPercent > 30) {
            settextcolor(RGB(255, 200, 0));    // 黄色
        }
        else {
            settextcolor(RGB(255, 0, 0));      // 红色
        }
        _stprintf_s(str, _T("HP: %d/%d (%.0f%%)"), boss->GetHp(), boss->GetMaxHp(), (float)hpPercent);
        outtextxy(uiX + 90, startY, str);

        // 绘制 Boss 血条
        startY += lineHeight - 10;
        int barWidth = uiWidth - 30;
        int barHeight = 12;
        int barX = uiX + 15;

        // 背景条（灰色）
        setfillcolor(RGB(60, 60, 60));
        fillrectangle(barX, startY, barX + barWidth, startY + barHeight);

        // 前景条（根据血量变色）
        int fillWidth = barWidth * boss->GetHp() / boss->GetMaxHp();
        COLORREF barColor;
        if (hpPercent > 60) barColor = RGB(0, 200, 0);
        else if (hpPercent > 30) barColor = RGB(255, 200, 0);
        else barColor = RGB(200, 0, 0);
        setfillcolor(barColor);
        fillrectangle(barX, startY, barX + fillWidth, startY + barHeight);
    }

    // 9. 游戏时间显示
    startY += lineHeight + 10;
    settextcolor(RGB(150, 150, 200));
    _stprintf_s(str, _T("⏱ 时间: %d 秒"), ::gametime / 60);
    outtextxy(uiX + 15, startY, str);

    // 10. 操作提示（底部）
    startY = SCREEN_HEIGHT - 100;
    settextcolor(RGB(150, 150, 150));
    settextstyle(14, 0, _T("微软雅黑"));
    outtextxy(uiX + 15, startY, _T("[WASD/方向键] 移动"));
    outtextxy(uiX + 15, startY + 20, _T("[Z/空格] 射击"));
    outtextxy(uiX + 15, startY + 40, _T("[ESC] 返回菜单"));

    // ========== 游戏结束画面 ==========
    if (!gameRunning) {
        // 设置文字背景透明（关键）
        setbkmode(TRANSPARENT);

        settextstyle(48, 0, _T("微软雅黑"));
        settextcolor(RGB(255, 50, 50));
        outtextxy(GAME_AREA_RIGHT / 2 - 120, SCREEN_HEIGHT / 2 - 80, _T("G A M E   O V E R"));

        settextstyle(20, 0, _T("微软雅黑"));
        settextcolor(RGB(1, 1, 1));
        outtextxy(GAME_AREA_RIGHT / 2 - 100, SCREEN_HEIGHT / 2 - 20, _T("按 R 键重新开始"));
        outtextxy(GAME_AREA_RIGHT / 2 - 100, SCREEN_HEIGHT / 2 + 15, _T("按 ESC 键返回菜单"));

        TCHAR scoreText[64];
        _stprintf_s(scoreText, _T("最 终 得 分 :  %d"), score);
        outtextxy(GAME_AREA_RIGHT / 2 - 100, SCREEN_HEIGHT / 2 + 60, scoreText);

        // 恢复默认背景模式（可选）
        setbkmode(OPAQUE);
    }
}


