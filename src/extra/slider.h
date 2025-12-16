#pragma once
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

void renderSlider(Slider *slider)
{
	float step = (slider->maxVal - slider->minVal) / (slider->sliderArea.w);
	SDL_RenderRect(renderer, slider->sliderArea);
	float circleX = slider->curVal / step;
	float circleY = slider->sliderArea.y + slider->sliderArea.h / 2;
	renderCircle(circleX, circleY, slider->sliderArea.h);
}

#endif
