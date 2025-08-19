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
    struct plane floor = plane_new_plane();
    floor.material.ambient = 0.2;
    floor.material.specular = 0.2;
    floor.material.diffuse = 1;
    floor.material.shininess = 50;
    floor.material.reflective = 0;
    floor.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    plane_add_transform_to_pattern(&floor, matrix_new_scaling4(2, 2, 2));

    // Ceil
    struct plane ceil = plane_new_plane();
    ceil.material.ambient = 0.2;
    ceil.material.specular = 0.2;
    ceil.material.diffuse = 1;
    ceil.material.shininess = 50;
    ceil.material.reflective = 0;
    ceil.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    plane_add_transform(&ceil, matrix_new_translation4(0, 16, 0));
    plane_add_transform_to_pattern(&ceil, matrix_new_scaling4(2, 2, 2));

    // Front
    struct plane front = plane_new_plane();
    front.material.ambient = 0.2;
    front.material.specular = 0.2;
    front.material.diffuse = 0.6;
    front.material.shininess = 50;
    front.material.reflective = 0;
    front.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    plane_add_transform(&front, matrix_new_rotate_x(-PI/2));
    plane_add_transform(&front, matrix_new_translation4(0, 0, 16));
    plane_add_transform_to_pattern(&front, matrix_new_scaling4(2, 2, 2));

    // Back
    struct plane back = plane_new_plane();
    back.material.ambient = 0.2;
    back.material.specular = 0.2;
    back.material.diffuse = 0.6;
    back.material.shininess = 50;
    back.material.reflective = 0;
    back.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    plane_add_transform(&back, matrix_new_rotate_x(PI/2));
    plane_add_transform(&back, matrix_new_translation4(0, 0, -16));
    plane_add_transform_to_pattern(&back, matrix_new_scaling4(2, 2, 2));

    // Left
    struct plane left = plane_new_plane();
    left.material.ambient = 0.2;
    left.material.specular = 0.2;
    left.material.diffuse = 0.6;
    left.material.shininess = 50;
    left.material.reflective = 0;
    left.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    plane_add_transform(&left, matrix_new_rotate_z(-PI/2));
    plane_add_transform(&left, matrix_new_translation4(-16, 0, 0));
    plane_add_transform_to_pattern(&left, matrix_new_scaling4(2, 2, 2));
    plane_add_transform_to_pattern(&left, matrix_new_translation4(2, 0, 0));

    // Right
    struct plane right = plane_new_plane();
    right.material.ambient = 0.2;
    right.material.specular = 0.2;
    right.material.diffuse = 0.6;
    right.material.shininess = 50;
    right.material.reflective = 0;
    right.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    plane_add_transform(&right, matrix_new_rotate_z(PI/2));
    plane_add_transform(&right, matrix_new_translation4(16, 0, 0));
    plane_add_transform_to_pattern(&right, matrix_new_scaling4(2, 2, 2));
    plane_add_transform_to_pattern(&right, matrix_new_translation4(2, 0, 0));

    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s1, matrix_new_scaling4(3, 3, 3));
    sphere_add_transform(&s1, matrix_new_translation4(13, 3, 13));
    s1.material.diffuse = 0.7;
    s1.material.ambient = 0.75;
    s1.material.specular = 0.6;
    s1.material.shininess = 100;
    s1.material.reflective = 0.75;
    s1.material.color = tuple_new_color(0, 0, 0);
    sphere_add_transform_to_pattern(&s1, matrix_new_scaling4(3, 3, 3));
    sphere_add_transform_to_pattern(&s1, matrix_new_translation4(1, 0, 0));

    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s2, matrix_new_scaling4(2, 2, 2));
    sphere_add_transform(&s2, matrix_new_translation4(9, 2, 3));
    s2.material.ambient = 0.4;
    s2.material.diffuse = 1.5;
    s2.material.specular = 0.6;
    s2.material.shininess = 100;
    s2.material.reflective = 0.03;
    s2.material.pattern = pattern_gradient(NORD10, NORD7);
    sphere_add_transform_to_pattern(&s2, matrix_new_scaling4(2, 2, 2));
    sphere_add_transform_to_pattern(&s2, matrix_new_translation4(1, 0, 0));
    sphere_add_transform_to_pattern(&s2, matrix_new_rotate_y(PI));
    //s2.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_rotate_y(PI/3), s2.material.pattern.default_transformation);

    struct sphere s3 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s3.material.diffuse = 0.01;
    s3.material.ambient = 0.01;
    s3.material.specular = 1;
    s3.material.shininess = 5;
    s3.material.reflective = 0.9;
    s3.material.transparency = 0.95;
    s3.material.refractive_index = 1.52;
    s3.material.color = tuple_new_color(1, 1, 1);
    sphere_add_transform(&s3, matrix_new_scaling4(1, 1, 1));
    sphere_add_transform(&s3, matrix_new_translation4(5, 1, 9));

    struct sphere s4 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s4, matrix_new_scaling4(1.5, 1.5, 1.5));
    sphere_add_transform(&s4, matrix_new_translation4(3, 1.5, 7));
    s4.material.ambient = 0.4;
    s4.material.diffuse = 1.5;
    s4.material.specular = 0.6;
    s4.material.shininess = 100;
    s4.material.reflective = 0.03;
    s4.material.pattern = pattern_gradient(NORD9, NORD12);
    sphere_add_transform_to_pattern(&s4, matrix_new_scaling4(2, 2, 2));
    sphere_add_transform_to_pattern(&s4, matrix_new_translation4(1, 0, 0));
    sphere_add_transform_to_pattern(&s4, matrix_new_rotate_y(PI/2));
    sphere_add_transform_to_pattern(&s4, matrix_new_rotate_x(PI/3));

    struct sphere s5 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s5.material.diffuse = 0.1;
    s5.material.ambient = 0.1;
    s5.material.specular = 1;
    s5.material.shininess = 5;
    s5.material.reflective = 0.7;
    s5.material.transparency = 0.8;
    s5.material.refractive_index = 1.52;
    s5.material.color = NORD13;
    sphere_add_transform(&s5, matrix_new_scaling4(1, 1, 1));
    sphere_add_transform(&s5, matrix_new_translation4(7, 1, 5));

    // World
    struct world* world = world_new_world();
    world_add_plane(world, &floor);
    world_add_plane(world, &ceil);
    world_add_plane(world, &front);
    world_add_plane(world, &back);
    world_add_plane(world, &left);
    world_add_plane(world, &right);
    world_add_sphere(world, &s1);
    world_add_sphere(world, &s2);
    world_add_sphere(world, &s3);
    world_add_sphere(world, &s4);
    world_add_sphere(world, &s5);

    struct point_light light1 = lights_new_point_light(tuple_new_point(0, 8, 0), tuple_new_color(1, 1, 1));
    struct point_light light2 = lights_new_point_light(tuple_new_point(15, 15, 15), tuple_new_color(0.2, 0.2, 0.2));
    world_add_point_light(world, &light1);
    world_add_point_light(world, &light2);

    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/3);
    struct tuple from = tuple_new_point(12, 2, 3);
    struct tuple to = tuple_new_point(12, 3.3, 16);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
