#include "../ProjectVars.h"
#include "../object/CustomObject.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
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
	
	SDL_FColor color = {255, 255, 255, 255};
	SDL_FPoint point = {0, 0};
	for (int i = 0; i < object->nextPointId; i+=3)
	{
		//ugly ass code but whatev 4 now
		SDL_Vertex vertex[3] = {
			{(SDL_FPoint){object->points[object->pointId[i + 0]].x, object->points[object->pointId[i + 0]].y}, color, point},
			{(SDL_FPoint){object->points[object->pointId[i + 1]].x, object->points[object->pointId[i + 1]].y}, color, point},
			{(SDL_FPoint){object->points[object->pointId[i + 2]].x, object->points[object->pointId[i + 2]].y}, color, point}
		};

		SDL_RenderGeometry(renderer, NULL, vertex, 3, NULL, 0);

	}
}
