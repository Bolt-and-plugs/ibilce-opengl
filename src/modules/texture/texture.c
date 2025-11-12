#include "texture.h"

// A "struct interna" que você definiu. Perfeita.
typedef struct {
    char path[256];
    u32 idx;
} texture_entry;

// O NOSSO GERENCIADOR: Um array dinâmico global e estático
static DynamicArray* texture_manager = NULL;

// Função de inicialização (chamada na primeira carga)
static void init_texture_manager() {
    if (texture_manager == NULL) {
        texture_manager = darray_create(sizeof(texture_entry), 10);
    }
}

// Esta é a função que o .mtl parser vai chamar
u32 load_texture(const char *path) {
    init_texture_manager();

    texture_entry* entries = (texture_entry*)texture_manager->data;
    for (int i = 0; i < texture_manager->size; i++) {
        if (strcmp(entries[i].path, path) == 0) {
            // Encontramos! Retorna o ID que já temos.
            return entries[i].idx;
        }
    }

    c_info("Carregando nova textura: %s", path);
    
    int w, h, channels;
    unsigned char *data = stbi_load(path, &w, &h, &channels, 0);
    if (!data) {
        c_error(FILE_OPEN_FAILURE, "Falha ao carregar textura");
        return 0; // Retorna ID inválido
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Melhor qualidade
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    gluBuild2DMipmaps(
        GL_TEXTURE_2D, 
        format,        
        w,             
        h,             
        format,        
        GL_UNSIGNED_BYTE, 
        data           
    );

    stbi_image_free(data);

    texture_entry new_entry;
    strncpy(new_entry.path, path, 256);
    new_entry.idx = texID;
    
    darray_push(texture_manager, &new_entry);

    return new_entry.idx; 
}


void free_all_textures() {
    if (texture_manager == NULL) return;

    c_info("Liberando %d texturas...", texture_manager->size);
    texture_entry* entries = (texture_entry*)texture_manager->data;
    for (int i = 0; i < texture_manager->size; i++) {
        glDeleteTextures(1, &entries[i].idx);
    }
    
    darray_destroy(texture_manager);
    texture_manager = NULL;
}
