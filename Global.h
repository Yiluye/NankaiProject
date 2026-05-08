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
#endif