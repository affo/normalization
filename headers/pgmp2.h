#include <malloc.h>

typedef struct pgmp2 {
	int min;
	int max;
	int width;
	int height;
	int* pixels;
} pgmp2_t;

pgmp2_t init(int, int);

int get(pgmp2_t, int);

void set(pgmp2_t, int, int);