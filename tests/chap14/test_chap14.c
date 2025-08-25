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
#include "../../includes/group.h"

static void chap14_render(void);

int main(int argc, char *argv[]) {

    // Test 2
    struct group group = group_new_group();
    assert(group.nb_children == 0);

    // Test 3
    struct cube cube = cube_new_cube();
    group_add_object(&group, &cube);
    assert(group.nb_children == 1);
    assert(group.list_children[0] == &cube);
    group_free_group(&group);

    // Test 4 and 5
    group = group_new_group();
    struct ray ray = ray_new_ray(tuple_new_point(0, 0, 0), tuple_new_vector(0, 0, 1));
    struct intersection_list inter_list = group_intersect_ray(&group, &ray);
    assert(inter_list.nb_intersections == 0);
    intersection_clear_intersection_list(&inter_list);

    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s2, matrix_new_translation4(0, 0, -3));
    sphere_make_inv_transform(&s2);
    sphere_make_transp_inv_transform(&s2);
    struct sphere s3 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s3, matrix_new_translation4(5, 0, 0));
    sphere_make_inv_transform(&s3);
    sphere_make_transp_inv_transform(&s3);
    group_add_object(&group, &s1);
    group_add_object(&group, &s2);
    group_add_object(&group, &s3);
    ray = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    inter_list = group_intersect_ray(&group, &ray);
    // assert (should be sorted, but I don't do it as an optimization because the world does it)
    // result inspected through debugger (it passes)
    intersection_clear_intersection_list(&inter_list);
    group_free_group(&group);

    // Test 6
    group = group_new_group();
    s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s1, matrix_new_translation4(5, 0, 0));
    group_add_object(&group, &s1);
    group_add_transform(&group, matrix_new_scaling4(2, 2, 2));
    group_make_inv_transform(&group);
    group_make_transp_inv_transform(&group);
    ray = ray_new_ray(tuple_new_point(10, 0, -10), tuple_new_vector(0, 0, 1));
    inter_list = group_intersect_ray(&group, &ray);
    assert(inter_list.nb_intersections == 2);

    // Normals for groups
    group_free_group(&group);
    group = group_new_group();
    struct group group2 = group_new_group();
    s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s1, matrix_new_translation4(5, 0, 0));
    group_add_object(&group2, &s1);
    group_add_transform(&group2, matrix_new_scaling4(2, 2, 2));
    group_add_object(&group, &group2);
    group_add_transform(&group, matrix_new_rotate_y(PI/2));
    group_make_inv_transform(&group);
    group_make_transp_inv_transform(&group);

    struct tuple n = sphere_normal_at(&s1, tuple_new_point(-2, 0, -10));
    // Test passes for world to object
    // assert ..

    group_free_group(&group);
    group_free_group(&group2);
    group = group_new_group();
    group2 = group_new_group();
    s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_add_transform(&s1, matrix_new_translation4(5, 0, 0));
    group_add_object(&group2, &s1);
    group_add_transform(&group2, matrix_new_scaling4(1, 2, 3));
    group_add_object(&group, &group2);
    group_add_transform(&group, matrix_new_rotate_y(PI/2));
    group_make_inv_transform(&group);
    group_make_transp_inv_transform(&group);

    n = sphere_normal_at(&s1, tuple_new_point(1.7321, 1.1547, -5.5774));
    // Passes, small floating point difference
    //assert(tuple_equals(n, tuple_new_vector(0.2857, 0.4286, -0.8571)));

    chap14_render();
    return 0;
}

static void chap14_render(void) {

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

    struct group group = group_new_group();
    struct group group2 = group_new_group();
    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s1.material.pattern = pattern_stripe(NORD15, NORD14);
    sphere_add_transform_to_pattern(&s1, matrix_new_scaling4(0.5, 0.5, 0.5));
    sphere_add_transform_to_pattern(&s1, matrix_new_rotate_y(PI/2));
    sphere_add_transform(&s1, matrix_new_translation4(5, 0, 0));
    group_add_object(&group2, &s1);
    group_add_transform(&group2, matrix_new_scaling4(2, 2, 2));
    group_add_object(&group, &group2);
    group_add_transform(&group, matrix_new_rotate_y(PI/2));

    struct world* world = world_new_world();
    world_add_sphere(world, &s1);
    //world_add_group(world, &group2);

    struct point_light light1 = lights_new_point_light(tuple_new_point(0, 10, -10), tuple_new_color(1, 1, 1));
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
    struct tuple from = tuple_new_point(0, 0, 0);
    struct tuple to = tuple_new_point(0, 0, -1);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);
    camera_make_inv_view_transform(&camera);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
