#ifndef _PGMP2_H
#define _PGMP2_H
#include <malloc.h>

typedef struct pgmp2 {
	int min;
	int max;
	int width;
	int height;
	int* pixels;
} pgmp2_t;

extern pgmp2_t init(int width, int height);

extern int get(pgmp2_t img, int i);

extern void set(pgmp2_t img, int i, int value);

#endif