#ifndef INCLUDES_RAY_H
#define INCLUDES_RAY_H

#include "tuple.h"
#include "matrix.h"

struct ray {
    struct tuple o; // Origin
    struct tuple dir; // Direction
};

void
ray_init(struct ray* r, struct tuple o, struct tuple d);

struct tuple
ray_position_at(struct ray* r, double t);

struct ray
ray_transform_ray(struct ray r, matrix4 m);


#endif
