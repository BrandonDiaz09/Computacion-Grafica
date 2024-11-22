#include <stdio.h>
#include "mesh.h"
#include "vector.h"  // Asegúrate de tener vector.h si defines `vec3_t`

mesh_t mesh = { NULL, NULL }; // Definición de la malla global

void load_obj_file_data(const char* filename) {
    // Abre el archivo .obj
    FILE* file = fopen("T:/IPN/P3/cmake-build-debug/cube.obj", "r");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo %s.\n", filename);
        return;
    }
    printf("Archivo %s abierto correctamente.\n", filename);

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        // Procesa líneas de vértice
        if (line[0] == 'v' && line[1] == ' ') {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            // Escala el vértice para asegurarse de que está en el campo de visión
            vertex.x *= 0.1;
            vertex.y *= 0.1;
            vertex.z *= 0.1;
            printf("Vértice cargado: %f, %f, %f\n", vertex.x, vertex.y, vertex.z);
            // Aquí agregarías `vertex` a tu estructura de malla
        }
        // Procesa líneas de cara
        else if (line[0] == 'f' && line[1] == ' ') {
            int vertex_indices[3];
            sscanf(line, "f %d %d %d", &vertex_indices[0], &vertex_indices[1], &vertex_indices[2]);
            printf("Cara cargada: %d, %d, %d\n", vertex_indices[0], vertex_indices[1], vertex_indices[2]);
            // Aquí agregarías los índices de la cara a tu estructura de malla
        }
    }

    fclose(file);
    printf("Archivo %s cerrado.\n", filename);
}

// Función para inicializar y cargar el archivo .obj
void initialize_mesh() {
    const char* obj_path = "T:/IPN/P3/cmake-build-debug/cube.obj";
    load_obj_file_data(obj_path);
}
