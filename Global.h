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
extern int zhenpermiao;

extern int gametime;   // 游戏全局时间

extern int totalhp;

// 声明贴图资源
extern IMAGE imgSprite;        // 精灵表（包含所有弹幕）
extern int iconW, iconH;       // 每个弹幕的宽高（16×16）

// 加载所有贴图
void LoadAllTextures();


extern IMAGE imgMenu;   // 主界面背景
extern IMAGE imgGame;   // 游戏界面背景

extern IMAGE imgBoss;   // Boss 图片
extern IMAGE imgEnemy;   // 小怪图片
extern IMAGE imgPlayer;  //玩家图片

extern IMAGE imgPowerRed;   // 红色 P 点
extern IMAGE imgPowerBlue;  // 蓝色 P 点

// 全局区域定义
extern const int GAME_AREA_WIDTH;   // 游戏区域宽度（750）
extern const int UI_AREA_WIDTH;     // UI 区域宽度（250）
extern const int GAME_AREA_LEFT;    // 游戏区域左边界（0）
extern const int GAME_AREA_RIGHT;   // 游戏区域右边界（750）
extern IMAGE imgPlayerBullet;   // 玩家子弹精灵表
#endif