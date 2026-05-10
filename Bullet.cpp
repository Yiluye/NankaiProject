#include"Bullet.h"
#include"Global.h"


// 圆形构造（使用贴图）
Bullet::Bullet(double x, double y, double vx, double vy, int r, Camp c, BulletColor col)
	: Gameobject(r * 2, r * 2),
	velocity{ vx, vy },
	radius(r),
	width(r * 2),
	height(r * 2),
	rotation(0.0),
	active(true),
	camp(c),
	shape(BulletShape::CIRCLE),
	color(col)
{
	pos.x = x;
	pos.y = y;
}

// 矩形构造（无旋转）
Bullet::Bullet(double x, double y, double vx, double vy, int w, int h, Camp c)
	: Gameobject(w, h),
	velocity{ vx, vy },
	radius(((w > h ? w : h) + 1) / 2),
	width(w),
	height(h),
	rotation(0.0),
	active(true),
	camp(c),
	shape(BulletShape::RECT) {
	pos.x = x;
	pos.y = y;
}

// 矩形构造（带旋转）
Bullet::Bullet(double x, double y, double vx, double vy, int w, int h, double rot, Camp c)
	: Gameobject(w, h),
	velocity{ vx, vy },
	radius(((w > h ? w : h) + 1) / 2),
	width(w),
	height(h),
	rotation(rot),
	active(true),
	camp(c),
	shape(BulletShape::RECT) {
	pos.x = x;
	pos.y = y;
}



void Bullet::Update() 
{
	pos.x += velocity.x;
	pos.y += velocity.y;

	// 超出屏幕则失效
	if (pos.x + radius < 0 || pos.x - radius > GAME_AREA_RIGHT ||
		pos.y + radius < 0 || pos.y - radius > SCREEN_HEIGHT) {
		active = false;
	}
}

static POINT RotatePoint(double cx, double cy, double x, double y, double angle) {
	double dx = x - cx;
	double dy = y - cy;
	double cosA = cos(angle);
	double sinA = sin(angle);
	double nx = cx + dx * cosA - dy * sinA;
	double ny = cy + dx * sinA + dy * cosA;
	return { (int)nx, (int)ny };
}

void Bullet::Draw() {
	if (shape == BulletShape::CIRCLE) {
		int drawSize = radius * 2;
		int dstX = (int)(pos.x - drawSize / 2);
		int dstY = (int)(pos.y - drawSize / 2);

		if (camp == Camp::PLAYER) {
			// 玩家子弹使用独立精灵表，从 (128, 16) 截取
			putimage(dstX, dstY, drawSize, drawSize, &imgPlayerBullet, 128, 16, SRCCOPY);
		}
		else {
			// 敌方子弹使用原精灵表
			if (color == BTRED) {
				putimage(dstX, dstY, drawSize, drawSize, &imgSprite, 32, 32, SRCCOPY);
			}
			else if (color == BTBLUE) {
				putimage(dstX, dstY, drawSize, drawSize, &imgSprite, 80, 48, SRCCOPY);
			}
			else if (color == BTCYAN) {
				putimage(dstX, dstY, drawSize, drawSize, &imgSprite, 112, 32, SRCCOPY);
			}
			else if (color == BTYELLOW) {
				putimage(dstX, dstY, drawSize, drawSize, &imgSprite, 192, 32, SRCCOPY);
			}
			else if (color == BTPINK) {
				putimage(dstX, dstY, drawSize, drawSize, &imgSprite, 64, 32, SRCCOPY);
			}
		}
	}
	else if (shape == BulletShape::RECT) {
		// 梭形（旋转矩形）
		double hw = width / 2.0;
		double hh = height / 2.0;
		POINT pts[4] = {
			{ (int)(-hw), (int)(-hh) },
			{ (int)(hw), (int)(-hh) },
			{ (int)(hw), (int)(hh) },
			{ (int)(-hw), (int)(hh) }
		};
		POINT worldPts[4];
		for (int i = 0; i < 4; ++i) {
			worldPts[i] = RotatePoint(0, 0, pts[i].x, pts[i].y, rotation);
			worldPts[i].x += (int)pos.x;
			worldPts[i].y += (int)pos.y;
		}
		setfillcolor(CYAN);
		fillpolygon(worldPts, 4);
	}
	// SPRITE 类型暂不处理
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