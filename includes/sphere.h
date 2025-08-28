#ifndef INCLUDES_SPHERE_H
#define INCLUDES_SPHERE_H

#include "intersection.h"
#include "materials.h"
#include "matrix.h"
#include "ray.h"
#include "ct_math.h"

struct sphere {
    t_object type;
    int id;
    struct matrix4 transform;
    struct matrix4 inv_transform;
    struct matrix4 transp_inv_transform;
    struct material material;
    struct tuple o;
    double radius;
    int xs_count;
    double xs[2];
};

struct sphere*
sphere_new_sphere(void);

struct sphere
sphere_new_glass_sphere(void);

struct intersection_list
sphere_intersect_ray(struct sphere* s, struct ray* r);

void
sphere_set_transform(struct sphere* s, struct matrix4 m);

struct tuple
sphere_normal_at(struct sphere* s, struct tuple p);

struct tuple
sphere_reflect(struct tuple v, struct tuple n);

void
sphere_set_material(struct sphere* s, struct material m);

void
sphere_add_transform(struct sphere* s, struct matrix4 m);

void
sphere_add_transform_to_pattern(struct sphere* s, struct matrix4 m);

void
sphere_make_inv_transform(struct sphere* s);

void
sphere_make_transp_inv_transform(struct sphere* s);

void
sphere_delete_sphere(struct sphere* s);

#endif
