#pragma once
// =================================================================
//
// File: utils.h
// Author: Pedro Perez
// Description: This file contains the implementation of the
//				functions used to take the time and perform the
//				speed up calculation; as well as functions for
//				initializing integer arrays.
//
// Copyright (c) 2020 by Tecnologico de Monterrey.
// All Rights Reserved. May be reproduced for any non-commercial
// purpose.
//
// =================================================================

#ifndef UTILS_H
#define UTILS_H
#define WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

// MSVC defines this in winsock2.h!?
typedef struct timeval {
    long tv_sec;
    long tv_usec;
} timeval;

int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    time = ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec = (long)((time - EPOCH) / 10000000L);
    tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
    return 0;
}

#define MIN_VAL(a,b) (((a)<(b))?(a):(b))
#define MAX_VAL(a,b) (((a)>(b))?(a):(b))

#define N 			10
#define DISPLAY		100
#define TOP_VALUE	10000

typedef enum color { BLUE, GREEN, RED } Color;

struct timeval startTime, stopTime;
int started = 0;

void start_timer() {
    started = 1;
    gettimeofday(&startTime, NULL);
}

double stop_timer() {
    long seconds, useconds;
    double duration = -1;

    if (started) {
        gettimeofday(&stopTime, NULL);
        seconds = stopTime.tv_sec - startTime.tv_sec;
        useconds = stopTime.tv_usec - startTime.tv_usec;
        duration = (seconds * 1000.0) + (useconds / 1000.0);
        started = 0;
    }
    return duration;
}
void random_array(int* array, int size) {
    int i;

    srand(time(0));
    for (i = 0; i < size; i++) {
        array[i] = (rand() % 100) + 1;
    }
}

#endif
