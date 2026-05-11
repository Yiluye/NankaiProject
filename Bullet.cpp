#include"Bullet.h"
#include"Global.h"

#include <windows.h>      
#pragma comment(lib, "Msimg32.lib") 

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
			// 玩家子弹（从 player_bullet.png 截取）
			TransparentBlt(GetImageHDC(NULL), dstX, dstY, drawSize, drawSize,
				GetImageHDC(&imgPlayerBullet), 128, 16, 16, 16,
				RGB(0, 0, 0));
		}
		else {
			// 敌方子弹（从 imgSprite 截取）
			int srcX = 32, srcY = 32;
			if (color == BTRED) { srcX = 32; srcY = 32; }
			else if (color == BTBLUE) { srcX = 80; srcY = 48; }
			else if (color == BTCYAN) { srcX = 112; srcY = 32; }
			else if (color == BTYELLOW) { srcX = 192; srcY = 32; }
			else if (color == BTPINK) { srcX = 64; srcY = 32; }

			TransparentBlt(GetImageHDC(NULL), dstX, dstY, drawSize, drawSize,
				GetImageHDC(&imgSprite), srcX, srcY, 16, 16,
				RGB(67,54,54));
		}
	}
	else if (shape == BulletShape::RECT) {
		// 梭形（旋转矩形）保持不变
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
		setfillcolor(YELLOW);
		setlinecolor(YELLOW);
		fillpolygon(worldPts, 4);
	}
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