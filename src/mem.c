/*
 * mem.c
 *
 *  Created on: Apr 28, 2019
 *      Author: Jesper
 */
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

void *malloc(size_t xBytes) { return (void *)pvPortMalloc(xBytes); }

void free(void *pvBuffer) { vPortFree(pvBuffer); }

#pragma GCC diagnostic pop

void vApplicationMallocFailedHook(void)
{
    while (1)
        ;
}
