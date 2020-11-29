#pragma once
typedef	unsigned int		uint;
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
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
using namespace tbb;
char charset[] = "abcdefghijklmnopqrstuvwxyz";

class BruteForce
{
private:
	unsigned char* prefixes;
	int length, offset_start, offeset_end, prefix_to_calc;
	uint v1, v2, v3, v4;
	unsigned char* password;
public:
	BruteForce(unsigned char* prefixes, int length, int offset_start, int offeset_end, int prefix_to_calc, uint v1, uint v2, uint v3, uint v4, unsigned char* password) :
		prefixes(prefixes), length(length), offset_start(offset_start), offeset_end(offeset_end), prefix_to_calc(prefix_to_calc), v1(v1), v2(v2), v3(v3), v4(v4), password(password) {}
	void operator() (const blocked_range<int>& r) const {
		for (int i = r.begin(); i != r.end(); i++) {
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
};

