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

static void chap12_render(void);

int main(int argc, char *argv[]) {

    // Test 1
    struct cube c = cube_new_cube();
    struct ray ray = ray_new_ray(tuple_new_point(5, 0.5, 0), tuple_new_vector(-1, 0, 0));
    struct intersection_list inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 4));
    assert(ctm_floats_equal(inter_list.list[1].t, 6));
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(-5, 0.5, 0), tuple_new_vector(1, 0, 0));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 4));
    assert(ctm_floats_equal(inter_list.list[1].t, 6));
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(0.5, 5, 0), tuple_new_vector(0, -1, 0));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 4));
    assert(ctm_floats_equal(inter_list.list[1].t, 6));
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(0.5, -5, 0), tuple_new_vector(0, 1, 0));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 4));
    assert(ctm_floats_equal(inter_list.list[1].t, 6));
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(0.5, 0, 5), tuple_new_vector(0, 0, -1));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 4));
    assert(ctm_floats_equal(inter_list.list[1].t, 6));
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(0.5, 0, -5), tuple_new_vector(0, 0, 1));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 4));
    assert(ctm_floats_equal(inter_list.list[1].t, 6));
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(0, 0.5, 0), tuple_new_vector(0, 0, 1));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, -1));
    assert(ctm_floats_equal(inter_list.list[1].t, 1));
    intersection_clear_intersection_list(&inter_list);

    // Test 2
    ray = ray_new_ray(tuple_new_point(-2, 0, 0), tuple_new_vector(0.2673, 0.5345, 0.8018));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(0, -2, 0), tuple_new_vector(0.8018, 0.2673, 0.5345));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(0, 0, -2), tuple_new_vector(0.5345, 0.8018, 0.2673));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(2, 0, 2), tuple_new_vector(0, 0, -1));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(0, 2, 2), tuple_new_vector(0, -1, 0));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    ray = ray_new_ray(tuple_new_point(2, 2, 0), tuple_new_vector(-1, 0, 0));
    inter_list = cube_intersect_ray(&c, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    // Test 3
    struct tuple point = tuple_new_point(1, 0.5, -0.8);
    assert(tuple_equals(cube_normal_at(&c, point), tuple_new_vector(1, 0, 0)));
    point = tuple_new_point(-1, -0.2, 0.9);
    assert(tuple_equals(cube_normal_at(&c, point), tuple_new_vector(-1, 0, 0)));
    point = tuple_new_point(-0.4, 1, -0.1);
    assert(tuple_equals(cube_normal_at(&c, point), tuple_new_vector(0, 1, 0)));
    point = tuple_new_point(0.3, -1, -0.7);
    assert(tuple_equals(cube_normal_at(&c, point), tuple_new_vector(0, -1, 0)));
    point = tuple_new_point(-0.6, 0.3, 1);
    assert(tuple_equals(cube_normal_at(&c, point), tuple_new_vector(0, 0, 1)));
    point = tuple_new_point(0.4, 0.4, -1);
    assert(tuple_equals(cube_normal_at(&c, point), tuple_new_vector(0, 0, -1)));
    point = tuple_new_point(1, 1, 1);
    assert(tuple_equals(cube_normal_at(&c, point), tuple_new_vector(1, 0, 0)));
    point = tuple_new_point(-1, -1, -1);
    assert(tuple_equals(cube_normal_at(&c, point), tuple_new_vector(-1, 0, 0)));

    chap12_render();

    return 0;
}

static void chap12_render(void) {

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

    // Floor
    struct cube floor = cube_new_cube();
    floor.material.ambient = 0.2;
    floor.material.specular = 0.2;
    floor.material.diffuse = 1;
    floor.material.shininess = 50;
    floor.material.reflective = 0;
    floor.material.pattern = pattern_ring(tuple_new_color(1, 1, 1), NORD10);
    cube_add_transform(&floor, matrix_new_scaling4(10, 0.1, 35));
    cube_add_transform(&floor, matrix_new_translation4(0, 0, 20));
    cube_add_transform_to_pattern(&floor, matrix_new_scaling4(2./10, 1, 2./35));

    // Ceil
    struct cube ceil = cube_new_cube();
    ceil.material.ambient = 0.2;
    ceil.material.specular = 0.2;
    ceil.material.diffuse = 1;
    ceil.material.shininess = 50;
    ceil.material.reflective = 0;
    ceil.material.pattern = pattern_ring(tuple_new_color(1, 1, 1), NORD10);
    cube_add_transform(&ceil, matrix_new_scaling4(10, 0.1, 35));
    cube_add_transform(&ceil, matrix_new_translation4(0, 20, 20));
    cube_add_transform_to_pattern(&ceil, matrix_new_scaling4(2./10, 1, 2./35));
    //cube_add_transform_to_pattern(&floor, matrix_new_rotate_z(-PI/2));

    // c1
    struct cube c1 = cube_new_cube();
    c1.material.ambient = 0.2;
    c1.material.specular = 0.2;
    c1.material.diffuse = 1;
    c1.material.shininess = 50;
    c1.material.reflective = 0;
    c1.material.pattern = pattern_stripe(tuple_new_color(1, 1, 1), NORD10);
    cube_add_transform(&c1, matrix_new_scaling4(0.25, 3, 35));
    cube_add_transform(&c1, matrix_new_translation4(-10.25, 2, 20));
    cube_add_transform_to_pattern(&c1, matrix_new_scaling4(1, 1, 1));
    cube_add_transform_to_pattern(&c1, matrix_new_rotate_z(PI/2));
    cube_add_transform_to_pattern(&c1, matrix_new_translation4(0, 1.15, 0));

    // c2
    struct cube c2 = cube_new_cube();
    c2.material.ambient = 0.2;
    c2.material.specular = 0.2;
    c2.material.diffuse = 1;
    c2.material.shininess = 50;
    c2.material.reflective = 0;
    c2.material.pattern = pattern_stripe(tuple_new_color(1, 1, 1), NORD10);
    cube_add_transform(&c2, matrix_new_scaling4(0.25, 3, 35));
    cube_add_transform(&c2, matrix_new_translation4(-10.25, 18, 20));
    cube_add_transform_to_pattern(&c2, matrix_new_scaling4(1, 1, 1));
    cube_add_transform_to_pattern(&c2, matrix_new_rotate_z(PI/2));
    cube_add_transform_to_pattern(&c2, matrix_new_translation4(0, 0.85, 0));

    // c3
    struct cube c3 = cube_new_cube();
    c3.material.ambient = 0.2;
    c3.material.specular = 0.2;
    c3.material.diffuse = 1;
    c3.material.shininess = 50;
    c3.material.reflective = 0;
    c3.material.pattern = pattern_stripe(tuple_new_color(1, 1, 1), NORD10);
    cube_add_transform(&c3, matrix_new_scaling4(0.25, 5, 3));
    cube_add_transform(&c3, matrix_new_translation4(-10.25, 10, 52));
    cube_add_transform_to_pattern(&c3, matrix_new_scaling4(0.51, 0.5, 0.5));
    cube_add_transform_to_pattern(&c3, matrix_new_rotate_z(PI/2));
    cube_add_transform_to_pattern(&c3, matrix_new_translation4(0, 1, 0));

    // c4
    struct cube c4 = cube_new_cube();
    c4.material.ambient = 0.2;
    c4.material.specular = 0.2;
    c4.material.diffuse = 1;
    c4.material.shininess = 50;
    c4.material.reflective = 0;
    c4.material.pattern = pattern_stripe(tuple_new_color(1, 1, 1), NORD10);
    cube_add_transform(&c4, matrix_new_scaling4(0.25, 5, 3));
    cube_add_transform(&c4, matrix_new_translation4(-10.25, 10, 32));
    cube_add_transform_to_pattern(&c4, matrix_new_scaling4(0.51, 0.5, 0.5));
    cube_add_transform_to_pattern(&c4, matrix_new_rotate_z(PI/2));
    cube_add_transform_to_pattern(&c4, matrix_new_translation4(0, 1, 0));

    // c5
    struct cube c5 = cube_new_cube();
    c5.material.ambient = 0.2;
    c5.material.specular = 0.2;
    c5.material.diffuse = 1;
    c5.material.shininess = 50;
    c5.material.reflective = 0;
    c5.material.pattern = pattern_stripe(tuple_new_color(1, 1, 1), NORD10);
    cube_add_transform(&c5, matrix_new_scaling4(0.25, 5, 3));
    cube_add_transform(&c5, matrix_new_translation4(-10.25, 10, 12));
    cube_add_transform_to_pattern(&c5, matrix_new_scaling4(0.51, 0.5, 0.5));
    cube_add_transform_to_pattern(&c5, matrix_new_rotate_z(PI/2));
    cube_add_transform_to_pattern(&c5, matrix_new_translation4(0, 1, 0));

    // glass
    struct cube glass = cube_new_cube();
    glass.material.ambient = 0.01;
    glass.material.specular = 0.8;
    glass.material.diffuse = 0.01;
    glass.material.shininess = 10;
    glass.material.reflective = 0.8;
    glass.material.transparency = 0.9;
    glass.material.refractive_index = 1.52;
    glass.material.color = tuple_new_color(1, 1, 1);
    glass.material.casts_shadows = 0;
    cube_add_transform(&glass, matrix_new_scaling4(0.05, 10, 35));
    cube_add_transform(&glass, matrix_new_translation4(-10.1, 10, 20));

    // Back
    struct cube back = cube_new_cube();
    back.material.ambient = 0.2;
    back.material.specular = 0.2;
    back.material.diffuse = 1;
    back.material.shininess = 50;
    back.material.reflective = 0;
    back.material.pattern = pattern_stripe(tuple_new_color(1, 1, 1), NORD10);
    cube_add_transform(&back, matrix_new_scaling4(10, 10, 0.1));
    cube_add_transform(&back, matrix_new_translation4(0, 10, 55));
    cube_add_transform_to_pattern(&back, matrix_new_scaling4(0.25, 0.25, 0.25));
    cube_add_transform_to_pattern(&back, matrix_new_rotate_z(PI/2));

    // Right
    struct cube right = cube_new_cube();
    right.material.ambient = 0.2;
    right.material.specular = 0.2;
    right.material.diffuse = 0.6;
    right.material.shininess = 50;
    right.material.reflective = 0;
    right.material.pattern = pattern_stripe(tuple_new_color(1, 1, 1), NORD10);
    cube_add_transform(&right, matrix_new_scaling4(0.1, 10, 35));
    cube_add_transform(&right, matrix_new_translation4(10, 10, 20));
    cube_add_transform_to_pattern(&right, matrix_new_scaling4(0.25, 0.25, 0.25));
    cube_add_transform_to_pattern(&right, matrix_new_rotate_z(PI/2));

    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s1, matrix_new_scaling4(3, 3, 3));
    sphere_add_transform(&s1, matrix_new_translation4(5, 3, 45));
    s1.material.diffuse = 0.03;
    s1.material.ambient = 0.03;
    s1.material.specular = 1;
    s1.material.shininess = 400;
    s1.material.transparency = 0.98;
    s1.material.refractive_index = 2.417;
    s1.material.reflective = 0.5;
    s1.material.color = tuple_new_color(1, 1, 1);

    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s2, matrix_new_scaling4(5, 5, 5));
    sphere_add_transform(&s2, matrix_new_translation4(-5, 5, 35));
    s2.material.diffuse = 0.03;
    s2.material.ambient = 0.03;
    s2.material.specular = 1;
    s2.material.shininess = 400;
    s2.material.transparency = 0.98;
    s2.material.refractive_index = 2.417;
    s2.material.reflective = 0.5;
    s2.material.color = tuple_new_color(1, 1, 1);

    struct sphere s3 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s3, matrix_new_scaling4(4, 4, 4));
    sphere_add_transform(&s3, matrix_new_translation4(-5, 4, 25));
    s3.material.diffuse = 0.03;
    s3.material.ambient = 0.03;
    s3.material.specular = 1;
    s3.material.shininess = 400;
    s3.material.transparency = 0.98;
    s3.material.refractive_index = 2.417;
    s3.material.reflective = 0.5;
    s3.material.color = tuple_new_color(1, 1, 1);

    struct cube cubie1 = cube_new_cube();
    cube_add_transform(&cubie1, matrix_new_scaling4(2, 2, 2));
    cube_add_transform(&cubie1, matrix_new_rotate_y(PI/3));
    cube_add_transform(&cubie1, matrix_new_translation4(3, 2, 15));
    cubie1.material.diffuse = 0.1;
    cubie1.material.ambient = 0.1;
    cubie1.material.specular = 1;
    cubie1.material.shininess = 400;
    cubie1.material.transparency = 0.98;
    cubie1.material.refractive_index = 1.52;
    cubie1.material.reflective = 0.3;
    cubie1.material.color = NORD11;

    struct cube cubie2 = cube_new_cube();
    cube_add_transform(&cubie2, matrix_new_scaling4(1, 1, 1));
    cube_add_transform(&cubie2, matrix_new_rotate_y(PI/6));
    cube_add_transform(&cubie2, matrix_new_translation4(3, 5, 15));
    cubie2.material.diffuse = 0.1;
    cubie2.material.ambient = 0.1;
    cubie2.material.specular = 1;
    cubie2.material.shininess = 400;
    cubie2.material.transparency = 0.98;
    cubie2.material.refractive_index = 1.52;
    cubie2.material.reflective = 0.3;
    cubie2.material.color = NORD14;

    struct cube cubie3 = cube_new_cube();
    cube_add_transform(&cubie3, matrix_new_scaling4(0.5, 7, 0.5));
    cube_add_transform(&cubie3, matrix_new_rotate_y(PI/2.5));
    cube_add_transform(&cubie3, matrix_new_translation4(5, 7, 20));
    cubie3.material.diffuse = 0.1;
    cubie3.material.ambient = 0.1;
    cubie3.material.specular = 1;
    cubie3.material.shininess = 400;
    cubie3.material.transparency = 0.98;
    cubie3.material.refractive_index = 1.52;
    cubie3.material.reflective = 0.3;
    cubie3.material.color = NORD10;

    // World
    struct world* world = world_new_world();
    world_add_cube(world, &floor);
    world_add_cube(world, &ceil);
    world_add_cube(world, &right);
    world_add_cube(world, &back);
    world_add_cube(world, &glass);
    world_add_cube(world, &c1);
    world_add_cube(world, &c2);
    world_add_cube(world, &c3);
    world_add_cube(world, &c4);
    world_add_cube(world, &c5);
    world_add_sphere(world, &s1);
    world_add_sphere(world, &s2);
    world_add_sphere(world, &s3);
    world_add_cube(world, &cubie1);
    world_add_cube(world, &cubie2);
    world_add_cube(world, &cubie3);

    struct point_light light1 = lights_new_point_light(tuple_new_point(-100, 50.1, -6), tuple_new_color(1, 1, 1));
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
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/1.8);
    struct tuple from = tuple_new_point(0, 9, 0);
    struct tuple to = tuple_new_point(0, 8, 10);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
