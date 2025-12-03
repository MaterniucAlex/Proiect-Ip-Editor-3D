#pragma once
#include "object/CustomObject.h"
#ifndef VARS
#define VARS
#include <SDL3/SDL.h>
#include "extra/utils.h"

SDL_Window   *window;
SDL_Renderer *renderer;
SDL_Event event;
bool isRunning = true;

object exObj;

#endif
