#include "../ProjectVars.h"
#include "../object/CustomObject.h"
#include <SDL3/SDL_render.h>

void renderObject(object *object, SDL_Renderer *renderer);

void render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	renderObject(&exObj, renderer);

	SDL_RenderPresent(renderer);
}

void renderObject(object *object, SDL_Renderer *renderer)
{
	//TODO: Change this method & object file to use SDL_RenderGeometry()
	for (int i = 0; i < object->nextPointId; i+=3)
	{
		point p1 = object->points[object->pointId[i + 0]];
		point p2 = object->points[object->pointId[i + 1]];
		point p3 = object->points[object->pointId[i + 2]];

		SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
		SDL_RenderLine(renderer, p1.x, p1.y, p3.x, p3.y);
		SDL_RenderLine(renderer, p2.x, p2.y, p3.x, p3.y);

	}
}
