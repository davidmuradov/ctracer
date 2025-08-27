#ifndef INCLUDES_CYLINDER_H
#define INCLUDES_CYLINDER_H

#include "intersection.h"
#include "materials.h"

struct cylinder {
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

struct cylinder*
cylinder_new_cylinder(void);

struct intersection_list
cylinder_intersect_ray(struct cylinder* cyl, struct ray* r);

struct tuple
cylinder_normal_at(struct cylinder* cyl, struct tuple p);

void
cylinder_make_inv_transform(struct cylinder* cyl);

void
cylinder_make_transp_inv_transform(struct cylinder* cyl);

void
cylinder_add_transform(struct cylinder* cyl, struct matrix4 m);

void
cylinder_add_transform_to_pattern(struct cylinder* cyl, struct matrix4 m);

void
cylinder_delete_cylinder(struct cylinder* cyl);

#endif
