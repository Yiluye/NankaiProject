#include"Button.h"
#include"messege.h"
Button::Button(const Rect& range, const std::wstring& text)
	: _range(range), name(text), _onclick(nullptr)
{
	for (int i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		_color[i] = RED;
	}
}
Button* Button::Setupstyle(COLORREF color) {
	_color[BUTTON_UP] = color;
	return this;
}
Button* Button::Setdownstyle(COLORREF color) {
	_color[BUTTON_DOWN] = color;
	return this;
}
Button* Button::Sethoverstyle(COLORREF color) {
	_color[BUTTON_HOVER] = color;
	return this;
}
Button* Button::Settext(const std::wstring text) {
	name = text;
	return this;
}
Button* Button::Setonclick(void (*onclick)(void)) {
	_onclick = onclick;
	return this;
}
void Button::Update() {
	//主循环中调用了peekmousemessege，这里直接赋值即可
	Mousemessege* msg = Getmousemessege();
	if (_range.Contains(msg->pos))
	{
		if (msg->isdown)
		{
			state = BUTTON_DOWN;
		}
		else if (msg->isup)
		{
			if ((state == BUTTON_DOWN) && _onclick)
			{
				_onclick();
			}
			state = BUTTON_UP;
		}
		else {
			if (state != BUTTON_DOWN)
			{
				state = BUTTON_HOVER;
			}
		}
	}
	else
	{
		state = BUTTON_UP;
	}
}
void Button::Draw() {
	setlinestyle(PS_SOLID, 3);
	setlinecolor(_color[state]);//设置颜色
	RECT rect = _range.Toeasyrect();
	rectangle(rect.left, rect.top, rect.right, rect.bottom);
	drawtext(name.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
