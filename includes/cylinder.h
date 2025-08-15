#ifndef INCLUDES_CYLINDER_H
#define INCLUDES_CYLINDER_H

#include "intersection.h"
#include "materials.h"

struct cylinder {
    t_object type;
    int id;
    struct matrix4 default_transformation;
    struct material material;
};

struct cylinder
cylinder_new_cylinder(void);

#endif
