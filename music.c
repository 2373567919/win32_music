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
    // ������·���������ļ�
    hMusicStream = BASS_StreamCreateFile(FALSE, list.array[p].path, 0, 0, BASS_FLAG);
    BASS_ChannelSetSync(hMusicStream, BASS_SYNC_END, 0, endCallback, 0);
    SetWindowText(hMusicText, list.array[p].name);
    length = BASS_ChannelBytes2Seconds(hMusicStream, BASS_ChannelGetLength(hMusicStream, BASS_POS_BYTE));
    position = 0;
    SendMessage(hProgress, TBM_SETRANGE, TRUE, MAKELPARAM(0, length));
}

void CALLBACK endCallback(HSYNC handle, DWORD channel, DWORD data, void *pTarget) {
    p++;
    if (p == ListView_GetItemCount(hListCtrl)) //��������һ��,����һ���ǵ�һ��
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
            hBrush = CreateSolidBrush(RGB(255, 255, 255)); //����һ����ˢ
            BASS_Init(-1, 44100, BASS_DEVICE_CPSPEAKERS, hWnd, 0);
            HFONT hFont1 = CreateFont(
                20, 0, //�߶�40, ��ȡ0��ʾ��ϵͳѡ�����ֵ
                0, 0, //�ı���б����������б��Ϊ0
                FW_NORMAL,
                0, 0, 0, //б�壬�»��ߣ����л���
                DEFAULT_CHARSET, //�ַ���
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, //һϵ�е�Ĭ��ֵ
                DEFAULT_PITCH | FF_DONTCARE,
                _T("����") //��������
            );
            HFONT hFont2 = CreateFont(
                20, 0, //�߶�40, ��ȡ0��ʾ��ϵͳѡ�����ֵ
                0, 0, //�ı���б����������б��Ϊ0
                FW_NORMAL,
                0, 0, 0, //б�壬�»��ߣ����л���
                DEFAULT_CHARSET, //�ַ���
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, //һϵ�е�Ĭ��ֵ
                DEFAULT_PITCH | FF_DONTCARE,
                _T("΢���ź�") //��������
            );
        //����������
            hProgress = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE,
                                     2, 52, 480, 30, hWnd, (HMENU) IDC_PROGRESS, NULL, NULL);
            SendMessage(hProgress, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
            SendMessage(hProgress, TBM_SETPAGESIZE, 0, 1);
            hMusicText = CreateWindow(_T("STATIC"), _T("����"), SS_ENDELLIPSIS | WS_VISIBLE | WS_CHILD, 2, 17, 380, 35,
                                      hWnd, (HMENU) 2313, NULL, NULL);
            hTimeText = CreateWindow(_T("STATIC"), _T("00:00/00:00"), SS_ENDELLIPSIS | WS_VISIBLE | WS_CHILD, 390 + 2,
                                     17, 150, 35, hWnd, (HMENU) 2314, NULL, NULL);
        //���ö�ʱ��
            SetTimer(NULL, 0, 200, SetProgressThread);
        //������ʾ�б�
            hListBox = CreateWindow(_T("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
                                    2, 86, 480, 690, hWnd,
                                    (HMENU) IDC_LIST_1, NULL, NULL);
        //�����б���ʾ����
            SendMessage(hListBox, WM_SETFONT, (WPARAM) hFont2, TRUE);
            SendMessage(hMusicText, WM_SETFONT, (WPARAM) hFont1, TRUE);
            SendMessage(hTimeText, WM_SETFONT, (WPARAM) hFont2, TRUE);
        //��ȡ���ֲ����б���Ϣ
            findMusicFile();
        //�����ֲ�����Ϣ��ӵ��б���
            listAddMusicName();
            break;
        case WM_COMMAND:

            if (LOWORD(wParam) == IDC_LIST_1 && HIWORD(wParam) == LBN_DBLCLK) {
                p = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                open();
                BASS_ChannelPlay(hMusicStream, FALSE); //���������ļ�
            }
            break;
        case WM_HSCROLL:
            position = SendMessage(hProgress, TBM_GETPOS, 0, 0);
            BASS_ChannelSetPosition(hMusicStream, BASS_ChannelSeconds2Bytes(hMusicStream, position), BASS_POS_BYTE);
            break;
        case WM_CTLCOLORSTATIC:
            HDC hdc = (HDC) wParam; //���������Ĳ���ת����ȷ�����ͣ�������ʹ��
            SetTextColor(hdc, RGB(0, 0, 0)); //����������ɫ
            SetBkColor(hdc, RGB(255, 255, 255)); //�������ֱ�����ɫ
            return (LRESULT) hBrush; //���ػ�ˢ�������޸�static�ؼ���ɫ
        case WM_DESTROY:
            //�������˳���Ϣ
            BASS_ChannelFree(hMusicStream);
            PostQuitMessage(0);
            break;
        default: //�����������Ĭ�Ϻ�������
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
