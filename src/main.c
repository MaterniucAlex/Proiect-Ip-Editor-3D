#include "ProjectVars.h"
#include "initAndQuit/initAndQuit.c"
#include "update/update.c"
#include "render/render.c"

int main()
{
	init();

	while(isRunning)
	{
		update();

		render();
	}

	quit();

	return 0;
}
