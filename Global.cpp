#include"Global.h"
#include"Application.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;


static Application application;

Application* Getapplication() {
	return &application;
}

bool Isrunning = true;

int gamemode = 0;

double pai = 3.141592653;