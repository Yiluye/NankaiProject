#include"Interface.h"
#include"messege.h"
#include"Global.h"
DanmakuGameInterface::DanmakuGameInterface()
	: Interface(L"DanmakuGame") 
{

}
DanmakuGameInterface::~DanmakuGameInterface() 
{

}

void DanmakuGameInterface::Onenter() 
{
	circle(500, 100, 100);
}

void DanmakuGameInterface::Onexit()
{

}

void DanmakuGameInterface::Update()
{
	if (Iskeydown(VK_ESCAPE))
	{
		Getapplication()->Setinterface(L"Main");
	}
}

void DanmakuGameInterface::Draw()
{

}