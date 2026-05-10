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

// 定义贴图资源
IMAGE imgSprite;
int iconW = 16;
int iconH = 16;

void LoadAllTextures() {
    // 加载精灵表
    loadimage(&imgSprite, _T("res/etama3.png"), 256, 256);
}