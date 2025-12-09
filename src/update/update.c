#pragma once
#include <stdio.h>
#ifndef UPDATE
#define UPDATE

#include "../ProjectVars.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_timer.h>
#include <math.h>

float mouseX, mouseY;
bool mouseHold = false;
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
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				mouseHold = true;
				break;
		}

	}
	if (mouseHold)
	{
		SDL_GetMouseState(&mouseX, &mouseY);
		if (isInsideButton(mouseX, mouseY, spinCamLeft))
			spinCamLeft.action();
		if (isInsideButton(mouseX, mouseY, spinCamRight))
			spinCamRight.action();
	}

	cameraPosition[0] = cos(cameraAngle);
	cameraPosition[1] = -1.f;
	cameraPosition[2] = sin(cameraAngle);
}

void rotateCameraLeft()
{
	cameraAngle += 0.1f;
}
void rotateCameraRight()
{
	cameraAngle -= 0.1f;
}

#endif
