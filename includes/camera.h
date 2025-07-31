#ifndef INCLUDES_CAMERA_H
#define INCLUDES_CAMERA_H

#include "sphere_list.h"
#include "vec3.h"

struct camera {
    int image_width;
    int image_height;
    struct vec3 center;
    struct vec3 px00_loc;
    struct vec3 px_del_u;
    struct vec3 px_del_v;
};

void camera_render(struct camera* camera, struct sphere_list* sl);

#endif
