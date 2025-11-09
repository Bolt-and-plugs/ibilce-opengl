#ifndef _GL_DRAW_H_
#define _GL_DRAW_H_

#include "../../defines.h"

typedef enum {
    DRAW_CUBE_WIREFRAME,
    DRAW_CUBE_SOLID
} DrawCubeType;

void draw_wall(vec3f p1, vec3f p2, float height, Color color, u32 texture_id, DrawCubeType type);
void draw_floor(vec3f p1, vec3f p2, Color color, u32 texture_id);

void draw_grass(vec3f p1, vec3f p2, Color color, u32 texture_id);
void draw_ceiling(vec3f p1, vec3f p2, float height, Color color, u32 texture_id);
void draw_cube(vec3f position, float size, Color color, u32 texture_id, DrawCubeType type);
void draw_skybox(float size, u32 texture_id, vec3f player_pos, Color color);

#endif
