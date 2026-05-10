#pragma once
#ifndef _INTERFACE_H
#define _INTERFACE_H
#include<string>
#include"Button.h"
#include<memory>
#include<vector>
#include <functional>
#include"item.h"

class Interface {
protected:
	std::wstring name;

public:
	Interface(const std::wstring& _name) :name(_name) {}
	virtual ~Interface() {}

	const std::wstring& Name() const { return name; }

	virtual void Onenter() {}
	virtual void Onexit() {}

	virtual void Update() {}
	virtual void Draw() {}

};


class Maininterface :public Interface {
private:
	Button* _pstartbutton;
	Button* _pexitbutton;

	//点击事件
	static void _Onclickstartbutton();
	static void _Onclickexitbutton();


public:
	Maininterface();
	~Maininterface();
	void Update();
	void Draw();

};

//前置声明
class Player;
class Boss;
class Enemy;
class Bullet;

class DanmakuGameInterface : public Interface {
private:

    std::shared_ptr<Player> player;                     // 玩家
    std::shared_ptr<Boss> boss;                         // 主Boss
    std::vector<std::shared_ptr<Enemy>> minions;        // 小怪（原 enemies 改名）

    std::vector<std::shared_ptr<Bullet>> bullets;       // 所有子弹
    std::vector<std::shared_ptr<Item>> items;

    int score;              // 得分
    bool gameRunning;       // 游戏是否进行中


    int shootCooldown;
    static constexpr int SHOOT_DELAY = 8;   // 玩家射击间隔（帧）


    int minionSpawnTimer;       // 当前剩余冷却帧数
    int minionSpawnDelay;       // 动态生成间隔（帧）


    //下面为按时间进行的变量
    int currentEventIndex;             // 当前执行到第几个事件
    std::vector<std::pair<int, std::function<void()>>> eventList;  // 事件列表

    void initEvents();                 // 初始化事件列表
    void checkEvents();                // 检查并执行事件

public:
    DanmakuGameInterface();
    virtual ~DanmakuGameInterface();

    // 界面生命周期
    void Onenter() override;
    void Onexit() override;
    void Update() override;
    void Draw() override;

private:
    // 辅助方法
    void updateBullets();               // 更新所有子弹位置
    void checkCollisions();             // 碰撞检测（玩家/Boss/小怪/子弹）
    void RemoveInactiveGameobject();
};
#endif
