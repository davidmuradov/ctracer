#include "../includes/bounding_box.h"
#include "ct_math.h"
#include "intersection.h"
#include "matrix.h"
#include "object_utils.h"
#include "tuple.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static struct bbox*
bbox_for_group(struct group* g);

static void
bbox_check_axis(const double o, const double d, double* t, double min, double max);

static double
bbox_t_max(const double xtmin, const double ytmin, const double ztmin);

static double
bbox_t_min(const double xtmax, const double ytmax, const double ztmax);

static double
cube_max(const double x, const double y, const double z);

struct bbox*
bbox_new_empty_bbox(void) {
    struct bbox* b = malloc(sizeof(struct bbox));
    if (!b) {
	fprintf(stderr, "Failed to allocate memory for bounding box\n");
	exit(1);
    }

    b->min = tuple_new_point(INFINITY, INFINITY, INFINITY);
    b->max = tuple_new_point(-INFINITY, -INFINITY, -INFINITY);

    return b;
}

struct bbox*
bbox_new_bbox(struct tuple min, struct tuple max) {
    struct bbox* b = bbox_new_empty_bbox();

    b->min = min;
    b->max = max;

    return b;
}

void
bbox_add_point(struct bbox* box, struct tuple point) {
    box->min.x = (point.x < box->min.x) ? (point.x):(box->min.x);
    box->min.y = (point.y < box->min.y) ? (point.y):(box->min.y);
    box->min.z = (point.z < box->min.z) ? (point.z):(box->min.z);

    box->max.x = (point.x > box->max.x) ? (point.x):(box->max.x);
    box->max.y = (point.y > box->max.y) ? (point.y):(box->max.y);
    box->max.z = (point.z > box->max.z) ? (point.z):(box->max.z);
}

void
bbox_add_bbox(struct bbox* box, struct bbox* child_box) {
    bbox_add_point(box, child_box->min);
    bbox_add_point(box, child_box->max);
}

struct bbox*
bbox_bounds_of_shape(void* shape) {
    struct bbox* b = bbox_new_empty_bbox();
    struct bbox* new_box;
    t_object obj_type = object_utils_get_object_type(shape);
    struct sphere* maybe_sphere = shape;
    struct plane* maybe_plane = shape;
    struct cube* maybe_cube = shape;
    struct cylinder* maybe_cylinder = shape;
    struct cone* maybe_cone = shape;
    //struct triangle* maybe_triangle = shape;
    double a;
    double bb;
    double limit;
    switch (obj_type) {
	case SPHERE:
	    b->min = tuple_new_point(-1, -1, -1);
	    b->max = tuple_new_point(1, 1, 1);
	    new_box = bbox_transform(b, maybe_sphere->transform);
	    bbox_delete_box(b);
	    break;
	case PLANE:
	    b->min = tuple_new_point(-INFINITY, 0, -INFINITY);
	    b->max = tuple_new_point(INFINITY, 0, INFINITY);
	    new_box = bbox_transform(b, maybe_plane->transform);
	    bbox_delete_box(b);
	    break;
	case CUBE:
	    b->min = tuple_new_point(-1, -1, -1);
	    b->max = tuple_new_point(1, 1, 1);
	    new_box = bbox_transform(b, maybe_cube->transform);
	    bbox_delete_box(b);
	    break;
	case CYLINDER:
	    b->min = tuple_new_point(-1, maybe_cylinder->min, -1);
	    b->max = tuple_new_point(1, maybe_cylinder->max, 1);
	    new_box = bbox_transform(b, maybe_cylinder->transform);
	    bbox_delete_box(b);
	    break;
	case CONE:
	    a = ctm_abs(maybe_cone->min);
	    bb = ctm_abs(maybe_cone->max);
	    limit = ctm_max(a, bb);
	    b->min = tuple_new_point(-limit, maybe_cone->min, -limit);
	    b->max = tuple_new_point(limit, maybe_cone->max, limit);
	    new_box = bbox_transform(b, maybe_cone->transform);
	    bbox_delete_box(b);
	    break;
	case TRIANGLE:
	    //bbox_add_point(b, tuple_new_point(maybe_triangle->p1, maybe_triangle->p2, maybe_triangle->p3));
	    //new_box = bbox_transform(b, maybe_triangle->transform);
	    //bbox_delete_box(b);
	    break;
	case GROUP:
	    new_box = bbox_for_group((struct group*) shape);
	    bbox_delete_box(b);
	    break;
	default:
	    break;
    }

    return new_box;
}

int
bbox_contains_point(struct bbox* box, struct tuple point) {
    if ((point.x >= box->min.x && point.x <= box->max.x) &&
	    (point.y >= box->min.y && point.y <= box->max.y) &&
	    (point.z >= box->min.z && point.z <= box->max.z))
	return 1;

    return 0;
}

int
bbox_contains_bbox(struct bbox* box, struct bbox* child_box) {
    if (bbox_contains_point(box, child_box->min) && bbox_contains_point(box, child_box->max))
	return 1;

    return 0;
}

struct bbox*
bbox_transform(struct bbox* box, struct matrix4 m) {
    struct tuple p[8];
    p[0] = box->min;
    p[1] = tuple_new_point(box->min.x, box->min.y, box->max.z);
    p[2] = tuple_new_point(box->min.x, box->max.y, box->min.z);
    p[3] = tuple_new_point(box->min.x, box->max.y, box->max.z);
    p[4] = tuple_new_point(box->max.x, box->min.y, box->min.z);
    p[5] = tuple_new_point(box->max.x, box->min.y, box->max.z);
    p[6] = tuple_new_point(box->max.x, box->max.y, box->min.z);
    p[7] = box->max;

    struct bbox* new_box = bbox_new_empty_bbox();

    for (int i = 0; i < 8; i++)
	bbox_add_point(new_box, matrix_mult_matrix4_tuple(m, p[i]));

    return new_box;
}

int
bbox_intersect_ray(struct bbox* box, struct ray* ray) {
    double xt[2] = {0};
    double yt[2] = {0};
    double zt[2] = {0};

    bbox_check_axis(ray->o.x, ray->dir.x, xt, box->min.x, box->max.x);
    bbox_check_axis(ray->o.y, ray->dir.y, yt, box->min.y, box->max.y);
    bbox_check_axis(ray->o.z, ray->dir.z, zt, box->min.z, box->max.z);

    double tmin = bbox_t_max(xt[0], yt[0], zt[0]);
    double tmax = bbox_t_min(xt[1], yt[1], zt[1]);

    if (tmin > tmax)
	return 0;

    return 1;
}

void
bbox_split_bounds(struct bbox* box, struct bbox* split[2]) {
    double dx = box->max.x - box->min.x;
    double dy = box->max.y - box->min.y;
    double dz = box->max.z - box->min.z;

    double greatest = ctm_max(ctm_max(dx, dy), dz);

    struct tuple p0 = tuple_new_point(box->min.x, box->min.y, box->min.z);
    struct tuple p1 = tuple_new_point(box->max.x, box->max.y, box->max.z);

    if (ctm_floats_equal(greatest, dx))
	p0.x = p1.x = p0.x + (dx / 2);
    else if (ctm_floats_equal(greatest, dy))
	p0.y = p1.y = p0.y + (dy / 2);
    else
	p0.z = p1.z = p0.z + (dz / 2);

    struct tuple mid_min = tuple_new_point(p0.x, p0.y, p0.z);
    struct tuple mid_max = tuple_new_point(p1.x, p1.y, p1.z);

    split[0] = bbox_new_bbox(box->min, mid_max);
    split[1] = bbox_new_bbox(mid_min, box->max);
}

void
bbox_delete_box(struct bbox* b) {
    free(b);
    b = NULL;
}

static struct bbox*
bbox_for_group(struct group* g) {
    struct bbox* box = bbox_new_empty_bbox();
    struct bbox* cbox;

    for (int i = 0; i < g->nb_children; i++) {
	cbox = bbox_bounds_of_shape(g->list_children[i]);
	bbox_add_bbox(box, cbox);
	bbox_delete_box(cbox);
    }

    return box;
}

static void
bbox_check_axis(const double o, const double d, double* t, double min, double max) {
    double tmin_num = min - o;
    double tmax_num = max - o;

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
bbox_t_max(const double xtmin, const double ytmin, const double ztmin) {
    double tmin = xtmin;
    if (ytmin > tmin)
	tmin = ytmin;
    if (ztmin > tmin)
	tmin = ztmin;
    return tmin;
}

static double
bbox_t_min(const double xtmax, const double ytmax, const double ztmax) {
    double tmax = xtmax;
    if (ytmax < tmax)
	tmax = ytmax;
    if (ztmax < tmax)
	tmax = ztmax;
    return tmax;
}

static double
bbox_max(const double x, const double y, const double z) {
    double max = x;
    if (y > x)
	max = y;
    if (z > max)
	max = z;
    return max;
}
