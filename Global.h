#pragma once
#pragma once
#ifndef _GLOBAL_H
#define _GLOBAL_H
#include"Application.h"

//定义画布大小
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;


class Application;

Application* Getapplication();

extern bool Isrunning;

extern int gamemode;

extern double pai;


// 声明贴图资源
extern IMAGE imgSprite;        // 精灵表（包含所有弹幕）
extern int iconW, iconH;       // 每个弹幕的宽高（16×16）

// 加载所有贴图
void LoadAllTextures();
#endif