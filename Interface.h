#pragma once
#ifndef _INTERFACE_H
#define _INTERFACE_H
#include<string>
#include"Button.h"
#include<memory>
#include<vector>
class Interface {
protected:
	std::wstring name;

public:
	Interface(const std::wstring& _name) :name(_name) {}
	virtual ~Interface() {}

	const std::wstring& Name() const { return name; }

	virtual void Onenter() {}
	virtual void Onexit() {}

	virtual void Update() {}
	virtual void Draw() {}

};


class Maininterface :public Interface {
private:
	Button* _pstartbutton;
	Button* _pexitbutton;

	//µã»÷ÊÂ¼þ
	static void _Onclickstartbutton();
	static void _Onclickexitbutton();


public:
	Maininterface();
	~Maininterface();
	void Update();
	void Draw();

};

class DanmakuGameInterface : public Interface {
private:
    int score;
    int enemySpawnCounter;
    bool gameRunning;

    // Éä»÷ÀäÈ´
    int shootCooldown;
    const int SHOOT_DELAY = 8;

public:
    DanmakuGameInterface();
    ~DanmakuGameInterface();

    void Onenter() override;
    void Onexit() override;
    void Update() override;
    void Draw() override;
};

#endif
