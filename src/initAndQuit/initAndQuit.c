#include "stdlib.h"
#include "../ProjectVars.h"
#include "../update/update.c"
#include "../extra/matrixMath.h"
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
void init()
{
	SDL_Init(SDL_INIT_VIDEO);
	window   = safeInit(SDL_CreateWindow("Editor 3D", SCREEN_W, SCREEN_H, 0));
	renderer = safeInit(SDL_CreateRenderer(window, NULL));

	depthBuffer 	= malloc(sizeof(float)  * (SCREEN_W * SCREEN_H));
	pixels = (Uint32 *)malloc(SCREEN_W * SCREEN_H * sizeof(Uint32));
	frameBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888, // Standard 32-bit format (Alpha, Red, Green, Blue)
		SDL_TEXTUREACCESS_STREAMING, // Crucial: Allows frequent locking/updating
		SCREEN_W,
		SCREEN_H
	);

	for(int i = 0; i < SCREEN_W; i++)
	{
		for (int j = 0; j < SCREEN_H; j++)
		{
			depthBuffer[j * SCREEN_W + i] = 99999;
		}
	}

	initEmptyObject(&exObj);

	//BUTTONS
	spinCamLeft.buttonRect = (SDL_FRect){10, 10, 100, 50};
	spinCamLeft.action = rotateCameraLeft;
	spinCamRight.buttonRect = (SDL_FRect){10, 70, 100, 50};
	spinCamRight.action = rotateCameraRight;
	spinCamUp.buttonRect = (SDL_FRect){690, 10, 100, 50};
	spinCamUp.action = rotateCameraUp;
	spinCamDown.buttonRect = (SDL_FRect){690, 70, 100, 50};
	spinCamDown.action = rotateCameraDown;

	wireframeButton.buttonRect = (SDL_FRect){SCREEN_W / 2.f - 50, 10, 100, 50};
	wireframeButton.action = toggleWireframe;

	addPointToObject(&exObj, (point){{-0.2, -0.2, -0.2}, {255, 0  , 0  , 255}}); //BBL
	addPointToObject(&exObj, (point){{ 0.2, -0.2, -0.2}, {255, 0  , 0  , 255}}); //BBR
	addPointToObject(&exObj, (point){{-0.2,  0.2, -0.2}, {255, 0  , 0  , 255}}); //BTL
	addPointToObject(&exObj, (point){{ 0.2,  0.2, -0.2}, {255, 0  , 0  , 255}}); //BTR
	
	addPointToObject(&exObj, (point){{-0.2, -0.2, 0.2}, {0  , 0  , 255, 255}}); //BBL
	addPointToObject(&exObj, (point){{ 0.2, -0.2, 0.2}, {0  , 0  , 255, 255}}); //BBR
	addPointToObject(&exObj, (point){{-0.2,  0.2, 0.2}, {0  , 0  , 255, 255}}); //BTL
	addPointToObject(&exObj, (point){{ 0.2,  0.2, 0.2}, {0  , 0  , 255, 255}}); //BTR

	createTriangleFromPointIds(&exObj, 0, 1, 2);
	createTriangleFromPointIds(&exObj, 1, 2, 3);

	createTriangleFromPointIds(&exObj, 4, 5, 6);
	createTriangleFromPointIds(&exObj, 5, 6, 7);

	createTriangleFromPointIds(&exObj, 1, 3, 5);
	createTriangleFromPointIds(&exObj, 3, 5, 7);

	createTriangleFromPointIds(&exObj, 0, 2, 4);
	createTriangleFromPointIds(&exObj, 2, 4, 6);

	initEmptyObject(&levelFloor);
	addPointToObject(&levelFloor, (point){{-1.0, 0.5, -1.0}, {255, 0  , 0  , 255}}); //BBL
	addPointToObject(&levelFloor, (point){{ 1.0, 0.5, -1.0}, {0  , 0  , 255, 255}}); //BBR
	addPointToObject(&levelFloor, (point){{-1.0, 0.5,  1.0}, {0  , 255, 0  , 255}}); //BTL
	addPointToObject(&levelFloor, (point){{ 1.0, 0.5,  1.0}, {255, 255, 255, 255}}); //BTR

	createTriangleFromPointIds(&levelFloor, 0, 1, 2);
	createTriangleFromPointIds(&levelFloor, 1, 2, 3);

	pointerCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
	normalCursor  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);

}

void quit()
{
	free(pixels);
	free(depthBuffer);
	SDL_DestroyCursor(normalCursor);
	SDL_DestroyCursor(pointerCursor);
	SDL_DestroyTexture(frameBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
