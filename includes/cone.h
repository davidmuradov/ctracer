#ifndef INCLUDES_CONE_H
#define INCLUDES_CONE_H

#include "intersection.h"
#include "materials.h"

struct cone {
    t_object type;
    int id;
    struct matrix4 transform;
    struct matrix4 inv_transform;
    struct matrix4 transp_inv_transform;
    struct material material;
    double min;
    double max;
    int closed;
};

struct cone*
cone_new_cone(void);

struct intersection_list
cone_intersect_ray(struct cone* co, struct ray* r);

struct tuple
cone_normal_at(struct cone* co, struct tuple p);

void
cone_make_inv_transform(struct cone* co);

void
cone_make_transp_inv_transform(struct cone* co);

void
cone_add_transform(struct cone* co, struct matrix4 m);

void
cone_add_transform_to_pattern(struct cone* co, struct matrix4 m);

void
cone_delete_cone(struct cone* co);

#endif
