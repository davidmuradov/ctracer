#ifndef INCLUDES_RAY_H
#define INCLUDES_RAY_H

#include "vec3.h"

struct ray {
    struct vec3 o; // Origin
    struct vec3 dir; // Direction
};

void
ray_init(struct ray* r, struct vec3 o, struct vec3 dir);

struct vec3
ray_at(struct ray r, float t);

#endif
