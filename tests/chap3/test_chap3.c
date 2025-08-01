#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/tuple.h"
#include "../../includes/ct_math.h"
#include "../../includes/canvas.h"
#include "../../includes/matrix.h"

int main(int argc, char *argv[]) {

    matrix4 m1;
    double r1[4] = {1,2,3,4};
    double r2[4] = {5.5,6.5,7.5,8.5};
    double r3[4] = {9,10,11,12};
    double r4[4] = {13.5,14.5,15.5,16.5};
    matrix_init_matrix4(m1, r1, r2, r3, r4);
    assert(ctm_floats_equal(m1[0][0], 1));
    assert(ctm_floats_equal(m1[0][1], 2));
    assert(ctm_floats_equal(m1[0][2], 3));
    assert(ctm_floats_equal(m1[0][3], 4));
    assert(ctm_floats_equal(m1[1][0], 5.5));
    assert(ctm_floats_equal(m1[1][1], 6.5));
    assert(ctm_floats_equal(m1[1][2], 7.5));
    assert(ctm_floats_equal(m1[1][3], 8.5));
    assert(ctm_floats_equal(m1[2][0], 9));
    assert(ctm_floats_equal(m1[2][1], 10));
    assert(ctm_floats_equal(m1[2][2], 11));
    assert(ctm_floats_equal(m1[2][3], 12));
    assert(ctm_floats_equal(m1[3][0], 13.5));
    assert(ctm_floats_equal(m1[3][1], 14.5));
    assert(ctm_floats_equal(m1[3][2], 15.5));
    assert(ctm_floats_equal(m1[3][3], 16.5));

    matrix2 m2;
    double r5[2] = {-3,5};
    double r6[2] = {1,-2};
    matrix_init_matrix2(m2, r5, r6);
    assert(ctm_floats_equal(m2[0][0], -3));
    assert(ctm_floats_equal(m2[0][1], 5));
    assert(ctm_floats_equal(m2[1][0], 1));
    assert(ctm_floats_equal(m2[1][1], -2));

    matrix3 m3;
    double r7[3] = {-3,5,0};
    double r8[3] = {1,-2,-7};
    double r9[3] = {0,1,1};
    matrix_init_matrix3(m3, r7, r8, r9);
    assert(ctm_floats_equal(m3[0][0], -3));
    assert(ctm_floats_equal(m3[0][1], 5));
    assert(ctm_floats_equal(m3[0][2], 0));
    assert(ctm_floats_equal(m3[1][0], 1));
    assert(ctm_floats_equal(m3[1][1], -2));
    assert(ctm_floats_equal(m3[1][2], -7));
    assert(ctm_floats_equal(m3[2][0], 0));
    assert(ctm_floats_equal(m3[2][1], 1));
    assert(ctm_floats_equal(m3[2][2], 1));

    matrix4 m4;
    double t1[4] = {1,2,3,4};
    double t2[4] = {5,6,7,8};
    double t3[4] = {9,8,7,6};
    double t4[4] = {5,4,3,2};
    matrix_init_matrix4(m4, t1, t2, t3, t4);

    matrix4 m5;
    double v1[4] = {1,2,3,4};
    double v2[4] = {5,6,7,8};
    double v3[4] = {9,8,7,6};
    double v4[4] = {5,4,3,2};
    matrix_init_matrix4(m5, v1, v2, v3, v4);
    assert(matrix_compare_matrix4(m4, m5));

    matrix4 m6;
    double c1[4] = {2,3,4,5};
    double c2[4] = {6,7,7,9};
    double c3[4] = {8,7,6,5};
    double c4[4] = {4,3,2,1};
    matrix_init_matrix4(m6, c1, c2, c3, c4);
    assert(!matrix_compare_matrix4(m4, m6));

    // m4 * m7
    matrix4 m7;
    double a1[4] = {-2,1,2,3};
    double a2[4] = {3,2,1,-1};
    double a3[4] = {4,3,6,5};
    double a4[4] = {1,2,7,8};
    matrix_init_matrix4(m7, a1, a2, a3, a4);

    matrix4 res;
    matrix_mult_matrix4(m4, m7, res);
    matrix4 expected;
    double b1[4] = {20,22,50,48};
    double b2[4] = {44,54,114,108};
    double b3[4] = {40,58,110,102};
    double b4[4] = {16,26,46,42};
    matrix_init_matrix4(expected, b1, b2, b3, b4);
    assert(matrix_compare_matrix4(res, expected));

    matrix4 m8;
    double aa1[4] = {1,2,3,4};
    double aa2[4] = {2,4,4,2};
    double aa3[4] = {8,6,4,1};
    double aa4[4] = {0,0,0,1};
    matrix_init_matrix4(m8, aa1, aa2, aa3, aa4);
    struct tuple p1 = tuple_new_point(1, 2, 3);
    struct tuple pres = (struct tuple) {18, 24, 33, 1};
    assert(tuple_equals(pres, matrix_mult_matrix4_tuple(m8, p1)));

    matrix4 IDENTITY4;
    matrix_make_identity4(IDENTITY4);

    matrix4 mm1;
    double ab1[4] = {0,1,2,4};
    double ab2[4] = {1,2,4,8};
    double ab3[4] = {2,4,8,16};
    double ab4[4] = {4,8,16,32};
    matrix_init_matrix4(mm1, ab1, ab2, ab3, ab4);
    matrix4 mm1res;
    matrix_mult_matrix4(mm1, IDENTITY4, mm1res);
    assert(matrix_compare_matrix4(mm1, mm1res));

    matrix4 mm2;
    double ac1[4] = {0,9,3,0};
    double ac2[4] = {9,8,0,8};
    double ac3[4] = {1,8,5,3};
    double ac4[4] = {0,0,5,8};
    matrix_init_matrix4(mm2, ac1, ac2, ac3, ac4);
    matrix4 mm2res;
    matrix_transpose4(mm2, mm2res);
    matrix4 mm3;
    double ad1[4] = {0,9,1,0};
    double ad2[4] = {9,8,8,0};
    double ad3[4] = {3,0,5,5};
    double ad4[4] = {0,8,3,8};
    matrix_init_matrix4(mm3, ad1, ad2, ad3, ad4);
    assert(matrix_compare_matrix4(mm2res, mm3));
    matrix_transpose4(IDENTITY4, mm3);
    assert(matrix_compare_matrix4(IDENTITY4, mm3));

    matrix2 mm4;
    double ae1[2] = {1,5};
    double ae2[2] = {-3,2};
    matrix_init_matrix2(mm4, ae1, ae2);
    assert(ctm_floats_equal(17, matrix_det_matrix2(mm4)));

    matrix3 mm5;
    double af1[3] = {1,5,0};
    double af2[3] = {-3,2,7};
    double af3[3] = {0,6,-3};
    matrix_init_matrix3(mm5, af1, af2, af3);
    matrix2 mm6;
    double ag1[2] = {-3,2};
    double ag2[2] = {0,6};
    matrix_init_matrix2(mm6, ag1, ag2);
    matrix2 mm7;
    matrix_sub_matrix3(mm5, 0, 2, mm7);
    assert(matrix_compare_matrix2(mm7, mm6));

    matrix4 mm8;
    double ah1[4] = {-6,1,1,6};
    double ah2[4] = {-8,5,8,6};
    double ah3[4] = {-1,0,8,2};
    double ah4[4] = {-7,1,-1,1};
    matrix_init_matrix4(mm8, ah1, ah2, ah3, ah4);

    matrix3 mm9;
    double ai1[3] = {-6,1,6};
    double ai2[3] = {-8,8,6};
    double ai3[3] = {-7,-1,1};
    matrix_init_matrix3(mm9, ai1, ai2, ai3);

    matrix3 mm9_res;
    matrix_sub_matrix4(mm8, 2, 1, mm9_res);
    assert(matrix_compare_matrix3(mm9, mm9_res));

    matrix3 mmm1;
    double aj1[3] = {3,5,0};
    double aj2[3] = {2,-1,-7};
    double aj3[3] = {6,-1,5};
    matrix_init_matrix3(mmm1, aj1, aj2, aj3);
    assert(ctm_floats_equal(25, matrix_minor_matrix3(mmm1, 1, 0)));

    assert(ctm_floats_equal(-12, matrix_minor_matrix3(mmm1, 0, 0)));
    assert(ctm_floats_equal(-12, matrix_cofactor_matrix3(mmm1, 0, 0)));
    assert(ctm_floats_equal(25, matrix_minor_matrix3(mmm1, 1, 0)));
    assert(ctm_floats_equal(-25, matrix_cofactor_matrix3(mmm1, 1, 0)));

    matrix3 mmm2;
    double ak1[3] = {1,2,6};
    double ak2[3] = {-5,8,-4};
    double ak3[3] = {2,6,4};
    matrix_init_matrix3(mmm2, ak1, ak2, ak3);
    assert(ctm_floats_equal(56, matrix_cofactor_matrix3(mmm2, 0, 0)));
    assert(ctm_floats_equal(12, matrix_cofactor_matrix3(mmm2, 0, 1)));
    assert(ctm_floats_equal(-46, matrix_cofactor_matrix3(mmm2, 0, 2)));
    assert(ctm_floats_equal(-196, matrix_det_matrix3(mmm2)));

    matrix4 mmm3;
    double al1[4] = {-2,-8,3,5};
    double al2[4] = {-3,1,7,3};
    double al3[4] = {1,2,-9,6};
    double al4[4] = {-6,7,7,-9};
    matrix_init_matrix4(mmm3, al1, al2, al3, al4);
    assert(ctm_floats_equal(690, matrix_cofactor_matrix4(mmm3, 0, 0)));
    assert(ctm_floats_equal(447, matrix_cofactor_matrix4(mmm3, 0, 1)));
    assert(ctm_floats_equal(210, matrix_cofactor_matrix4(mmm3, 0, 2)));
    assert(ctm_floats_equal(51, matrix_cofactor_matrix4(mmm3, 0, 3)));
    assert(ctm_floats_equal(-4071, matrix_det_matrix4(mmm3)));

    matrix4 mmm4;
    double am1[4] = {6,4,4,4};
    double am2[4] = {5,5,7,6};
    double am3[4] = {4,-9,3,-7};
    double am4[4] = {9,1,7,-6};
    matrix_init_matrix4(mmm4, am1, am2, am3, am4);
    assert(ctm_floats_equal(-2120, matrix_det_matrix4(mmm4)));
    assert(matrix_inverse_matrix4(mmm4, mmm3));
    matrix4 mmm5;
    double ba1[4] = {-4,2,-2,-3};
    double ba2[4] = {9,6,2,6};
    double ba3[4] = {0,-5,1,-5};
    double ba4[4] = {0,0,0,0};
    matrix_init_matrix4(mmm5, ba1, ba2, ba3, ba4);
    assert(ctm_floats_equal(0, matrix_det_matrix4(mmm5)));
    assert(!matrix_inverse_matrix4(mmm5, mmm3));

    matrix4 mat_a;
    double bb1[4] = {-5,2,6,-8};
    double bb2[4] = {1,-5,1,8};
    double bb3[4] = {7,7,-6,-7};
    double bb4[4] = {1,-3,7,4};
    matrix_init_matrix4(mat_a, bb1, bb2, bb3, bb4);
    assert(ctm_floats_equal(532, matrix_det_matrix4(mat_a)));
    assert(ctm_floats_equal(-160, matrix_cofactor_matrix4(mat_a, 2, 3)));
    assert(ctm_floats_equal(105, matrix_cofactor_matrix4(mat_a, 3, 2)));

    matrix4 mat_b;
    double bc1[4] = {0.21805, 0.45113, 0.24060, -0.04511};
    double bc2[4] = {-0.80827, -1.45677, -0.44361, 0.52068};
    double bc3[4] = {-0.07895, -0.22368, -0.05263, 0.19737};
    double bc4[4] = {-0.52256, -0.81391, -0.30075, 0.30639};
    matrix_init_matrix4(mat_b, bc1, bc2, bc3, bc4);
    matrix4 mat_b_res;
    matrix_inverse_matrix4(mat_a, mat_b_res);
    assert(ctm_floats_equal(-160./532, mat_b_res[3][2]));
    assert(ctm_floats_equal(105./532, mat_b_res[2][3]));
    assert(matrix_compare_matrix4(mat_b, mat_b_res));

    double bd1[4] = {8,-5,9,2};
    double bd2[4] = {7,5,6,1};
    double bd3[4] = {-6,0,9,6};
    double bd4[4] = {-3,0,-9,-4};
    matrix_init_matrix4(mat_a, bd1, bd2, bd3, bd4);
    double be1[4] = {-0.15385, -0.15385, -0.28205, -0.53846};
    double be2[4] = {-0.07692, 0.12308, 0.02564, 0.03077};
    double be3[4] = {0.35897, 0.35897, 0.43590, 0.92308};
    double be4[4] = {-0.69231, -0.69231, -0.76923, -1.92308};
    matrix_init_matrix4(mat_b, be1, be2, be3, be4);
    matrix_inverse_matrix4(mat_a, mat_b_res);
    assert(matrix_compare_matrix4(mat_b, mat_b_res));

    double bf1[4] = {9,3,0,9};
    double bf2[4] = {-5,-2,-6,-3};
    double bf3[4] = {-4,9,6,4};
    double bf4[4] = {-7,6,6,2};
    matrix_init_matrix4(mat_a, bf1, bf2, bf3, bf4);
    double bg1[4] = {-0.04074, -0.07778, 0.14444, -0.22222};
    double bg2[4] = {-0.07778, 0.03333, 0.36667, -0.33333};
    double bg3[4] = {-0.02901, -0.14630, -0.10926, 0.12963};
    double bg4[4] = {0.17778, 0.06667, -0.26667, 0.33333};
    matrix_init_matrix4(mat_b, bg1, bg2, bg3, bg4);
    matrix_inverse_matrix4(mat_a, mat_b_res);
    assert(matrix_compare_matrix4(mat_b, mat_b_res));

    double bh1[4] = {3,-9,7,3};
    double bh2[4] = {3,-8,2,-9};
    double bh3[4] = {-4,4,4,1};
    double bh4[4] = {-6,5,-1,1};
    matrix_init_matrix4(mat_a, bh1, bh2, bh3, bh4);
    double bi1[4] = {8,2,2,2};
    double bi2[4] = {3,-1,7,0};
    double bi3[4] = {7,0,5,4};
    double bi4[4] = {6,-2,0,5};
    matrix_init_matrix4(mat_b, bi1, bi2, bi3, bi4);
    matrix4 mat_c;
    matrix_mult_matrix4(mat_a, mat_b, mat_c);
    matrix4 mat_inv_b;
    matrix_inverse_matrix4(mat_b, mat_inv_b);
    matrix4 cba;
    matrix_mult_matrix4(mat_c, mat_inv_b, cba);
    assert(matrix_compare_matrix4(mat_a, cba));

    return EXIT_SUCCESS;
}
