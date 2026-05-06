#include"Global.h"
#include"Application.h"

static Application application;

Application* Getapplication() {
	return &application;
}

bool Isrunning = true;