#pragma once
#ifndef RENDER_OBJECT
#define RENDER_OBJECT

typedef struct point {
	float coords[4];
	unsigned char color[4];
	char pointId;
} point;

typedef struct object {
	point points[3 * 300]; // 3 points per triangle * 300 max points
	int pointId[3 * 200]; //3 points per triangle * 200 triangles
	int nextPointInList;
	int nextPointId;

} object;

void initEmptyObject(object *obj)
{
	obj->nextPointInList = 0;
}

void addPointToObject(object *obj, point point)
{
	point.pointId = obj->nextPointInList;
	obj->points[obj->nextPointInList] = point;
	obj->nextPointInList += 1;
}

void createTriangleFromPointIds(object *obj, int id1, int id2, int id3)
{
	obj->pointId[obj->nextPointId + 0] = id1;
	obj->pointId[obj->nextPointId + 1] = id2;
	obj->pointId[obj->nextPointId + 2] = id3;

	obj->nextPointId += 3;
}

#endif

