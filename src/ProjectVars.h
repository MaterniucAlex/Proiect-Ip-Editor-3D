#pragma once
#include "extra/matrixMath.h"
#include "object/CustomObject.h"
#include <SDL3/SDL_mouse.h>
#ifndef VARS
#define VARS
#include <SDL3/SDL.h>
#include "extra/utils.h"

const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int FPS = 30;

SDL_Window   *window;
SDL_Renderer *renderer;

bool wireframeRender = false;

float *depthBuffer;
Uint32 *pixels;
SDL_Texture *frameBufferTexture;

vec4 cameraPosition = {0.f, 0.f, -1.f};
vec4 cameraTarget   = {0.f, 0.f, 0.f};
vec4 cameraUp 	    = {0.f, 1.f, 0.f};

float cameraXangle = 0.5;
float cameraYangle = -0.5;

float mouseX = 0, mouseY = 0;
float camDragStartX = 0, camDragStartY = 0;
float camWhenDragStartX = 0, camWhenDragStartY = 0;

bool mouseHold = false;
bool mouseClicked= false;
SDL_Cursor *pointerCursor;
SDL_Cursor *normalCursor;

SDL_Event event;
bool isRunning = true;

object exObj;
object levelFloor;

#include "extra/button.h"

Button wireframeButton;
Button nextPoint;
Button prevPoint;

Button pointPlusX;

point *selectedPoint;
int pointX;
int pointY;

#endif
