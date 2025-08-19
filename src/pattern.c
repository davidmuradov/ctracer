#include "../includes/pattern.h"
#include "ct_math.h"
#include "intersection.h"
#include "matrix.h"
#include "object_utils.h"
#include "tuple.h"
#include "world.h"
#include <math.h>

struct pattern
pattern_test(void) {
    struct pattern patt;

    patt.type = TEST;
    patt.color_a = tuple_new_color(0, 0, 0);
    patt.color_b = tuple_new_color(0, 0, 0);
    patt.default_transformation = matrix_make_identity4();

    return patt;
}

struct tuple
pattern_test_at(struct pattern pattern, struct tuple point) {
    return tuple_new_color(point.x, point.y, point.z);
}

struct tuple
pattern_test_at_object(struct pattern pattern, void* object, struct tuple world_point) {
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;
    t_object obj_type = world_get_object_type(object);
    struct matrix4 transform = object_utils_get_transform(object);

    switch (obj_type) {
	case SPHERE:
	    transform = maybe_sphere->default_transformation;
	    break;
	case PLANE:
	    transform = maybe_plane->default_transformation;
	    break;
	case CUBE:
	    transform = maybe_cube->default_transformation;
	    break;
	case CYLINDER:
	    transform = maybe_cylinder->default_transformation;
	    break;
	default:
	    transform = matrix_make_identity4();
	    break;
    }

    struct matrix4 inv_obj;
    matrix_inverse_matrix4(transform, &inv_obj); // Should check for inversion
    struct tuple obj_point = matrix_mult_matrix4_tuple(inv_obj, world_point);
    struct matrix4 inv_pat;
    matrix_inverse_matrix4(pattern.default_transformation, &inv_pat); // Should check for inversion
    struct tuple pattern_point = matrix_mult_matrix4_tuple(inv_pat, obj_point);

    return pattern_test_at(pattern, pattern_point);
}

struct pattern
pattern_stripe(struct tuple color_a, struct tuple color_b) {
    struct pattern patt;

    patt.type = STRIPE;
    patt.color_a = color_a;
    patt.color_b = color_b;
    patt.default_transformation = matrix_make_identity4();

    return patt;
}

struct tuple
pattern_stripe_at(struct pattern pattern, struct tuple point) {
    int x = floor(point.x);
    
    if (x % 2)
	return pattern.color_b;

    return pattern.color_a;
}

struct tuple
pattern_stripe_at_object(struct pattern pattern, void* object, struct tuple world_point) {
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;
    t_object obj_type = world_get_object_type(object);
    struct matrix4 transform;

    switch (obj_type) {
	case SPHERE:
	    transform = maybe_sphere->default_transformation;
	    break;
	case PLANE:
	    transform = maybe_plane->default_transformation;
	    break;
	case CUBE:
	    transform = maybe_cube->default_transformation;
	    break;
	case CYLINDER:
	    transform = maybe_cylinder->default_transformation;
	    break;
	default:
	    transform = matrix_make_identity4();
	    break;
    }

    struct matrix4 inv_obj;
    matrix_inverse_matrix4(transform, &inv_obj); // Should check for inversion
    struct tuple obj_point = matrix_mult_matrix4_tuple(inv_obj, world_point);
    struct matrix4 inv_pat;
    matrix_inverse_matrix4(pattern.default_transformation, &inv_pat); // Should check for inversion
    struct tuple pattern_point = matrix_mult_matrix4_tuple(inv_pat, obj_point);

    return pattern_stripe_at(pattern, pattern_point);
}

struct pattern
pattern_gradient(struct tuple color_a, struct tuple color_b) {
    struct pattern patt;

    patt.type = GRADIENT;
    patt.color_a = color_a;
    patt.color_b = color_b;
    patt.default_transformation = matrix_make_identity4();

    return patt;
}

struct tuple
pattern_gradient_at(struct pattern pattern, struct tuple point) {
    struct tuple distance = tuple_sub(pattern.color_b, pattern.color_a);
    double fraction = point.x - floor(point.x);

    return tuple_add(pattern.color_a, tuple_scalar_mult(distance, fraction));
}

struct tuple
pattern_gradient_at_object(struct pattern pattern, void* object, struct tuple world_point) {
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;
    t_object obj_type = world_get_object_type(object);
    struct matrix4 transform;

    switch (obj_type) {
	case SPHERE:
	    transform = maybe_sphere->default_transformation;
	    break;
	case PLANE:
	    transform = maybe_plane->default_transformation;
	    break;
	case CUBE:
	    transform = maybe_cube->default_transformation;
	    break;
	case CYLINDER:
	    transform = maybe_cylinder->default_transformation;
	    break;
	default:
	    transform = matrix_make_identity4();
	    break;
    }

    struct matrix4 inv_obj;
    matrix_inverse_matrix4(transform, &inv_obj); // Should check for inversion
    struct tuple obj_point = matrix_mult_matrix4_tuple(inv_obj, world_point);
    struct matrix4 inv_pat;
    matrix_inverse_matrix4(pattern.default_transformation, &inv_pat); // Should check for inversion
    struct tuple pattern_point = matrix_mult_matrix4_tuple(inv_pat, obj_point);

    return pattern_gradient_at(pattern, pattern_point);
}

struct pattern
pattern_ring(struct tuple color_a, struct tuple color_b) {
    struct pattern patt;

    patt.type = RING;
    patt.color_a = color_a;
    patt.color_b = color_b;
    patt.default_transformation = matrix_make_identity4();

    return patt;
}

struct tuple
pattern_ring_at(struct pattern pattern, struct tuple point) {
    int x = floor(sqrt(point.x * point.x + point.z * point.z));
    
    if (x % 2 == 0)
	return pattern.color_a;

    return pattern.color_b;
}

struct tuple
pattern_ring_at_object(struct pattern pattern, void* object, struct tuple world_point) {
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;
    t_object obj_type = world_get_object_type(object);
    struct matrix4 transform;

    switch (obj_type) {
	case SPHERE:
	    transform = maybe_sphere->default_transformation;
	    break;
	case PLANE:
	    transform = maybe_plane->default_transformation;
	    break;
	case CUBE:
	    transform = maybe_cube->default_transformation;
	    break;
	case CYLINDER:
	    transform = maybe_cylinder->default_transformation;
	    break;
	default:
	    transform = matrix_make_identity4();
	    break;
    }

    struct matrix4 inv_obj;
    matrix_inverse_matrix4(transform, &inv_obj); // Should check for inversion
    struct tuple obj_point = matrix_mult_matrix4_tuple(inv_obj, world_point);
    struct matrix4 inv_pat;
    matrix_inverse_matrix4(pattern.default_transformation, &inv_pat); // Should check for inversion
    struct tuple pattern_point = matrix_mult_matrix4_tuple(inv_pat, obj_point);

    return pattern_ring_at(pattern, pattern_point);
}

struct pattern
pattern_checker(struct tuple color_a, struct tuple color_b) {
    struct pattern patt;

    patt.type = CHECKER;
    patt.color_a = color_a;
    patt.color_b = color_b;
    patt.default_transformation = matrix_make_identity4();

    return patt;
}

struct tuple
pattern_checker_at(struct pattern pattern, struct tuple point) {
    int x = floor(point.x) + floor(point.y) + floor(point.z);
    
    if (x % 2 == 0)
	return pattern.color_a;

    return pattern.color_b;
}

struct tuple
pattern_checker_at_for_plane(struct pattern pattern, struct tuple point) {
    int x = floor(point.x) + floor(point.z);
    
    if (x % 2 == 0)
	return pattern.color_a;

    return pattern.color_b;
}

struct tuple
pattern_checker_at_object(struct pattern pattern, void* object, struct tuple world_point) {
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;
    t_object obj_type = world_get_object_type(object);
    struct matrix4 transform;

    switch (obj_type) {
	case SPHERE:
	    transform = maybe_sphere->default_transformation;
	    break;
	case PLANE:
	    transform = maybe_plane->default_transformation;
	    break;
	case CUBE:
	    transform = maybe_cube->default_transformation;
	    break;
	case CYLINDER:
	    transform = maybe_cylinder->default_transformation;
	    break;
	default:
	    transform = matrix_make_identity4();
	    break;
    }

    struct matrix4 inv_obj;
    matrix_inverse_matrix4(transform, &inv_obj); // Should check for inversion
    struct tuple obj_point = matrix_mult_matrix4_tuple(inv_obj, world_point);
    struct matrix4 inv_pat;
    matrix_inverse_matrix4(pattern.default_transformation, &inv_pat); // Should check for inversion
    struct tuple pattern_point = matrix_mult_matrix4_tuple(inv_pat, obj_point);

    if (obj_type == PLANE)
	return pattern_checker_at_for_plane(pattern, pattern_point);

    return pattern_checker_at(pattern, pattern_point);
}
