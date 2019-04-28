/*
 * mem.c
 *
 *  Created on: Apr 28, 2019
 *      Author: Jesper
 */
#include <stdlib.h>

void *malloc(size_t xBytes) { return pvPortMalloc(xBytes); }

void free(void *pvBuffer) { vPortFree(pvBuffer); }
