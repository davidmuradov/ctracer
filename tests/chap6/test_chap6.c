#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/tuple.h"
#include "../../includes/ct_math.h"
#include "../../includes/canvas.h"
#include "../../includes/matrix.h"
#include "../../includes/ray.h"
#include "intersection.h"
#include "sphere.h"

static void chap5_render(void);

int main(int argc, char *argv[]) {

    //chap5_render();

    return EXIT_SUCCESS;
}

static void chap5_render(void) {

    struct ray r = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    struct sphere s = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct canvas canvas = canvas_new();
    struct tuple bg = tuple_new_color(76./255, 86./255, 106./255);
    struct tuple fg = tuple_new_color(136./255, 192./255, 208./255);

    //struct tuple viewport_origin = tuple_new_point(0, 0, 10);
    double viewport_height = 9;
    double viewport_width = 16;
    struct tuple viewport_top_left = tuple_new_point(-viewport_width/2, viewport_height/2, 10);
    struct tuple viewport_u = tuple_new_vector(viewport_width, 0, 0);
    struct tuple viewport_v = tuple_new_vector(0, -viewport_height, 0);
    struct tuple viewport_u_del = tuple_scalar_div(viewport_u, CANVAS_WIDTH);
    struct tuple viewport_v_del = tuple_scalar_div(viewport_v, CANVAS_HEIGHT);
    struct tuple diag = tuple_scalar_mult(tuple_add(viewport_u_del, viewport_v_del), 0.5);
    struct tuple pixel00_loc = tuple_add(viewport_top_left, diag);

    struct tuple current_pixel;
    struct intersection_list inter_list;

    // Main render loop
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
	for (int j = 0; j < CANVAS_WIDTH; j++) {
	    current_pixel = tuple_add(pixel00_loc, tuple_add(tuple_scalar_mult(viewport_u_del, j),
			tuple_scalar_mult(viewport_v_del, i)));
	    r.dir = tuple_sub(current_pixel, r.o);
	    inter_list = sphere_intersect_ray(&s, &r);

	    if (inter_list.nb_intersections) {
		canvas_write_pixel(&canvas, i, j, fg);
	    }
	    else {
		canvas_write_pixel(&canvas, i, j, bg);
	    }

	    intersection_clear_intersection_list(&inter_list);
	}
    }

    // Write to ppm image
    canvas_to_ppm(&canvas);
    canvas_free_canvas(&canvas);
}
