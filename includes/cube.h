#ifndef INCLUDES_CUBE_H
#define INCLUDES_CUBE_H

#include "intersection.h"
#include "materials.h"

struct cube {
    t_object type;
    int id;
    struct matrix4 default_transformation;
    struct material material;
};

struct cube
cube_new_cube(void);

struct intersection_list
cube_intersect_ray(struct cube* c, struct ray* r);

struct tuple
cube_normal_at(struct cube* c, struct tuple p);

#endif
