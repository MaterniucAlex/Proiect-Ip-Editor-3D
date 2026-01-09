#pragma once
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#ifndef UPDATE
#define UPDATE

#include "SDL3/SDL_dialog.h"
#include "SDL3/SDL_filesystem.h"
#include <stdio.h>
#include <string.h>
#include "../ObjLoader/ObjLoader.h"
#include "../ProjectVars.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_timer.h>
#include <math.h>

void update()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type) 
		{
			case SDL_EVENT_QUIT:
				isRunning = 0;
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				mouseHold = false;
				mouseClicked = true;
				if (event.button.button == SDL_BUTTON_RIGHT && camDragStartX != 0)
				{
					camDragStartX = 0;
					camDragStartY = 0;
				}
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					mouseHold = true;
				if (event.button.button == SDL_BUTTON_RIGHT && camDragStartX == 0)
				{
					SDL_GetMouseState(&camDragStartX, &camDragStartY);
					camWhenDragStartX = cameraXangle;
					camWhenDragStartY = cameraYangle;
				}
				break;
			case SDL_EVENT_MOUSE_WHEEL:
				cameraZoom += 0.05 * (event.wheel.y);
				if (cameraZoom < 0.4f) cameraZoom = 0.4f;
				if (cameraZoom > 3.f) cameraZoom = 3.f;
				break;
		}

	}

	SDL_GetMouseState(&mouseX, &mouseY);

	if (camDragStartX != 0 && camDragStartY != 0)
	{
		cameraXangle = camWhenDragStartX + (camDragStartX - mouseX) / 200.f;
		cameraYangle = camWhenDragStartY + (camDragStartY - mouseY) / 200.f;
	}

	buttonsUpdated = 0;

	buttonsUpdated += updateButton(&wireframeButton);

	buttonsUpdated += updateButton(&loadObjButton);
	buttonsUpdated += updateButton(&saveObjButton);

	buttonsUpdated += updateButton(&toggleModelMenu);
	buttonsUpdated += updateButton(&togglePointMenu);

	updateMenu(&modelTransformMenu);
	updateMenu(&pointTransformMenu);

	updateMenu(&colorMenu);
	updateMenu(&scaleMenu);
	updateMenu(&translateMenu);
	updateMenu(&rotateMenu);

	mouseClicked = false;

	objScalingMat[0 * 4 + 0] = scaleMenu.sliderList[0].curVal;
	objScalingMat[1 * 4 + 1] = scaleMenu.sliderList[1].curVal;
	objScalingMat[2 * 4 + 2] = scaleMenu.sliderList[2].curVal;

	objTranslateMat[0 * 4 + 3] = translateMenu.sliderList[0].curVal;
	objTranslateMat[1 * 4 + 3] = -translateMenu.sliderList[1].curVal;
	objTranslateMat[2 * 4 + 3] = translateMenu.sliderList[2].curVal;

	selectedPoint->color[0] = colorMenu.sliderList[0].curVal;
	selectedPoint->color[1] = colorMenu.sliderList[1].curVal;
	selectedPoint->color[2] = colorMenu.sliderList[2].curVal;

	initRotationalMatrix(objRotateMat, rotateMenu.sliderList[0].curVal, rotateMenu.sliderList[1].curVal, rotateMenu.sliderList[2].curVal);

	if (buttonsUpdated > 0)
		SDL_SetCursor(pointerCursor);
	else
		SDL_SetCursor(normalCursor);

	cameraPosition[0] = cos(cameraXangle) / cameraZoom;
	cameraPosition[1] = cameraYangle / cameraZoom;
	cameraPosition[2] = sin(cameraXangle) / cameraZoom;
}

void toggleWireframe()
{
	wireframeRender = !wireframeRender;
}

void selectNextPoint()
{
	if (selectedPoint->pointId < exObj.nextPointInList)
		selectedPoint = &exObj.points[selectedPoint->pointId+1];
	else
		selectedPoint = &exObj.points[0];

	colorMenu.sliderList[0].curVal = selectedPoint->color[0];
	colorMenu.sliderList[1].curVal = selectedPoint->color[1];
	colorMenu.sliderList[2].curVal = selectedPoint->color[2];
}

void selectPrevPoint()
{
	if (selectedPoint->pointId > 0)
		selectedPoint = &exObj.points[selectedPoint->pointId-1];
	else
		selectedPoint = &exObj.points[exObj.nextPointInList-1];

	colorMenu.sliderList[0].curVal = selectedPoint->color[0];
	colorMenu.sliderList[1].curVal = selectedPoint->color[1];
	colorMenu.sliderList[2].curVal = selectedPoint->color[2];
}

const float changeVal = 0.01;

void plusPointX()
{
	selectedPoint->coords[0] += changeVal;
}

void minusPointX()
{
	selectedPoint->coords[0] -= changeVal;
}

void plusPointY()
{
	selectedPoint->coords[1] -= changeVal;
}

void minusPointY()
{
	selectedPoint->coords[1] += changeVal;
}

void plusPointZ()
{
	selectedPoint->coords[2] += changeVal;
}

void minusPointZ()
{
	selectedPoint->coords[2] -= changeVal;
}

char mode = 'L';
void SDLCALL FileDialogCallback(void *userdata, const char * const *filelist, int filter) {
    if (!filelist) {
        printf("Error: %s\n", SDL_GetError());
        return;
    }

    if (!*filelist) return;

    FILE *input;

    if (mode == 'L')
    {
	    input = fopen(filelist[0], "rb");
	    if (input == NULL) return;

	    char buffer[4];
	    fread(&buffer, sizeof(char) * 3, 1, input);
	    buffer[3] = '\0';
	    if (strcmp(buffer, "obj") == 0)
	    {
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
	    }
	    else
	    {
		    loadObjFromObjFile(&exObj, filelist[0]);
	    }

	    selectedPoint = &exObj.points[0];
	    colorMenu.sliderList[0].curVal = selectedPoint->color[0];
	    colorMenu.sliderList[1].curVal = selectedPoint->color[1];
	    colorMenu.sliderList[2].curVal = selectedPoint->color[2];
    }
    if (mode == 'S')
    {
	    input = fopen(filelist[0], "wb");
	    if (input == NULL) return;

	    fwrite("obj", sizeof(char) * 3, 1, input);
	    fwrite(&exObj, sizeof(object), 1, input);
	    fwrite(objScalingMat, sizeof(mat4), 1, input);
	    fwrite(&rotateMenu.sliderList[0].curVal, sizeof(float), 1, input);
	    fwrite(&rotateMenu.sliderList[1].curVal, sizeof(float), 1, input);
	    fwrite(&rotateMenu.sliderList[2].curVal, sizeof(float), 1, input);
	    fwrite(objTranslateMat, sizeof(mat4), 1, input);
    }
	    fclose(input);
}

void objLoad()
{
	mode = 'L';
	SDL_ShowOpenFileDialog(FileDialogCallback, NULL, window, NULL, 0, SDL_GetBasePath(), false);
}

void objSave()
{
	mode = 'S';
	SDL_ShowSaveFileDialog(FileDialogCallback, NULL, window, NULL, 0, SDL_GetBasePath());
}

void colorMenuToggle()
{
	colorMenu.isVisible = !colorMenu.isVisible;
}

void scaleMenuToggle()
{
	scaleMenu.isVisible = !scaleMenu.isVisible;
}

void translateMenuToggle()
{
	translateMenu.isVisible = !translateMenu.isVisible;
}

void rotateMenuToggle()
{
	rotateMenu.isVisible = !rotateMenu.isVisible;
}

void modelMenuToggle()
{
	modelTransformMenu.isVisible = !modelTransformMenu.isVisible;
}

void pointMenuToggle()
{
	pointTransformMenu.isVisible = !pointTransformMenu.isVisible;
}
#endif
