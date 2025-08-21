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

static int UNIQUE_ID_CYLINDER = 0;

static int
cylinder_check_cap(struct ray ray, double t);

static void
cylinder_intersect_caps(struct cylinder* cyl, struct ray ray, struct intersection_list* inter_list);

struct cylinder
cylinder_new_cylinder(void) {
    struct cylinder c;
    c.transform = matrix_make_identity4();
    c.inv_transform = matrix_make_identity4();
    c.transp_inv_transform = matrix_make_identity4();
    c.type = CYLINDER;
    c.id = UNIQUE_ID_CYLINDER;
    c.material = materials_new_material();
    c.min = -INFINITY;
    c.max = INFINITY;
    c.closed = 0;
    UNIQUE_ID_CYLINDER++;
    return c;
}

struct intersection_list
cylinder_intersect_ray(struct cylinder* cyl, struct ray* r) {
    /* -- COMMON TO ALL SHAPES -- */
    struct ray r2 = ray_transform_ray(*r, cyl->inv_transform);
    /* -- END COMMON TO ALL SHAPES -- */

    struct intersection_list inter_list = intersection_new_intersection_list();

    double a = r2.dir.x * r2.dir.x + r2.dir.z * r2.dir.z;

    if (!ctm_floats_equal(a, 0)) {
	double b = 2 * r2.o.x * r2.dir.x + 2 * r2.o.z * r2.dir.z;
	double c = r2.o.x * r2.o.x + r2.o.z * r2.o.z - 1;
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

	if (cyl->min < y0 && y0 < cyl->max)
	    intersection_add_intersection_to_list(&inter_list, intersection_new_intersection(t0, cyl));

	double y1 = r2.o.y + t1 * r2.dir.y;

	if (cyl->min < y1 && y1 < cyl->max)
	    intersection_add_intersection_to_list(&inter_list, intersection_new_intersection(t1, cyl));
    }

    cylinder_intersect_caps(cyl, r2, &inter_list);

    return inter_list;
}

struct tuple
cylinder_normal_at(struct cylinder* cyl, struct tuple p) {

    /* -- COMMON TO ALL SHAPES -- */
    struct tuple o_point = matrix_mult_matrix4_tuple(cyl->inv_transform, p);
    /* -- END COMMON TO ALL SHAPES -- */
    struct tuple o_normal;

    double dist = o_point.x * o_point.x + o_point.z * o_point.z;

    if (dist < 1 && o_point.y >= cyl->max - EPSILON)
	o_normal = tuple_new_vector(0, 1, 0);
    else if (dist < 1 && o_point.y <= cyl->min + EPSILON)
	o_normal = tuple_new_vector(0, -1, 0);
    else
	o_normal = tuple_new_vector(o_point.x, 0, o_point.z);

    /* -- COMMON TO ALL SHAPES -- */
    struct tuple w_normal = matrix_mult_matrix4_tuple(cyl->transp_inv_transform, o_normal);
    w_normal.w = 0;
    /* -- END COMMON TO ALL SHAPES -- */

    return tuple_normalize(w_normal);
}

void
cylinder_make_inv_transform(struct cylinder* cyl) {
    matrix_inverse_matrix4(cyl->transform, &(cyl->inv_transform));
}

void
cylinder_make_transp_inv_transform(struct cylinder* cyl) {
    cyl->transp_inv_transform = matrix_transpose4(cyl->inv_transform);
}

void
cylinder_add_transform(struct cylinder* cyl, struct matrix4 m) {
    cyl->transform = matrix_mult_matrix4(m, cyl->transform);
}

void
cylinder_add_transform_to_pattern(struct cylinder* cyl, struct matrix4 m) {
    cyl->material.pattern.transform = matrix_mult_matrix4(m, cyl->material.pattern.transform);
}

static int
cylinder_check_cap(struct ray ray, double t) {
    double x = ray.o.x + t * ray.dir.x;
    double z = ray.o.z + t * ray.dir.z;

    if (x * x + z * z <= 1)
	return 1;

    return 0;
}

static void
cylinder_intersect_caps(struct cylinder* cyl, struct ray ray, struct intersection_list* inter_list) {
    if (!cyl->closed || ctm_floats_equal(ray.dir.y, 0))
	return;

    double t = (cyl->min - ray.o.y) / ray.dir.y;

    if (cylinder_check_cap(ray, t))
	intersection_add_intersection_to_list(inter_list, intersection_new_intersection(t, cyl));

    t = (cyl->max - ray.o.y) / ray.dir.y;

    if (cylinder_check_cap(ray, t))
	intersection_add_intersection_to_list(inter_list, intersection_new_intersection(t, cyl));
}
