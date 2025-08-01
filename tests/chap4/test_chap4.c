#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/tuple.h"
#include "../../includes/ct_math.h"
#include "../../includes/canvas.h"
#include "../../includes/matrix.h"

int main(int argc, char *argv[]) {

    matrix4 m_translate;
    double r1[4] = {1,0,0,5};
    double r2[4] = {0,1,0,-3};
    double r3[4] = {0,0,1,2};
    double r4[4] = {0,0,0,1};
    matrix_new_translation4(5, -3, 2, m_translate);
    matrix4 m_test;
    matrix_init_matrix4(m_test, r1, r2, r3, r4);
    assert(matrix_compare_matrix4(m_translate, m_test));
    struct tuple p = tuple_new_point(-3, 4, 5);
    struct tuple p_res = matrix_mult_matrix4_tuple(m_translate, p);
    struct tuple p_exp = tuple_new_point(2, 1, 7);
    assert(tuple_equals(p_res, p_exp));

    matrix4 inv_m_translate;
    matrix_inverse_matrix4(m_translate, inv_m_translate);
    p_res = matrix_mult_matrix4_tuple(inv_m_translate, p);
    p_exp = tuple_new_point(-8, 7, 3);
    assert(tuple_equals(p_res, p_exp));
    struct tuple p_vec = tuple_new_vector(-3, 4, 5);
    struct tuple p_vec_res = matrix_mult_matrix4_tuple(m_translate, p_vec);
    assert(tuple_equals(p_vec_res, p_vec));

    matrix4 m_scaling;
    double a1[4] = {2,0,0,0};
    double a2[4] = {0,3,0,0};
    double a3[4] = {0,0,4,0};
    double a4[4] = {0,0,0,1};
    matrix_new_scaling4(2, 3, 4, m_scaling);
    matrix4 m_test_scale;
    matrix_init_matrix4(m_test_scale, a1, a2, a3, a4);
    assert(matrix_compare_matrix4(m_scaling, m_test_scale));
    p = tuple_new_point(-4, 6, 8);
    p_res = matrix_mult_matrix4_tuple(m_scaling, p);
    p_exp = tuple_new_point(-8, 18, 32);
    assert(tuple_equals(p_res, p_exp));
    p_vec = tuple_new_vector(-4, 6, 8);
    p_vec_res = matrix_mult_matrix4_tuple(m_scaling, p_vec);
    struct tuple p_vec_exp = tuple_new_vector(-8, 18, 32);
    assert(tuple_equals(p_vec_exp, p_vec_res));

    matrix4 inv_m_scaling;
    matrix_inverse_matrix4(m_scaling, inv_m_scaling);
    p_res = matrix_mult_matrix4_tuple(inv_m_scaling, p_vec);
    p_exp = tuple_new_point(-2, 2, 2);
    assert(tuple_equals(p_res, p_exp));

    matrix4 m_reflect_x;
    matrix_new_scaling4(-1, 1, 1, m_reflect_x);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_reflect_x, p);
    p_exp = tuple_new_point(-2, 3, 4);
    assert(tuple_equals(p_res, p_exp));

    matrix4 m_rotate_x_pi4;
    matrix4 m_rotate_x_pi2;
    matrix_new_rotate_x(PI / 4, m_rotate_x_pi4);
    matrix_new_rotate_x(PI / 2, m_rotate_x_pi2);
    p = tuple_new_point(0, 1, 0);
    p_res = matrix_mult_matrix4_tuple(m_rotate_x_pi4, p);
    p_exp = tuple_new_point(0, sqrt(2)/2, sqrt(2)/2);
    assert(tuple_equals(p_res, p_exp));

    p = tuple_new_point(0, 1, 0);
    p_res = matrix_mult_matrix4_tuple(m_rotate_x_pi2, p);
    p_exp = tuple_new_point(0, 0, 1);
    assert(tuple_equals(p_res, p_exp));

    matrix4 m_inv_rotate_x_pi4;
    matrix_inverse_matrix4(m_rotate_x_pi4, m_inv_rotate_x_pi4);
    p_res = matrix_mult_matrix4_tuple(m_inv_rotate_x_pi4, p);
    p_exp = tuple_new_point(0, sqrt(2)/2, -sqrt(2)/2);
    assert(tuple_equals(p_res, p_exp));

    matrix4 m_rotate_y_pi4;
    matrix4 m_rotate_y_pi2;
    matrix_new_rotate_y(PI / 4, m_rotate_y_pi4);
    matrix_new_rotate_y(PI / 2, m_rotate_y_pi2);
    p = tuple_new_point(0, 0, 1);
    p_res = matrix_mult_matrix4_tuple(m_rotate_y_pi4, p);
    p_exp = tuple_new_point(sqrt(2)/2, 0, sqrt(2)/2);
    assert(tuple_equals(p_res, p_exp));
    p_res = matrix_mult_matrix4_tuple(m_rotate_y_pi2, p);
    p_exp = tuple_new_point(1, 0, 0);
    assert(tuple_equals(p_res, p_exp));

    matrix4 m_rotate_z_pi4;
    matrix4 m_rotate_z_pi2;
    matrix_new_rotate_z(PI / 4, m_rotate_z_pi4);
    matrix_new_rotate_z(PI / 2, m_rotate_z_pi2);
    p = tuple_new_point(0, 1, 0);
    p_res = matrix_mult_matrix4_tuple(m_rotate_z_pi4, p);
    p_exp = tuple_new_point(-sqrt(2)/2, sqrt(2)/2, 0);
    assert(tuple_equals(p_res, p_exp));
    p_res = matrix_mult_matrix4_tuple(m_rotate_z_pi2, p);
    p_exp = tuple_new_point(-1, 0, 0);
    assert(tuple_equals(p_res, p_exp));

    matrix4 m_shearing;
    matrix_new_shearing(1, 0, 0, 0, 0, 0, m_shearing);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(5, 3, 4);
    assert(tuple_equals(p_res, p_exp));
    matrix_new_shearing(0, 1, 0, 0, 0, 0, m_shearing);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(6, 3, 4);
    assert(tuple_equals(p_res, p_exp));
    matrix_new_shearing(0, 0, 1, 0, 0, 0, m_shearing);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(2, 5, 4);
    assert(tuple_equals(p_res, p_exp));
    matrix_new_shearing(0, 0, 0, 1, 0, 0, m_shearing);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(2, 7, 4);
    assert(tuple_equals(p_res, p_exp));
    matrix_new_shearing(0, 0, 0, 0, 1, 0, m_shearing);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(2, 3, 6);
    assert(tuple_equals(p_res, p_exp));
    matrix_new_shearing(0, 0, 0, 0, 0, 1, m_shearing);
    p = tuple_new_point(2, 3, 4);
    p_res = matrix_mult_matrix4_tuple(m_shearing, p);
    p_exp = tuple_new_point(2, 3, 7);
    assert(tuple_equals(p_res, p_exp));

    p = tuple_new_point(1, 0, 1);
    matrix_new_rotate_x(PI / 2, m_rotate_x_pi2);
    matrix_new_scaling4(5, 5, 5, m_scaling);
    matrix_new_translation4(10, 5, 7, m_translate);
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
