#include "lighting.h"
#include "display.h"
color_t light_apply_intensity(color_t original_color, float percentage_factor) {
    if (percentage_factor < 0) percentage_factor = 0;
    if (percentage_factor > 1) percentage_factor = 1;

    // Separar los canales de color y aplicar la intensidad
    uint8_t a = (original_color >> 24) & 0xFF;
    uint8_t r = ((original_color >> 16) & 0xFF) * percentage_factor;
    uint8_t g = ((original_color >> 8) & 0xFF) * percentage_factor;
    uint8_t b = (original_color & 0xFF) * percentage_factor;

    return (a << 24) | (r << 16) | (g << 8) | b;
}
