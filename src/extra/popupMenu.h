#pragma once
#ifndef POPUP_MENU
#define POPUP_MENU

#include <stdlib.h>
#include "button.h"
#include "slider.h"

typedef struct PopupMenu {
	Button *buttonList;
	Slider *sliderList;
	int nrOfSliders;
	int nrOfButtons;
	char isVisible;
} PopupMenu;

void initMenu(PopupMenu *menu)
{
	menu->isVisible  = 0;
	menu->buttonList = NULL;
	menu->nrOfButtons = 0;
	menu->sliderList = NULL;
	menu->nrOfSliders = 0;
}

PopupMenu createMenu(int nrOfButtons, int nrOfSliders)
{
	PopupMenu menu;
	menu.isVisible   = 0;
	menu.sliderList  = malloc(sizeof(Slider) * nrOfSliders);
	menu.nrOfSliders = nrOfSliders;
	menu.buttonList  = malloc(sizeof(Button) * nrOfButtons);
	menu.nrOfButtons = nrOfButtons;
	return menu;
}

void renderMenu(PopupMenu *menu)
{
	if (!menu->isVisible) return;

	for(int i = 0; i < menu->nrOfButtons; i++)
		renderButton(menu->buttonList + i);

	for(int i = 0; i < menu->nrOfSliders; i++)
		renderSlider(menu->sliderList + i);

}

void updateMenu(PopupMenu *menu)
{
	if (!menu->isVisible) return;

	for(int i = 0; i < menu->nrOfButtons; i++)
		updateButton(&menu->buttonList[i]);

	for(int i = 0; i < menu->nrOfSliders; i++)
		updateSlider(&menu->sliderList[i]);

}

void freeMenu(PopupMenu *menu)
{
	free(menu->sliderList);
	free(menu->buttonList);
	menu->nrOfSliders = 0;
	menu->nrOfSliders = 0;
	menu->isVisible = 0;
}

#endif
