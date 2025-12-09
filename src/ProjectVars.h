#pragma once
#include "extra/matrixMath.h"
#include "extra/button.h"
#include "object/CustomObject.h"
#ifndef VARS
#define VARS
#include <SDL3/SDL.h>
#include "extra/utils.h"

const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int FPS = 30;

SDL_Window   *window;
SDL_Renderer *renderer;

float *depthBuffer;
typedef struct Screen 
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} Screen;
Screen *screen;

vec4 cameraPosition = {0.f, 0.f, -1.f};
vec4 cameraTarget   = {0.f, 0.f, 0.f};
vec4 cameraUp 	    = {0.f, 1.f, 0.f};

float cameraAngle = 0;

Button spinCamLeft;
Button spinCamRight;

SDL_Event event;
bool isRunning = true;

object exObj;

#endif
