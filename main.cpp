#include <Windows.h>
#include <easyx.h>
#include "messege.h"
#include "Button.h"
#include "Global.h"
#include "Application.h"
#include"Interface.h"

void Init();

int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd) {

    Init();
    initgraph(1000, 600);  // ´°¿Ú´óÐ¡ 1000x600
    BeginBatchDraw();

    Application* app = Getapplication();

    while (Isrunning)
    {
        if (Iskeydown(VK_Q))
        {
            break;
        }
        cleardevice();
        Peekmousemessege();
        app->Update();
        app->Draw();

        FlushBatchDraw();
        Sleep(15);
    }


    EndBatchDraw();
    closegraph();
    return 0;
}

void Init()
{
    Application* app = Getapplication();
    app->Registerinterface(new Maininterface())
        ->Registerinterface(new DanmakuGameInterface());
}