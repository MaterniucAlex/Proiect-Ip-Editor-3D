#include "../ProjectVars.h"
#include "../object/CustomObject.h"
#include "../extra/matrixMath.h"
#include "customRenderFunctions.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>

void renderObject(object *object, SDL_Renderer *renderer, mat4 transformMat);
void drawSelectedPoint(mat4 transformMat);

void render()
{
	int renderStart = SDL_GetTicks();

	if (!hasStarted)
	{
		SDL_FRect renderRect = {0, 0, 800, 600};
		SDL_RenderTexture(renderer, startTexture, &renderRect, &renderRect);

		SDL_RenderPresent(renderer);

		int renderEnd = SDL_GetTicks();
		int waitTime =  (1000 / FPS) - (renderEnd - renderStart);
		SDL_Delay(waitTime > 0 ? waitTime : 0);

		return;
	}

	memset(pixels, 0, SCREEN_W * SCREEN_H * sizeof(Uint32));
	memset(depthBuffer, 127, SCREEN_W * SCREEN_H * sizeof(float));
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	mat4 transformMat;

	//initScalingMatrixS(transformMat, 10);
	//renderObject(&levelBounds, renderer, transformMat);

	initScalingMatrixS(transformMat, 1);
	renderObject(&levelFloor, renderer, transformMat);

	copyToMat(transformMat, objScalingMat);
	multMatByLeftMat(transformMat, objRotateMat);
	multMatByLeftMat(transformMat, objTranslateMat);

	renderObject(&exObj, renderer, transformMat);
	drawSelectedPoint(transformMat);

	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	renderCircle(pointX, pointY, 5);

	///////////////////////////

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	renderMenu(&colorMenu);
	renderMenu(&scaleMenu);
	renderMenu(&translateMenu);
	renderMenu(&rotateMenu);

	renderCustomScreen();

	renderMenu(&modelTransformMenu);
	renderMenu(&pointTransformMenu);

	SDL_SetRenderDrawColor(renderer, 255, 50, 50, 125);

	renderButton(&wireframeButton);
	renderButton(&loadObjButton);
	renderButton(&saveObjButton);

	renderButton(&toggleModelMenu);
	renderButton(&togglePointMenu);

	////
	SDL_RenderPresent(renderer);

	int renderEnd = SDL_GetTicks();
	int waitTime =  (1000 / FPS) - (renderEnd - renderStart);
	SDL_Delay(waitTime > 0 ? waitTime : 0);
}

/*
int isPointInbounds(point *p)
{
	float x = p->coords[0];
	float y = p->coords[1];

	if (x < 0 || x > SCREEN_W * 2) return 0;
	if (y < 0 || y > SCREEN_H * 2) return 0;

	return 1;
}
*/

void renderObject(object *object, SDL_Renderer *renderer, mat4 transformMat)
{
	mat4 calcMat;

	initLookAtMatrix(calcMat, cameraPosition, cameraTarget, cameraUp);
	multMatByLeftMat(transformMat, calcMat);

	initProjectionMatrix(calcMat, 90, (float)SCREEN_W / SCREEN_H, 0.1f, 100.f);
	multMatByLeftMat(transformMat, calcMat);

	point p1, p2, p3;
	p1.pointId = -1;
	p2.pointId = -1;
	p3.pointId = -1;
	
	for (int i = 0; i < (object->nextPointId - object->nextPointId % 3); i+=3)
	{
		if (object->pointId[i + 0] == -1) continue;
		if (object->pointId[i + 0] == p2.pointId)
			p1 = p2;
		else
		{
			p1 = object->points[object->pointId[i + 0]];

			multMatByVec(transformMat, p1.coords);
			applyPerspectiveVector(p1.coords);

			turnScreenCoordToVecCoord(p1.coords, SCREEN_W, SCREEN_H);
		}

		if (!p1.isVisible) continue;
		if (object->pointId[i + 1] == p3.pointId)
			p2 = p3;
		else
		{
			p2 = object->points[object->pointId[i + 1]];

			multMatByVec(transformMat, p2.coords);
			applyPerspectiveVector(p2.coords);

			turnScreenCoordToVecCoord(p2.coords, SCREEN_W, SCREEN_H);
		}

		if (!p2.isVisible) continue;
		p3 = object->points[object->pointId[i + 2]];
		if (!p3.isVisible) continue;

		multMatByVec(transformMat, p3.coords);
		applyPerspectiveVector(p3.coords);
		turnScreenCoordToVecCoord(p3.coords, SCREEN_W, SCREEN_H);

		if (!wireframeRender)
			draw_filled_triangle(p1, p2, p3);
		else
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			drawLineCustom(p1, p2);
			drawLineCustom(p1, p3);
			drawLineCustom(p2, p3);
		}

		if (selectedPoint->pointId == p1.pointId)
		{
			pointX = p1.coords[0];
			pointY = p1.coords[1];
		}
		if (selectedPoint->pointId == p2.pointId)
		{
			pointX = p2.coords[0];
			pointY = p2.coords[1];
		}
		if (selectedPoint->pointId == p3.pointId)
		{
			pointX = p3.coords[0];
			pointY = p3.coords[1];
		}

	}

}

void drawSelectedPoint(mat4 transformMat)
{
	if (selectedPoint->pointId == -1) return;

	point pOrigin = (point){.coords = {
		selectedPoint->coords[0],
		selectedPoint->coords[1],
		selectedPoint->coords[2],
		selectedPoint->coords[3]
	}, .color = {0, 255, 255, 255}};

	point pX = (point){.coords = {
		selectedPoint->coords[0] + 0.1,
		selectedPoint->coords[1],
		selectedPoint->coords[2],
		selectedPoint->coords[3]
	}, .color = {125, 0, 0, 255}};

	point pY = (point){.coords = {
		selectedPoint->coords[0],
		selectedPoint->coords[1] - 0.1,
		selectedPoint->coords[2],
		selectedPoint->coords[3]
	}, .color = {0, 125, 0, 255}};

	point pZ = (point){.coords = {
		selectedPoint->coords[0],
		selectedPoint->coords[1],
		selectedPoint->coords[2] + 0.1,
		selectedPoint->coords[3]
	}, .color = {0, 0, 125, 255}};

	multMatByVec(transformMat, pOrigin.coords);
	applyPerspectiveVector(pOrigin.coords);
	turnScreenCoordToVecCoord(pOrigin.coords, SCREEN_W, SCREEN_H);

	multMatByVec(transformMat, pX.coords);
	applyPerspectiveVector(pX.coords);
	turnScreenCoordToVecCoord(pX.coords, SCREEN_W, SCREEN_H);

	multMatByVec(transformMat, pY.coords);
	applyPerspectiveVector(pY.coords);
	turnScreenCoordToVecCoord(pY.coords, SCREEN_W, SCREEN_H);

	multMatByVec(transformMat, pZ.coords);
	applyPerspectiveVector(pZ.coords);
	turnScreenCoordToVecCoord(pZ.coords, SCREEN_W, SCREEN_H);

	drawLineCustomT(pOrigin, pX, 2);
	drawLineCustomT(pOrigin, pY, 2);
	drawLineCustomT(pOrigin, pZ, 2);

}
