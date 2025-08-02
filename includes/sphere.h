#ifndef INCLUDES_SPHERE_H
#define INCLUDES_SPHERE_H

#include "intersection.h"
#include "matrix.h"
#include "ray.h"

#define UNDEF_TIME	(-9999)

struct sphere {
    struct matrix4 default_transformation;
    t_object type;
    int id;
    struct tuple o;
    double radius;
    int xs_count;
    double xs[2];
};

struct sphere
sphere_new_sphere(struct tuple o, double r);

struct intersection_list
sphere_intersect_ray(struct sphere* s, struct ray* r);

void
sphere_set_transform(struct sphere* s, struct matrix4 m);

#endif
