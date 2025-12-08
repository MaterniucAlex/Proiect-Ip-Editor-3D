#include "../ProjectVars.h"
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
		}

	}

	cameraPosition[0] = cos(SDL_GetTicks() / 500.f);
	cameraPosition[1] = -1.f;
	cameraPosition[2] = sin(SDL_GetTicks() / 500.f);
}
