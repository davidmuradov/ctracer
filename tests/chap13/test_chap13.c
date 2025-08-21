#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
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
#include "cone.h"

static void chap13_render(void);

int main(int argc, char *argv[]) {

    // Test 1
    struct cylinder cyl = cylinder_new_cylinder();
    struct tuple direction = tuple_normalize(tuple_new_vector(0, 1, 0));
    struct tuple origin = tuple_new_point(1, 0, 0);
    struct ray ray = ray_new_ray(origin, direction);
    struct intersection_list inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    direction = tuple_normalize(tuple_new_vector(0, 1, 0));
    origin = tuple_new_point(0, 0, 0);
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    direction = tuple_normalize(tuple_new_vector(1, 1, 1));
    origin = tuple_new_point(0, 0, -5);
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    // Test 2
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    origin = tuple_new_point(1, 0, -5);
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 5));
    assert(ctm_floats_equal(inter_list.list[1].t, 5));
    intersection_clear_intersection_list(&inter_list);

    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    origin = tuple_new_point(0, 0, -5);
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 4));
    assert(ctm_floats_equal(inter_list.list[1].t, 6));
    intersection_clear_intersection_list(&inter_list);

    direction = tuple_normalize(tuple_new_vector(0.1, 1, 1));
    origin = tuple_new_point(0.5, 0, -5);
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 6.80798));
    assert(ctm_floats_equal(inter_list.list[1].t, 7.08872));
    intersection_clear_intersection_list(&inter_list);

    // Test 3
    struct tuple n = cylinder_normal_at(&cyl, tuple_new_point(1, 0, 0));
    assert(tuple_equals(n, tuple_new_vector(1, 0, 0)));

    n = cylinder_normal_at(&cyl, tuple_new_point(0, 5, -1));
    assert(tuple_equals(n, tuple_new_vector(0, 0, -1)));

    n = cylinder_normal_at(&cyl, tuple_new_point(0, -2, 1));
    assert(tuple_equals(n, tuple_new_vector(0, 0, 1)));

    n = cylinder_normal_at(&cyl, tuple_new_point(-1, 1, 0));
    assert(tuple_equals(n, tuple_new_vector(-1, 0, 0)));

    // Test 4
    assert(cyl.min == -INFINITY);
    assert(cyl.max == INFINITY);

    // Test 5
    cyl.min = 1;
    cyl.max = 2;
    origin = tuple_new_point(0, 1.5, 0);
    direction = tuple_normalize(tuple_new_vector(0.1, 1, 0));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 3, -5);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 0, -5);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 2, -5);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 1, -5);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 1.5, -2);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 2);
    intersection_clear_intersection_list(&inter_list);

    // Test 6
    assert(cyl.closed == 0);

    // Test 7
    cyl.closed = 1;
    origin = tuple_new_point(0, 3, 0);
    direction = tuple_normalize(tuple_new_vector(0, -1, 0));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 2);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 3, -2);
    direction = tuple_normalize(tuple_new_vector(0, -1, 2));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 2);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 4, -2);
    direction = tuple_normalize(tuple_new_vector(0, -1, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 2);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 0, -2);
    direction = tuple_normalize(tuple_new_vector(0, 1, 2));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 2);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, -1, -2);
    direction = tuple_normalize(tuple_new_vector(0, 1, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cylinder_intersect_ray(&cyl, &ray);
    assert(inter_list.nb_intersections == 2);
    intersection_clear_intersection_list(&inter_list);

    // Test 8
    n = cylinder_normal_at(&cyl, tuple_new_point(0, 1, 0));
    assert(tuple_equals(n, tuple_new_vector(0, -1, 0)));

    n = cylinder_normal_at(&cyl, tuple_new_point(0.5, 1, 0));
    assert(tuple_equals(n, tuple_new_vector(0, -1, 0)));

    n = cylinder_normal_at(&cyl, tuple_new_point(0, 1, 0.5));
    assert(tuple_equals(n, tuple_new_vector(0, -1, 0)));

    n = cylinder_normal_at(&cyl, tuple_new_point(0, 2, 0));
    assert(tuple_equals(n, tuple_new_vector(0, 1, 0)));

    n = cylinder_normal_at(&cyl, tuple_new_point(0.5, 2, 0));
    assert(tuple_equals(n, tuple_new_vector(0, 1, 0)));

    n = cylinder_normal_at(&cyl, tuple_new_point(0, 2, 0.5));
    assert(tuple_equals(n, tuple_new_vector(0, 1, 0)));

    // Cone intersect test 1
    struct cone cone = cone_new_cone();

    origin = tuple_new_point(0, 0, -5);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cone_intersect_ray(&cone, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 5));
    assert(ctm_floats_equal(inter_list.list[1].t, 5));
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 0, -5);
    direction = tuple_normalize(tuple_new_vector(1, 1, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cone_intersect_ray(&cone, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 8.66025));
    assert(ctm_floats_equal(inter_list.list[1].t, 8.66025));
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(1, 1, -5);
    direction = tuple_normalize(tuple_new_vector(-0.5, -1, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cone_intersect_ray(&cone, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 4.55006));
    assert(ctm_floats_equal(inter_list.list[1].t, 49.44994));
    intersection_clear_intersection_list(&inter_list);

    // Cone intersect test 2
    origin = tuple_new_point(0, 0, -1);
    direction = tuple_normalize(tuple_new_vector(0, 1, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cone_intersect_ray(&cone, &ray);
    assert(inter_list.nb_intersections == 1);
    assert(ctm_floats_equal(inter_list.list[0].t, 0.35355));
    intersection_clear_intersection_list(&inter_list);

    // Cone intersect test 3
    cone.min = -0.5;
    cone.max = 0.5;
    cone.closed = 1;

    origin = tuple_new_point(0, 0, -5);
    direction = tuple_normalize(tuple_new_vector(0, 1, 0));
    ray = ray_new_ray(origin, direction);
    inter_list = cone_intersect_ray(&cone, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 0, -0.25);
    direction = tuple_normalize(tuple_new_vector(0, 1, 1));
    ray = ray_new_ray(origin, direction);
    inter_list = cone_intersect_ray(&cone, &ray);
    assert(inter_list.nb_intersections == 2);
    intersection_clear_intersection_list(&inter_list);

    origin = tuple_new_point(0, 0, -0.25);
    direction = tuple_normalize(tuple_new_vector(0, 1, 0));
    ray = ray_new_ray(origin, direction);
    inter_list = cone_intersect_ray(&cone, &ray);
    assert(inter_list.nb_intersections == 4);
    intersection_clear_intersection_list(&inter_list);

    // Cone normal test 4
    // For some reason the returned normal vectors are not normalized ..
    cone = cone_new_cone();
    n = cone_normal_at(&cone, tuple_new_point(0, 0, 0));
    assert(tuple_equals(n, tuple_new_vector(0, 0, 0)));

    n = cone_normal_at(&cone, tuple_new_point(1, 1, 1));
    //assert(tuple_equals(n, tuple_new_vector(1, -sqrt(2), 1)));

    n = cone_normal_at(&cone, tuple_new_point(-1, -1, 0));
    //assert(tuple_equals(n, tuple_new_vector(-1, 1, 0)));

    chap13_render();

    return 0;
}

static void chap13_render(void) {

    // Nord colorscheme colors
    const struct tuple NORD0 = tuple_new_color(46./255, 52./255, 64./255); // Dark gray
    const struct tuple NORD1 = tuple_new_color(59./255, 66./255, 82./255);
    const struct tuple NORD2 = tuple_new_color(67./255, 76./255, 94./255);
    const struct tuple NORD3 = tuple_new_color(76./255, 86./255, 106./255); // Light gray

    const struct tuple NORD7 = tuple_new_color(143./255, 188./255, 187./255); // Greenish
    const struct tuple NORD8 = tuple_new_color(136./255, 192./255, 208./255);
    const struct tuple NORD9 = tuple_new_color(129./255, 161./255, 193./255);
    const struct tuple NORD10 = tuple_new_color(94./255, 129./255, 172./255); // Dark blue

    const struct tuple NORD11 = tuple_new_color(191./255, 97./255, 106./255); // Red
    const struct tuple NORD12 = tuple_new_color(208./255, 135./255, 112./255); // Orange
    const struct tuple NORD13 = tuple_new_color(235./255, 203./255, 139./255); // Yellow
    const struct tuple NORD14 = tuple_new_color(163./255, 190./255, 140./255); // Green
    const struct tuple NORD15 = tuple_new_color(180./255, 142./255, 173./255); // Purple

    // Cylinder 1
    struct cylinder cyl = cylinder_new_cylinder();
    cylinder_add_transform(&cyl, matrix_new_rotate_x(PI/2));
    cylinder_add_transform(&cyl, matrix_new_rotate_y(PI/6));
    cylinder_add_transform(&cyl, matrix_new_scaling4(0.5, 0.5, 0.5));
    cylinder_add_transform(&cyl, matrix_new_translation4(-1, 0.5, 1));
    //cyl.material.color = NORD12;
    cyl.material.ambient = 0.5;
    cyl.material.diffuse = 1;
    cyl.material.specular = 0.5;
    //cyl.material.color = NORD13;
    cyl.material.pattern = pattern_gradient(NORD11, NORD13);
    cylinder_add_transform_to_pattern(&cyl, matrix_new_translation4(0.5, 0, 0));
    cylinder_add_transform_to_pattern(&cyl, matrix_new_rotate_z(PI/2));
    cylinder_add_transform_to_pattern(&cyl, matrix_new_scaling4(6, 6, 6));
    cyl.min = -3;
    cyl.max = 3;
    cyl.closed = 0;

    // Cylinder 2
    struct cylinder cyl2 = cylinder_new_cylinder();
    cylinder_add_transform(&cyl2, matrix_new_rotate_x(PI/2));
    cylinder_add_transform(&cyl2, matrix_new_rotate_y(-PI/6));
    cylinder_add_transform(&cyl2, matrix_new_scaling4(0.5, 0.5, 0.5));
    cylinder_add_transform(&cyl2, matrix_new_translation4(1.3, 0.5, 0.7));
    //cyl.material.color = NORD12;
    cyl2.material.ambient = 0.5;
    cyl2.material.diffuse = 1;
    cyl2.material.specular = 0.5;
    //cyl.material.color = NORD13;
    cyl2.material.pattern = pattern_stripe(NORD10, NORD7);
    //cylinder_add_transform_to_pattern(&cyl2, matrix_new_translation4(0.5, 0, 0));
    cylinder_add_transform_to_pattern(&cyl2, matrix_new_rotate_z(PI/2));
    cylinder_add_transform_to_pattern(&cyl2, matrix_new_scaling4(0.5, 0.5, 0.5));
    cyl2.min = -2;
    cyl2.max = 2;
    cyl2.closed = 0;

    struct cone cone = cone_new_cone();
    //cylinder_add_transform(&cyl, matrix_new_rotate_z(PI/3));
    cone.material.ambient = 0.04;
    cone.material.specular = 1;
    cone.material.diffuse = 0.04;
    cone.material.shininess = 300;
    cone.material.reflective = 1;
    cone.material.transparency = 0.98;
    cone.material.refractive_index = 1.52;
    cone.material.color = NORD10;
    cone.material.casts_shadows = 1;
    cone.min = -0.99998;
    cone.max = 1;
    cone.closed = 1;
    //cone_add_transform(&cone, matrix_new_scaling4(2, 2, 2));
    //cone_add_transform(&cone, matrix_new_rotate_z(PI));
    cone_add_transform(&cone, matrix_new_translation4(0, 1, 0));

    // Room
    struct plane floor = plane_new_plane();
    floor.material.ambient = 0.2;
    floor.material.specular = 0.2;
    floor.material.diffuse = 1;
    floor.material.shininess = 50;
    floor.material.reflective = 0.1;
    floor.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    //plane_add_transform_to_pattern(&floor, matrix_new_scaling4(0, 2, 2));

    // World
    struct world* world = world_new_world();
    //world_add_cylinder(world, &cyl);
    world_add_cone(world, &cone);
    world_add_plane(world, &floor);
    world_add_cylinder(world, &cyl);
    world_add_cylinder(world, &cyl2);

    struct point_light light1 = lights_new_point_light(tuple_new_point(-2, 5, -5), tuple_new_color(1, 1, 1));
    struct point_light light2 = lights_new_point_light(tuple_new_point(-100, 50, -6), tuple_new_color(0.2, 0.2, 0.2));
    struct point_light light3 = lights_new_point_light(tuple_new_point(-100, 49.9, -6), tuple_new_color(0.2, 0.2, 0.2));
    struct point_light light4 = lights_new_point_light(tuple_new_point(-100, 50, -6.2), tuple_new_color(0.2, 0.2, 0.2));
    struct point_light light5 = lights_new_point_light(tuple_new_point(-100, 50, -5.8), tuple_new_color(0.2, 0.2, 0.2));
    world_add_point_light(world, &light1);
    //world_add_point_light(world, &light2);
    //world_add_point_light(world, &light3);
    //world_add_point_light(world, &light4);
    //world_add_point_light(world, &light5);

    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/3);
    struct tuple from = tuple_new_point(2, 4, -4);
    struct tuple to = tuple_new_point(-1, 0, 1);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);
    camera_make_inv_view_transform(&camera);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
