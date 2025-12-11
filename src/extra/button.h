#pragma once
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include "../ProjectVars.h"
#include "SDL3/SDL_render.h"
#ifndef BUTTON
#define BUTTON

#define BUTTON_HOLD 1
#define BUTTON_CLICK 2
typedef struct Button {
	
	char type;
	SDL_FRect buttonRect;
	SDL_FRect textureRect;
	void (*action)();

} Button;

Button createButton(char buttonType, SDL_FRect buttonDimensions, SDL_FRect textureDim, void (*func)())
{
	return (Button){
		.type = buttonType,
		.buttonRect = buttonDimensions,
		.action = func,
		.textureRect = textureDim
	};
}

void renderButton(Button *btn)
{
	if (!SDL_RenderTexture(renderer, buttonTexture, &btn->textureRect, &btn->buttonRect))
		SDL_RenderFillRect(renderer, &btn->buttonRect);
}

int isInsideButton(int x, int y, Button *btn)
{
	return (x > btn->buttonRect.x && x < (btn->buttonRect.x + btn->buttonRect.w) &&
	y > btn->buttonRect.y && y < (btn->buttonRect.y + btn->buttonRect.h));
}

int updateButton(Button btn)
{
	if (isInsideButton(mouseX, mouseY, &btn))
	{
		if (	(mouseHold && btn.type == BUTTON_HOLD) 	 ||
			(mouseClicked && btn.type == BUTTON_CLICK))
			btn.action();
		return 1;
	}
	return 0;
}

#endif
