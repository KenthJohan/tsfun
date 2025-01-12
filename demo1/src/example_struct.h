#pragma once
#include <stdint.h>

typedef float real;

typedef struct {
	int x;
	uint32_t y;
	real z;
} Point;

typedef struct {
	Point start;
	Point end;
} Line;