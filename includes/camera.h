#ifndef INCLUDES_CAMERA_H
#define INCLUDES_CAMERA_H

#include "matrix.h"
#include "tuple.h"
#include "world.h"
#include "canvas.h"

#define DOF_SAMPLES	10

struct camera {
    int hsize;
    int vsize;
    double fov;
    double aperture_radius;
    double focal_dist;
    double half_width;
    double half_height;
    double pixel_size;
    struct matrix4 transform;
    struct matrix4 inv_transform;
};

struct camera
camera_new_camera(int hsize, int vsize, double fov, double aperture_radius, double focal_dist);

struct ray
camera_ray_for_pixel(struct camera* camera, int x, int y);

struct canvas
camera_render(struct camera* camera, struct world* world);

void
camera_make_view_transform(struct camera* cam, struct tuple from, struct tuple to, struct tuple up);

void
camera_make_inv_view_transform(struct camera* camera);

struct tuple
camera_make_point_on_aperture(struct camera* camera);

#endif
