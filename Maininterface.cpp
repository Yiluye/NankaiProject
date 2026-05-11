#include"Interface.h"
#include "Application.h"
#include "Global.h"

void Maininterface::_Onclickstartbutton()
{
	Application* app = Getapplication();
	app->Changeinterface(L"DanmakuGame");
}

void Maininterface::_Onclickexitbutton()
{
	Isrunning = false;
}

Maininterface::Maininterface()
	: Interface(L"Main")
{
	// 开始游戏按钮
	_pstartbutton = new Button(Rect(400, 250, 200, 60), L"Start Game");
	_pstartbutton->Setupstyle(RED)->Sethoverstyle(YELLOW)->Setdownstyle(MAGENTA)
		->Setonclick(_Onclickstartbutton);

	// 退出按钮
	_pexitbutton = new Button(Rect(900, 500, 60, 60), L"Exit");
	_pexitbutton->Setupstyle(RED)->Sethoverstyle(YELLOW)->Setdownstyle(MAGENTA)
		->Setonclick(_Onclickexitbutton);
}
Maininterface::~Maininterface()
{
	delete _pstartbutton;
	delete _pexitbutton;
}

void Maininterface::Update()
{
	_pstartbutton->Update();
	_pexitbutton->Update();
}

void Maininterface::Draw()
{
	putimage(0, 0, &imgMenu);

	RECT rect{ 0, 0, 1000, 200 };
	LOGFONT style;
	setbkmode(TRANSPARENT);
	gettextstyle(&style);
	settextstyle(60, 0, L"Consolas");
	settextcolor(RGB(1, 1, 1));
	drawtext(L"Nankai Project", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	settextstyle(&style);

	_pstartbutton->Draw();
	_pexitbutton->Draw();
}
