#include "../includes/ray.h"
#include "matrix.h"
#include "tuple.h"

struct ray
ray_new_ray(struct tuple o, struct tuple d) {
    struct ray r;
    r.o = o;
    r.dir = d;
    return r;
}

struct tuple
ray_position_at(struct ray* r, double t) {
    return tuple_add(r->o, tuple_scalar_mult(r->dir, t));
}

struct ray
ray_transform_ray(struct ray r, struct matrix4 m) {
    struct ray new_ray;
    new_ray.o = matrix_mult_matrix4_tuple(m, r.o);
    new_ray.dir = matrix_mult_matrix4_tuple(m, r.dir);
    return new_ray;
}
