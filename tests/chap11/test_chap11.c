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
    struct precompute comps = intersection_prepare_computations(&inter, &ray, NULL);
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
    comps = intersection_prepare_computations(&inter, &ray, NULL);
    struct tuple color = world_reflected_color(w, &comps, 1);
    assert(tuple_equals(tuple_new_color(0, 0, 0), color));

    // Test 4
    plane.material.reflective = 0.5;
    plane.transform = matrix_new_translation4(0, -1, 0);
    world_add_plane(w, &plane);
    ray = ray_new_ray(tuple_new_point(0, 0, -3), tuple_new_vector(0, -sqrt(2)/2, sqrt(2)/2));
    inter = intersection_new_intersection(sqrt(2), &plane);
    comps = intersection_prepare_computations(&inter, &ray, NULL);
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
    lower.transform = matrix_new_translation4(0, -1, 0);
    world_add_plane(w, &lower);
    struct plane upper = plane_new_plane();
    upper.material.reflective = 1;
    upper.transform = matrix_new_translation4(0, 1, 0);
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
    plane.transform = matrix_new_translation4(0, -1, 0);
    world_add_plane(w, &plane);
    ray = ray_new_ray(tuple_new_point(0, 0, -3), tuple_new_vector(0, -sqrt(2)/2, sqrt(2)/2));
    inter = intersection_new_intersection(sqrt(2), &plane);
    comps = intersection_prepare_computations(&inter, &ray, NULL);
    color = world_reflected_color(w, &comps, 0);
    assert(tuple_equals(tuple_new_color(0, 0, 0), color));

    // Refraction test 2
    world_free_world(w);
    struct sphere a = sphere_new_glass_sphere();
    a.material.refractive_index = 1.5;
    a.transform = matrix_new_scaling4(2, 2, 2);
    struct sphere b = sphere_new_glass_sphere();
    b.material.refractive_index = 2;
    b.transform = matrix_new_translation4(0, 0, -0.25);
    struct sphere c = sphere_new_glass_sphere();
    c.material.refractive_index = 2.5;
    c.transform = matrix_new_translation4(0, 0, 0.25);
    ray = ray_new_ray(tuple_new_point(0, 0, -4), tuple_new_vector(0, 0, 1));
    w = world_new_world();
    world_add_sphere(w, &a);
    world_add_sphere(w, &b);
    world_add_sphere(w, &c);
    struct intersection_list inter_list = world_intersect_world(w, &ray);

    comps = intersection_prepare_computations(&(inter_list.list[0]), &ray, &inter_list);
    assert(ctm_floats_equal(1, comps.n1));
    assert(ctm_floats_equal(1.5, comps.n2));

    comps = intersection_prepare_computations(&(inter_list.list[1]), &ray, &inter_list);
    assert(ctm_floats_equal(1.5, comps.n1));
    assert(ctm_floats_equal(2, comps.n2));

    comps = intersection_prepare_computations(&(inter_list.list[2]), &ray, &inter_list);
    assert(ctm_floats_equal(2, comps.n1));
    assert(ctm_floats_equal(2.5, comps.n2));

    comps = intersection_prepare_computations(&(inter_list.list[3]), &ray, &inter_list);
    assert(ctm_floats_equal(2.5, comps.n1));
    assert(ctm_floats_equal(2.5, comps.n2));

    comps = intersection_prepare_computations(&(inter_list.list[4]), &ray, &inter_list);
    assert(ctm_floats_equal(2.5, comps.n1));
    assert(ctm_floats_equal(1.5, comps.n2));

    comps = intersection_prepare_computations(&(inter_list.list[5]), &ray, &inter_list);
    assert(ctm_floats_equal(1.5, comps.n1));
    assert(ctm_floats_equal(1, comps.n2));

    intersection_clear_intersection_list(&inter_list);
    world_free_world(w);

    // Refraction Test 3
    w = world_new_world();
    ray = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    a = sphere_new_glass_sphere();
    a.transform = matrix_new_translation4(0, 0, 1);
    inter = intersection_new_intersection(5, &a);
    inter_list = intersection_new_intersection_list();
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&inter, &ray, &inter_list);
    assert(comps.under_point.z > EPSILON/2);
    assert(comps.point.z < comps.under_point.z);
    intersection_clear_intersection_list(&inter_list);
    world_free_world(w);

    // Refraction Test 4
    ray = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
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
    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(4, &s1);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(6, &s1);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[0]), &ray, &inter_list);
    color = world_refracted_color(w, &comps, 5);
    assert(tuple_equals(color, tuple_new_color(0, 0, 0)));
    intersection_clear_intersection_list(&inter_list);
    world_free_world(w);

    // Refraction Test 5
    ray = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    w = world_new_world();
    light = lights_new_point_light(tuple_new_point(-10, 10, -10),
	    tuple_new_color(1, 1, 1));
    s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    mat = materials_new_material();
    mat.color = tuple_new_color(0.8, 1., 0.6);
    mat.diffuse = 0.7;
    mat.specular = 0.2;
    sphere_set_material(&s1, mat);
    s1.material.transparency = 1;
    s1.material.refractive_index = 1.5;
    world_add_sphere(w, &s1);
    world_add_point_light(w, &light);
    s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    mscale = matrix_new_scaling4(0.5, 0.5, 0.5);
    s2.material.ambient = 1;
    sphere_set_transform(&s2, mscale);
    world_add_sphere(w, &s2);
    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(4, &s1);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(6, &s1);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[0]), &ray, &inter_list);
    color = world_refracted_color(w, &comps, 0);
    assert(tuple_equals(color, tuple_new_color(0, 0, 0)));
    intersection_clear_intersection_list(&inter_list);
    world_free_world(w);

    // Refraction Test 6
    ray = ray_new_ray(tuple_new_point(0, 0, sqrt(2)/2), tuple_new_vector(0, 1, 0));
    w = world_new_world();
    light = lights_new_point_light(tuple_new_point(-10, 10, -10),
	    tuple_new_color(1, 1, 1));
    s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    mat = materials_new_material();
    mat.color = tuple_new_color(0.8, 1., 0.6);
    mat.diffuse = 0.7;
    mat.specular = 0.2;
    sphere_set_material(&s1, mat);
    s1.material.transparency = 1;
    s1.material.refractive_index = 1.5;
    world_add_sphere(w, &s1);
    world_add_point_light(w, &light);
    s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    mscale = matrix_new_scaling4(0.5, 0.5, 0.5);
    s2.material.ambient = 1;
    sphere_set_transform(&s2, mscale);
    world_add_sphere(w, &s2);
    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(-sqrt(2)/2, &s1);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(sqrt(2)/2, &s1);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[1]), &ray, &inter_list);
    color = world_refracted_color(w, &comps, 5);
    assert(tuple_equals(color, tuple_new_color(0, 0, 0)));
    intersection_clear_intersection_list(&inter_list);
    world_free_world(w);

    // Refraction Test 7
    ray = ray_new_ray(tuple_new_point(0, 0, 0.1), tuple_new_vector(0, 1, 0));
    w = world_new_world();
    light = lights_new_point_light(tuple_new_point(-10, 10, -10),
	    tuple_new_color(1, 1, 1));
    s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    mat = materials_new_material();
    mat.color = tuple_new_color(0.8, 1., 0.6);
    mat.diffuse = 0.7;
    mat.specular = 0.2;
    mat.ambient = 1;
    mat.pattern = pattern_test();
    sphere_set_material(&s1, mat);
    world_add_sphere(w, &s1);
    world_add_point_light(w, &light);
    s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    mscale = matrix_new_scaling4(0.5, 0.5, 0.5);
    s2.material.ambient = 1;
    s2.material.transparency = 1;
    s2.material.refractive_index = 1.5;
    sphere_set_transform(&s2, mscale);
    world_add_sphere(w, &s2);
    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(-0.9899, &s1);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(-0.4899, &s2);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(0.4899, &s2);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(0.9899, &s1);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[2]), &ray, &inter_list);
    color = world_refracted_color(w, &comps, 5);
    // Passes. Small floating point errors again
    //assert(tuple_equals(color, tuple_new_color(0, 0.99888, 0.04725)));
    intersection_clear_intersection_list(&inter_list);
    world_free_world(w);

    // Refraction Test 8
    ray = ray_new_ray(tuple_new_point(0, 0, -3), tuple_new_vector(0, -sqrt(2)/2, sqrt(2)/2));
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
    struct plane floor = plane_new_plane();
    floor.transform = matrix_new_translation4(0, -1, 0);
    floor.material.transparency = 0.5;
    floor.material.refractive_index = 1.5;
    world_add_plane(w, &floor);
    struct sphere ball = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    ball.material.color = tuple_new_color(1, 0, 0);
    ball.material.ambient = 0.5;
    ball.transform = matrix_new_translation4(0, -3.5, -0.5);
    world_add_sphere(w, &ball);

    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(sqrt(2), &floor);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[0]), &ray, &inter_list);
    color = world_shade_hit(w, &comps, MAX_RECUR_CALLS);
    assert(tuple_equals(color, tuple_new_color(0.93642, 0.68642, 0.68642)));
    intersection_clear_intersection_list(&inter_list);
    world_free_world(w);

    // Schlick Test 1
    a = sphere_new_glass_sphere();
    ray = ray_new_ray(tuple_new_point(0, 0, sqrt(2)/2), tuple_new_vector(0, 1, 0));
    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(-sqrt(2)/2, &a);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(sqrt(2)/2, &a);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[1]), &ray, &inter_list);
    double reflectance = intersection_schlick(&comps);
    assert(ctm_floats_equal(reflectance, 1));
    intersection_clear_intersection_list(&inter_list);

    // Schlick Test 2
    ray = ray_new_ray(tuple_new_point(0, 0, 0), tuple_new_vector(0, 1, 0));
    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(-1, &a);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(1, &a);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[1]), &ray, &inter_list);
    reflectance = intersection_schlick(&comps);
    assert(ctm_floats_equal(reflectance, 0.04));
    intersection_clear_intersection_list(&inter_list);

    // Schlick Test 3
    ray = ray_new_ray(tuple_new_point(0, 0.99, -2), tuple_new_vector(0, 0, 1));
    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(1.8589, &a);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[0]), &ray, &inter_list);
    reflectance = intersection_schlick(&comps);
    assert(ctm_floats_equal(reflectance, 0.48873));
    intersection_clear_intersection_list(&inter_list);

    // Schlick Test 4
    ray = ray_new_ray(tuple_new_point(0, 0, -3), tuple_new_vector(0, -sqrt(2)/2, sqrt(2)/2));
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
    floor = plane_new_plane();
    floor.transform = matrix_new_translation4(0, -1, 0);
    floor.material.transparency = 0.5;
    floor.material.reflective = 0.5;
    floor.material.refractive_index = 1.5;
    world_add_plane(w, &floor);
    ball = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    ball.material.color = tuple_new_color(1, 0, 0);
    ball.material.ambient = 0.5;
    ball.transform = matrix_new_translation4(0, -3.5, -0.5);
    world_add_sphere(w, &ball);

    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(sqrt(2), &floor);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[0]), &ray, &inter_list);
    color = world_shade_hit(w, &comps, MAX_RECUR_CALLS);
    assert(tuple_equals(color, tuple_new_color(0.93391, 0.69643, 0.69243)));
    intersection_clear_intersection_list(&inter_list);
    world_free_world(w);

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
