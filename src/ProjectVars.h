#pragma once
#include "object/CustomObject.h"
#ifndef VARS
#define VARS
#include <SDL3/SDL.h>
#include "extra/utils.h"

const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int FPS = 60;

SDL_Window   *window;
SDL_Renderer *renderer;
SDL_Event event;
bool isRunning = true;

object exObj;

#endif
