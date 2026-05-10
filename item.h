#pragma once
#ifndef _ITEM_H
#define _ITEM_H

#include "Gameobject.h"
#include "Defines.h"

enum class ItemType {
    POWER_RED,   // 红色 P 点
    POWER_BLUE   // 蓝色 P 点
};

class Item : public Gameobject {
private:
    ItemType type;
    int value;          // 道具数值（P点增加量、分数值、回复量）
    int lifeTime;       // 存活时间（帧），超时自动消失
    bool active;

public:
    Item(double x, double y, ItemType t, int v);
    ~Item() = default;

    void Update() override;
    void Draw() override;

    bool IsActive() const { return active; }
    ItemType GetType() const { return type; }
    int GetValue() const { return value; }
    void Collect();     // 被收集时调用
};

#endif