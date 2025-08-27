#ifndef INCLUDES_PLANE_H
#define INCLUDES_PLANE_H

#include "intersection.h"
#include "materials.h"

struct plane {
    t_object type;
    int id;
    struct matrix4 transform;
    struct matrix4 inv_transform;
    struct matrix4 transp_inv_transform;
    struct material material;
    int xs_count;
    double xs[1];
};

struct plane*
plane_new_plane(void);

struct tuple
plane_normal_at(struct plane* plane, struct tuple point);

struct intersection_list
plane_intersect_ray(struct plane* plane, struct ray* r);

void
plane_add_transform(struct plane* p, struct matrix4 m);

void
plane_add_transform_to_pattern(struct plane* p, struct matrix4 m);

void
plane_make_inv_transform(struct plane* p);

void
plane_make_transp_inv_transform(struct plane* p);

void
plane_delete_plane(struct plane* plane);

#endif
