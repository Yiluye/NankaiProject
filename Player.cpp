#include"Player.h"
#include"messege.h"
#include"Global.h"
Player::Player()
	:Gameobject(30,30),hp(50000),radius(5),speed(6)
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
	// 将 60×60 的图片缩放到 32×32 绘制
	int drawSize = 32;
	int drawX = (int)(pos.x - drawSize / 2);
	int drawY = (int)(pos.y - drawSize / 2-12);
	putimage(drawX, drawY, drawSize, drawSize, &imgPlayer, 0, 0, SRCCOPY);

	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillcircle((int)pos.x, (int)pos.y, 4);  // radius = 16
	// 血条（按 32×32 调整位置）
	//setfillcolor(RED);
	//fillrectangle((int)(pos.x - 16), (int)(pos.y - 22),
		//(int)(pos.x - 16 + (hp * 32 / 100)), (int)(pos.y - 18));
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