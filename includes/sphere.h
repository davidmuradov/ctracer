#ifndef INCLUDES_SPHERE_H
#define INCLUDES_SPHERE_H

#include "intersection.h"
#include "matrix.h"
#include "ray.h"

#define UNDEF_TIME	(-9999)

struct sphere {
    matrix4 default_transformation;
    t_object type;
    int id;
    struct tuple o;
    double radius;
    int xs_count;
    double xs[2];
};

void
sphere_init(struct sphere* s, struct tuple o, double r);

void
sphere_intersect_ray(struct sphere* s, struct ray* r, struct intersection_list* inter_list);

void
sphere_set_transform(struct sphere* s, matrix4 m);

#endif
