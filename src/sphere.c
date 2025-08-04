#include "../includes/sphere.h"
#include "intersection.h"
#include "materials.h"
#include "matrix.h"
#include "ray.h"
#include "tuple.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int UNIQUE_ID = 0;

struct sphere
sphere_new_sphere(struct tuple o, double r) {
    struct sphere s;
    s.default_transformation = matrix_make_identity4();
    s.type = SPHERE;
    s.id = UNIQUE_ID;
    s.o = o;
    s.radius = r;
    s.material = materials_new_material();
    s.xs_count = 0;
    s.xs[0] = UNDEF_TIME;
    s.xs[1] = UNDEF_TIME;
    UNIQUE_ID++;
    return s;
}

struct intersection_list
sphere_intersect_ray(struct sphere* s, struct ray* r) {
    struct matrix4 inv_transform;
    int inverted = matrix_inverse_matrix4(s->default_transformation, &inv_transform);
    struct ray r2;
    if (inverted)
	r2 = ray_transform_ray(*r, inv_transform);
    else {
	fprintf(stderr, "Matrix is non invertible\n");
	exit(1);
	r2 = ray_transform_ray(*r, s->default_transformation);
    }
    struct intersection_list inter_list = intersection_new_intersection_list();
    struct tuple sphere_to_ray = tuple_sub(r2.o, s->o);
    double a = tuple_dot(r2.dir, r2.dir);
    double b = 2 * tuple_dot(r2.dir, sphere_to_ray);
    double c = tuple_dot(sphere_to_ray, sphere_to_ray) - 1;
    s->xs_count = 0;
    s->xs[0] = UNDEF_TIME;
    s->xs[1] = UNDEF_TIME;

    double delta = b*b -4*a*c;

    if (delta < 0)
	return inter_list;

    s->xs_count = 2;
    double t1 = (-b -sqrt(delta)) / (2*a);
    double t2 = (-b +sqrt(delta)) / (2*a);

    if (t1 < t2) {
	struct intersection inter;
	s->xs[0] = t1;
	inter = intersection_new_intersection(s->xs[0], s);
	intersection_add_intersection_to_list(&inter_list, inter);
	s->xs[1] = t2;
	inter = intersection_new_intersection(s->xs[1], s);
	intersection_add_intersection_to_list(&inter_list, inter);
    }
    else {
	struct intersection inter;
	s->xs[0] = t2;
	inter = intersection_new_intersection(s->xs[0], s);
	intersection_add_intersection_to_list(&inter_list, inter);
	s->xs[1] = t1;
	inter = intersection_new_intersection(s->xs[1], s);
	intersection_add_intersection_to_list(&inter_list, inter);
    }

    return inter_list;
}

void
sphere_set_transform(struct sphere* s, struct matrix4 m) {
    s->default_transformation = m;
}

struct tuple
sphere_normal_at(struct sphere* s, struct tuple p) {
    struct matrix4 s_inv_transf;
    struct tuple o_point;
    int inverted = matrix_inverse_matrix4(s->default_transformation, &s_inv_transf);
    if (inverted)
	o_point = matrix_mult_matrix4_tuple(s_inv_transf, p);
    else {
	fprintf(stderr, "Matrix is non invertible\n");
	exit(1);
	o_point = matrix_mult_matrix4_tuple(s->default_transformation, p);
    }
    struct tuple o_normal = tuple_sub(o_point, s->o); // or (0,0,0), to be determined
    struct tuple w_normal = matrix_mult_matrix4_tuple(matrix_transpose4(s_inv_transf), o_normal);
    w_normal.w = 0;
    return tuple_normalize(w_normal);
}

struct tuple
sphere_reflect(struct tuple v, struct tuple n) {
    double t = tuple_dot(v, n);
    t *= 2;
    return tuple_sub(v, tuple_scalar_mult(n, t));
}

void
sphere_set_material(struct sphere* s, struct material m) {
    s->material = m;
}
