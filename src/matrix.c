#include "../includes/matrix.h"
#include "ct_math.h"
#include <math.h>

void matrix_init_matrix4(matrix4 m, double* r1, double* r2, double* r3, double* r4) {

    m[0][0] = r1[0];
    m[0][1] = r1[1];
    m[0][2] = r1[2];
    m[0][3] = r1[3];

    m[1][0] = r2[0];
    m[1][1] = r2[1];
    m[1][2] = r2[2];
    m[1][3] = r2[3];

    m[2][0] = r3[0];
    m[2][1] = r3[1];
    m[2][2] = r3[2];
    m[2][3] = r3[3];

    m[3][0] = r4[0];
    m[3][1] = r4[1];
    m[3][2] = r4[2];
    m[3][3] = r4[3];
}

void matrix_init_matrix3(matrix3 m, double* r1, double* r2, double* r3) {

    m[0][0] = r1[0];
    m[0][1] = r1[1];
    m[0][2] = r1[2];

    m[1][0] = r2[0];
    m[1][1] = r2[1];
    m[1][2] = r2[2];

    m[2][0] = r3[0];
    m[2][1] = r3[1];
    m[2][2] = r3[2];
}

void matrix_init_matrix2(matrix2 m, double* r1, double* r2) {

    m[0][0] = r1[0];
    m[0][1] = r1[1];

    m[1][0] = r2[0];
    m[1][1] = r2[1];
}

void matrix_set_from_matrix4(matrix4 m, matrix4 d) {
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    m[i][j] = d[i][j];
}

int matrix_compare_matrix4(matrix4 a, matrix4 b) {
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    if (!ctm_floats_equal(a[i][j], b[i][j]))
		return 0;
    return 1;
}

int matrix_compare_matrix3(matrix3 a, matrix3 b) {
    for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	    if (!ctm_floats_equal(a[i][j], b[i][j]))
		return 0;
    return 1;
}

int matrix_compare_matrix2(matrix2 a, matrix2 b) {
    for (int i = 0; i < 2; i++)
	for (int j = 0; j < 2; j++)
	    if (!ctm_floats_equal(a[i][j], b[i][j]))
		return 0;
    return 1;
}

void matrix_mult_matrix4(matrix4 a, matrix4 b, matrix4 c) {
    double t = 0;
    for (int k = 0; k < 4; k++) {
	for (int i = 0; i < 4; i++) {
	    for (int j = 0; j < 4; j++) {
		t += a[k][j] * b[j][i]; 
	    }
	    c[k][i] = t;
	    t = 0;
	}
    }
}

struct tuple matrix_mult_matrix4_tuple(matrix4 a, struct tuple b) {
    double t[4] = {0};
    double tt = 0;
    for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 4; j++) {
	    if (j == 0)
		tt += a[i][j] * b.x;
	    else if (j == 1)
		tt += a[i][j] * b.y;
	    else if (j == 2)
		tt += a[i][j] * b.z;
	    else if (j == 3)
		tt += a[i][j] * b.w;
	}
	t[i] = tt;
	tt = 0;
    }
    return (struct tuple) {t[0], t[1], t[2], t[3]};
}

void matrix_make_identity4(matrix4 m) {
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;

    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;

    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

void matrix_transpose4(matrix4 m, matrix4 d) {
    d[0][0] = m[0][0];
    d[1][1] = m[1][1];
    d[2][2] = m[2][2];
    d[3][3] = m[3][3];

    d[0][1] = m[1][0];
    d[1][0] = m[0][1];

    d[0][2] = m[2][0];
    d[2][0] = m[0][2];

    d[1][2] = m[2][1];
    d[2][1] = m[1][2];

    d[0][3] = m[3][0];
    d[3][0] = m[0][3];

    d[1][3] = m[3][1];
    d[3][1] = m[1][3];

    d[2][3] = m[3][2];
    d[3][2] = m[2][3];
}

double matrix_det_matrix2(matrix2 m) {
    return (m[0][0] * m[1][1] - m[1][0] * m[0][1]);
}

void matrix_sub_matrix3(matrix3 m, int i, int j, matrix2 d) {
    int j_range[2];
    int i_range[2];

    if (j == 0) {
	j_range[0] = 1;
	j_range[1] = 2;
    }
    else if (j == 1) {
	j_range[0] = 0;
	j_range[1] = 2;
    }
    else if (j == 2) {
	j_range[0] = 0;
	j_range[1] = 1;
    }

    if (i == 0) {
	i_range[0] = 1;
	i_range[1] = 2;
    }
    else if (i == 1) {
	i_range[0] = 0;
	i_range[1] = 2;
    }
    else if (i == 2) {
	i_range[0] = 0;
	i_range[1] = 1;
    }

    d[0][0] = m[i_range[0]][j_range[0]];
    d[0][1] = m[i_range[0]][j_range[1]];

    d[1][0] = m[i_range[1]][j_range[0]];
    d[1][1] = m[i_range[1]][j_range[1]];
}

void matrix_sub_matrix4(matrix4 m, int i, int j, matrix3 d) {
    int j_range[3];
    int i_range[3];

    if (j == 0) {
	j_range[0] = 1;
	j_range[1] = 2;
	j_range[2] = 3;
    }
    else if (j == 1) {
	j_range[0] = 0;
	j_range[1] = 2;
	j_range[2] = 3;
    }
    else if (j == 2) {
	j_range[0] = 0;
	j_range[1] = 1;
	j_range[2] = 3;
    }
    else if (j == 3) {
	j_range[0] = 0;
	j_range[1] = 1;
	j_range[2] = 2;
    }

    if (i == 0) {
	i_range[0] = 1;
	i_range[1] = 2;
	i_range[2] = 3;
    }
    else if (i == 1) {
	i_range[0] = 0;
	i_range[1] = 2;
	i_range[2] = 3;
    }
    else if (i == 2) {
	i_range[0] = 0;
	i_range[1] = 1;
	i_range[2] = 3;
    }
    else if (i == 3) {
	i_range[0] = 0;
	i_range[1] = 1;
	i_range[2] = 2;
    }

    d[0][0] = m[i_range[0]][j_range[0]];
    d[0][1] = m[i_range[0]][j_range[1]];
    d[0][2] = m[i_range[0]][j_range[2]];

    d[1][0] = m[i_range[1]][j_range[0]];
    d[1][1] = m[i_range[1]][j_range[1]];
    d[1][2] = m[i_range[1]][j_range[2]];

    d[2][0] = m[i_range[2]][j_range[0]];
    d[2][1] = m[i_range[2]][j_range[1]];
    d[2][2] = m[i_range[2]][j_range[2]];
}

double matrix_minor_matrix3(matrix3 m, int i, int j) {
    matrix2 sub;
    matrix_sub_matrix3(m, i, j, sub);
    return matrix_det_matrix2(sub);
}

double matrix_cofactor_matrix3(matrix3 m, int i, int j) {
    double t = matrix_minor_matrix3(m, i, j);
    int i_sign = (i % 2) ? (-1):(1);
    int j_sign = (j % 2) ? (-1):(1);
    int sign = i_sign * j_sign;
    return t * sign;
}

double matrix_det_matrix3(matrix3 m) {
    double det = 0;
    for (int j = 0; j < 3; j++) {
	det += m[0][j] * matrix_cofactor_matrix3(m, 0, j);
    }
    return det;
}

double matrix_minor_matrix4(matrix4 m, int i, int j) {
    matrix3 sub;
    matrix_sub_matrix4(m, i, j, sub);
    return matrix_det_matrix3(sub);
}

double matrix_cofactor_matrix4(matrix4 m, int i, int j) {
    double t = matrix_minor_matrix4(m, i, j);
    int i_sign = (i % 2) ? (-1):(1);
    int j_sign = (j % 2) ? (-1):(1);
    int sign = i_sign * j_sign;
    return t * sign;
}

double matrix_det_matrix4(matrix4 m) {
    double det = 0;
    for (int j = 0; j < 4; j++) {
	det += m[0][j] * matrix_cofactor_matrix4(m, 0, j);
    }
    return det;
}

int matrix_inverse_matrix4(matrix4 m, matrix4 d) {
    double det = matrix_det_matrix4(m);
    double c = 0;

    if (ctm_floats_equal(det, 0))
	return 0;

    for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 4; j++) {
	    c = matrix_cofactor_matrix4(m, i, j);
	    d[j][i] = c / det;
	}
    }

    return 1;
}

void matrix_new_translation4(double x, double y, double z, matrix4 d) {
    double r1[4] = {1,0,0,x};
    double r2[4] = {0,1,0,y};
    double r3[4] = {0,0,1,z};
    double r4[4] = {0,0,0,1};
    matrix_init_matrix4(d, r1, r2, r3, r4);
}

void matrix_new_scaling4(double x, double y, double z, matrix4 d) {
    double r1[4] = {x,0,0,0};
    double r2[4] = {0,y,0,0};
    double r3[4] = {0,0,z,0};
    double r4[4] = {0,0,0,1};
    matrix_init_matrix4(d, r1, r2, r3, r4);
}

void matrix_new_rotate_x(double a, matrix4 d) {
    double r1[4] = {1,0,0,0};
    double r2[4] = {0,cos(a),-sin(a),0};
    double r3[4] = {0,sin(a),cos(a),0};
    double r4[4] = {0,0,0,1};
    matrix_init_matrix4(d, r1, r2, r3, r4);
}

void matrix_new_rotate_y(double a, matrix4 d) {
    double r1[4] = {cos(a),0,sin(a),0};
    double r2[4] = {0,1,0,0};
    double r3[4] = {-sin(a),0,cos(a),0};
    double r4[4] = {0,0,0,1};
    matrix_init_matrix4(d, r1, r2, r3, r4);
}

void matrix_new_rotate_z(double a, matrix4 d) {
    double r1[4] = {cos(a),-sin(a),0,0};
    double r2[4] = {sin(a),cos(a),0,0};
    double r3[4] = {0,0,1,0};
    double r4[4] = {0,0,0,1};
    matrix_init_matrix4(d, r1, r2, r3, r4);
}

void matrix_new_shearing(double xy, double xz, double yx, double yz, double zx, double zy, matrix4 d) {
    double r1[4] = {1,xy,xz,0};
    double r2[4] = {yx,1,yz,0};
    double r3[4] = {zx,zy,1,0};
    double r4[4] = {0,0,0,1};
    matrix_init_matrix4(d, r1, r2, r3, r4);
}
