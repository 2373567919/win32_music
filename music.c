//
// Created by Administrator on 2023/1/17.
//

#include "music.h"

HSTREAM hMusicStream;
List list;
int p;
DWORD IDC_LIST_1 = 101;
DWORD IDC_PROGRESS = 201;
HWND hListCtrl;
HWND hListBox;
HWND hProgress;
HWND hMusicText;
HWND hTimeText;
int length = 0;
int position = 0;

void open() {
    if (0!=hMusicStream) {
        BASS_ChannelStop(hMusicStream);
        BASS_ChannelFree(hMusicStream);
    }
    // 打开音乐路径的音乐文件
    hMusicStream = BASS_StreamCreateFile(FALSE, list.array[p].path, 0, 0, BASS_FLAG);
    BASS_ChannelSetSync(hMusicStream, BASS_SYNC_END, 0, endCallback, 0);
    SetWindowText(hMusicText, list.array[p].name);
    length = BASS_ChannelBytes2Seconds(hMusicStream, BASS_ChannelGetLength(hMusicStream, BASS_POS_BYTE));
    position = 0;
    SendMessage(hProgress, TBM_SETRANGE, TRUE, MAKELPARAM(0, length));
}

void CALLBACK endCallback(HSYNC handle, DWORD channel, DWORD data, void *pTarget) {
    p++;
    if (p == ListView_GetItemCount(hListCtrl)) //如果是最后一首,则下一曲是第一首
        p = 0;
    SendMessage(hListBox, LB_SETCURSEL, p, 0);

    open();

    BASS_ChannelPlay(hMusicStream, FALSE
    );
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HBRUSH hBrush;
    switch (message) {
        case WM_CREATE:
            hBrush = CreateSolidBrush(RGB(255, 255, 255)); //创建一个画刷
            BASS_Init(-1, 44100, BASS_DEVICE_CPSPEAKERS, hWnd, 0);
            HFONT hFont1 = CreateFont(
                20, 0, //高度40, 宽取0表示由系统选择最佳值
                0, 0, //文本倾斜，与字体倾斜都为0
                FW_NORMAL,
                0, 0, 0, //斜体，下划线，无中划线
                DEFAULT_CHARSET, //字符集
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, //一系列的默认值
                DEFAULT_PITCH | FF_DONTCARE,
                _T("楷体") //字体名称
            );
            HFONT hFont2 = CreateFont(
                20, 0, //高度40, 宽取0表示由系统选择最佳值
                0, 0, //文本倾斜，与字体倾斜都为0
                FW_NORMAL,
                0, 0, 0, //斜体，下划线，无中划线
                DEFAULT_CHARSET, //字符集
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, //一系列的默认值
                DEFAULT_PITCH | FF_DONTCARE,
                _T("微软雅黑") //字体名称
            );
        //创建进度条
            hProgress = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE,
                                     2, 52, 480, 30, hWnd, (HMENU) IDC_PROGRESS, NULL, NULL);
            SendMessage(hProgress, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
            SendMessage(hProgress, TBM_SETPAGESIZE, 0, 1);
            hMusicText = CreateWindow(_T("STATIC"), _T("音乐"), SS_ENDELLIPSIS | WS_VISIBLE | WS_CHILD, 2, 17, 380, 35,
                                      hWnd, (HMENU) 2313, NULL, NULL);
            hTimeText = CreateWindow(_T("STATIC"), _T("00:00/00:00"), SS_ENDELLIPSIS | WS_VISIBLE | WS_CHILD, 390 + 2,
                                     17, 150, 35, hWnd, (HMENU) 2314, NULL, NULL);
        //设置定时器
            SetTimer(NULL, 0, 200, SetProgressThread);
        //创建显示列表
            hListBox = CreateWindow(_T("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
                                    2, 86, 480, 690, hWnd,
                                    (HMENU) IDC_LIST_1, NULL, NULL);
        //设置列表显示字体
            SendMessage(hListBox, WM_SETFONT, (WPARAM) hFont2, TRUE);
            SendMessage(hMusicText, WM_SETFONT, (WPARAM) hFont1, TRUE);
            SendMessage(hTimeText, WM_SETFONT, (WPARAM) hFont2, TRUE);
        //获取音乐播放列表信息
            findMusicFile();
        //将音乐播放信息添加到列表中
            listAddMusicName();
            break;
        case WM_COMMAND:

            if (LOWORD(wParam) == IDC_LIST_1 && HIWORD(wParam) == LBN_DBLCLK) {
                p = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                open();
                BASS_ChannelPlay(hMusicStream, FALSE); //播放音乐文件
            }
            break;
        case WM_HSCROLL:
            position = SendMessage(hProgress, TBM_GETPOS, 0, 0);
            BASS_ChannelSetPosition(hMusicStream, BASS_ChannelSeconds2Bytes(hMusicStream, position), BASS_POS_BYTE);
            break;
        case WM_CTLCOLORSTATIC:
            HDC hdc = (HDC) wParam; //将传过来的参数转成正确的类型，供后面使用
            SetTextColor(hdc, RGB(0, 0, 0)); //设置文字颜色
            SetBkColor(hdc, RGB(255, 255, 255)); //设置文字背景颜色
            return (LRESULT) hBrush; //返回画刷，用于修改static控件颜色
        case WM_DESTROY:
            //处理窗口退出消息
            BASS_ChannelFree(hMusicStream);
            PostQuitMessage(0);
            break;
        default: //其他情况调用默认函数处理
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void findFile(const TCHAR *folderPath) {
    WIN32_FIND_DATA findData;
    HANDLE hFind = NULL;
    TCHAR filePath[MAX_PATH];
    wsprintf(filePath, TEXT("%s\\*"), folderPath);
    hFind = FindFirstFile(filePath, &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (_tcscmp(findData.cFileName, TEXT(".")) != 0 && _tcscmp(findData.cFileName, TEXT("..")) != 0) {
                if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    wsprintf(filePath, TEXT("%s\\%s"), folderPath, findData.cFileName);
                    findFile(filePath);
                } else {
                    TCHAR ext[10];
                    TCHAR *pFilename = malloc(sizeof(TCHAR) * MAX_PATH);
                    _tsplitpath_s(findData.cFileName, NULL, 0, NULL, 0, pFilename, MAX_PATH, ext, 10);
                    if (!(_tcscmp(ext, TEXT(".mp3")) && _tcscmp(ext, TEXT(".wav")) && _tcscmp(ext, TEXT(".ogg")) &&
                          _tcscmp(ext, TEXT(".flac")) && _tcscmp(ext, TEXT(".m4a")))) {
                        TCHAR *pPath = malloc(sizeof(TCHAR) * MAX_PATH);
                        wsprintf(pPath, TEXT("%s\\%s"), folderPath, findData.cFileName);
                        E e;
                        e.path = pPath;
                        e.name = pFilename;
                        add(&list, e);
                    }
                }
            }
        } while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }
}
BOOL findMusicFile() {
    TCHAR lpSzPath[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, lpSzPath);
    wsprintf(lpSzPath, TEXT("%s\\Music"), lpSzPath);
    initList(&list);
    findFile(lpSzPath);
    return TRUE;
}

void listAddMusicName() {
    for (int i = 0; i < list.size; ++i) {
        TCHAR temp[MAX_PATH];
        wsprintf(temp, TEXT("%d  %s"), i + 1, list.array[i].name);
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM) temp);
    }
}

void WINAPI SetProgressThread(HWND pHWND__, UINT i, UINT_PTR i1, DWORD i2) {
    int pos = BASS_ChannelBytes2Seconds(hMusicStream, BASS_ChannelGetPosition(hMusicStream, BASS_POS_BYTE));
    if (pos <= position) {
        return;
    }
    position = pos;
    TCHAR timeTextStr[12];
    int posM = 0;
    int posS = 0;
    int lenM = 0;
    int lenS = 0;
    if (position > 0) {
        posM = position / 60;
        posS = position % 60;
        lenM = length / 60;
        lenS = length % 60;
    }
    wsprintf(timeTextStr, _T("%02d:%02d/%02d:%02d"), posM, posS, lenM, lenS);
    SetWindowText(hTimeText, timeTextStr);
    SendMessage(hProgress, TBM_SETPOS, TRUE, position);
}
