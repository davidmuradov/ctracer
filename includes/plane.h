#ifndef INCLUDES_PLANE_H
#define INCLUDES_PLANE_H

#include "intersection.h"
#include "materials.h"

struct plane {
    t_object type;
    int id;
    struct matrix4 default_transformation;
    struct material material;
};

struct plane
plane_new_plane(void);

struct tuple
plane_normal_at(struct plane* plane, struct tuple point);

struct intersection_list
plane_intersect_ray(struct plane* plane, struct ray* r);

#endif
