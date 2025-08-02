#ifndef INCLUDES_MATRIX_H
#define INCLUDES_MATRIX_H

#include "tuple.h"

struct matrix4 {
    double grid[4][4];
};

struct matrix3 {
    double grid[3][3];
};

struct matrix2 {
    double grid[2][2];
};

struct row4 {
    double a;
    double b;
    double c;
    double d;
};

struct row3 {
    double a;
    double b;
    double c;
};

struct row2 {
    double a;
    double b;
};

struct row4 matrix_new_row4(double a, double b, double c, double d);

struct row3 matrix_new_row3(double a, double b, double c);

struct row2 matrix_new_row2(double a, double b);

struct matrix4 matrix_new_matrix4(struct row4 r1, struct row4 r2, struct row4 r3, struct row4 r4);

struct matrix3 matrix_new_matrix3(struct row3 r1, struct row3 r2, struct row3 r3);

struct matrix2 matrix_new_matrix2(struct row2 r1, struct row2 r2);

//void matrix_set_from_matrix4(struct matrix4 src, struct matrix4* dst);

int matrix_compare_matrix4(struct matrix4 a, struct matrix4 b);

int matrix_compare_matrix3(struct matrix3 a, struct matrix3 b);

int matrix_compare_matrix2(struct matrix2 a, struct matrix2 b);

struct matrix4 matrix_mult_matrix4(struct matrix4 a, struct matrix4 b);

struct tuple matrix_mult_matrix4_tuple(struct matrix4 a, struct tuple b);

struct matrix4 matrix_make_identity4(void);

struct matrix4 matrix_transpose4(struct matrix4 a);

double matrix_det_matrix2(struct matrix2 a);

struct matrix2 matrix_sub_matrix3(struct matrix3 a, int i, int j);

struct matrix3 matrix_sub_matrix4(struct matrix4 a, int i, int j);

double matrix_minor_matrix3(struct matrix3 a, int i, int j);

double matrix_cofactor_matrix3(struct matrix3 a, int i, int j);

double matrix_det_matrix3(struct matrix3 a);

double matrix_minor_matrix4(struct matrix4 a, int i, int j);

double matrix_cofactor_matrix4(struct matrix4 a, int i, int j);

double matrix_det_matrix4(struct matrix4 a);

int matrix_inverse_matrix4(struct matrix4 src, struct matrix4* dst);

struct matrix4 matrix_new_translation4(double x, double y, double z);

struct matrix4 matrix_new_scaling4(double x, double y, double z);

struct matrix4 matrix_new_rotate_x(double a);

struct matrix4 matrix_new_rotate_y(double a);

struct matrix4 matrix_new_rotate_z(double a);

struct matrix4 matrix_new_shearing(double xy, double xz, double yx, double yz, double zx, double zy);

#endif
