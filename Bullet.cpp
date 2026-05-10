#include"Bullet.h"
#include"Global.h"
Bullet::Bullet(double x, double y, double vx, double vy, int r, Camp c)
	: Gameobject(r * 2, r * 2),           // 基类宽高 = 直径
	velocity{ vx, vy },
	radius(r),
	width(r * 2),
	height(r * 2),
	active(true),
	camp(c),
	shape(BulletShape::CIRCLE) {
	pos.x = x;
	pos.y = y;
}

Bullet::Bullet(double x, double y, double vx, double vy, int r, Camp c, int sx, int sy)
	: Gameobject(r * 2, r * 2),
	velocity{ vx, vy },
	radius(r),
	active(true),
	camp(c),
	shape(BulletShape::SPRITE),
	srcX(sx),
	srcY(sy) {
	pos.x = x;
	pos.y = y;
}

Bullet::Bullet(double x, double y, double vx, double vy, int w, int h, Camp c)
	: Gameobject(w, h),                    // 基类宽高为实际宽高
	velocity{ vx, vy },
	radius(((w > h ? w : h) + 1) / 2),    // 碰撞半径取半长轴（向上取整）
	width(w),
	height(h),
	active(true),
	camp(c),
	shape(BulletShape::RECT) {
	pos.x = x;
	pos.y = y;
}

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
	if (pos.x + radius < 0 || pos.x - radius > SCREEN_WIDTH ||
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
		// 使用贴图（从精灵表中截取）
		int drawSize = radius * 2;   // 绘制尺寸 = 直径
		int dstX = (int)(pos.x - drawSize / 2);
		int dstY = (int)(pos.y - drawSize / 2);

		// 从精灵表中截取指定区域并绘制（球形弹幕位置：srcX=32, srcY=48）
		putimage(dstX, dstY, drawSize, drawSize, &imgSprite, 32, 0, SRCCOPY);
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
		setfillcolor(camp == Camp::PLAYER ? GREEN : RED);
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