#include"Global.h"
#include"Application.h"

static Application application;

Application* Getapplication() {
	return &application;
}

bool Isrunning = true;

int gamemode = 0;

double pai = 3.141592653;
int zhenpermiao = 60;

int gametime = 0;

// ¶¨ÒåÌùÍ¼×ÊÔ´
IMAGE imgSprite;
int iconW = 16;
int iconH = 16;

IMAGE imgMenu;
IMAGE imgGame;

IMAGE imgBoss;

void LoadAllTextures() {
    // µ¯Ä»Í¼ 
    loadimage(&imgSprite, _T("res/etama3.png"), 256, 256);

    //±³¾°Í¼
    loadimage(&imgMenu, _T("res/menu_bg.jpg"), SCREEN_WIDTH, SCREEN_HEIGHT);
    //loadimage(&imgGame, _T("res/game_bg.jpg"), SCREEN_WIDTH, SCREEN_HEIGHT);

    loadimage(&imgBoss, _T("res/boss.png"), 66,150);
}

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 850;

const int GAME_AREA_WIDTH = 750;
const int UI_AREA_WIDTH = 250;
const int GAME_AREA_LEFT = 0;
const int GAME_AREA_RIGHT = GAME_AREA_WIDTH;  // 750