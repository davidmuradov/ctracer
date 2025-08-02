#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/tuple.h"
#include "../../includes/ct_math.h"
#include "../../includes/canvas.h"
#include "../../includes/matrix.h"

int main(int argc, char *argv[]) {

    struct row4 r1 = matrix_new_row4(1, 2, 3, 4);
    struct row4 r2 = matrix_new_row4(5.5, 6.5, 7.5, 8.5);
    struct row4 r3 = matrix_new_row4(9, 10, 11, 12);
    struct row4 r4 = matrix_new_row4(13.5, 14.5, 15.5, 16.5);
    struct matrix4 m1 = matrix_new_matrix4(r1, r2, r3, r4);
    assert(ctm_floats_equal(m1.grid[0][0], 1));
    assert(ctm_floats_equal(m1.grid[0][1], 2));
    assert(ctm_floats_equal(m1.grid[0][2], 3));
    assert(ctm_floats_equal(m1.grid[0][3], 4));
    assert(ctm_floats_equal(m1.grid[1][0], 5.5));
    assert(ctm_floats_equal(m1.grid[1][1], 6.5));
    assert(ctm_floats_equal(m1.grid[1][2], 7.5));
    assert(ctm_floats_equal(m1.grid[1][3], 8.5));
    assert(ctm_floats_equal(m1.grid[2][0], 9));
    assert(ctm_floats_equal(m1.grid[2][1], 10));
    assert(ctm_floats_equal(m1.grid[2][2], 11));
    assert(ctm_floats_equal(m1.grid[2][3], 12));
    assert(ctm_floats_equal(m1.grid[3][0], 13.5));
    assert(ctm_floats_equal(m1.grid[3][1], 14.5));
    assert(ctm_floats_equal(m1.grid[3][2], 15.5));
    assert(ctm_floats_equal(m1.grid[3][3], 16.5));

    struct row2 r5 = matrix_new_row2(-3, 5);
    struct row2 r6 = matrix_new_row2(1, -2);
    struct matrix2 m2 = matrix_new_matrix2(r5, r6);
    assert(ctm_floats_equal(m2.grid[0][0], -3));
    assert(ctm_floats_equal(m2.grid[0][1], 5));
    assert(ctm_floats_equal(m2.grid[1][0], 1));
    assert(ctm_floats_equal(m2.grid[1][1], -2));

    struct row3 r7 = matrix_new_row3(-3, 5, 0);
    struct row3 r8 = matrix_new_row3(1, -2, -7);
    struct row3 r9 = matrix_new_row3(0, 1, 1);
    struct matrix3 m3 = matrix_new_matrix3(r7, r8, r9);
    assert(ctm_floats_equal(m3.grid[0][0], -3));
    assert(ctm_floats_equal(m3.grid[0][1], 5));
    assert(ctm_floats_equal(m3.grid[0][2], 0));
    assert(ctm_floats_equal(m3.grid[1][0], 1));
    assert(ctm_floats_equal(m3.grid[1][1], -2));
    assert(ctm_floats_equal(m3.grid[1][2], -7));
    assert(ctm_floats_equal(m3.grid[2][0], 0));
    assert(ctm_floats_equal(m3.grid[2][1], 1));
    assert(ctm_floats_equal(m3.grid[2][2], 1));

    r1 = matrix_new_row4(1, 2, 3, 4);
    r2 = matrix_new_row4(5, 6, 7, 8);
    r3 = matrix_new_row4(9, 8, 7, 6);
    r4 = matrix_new_row4(5, 4, 3, 2);
    m1 = matrix_new_matrix4(r1, r2, r3, r4);

    r1 = matrix_new_row4(1, 2, 3, 4);
    r2 = matrix_new_row4(5, 6, 7, 8);
    r3 = matrix_new_row4(9, 8, 7, 6);
    r4 = matrix_new_row4(5, 4, 3, 2);
    struct matrix4 m4 = matrix_new_matrix4(r1, r2, r3, r4);
    assert(matrix_compare_matrix4(m1, m4));

    r1 = matrix_new_row4(2, 3, 4, 5);
    r2 = matrix_new_row4(6, 7, 8, 9);
    r3 = matrix_new_row4(8, 7, 6, 5);
    r4 = matrix_new_row4(4, 3, 2, 1);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);
    assert(!matrix_compare_matrix4(m1, m4));

    // m1 * m4
    r1 = matrix_new_row4(-2, 1, 2, 3);
    r2 = matrix_new_row4(3, 2, 1, -1);
    r3 = matrix_new_row4(4, 3, 6, 5);
    r4 = matrix_new_row4(1, 2, 7, 8);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);

    struct row4 rr1 = matrix_new_row4(20, 22, 50, 48);
    struct row4 rr2 = matrix_new_row4(44, 54, 114, 108);
    struct row4 rr3 = matrix_new_row4(40, 58, 110, 102);
    struct row4 rr4 = matrix_new_row4(16, 26, 46, 42);
    struct matrix4 expected = matrix_new_matrix4(rr1, rr2, rr3, rr4);
    assert(matrix_compare_matrix4(matrix_mult_matrix4(m1, m4), expected));

    r1 = matrix_new_row4(1, 2, 3, 4);
    r2 = matrix_new_row4(2, 4, 4, 2);
    r3 = matrix_new_row4(8, 6, 4, 1);
    r4 = matrix_new_row4(0, 0, 0, 1);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);

    struct tuple p1 = tuple_new_point(1, 2, 3);
    struct tuple pres = (struct tuple) {18, 24, 33, 1};
    assert(tuple_equals(pres, matrix_mult_matrix4_tuple(m4, p1)));

    struct matrix4 IDENTITY4 = matrix_make_identity4();
    
    r1 = matrix_new_row4(0, 1, 2, 4);
    r2 = matrix_new_row4(1, 2, 4, 8);
    r3 = matrix_new_row4(2, 4, 8, 16);
    r4 = matrix_new_row4(4, 8, 16, 32);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);

    assert(matrix_compare_matrix4(m4, matrix_mult_matrix4(m4, IDENTITY4)));

    r1 = matrix_new_row4(0, 9, 3, 0);
    r2 = matrix_new_row4(9, 8, 0, 8);
    r3 = matrix_new_row4(1, 8, 5, 3);
    r4 = matrix_new_row4(0, 0, 5, 8);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);

    rr1 = matrix_new_row4(0, 9, 1, 0);
    rr2 = matrix_new_row4(9, 8, 8, 0);
    rr3 = matrix_new_row4(3, 0, 5, 5);
    rr4 = matrix_new_row4(0, 8, 3, 8);
    expected = matrix_new_matrix4(rr1, rr2, rr3, rr4);

    assert(matrix_compare_matrix4(matrix_transpose4(m4), expected));
    assert(matrix_compare_matrix4(matrix_transpose4(IDENTITY4), IDENTITY4));

    r5 = matrix_new_row2(1, 5);
    r6 = matrix_new_row2(-3, 2);
    m2 = matrix_new_matrix2(r5, r6);
    assert(ctm_floats_equal(17, matrix_det_matrix2(m2)));

    r7 = matrix_new_row3(1, 5, 0);
    r8 = matrix_new_row3(-3, 2, 7);
    r9 = matrix_new_row3(0, 6, -3);
    m3 = matrix_new_matrix3(r7, r8, r9);

    r5 = matrix_new_row2(-3, 2);
    r6 = matrix_new_row2(0, 6);
    m2 = matrix_new_matrix2(r5, r6);
    assert(matrix_compare_matrix2(m2, matrix_sub_matrix3(m3, 0, 2)));

    r1 = matrix_new_row4(-6,1,1,6);
    r2 = matrix_new_row4(-8,5,8,6);
    r3 = matrix_new_row4(-1,0,8,2);
    r4 = matrix_new_row4(-7,1,-1,1);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);

    r7 = matrix_new_row3(-6,1,6);
    r8 = matrix_new_row3(-8,8,6);
    r9 = matrix_new_row3(-7,-1,1);
    m3 = matrix_new_matrix3(r7, r8, r9);
    assert(matrix_compare_matrix3(m3, matrix_sub_matrix4(m4, 2, 1)));

    r7 = matrix_new_row3(3,5,0);
    r8 = matrix_new_row3(2,-1,-7);
    r9 = matrix_new_row3(6,-1,5);
    m3 = matrix_new_matrix3(r7, r8, r9);
    assert(ctm_floats_equal(25, matrix_minor_matrix3(m3, 1, 0)));

    assert(ctm_floats_equal(-12, matrix_minor_matrix3(m3, 0, 0)));
    assert(ctm_floats_equal(-12, matrix_cofactor_matrix3(m3, 0, 0)));
    assert(ctm_floats_equal(25, matrix_minor_matrix3(m3, 1, 0)));
    assert(ctm_floats_equal(-25, matrix_cofactor_matrix3(m3, 1, 0)));

    r7 = matrix_new_row3(1,2,6);
    r8 = matrix_new_row3(-5,8,-4);
    r9 = matrix_new_row3(2,6,4);
    m3 = matrix_new_matrix3(r7, r8, r9);
    assert(ctm_floats_equal(56, matrix_cofactor_matrix3(m3, 0, 0)));
    assert(ctm_floats_equal(12, matrix_cofactor_matrix3(m3, 0, 1)));
    assert(ctm_floats_equal(-46, matrix_cofactor_matrix3(m3, 0, 2)));
    assert(ctm_floats_equal(-196, matrix_det_matrix3(m3)));

    r1 = matrix_new_row4(-2,-8,3,5);
    r2 = matrix_new_row4(-3,1,7,3);
    r3 = matrix_new_row4(1,2,-9,6);
    r4 = matrix_new_row4(-6,7,7,-9);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);
    assert(ctm_floats_equal(690, matrix_cofactor_matrix4(m4, 0, 0)));
    assert(ctm_floats_equal(447, matrix_cofactor_matrix4(m4, 0, 1)));
    assert(ctm_floats_equal(210, matrix_cofactor_matrix4(m4, 0, 2)));
    assert(ctm_floats_equal(51, matrix_cofactor_matrix4(m4, 0, 3)));
    assert(ctm_floats_equal(-4071, matrix_det_matrix4(m4)));

    r1 = matrix_new_row4(6,4,4,4);
    r2 = matrix_new_row4(5,5,7,6);
    r3 = matrix_new_row4(4,-9,3,-7);
    r4 = matrix_new_row4(9,1,7,-6);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);
    assert(ctm_floats_equal(-2120, matrix_det_matrix4(m4)));

    struct matrix4 m4_res;

    assert(matrix_inverse_matrix4(m4, &m4_res));

    r1 = matrix_new_row4(-4,2,-2,-3);
    r2 = matrix_new_row4(9,6,2,6);
    r3 = matrix_new_row4(0,-5,1,-5);
    r4 = matrix_new_row4(0,0,0,0);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);
    assert(ctm_floats_equal(0, matrix_det_matrix4(m4)));
    assert(!matrix_inverse_matrix4(m4, &m4_res));

    r1 = matrix_new_row4(-5,2,6,-8);
    r2 = matrix_new_row4(1,-5,1,8);
    r3 = matrix_new_row4(7,7,-6,-7);
    r4 = matrix_new_row4(1,-3,7,4);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);
    assert(ctm_floats_equal(532, matrix_det_matrix4(m4)));
    assert(ctm_floats_equal(-160, matrix_cofactor_matrix4(m4, 2, 3)));
    assert(ctm_floats_equal(105, matrix_cofactor_matrix4(m4, 3, 2)));

    rr1 = matrix_new_row4(0.21805, 0.45113, 0.24060, -0.04511);
    rr2 = matrix_new_row4(-0.80827, -1.45677, -0.44361, 0.52068);
    rr3 = matrix_new_row4(-0.07895, -0.22368, -0.05263, 0.19737);
    rr4 = matrix_new_row4(-0.52256, -0.81391, -0.30075, 0.30639);
    expected = matrix_new_matrix4(rr1, rr2, rr3, rr4);
    matrix_inverse_matrix4(m4, &m4_res);
    assert(ctm_floats_equal(-160./532, m4_res.grid[3][2]));
    assert(ctm_floats_equal(105./532, m4_res.grid[2][3]));
    assert(matrix_compare_matrix4(expected, m4_res));

    r1 = matrix_new_row4(8,-5,9,2);
    r2 = matrix_new_row4(7,5,6,1);
    r3 = matrix_new_row4(-6,0,9,6);
    r4 = matrix_new_row4(-3,0,-9,-4);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);

    rr1 = matrix_new_row4(-0.15385, -0.15385, -0.28205, -0.53846);
    rr2 = matrix_new_row4(-0.07692, 0.12308, 0.02564, 0.03077);
    rr3 = matrix_new_row4(0.35897, 0.35897, 0.43590, 0.92308);
    rr4 = matrix_new_row4(-0.69231, -0.69231, -0.76923, -1.92308);
    expected = matrix_new_matrix4(rr1, rr2, rr3, rr4);
    matrix_inverse_matrix4(m4, &m4_res);
    assert(matrix_compare_matrix4(expected, m4_res));

    r1 = matrix_new_row4(9,3,0,9);
    r2 = matrix_new_row4(-5,-2,-6,-3);
    r3 = matrix_new_row4(-4,9,6,4);
    r4 = matrix_new_row4(-7,6,6,2);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);

    rr1 = matrix_new_row4(-0.04074, -0.07778, 0.14444, -0.22222);
    rr2 = matrix_new_row4(-0.07778, 0.03333, 0.36667, -0.33333);
    rr3 = matrix_new_row4(-0.02901, -0.14630, -0.10926, 0.12963);
    rr4 = matrix_new_row4(0.17778, 0.06667, -0.26667, 0.33333);
    expected = matrix_new_matrix4(rr1, rr2, rr3, rr4);

    matrix_inverse_matrix4(m4, &m4_res);
    assert(matrix_compare_matrix4(expected, m4_res));

    r1 = matrix_new_row4(3,-9,7,3);
    r2 = matrix_new_row4(3,-8,2,-9);
    r3 = matrix_new_row4(-4,4,4,1);
    r4 = matrix_new_row4(-6,5,-1,1);
    m4 = matrix_new_matrix4(r1, r2, r3, r4);

    r1 = matrix_new_row4(8,2,2,2);
    r2 = matrix_new_row4(3,-1,7,0);
    r3 = matrix_new_row4(7,0,5,4);
    r4 = matrix_new_row4(6,-2,0,5);
    struct matrix4 mm4 = matrix_new_matrix4(r1, r2, r3, r4);
    struct matrix4 mm4_res = matrix_mult_matrix4(m4, mm4);
    matrix_inverse_matrix4(mm4, &m4_res);
    assert(matrix_compare_matrix4(m4, matrix_mult_matrix4(mm4_res, m4_res)));

    return EXIT_SUCCESS;
}
