#ifndef _GL_DRAW_H_
#define _GL_DRAW_H_

#include "../../defines.h"
#include "../obj_parser/obj_parser.h"

typedef enum {
    DRAW_CUBE_WIREFRAME,
    DRAW_CUBE_SOLID
} DrawCubeType;

void draw_skybox(float size, u32 texture_id, vec3f player_pos, Color color);


void draw_model(Model* model, vec3f position, vec3f rotation, vec3f scale);

#endif
