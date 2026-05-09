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
	setfillcolor(camp == Camp::PLAYER ? GREEN : RED);

	if (shape == BulletShape::CIRCLE) {
		fillcircle((int)pos.x, (int)pos.y, radius);
	}
	else { // RECT
		// 计算未旋转的四个顶点（中心在 (0,0)，半宽半高）
		double hw = width / 2.0;
		double hh = height / 2.0;
		POINT pts[4] = {
			{ (int)(-hw), (int)(-hh) },
			{ (int)(hw), (int)(-hh) },
			{ (int)(hw), (int)(hh) },
			{ (int)(-hw), (int)(hh) }
		};
		// 旋转并平移到世界坐标
		POINT worldPts[4];
		for (int i = 0; i < 4; ++i) {
			worldPts[i] = RotatePoint(0, 0, pts[i].x, pts[i].y, rotation);
			worldPts[i].x += (int)pos.x;
			worldPts[i].y += (int)pos.y;
		}
		setfillcolor(camp == Camp::PLAYER ? GREEN : RED);
		setlinecolor(WHITE); // 可选边框
		polygon(worldPts, 4);
		// 如果需要填充内部，需用 floodfill 或直接设置填充色，但 polygon 默认只画边框？ EasyX 中 polygon 可填充。
		// 正确用法：先 setfillcolor，然后 polygon 会自动填充。上面的代码只是画了边框。修正：
		// 改用 solidpolygon? 实际上 polygon 既画边又填充，但需要先设置填充色和线条色。
		// 为了清晰，我们直接调用 fillpolygon
		setfillcolor(camp == Camp::PLAYER ? GREEN : RED);
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