#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../includes/tuple.h"
#include "../../includes/ct_math.h"
#include "../../includes/canvas.h"
#include "../../includes/matrix.h"
#include "../../includes/ray.h"
#include "../../includes/intersection.h"
#include "../../includes/sphere.h"
#include "../../includes/lights.h"
#include "../../includes/materials.h"
#include "../../includes/world.h"
#include "../../includes/camera.h"
#include "../../includes/plane.h"
#include "../../includes/cube.h"
#include "../../includes/cylinder.h"
#include "../../includes/pattern.h"
#include "../../includes/cone.h"
#include "../../includes/group.h"
#include "../../includes/bounding_box.h"

static void soft_render_2(void);

int main(int argc, char *argv[]) {

    soft_render_2();
    
    return 0;
}

static void soft_render_2(void) {

    struct world* world = world_new_world();

    struct sphere* s = sphere_new_sphere();
    s->material.ambient = 0.01;
    s->material.specular = 0.95;
    s->material.diffuse = 0.45;
    s->material.reflective = 0.07;
    s->material.color = tuple_new_color(0.95, 1, 0.17);
    sphere_add_transform(s, matrix_new_scaling4(0.1, 0.1, 0.1));
    sphere_add_transform(s, matrix_new_translation4(0, 0.1, 0));
    world_add_sphere(world, s);

    s = sphere_new_sphere();
    s->material.ambient = 0.01;
    s->material.specular = 0.95;
    s->material.diffuse = 0.25;
    s->material.reflective = 0.03;
    s->material.color = tuple_new_color(1, 0.45, 0.76);
    sphere_add_transform(s, matrix_new_scaling4(0.075, 0.075, 0.075));
    sphere_add_transform(s, matrix_new_translation4(0.2, 0.075, -0.15));
    world_add_sphere(world, s);

    s = sphere_new_sphere();
    s->material.ambient = 0.01;
    s->material.specular = 0.95;
    s->material.diffuse = 0.25;
    s->material.reflective = 0.03;
    s->material.color = tuple_new_color(0.24, 0, 1);
    sphere_add_transform(s, matrix_new_scaling4(0.05, 0.05, 0.05));
    sphere_add_transform(s, matrix_new_translation4(-0.07, 0.05, -0.18));
    world_add_sphere(world, s);

    s = sphere_new_sphere();
    s->material.ambient = 0.01;
    s->material.specular = 0.95;
    s->material.diffuse = 0.25;
    s->material.reflective = 0.1;
    s->material.color = tuple_new_color(1, 0.2, 0);
    sphere_add_transform(s, matrix_new_scaling4(0.15, 0.15, 0.15));
    sphere_add_transform(s, matrix_new_translation4(0.1, 0.15, 0.5));
    world_add_sphere(world, s);

    s = sphere_new_sphere();
    s->material.ambient = 0.01;
    s->material.specular = 0.95;
    s->material.diffuse = 0.25;
    s->material.reflective = 0.15;
    s->material.color = tuple_new_color(0.25, 1, 0);
    sphere_add_transform(s, matrix_new_scaling4(0.12, 0.12, 0.12));
    sphere_add_transform(s, matrix_new_translation4(-0.45, 0.12, 0.3));
    world_add_sphere(world, s);

    struct plane* floor = plane_new_plane();
    floor->material.ambient = 0.005;
    floor->material.diffuse = 0.4;
    floor->material.specular = 0;
    floor->material.shininess = 1;
    //floor->material.color = tuple_new_color(1.2, 1.2, 1.2);
    floor->material.reflective = 0.08;
    floor->material.pattern =  pattern_checker(tuple_new_color(1, 1, 1), tuple_new_color(0.8, 0.8, 0.8));
    world_add_plane(world, floor);

    struct cube* cube = cube_new_cube();
    cube->material.color = tuple_new_color(1, 1, 1);
    cube->material.ambient = 1;
    cube->material.diffuse = 0;
    cube->material.specular = 0;
    cube->material.casts_shadows = 0;
    cube_add_transform(cube, matrix_new_scaling4(0.001, 0.1, 0.3));
    cube_add_transform(cube, matrix_new_translation4(1, 0.55, 0));
    world_add_cube(world, cube);
    

    struct tuple corner = tuple_new_point(1, 0.5, -0.3);
    struct tuple full_u = tuple_new_vector(0, 0, 0.6);
    struct tuple full_v = tuple_new_vector(0, 0.1, 0);
    struct area_light_rect light1 = lights_new_area_light_rect(corner, full_u, 5, full_v, 5, tuple_new_color(1, 1, 1));
    struct point_light light2 = lights_new_point_light(tuple_new_point(1, 0.5, -0.3), tuple_new_color(1, 1, 1));
    world_add_area_light_rect(world, &light1);
    //world_add_point_light(world, &light2);

    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, 0.95, 0.01, 0.547);
    struct tuple from = tuple_new_point(0.2, 0.2, -0.5);
    struct tuple to = tuple_new_point(-0.2, 0, 0.3);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera_make_view_transform(&camera, from, to, up);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
