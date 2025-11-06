#ifndef _GL_DRAW_H_
#define _GL_DRAW_H_

#include "../../defines.h"

void draw_wall(Vertex3f p1, Vertex3f p2, float height, Vertex3f color, u32 texture_id);

void draw_floor(Vertex3f p1, Vertex3f p2, Vertex3f color, u32 texture_id);

void draw_ceiling(Vertex3f p1, Vertex3f p2, float height, Vertex3f color, u32 texture_id);

void draw_cube(Vertex3f position, float size, Vertex3f color, u32 texture_id);

void draw_skybox(float size, u32 texture_id);

#endif
