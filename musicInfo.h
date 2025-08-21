//
// Created by Administrator on 2023/1/16.
//


#ifndef TEST_MUSIC_INFO_H
#define TEST_MUSIC_INFO_H
#define UNICODE
#define _UNICODE
#include <tchar.h>
#include <Windows.h>
struct MusicInfo {
    TCHAR *path;
    TCHAR *name;
};
typedef struct MusicInfo E;

struct List {
    E *array;
    int capacity;
    int size;
};
typedef struct List List;

typedef struct List *ArrayList;

int initList(ArrayList list);

void add(ArrayList list, E element);

#endif //TEST_MUSIC_INFO_H

