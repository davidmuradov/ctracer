#include "../includes/matrix.h"
#include "ct_math.h"
#include "tuple.h"
#include <math.h>

struct row4 matrix_new_row4(double a, double b, double c, double d) {
    return (struct row4) {a, b, c, d};
}

struct row3 matrix_new_row3(double a, double b, double c) {
    return (struct row3) {a, b, c};
}

struct row2 matrix_new_row2(double a, double b) {
    return (struct row2) {a, b};
}

struct matrix4 matrix_new_matrix4(struct row4 r1, struct row4 r2, struct row4 r3, struct row4 r4) {

    struct matrix4 m;

    m.grid[0][0] = r1.a;
    m.grid[0][1] = r1.b;
    m.grid[0][2] = r1.c;
    m.grid[0][3] = r1.d;

    m.grid[1][0] = r2.a;
    m.grid[1][1] = r2.b;
    m.grid[1][2] = r2.c;
    m.grid[1][3] = r2.d;

    m.grid[2][0] = r3.a;
    m.grid[2][1] = r3.b;
    m.grid[2][2] = r3.c;
    m.grid[2][3] = r3.d;

    m.grid[3][0] = r4.a;
    m.grid[3][1] = r4.b;
    m.grid[3][2] = r4.c;
    m.grid[3][3] = r4.d;

    return m;
}

struct matrix3 matrix_new_matrix3(struct row3 r1, struct row3 r2, struct row3 r3) {

    struct matrix3 m;

    m.grid[0][0] = r1.a;
    m.grid[0][1] = r1.b;
    m.grid[0][2] = r1.c;

    m.grid[1][0] = r2.a;
    m.grid[1][1] = r2.b;
    m.grid[1][2] = r2.c;

    m.grid[2][0] = r3.a;
    m.grid[2][1] = r3.b;
    m.grid[2][2] = r3.c;

    return m;
}

struct matrix2 matrix_new_matrix2(struct row2 r1, struct row2 r2) {

    struct matrix2 m;

    m.grid[0][0] = r1.a;
    m.grid[0][1] = r1.b;

    m.grid[1][0] = r2.a;
    m.grid[1][1] = r2.b;

    return m;
}

/*
void matrix_set_from_matrix4(struct matrix4 src, struct matrix4* dst) {
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    src.grid[i][j] = dst->grid[i][j];
}
*/

int matrix_compare_matrix4(struct matrix4 a, struct matrix4 b) {
    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    if (!ctm_floats_equal(a.grid[i][j], b.grid[i][j]))
		return 0;
    return 1;
}

int matrix_compare_matrix3(struct matrix3 a, struct matrix3 b) {
    for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	    if (!ctm_floats_equal(a.grid[i][j], b.grid[i][j]))
		return 0;
    return 1;
}

int matrix_compare_matrix2(struct matrix2 a, struct matrix2 b) {
    for (int i = 0; i < 2; i++)
	for (int j = 0; j < 2; j++)
	    if (!ctm_floats_equal(a.grid[i][j], b.grid[i][j]))
		return 0;
    return 1;
}

struct matrix4 matrix_mult_matrix4(struct matrix4 a, struct matrix4 b) {
    double t = 0;
    struct matrix4 c;
    for (int k = 0; k < 4; k++) {
	for (int i = 0; i < 4; i++) {
	    for (int j = 0; j < 4; j++) {
		t += a.grid[k][j] * b.grid[j][i]; 
	    }
	    c.grid[k][i] = t;
	    t = 0;
	}
    }
    
    return c;
}

struct tuple matrix_mult_matrix4_tuple(struct matrix4 a, struct tuple b) {
    double t[4] = {0};
    double tt = 0;
    for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 4; j++) {
	    if (j == 0)
		tt += a.grid[i][j] * b.x;
	    else if (j == 1)
		tt += a.grid[i][j] * b.y;
	    else if (j == 2)
		tt += a.grid[i][j] * b.z;
	    else if (j == 3)
		tt += a.grid[i][j] * b.w;
	}
	t[i] = tt;
	tt = 0;
    }

    return (struct tuple) {t[0], t[1], t[2], t[3]};
}

struct matrix4 matrix_make_identity4(void) {

    struct matrix4 m;

    m.grid[0][0] = 1;
    m.grid[0][1] = 0;
    m.grid[0][2] = 0;
    m.grid[0][3] = 0;

    m.grid[1][0] = 0;
    m.grid[1][1] = 1;
    m.grid[1][2] = 0;
    m.grid[1][3] = 0;

    m.grid[2][0] = 0;
    m.grid[2][1] = 0;
    m.grid[2][2] = 1;
    m.grid[2][3] = 0;

    m.grid[3][0] = 0;
    m.grid[3][1] = 0;
    m.grid[3][2] = 0;
    m.grid[3][3] = 1;

    return m;
}

struct matrix4 matrix_transpose4(struct matrix4 a) {

    struct matrix4 m;

    m.grid[0][0] = a.grid[0][0];
    m.grid[1][1] = a.grid[1][1];
    m.grid[2][2] = a.grid[2][2];
    m.grid[3][3] = a.grid[3][3];

    m.grid[0][1] = a.grid[1][0];
    m.grid[1][0] = a.grid[0][1];

    m.grid[0][2] = a.grid[2][0];
    m.grid[2][0] = a.grid[0][2];

    m.grid[1][2] = a.grid[2][1];
    m.grid[2][1] = a.grid[1][2];

    m.grid[0][3] = a.grid[3][0];
    m.grid[3][0] = a.grid[0][3];

    m.grid[1][3] = a.grid[3][1];
    m.grid[3][1] = a.grid[1][3];

    m.grid[2][3] = a.grid[3][2];
    m.grid[3][2] = a.grid[2][3];

    return m;
}

double matrix_det_matrix2(struct matrix2 a) {
    return (a.grid[0][0] * a.grid[1][1] - a.grid[1][0] * a.grid[0][1]);
}

struct matrix2 matrix_sub_matrix3(struct matrix3 a, int i, int j) {

    struct matrix2 m;

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

    m.grid[0][0] = a.grid[i_range[0]][j_range[0]];
    m.grid[0][1] = a.grid[i_range[0]][j_range[1]];

    m.grid[1][0] = a.grid[i_range[1]][j_range[0]];
    m.grid[1][1] = a.grid[i_range[1]][j_range[1]];

    return m;
}


struct matrix3 matrix_sub_matrix4(struct matrix4 a, int i, int j) {

    struct matrix3 m;

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

    m.grid[0][0] = a.grid[i_range[0]][j_range[0]];
    m.grid[0][1] = a.grid[i_range[0]][j_range[1]];
    m.grid[0][2] = a.grid[i_range[0]][j_range[2]];

    m.grid[1][0] = a.grid[i_range[1]][j_range[0]];
    m.grid[1][1] = a.grid[i_range[1]][j_range[1]];
    m.grid[1][2] = a.grid[i_range[1]][j_range[2]];

    m.grid[2][0] = a.grid[i_range[2]][j_range[0]];
    m.grid[2][1] = a.grid[i_range[2]][j_range[1]];
    m.grid[2][2] = a.grid[i_range[2]][j_range[2]];

    return m;
}

double matrix_minor_matrix3(struct matrix3 a, int i, int j) {
    struct matrix2 sub = matrix_sub_matrix3(a, i, j);
    return matrix_det_matrix2(sub);
}

double matrix_cofactor_matrix3(struct matrix3 a, int i, int j) {
    double t = matrix_minor_matrix3(a, i, j);
    int i_sign = (i % 2) ? (-1):(1);
    int j_sign = (j % 2) ? (-1):(1);
    int sign = i_sign * j_sign;
    return t * sign;
}

double matrix_det_matrix3(struct matrix3 a) {
    double det = 0;
    for (int j = 0; j < 3; j++) {
	det += a.grid[0][j] * matrix_cofactor_matrix3(a, 0, j);
    }
    return det;
}

double matrix_minor_matrix4(struct matrix4 a, int i, int j) {
    struct matrix3 sub = matrix_sub_matrix4(a, i, j);
    return matrix_det_matrix3(sub);
}

double matrix_cofactor_matrix4(struct matrix4 a, int i, int j) {
    double t = matrix_minor_matrix4(a, i, j);
    int i_sign = (i % 2) ? (-1):(1);
    int j_sign = (j % 2) ? (-1):(1);
    int sign = i_sign * j_sign;
    return t * sign;
}

double matrix_det_matrix4(struct matrix4 a) {
    double det = 0;
    for (int j = 0; j < 4; j++) {
	det += a.grid[0][j] * matrix_cofactor_matrix4(a, 0, j);
    }
    return det;
}

int matrix_inverse_matrix4(struct matrix4 src, struct matrix4* dst) {
    double det = matrix_det_matrix4(src);
    double c = 0;

    if (ctm_floats_equal(det, 0))
	return 0;

    for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 4; j++) {
	    c = matrix_cofactor_matrix4(src, i, j);
	    dst->grid[j][i] = c / det;
	}
    }

    return 1;
}

struct matrix4 matrix_new_translation4(double x, double y, double z) {
    struct row4 r1 = matrix_new_row4(1, 0, 0, x);
    struct row4 r2 = matrix_new_row4(0, 1, 0, y);
    struct row4 r3 = matrix_new_row4(0, 0, 1, z);
    struct row4 r4 = matrix_new_row4(0, 0, 0, 1);
    return matrix_new_matrix4(r1, r2, r3, r4);
}

struct matrix4 matrix_new_scaling4(double x, double y, double z) {
    struct row4 r1 = matrix_new_row4(x,0,0,0);
    struct row4 r2 = matrix_new_row4(0,y,0,0);
    struct row4 r3 = matrix_new_row4(0,0,z,0);
    struct row4 r4 = matrix_new_row4(0,0,0,1);
    return matrix_new_matrix4(r1, r2, r3, r4);
}

struct matrix4 matrix_new_rotate_x(double a) {
    struct row4 r1 = matrix_new_row4(1,0,0,0);
    struct row4 r2 = matrix_new_row4(0,cos(a),-sin(a),0);
    struct row4 r3 = matrix_new_row4(0,sin(a),cos(a),0);
    struct row4 r4 = matrix_new_row4(0,0,0,1);
    return matrix_new_matrix4(r1, r2, r3, r4);
}

struct matrix4 matrix_new_rotate_y(double a) {
    struct row4 r1 = matrix_new_row4(cos(a),0,sin(a),0);
    struct row4 r2 = matrix_new_row4(0,1,0,0);
    struct row4 r3 = matrix_new_row4(-sin(a),0,cos(a),0);
    struct row4 r4 = matrix_new_row4(0,0,0,1);
    return matrix_new_matrix4(r1, r2, r3, r4);
}

struct matrix4 matrix_new_rotate_z(double a) {
    struct row4 r1 = matrix_new_row4(cos(a),-sin(a),0,0);
    struct row4 r2 = matrix_new_row4(sin(a),cos(a),0,0);
    struct row4 r3 = matrix_new_row4(0,0,1,0);
    struct row4 r4 = matrix_new_row4(0,0,0,1);
    return matrix_new_matrix4(r1, r2, r3, r4);
}

struct matrix4 matrix_new_shearing(double xy, double xz, double yx, double yz, double zx, double zy) {
    struct row4 r1 = matrix_new_row4(1,xy,xz,0);
    struct row4 r2 = matrix_new_row4(yx,1,yz,0);
    struct row4 r3 = matrix_new_row4(zx,zy,1,0);
    struct row4 r4 = matrix_new_row4(0,0,0,1);
    return matrix_new_matrix4(r1, r2, r3, r4);
}

struct matrix4 matrix_view_transform(struct tuple from, struct tuple to, struct tuple up) {
    struct tuple forward = tuple_normalize(tuple_sub(to, from));
    struct tuple upn = tuple_normalize(up);
    struct tuple left = tuple_cross(forward, upn);
    struct tuple true_up = tuple_cross(left, forward);

    struct row4 r1 = matrix_new_row4(left.x, left.y, left.z, 0);
    struct row4 r2 = matrix_new_row4(true_up.x, true_up.y, true_up.z, 0);
    struct row4 r3 = matrix_new_row4(-forward.x, -forward.y, -forward.z, 0);
    struct row4 r4 = matrix_new_row4(0, 0, 0, 1);
    struct matrix4 orientation = matrix_new_matrix4(r1, r2, r3, r4);

    return matrix_mult_matrix4(orientation, matrix_new_translation4(-from.x, -from.y, -from.z));
}
