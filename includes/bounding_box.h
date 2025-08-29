#ifndef INCLUDES_BOUNDING_BOX_H
#define INCLUDES_BOUNDING_BOX_H

#include "matrix.h"
#include "ray.h"
#include "tuple.h"

struct bbox {
    struct tuple min;
    struct tuple max;
};

struct bbox*
bbox_new_empty_bbox(void);

struct bbox*
bbox_new_bbox(struct tuple min, struct tuple max);

void
bbox_add_point(struct bbox* box, struct tuple point);

void
bbox_add_bbox(struct bbox* box, struct bbox* child_box);

struct bbox*
bbox_bounds_of_shape(void* shape);

int
bbox_contains_point(struct bbox* box, struct tuple point);

int
bbox_contains_bbox(struct bbox* box, struct bbox* child_box);

int
bbox_contains_bbox(struct bbox* box, struct bbox* child_box);

struct bbox*
bbox_transform(struct bbox* box, struct matrix4 m);

int
bbox_intersect_ray(struct bbox* box, struct ray* ray);

void
bbox_delete_box(struct bbox* b);

#endif
