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