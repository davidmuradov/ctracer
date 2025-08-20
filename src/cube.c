#include "../includes/cube.h"
#include "../includes/intersection.h"
#include "../includes/materials.h"
#include "../includes/matrix.h"
#include "../includes/ray.h"
#include "../includes/tuple.h"
#include "ct_math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int UNIQUE_ID_CUBE = 0;

static void
cube_check_axis(const double o, const double d, double* t);

static double
cube_t_max(const double xtmin, const double ytmin, const double ztmin);

static double
cube_t_min(const double xtmax, const double ytmax, const double ztmax);

static double
cube_max(const double x, const double y, const double z);

struct cube
cube_new_cube(void) {
    struct cube c;
    c.transform = matrix_make_identity4();
    c.inv_transform = matrix_make_identity4();
    c.transp_inv_transform = matrix_make_identity4();
    c.type = CUBE;
    c.id = UNIQUE_ID_CUBE;
    c.material = materials_new_material();
    UNIQUE_ID_CUBE++;
    return c;
}

struct intersection_list
cube_intersect_ray(struct cube* c, struct ray* r) {
    /* -- COMMON TO ALL SHAPES -- */
    //struct matrix4 inv_transform;
    struct ray r2;
    /*
    int inverted = matrix_inverse_matrix4(c->transform, &inv_transform);
    if (inverted)
	r2 = ray_transform_ray(*r, inv_transform);
    else {
	fprintf(stderr, "Matrix is non invertible\n");
	exit(1);
	r2 = ray_transform_ray(*r, c->transform);
    }
    */
    r2 = ray_transform_ray(*r, c->inv_transform);
    /* -- END COMMON TO ALL SHAPES -- */

    struct intersection_list inter_list = intersection_new_intersection_list();
    struct intersection inter;
    double xt[2] = {0};
    double yt[2] = {0};
    double zt[2] = {0};

    cube_check_axis(r2.o.x, r2.dir.x, xt);
    cube_check_axis(r2.o.y, r2.dir.y, yt);
    cube_check_axis(r2.o.z, r2.dir.z, zt);

    double tmin = cube_t_max(xt[0], yt[0], zt[0]);
    double tmax = cube_t_min(xt[1], yt[1], zt[1]);

    if (tmin > tmax)
	return inter_list;

    inter = intersection_new_intersection(tmin, c);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(tmax, c);
    intersection_add_intersection_to_list(&inter_list, inter);

    return inter_list;
}

struct tuple
cube_normal_at(struct cube* c, struct tuple p) {
    /* -- COMMON TO ALL SHAPES -- */
    //struct matrix4 s_inv_transf;
    struct tuple o_point;
    //int inverted = matrix_inverse_matrix4(c->transform, &s_inv_transf);
    /*
    if (inverted)
	o_point = matrix_mult_matrix4_tuple(s_inv_transf, p);
    else {
	fprintf(stderr, "Matrix is non invertible\n");
	exit(1);
	o_point = matrix_mult_matrix4_tuple(c->transform, p);
    }
    */
    o_point = matrix_mult_matrix4_tuple(c->inv_transform, p);
    /* -- END COMMON TO ALL SHAPES -- */
    struct tuple o_normal;
    double maxc = cube_max(ctm_abs(o_point.x), ctm_abs(o_point.y), ctm_abs(o_point.z));

    if (ctm_floats_equal(maxc, ctm_abs(o_point.x)))
	o_normal = tuple_new_vector(o_point.x, 0, 0);
    else if (ctm_floats_equal(maxc, ctm_abs(o_point.y)))
	o_normal = tuple_new_vector(0, o_point.y, 0);
    else
	o_normal = tuple_new_vector(0, 0, o_point.z);

    /* -- COMMON TO ALL SHAPES -- */
    struct tuple w_normal = matrix_mult_matrix4_tuple(c->transp_inv_transform, o_normal);
    w_normal.w = 0;
    /* -- END COMMON TO ALL SHAPES -- */

    return tuple_normalize(w_normal);
}

void
cube_add_transform(struct cube* c, struct matrix4 m) {
    c->transform = matrix_mult_matrix4(m, c->transform);
}

void
cube_add_transform_to_pattern(struct cube* c, struct matrix4 m) {
    c->material.pattern.transform = matrix_mult_matrix4(m, c->material.pattern.transform);
}

void
cube_make_inv_transform(struct cube* c) {
    matrix_inverse_matrix4(c->transform, &(c->inv_transform));
}

void
cube_make_transp_inv_transform(struct cube* c) {
    c->transp_inv_transform = matrix_transpose4(c->inv_transform);
}

static void
cube_check_axis(const double o, const double d, double* t) {
    double tmin_num = -1 - o;
    double tmax_num = 1 - o;

    if (!ctm_floats_equal(d, 0)) {
	t[0] = tmin_num / d;
	t[1] = tmax_num / d;
    }
    else {
	t[0] = tmin_num * INFINITY;
	t[1] = tmax_num * INFINITY;
    }

    if (t[0] > t[1]) {
	double temp = t[0];
	t[0] = t[1];
	t[1] = temp;
    }
}

static double
cube_t_max(const double xtmin, const double ytmin, const double ztmin) {
    double tmin = xtmin;
    if (ytmin > tmin)
	tmin = ytmin;
    if (ztmin > tmin)
	tmin = ztmin;
    return tmin;
}

static double
cube_t_min(const double xtmax, const double ytmax, const double ztmax) {
    double tmax = xtmax;
    if (ytmax < tmax)
	tmax = ytmax;
    if (ztmax < tmax)
	tmax = ztmax;
    return tmax;
}

static double
cube_max(const double x, const double y, const double z) {
    double max = x;
    if (y > x)
	max = y;
    if (z > max)
	max = z;
    return max;
}
