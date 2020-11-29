// Crackingmd5Seq.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <omp.h>
#include <iostream>
#define SIZEP 630188182 
#define MAX_LENGTH 7 
#define CHARSETLEN 26 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"
#include "utils.h"

char charset[] = "abcdefghijklmnopqrstuvwxyz";

void bruteForce(unsigned char* prefixes, int length, int offset_start, int offeset_end, int prefix_to_calc, uint v1, uint v2, uint v3, uint v4, unsigned char* password)
{
    #pragma omp parallel for
    for (int i = 0; i < prefix_to_calc; i++) {
        //Need to optimize 7758650592 
        memcpy(prefixes + (offeset_end + i) * MAX_LENGTH, prefixes + (offset_start + i / 26) * MAX_LENGTH, (length - 1) * sizeof(char));
        prefixes[(offeset_end + i) * MAX_LENGTH + length - 1] = charset[i % 26];
        uint c1 = 0, c2 = 0, c3 = 0, c4 = 0;
        md5_vfy(prefixes + (offeset_end + i) * MAX_LENGTH, length, &c1, &c2, &c3, &c4);
        if (c1 == v1 && c2 == v2 && c3 == v3 && c4 == v4)
        {
            memcpy(password, prefixes + (offeset_end + i) * MAX_LENGTH, length * sizeof(char));
            break;
        }
    }
}

int main(int argc, unsigned char* argv[])
{
    unsigned char* prefixes;

    unsigned char hash[33] = "eba021d91b44a97dec2588bbea58a447";
    unsigned char password[MAX_LENGTH];
    prefixes = (unsigned char*)malloc(SIZEP * sizeof(char) * MAX_LENGTH);
    memset(prefixes, '\0', SIZEP * sizeof(unsigned char) * MAX_LENGTH);



    uint v1, v2, v3, v4;
    md5_to_ints(hash, &v1, &v2, &v3, &v4);
    for (int i = 0; i < CHARSETLEN; i++) prefixes[i * MAX_LENGTH] = charset[i];


    double ms;
    printf("Starting...\n");
    ms = 0;
    for (int h = 0; h < N; h++) {
        memset(password, '\0', sizeof(unsigned char) * MAX_LENGTH);
        long prefix_n = 26;
        int length = 5;
        long offset_start = 0;
        long offest_end = 26;

        start_timer();
        for (int i = 2; i <= length; i++) {
            prefix_n = prefix_n * CHARSETLEN; 
            bruteForce(prefixes, i, offset_start, offest_end, prefix_n, v1, v2, v3, v4, password);
            if (password[0] != '\0')
            {
                printf("We found %s as %s  ", hash, password);
                break;
            }
            offset_start = offest_end;
            offest_end = offset_start + prefix_n;

        }
        ms += stop_timer();
    }
    printf("avg time omp = %.5lf ms\n", (ms / N));
    free(prefixes);
}

