#pragma once
#include <stdint.h>

typedef float real;

typedef struct {
	int x;
	uint32_t * y;
	real z[3];
} Point;

typedef struct {
	Point start;
	Point end;
} Line;