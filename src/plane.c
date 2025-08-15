#include "../includes/plane.h"
#include "../includes/intersection.h"
#include "../includes/materials.h"
#include "../includes/matrix.h"
#include "../includes/ray.h"
#include "../includes/tuple.h"
#include "ct_math.h"
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
    p.xs_count = 0;
    p.xs[0] = UNDEF_TIME;
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

struct intersection_list
plane_intersect_ray(struct plane* plane, struct ray* r) {

    /* -- COMMON TO ALL SHAPES -- */
    struct matrix4 inv_transform;
    int inverted = matrix_inverse_matrix4(plane->default_transformation, &inv_transform);
    struct ray r2;
    if (inverted)
	r2 = ray_transform_ray(*r, inv_transform);
    else {
	fprintf(stderr, "Matrix is non invertible\n");
	exit(1);
	r2 = ray_transform_ray(*r, plane->default_transformation);
    }
    /* -- END COMMON TO ALL SHAPES -- */

    struct intersection_list inter_list = intersection_new_intersection_list();

    if (ctm_floats_equal(r2.dir.y, 0))
	return inter_list;

    plane->xs_count = 1;
    double t = (-r2.o.y / r2.dir.y);
    plane->xs[0] = t;

    struct intersection inter = intersection_new_intersection(t, plane);
    intersection_add_intersection_to_list(&inter_list, inter);

    return inter_list;
}
