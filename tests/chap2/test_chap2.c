#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/tuple.h"
#include "../../includes/ct_math.h"
#include "../../includes/canvas.h"

int main(int argc, char *argv[]) {

    struct tuple v1 = tuple_new_color(-0.5, 0.4, 1.7);
    //tuple_print(v1);

    v1 = tuple_new_color(0.9, 0.6, 0.75);
    struct tuple v2 = tuple_new_color(0.7, 0.1, 0.25);
    struct tuple v3 = tuple_add(v1, v2);
    struct tuple v4 = tuple_new_color(1.6, 0.7, 1);
    assert(tuple_equals(v3, v4));

    v1 = tuple_new_color(0.9, 0.6, 0.75);
    v2 = tuple_new_color(0.7, 0.1, 0.25);
    v3 = tuple_sub(v1, v2);
    v4 = tuple_new_color(0.2, 0.5, 0.5);
    assert(tuple_equals(v3, v4));

    v1 = tuple_new_color(0.2, 0.3, 0.4);
    v3 = tuple_scalar_mult(v1, 2);
    v4 = tuple_new_color(0.4, 0.6, 0.8);
    assert(tuple_equals(v3, v4));

    v1 = tuple_new_color(1.0, 0.2, 0.4);
    v2 = tuple_new_color(0.9, 1.0, 0.1);
    v3 = tuple_color_mult(v1, v2);
    v4 = tuple_new_color(0.9, 0.2, 0.04);
    assert(tuple_equals(v3, v4));

    struct canvas canvas = canvas_new();
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
	for (int j = 0; j < CANVAS_WIDTH; j++) {
	    assert(ctm_floats_equal(0, canvas.grid[INDEX(i, j)].x));
	    assert(ctm_floats_equal(0, canvas.grid[INDEX(i, j)].y));
	    assert(ctm_floats_equal(0, canvas.grid[INDEX(i, j)].z));
	    assert(ctm_floats_equal(2, canvas.grid[INDEX(i, j)].w));
	}
    }

    v1 = tuple_new_color(1, 0, 0);
    canvas_write_pixel(&canvas, 2, 3, v1);
    assert(tuple_equals(canvas.grid[INDEX(2, 3)], v1));

    canvas_to_ppm(&canvas);

    return EXIT_SUCCESS;
}
