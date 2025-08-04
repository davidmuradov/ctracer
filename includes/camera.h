#ifndef INCLUDES_CAMERA_H
#define INCLUDES_CAMERA_H

#include "matrix.h"
#include "world.h"
#include "canvas.h"

struct camera {
    int hsize;
    int vsize;
    double fov;
    double half_width;
    double half_height;
    double pixel_size;
    struct matrix4 transform;
};

struct camera
camera_new_camera(int hsize, int vsize, double fov);

struct ray
camera_ray_for_pixel(struct camera* camera, int x, int y);

struct canvas
camera_render(struct camera* camera, struct world* world);

#endif
