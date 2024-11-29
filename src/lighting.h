//
// Created by Brandon Diaz on 29/11/24.
//

#ifndef LIGHTING_H
#define LIGHTING_H
#include "vector.h"
#endif //LIGHTING_H
#include "display.h"

typedef struct {
    vec3_t direction;
} light_t;

// Función para aplicar intensidad de luz
color_t light_apply_intensity(color_t original_color, float percentage_factor);


