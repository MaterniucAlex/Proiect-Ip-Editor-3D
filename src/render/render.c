#include "../ProjectVars.h"
#include "../object/CustomObject.h"
#include "../extra/matrixMath.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <math.h>
#include <stdio.h>

void renderObject(object *object, SDL_Renderer *renderer);

void render()
{
	int renderStart = SDL_GetTicks();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	renderObject(&exObj, renderer);

	SDL_RenderPresent(renderer);

	int renderEnd = SDL_GetTicks();
	int waitTime =  (1000 / FPS) - (renderEnd - renderStart);
	SDL_Delay(waitTime > 0 ? waitTime : 0);
}

void renderObject(object *object, SDL_Renderer *renderer)
{
	mat4 transformMat;
	initScalingMatrix(transformMat, 1, 1, 1);
	mat4 calcMat;
	initRotationalMatrix(calcMat, 0.f, SDL_GetTicks() / 500.0f, 0.f);
	multMatByLeftMat(transformMat, calcMat);
	initTranslationalMatrix(calcMat, 0.5, -0.5, 0);
	multMatByLeftMat(transformMat, calcMat);
	
	SDL_FPoint pt = {0, 0};
	for (int i = 0; i < object->nextPointId; i+=3)
	{
		//ugly ass code but whatev 4 now
		point p1 = object->points[object->pointId[i + 0]];
		multMatByVec(transformMat, p1.coords);
		turnScreenCoordToVecCoord(p1.coords, SCREEN_W, SCREEN_H);

		point p2 = object->points[object->pointId[i + 1]];
		multMatByVec(transformMat, p2.coords);
		turnScreenCoordToVecCoord(p2.coords, SCREEN_W, SCREEN_H);

		point p3 = object->points[object->pointId[i + 2]];
		multMatByVec(transformMat, p3.coords);
		turnScreenCoordToVecCoord(p3.coords, SCREEN_W, SCREEN_H);

		SDL_Vertex vertex[3] = {
			{(SDL_FPoint){p1.coords[0], p1.coords[1]}, {p1.color[0], p1.color[1], p1.color[2], p1.color[3]}, pt},
			{(SDL_FPoint){p2.coords[0], p2.coords[1]}, {p2.color[0], p2.color[1], p2.color[2], p2.color[3]}, pt},
			{(SDL_FPoint){p3.coords[0], p3.coords[1]}, {p3.color[0], p3.color[1], p3.color[2], p3.color[3]}, pt},
		};

		SDL_RenderGeometry(renderer, NULL, vertex, 3, NULL, 0);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderLine(renderer, p1.coords[0], p1.coords[1], p2.coords[0], p2.coords[1]);
		SDL_RenderLine(renderer, p3.coords[0], p3.coords[1], p2.coords[0], p2.coords[1]);
		SDL_RenderLine(renderer, p1.coords[0], p1.coords[1], p3.coords[0], p3.coords[1]);

	}
}
