#include "../includes/cylinder.h"
#include "../includes/intersection.h"
#include "../includes/materials.h"
#include "../includes/matrix.h"
#include "../includes/ray.h"
#include "../includes/tuple.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int UNIQUE_ID_CYLINDER = 0;

struct cylinder
cylinder_new_cylinder(void) {
    struct cylinder c;
    c.transform = matrix_make_identity4();
    c.inv_transform = matrix_make_identity4();
    c.transp_inv_transform = matrix_make_identity4();
    c.type = CYLINDER;
    c.id = UNIQUE_ID_CYLINDER;
    c.material = materials_new_material();
    UNIQUE_ID_CYLINDER++;
    return c;
}

void
cylinder_make_inv_transform(struct cylinder* cyl) {
    matrix_inverse_matrix4(cyl->transform, &(cyl->inv_transform));
}

void
cylinder_make_transp_inv_transform(struct cylinder* cyl) {
    cyl->transp_inv_transform = matrix_transpose4(cyl->inv_transform);
}
