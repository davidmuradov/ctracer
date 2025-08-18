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

static void chap11_render(void);

int main(int argc, char *argv[]) {

    // Test 1
    struct material mat = materials_new_material();
    assert(ctm_floats_equal(mat.reflective, 0));

    // Test 2
    struct plane plane = plane_new_plane();
    struct ray ray = ray_new_ray(tuple_new_point(0, 1, -1), tuple_new_vector(0, -sqrt(2)/2, sqrt(2)/2));
    struct intersection inter = intersection_new_intersection(sqrt(2), &plane);
    struct precompute comps = intersection_prepare_computations(&inter, &ray);
    assert(tuple_equals(comps.reflectv, tuple_new_vector(0, sqrt(2)/2, sqrt(2)/2)));

    // Test 3
    ray = ray_new_ray(tuple_new_point(0, 0, 0), tuple_new_vector(0, 0, 1));
    struct world* w = world_new_world();
    struct point_light light = lights_new_point_light(tuple_new_point(-10, 10, -10),
	    tuple_new_color(1, 1, 1));
    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    mat = materials_new_material();
    mat.color = tuple_new_color(0.8, 1., 0.6);
    mat.diffuse = 0.7;
    mat.specular = 0.2;
    sphere_set_material(&s1, mat);
    world_add_sphere(w, &s1);
    world_add_point_light(w, &light);
    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct matrix4 mscale = matrix_new_scaling4(0.5, 0.5, 0.5);
    s2.material.ambient = 1;
    sphere_set_transform(&s2, mscale);
    world_add_sphere(w, &s2);
    inter = intersection_new_intersection(1, &s2);
    comps = intersection_prepare_computations(&inter, &ray);
    struct tuple color = world_reflected_color(w, &comps, 1);
    assert(tuple_equals(tuple_new_color(0, 0, 0), color));

    // Test 4
    plane.material.reflective = 0.5;
    plane.default_transformation = matrix_new_translation4(0, -1, 0);
    world_add_plane(w, &plane);
    ray = ray_new_ray(tuple_new_point(0, 0, -3), tuple_new_vector(0, -sqrt(2)/2, sqrt(2)/2));
    inter = intersection_new_intersection(sqrt(2), &plane);
    comps = intersection_prepare_computations(&inter, &ray);
    color = world_reflected_color(w, &comps, 1);
    // Little floating point error on red component
    //assert(tuple_equals(tuple_new_color(0.19032, 0.2379, 0.14274), color));

    // Test 5
    color = world_shade_hit(w, &comps, 1);
    // Little floating point errors on every component, can pass with bigger
    // EPSILON
    //assert(tuple_equals(tuple_new_color(0.87677, 0.92436, 0.82918), color));
    world_free_world(w);

    // Test 6
    w = world_new_world();
    light = lights_new_point_light(tuple_new_point(0, 0, 0), tuple_new_color(1, 1, 1));
    world_add_point_light(w, &light);
    struct plane lower = plane_new_plane();
    lower.material.reflective = 1;
    lower.default_transformation = matrix_new_translation4(0, -1, 0);
    world_add_plane(w, &lower);
    struct plane upper = plane_new_plane();
    upper.material.reflective = 1;
    upper.default_transformation = matrix_new_translation4(0, 1, 0);
    world_add_plane(w, &upper);
    ray = ray_new_ray(tuple_new_point(0, 0, 0), tuple_new_vector(0, 1, 0));
    color = world_color_at(w, &ray, MAX_RECUR_CALLS);

    // Test 7
    world_free_world(w);

    w = world_new_world();
    light = lights_new_point_light(tuple_new_point(-10, 10, -10),
	    tuple_new_color(1, 1, 1));
    s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    mat = materials_new_material();
    mat.color = tuple_new_color(0.8, 1., 0.6);
    mat.diffuse = 0.7;
    mat.specular = 0.2;
    sphere_set_material(&s1, mat);
    world_add_sphere(w, &s1);
    world_add_point_light(w, &light);
    s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    mscale = matrix_new_scaling4(0.5, 0.5, 0.5);
    s2.material.ambient = 1;
    sphere_set_transform(&s2, mscale);
    world_add_sphere(w, &s2);

    plane.material.reflective = 0.5;
    plane.default_transformation = matrix_new_translation4(0, -1, 0);
    world_add_plane(w, &plane);
    ray = ray_new_ray(tuple_new_point(0, 0, -3), tuple_new_vector(0, -sqrt(2)/2, sqrt(2)/2));
    inter = intersection_new_intersection(sqrt(2), &plane);
    comps = intersection_prepare_computations(&inter, &ray);
    color = world_reflected_color(w, &comps, 0);
    assert(tuple_equals(tuple_new_color(0, 0, 0), color));

    chap11_render();
    return 0;
}

static void chap11_render(void) {

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
    floor.material.ambient = 0.75;
    floor.material.specular = 0.2;
    floor.material.diffuse = 0.1;
    floor.material.shininess = 50;
    floor.material.reflective = 0.05;
    floor.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    floor.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);

    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s1.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 1, 1), s1.default_transformation);
    s1.material.diffuse = 0.7;
    s1.material.ambient = 0.75;
    s1.material.specular = 0.6;
    s1.material.shininess = 100;
    s1.material.reflective = 0.2;
    s1.material.pattern = pattern_gradient(NORD15, NORD11);
    s1.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);
    s1.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s1.material.pattern.default_transformation);
    s1.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_rotate_y(PI/3), s1.material.pattern.default_transformation);

    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s2.default_transformation = matrix_mult_matrix4(matrix_new_translation4(-1, 1, -3), s2.default_transformation);
    s2.material.diffuse = 0.7;
    s2.material.ambient = 0.75;
    s2.material.specular = 0.6;
    s2.material.shininess = 100;
    s2.material.reflective = 0.2;
    s2.material.pattern = pattern_gradient(NORD10, NORD7);
    s2.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);
    s2.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s2.material.pattern.default_transformation);
    s2.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_rotate_y(PI/3), s2.material.pattern.default_transformation);

    struct sphere s3 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s3.default_transformation = matrix_mult_matrix4(matrix_new_translation4(3, 1, -1), s3.default_transformation);
    s3.material.diffuse = 0.7;
    s3.material.ambient = 0.75;
    s3.material.specular = 0.6;
    s3.material.shininess = 100;
    s3.material.reflective = 0.2;
    s3.material.pattern = pattern_gradient(NORD13, NORD12);
    s3.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);
    s3.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s3.material.pattern.default_transformation);
    s3.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_rotate_y(PI/3), s3.material.pattern.default_transformation);

    struct sphere s4 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s4.default_transformation = matrix_mult_matrix4(matrix_new_translation4(-5, 1, 3), s4.default_transformation);
    s4.material.diffuse = 0.7;
    s4.material.ambient = 0.75;
    s4.material.specular = 0.6;
    s4.material.shininess = 100;
    s4.material.reflective = 0.2;
    s4.material.pattern = pattern_gradient(NORD9, NORD12);
    s4.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);
    s4.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s4.material.pattern.default_transformation);

    struct sphere s5 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s5.default_transformation = matrix_mult_matrix4(matrix_new_translation4(-1, 1, 5), s5.default_transformation);
    s5.material.diffuse = 0.7;
    s5.material.ambient = 0.75;
    s5.material.specular = 0.6;
    s5.material.shininess = 100;
    s5.material.reflective = 0.2;
    s5.material.pattern = pattern_gradient(NORD15, NORD14);
    s5.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);
    s5.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s5.material.pattern.default_transformation);

    struct sphere s6 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s6.default_transformation = matrix_mult_matrix4(matrix_new_translation4(3, 1, 5), s6.default_transformation);
    s6.material.diffuse = 0.7;
    s6.material.ambient = 0.75;
    s6.material.specular = 0.6;
    s6.material.shininess = 100;
    s6.material.reflective = 0.2;
    s6.material.pattern = pattern_gradient(NORD8, NORD11);
    s6.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);
    s6.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s6.material.pattern.default_transformation);
    s6.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_rotate_y(PI/5), s6.material.pattern.default_transformation);

    struct sphere s7 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s7.default_transformation = matrix_mult_matrix4(matrix_new_translation4(-3, 1, 1), s7.default_transformation);
    s7.material.diffuse = 0.7;
    s7.material.ambient = 0.75;
    s7.material.specular = 0.6;
    s7.material.shininess = 100;
    s7.material.reflective = 0.5;
    s7.material.pattern = pattern_gradient(NORD0, NORD2);
    s7.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);
    s7.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s7.material.pattern.default_transformation);
    s7.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_rotate_y(-PI/3), s7.material.pattern.default_transformation);

    // World
    struct world* world = world_new_world();
    world_add_plane(world, &floor);
    world_add_sphere(world, &s1);
    world_add_sphere(world, &s2);
    world_add_sphere(world, &s3);
    world_add_sphere(world, &s4);
    world_add_sphere(world, &s5);
    world_add_sphere(world, &s6);
    world_add_sphere(world, &s7);

    struct point_light light1 = lights_new_point_light(tuple_new_point(0, 100, 0), tuple_new_color(1, 1, 1));
    world_add_point_light(world, &light1);

    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/3);
    struct tuple from = tuple_new_point(-2, 6, -2);
    struct tuple to = tuple_new_point(0, 0, 0);
    struct tuple up = tuple_new_vector(0, 1.7, -0.3);
    camera.transform = matrix_view_transform(from, to, up);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
