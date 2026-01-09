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

				if (fabsf(x) > normalizer)
					normalizer = fabsf(x);

				buffer2 = strtok(NULL , " ");
				if (buffer2) y = -atof(buffer2);

				buffer2 = strtok(NULL , " ");
				if (buffer2) z = atof(buffer2);

				if (buffer2) addPointToObject(obj, (point){{x, -y, z, 1.f}});

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

			//strtok(NULL, " ");
			//buffer2 = strtok(NULL, "/");
			//if (buffer2)
			//{
				//p2 = atoi(buffer2) - 1;
				//createTriangleFromPointIds(obj, p1, p2, p3);
			//}
		}
	}

	fclose(file);

	for(int i = 0; i < obj->nextPointInList; i++)
	{
		obj->points[i].coords[0] /= normalizer;
		obj->points[i].coords[1] /= -normalizer;
		obj->points[i].coords[2] /= normalizer;

		obj->points[i].color[0] = (obj->points[i].coords[0] + 1) / 2 * 255;
		obj->points[i].color[1] = 0;//obj->points[i].coords[0] * 255;
		obj->points[i].color[2] = 0;//obj->points[i].coords[0] * 255;
	}

}

#endif
