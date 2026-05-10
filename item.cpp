#include "Item.h"
#include "Global.h"
#include <cmath>

Item::Item(double x, double y, ItemType t, int v)
    : Gameobject(12, 12), type(t), value(v), lifeTime(300), active(true) {
    pos.x = x;
    pos.y = y;
}

void Item::Update() {
    // 缓慢下落
    pos.y += 2;

    // 生命周期减少
    lifeTime--;
    if (lifeTime <= 0) {
        active = false;
    }

    // 超出屏幕底部也消失
    if (pos.y > SCREEN_HEIGHT) {
        active = false;
    }
}

void Item::Draw() {
    int drawSize = 16;
    int dstX = (int)(pos.x - 8);
    int dstY = (int)(pos.y - 8);

    if (type == ItemType::POWER_RED) {
        // 红色 P 点：从 (0, 0) 截取
        putimage(dstX, dstY, drawSize, drawSize, &imgSprite, 0, 0, SRCCOPY);
    }
    else if (type == ItemType::POWER_BLUE) {
        // 蓝色 P 点：从 (16, 0) 截取
        putimage(dstX, dstY, drawSize, drawSize, &imgSprite, 16, 0, SRCCOPY);
    }
}

void Item::Collect() {
    active = false;
}