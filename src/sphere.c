#include "../includes/sphere.h"
#include "intersection.h"
#include "matrix.h"
#include "tuple.h"
#include <math.h>

static int UNIQUE_ID = 0;

void
sphere_init(struct sphere* s, struct tuple o, double r) {
    s->default_transformation = matrix_make_identity4();
    s->type = SPHERE;
    s->id = UNIQUE_ID;
    s->o = o;
    s->radius = r;
    s->xs_count = 0;
    s->xs[0] = UNDEF_TIME;
    s->xs[1] = UNDEF_TIME;
    UNIQUE_ID++;
}

void
sphere_intersect_ray(struct sphere* s, struct ray* r, struct intersection_list* inter_list) {
    struct tuple sphere_to_ray = tuple_sub(r->o, s->o);
    double a = tuple_dot(r->dir, r->dir);
    double b = 2 * tuple_dot(r->dir, sphere_to_ray);
    double c = tuple_dot(sphere_to_ray, sphere_to_ray) - 1;
    s->xs_count = 0;
    s->xs[0] = UNDEF_TIME;
    s->xs[1] = UNDEF_TIME;

    double delta = b*b -4*a*c;

    if (delta < 0)
	return;

    s->xs_count = 2;
    double t1 = (-b -sqrt(delta)) / (2*a);
    double t2 = (-b +sqrt(delta)) / (2*a);

    if (t1 < t2) {
	struct intersection inter;
	s->xs[0] = t1;
	intersection_new_intersection(&inter, s->xs[0], s);
	intersection_add_intersection_to_list(inter_list, inter);
	s->xs[1] = t2;
	intersection_new_intersection(&inter, s->xs[1], s);
	intersection_add_intersection_to_list(inter_list, inter);
    }
    else {
	struct intersection inter;
	s->xs[0] = t2;
	intersection_new_intersection(&inter, s->xs[0], s);
	intersection_add_intersection_to_list(inter_list, inter);
	s->xs[1] = t1;
	intersection_new_intersection(&inter, s->xs[1], s);
	intersection_add_intersection_to_list(inter_list, inter);
    }
}

void
sphere_set_transform(struct sphere* s, struct matrix4 m) {
    s->default_transformation = m;
}
