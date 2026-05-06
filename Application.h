#pragma once
#ifndef _APPLICATION_H
#define _APPLICATION_H
#include<string>
#include<unordered_map>
#include"interface.h"

//界面管理器
class Application {
private:
	std::unordered_map<std::wstring, Interface*> _interface;
	Interface* _currentinterface;
	Interface* _nextinterface;

public:
	Application();
	~Application();
	Application* Registerinterface(Interface* intf);
	//在一轮更新后改变目标界面
	void Changeinterface(const std::wstring& name);
	//直接改变目标界面
	void Setinterface(const std::wstring& name);

	void Update();
	void Draw();
};
#endif;

