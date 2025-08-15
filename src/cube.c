#include "../includes/cube.h"
#include "../includes/intersection.h"
#include "../includes/materials.h"
#include "../includes/matrix.h"
#include "../includes/ray.h"
#include "../includes/tuple.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int UNIQUE_ID_CUBE = 0;

struct cube
cube_new_cube(void) {
    struct cube c;
    c.default_transformation = matrix_make_identity4();
    c.type = CUBE;
    c.id = UNIQUE_ID_CUBE;
    c.material = materials_new_material();
    UNIQUE_ID_CUBE++;
    return c;
}
