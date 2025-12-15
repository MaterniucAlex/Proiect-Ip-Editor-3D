#pragma once
#include "SDL3/SDL_dialog.h"
#include "SDL3/SDL_filesystem.h"
#include <stdio.h>
#include <string.h>
#ifndef UPDATE
#define UPDATE

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
		}

	}

	SDL_GetMouseState(&mouseX, &mouseY);

	if (camDragStartX != 0 && camDragStartY != 0)
	{
		cameraXangle = camWhenDragStartX + (camDragStartX - mouseX) / 200.f;
		cameraYangle = camWhenDragStartY + (camDragStartY - mouseY) / 200.f;
	}


	int buttonsUpdated = 0;

	buttonsUpdated += updateButton(wireframeButton);
	buttonsUpdated += updateButton(nextPoint);
	buttonsUpdated += updateButton(prevPoint);

	buttonsUpdated += updateButton(pointPlusX);
	buttonsUpdated += updateButton(pointMinusX);
	buttonsUpdated += updateButton(pointPlusY);
	buttonsUpdated += updateButton(pointMinusY);
	buttonsUpdated += updateButton(pointPlusZ);
	buttonsUpdated += updateButton(pointMinusZ);

	buttonsUpdated += updateButton(loadObjButton);
	buttonsUpdated += updateButton(saveObjButton);
	mouseClicked = false;

	if (buttonsUpdated > 0)
		SDL_SetCursor(pointerCursor);
	else
		SDL_SetCursor(normalCursor);

	cameraPosition[0] = cos(cameraXangle);
	cameraPosition[1] = cameraYangle;
	cameraPosition[2] = sin(cameraXangle);
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
}

void selectPrevPoint()
{
	if (selectedPoint->pointId > 0)
		selectedPoint = &exObj.points[selectedPoint->pointId-1];
	else
		selectedPoint = &exObj.points[exObj.nextPointInList-1];
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
		    fread(&exObj, sizeof(object), 1, input);
    }
    if (mode == 'S')
    {
	    input = fopen(filelist[0], "wb");
	    if (input == NULL) return;

	    fwrite("obj", sizeof(char) * 3, 1, input);
	    fwrite(&exObj, sizeof(object), 1, input);
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
#endif
