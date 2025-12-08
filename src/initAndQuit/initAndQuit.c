#include "stdlib.h"
#include "../ProjectVars.h"
#include "../extra/matrixMath.h"
#include <stdio.h>
void init()
{
	SDL_Init(SDL_INIT_VIDEO);
	window   = safeInit(SDL_CreateWindow("Editor 3D", SCREEN_W, SCREEN_H, 0));
	renderer = safeInit(SDL_CreateRenderer(window, NULL));

	depthBuffer 	= malloc(sizeof(float)  * (SCREEN_W * SCREEN_H));
	screen		= malloc(sizeof(Screen) * (SCREEN_W * SCREEN_H));

	for(int i = 0; i < SCREEN_W; i++)
	{
		for (int j = 0; j < SCREEN_H; j++)
		{
			depthBuffer[j * SCREEN_W + i] = 99999;
		}
	}

	initEmptyObject(&exObj);

	addPointToObject(&exObj, (point){{-0.2, -0.2, -0.2}, {255, 0  , 0  , 255}}); //BBL
	addPointToObject(&exObj, (point){{ 0.2, -0.2, -0.2}, {255, 0  , 0  , 255}}); //BBR
	addPointToObject(&exObj, (point){{-0.2,  0.2, -0.2}, {255, 0  , 0  , 255}}); //BTL
	addPointToObject(&exObj, (point){{ 0.2,  0.2, -0.2}, {255, 0  , 0  , 255}}); //BTR
	
	addPointToObject(&exObj, (point){{-0.2, -0.2, 0.2}, {0  , 255, 0  , 255}}); //BBL
	addPointToObject(&exObj, (point){{ 0.2, -0.2, 0.2}, {0  , 255, 0  , 255}}); //BBR
	addPointToObject(&exObj, (point){{-0.2,  0.2, 0.2}, {0  , 255, 0  , 255}}); //BTL
	addPointToObject(&exObj, (point){{ 0.2,  0.2, 0.2}, {0  , 255, 0  , 255}}); //BTR

	createTriangleFromPointIds(&exObj, 0, 1, 2);
	createTriangleFromPointIds(&exObj, 1, 2, 3);

	createTriangleFromPointIds(&exObj, 4, 5, 6);
	createTriangleFromPointIds(&exObj, 5, 6, 7);

	createTriangleFromPointIds(&exObj, 1, 3, 5);
	createTriangleFromPointIds(&exObj, 3, 5, 7);

	createTriangleFromPointIds(&exObj, 0, 2, 4);
	createTriangleFromPointIds(&exObj, 2, 4, 6);
}

void quit()
{
	free(screen);
	free(depthBuffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
