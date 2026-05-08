#include"Bullet.h"
#include"Global.h"
Bullet::Bullet(double x, double y, double vx, double vy, int r, Camp c)
	: Gameobject(r * 2, r * 2), velocity{ vx, vy }, radius(r), active(true), camp(c) {
	pos.x = x;
	pos.y = y;
}


void Bullet::Update() 
{
	pos.x += velocity.x;
	pos.y += velocity.y;

	// ³¬³öÆÁÄ»ÔòÊ§Ð§
	if (pos.x + radius < 0 || pos.x - radius > SCREEN_WIDTH ||
		pos.y + radius < 0 || pos.y - radius > SCREEN_HEIGHT) {
		active = false;
	}
}

void Bullet::Draw()
{
	setfillcolor(camp == Camp::PLAYER ? GREEN : RED);
	fillcircle((int)pos.x, (int)pos.y, radius);
}

bool Bullet::IsActive() const 
{
	return active; 
}

void Bullet::Deactivate()
{
	active = false;
}

Camp Bullet::GetCamp() const
{ 
	return camp;
}

int Bullet::GetRadius() const 
{
	return radius;
}