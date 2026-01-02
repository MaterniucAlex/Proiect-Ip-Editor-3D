#pragma once
#include <math.h>
#ifndef LOAD_OBJ_FILE
#define LOAD_OBJ_FILE

#include "../object/CustomObject.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadObjFromObjFile(object *obj, const char *path)
{
	initEmptyObject(obj);

	unsigned char r = 0, g = 0, b = 0;

	FILE *file = fopen(path, "r");
	assert(file);

	char buffer[100];

	float normalizer = 1;
	while (fgets(buffer, 100, file))
	{
		if(buffer[0] == 'v' && buffer[1] == ' ')
		{
			char *buffer2 = strtok(buffer, " \n\t");
			while(buffer2)
			{
				float x, y, z;
				buffer2 = strtok(NULL , " ");
				if (buffer2) x = atof(buffer2);
				if (fabsf(x) > 1 && normalizer == 1)
					normalizer = x * 2;
				x /= normalizer;

				buffer2 = strtok(NULL , " ");
				if (buffer2) y = -atof(buffer2);
				y /= normalizer;

				buffer2 = strtok(NULL , " ");
				if (buffer2) z = atof(buffer2);
				z /= normalizer;

				if (buffer2) addPointToObject(obj, (point){{x, -y, z, 1.f}, {r, g, b}});

				r = (r < 255 - 1 ? r + 1 : 0);
				g = (g < 255 - 2 ? g + 2 : 0);
				b = (b < 255 - 3 ? b + 3 : 0);

				buffer2 = strtok(NULL , " ");
			}
		}
		if(buffer[0] == 'f')
		{
			strtok(buffer, " ");
			char *buffer2 = strtok(NULL, "/");

			unsigned int p1, p2, p3;
			if (buffer2) p1 = atoi(buffer2) - 1;

			strtok(NULL, " ");
			buffer2 = strtok(NULL, "/");
			if (buffer2) p2 = atoi(buffer2) - 1;

			strtok(NULL, " ");
			buffer2 = strtok(NULL, "/");
			if (buffer2) p3 = atoi(buffer2) - 1;

			if (buffer2) createTriangleFromPointIds(obj, p1, p2, p3);
		}
	}

	fclose(file);

}

#endif
