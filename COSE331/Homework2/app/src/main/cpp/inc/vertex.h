#ifndef HW2_VERTEX_H
#define HW2_VERTEX_H

#include "global.h"

struct Vertex {
    vec3 pos;
    vec3 nor;
    vec2 tex;
    ivec4 bone;
    vec4 weight;
};
typedef GLushort Index;

#endif //HW2_VERTEX_H
