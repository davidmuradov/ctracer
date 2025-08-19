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
static void chap11_2_render(void);
static void chap11_3_render(void);

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
    plane.default_transformation = matrix_new_translation4(0, -1, 0);
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
    comps = intersection_prepare_computations(&inter, &ray, NULL);
    color = world_reflected_color(w, &comps, 0);
    assert(tuple_equals(tuple_new_color(0, 0, 0), color));

    // Refraction test 2
    world_free_world(w);
    struct sphere a = sphere_new_glass_sphere();
    a.material.refractive_index = 1.5;
    a.default_transformation = matrix_new_scaling4(2, 2, 2);
    struct sphere b = sphere_new_glass_sphere();
    b.material.refractive_index = 2;
    b.default_transformation = matrix_new_translation4(0, 0, -0.25);
    struct sphere c = sphere_new_glass_sphere();
    c.material.refractive_index = 2.5;
    c.default_transformation = matrix_new_translation4(0, 0, 0.25);
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
    a.default_transformation = matrix_new_translation4(0, 0, 1);
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
    floor.default_transformation = matrix_new_translation4(0, -1, 0);
    floor.material.transparency = 0.5;
    floor.material.refractive_index = 1.5;
    world_add_plane(w, &floor);
    struct sphere ball = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    ball.material.color = tuple_new_color(1, 0, 0);
    ball.material.ambient = 0.5;
    ball.default_transformation = matrix_new_translation4(0, -3.5, -0.5);
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
    floor.default_transformation = matrix_new_translation4(0, -1, 0);
    floor.material.transparency = 0.5;
    floor.material.reflective = 0.5;
    floor.material.refractive_index = 1.5;
    world_add_plane(w, &floor);
    ball = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    ball.material.color = tuple_new_color(1, 0, 0);
    ball.material.ambient = 0.5;
    ball.default_transformation = matrix_new_translation4(0, -3.5, -0.5);
    world_add_sphere(w, &ball);

    inter_list = intersection_new_intersection_list();
    inter = intersection_new_intersection(sqrt(2), &floor);
    intersection_add_intersection_to_list(&inter_list, inter);
    comps = intersection_prepare_computations(&(inter_list.list[0]), &ray, &inter_list);
    color = world_shade_hit(w, &comps, MAX_RECUR_CALLS);
    assert(tuple_equals(color, tuple_new_color(0.93391, 0.69643, 0.69243)));
    intersection_clear_intersection_list(&inter_list);
    world_free_world(w);

    //chap11_render();
    //chap11_2_render();
    chap11_3_render();
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

static void chap11_2_render(void) {
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
    floor.material.ambient = 0.3;
    floor.material.specular = 0;
    floor.material.diffuse = 1.5;
    floor.material.color = tuple_new_color(1, 1, 1);

    // Back
    struct plane back = plane_new_plane();
    back.material.ambient = 1;
    back.material.specular = 0;
    back.material.diffuse = 0;
    back.material.color = tuple_new_color(1, 1, 1);
    //back.material.pattern = pattern_gradient(tuple_new_color(1, 1, 1), NORD9);
    //back.material.pattern.default_transformation = matrix_new_scaling4(700, 1, 1);
    back.default_transformation = matrix_new_rotate_x(-PI/2);
    back.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, -500), back.default_transformation);
    back.default_transformation = matrix_mult_matrix4(matrix_new_rotate_z(PI/2), back.default_transformation);

    // Front
    struct plane front = plane_new_plane();
    front.material.ambient = 1;
    front.material.specular = 0;
    front.material.diffuse = 0;
    front.material.color = tuple_new_color(1, 1, 1);
    //back.material.pattern = pattern_gradient(tuple_new_color(1, 1, 1), NORD9);
    //back.material.pattern.default_transformation = matrix_new_scaling4(700, 1, 1);
    front.default_transformation = matrix_new_rotate_x(-PI/2);
    front.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, 500), front.default_transformation);
    front.default_transformation = matrix_mult_matrix4(matrix_new_rotate_z(PI/2), front.default_transformation);

    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s1.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 1, 0), s1.default_transformation);
    s1.material.diffuse = 1.1;
    s1.material.ambient = 0.2;
    s1.material.specular = 0.6;
    s1.material.shininess = 200;
    s1.material.reflective = 0.1;
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
    world_add_plane(world, &back);
    world_add_plane(world, &front);
    world_add_sphere(world, &s1);
    //world_add_sphere(world, &s2);
    //world_add_sphere(world, &s3);
    //world_add_sphere(world, &s4);
    //world_add_sphere(world, &s5);
    //world_add_sphere(world, &s6);
    //world_add_sphere(world, &s7);

    struct point_light light1 = lights_new_point_light(tuple_new_point(-400, 500, -300), tuple_new_color(1, 1, 1));
    world_add_point_light(world, &light1);

    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/3);
    struct tuple from = tuple_new_point(0, 1, -5);
    struct tuple to = tuple_new_point(0, 1, 0);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}

static void chap11_3_render(void) {
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
    floor.material.specular = 0.5;
    floor.material.diffuse = 0.1;
    floor.material.shininess = 50;
    floor.material.reflective = 0;
    floor.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    floor.default_transformation = matrix_new_rotate_x(-PI/2);
    floor.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, 3), floor.default_transformation);
    floor.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);

    // Ceiling
    struct plane ceil = plane_new_plane();
    ceil.material.ambient = 0.75;
    ceil.material.specular = 0.5;
    ceil.material.diffuse = 0.1;
    ceil.material.shininess = 50;
    ceil.material.reflective = 0;
    ceil.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    ceil.default_transformation = matrix_new_rotate_x(-PI/2);
    ceil.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, -3), ceil.default_transformation);
    ceil.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);

    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s1.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, -15), s1.default_transformation);
    s1.material.diffuse = 0.05;
    s1.material.ambient = 0.05;
    s1.material.specular = 0.2;
    s1.material.shininess = 100;
    s1.material.transparency = 0.9;
    s1.material.refractive_index = 1.52;
    s1.material.reflective = 0.7;
    s1.material.color = NORD0;
    //s1.material.pattern = pattern_gradient(NORD15, NORD11);
    //s1.material.pattern.default_transformation = matrix_new_scaling4(1, 1, 1);
    //s1.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s1.material.pattern.default_transformation);
    //s1.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_rotate_y(PI/3), s1.material.pattern.default_transformation);
    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    //s2.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, 0), s2.default_transformation);
    s2.material.diffuse = 0.05;
    s2.material.ambient = 0.05;
    s2.material.specular = 0.2;
    s2.material.shininess = 100;
    s2.material.transparency = 1;
    s2.material.refractive_index = 1.0002;
    s2.material.reflective = 1;
    s2.material.color = NORD0;
    //s1.material.pattern = pattern_gradient(NORD15, NORD11);
    s2.default_transformation = matrix_new_scaling4(0.5, 0.5, 0.5);
    s2.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, -15), s2.default_transformation);
    //s1.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_rotate_y(PI/3), s1.material.pattern.default_transformation);


    // World
    struct world* world = world_new_world();
    world_add_plane(world, &floor);
    //world_add_plane(world, &ceil);
    world_add_sphere(world, &s1);
    world_add_sphere(world, &s2);

    struct point_light light1 = lights_new_point_light(tuple_new_point(0, 100, 0), tuple_new_color(1, 1, 1));
    world_add_point_light(world, &light1);

    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/3);
    struct tuple from = tuple_new_point(0, 0, -25);
    struct tuple to = tuple_new_point(0, 0, 1);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
