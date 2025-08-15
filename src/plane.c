#include "../includes/plane.h"
#include "../includes/intersection.h"
#include "../includes/materials.h"
#include "../includes/matrix.h"
#include "../includes/ray.h"
#include "../includes/tuple.h"
#include "ct_math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int UNIQUE_ID_PLANE = 0;

struct plane
plane_new_plane(void) {
    struct plane p;
    p.default_transformation = matrix_make_identity4();
    p.type = PLANE;
    p.id = UNIQUE_ID_PLANE;
    p.material = materials_new_material();
    UNIQUE_ID_PLANE++;
    return p;
}

struct tuple
plane_normal_at(struct plane* plane, struct tuple point) {

    /* -- COMMON TO ALL SHAPES -- */
    struct matrix4 s_inv_transf;
    struct tuple o_point;
    int inverted = matrix_inverse_matrix4(plane->default_transformation, &s_inv_transf);
    if (inverted)
	o_point = matrix_mult_matrix4_tuple(s_inv_transf, point);
    else {
	fprintf(stderr, "Matrix is non invertible\n");
	exit(1);
	o_point = matrix_mult_matrix4_tuple(plane->default_transformation, point);
    }
    /* -- END COMMON TO ALL SHAPES -- */
    struct tuple o_normal;
    if (ctm_floats_equal(o_point.y, 0))
	o_normal = tuple_new_vector(0, 1, 0);
    else
	o_normal = tuple_new_vector(0, 0, 0);

    /* -- COMMON TO ALL SHAPES -- */
    struct tuple w_normal = matrix_mult_matrix4_tuple(matrix_transpose4(s_inv_transf), o_normal);
    w_normal.w = 0;
    /* -- END COMMON TO ALL SHAPES -- */

    if (tuple_equals(tuple_new_vector(0, 0, 0), w_normal))
	return tuple_new_vector(0, 0, 0);

    return tuple_normalize(w_normal);
}

//struct intersection_list
//plane_intersect_ray(struct plane* plane, struct ray* r) {
//
//    /* -- COMMON TO ALL SHAPES -- */
//    struct matrix4 inv_transform;
//    int inverted = matrix_inverse_matrix4(plane->default_transformation, &inv_transform);
//    struct ray r2;
//    if (inverted)
//	r2 = ray_transform_ray(*r, inv_transform);
//    else {
//	fprintf(stderr, "Matrix is non invertible\n");
//	exit(1);
//	r2 = ray_transform_ray(*r, plane->default_transformation);
//    }
//    /* -- END COMMON TO ALL SHAPES -- */
//    struct intersection_list inter_list = intersection_new_intersection_list();
//    struct tuple sphere_to_ray = tuple_sub(r2.o, s->o);
//    double a = tuple_dot(r2.dir, r2.dir);
//    double b = 2 * tuple_dot(r2.dir, sphere_to_ray);
//    double c = tuple_dot(sphere_to_ray, sphere_to_ray) - 1;
//    s->xs_count = 0;
//    s->xs[0] = UNDEF_TIME;
//    s->xs[1] = UNDEF_TIME;
//
//    double delta = b*b -4*a*c;
//
//    if (delta < 0)
//	return inter_list;
//
//    s->xs_count = 2;
//    double t1 = (-b -sqrt(delta)) / (2*a);
//    double t2 = (-b +sqrt(delta)) / (2*a);
//
//    if (t1 < t2) {
//	struct intersection inter;
//	s->xs[0] = t1;
//	inter = intersection_new_intersection(s->xs[0], s);
//	intersection_add_intersection_to_list(&inter_list, inter);
//	s->xs[1] = t2;
//	inter = intersection_new_intersection(s->xs[1], s);
//	intersection_add_intersection_to_list(&inter_list, inter);
//    }
//    else {
//	struct intersection inter;
//	s->xs[0] = t2;
//	inter = intersection_new_intersection(s->xs[0], s);
//	intersection_add_intersection_to_list(&inter_list, inter);
//	s->xs[1] = t1;
//	inter = intersection_new_intersection(s->xs[1], s);
//	intersection_add_intersection_to_list(&inter_list, inter);
//    }
//
//    return inter_list;
//}
