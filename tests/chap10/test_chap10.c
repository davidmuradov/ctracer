#include <assert.h>
#include <stdio.h>
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

static void chap10_render(void);

int main(int argc, char *argv[]) {

    const struct tuple black = tuple_new_color(0, 0, 0);
    const struct tuple white = tuple_new_color(1, 1, 1);

    struct pattern pat = pattern_stripe(white, black);
    assert(tuple_equals(pat.color_a, white));
    assert(tuple_equals(pat.color_b, black));

    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(0, 0, 0)), white));
    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(0, 1, 0)), white));
    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(0, 2, 0)), white));

    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(0, 0, 1)), white));
    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(0, 0, 2)), white));

    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(0.9, 0, 0)), white));
    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(1, 0, 0)), black));
    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(-0.1, 0, 0)), black));
    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(-1, 0, 0)), black));
    assert(tuple_equals(pattern_stripe_at(pat, tuple_new_point(-1.1, 0, 0)), white));

    struct material m = materials_new_material();
    m.pattern = pattern_stripe(white, black);
    m.ambient = 1;
    m.diffuse = 0;
    m.specular = 0;
    struct tuple eyev = tuple_new_vector(0, 0, -1);
    struct tuple normalv = tuple_new_vector(0, 0, -1);
    struct point_light light = lights_new_point_light(tuple_new_point(0, 0, -10), tuple_new_color(1, 1, 1));
    struct sphere temp_sphere = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct tuple c1 = lights_lighting_sphere(m, &temp_sphere, light, tuple_new_point(0.9, 0, 0), eyev, normalv, 0);
    struct tuple c2 = lights_lighting_sphere(m, &temp_sphere, light, tuple_new_point(1.1, 0, 0), eyev, normalv, 0);
    assert(tuple_equals(c1, white));
    assert(tuple_equals(c2, black));

    struct sphere s = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    sphere_set_transform(&s, matrix_new_scaling4(2, 2, 2));
    assert(tuple_equals(white, pattern_stripe_at_object(pat, &s, tuple_new_point(1.5, 0, 0))));

    pat.transform = matrix_new_scaling4(2, 2, 2);
    sphere_set_transform(&s, matrix_make_identity4());
    assert(tuple_equals(white, pattern_stripe_at_object(pat, &s, tuple_new_point(1.5, 0, 0))));

    pat.transform = matrix_new_translation4(0.5, 0, 0);
    sphere_set_transform(&s, matrix_new_scaling4(2, 2, 2));
    assert(tuple_equals(white, pattern_stripe_at_object(pat, &s, tuple_new_point(2.5, 0, 0))));

    pat = pattern_gradient(white, black);
    assert(tuple_equals(pattern_gradient_at(pat, tuple_new_point(0, 0, 0)), white));
    assert(tuple_equals(pattern_gradient_at(pat, tuple_new_point(0.25, 0, 0)), tuple_new_color(0.75, 0.75, 0.75)));
    assert(tuple_equals(pattern_gradient_at(pat, tuple_new_point(0.5, 0, 0)), tuple_new_color(0.5, 0.5, 0.5)));
    assert(tuple_equals(pattern_gradient_at(pat, tuple_new_point(0.75, 0, 0)), tuple_new_color(0.25, 0.25, 0.25)));

    pat = pattern_ring(white, black);
    assert(tuple_equals(pattern_ring_at(pat, tuple_new_point(0, 0, 0)), white));
    assert(tuple_equals(pattern_ring_at(pat, tuple_new_point(1, 0, 0)), black));
    assert(tuple_equals(pattern_ring_at(pat, tuple_new_point(0, 0, 1)), black));
    assert(tuple_equals(pattern_ring_at(pat, tuple_new_point(0.708, 0, 0.708)), black));

    pat = pattern_checker(white, black);
    assert(tuple_equals(pattern_checker_at(pat, tuple_new_point(0, 0, 0)), white));
    assert(tuple_equals(pattern_checker_at(pat, tuple_new_point(0.99, 0, 0)), white));
    assert(tuple_equals(pattern_checker_at(pat, tuple_new_point(1.01, 0, 0)), black));

    assert(tuple_equals(pattern_checker_at(pat, tuple_new_point(0, 0.99, 0)), white));
    assert(tuple_equals(pattern_checker_at(pat, tuple_new_point(0, 1.01, 0)), black));

    assert(tuple_equals(pattern_checker_at(pat, tuple_new_point(0, 0, 0.99)), white));
    assert(tuple_equals(pattern_checker_at(pat, tuple_new_point(0, 0, 1.01)), black));

    chap10_render();

    return 0;
}

static void chap10_render(void) {

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
    floor.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), NORD3);
    floor.material.pattern.transform = matrix_new_scaling4(2, 2, 2);

    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s1.transform = matrix_mult_matrix4(matrix_new_translation4(1, 1, 1), s1.transform);
    s1.material.diffuse = 0.7;
    s1.material.ambient = 0.75;
    s1.material.specular = 0.6;
    s1.material.shininess = 100;
    s1.material.pattern = pattern_gradient(NORD15, NORD11);
    s1.material.pattern.transform = matrix_new_scaling4(2, 2, 2);
    s1.material.pattern.transform = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s1.material.pattern.transform);
    s1.material.pattern.transform = matrix_mult_matrix4(matrix_new_rotate_y(PI/3), s1.material.pattern.transform);

    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s2.transform = matrix_mult_matrix4(matrix_new_translation4(-1, 1, -3), s2.transform);
    s2.material.diffuse = 0.7;
    s2.material.ambient = 0.75;
    s2.material.specular = 0.6;
    s2.material.shininess = 100;
    s2.material.pattern = pattern_gradient(NORD10, NORD7);
    s2.material.pattern.transform = matrix_new_scaling4(2, 2, 2);
    s2.material.pattern.transform = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s2.material.pattern.transform);
    s2.material.pattern.transform = matrix_mult_matrix4(matrix_new_rotate_y(PI/3), s2.material.pattern.transform);

    struct sphere s3 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s3.transform = matrix_mult_matrix4(matrix_new_translation4(3, 1, -1), s3.transform);
    s3.material.diffuse = 0.7;
    s3.material.ambient = 0.75;
    s3.material.specular = 0.6;
    s3.material.shininess = 100;
    s3.material.pattern = pattern_gradient(NORD13, NORD12);
    s3.material.pattern.transform = matrix_new_scaling4(2, 2, 2);
    s3.material.pattern.transform = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s3.material.pattern.transform);
    s3.material.pattern.transform = matrix_mult_matrix4(matrix_new_rotate_y(PI/3), s3.material.pattern.transform);

    struct sphere s4 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s4.transform = matrix_mult_matrix4(matrix_new_translation4(-5, 1, 3), s4.transform);
    s4.material.diffuse = 0.7;
    s4.material.ambient = 0.75;
    s4.material.specular = 0.6;
    s4.material.shininess = 100;
    s4.material.pattern = pattern_gradient(NORD9, NORD12);
    s4.material.pattern.transform = matrix_new_scaling4(2, 2, 2);
    s4.material.pattern.transform = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s4.material.pattern.transform);

    struct sphere s5 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s5.transform = matrix_mult_matrix4(matrix_new_translation4(-1, 1, 5), s5.transform);
    s5.material.diffuse = 0.7;
    s5.material.ambient = 0.75;
    s5.material.specular = 0.6;
    s5.material.shininess = 100;
    s5.material.pattern = pattern_gradient(NORD15, NORD14);
    s5.material.pattern.transform = matrix_new_scaling4(2, 2, 2);
    s5.material.pattern.transform = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s5.material.pattern.transform);

    struct sphere s6 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s6.transform = matrix_mult_matrix4(matrix_new_translation4(3, 1, 5), s6.transform);
    s6.material.diffuse = 0.7;
    s6.material.ambient = 0.75;
    s6.material.specular = 0.6;
    s6.material.shininess = 100;
    s6.material.pattern = pattern_gradient(NORD8, NORD11);
    s6.material.pattern.transform = matrix_new_scaling4(2, 2, 2);
    s6.material.pattern.transform = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s6.material.pattern.transform);
    s6.material.pattern.transform = matrix_mult_matrix4(matrix_new_rotate_y(PI/5), s6.material.pattern.transform);

    struct sphere s7 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s7.transform = matrix_mult_matrix4(matrix_new_translation4(-3, 1, 1), s7.transform);
    s7.material.diffuse = 0.7;
    s7.material.ambient = 0.75;
    s7.material.specular = 0.6;
    s7.material.shininess = 100;
    s7.material.pattern = pattern_gradient(NORD0, NORD2);
    s7.material.pattern.transform = matrix_new_scaling4(2, 2, 2);
    s7.material.pattern.transform = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s7.material.pattern.transform);
    s7.material.pattern.transform = matrix_mult_matrix4(matrix_new_rotate_y(-PI/3), s7.material.pattern.transform);

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
