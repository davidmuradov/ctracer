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
};

struct cylinder
cylinder_new_cylinder(void);

void
cylinder_make_inv_transform(struct cylinder* cyl);

void
cylinder_make_transp_inv_transform(struct cylinder* cyl);

#endif
