#include "SDL3_image/SDL_image.h"
#include "stdlib.h"
#include "../ProjectVars.h"
#include "../update/update.c"
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <string.h>

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
	wireframeButton = createButton(BUTTON_CLICK, (SDL_FRect){SCREEN_W / 2.f - 50, 10, 100, 50}, (SDL_FRect){200, 200, 200, 100}, toggleWireframe);

	prevPoint = createButton(BUTTON_CLICK, (SDL_FRect){10, SCREEN_H - 50 - 10, 50, 50}, (SDL_FRect){100, 0, 100, 100}, selectPrevPoint);
	nextPoint = createButton(BUTTON_CLICK, (SDL_FRect){70, SCREEN_H - 50 - 10, 50, 50}, (SDL_FRect){0, 0, 100, 100}, selectNextPoint);

	pointMinusX = createButton(BUTTON_HOLD, (SDL_FRect){10, SCREEN_H - 100 - 20, 50, 50}, (SDL_FRect){100, 100, 100, 100}, minusPointX);
	pointPlusX  = createButton(BUTTON_HOLD, (SDL_FRect){70, SCREEN_H - 100 - 20, 50, 50}, (SDL_FRect){0,   100, 100, 100}, plusPointX);
	pointMinusY = createButton(BUTTON_HOLD, (SDL_FRect){10, SCREEN_H - 150 - 25, 50, 50}, (SDL_FRect){100, 200, 100, 100}, minusPointY);
	pointPlusY  = createButton(BUTTON_HOLD, (SDL_FRect){70, SCREEN_H - 150 - 25, 50, 50}, (SDL_FRect){0,   200, 100, 100}, plusPointY);
	pointMinusZ = createButton(BUTTON_HOLD, (SDL_FRect){10, SCREEN_H - 200 - 30, 50, 50}, (SDL_FRect){100, 300, 100, 100}, minusPointZ);
	pointPlusZ  = createButton(BUTTON_HOLD, (SDL_FRect){70, SCREEN_H - 200 - 30, 50, 50}, (SDL_FRect){0,   300, 100, 100}, plusPointZ);

	loadObjButton = createButton(BUTTON_CLICK, (SDL_FRect){10, 10, 100, 50}, (SDL_FRect){200, 0  , 200, 100}, objLoad);
	saveObjButton = createButton(BUTTON_CLICK, (SDL_FRect){10, 70, 100, 50}, (SDL_FRect){200, 100, 200, 100}, objSave);

	toggleColorMenu     = createButton(BUTTON_CLICK, (SDL_FRect){SCREEN_W - 200 - 40, SCREEN_H - 50 - 10, 50, 50}, (SDL_FRect){  0, 400, 100, 100}, colorMenuToggle);
	toggleScaleMenu     = createButton(BUTTON_CLICK, (SDL_FRect){SCREEN_W - 150 - 30, SCREEN_H - 50 - 10, 50, 50}, (SDL_FRect){100, 400, 100, 100}, scaleMenuToggle);
	toggleTranslateMenu = createButton(BUTTON_CLICK, (SDL_FRect){SCREEN_W - 50  - 10, SCREEN_H - 50 - 10, 50, 50}, (SDL_FRect){200, 400, 100, 100}, translateMenuToggle);
	toggleRotateMenu    = createButton(BUTTON_CLICK, (SDL_FRect){SCREEN_W - 100 - 20, SCREEN_H - 50 - 10, 50, 50}, (SDL_FRect){300, 400, 100, 100}, rotateMenuToggle);

	//initMenu(&translateMenu);

	colorMenu = createMenu(0, 3);
	colorMenu.isVisible = 0;
	colorMenu.sliderList[0] = createSlider(0.0, 255.0, (SDL_FRect){SCREEN_W - 200 - 20, SCREEN_H - 120 - 100, 100, 10}, (SDL_Color){255,   0,   0, 255});
	colorMenu.sliderList[1] = createSlider(0.0, 255.0, (SDL_FRect){SCREEN_W - 200 - 20, SCREEN_H - 120 - 80 , 100, 10}, (SDL_Color){  0, 255,   0, 255});
	colorMenu.sliderList[2] = createSlider(0.0, 255.0, (SDL_FRect){SCREEN_W - 200 - 20, SCREEN_H - 120 - 60 , 100, 10}, (SDL_Color){  0,   0, 255, 255});

	scaleMenu = createMenu(0, 3);
	scaleMenu.isVisible = 0;
	scaleMenu.sliderList[0] = createSlider(0.1, 2.0, (SDL_FRect){SCREEN_W - 200 - 20, SCREEN_H - 50 - 100, 100, 10}, (SDL_Color){255,   0,   0, 255});
	scaleMenu.sliderList[1] = createSlider(0.1, 2.0, (SDL_FRect){SCREEN_W - 200 - 20, SCREEN_H - 50 - 80 , 100, 10}, (SDL_Color){  0, 255,   0, 255});
	scaleMenu.sliderList[2] = createSlider(0.1, 2.0, (SDL_FRect){SCREEN_W - 200 - 20, SCREEN_H - 50 - 60 , 100, 10}, (SDL_Color){  0,   0, 255, 255});

	translateMenu = createMenu(0, 3);
	translateMenu.isVisible = 0;
	translateMenu.sliderList[0] = createSlider(-1.0, 1.0, (SDL_FRect){SCREEN_W - 100 - 10, SCREEN_H - 50 - 100, 100, 10}, (SDL_Color){255,   0,   0, 255});
	translateMenu.sliderList[1] = createSlider(-1.0, 1.0, (SDL_FRect){SCREEN_W - 100 - 10, SCREEN_H - 50 - 80 , 100, 10}, (SDL_Color){  0, 255,   0, 255});
	translateMenu.sliderList[2] = createSlider(-1.0, 1.0, (SDL_FRect){SCREEN_W - 100 - 10, SCREEN_H - 50 - 60 , 100, 10}, (SDL_Color){  0,   0, 255, 255});

	rotateMenu = createMenu(0, 3);
	rotateMenu.isVisible = 0;
	rotateMenu.sliderList[0] = createSlider(-1.0, 1.0, (SDL_FRect){SCREEN_W - 100 - 10, SCREEN_H - 120 - 100, 100, 10}, (SDL_Color){255,   0,   0, 255});
	rotateMenu.sliderList[1] = createSlider(-1.0, 1.0, (SDL_FRect){SCREEN_W - 100 - 10, SCREEN_H - 120 - 80 , 100, 10}, (SDL_Color){  0, 255,   0, 255});
	rotateMenu.sliderList[2] = createSlider(-1.0, 1.0, (SDL_FRect){SCREEN_W - 100 - 10, SCREEN_H - 120 - 60 , 100, 10}, (SDL_Color){  0,   0, 255, 255});

	if (!loadObj())
	{
		initEmptyObject(&exObj);

		addPointToObject(&exObj, (point){{-0.2, -0.2, -0.2, 1.f}, {255, 0  , 0  , 255}}); //BBL
		addPointToObject(&exObj, (point){{ 0.2, -0.2, -0.2, 1.f}, {255, 0  , 0  , 255}}); //BBR
		addPointToObject(&exObj, (point){{-0.2,  0.2, -0.2, 1.f}, {255, 0  , 0  , 255}}); //BTL
		addPointToObject(&exObj, (point){{ 0.2,  0.2, -0.2, 1.f}, {255, 0  , 0  , 255}}); //BTR

		addPointToObject(&exObj, (point){{-0.2, -0.2, 0.2, 1.f}, {0  , 0  , 255, 255}}); //FBL
		addPointToObject(&exObj, (point){{ 0.2, -0.2, 0.2, 1.f}, {0  , 0  , 255, 255}}); //FBR
		addPointToObject(&exObj, (point){{-0.2,  0.2, 0.2, 1.f}, {0  , 0  , 255, 255}}); //FTL
		addPointToObject(&exObj, (point){{ 0.2,  0.2, 0.2, 1.f}, {0  , 0  , 255, 255}}); //FTR

		createTriangleFromPointIds(&exObj, 0, 1, 2);
		createTriangleFromPointIds(&exObj, 1, 2, 3);

		createTriangleFromPointIds(&exObj, 4, 5, 6);
		createTriangleFromPointIds(&exObj, 5, 6, 7);

		createTriangleFromPointIds(&exObj, 1, 3, 5);
		createTriangleFromPointIds(&exObj, 3, 5, 7);

		createTriangleFromPointIds(&exObj, 0, 2, 4);
		createTriangleFromPointIds(&exObj, 2, 4, 6);

		initScalingMatrix(objScalingMat, 1, 1, 1);
		initTranslationalMatrix(objTranslateMat, 0, 0, 0);
		initRotationalMatrix(objRotateMat, 0, 0, 0);
	}
	selectedPoint = &exObj.points[0];

	colorMenu.sliderList[0].curVal = selectedPoint->color[0];
	colorMenu.sliderList[1].curVal = selectedPoint->color[1];
	colorMenu.sliderList[2].curVal = selectedPoint->color[2];


	initEmptyObject(&levelFloor);
	addPointToObject(&levelFloor, (point){{-1.0, 0.5, -1.0, 1}, {255, 0  , 0  , 255}});
	addPointToObject(&levelFloor, (point){{ 1.0, 0.5, -1.0, 1}, {0  , 0  , 255, 255}});
	addPointToObject(&levelFloor, (point){{-1.0, 0.5,  1.0, 1}, {0  , 255, 0  , 255}});
	addPointToObject(&levelFloor, (point){{ 1.0, 0.5,  1.0, 1}, {255, 255, 255, 255}});

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
	freeMenu(&colorMenu);
	freeMenu(&scaleMenu);
	freeMenu(&rotateMenu);
	freeMenu(&translateMenu);
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

	char buffer[4];
	fread(buffer, sizeof(char) * 3, 1, input);
	buffer[3] = 0;
	if (strcmp(buffer, "obj") != 0)
	{
		fclose(input);
		return 0;
	}
	fread(&exObj, sizeof(object), 1, input);

	fread(objScalingMat, sizeof(mat4), 1, input);
	scaleMenu.sliderList[0].curVal = objScalingMat[0 * 4 + 0];
	scaleMenu.sliderList[1].curVal = objScalingMat[1 * 4 + 1];
	scaleMenu.sliderList[2].curVal = objScalingMat[2 * 4 + 2];

	fread(&rotateMenu.sliderList[0].curVal, sizeof(float), 1, input);
	fread(&rotateMenu.sliderList[1].curVal, sizeof(float), 1, input);
	fread(&rotateMenu.sliderList[2].curVal, sizeof(float), 1, input);

	fread(objTranslateMat, sizeof(mat4), 1, input);
	translateMenu.sliderList[0].curVal = objTranslateMat[0 * 4 + 3];
	translateMenu.sliderList[1].curVal = objTranslateMat[1 * 4 + 3];
	translateMenu.sliderList[2].curVal = objTranslateMat[2 * 4 + 3];

	fclose(input);

	return 1;
}

void saveObj()
{
	FILE *output = fopen("assets/object.me", "wb");

	fwrite("obj", sizeof(char) * 3, 1, output);
	fwrite(&exObj, sizeof(object), 1, output);
	fwrite(objScalingMat, sizeof(mat4), 1, output);
	fwrite(&rotateMenu.sliderList[0].curVal, sizeof(float), 1, output);
	fwrite(&rotateMenu.sliderList[1].curVal, sizeof(float), 1, output);
	fwrite(&rotateMenu.sliderList[2].curVal, sizeof(float), 1, output);
	fwrite(objTranslateMat, sizeof(mat4), 1, output);

	fclose(output);
}
