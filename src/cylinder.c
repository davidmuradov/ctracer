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
    c.default_transformation = matrix_make_identity4();
    c.type = CYLINDER;
    c.id = UNIQUE_ID_CYLINDER;
    c.material = materials_new_material();
    UNIQUE_ID_CYLINDER++;
    return c;
}
