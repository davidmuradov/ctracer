#include "../includes/camera.h"
#include "../includes/ray.h"
#include "canvas.h"
#include "matrix.h"
#include "tuple.h"
#include "world.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct camera
camera_new_camera(int hsize, int vsize, double fov) {
    struct camera cam;
    cam.hsize = hsize;
    cam.vsize = vsize;
    cam.fov = fov;
    cam.transform = matrix_make_identity4();
    cam.inv_transform = matrix_make_identity4();

    double half_view = tan(cam.fov/2.);
    double aspect = ((double) cam.hsize) / cam.vsize;

    if (aspect >= 1) {
	cam.half_width = half_view;
	cam.half_height = half_view / aspect;
    }
    else {
	cam.half_width = half_view * aspect;
	cam.half_height = half_view;
    }

    cam.pixel_size = (cam.half_width * 2) / cam.hsize;

    return cam;
}

struct ray
camera_ray_for_pixel(struct camera* camera, int i, int j) {
    double ioffset = ((double) i + 0.5) * camera->pixel_size;
    double joffset = ((double) j + 0.5) * camera->pixel_size;
    double world_x = camera->half_width - joffset;
    double world_y = camera->half_height - ioffset;

    /*
    struct matrix4 inv_transform;
    int su = matrix_inverse_matrix4(camera->transform, &inv_transform);
    if (su) {
	struct tuple pixel = matrix_mult_matrix4_tuple(inv_transform,
		tuple_new_point(world_x, world_y, -1));
	struct tuple origin = matrix_mult_matrix4_tuple(inv_transform, tuple_new_point(0, 0, 0));
	struct tuple direction = tuple_normalize(tuple_sub(pixel, origin));

	return ray_new_ray(origin, direction);
    }
    else {
	fprintf(stderr, "Matrix is non invertible\n");
	exit(1);
    }
    */
    struct tuple pixel = matrix_mult_matrix4_tuple(camera->inv_transform,
	    tuple_new_point(world_x, world_y, -1));
    struct tuple origin = matrix_mult_matrix4_tuple(camera->inv_transform, tuple_new_point(0, 0, 0));
    struct tuple direction = tuple_normalize(tuple_sub(pixel, origin));

    return ray_new_ray(origin, direction);
}

struct canvas
camera_render(struct camera* camera, struct world* world) {
    struct canvas canvas = canvas_new(camera->hsize, camera->vsize);
    struct ray ray;
    struct tuple color;

    for (int i = 0; i < camera->vsize; i++) {
	for (int j = 0; j < camera->hsize; j++) {
	    ray = camera_ray_for_pixel(camera, i, j);
	    color = world_color_at(world, &ray, MAX_RECUR_CALLS);
	    canvas_write_pixel(&canvas, i, j, color);
	}
    }

    return canvas;
}

void camera_make_inv_view_transform(struct camera* camera) {
    if (!matrix_inverse_matrix4(camera->transform, &(camera->inv_transform))) {
	fprintf(stderr, "Failed to calculate inverse transform of camera (cannot invert matrix)\n");
	exit(1);
    }
}
