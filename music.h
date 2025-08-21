//
// Created by Administrator on 2023/1/17.
//

#ifndef TEST_MUSIC_H
#define TEST_MUSIC_H
#define UNICODE
#define _UNICODE
#include "musicInfo.h"
#include <Windows.h>
#include "bassflac.h"
#include <shlobj.h>
#include <tchar.h>




#ifdef UNICODE
#define BASS_FLAG (BASS_SAMPLE_FLOAT|BASS_UNICODE)
#define Main wWinMain
#endif
#ifndef UNICODE
#define BASS_FLAG BASS_SAMPLE_FLOAT
#define Main WinMain
#endif



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL findMusicFile();

void listAddMusicName();

void CALLBACK endCallback(HSYNC handle, DWORD channel, DWORD data, void *pTarget);
void WINAPI SetProgressThread(HWND pHWND__, UINT i, UINT_PTR i1, DWORD i2);
#endif //TEST_MUSIC_H
