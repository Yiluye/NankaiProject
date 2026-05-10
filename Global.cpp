#include"Global.h"
#include"Application.h"

const int SCREEN_WIDTH = 750;
const int SCREEN_HEIGHT = 850;


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