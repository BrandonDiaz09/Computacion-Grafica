// mesh.h

#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

// Vértices y caras predefinidos para el cubo
extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

// Define una estructura para mallas de tamaño dinámico, con arrays de vértices y caras
typedef struct {
    vec3_t* vertices;   // Array dinámico de vértices
    face_t* faces;      // Array dinámico de caras
} mesh_t;

// Declaración de una variable global para la malla dinámica
extern mesh_t mesh;

// Funciones de carga de malla
void load_cube_mesh_data(void); // Carga datos de un cubo predefinido
void load_obj_file_data(const char* filename); // Carga datos desde un archivo .obj

#endif // MESH_H
