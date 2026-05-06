#include"Defines.h"
#include<easyx.h>
#include"messege.h"
//static保证mouse只在当前文件中可见
//设置mouse来接受鼠标消息
//Peekmousemessege用来接受消息
//Getmousemessege用来输出鼠标状态
static Mousemessege mouse;
void Peekmousemessege()
{
	ExMessage msg;
	mouse.isdown = false;
	mouse.isup = false;
	while (peekmessage(&msg, EX_MOUSE)) {
		if (msg.message == WM_MOUSEMOVE) {
			mouse.pos.x = msg.x;
			mouse.pos.y = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP) {
			mouse.isup = true;
			mouse.isdown = false;
		}
		else if (msg.message == WM_LBUTTONDOWN) {
			mouse.isup = false;
			mouse.isdown = true;
		}
	}
}
//找ai问的
bool Iskeydown(int key) {
	return (GetAsyncKeyState(key) & 0x8000);
}

Mousemessege* Getmousemessege() {
	return &mouse;
}