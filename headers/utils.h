#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef HEADER
	#include "headers/pgmp2.h"
#endif

#define MAX_LINE_LENGTH 40

pgmp2_t load(char*);

void store(pgmp2_t);