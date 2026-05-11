#include"Player.h"
#include"messege.h"
#include"Global.h"

#include <windows.h>      
#pragma comment(lib, "Msimg32.lib")  

Player::Player()
	:Gameobject(30,30),hp(totalhp),radius(5),speed(6)
{
	pos.x = SCREEN_WIDTH/2;
	pos.y = 500;
}


void Player::Update()
{
	// 移动（WASD 或 方向键）
	if (Iskeydown(VK_LEFT) || Iskeydown('A')) pos.x -= speed;
	if (Iskeydown(VK_RIGHT) || Iskeydown('D')) pos.x += speed;
	if (Iskeydown(VK_UP) || Iskeydown('W')) pos.y -= speed;
	if (Iskeydown(VK_DOWN) || Iskeydown('S')) pos.y += speed;

	//调节速度
	if (Iskeydown(VK_U)) speed += 2;
	if (Iskeydown(VK_J))speed -= 2;
	//修正位置
	if (pos.x < radius) { pos.x = radius; }
	if (pos.x > GAME_AREA_RIGHT - radius) { pos.x = GAME_AREA_RIGHT - radius; }
	if (pos.y < radius) { pos.y = radius; }
	if (pos.y > SCREEN_HEIGHT - radius) { pos.y = SCREEN_HEIGHT - radius; }

}
void Player::Draw() {
	int drawSize = 32;
	int drawX = (int)(pos.x - drawSize / 2);
	int drawY = (int)(pos.y - drawSize / 2 - 12);

	// 使用透明绘制（抠掉黑色背景）
	TransparentBlt(GetImageHDC(NULL), drawX, drawY, drawSize, drawSize,
		GetImageHDC(&imgPlayer), 0, 0, 32, 32,
		RGB(0, 0, 0));  // 抠掉黑色背景
	//绘制判定点
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillcircle((int)pos.x, (int)pos.y, 4);  // 白色实心圆，半径 4
}

int Player::GetHp() const
{
	return hp;
}

void Player::TakeDamage(int damage)
{
	hp -= damage;
	if (hp < 0) hp = 0;
}

bool Player::IsAlive() const
{
	return hp > 0;
}

int Player::GetRadius() const
{
	return radius;
}

void Player::Reset()
{
	hp = 100;
	pos.x = SCREEN_WIDTH / static_cast<double>(2);
	pos.y = 500;
}

void Player::AddHp(int amount) {
	hp += amount;
	if (hp > totalhp)
	{
		hp = totalhp;
	}
}

void Player::SubHp(int amount) {
	hp -= amount;
	if (hp < 0) hp = 0;   // 血量不能为负
}