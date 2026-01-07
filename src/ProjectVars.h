#pragma once
#include "SDL3/SDL_render.h"
#include "extra/matrixMath.h"
#include "object/CustomObject.h"
#include <SDL3/SDL_mouse.h>
#ifndef VARS
#define VARS
#include <SDL3/SDL.h>
#include "extra/utils.h"

int SCREEN_W = 800;
int SCREEN_H = 600;
const int FPS = 30;

SDL_Window   *window;
SDL_Renderer *renderer;

bool wireframeRender = false;

float *depthBuffer;
Uint32 *pixels;
SDL_Texture *frameBufferTexture;

vec4 cameraPosition = {0.f, 0.f, -1.f, 1.0f};
vec4 cameraTarget   = {0.f, 0.f, 0.f, 1.0f};
vec4 cameraUp 	    = {0.f, 1.f, 0.f, 1.0f};

mat4 objScalingMat;
mat4 objTranslateMat;
mat4 objRotateMat;

float cameraXangle = 0.5;
float cameraYangle = -0.5;
float cameraZoom = 1;

SDL_Texture *buttonTexture;

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

#include "extra/popupMenu.h"

Button wireframeButton;
Button nextPoint;
Button prevPoint;

Button pointPlusX;
Button pointMinusX;
Button pointPlusY;
Button pointMinusY;
Button pointPlusZ;
Button pointMinusZ;

Button rotateObjButton;
Button translateObjButton;
Button scaleObjButton;

Button loadObjButton;
Button saveObjButton;

Button toggleColorMenu;
Button toggleScaleMenu;
Button toggleTranslateMenu;
Button toggleRotateMenu;

point *selectedPoint;

PopupMenu colorMenu;
PopupMenu scaleMenu;
PopupMenu rotateMenu;
PopupMenu translateMenu;

int pointX;
int pointY;

#endif
