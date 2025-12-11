#pragma once
#include <stdio.h>
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
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				mouseHold = true;
				break;
		}

	}

	SDL_GetMouseState(&mouseX, &mouseY);

	int buttonsUpdated = 0;
	buttonsUpdated += updateButton(spinCamLeft);
	buttonsUpdated += updateButton(spinCamRight);
	buttonsUpdated += updateButton(spinCamUp);
	buttonsUpdated += updateButton(spinCamDown);

	buttonsUpdated += updateButton(wireframeButton);
	buttonsUpdated += updateButton(nextPoint);
	buttonsUpdated += updateButton(prevPoint);

	buttonsUpdated += updateButton(pointPlusX);
	mouseClicked = false;

	if (buttonsUpdated > 0)
		SDL_SetCursor(pointerCursor);
	else
		SDL_SetCursor(normalCursor);

	cameraPosition[0] = cos(cameraXangle);
	cameraPosition[1] = cameraYangle;
	cameraPosition[2] = sin(cameraXangle);
}

void rotateCameraLeft()
{
	cameraXangle += 0.1f;
}
void rotateCameraRight()
{
	cameraXangle -= 0.1f;
}

void rotateCameraUp()
{
	if (cameraYangle > -0.9)
		cameraYangle -= 0.1f;
}

void rotateCameraDown()
{
	if (cameraYangle < 0.9)
		cameraYangle += 0.1f;
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

void plusPointX()
{
	selectedPoint->coords[0] += 0.01;
}
#endif
