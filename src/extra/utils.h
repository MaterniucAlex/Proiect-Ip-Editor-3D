#ifndef UTILS
#define UTILS

#define bool char
#define true 1
#define false 0

#include <stdio.h>
#include <SDL3/SDL.h>

void *safeInit(void *ptr)
{
	if (ptr == NULL)
		printf("%s", SDL_GetError());
	return ptr;
}

#endif
