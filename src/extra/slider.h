#pragma once
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#ifndef SLIDER
#define SLIDER

#include "../ProjectVars.h"
#include "../render/customRenderFunctions.h"

typedef struct Slider {
	SDL_FRect sliderArea;
	float minVal;
	float maxVal;
	float curVal;
} Slider;

Slider createSlider(float minVal, float maxVal, SDL_FRect sliderArea)
{
	Slider slider;
	slider.sliderArea = sliderArea;
	slider.minVal = minVal;
	slider.maxVal = maxVal;
	slider.curVal = minVal + (maxVal - minVal) / 2;
	return slider;

}
void renderSlider(Slider *slider)
{
	float step = (slider->maxVal - slider->minVal) / (slider->sliderArea.w);
	renderRect(slider->sliderArea);

	float circleX = slider->sliderArea.x + (slider->curVal - slider->minVal) / step;
	float circleY = slider->sliderArea.y + slider->sliderArea.h / 2;
	SDL_SetRenderDrawColor(renderer, 200, 200, 125, 255);
	renderCircle(circleX, circleY, slider->sliderArea.h);
}

void updateSlider(Slider *slider)
{
	float step = (slider->maxVal - slider->minVal) / (slider->sliderArea.w);
	float circleX = slider->sliderArea.x + (slider->curVal - slider->minVal) / step;
	float circleY = slider->sliderArea.y + slider->sliderArea.h / 2;

	int distanceToMouseX = mouseX - circleX;
	int distanceToMouseY = mouseY - circleY;
	int distanceToMouse  = sqrt(distanceToMouseX * distanceToMouseX + distanceToMouseY * distanceToMouseY);
	if (distanceToMouse < slider->sliderArea.h && mouseHold)
	{
		circleX = mouseX;
		if (circleX < slider->sliderArea.x) circleX = slider->sliderArea.x;
		if (circleX > slider->sliderArea.x + slider->sliderArea.w) circleX = slider->sliderArea.x + slider->sliderArea.w;

		slider->curVal = slider->minVal + (circleX - slider->sliderArea.x) * step;
	}
}

#endif
