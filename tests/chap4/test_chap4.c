#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/tuple.h"
#include "../../includes/ct_math.h"
#include "../../includes/canvas.h"
#include "../../includes/matrix.h"

int main(int argc, char *argv[]) {

    struct matrix4 m_translate = matrix_new_translation4(5, -3, 2);

    struct tuple p = tuple_new_point(-3, 4, 5);
    struct tuple p_res = matrix_mult_matrix4_tuple(m_translate, p);
    struct tuple p_exp = tuple_new_point(2, 1, 7);
    assert(tuple_equals(p_res, p_exp));

    struct matrix4 inv_m_translate;
    matrix_inverse_matrix4(m_translate, &inv_m_translate);
    p_res = matrix_mult_matrix4_tuple(inv_m_translate, p);
    p_exp = tuple_new_point(-8, 7, 3);
    assert(tuple_equals(p_res, p_exp));
    struct tuple p_vec = tuple_new_vector(-3, 4, 5);
    struct tuple p_vec_res = matrix_mult_matrix4_tuple(m_translate, p_vec);
    assert(tuple_equals(p_vec_res, p_vec));

    struct matrix4 m_scaling = matrix_new_scaling4(2, 3, 4);
    p = tuple_new_point(-4, 6, 8);
    p_res = matrix_mult_matrix4_tuple(m_scaling, p);
    p_exp = tuple_new_point(-8, 18, 32);
    assert(tuple_equals(p_res, p_exp));
    p_vec = tuple_new_vector(-4, 6, 8);
    p_vec_res = matrix_mult_matrix4_tuple(m_scaling, p_vec);
    struct tuple p_vec_exp = tuple_new_vector(-8, 18, 32);
    assert(tuple_equals(p_vec_exp, p_vec_res));

    struct matrix4 inv_m_scaling;
    matrix_inverse_matrix4(m_scaling, &inv_m_scaling);
    p_res = matrix_mult_matrix4_tuple(inv_m_scaling, p_vec);
    p_exp = tuple_new_point(-2, 2, 2);
    assert(tuple_equals(p_res, p_exp));

    m_scaling = matrix_new_scaling4(-1, 1, 1);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_scaling, p);
    p_exp = tuple_new_point(-2, 3, 4);
    assert(tuple_equals(p_res, p_exp));

    struct matrix4 m_rotate_x_pi4 = matrix_new_rotate_x(PI / 4);
    struct matrix4 m_rotate_x_pi2 = matrix_new_rotate_x(PI / 2);
    p = tuple_new_point(0, 1, 0);
    p_res = matrix_mult_matrix4_tuple(m_rotate_x_pi4, p);
    p_exp = tuple_new_point(0, sqrt(2)/2, sqrt(2)/2);
    assert(tuple_equals(p_res, p_exp));

    p = tuple_new_point(0, 1, 0);
    p_res = matrix_mult_matrix4_tuple(m_rotate_x_pi2, p);
    p_exp = tuple_new_point(0, 0, 1);
    assert(tuple_equals(p_res, p_exp));

    struct matrix4 m_inv_rotate_x_pi4;
    matrix_inverse_matrix4(m_rotate_x_pi4, &m_inv_rotate_x_pi4);
    p_res = matrix_mult_matrix4_tuple(m_inv_rotate_x_pi4, p);
    p_exp = tuple_new_point(0, sqrt(2)/2, -sqrt(2)/2);
    assert(tuple_equals(p_res, p_exp));

    struct matrix4 m_rotate_y_pi4 = matrix_new_rotate_y(PI / 4);
    struct matrix4 m_rotate_y_pi2 = matrix_new_rotate_y(PI / 2);
    p = tuple_new_point(0, 0, 1);
    p_res = matrix_mult_matrix4_tuple(m_rotate_y_pi4, p);
    p_exp = tuple_new_point(sqrt(2)/2, 0, sqrt(2)/2);
    assert(tuple_equals(p_res, p_exp));
    p_res = matrix_mult_matrix4_tuple(m_rotate_y_pi2, p);
    p_exp = tuple_new_point(1, 0, 0);
    assert(tuple_equals(p_res, p_exp));

    struct matrix4 m_rotate_z_pi4 = matrix_new_rotate_z(PI / 4);
    struct matrix4 m_rotate_z_pi2 = matrix_new_rotate_z(PI / 2);
    p = tuple_new_point(0, 1, 0);
    p_res = matrix_mult_matrix4_tuple(m_rotate_z_pi4, p);
    p_exp = tuple_new_point(-sqrt(2)/2, sqrt(2)/2, 0);
    assert(tuple_equals(p_res, p_exp));
    p_res = matrix_mult_matrix4_tuple(m_rotate_z_pi2, p);
    p_exp = tuple_new_point(-1, 0, 0);
    assert(tuple_equals(p_res, p_exp));

    struct matrix4 m_shearing = matrix_new_shearing(1, 0, 0, 0, 0, 0);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(5, 3, 4);
    assert(tuple_equals(p_res, p_exp));
    m_shearing = matrix_new_shearing(0, 1, 0, 0, 0, 0);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(6, 3, 4);
    assert(tuple_equals(p_res, p_exp));
    m_shearing = matrix_new_shearing(0, 0, 1, 0, 0, 0);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(2, 5, 4);
    assert(tuple_equals(p_res, p_exp));
    m_shearing = matrix_new_shearing(0, 0, 0, 1, 0, 0);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(2, 7, 4);
    assert(tuple_equals(p_res, p_exp));
    m_shearing = matrix_new_shearing(0, 0, 0, 0, 1, 0);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(2, 3, 6);
    assert(tuple_equals(p_res, p_exp));
    m_shearing = matrix_new_shearing(0, 0, 0, 0, 0, 1);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(2, 3, 7);
    assert(tuple_equals(p_res, p_exp));

    p = tuple_new_point(1, 0, 1);
    m_rotate_x_pi2 = matrix_new_rotate_x(PI / 2);
    m_scaling = matrix_new_scaling4(5, 5, 5);
    m_translate = matrix_new_translation4(10, 5, 7);
    struct tuple p2 = matrix_mult_matrix4_tuple(m_rotate_x_pi2, p);
    assert(tuple_equals(p2, (struct tuple) {1,-1,0,1}));
    struct tuple p3 = matrix_mult_matrix4_tuple(m_scaling, p2);
    assert(tuple_equals(p3, (struct tuple) {5,-5,0,1}));
    struct tuple p4 = matrix_mult_matrix4_tuple(m_translate, p3);
    assert(tuple_equals(p4, (struct tuple) {15,0,7,1}));

    p_res = matrix_mult_matrix4_tuple(m_translate, matrix_mult_matrix4_tuple(m_scaling,
	    matrix_mult_matrix4_tuple(m_rotate_x_pi2, p)));
    p_exp = tuple_new_point(15, 0, 7);
    assert(tuple_equals(p_res, p_exp));

    return EXIT_SUCCESS;
}
