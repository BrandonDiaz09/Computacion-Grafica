//
// Created by Tadeito on 29/10/2024.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <stdint.h>
#include "vector.h"
#include "display.h"

typedef struct {
    int a;
    int b;
    int c;
    color_t color;
} face_t;

typedef struct {
    vec2_t points[3];
} triangle_t;

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_horizontal_line(int x1, int y, int x2, uint32_t color);
#endif //TRIANGLE_H
