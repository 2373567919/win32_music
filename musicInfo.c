//
// Created by Administrator on 2023/1/16.
//
#include "musicInfo.h"

int initList(ArrayList list) {
    list->capacity = 10;
    list->array = malloc(sizeof(E) * list->capacity);
    list->size = 0;
    return list->array != NULL ? 1 : 0;
}

void add(ArrayList list, E element) {
    if (list->size == list->capacity) {
        int newCapacity = list->capacity + (list->capacity >> 1);
        E *newArray = realloc(list->array, newCapacity * sizeof(E));
        list->array = newArray;
        list->capacity = newCapacity;
    }

    list->array[list->size] = element;
    list->size++;
}
