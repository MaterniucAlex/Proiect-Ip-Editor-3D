#pragma once
#include <SDL3/SDL_render.h>
#include <stdio.h>
#ifndef CUSTOM_RENDER
#define CUSTOM_RENDER

#include "../object/CustomObject.h"
#include "../ProjectVars.h"

void swapPoints(point *p1, point *p2)
{
	point aux = *p1;
	*p1 = *p2;
	*p2 = aux;
}

void drawLineCustom(point p1, point p2)
{
    unsigned char r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    float x1 = (float)p1.coords[0];
    float y1 = (float)p1.coords[1];
    float z1 = p1.coords[2];

    float x2 = (float)p2.coords[0];
    float y2 = (float)p2.coords[1];
    float z2 = p2.coords[2];

    float xDist = x2 - x1;
    float yDist = y2 - y1;
    float zDist = z2 - z1;
    
    float totalDistance = sqrtf(xDist * xDist + yDist * yDist);
    
    if (totalDistance == 0) return;

    float xStep = xDist / totalDistance;
    float yStep = yDist / totalDistance;
    float zStep = zDist / totalDistance;

    float rStep = (p2.color[0] - p1.color[0]) / totalDistance;
    float gStep = (p2.color[1] - p1.color[1]) / totalDistance;
    float bStep = (p2.color[2] - p1.color[2]) / totalDistance;

    for(int i = 0; i < (int)(totalDistance + 0.5f); i++)
    {
        int curX = (int)(x1 + xStep * i + 0.5f);
        int curY = (int)(y1 + yStep * i + 0.5f);
        float curZ = z1 + i * zStep;

        if (curX < 0 || curX >= SCREEN_W || curY < 0 || curY >= SCREEN_H) {
            continue;
        }

        int index = curX + curY * SCREEN_W;

        if (curZ < depthBuffer[index]) 
        {
            screen[index].r = p1.color[0] + i * rStep;
            screen[index].g = p1.color[1] + i * gStep;
            screen[index].b = p1.color[2] + i * bStep;
            
            depthBuffer[index] = curZ;
        }
    }
}

void step_point(point *p, float slope_x, float slope_z) {
    p->coords[0] += slope_x; // Update X
    p->coords[1] += 1.0f;    // Update Y (Move down 1 scanline)
    p->coords[2] += slope_z; // Update Z
}

void fill_flat_bottom_triangle(point v1, point v2, point v3) {
    // 1. Calculate Slopes (Change in X/Z per Y)
    // Note: We assume v1 is the top tip.
    float inv_slope1 = (float)(v2.coords[0] - v1.coords[0]) / (v2.coords[1] - v1.coords[1]);
    float inv_slope2 = (float)(v3.coords[0] - v1.coords[0]) / (v3.coords[1] - v1.coords[1]);

    float slope_z1 = (v2.coords[2] - v1.coords[2]) / (v2.coords[1] - v1.coords[1]);
    float slope_z2 = (v3.coords[2] - v1.coords[2]) / (v3.coords[1] - v1.coords[1]);

    // 2. Create Walkers
    // Both start at the top vertex (v1)
    point leftPoint = v1;
    point rightPoint = v1;

    // 3. Iterate Top to Bottom
    for (int scanline_y = (int)v1.coords[1]; scanline_y <= (int)v2.coords[1]; scanline_y++) {
        
        // Draw the horizontal span
        // Note: Because we used float accumulation, we might want to round coords for precision
        drawLineCustom(leftPoint, rightPoint);

        // 4. Step Walkers Down
        // We manually increment Y inside these points so drawLine knows where to draw!
        step_point(&leftPoint, inv_slope1, slope_z1);
        step_point(&rightPoint, inv_slope2, slope_z2);
    }
}

void fill_flat_top_triangle(point v1, point v2, point v3) {
    // Note: In this function, v3 is the Bottom Tip. v1 and v2 are the top flat edge.
    // To keep it simple, we will start at v1/v2 and move DOWN to v3.
    
    float inv_slope1 = (float)(v3.coords[0] - v1.coords[0]) / (v3.coords[1] - v1.coords[1]);
    float inv_slope2 = (float)(v3.coords[0] - v2.coords[0]) / (v3.coords[1] - v2.coords[1]);

    float slope_z1 = (v3.coords[2] - v1.coords[2]) / (v3.coords[1] - v1.coords[1]);
    float slope_z2 = (v3.coords[2] - v2.coords[2]) / (v3.coords[1] - v2.coords[1]);

    // Start walkers at the flat top edge
    point leftPoint = v1;
    point rightPoint = v2;

    // Iterate Top to Bottom
    for (int scanline_y = (int)v1.coords[1]; scanline_y <= (int)v3.coords[1]; scanline_y++) {
        
        drawLineCustom(leftPoint, rightPoint);

        step_point(&leftPoint, inv_slope1, slope_z1);
        step_point(&rightPoint, inv_slope2, slope_z2);
    }
}

void draw_filled_triangle(point v1, point v2, point v3) {
    // Sort vertices by Y (v1 top, v3 bottom)
    if (v1.coords[1] > v2.coords[1]) swapPoints(&v1, &v2);
    if (v1.coords[1] > v3.coords[1]) swapPoints(&v1, &v3);
    if (v2.coords[1] > v3.coords[1]) swapPoints(&v2, &v3);

    if (v2.coords[1] == v3.coords[1]) {
        fill_flat_bottom_triangle(v1, v2, v3);
    } 
    else if (v1.coords[1] == v2.coords[1]) {
        fill_flat_top_triangle(v1, v2, v3);
    } 
    else {
        point v4; 
        // Interpolate v4 on the long edge (v1-v3) at the Y height of v2
        float t = (float)(v2.coords[1] - v1.coords[1]) / (v3.coords[1] - v1.coords[1]);

        v4.coords[0] = v1.coords[0] + ((v3.coords[0] - v1.coords[0]) * t);
        v4.coords[1] = v2.coords[1]; // IMPORTANT: Explicitly set Y
        v4.coords[2] = v1.coords[2] + ((v3.coords[2] - v1.coords[2]) * t);

        fill_flat_bottom_triangle(v1, v2, v4);
        fill_flat_top_triangle(v2, v4, v3);
    }
}

void renderCustomScreen()
{
	for(int x = 0; x < SCREEN_W; x++)
	{
		for (int y = 0; y < SCREEN_H; y++)
		{
			Screen s = screen[x + y * SCREEN_W];
			SDL_SetRenderDrawColor(renderer, s.r, s.g, s.b, 255);
			SDL_RenderPoint(renderer, x, y);

			screen[x + y * SCREEN_W].r = 0;
			screen[x + y * SCREEN_W].g = 0;
			screen[x + y * SCREEN_W].b = 0;
			depthBuffer[x + y * SCREEN_W] = 99999;
		}
	}
}

#endif
