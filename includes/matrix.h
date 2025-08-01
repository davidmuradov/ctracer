#ifndef INCLUDES_MATRIX_H
#define INCLUDES_MATRIX_H

#include "tuple.h"

typedef double matrix2[2][2];
typedef double matrix3[3][3];
typedef double matrix4[4][4];

void matrix_init_matrix4(matrix4 m, double* r1, double* r2, double* r3, double* r4);

void matrix_init_matrix3(matrix3 m, double* r1, double* r2, double* r3);

void matrix_init_matrix2(matrix2 m, double* r1, double* r2);

int matrix_compare_matrix4(matrix4 a, matrix4 b);

int matrix_compare_matrix3(matrix3 a, matrix3 b);

int matrix_compare_matrix2(matrix2 a, matrix2 b);

void matrix_mult_matrix4(matrix4 a, matrix4 b, matrix4 c);

struct tuple matrix_mult_matrix4_tuple(matrix4 a, struct tuple b);

void matrix_make_identity4(matrix4 m);

void matrix_transpose4(matrix4 m, matrix4 d);

double matrix_det_matrix2(matrix2 m);

void matrix_sub_matrix3(matrix3 m, int i, int j, matrix2 d);

void matrix_sub_matrix4(matrix4 m, int i, int j, matrix3 d);

double matrix_minor_matrix3(matrix3 m, int i, int j);

double matrix_cofactor_matrix3(matrix3 m, int i, int j);

double matrix_det_matrix3(matrix3 m);

double matrix_minor_matrix4(matrix4 m, int i, int j);

double matrix_cofactor_matrix4(matrix4 m, int i, int j);

double matrix_det_matrix4(matrix4 m);

int matrix_inverse_matrix4(matrix4 m, matrix4 d);

#endif
