#include "includes/print_image.h"
#include "includes/pixels.h"
#include "includes/vec3.h"
#include "includes/ray.h"
#include "includes/sphere.h"
#include "includes/camera.h"
#include <math.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    struct pixel* im = init_image(H, W);

    struct camera cam;
    camera_render(&cam, NULL); // Need to pass a list of spheres instead of NULL

    return 0;
}
