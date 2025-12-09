#pragma once
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include "../ProjectVars.h"
#ifndef BUTTON
#define BUTTON

typedef struct Button {
	
	SDL_FRect buttonRect;
	char text[20];
	void (*action)();

} Button;

int isInsideButton(int x, int y, Button btn)
{
	return (x > btn.buttonRect.x && x < (btn.buttonRect.x + btn.buttonRect.w) &&
	y > btn.buttonRect.y && y < (btn.buttonRect.y + btn.buttonRect.h));
}

int updateButton(Button btn)
{
	if (isInsideButton(mouseX, mouseY, btn))
	{
		if (mouseHold) btn.action();
		return 1;
	}
	return 0;
}

#endif
