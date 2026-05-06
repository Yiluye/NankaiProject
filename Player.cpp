#include"Player.h"
#include"messege.h"
#include"Global.h"
Player::Player()
	:Gameobject(30,30),hp(100),radius(15),speed(6)
{
	pos.x = 400;
	pos.y = 500;
}


void Player::Update()
{
	// 移动（WASD 或 方向键）
	if (Iskeydown(VK_LEFT) || Iskeydown('A')) pos.x -= speed;
	if (Iskeydown(VK_RIGHT) || Iskeydown('D')) pos.x += speed;
	if (Iskeydown(VK_UP) || Iskeydown('W')) pos.y -= speed;
	if (Iskeydown(VK_DOWN) || Iskeydown('S')) pos.y += speed;

	//修正位置
	if (pos.x < radius) { pos.x = radius; }
	if (pos.x > SCREEN_WIDTH - radius) { pos.x = SCREEN_WIDTH - radius; }
	if (pos.y < radius) { pos.y = radius; }
	if (pos.y > SCREEN_HEIGHT - radius) { pos.y = SCREEN_HEIGHT - radius; }

}
void Player::Draw()
{
	setfillcolor(BLUE);
	fillcircle((int)pos.x, (int)pos.y, radius);

	// 血条
	setfillcolor(RED);
	fillrectangle((int)(pos.x - radius), (int)(pos.y - radius - 8),
		(int)(pos.x - radius + (hp * radius * 2 / 100)), (int)(pos.y - radius - 3));
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
bool Player::IsAlive() const {
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