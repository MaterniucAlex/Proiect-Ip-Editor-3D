#include "../ProjectVars.h"
#include "../object/CustomObject.h"
#include "../extra/matrixMath.h"
#include "customRenderFunctions.h"
#include "SDL3_image/SDL_image.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>

void renderObject(object *object, SDL_Renderer *renderer);

void render()
{
	int renderStart = SDL_GetTicks();

	memset(pixels, 0, SCREEN_W * SCREEN_H * sizeof(Uint32));
	memset(depthBuffer, 127, SCREEN_W * SCREEN_H * sizeof(float));
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	renderObject(&levelFloor, renderer);
	renderObject(&exObj, renderer);

	///////////////////////////

	renderCustomScreen();

	SDL_SetRenderDrawColor(renderer, 255, 50, 50, 125);

	renderButton(&wireframeButton);
	renderButton(&nextPoint);
	renderButton(&prevPoint);
	renderButton(&pointPlusX);
	renderButton(&pointMinusX);
	renderButton(&pointPlusY);
	renderButton(&pointMinusY);
	renderButton(&pointPlusZ);
	renderButton(&pointMinusZ);

	////
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_FRect rect = {pointX - 5, pointY - 5, 10, 10};
	////
	SDL_RenderFillRect(renderer, &rect);

	SDL_RenderPresent(renderer);

	int renderEnd = SDL_GetTicks();
	int waitTime =  (1000 / FPS) - (renderEnd - renderStart);
	SDL_Delay(waitTime > 0 ? waitTime : 0);
}

void drawSelectedPoint(mat4 transformMat);
bool isInside(point pt, point v1, point v2, point v3);
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

	point p1, p2, p3;
	p1.pointId = -1;
	p2.pointId = -1;
	p3.pointId = -1;
	
	for (int i = 0; i < (object->nextPointId - object->nextPointId % 3); i+=3)
	{
		if (object->pointId[i + 0] == p2.pointId)
		{
			p1 = p2;
			p1.color[0] = object->points[object->pointId[i + 0]].color[0];
			p1.color[1] = object->points[object->pointId[i + 0]].color[1];
			p1.color[2] = object->points[object->pointId[i + 0]].color[2];
		}
		else
		{
			p1 = object->points[object->pointId[i + 0]];
			multMatByVec(transformMat, p1.coords);
			turnScreenCoordToVecCoord(p1.coords, SCREEN_W, SCREEN_H);
		}

		if (object->pointId[i + 1] == p3.pointId)
		{
			p2 = p3;
			p2.color[0] = object->points[object->pointId[i + 1]].color[0];
			p2.color[1] = object->points[object->pointId[i + 1]].color[1];
			p2.color[2] = object->points[object->pointId[i + 1]].color[2];
		}
		else
		{
			p2 = object->points[object->pointId[i + 1]];
			multMatByVec(transformMat, p2.coords);
			turnScreenCoordToVecCoord(p2.coords, SCREEN_W, SCREEN_H);
		}

		p3 = object->points[object->pointId[i + 2]];
		multMatByVec(transformMat, p3.coords);
		turnScreenCoordToVecCoord(p3.coords, SCREEN_W, SCREEN_H);

		if (!wireframeRender)
		{
			if (isInside((point){{mouseX, mouseY, 0, 0}}, p1, p2, p3))
			{
				p1.color[0] += 100;
				p1.color[1] += 100;
				p1.color[1] += 100;

				p2.color[0] += 100;
				p2.color[1] += 100;
				p2.color[1] += 100;

				p3.color[0] += 100;
				p3.color[1] += 100;
				p3.color[1] += 100;
			}
			draw_filled_triangle(p1, p2, p3);
		}
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

	drawSelectedPoint(transformMat);

}

void drawSelectedPoint(mat4 transformMat)
{
	point pOrigin = (point){.coords = {
		selectedPoint->coords[0],
		selectedPoint->coords[1],
		selectedPoint->coords[2],
		selectedPoint->coords[3],
	}, .color = {0, 0, 0, 255}};

	point pX = (point){.coords = {
		selectedPoint->coords[0] + 0.1,
		selectedPoint->coords[1],
		selectedPoint->coords[2],
		selectedPoint->coords[3],
	}, .color = {0, 255, 0, 255}};

	point pY = (point){.coords = {
		selectedPoint->coords[0],
		selectedPoint->coords[1] - 0.1,
		selectedPoint->coords[2],
		selectedPoint->coords[3],
	}, .color = {255, 0, 0, 255}};

	point pZ = (point){.coords = {
		selectedPoint->coords[0],
		selectedPoint->coords[1],
		selectedPoint->coords[2] + 0.1,
		selectedPoint->coords[3],
	}, .color = {0, 0, 255, 255}};

	multMatByVec(transformMat, pOrigin.coords);
	turnScreenCoordToVecCoord(pOrigin.coords, SCREEN_W, SCREEN_H);
	multMatByVec(transformMat, pX.coords);
	turnScreenCoordToVecCoord(pX.coords, SCREEN_W, SCREEN_H);
	multMatByVec(transformMat, pY.coords);
	turnScreenCoordToVecCoord(pY.coords, SCREEN_W, SCREEN_H);
	multMatByVec(transformMat, pZ.coords);
	turnScreenCoordToVecCoord(pZ.coords, SCREEN_W, SCREEN_H);

	drawLineCustom(pOrigin, pX);
	drawLineCustom(pOrigin, pY);
	drawLineCustom(pOrigin, pZ);

}

float sign(point p, point a, point b) {
    return (p.coords[0] - b.coords[0]) * (a.coords[1] - b.coords[1]) - (a.coords[0] - b.coords[0]) * (p.coords[1] - b.coords[1]);
}

bool isInside(point pt, point v1, point v2, point v3) {
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}
