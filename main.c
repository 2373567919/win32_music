

#include "music.h"

#define APP_NAME TEXT("MusicWindow")
#define WIN_TITLE TEXT("音乐播放器")

int WINAPI Main(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPreInstance, _In_ PTSTR  szCmdLine, _In_  int iCmdShow) {
    HWND hWnd;
    MSG msg;
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = APP_NAME;

    //注册窗口类
    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("窗口类注册失败"), APP_NAME, MB_ICONERROR);
        return 0;
    }
    hWnd = CreateWindow(APP_NAME,//窗口类名
                        WIN_TITLE,//窗口标题
                        WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU, //窗口样式
                        CW_USEDEFAULT,//窗口x位置
                        CW_USEDEFAULT,//窗口y位置
                        500,//窗口宽度
                        808,//窗口高度
                        NULL,//父窗口的句柄
                        NULL,//窗口菜单句柄
                        hInstance,
                        NULL);
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}
