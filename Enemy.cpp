#include "Enemy.h"
#include "Global.h"
Enemy::Enemy(double x, double y, EnemyMove move)
	: Gameobject(28, 28), hp(30), radius(14), shootCooldown(0),bulletType(EnemyBulletType::NORMAL)
{
	pos.x = x;
	pos.y = y;
	enemymove = move;
	velocity = 1;
}
// ¾²Ö¹£ºÖ»¸üÐÂÀäÈ´
void Enemy::Update()
{
	if (shootCooldown > 0) shootCooldown--;
	if (enemymove == EnemyMove::LEFT) pos.x -= velocity;
	if (enemymove == EnemyMove::RIGHT) pos.x += velocity;

	if (pos.x < radius) pos.x = radius;
	if (pos.x > GAME_AREA_RIGHT - radius) pos.x = GAME_AREA_RIGHT - radius;
}


void Enemy::Draw()
{
	setfillcolor(RED);
	fillcircle((int)pos.x, (int)pos.y, radius);
}

int Enemy::GetHp() const {
	return hp;
}


bool Enemy::IsAlive() const 
{ 
	return hp > 0;
}

bool Enemy::CanShoot() const 
{ 
	return shootCooldown <= 0;
}

void Enemy::ResetShootCooldown() 
{
	shootCooldown = SHOOT_DELAY;
}


int Enemy::GetRadius() const 
{
	return radius; 
}

void Enemy::TakeDamage(int damage)
{
	hp -= damage;
	if (hp < 0) hp = 0;
}

void Enemy::SetBulletType(EnemyBulletType type) 
{
	bulletType = type;
}

EnemyBulletType Enemy::GetBulletType()
{
	return bulletType;
}