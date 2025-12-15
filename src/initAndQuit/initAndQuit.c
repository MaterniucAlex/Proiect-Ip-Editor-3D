#include "SDL3_image/SDL_image.h"
#include "stdlib.h"
#include "../ProjectVars.h"
#include "../update/update.c"
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>

int loadObj();
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

	buttonTexture = IMG_LoadTexture(renderer, "assets/buttonTexture.png");

	for(int i = 0; i < SCREEN_W; i++)
	{
		for (int j = 0; j < SCREEN_H; j++)
		{
			depthBuffer[j * SCREEN_W + i] = 99999;
		}
	}


	//BUTTONS
	wireframeButton = createButton(BUTTON_CLICK, (SDL_FRect){SCREEN_W / 2.f - 50, 10, 50, 50}, (SDL_FRect){0, 0, 100, 100}, toggleWireframe);

	nextPoint = createButton(BUTTON_CLICK, (SDL_FRect){10, 540, 50, 50}, (SDL_FRect){0, 0, 100, 100}, selectNextPoint);
	prevPoint = createButton(BUTTON_CLICK, (SDL_FRect){70, 540, 50, 50}, (SDL_FRect){100, 0, 100, 100}, selectPrevPoint);

	pointMinusX = createButton(BUTTON_HOLD, (SDL_FRect){10, 480, 50, 50}, (SDL_FRect){100, 100, 100, 100}, minusPointX);
	pointPlusX  = createButton(BUTTON_HOLD, (SDL_FRect){70, 480, 50, 50}, (SDL_FRect){0,   100, 100, 100}, plusPointX);
	pointMinusY = createButton(BUTTON_HOLD, (SDL_FRect){10, 420, 50, 50}, (SDL_FRect){100, 200, 100, 100}, minusPointY);
	pointPlusY  = createButton(BUTTON_HOLD, (SDL_FRect){70, 420, 50, 50}, (SDL_FRect){0,   200, 100, 100}, plusPointY);
	pointMinusZ = createButton(BUTTON_HOLD, (SDL_FRect){10, 360, 50, 50}, (SDL_FRect){100, 300, 100, 100}, minusPointZ);
	pointPlusZ  = createButton(BUTTON_HOLD, (SDL_FRect){70, 360, 50, 50}, (SDL_FRect){0,   300, 100, 100}, plusPointZ);

	if (!loadObj())
	{
		initEmptyObject(&exObj);

		addPointToObject(&exObj, (point){{-0.2, -0.2, -0.2}, {255, 0  , 0  , 255}}); //BBL
		addPointToObject(&exObj, (point){{ 0.2, -0.2, -0.2}, {255, 0  , 0  , 255}}); //BBR
		addPointToObject(&exObj, (point){{-0.2,  0.2, -0.2}, {255, 0  , 0  , 255}}); //BTL
		addPointToObject(&exObj, (point){{ 0.2,  0.2, -0.2}, {255, 0  , 0  , 255}}); //BTR

		addPointToObject(&exObj, (point){{-0.2, -0.2, 0.2}, {0  , 0  , 255, 255}}); //FBL
		addPointToObject(&exObj, (point){{ 0.2, -0.2, 0.2}, {0  , 0  , 255, 255}}); //FBR
		addPointToObject(&exObj, (point){{-0.2,  0.2, 0.2}, {0  , 0  , 255, 255}}); //FTL
		addPointToObject(&exObj, (point){{ 0.2,  0.2, 0.2}, {0  , 0  , 255, 255}}); //FTR

		createTriangleFromPointIds(&exObj, 0, 1, 2);
		createTriangleFromPointIds(&exObj, 1, 2, 3);

		createTriangleFromPointIds(&exObj, 4, 5, 6);
		createTriangleFromPointIds(&exObj, 5, 6, 7);

		createTriangleFromPointIds(&exObj, 1, 3, 5);
		createTriangleFromPointIds(&exObj, 3, 5, 7);

		createTriangleFromPointIds(&exObj, 0, 2, 4);
		createTriangleFromPointIds(&exObj, 2, 4, 6);
	}
	selectedPoint = &exObj.points[0];

	initEmptyObject(&levelFloor);
	addPointToObject(&levelFloor, (point){{-1.0, 0.5, -1.0}, {255, 0  , 0  , 255}});
	addPointToObject(&levelFloor, (point){{ 1.0, 0.5, -1.0}, {0  , 0  , 255, 255}});
	addPointToObject(&levelFloor, (point){{-1.0, 0.5,  1.0}, {0  , 255, 0  , 255}});
	addPointToObject(&levelFloor, (point){{ 1.0, 0.5,  1.0}, {255, 255, 255, 255}});

	createTriangleFromPointIds(&levelFloor, 0, 1, 2);
	createTriangleFromPointIds(&levelFloor, 1, 2, 3);

	pointerCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
	normalCursor  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);

}

void saveObj();
void quit()
{
	saveObj();
	free(pixels);
	free(depthBuffer);
	SDL_DestroyCursor(normalCursor);
	SDL_DestroyCursor(pointerCursor);
	SDL_DestroyTexture(frameBufferTexture);
	SDL_DestroyTexture(buttonTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int loadObj()
{
	FILE *input = fopen("assets/object.me", "rb");
	if (input == NULL)
		return 0;

	fread(&exObj, sizeof(object), 1, input);
	fclose(input);
	return 1;
}

void saveObj()
{
	FILE *output = fopen("assets/object.me", "wb");

	fwrite(&exObj, sizeof(object), 1, output);

	fclose(output);
}
