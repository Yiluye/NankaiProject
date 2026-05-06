#pragma once
#ifndef _BUTTON_H
#define _BUTTON_H
#include<easyx.h>
#include<string>
#include"Defines.h"
enum Buttonstate {
	BUTTON_UP = 0,
	BUTTON_DOWN,
	BUTTON_HOVER,
	BUTTON_STATE_NUMBER
};
class Button {
private:
	Rect _range;
	Buttonstate state;
	COLORREF _color[BUTTON_STATE_NUMBER];
	std::wstring name;
	void (*_onclick)(void);//点击状态
public:
	Button(const Rect& range, const std::wstring& text);
	virtual ~Button() = default;

	//后期可以考虑换成图片丰富效果
	//设置成指针事件实现调用的串联
	Button* Setupstyle(COLORREF color);
	Button* Setdownstyle(COLORREF color);
	Button* Sethoverstyle(COLORREF color);
	Button* Settext(const std::wstring);

	//点击事件
	Button* Setonclick(void (*onclick)(void));

	virtual void Update();
	virtual void Draw();
};


#endif // !_BUTTON_H
