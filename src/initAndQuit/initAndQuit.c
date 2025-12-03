#include "../ProjectVars.h"
void init()
{
	SDL_Init(SDL_INIT_VIDEO);
	window   = safeInit(SDL_CreateWindow("Editor 3D", 800, 600, 0));
	renderer = safeInit(SDL_CreateRenderer(window, NULL));

	initEmptyObject(&exObj);
	addPointToObject(&exObj, (point){100, 100, 0, 0, 0, 0});
	addPointToObject(&exObj, (point){200, 100, 0, 0, 0, 0});
	addPointToObject(&exObj, (point){100, 200, 0, 0, 0, 0});
	createTriangleFromPointIds(&exObj, 0, 1, 2);
}

void quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
