#include "../ProjectVars.h"
#include "../object/CustomObject.h"
#include "../extra/matrixMath.h"
#include "customRenderFunctions.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>

void renderObject(object *object, SDL_Renderer *renderer);

void render()
{
	int renderStart = SDL_GetTicks();

	renderObject(&exObj, renderer);

	///////////////////////////

	renderCustomScreen();
	SDL_RenderPresent(renderer);

	int renderEnd = SDL_GetTicks();
	int waitTime =  (1000 / FPS) - (renderEnd - renderStart);
	SDL_Delay(waitTime > 0 ? waitTime : 0);
}

void renderObject(object *object, SDL_Renderer *renderer)
{
	mat4 transformMat;
	mat4 calcMat;
	initScalingMatrix(transformMat, 1, 1, 1);

	//initRotationalMatrix(calcMat, 0.f, SDL_GetTicks() / 500.0f, 0.f);
	//multMatByLeftMat(transformMat, calcMat);

	//initTranslationalMatrix(calcMat, 0.5, -0.5, 0);
	//multMatByLeftMat(transformMat, calcMat);

	initLookAtMatrix(calcMat, cameraPosition, cameraTarget, cameraUp);
	multMatByLeftMat(transformMat, calcMat);

	initProjectionMatrix(calcMat, 90, (float)SCREEN_W / SCREEN_H, 0.1f, 10.f);
	multMatByLeftMat(transformMat, calcMat);
	
	for (int i = 0; i < object->nextPointId; i+=3)
	{
		point p1 = object->points[object->pointId[i + 0]];
		multMatByVec(transformMat, p1.coords);
		turnScreenCoordToVecCoord(p1.coords, SCREEN_W, SCREEN_H);

		point p2 = object->points[object->pointId[i + 1]];
		multMatByVec(transformMat, p2.coords);
		turnScreenCoordToVecCoord(p2.coords, SCREEN_W, SCREEN_H);

		point p3 = object->points[object->pointId[i + 2]];
		multMatByVec(transformMat, p3.coords);
		turnScreenCoordToVecCoord(p3.coords, SCREEN_W, SCREEN_H);

		draw_filled_triangle(p1, p2, p3);

		//unsigned char r, g, b, a;
		//SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		//SDL_SetRenderDrawColor(renderer, 255 - r, 255 - g, 255 - b, 255);
		//drawLineCustom(p1, p2);
		//drawLineCustom(p1, p3);
		//drawLineCustom(p2, p3);

	}
}
