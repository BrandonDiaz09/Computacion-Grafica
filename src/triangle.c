#include "display.h"
#include "triangle.h"

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // Ordenar los puntos por coordenada y (y0 <= y1 <= y2)
    if (y0 > y1) { int tempX = x0; x0 = x1; x1 = tempX; int tempY = y0; y0 = y1; y1 = tempY; }
    if (y1 > y2) { int tempX = x1; x1 = x2; x2 = tempX; int tempY = y1; y1 = y2; y2 = tempY; }
    if (y0 > y1) { int tempX = x0; x0 = x1; x1 = tempX; int tempY = y0; y0 = y1; y1 = tempY; }

    // Casos "flat-top" y "flat-bottom"
    if (y1 == y2) {
        // Caso "flat-bottom"
        draw_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
        // Caso "flat-top"
        draw_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    } else {
        // Caso general: dividir en "flat-top" y "flat-bottom"
        int x3 = x0 + (float)(y1 - y0) / (float)(y2 - y0) * (x2 - x0);  // Punto de división
        int y3 = y1;

        // Dibujar ambos subtriángulos
        draw_flat_bottom_triangle(x0, y0, x1, y1, x3, y3, color);
        draw_flat_top_triangle(x1, y1, x3, y3, x2, y2, color);
    }
}

void draw_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    float invslope1 = (float)(x1 - x0) / (y1 - y0);
    float invslope2 = (float)(x2 - x0) / (y2 - y0);

    float curx1 = x0;
    float curx2 = x0;

    for (int y = y0; y <= y1; y++) {
        draw_horizontal_line((int)curx1, y, (int)curx2, color);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void draw_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    float invslope1 = (float)(x2 - x0) / (y2 - y0);
    float invslope2 = (float)(x2 - x1) / (y2 - y1);

    float curx1 = x2;
    float curx2 = x2;

    for (int y = y2; y > y0; y--) {
        draw_horizontal_line((int)curx1, y, (int)curx2, color);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

// Dibuja una línea horizontal de x1 a x2 en la fila y
void draw_horizontal_line(int x1, int y, int x2, uint32_t color) {
    if (x1 > x2) { int temp = x1; x1 = x2; x2 = temp; }
    for (int x = x1; x <= x2; x++) {
        draw_pixel(x, y, color);
    }
}


// Draw a triangle using three raw line calls
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}