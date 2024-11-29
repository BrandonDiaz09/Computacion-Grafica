#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include "array.h"
#include "display.h"
#include "matrix.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"
#include "stb_ds.h"
#include "lighting.h"

#define N_POINTS (9*9*9)
vec3_t cube_points[N_POINTS];

triangle_t* ArrayTriangle = NULL;

vec3_t cube_rotation = {0, 0, 0};
vec3_t cube_translation = {0, 0, 0};
vec3_t cube_scale = {1, 1, 1};

float fov_factor = 250;
bool is_running = false;
int previous_frame_time = 0;

int radious = 10;
int radiousA = 35;
int typeOfFigure = 2; // 0 = cube, 1 = circle, 2 .obj

// Variables de control de funcionalidades
bool render_triangles = true;
bool render_vertices = false;
bool render_edges = true;
bool perspective_projection = true;

void load_cube_mesh_data(void) {
    FILE* file = fopen("/Users/brandondiaz/CLionProjects/practica3/cmake-build-debug/cube.obj", "r");
    if (!file) {
        printf("Error al abrir el archivo cube.obj\n");
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(mesh.vertices, vertex);
            printf("Vértice cargado: (%f, %f, %f)\n", vertex.x, vertex.y, vertex.z);  // Debug
        }
        else if (strncmp(line, "f ", 2) == 0) {
            face_t face;
            int vertex_indices[3];
            sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &vertex_indices[0], &vertex_indices[1], &vertex_indices[2]);
            face.a = vertex_indices[0] - 1;
            face.b = vertex_indices[1] - 1;
            face.c = vertex_indices[2] - 1;
            face.color = 0xFFFFFF;
            array_push(mesh.faces, face);
            printf("Cara cargada: (%d, %d, %d)\n", face.a, face.b, face.c);  // Debug
        }
    }
    fclose(file);
    printf("Archivo cube.obj cerrado correctamente.\n");
}

void setup(void) {
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    if (!color_buffer) {
        fprintf(stderr, "Error allocating memory for frame buffer,\n");
    }

    if (typeOfFigure == 2) {
        load_cube_mesh_data();
    }
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                is_running = false;
            }
            // Alternar renderizado de triángulos
            else if (event.key.keysym.sym == SDLK_f) {
                render_triangles = !render_triangles;
            }
            // Alternar visualización de vértices
            else if (event.key.keysym.sym == SDLK_v) {
                render_vertices = !render_vertices;
            }
            // Alternar visualización de aristas
            else if (event.key.keysym.sym == SDLK_l) {
                render_edges = !render_edges;
            }
            // Alternar entre proyección perspectiva y ortográfica
            else if (event.key.keysym.sym == SDLK_p) {
                perspective_projection = !perspective_projection;
            }
            break;
    }

    // Asegurar que siempre haya algo visible en pantalla
    if (!render_triangles && !render_vertices && !render_edges) {
        render_triangles = true;
    }
}

vec2_t project(vec3_t v3) {
    vec2_t projected_point;
    if (perspective_projection) {
        projected_point = (vec2_t){
            .x = (fov_factor * v3.x) / v3.z,
            .y = (fov_factor * v3.y) / v3.z
        };
    } else {
        projected_point = (vec2_t){
            .x = fov_factor * v3.x,
            .y = fov_factor * v3.y
        };
    }
    return projected_point;
}

int compare_triangles(const void* a, const void* b) {
    const triangle_t* tri1 = (const triangle_t*)a;
    const triangle_t* tri2 = (const triangle_t*)b;

    if (tri1->depth < tri2->depth) return -1; // tri1 viene antes
    if (tri1->depth > tri2->depth) return 1;  // tri2 viene antes
    return 0;                                // Son iguales
}

void update(void) {
    ArrayTriangle = NULL; // Limpiar el array de triángulos
    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    cube_translation.z = 5;

    mat4_t scale_matrix = mat4_make_scale(cube_scale.x, cube_scale.y, cube_scale.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(cube_rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(cube_rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(cube_rotation.z);
    mat4_t translation_matrix = mat4_make_translation(cube_translation.x, cube_translation.y, cube_translation.z);

    mat4_t world_matrix = mat4_identity();
    world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
    world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

    // Dirección de la luz
    vec3_t light_dir = {0, 0, 1};
    vec3_normalize(&light_dir); // Normalizar dirección de la luz

    light_t light = { .direction = light_dir };

    for (int i = 0; i < array_length(mesh.faces); i++) {
        int verticeCara[3] = {
            mesh.faces[i].a,
            mesh.faces[i].b,
            mesh.faces[i].c
        };
        vec3_t verticesCara[3] = {
            mesh.vertices[verticeCara[0]],
            mesh.vertices[verticeCara[1]],
            mesh.vertices[verticeCara[2]]
        };

        vec4_t transformed_points[3];
        vec2_t projected_points[3];

        for (int j = 0; j < 3; j++) {
            vec4_t transformed_point = vec4_from_vec3(verticesCara[j]);
            transformed_point = mat4_mul_vec4(world_matrix, transformed_point);
            transformed_points[j] = transformed_point;
        }

        vec3_t a = vec3_from_vec4(transformed_points[0]);
        vec3_t b = vec3_from_vec4(transformed_points[1]);
        vec3_t c = vec3_from_vec4(transformed_points[2]);

        vec3_t ab = vec3_sub(b, a);
        vec3_t ac = vec3_sub(c, a);

        // Calcular la normal del triángulo
        vec3_t normal = vec3_cross(ab, ac);
        vec3_normalize(&normal); // Normalizar la normal

        // Calcular la intensidad de luz
        float light_intensity = -vec3_dot(normal, light.direction);

        if (light_intensity < 0) continue; // Ignorar triángulos orientados hacia atrás

        // Proyectar los puntos
        for (int j = 0; j < 3; j++) {
            vec2_t projected_point = project(vec3_from_vec4(transformed_points[j]));
            projected_points[j] = projected_point;
            projected_points[j].x += (window_width / 2);
            projected_points[j].y += (window_height / 2);
        }

        triangle_t triangle = {
            .points[0] = projected_points[0],
            .points[1] = projected_points[1],
            .points[2] = projected_points[2],
            .depth = (a.z + b.z + c.z) / 3
        };

        // Aplicar flat shading
        triangle.color = light_apply_intensity(0xFFFFFF, light_intensity);

        array_push(ArrayTriangle, triangle);
    }

    qsort(ArrayTriangle, array_length(ArrayTriangle), sizeof(triangle_t), compare_triangles);
}


void render(void) {
    draw_grid();

    // Renderizado de triángulos
    if (render_triangles) {
        for (int i = 0; i < array_length(ArrayTriangle); i++) {
            triangle_t tempTriangle = ArrayTriangle[i];

            // Usar el color calculado durante el flat shading
            uint32_t color = tempTriangle.color;

            draw_filled_triangle(
                tempTriangle.points[0].x, tempTriangle.points[0].y,
                tempTriangle.points[1].x, tempTriangle.points[1].y,
                tempTriangle.points[2].x, tempTriangle.points[2].y,
                color
            );
        }
    }

    // Renderizado de aristas
    if (render_edges) {
        for (int i = 0; i < array_length(ArrayTriangle); i++) {
            triangle_t tempTriangle = ArrayTriangle[i];

            draw_triangle(
                tempTriangle.points[0].x, tempTriangle.points[0].y,
                tempTriangle.points[1].x, tempTriangle.points[1].y,
                tempTriangle.points[2].x, tempTriangle.points[2].y,
                0xFF888888 // Color gris para las aristas
            );
        }
    }

    // Renderizado de vértices
    if (render_vertices) {
        // Crear las matrices de transformación
        mat4_t scale_matrix = mat4_make_scale(cube_scale.x, cube_scale.y, cube_scale.z);
        mat4_t rotation_matrix_x = mat4_make_rotation_x(cube_rotation.x);
        mat4_t rotation_matrix_y = mat4_make_rotation_y(cube_rotation.y);
        mat4_t rotation_matrix_z = mat4_make_rotation_z(cube_rotation.z);
        mat4_t translation_matrix = mat4_make_translation(cube_translation.x, cube_translation.y, cube_translation.z);

        // Crear la matriz de transformación global combinada
        mat4_t world_matrix = mat4_identity();
        world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
        world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
        world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
        world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
        world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

        // Dibujar cada vértice transformado
        for (int i = 0; i < array_length(mesh.vertices); i++) {
            vec3_t vertex = mesh.vertices[i];
            vec4_t transformed_vertex = vec4_from_vec3(vertex);

            // Aplicar la transformación al vértice
            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            // Proyectar el vértice transformado a 2D
            vec2_t projected_point = project(vec3_from_vec4(transformed_vertex));

            int screen_x = window_width / 2;
            int screen_y = window_height / 2;

            // Dibujar un pequeño cuadrado para representar el vértice
            draw_rect(
                projected_point.x + screen_x - 2, // Centrar el rectángulo en el punto
                projected_point.y + screen_y - 2,
                4, // Ancho del rectángulo
                4, // Alto del rectángulo
                0xFFFF0000 // Color rojo para los vértices
            );
        }
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    is_running = initialize_window();
    setup();

    while (is_running) {
        previous_frame_time = SDL_GetTicks();
        process_input();
        update();
        render();
        int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
        if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
            SDL_Delay(time_to_wait);
        }
    }
    destroy_window();
    return 0;
}