#include "../includes/camera.h"
#include "../includes/ray.h"
#include "canvas.h"
#include "ct_math.h"
#include "matrix.h"
#include "tuple.h"
#include "world.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static struct tuple
camera_color_for_aperture_pixel(struct camera* camera, struct world* world, const int i, const int j);

struct ray
camera_ray_for_aperture_pixel(struct camera* camera, int i, int j);

struct camera
camera_new_camera(int hsize, int vsize, double fov, double aperture_radius, double focal_dist) {
    struct camera cam;
    cam.hsize = hsize;
    cam.vsize = vsize;
    cam.fov = fov;
    cam.aperture_radius = (aperture_radius > 0) ? (aperture_radius):(0);
    cam.focal_dist = (aperture_radius > 0) ? (focal_dist):(1);
    cam.transform = matrix_make_identity4();
    cam.inv_transform = matrix_make_identity4();

    double half_view = cam.focal_dist * tan(cam.fov/2.);
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

    // Use depth of field
    if (camera->aperture_radius > 0) {
	for (int i = 0; i < camera->vsize; i++) {
	    for (int j = 0; j < camera->hsize; j++) {
		color = camera_color_for_aperture_pixel(camera, world, i, j);
		canvas_write_pixel(&canvas, i, j, color);
	    }
	}
    }

    // Do not use depth of field
    else {
	for (int i = 0; i < camera->vsize; i++) {
	    for (int j = 0; j < camera->hsize; j++) {
		ray = camera_ray_for_pixel(camera, i, j);
		color = world_color_at(world, &ray, MAX_RECUR_CALLS);
		canvas_write_pixel(&canvas, i, j, color);
	    }
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

void camera_make_view_transform(struct camera* cam, struct tuple from, struct tuple to, struct tuple up) {
    struct tuple forward = tuple_normalize(tuple_sub(to, from));
    struct tuple upn = tuple_normalize(up);
    struct tuple left = tuple_cross(forward, upn);
    struct tuple true_up = tuple_cross(left, forward);

    struct row4 r1 = matrix_new_row4(left.x, left.y, left.z, 0);
    struct row4 r2 = matrix_new_row4(true_up.x, true_up.y, true_up.z, 0);
    struct row4 r3 = matrix_new_row4(-forward.x, -forward.y, -forward.z, 0);
    struct row4 r4 = matrix_new_row4(0, 0, 0, 1);
    struct matrix4 orientation = matrix_new_matrix4(r1, r2, r3, r4);

    cam->transform = matrix_mult_matrix4(orientation, matrix_new_translation4(-from.x, -from.y, -from.z));
    cam->aperture_u = tuple_normalize(left);
    cam->aperture_v = tuple_normalize(true_up);
    camera_make_inv_view_transform(cam);
}

struct tuple
camera_make_point_on_aperture(struct camera* camera) {
    struct tuple scaled_u;
    struct tuple scaled_v;
    struct tuple point;

    for (;;) {
	scaled_u = tuple_scalar_mult(tuple_new_vector(1, 0, 0), (-1 + 2 * ctm_random_jitter()) * camera->aperture_radius);
	scaled_v = tuple_scalar_mult(tuple_new_vector(0, 1, 0), (-1 + 2 * ctm_random_jitter()) * camera->aperture_radius);
	if (tuple_mag(tuple_add(scaled_u, scaled_v)) <= camera->aperture_radius) {
	    point = tuple_add(scaled_u, scaled_v);
	    point.w = 1;
	    break;
	}
    }

    return point;
}

struct ray
camera_ray_for_aperture_pixel(struct camera* camera, int i, int j) {
    double ioffset = ((double) i + 0.5) * camera->pixel_size;
    double joffset = ((double) j + 0.5) * camera->pixel_size;
    double world_x = camera->half_width - joffset;
    double world_y = camera->half_height - ioffset;

    struct tuple pixel = matrix_mult_matrix4_tuple(camera->inv_transform,
	    tuple_new_point(world_x, world_y, -1 * camera->focal_dist));
    struct tuple origin = matrix_mult_matrix4_tuple(camera->inv_transform, camera_make_point_on_aperture(camera));
    struct tuple direction = tuple_normalize(tuple_sub(pixel, origin));

    return ray_new_ray(origin, direction);
}

static struct tuple
camera_color_for_aperture_pixel(struct camera* camera, struct world* world, const int i, const int j) {
    struct tuple color = tuple_new_color(0, 0, 0);
    struct ray ray;

    for (int k = 0; k < DOF_SAMPLES; k++) {
	ray = camera_ray_for_aperture_pixel(camera, i, j);
	color = tuple_add(color, world_color_at(world, &ray, MAX_RECUR_CALLS));
    }

    return tuple_scalar_div(color, DOF_SAMPLES);
}
