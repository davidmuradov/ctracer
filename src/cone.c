#include "../includes/cone.h"

#include "../includes/cylinder.h"
#include "../includes/intersection.h"
#include "../includes/materials.h"
#include "../includes/matrix.h"
#include "../includes/ray.h"
#include "../includes/tuple.h"
#include "ct_math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int UNIQUE_ID_CONE = 0;

static int
cone_check_cap(struct ray ray, double t, double py);

static void
cone_intersect_caps(struct cone* co, struct ray ray, struct intersection_list* inter_list);

struct cone*
cone_new_cone(void) {
    struct cone* co = malloc(sizeof(struct cone));
    if (!co) {
	fprintf(stderr, "Error allocating memory for sphere\n");
	exit(1);
    }
    co->transform = matrix_make_identity4();
    co->inv_transform = matrix_make_identity4();
    co->transp_inv_transform = matrix_make_identity4();
    co->type = CONE;
    co->id = UNIQUE_ID_CONE;
    co->material = materials_new_material();
    co->min = -INFINITY;
    co->max = INFINITY;
    co->closed = 0;
    UNIQUE_ID_CONE++;
    return co;
}

struct intersection_list
cone_intersect_ray(struct cone* co, struct ray* r) {
    /* -- COMMON TO ALL SHAPES -- */
    struct ray r2 = ray_transform_ray(*r, co->inv_transform);
    /* -- END COMMON TO ALL SHAPES -- */

    struct intersection_list inter_list = intersection_new_intersection_list();

    double a = r2.dir.x * r2.dir.x - r2.dir.y * r2.dir.y + r2.dir.z * r2.dir.z;
    double b = 2 * r2.o.x * r2.dir.x - 2 * r2.o.y * r2.dir.y + 2 * r2.o.z * r2.dir.z;
    double c;

    if (ctm_floats_equal(a, 0) && ctm_floats_equal(b, 0))
	return inter_list;
    else if (ctm_floats_equal(a, 0) && !ctm_floats_equal(b, 0)) {
	c = r2.o.x * r2.o.x - r2.o.y * r2.o.y + r2.o.z * r2.o.z;
	double t = -c / (2 * b);
	intersection_add_intersection_to_list(&inter_list, intersection_new_intersection(t, co));
    }
    else {
	c = r2.o.x * r2.o.x - r2.o.y * r2.o.y + r2.o.z * r2.o.z;
	double delta = b * b - 4 * a * c;

	if (delta < 0)
	    return inter_list;

	double t0 = (-b - sqrt(delta)) / (2 * a);
	double t1 = (-b + sqrt(delta)) / (2 * a);

	if (t0 > t1) {
	    double temp = t0;
	    t0 = t1;
	    t1 = temp;
	}

	double y0 = r2.o.y + t0 * r2.dir.y;

	if (co->min < y0 && y0 < co->max)
	    intersection_add_intersection_to_list(&inter_list, intersection_new_intersection(t0, co));

	double y1 = r2.o.y + t1 * r2.dir.y;

	if (co->min < y1 && y1 < co->max)
	    intersection_add_intersection_to_list(&inter_list, intersection_new_intersection(t1, co));
    }

    cone_intersect_caps(co, r2, &inter_list);

    return inter_list;
}

struct tuple
cone_normal_at(struct cone* co, struct tuple p) {

    /* -- COMMON TO ALL SHAPES -- */
    struct tuple o_point = matrix_mult_matrix4_tuple(co->inv_transform, p);
    /* -- END COMMON TO ALL SHAPES -- */
    struct tuple o_normal;

    double dist = o_point.x * o_point.x + o_point.z * o_point.z;

    if (dist < ctm_abs(co->max) * ctm_abs(co->max) && o_point.y >= co->max - EPSILON)
	o_normal = tuple_new_vector(0, 1, 0);
    else if (dist < ctm_abs(co->min) * ctm_abs(co->min) && o_point.y <= co->min + EPSILON)
	o_normal = tuple_new_vector(0, -1, 0);
    else {
	double y = sqrt(dist);
	y = (o_point.y > 0) ? (-y):(y);
	o_normal = tuple_new_vector(o_point.x, y, o_point.z);
    }

    /* -- COMMON TO ALL SHAPES -- */
    struct tuple w_normal = matrix_mult_matrix4_tuple(co->transp_inv_transform, o_normal);
    w_normal.w = 0;
    /* -- END COMMON TO ALL SHAPES -- */

    if (tuple_equals(w_normal, tuple_new_vector(0, 0, 0)))
	return w_normal;

    return tuple_normalize(w_normal);
    //return w_normal;
}

void
cone_make_inv_transform(struct cone* co) {
    matrix_inverse_matrix4(co->transform, &(co->inv_transform));
}

void
cone_make_transp_inv_transform(struct cone* co) {
    co->transp_inv_transform = matrix_transpose4(co->inv_transform);
}

void
cone_add_transform(struct cone* co, struct matrix4 m) {
    co->transform = matrix_mult_matrix4(m, co->transform);
}

void
cone_add_transform_to_pattern(struct cone* co, struct matrix4 m) {
    co->material.pattern.transform = matrix_mult_matrix4(m, co->material.pattern.transform);
}

void
cone_delete_cone(struct cone* co) {
    free(co);
    co = NULL;
}

static int
cone_check_cap(struct ray ray, double t, double py) {
    double x = ray.o.x + t * ray.dir.x;
    double z = ray.o.z + t * ray.dir.z;

    if (x * x + z * z < ctm_abs(py) * ctm_abs(py))
	return 1;

    return 0;
}

static void
cone_intersect_caps(struct cone* co, struct ray ray, struct intersection_list* inter_list) {
    if (!co->closed || ctm_floats_equal(ray.dir.y, 0))
	return;

    double t = (co->min - ray.o.y) / ray.dir.y;

    if (cone_check_cap(ray, t, co->min))
	intersection_add_intersection_to_list(inter_list, intersection_new_intersection(t, co));

    t = (co->max - ray.o.y) / ray.dir.y;

    if (cone_check_cap(ray, t, co->max))
	intersection_add_intersection_to_list(inter_list, intersection_new_intersection(t, co));
}
