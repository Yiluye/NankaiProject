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
void Enemy::Update() {
	if (shootCooldown > 0) shootCooldown--;

	// 左右移动
	if (enemymove == EnemyMove::LEFT) pos.x -= velocity;
	if (enemymove == EnemyMove::RIGHT) pos.x += velocity;

	// 超出边界则死亡（不再限制位置）
	if (pos.x + radius < 0 || pos.x - radius > GAME_AREA_RIGHT ||
		pos.y + radius < 0 || pos.y - radius > SCREEN_HEIGHT) {
		hp = 0;
	}
}

void Enemy::Draw() {
	// 使用贴图绘制小怪（49×49，居中绘制）
	int drawX = (int)(pos.x - 24.5);   // 49/2 = 24.5
	int drawY = (int)(pos.y - 24.5);
	putimage(drawX, drawY, &imgEnemy);
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