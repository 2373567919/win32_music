

#include "music.h"

#define APP_NAME TEXT("MusicWindow")
#define WIN_TITLE TEXT("���ֲ�����")

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

    //ע�ᴰ����
    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("������ע��ʧ��"), APP_NAME, MB_ICONERROR);
        return 0;
    }
    hWnd = CreateWindow(APP_NAME,//��������
                        WIN_TITLE,//���ڱ���
                        WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU, //������ʽ
                        CW_USEDEFAULT,//����xλ��
                        CW_USEDEFAULT,//����yλ��
                        500,//���ڿ��
                        808,//���ڸ߶�
                        NULL,//�����ڵľ��
                        NULL,//���ڲ˵����
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
