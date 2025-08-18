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

    pat.default_transformation = matrix_new_scaling4(2, 2, 2);
    sphere_set_transform(&s, matrix_make_identity4());
    assert(tuple_equals(white, pattern_stripe_at_object(pat, &s, tuple_new_point(1.5, 0, 0))));

    pat.default_transformation = matrix_new_translation4(0.5, 0, 0);
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

    // Floor
    struct plane floor = plane_new_plane();
    floor.material.color = tuple_new_color(76./255, 86./255, 106./255);
    floor.material.ambient = 0.1;
    floor.material.specular = 0;
    floor.material.diffuse = 0.55;
    floor.default_transformation = matrix_new_rotate_x(-PI/2);
    floor.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, 5), floor.default_transformation);
    //floor.material.pattern = pattern_stripe(tuple_new_color(1, 1, 1), tuple_new_color(76./255, 86./255, 106./255));
    //floor.material.pattern = pattern_ring(tuple_new_color(1, 1, 1), tuple_new_color(76./255, 86./255, 106./255));
    floor.material.pattern = pattern_checker(tuple_new_color(1, 1, 1), tuple_new_color(76./255, 86./255, 106./255));
    //floor.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);

    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s1.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 1, 3), s1.default_transformation);
    s1.default_transformation = matrix_mult_matrix4(matrix_new_scaling4(3, 3, 3), s1.default_transformation);
    s1.material.color = tuple_new_color(235./255, 203./255, 139./255);
    s1.material.diffuse = 1;
    s1.material.ambient = 0.1;
    s1.material.specular = 0.1;
    s1.material.shininess = 5;
    //s1.material.pattern = pattern_stripe(tuple_new_color(1, 1, 1), tuple_new_color(76./255, 86./255, 106./255));
    //s1.material.pattern = pattern_gradient(tuple_new_color(1, 1, 1), tuple_new_color(76./255, 86./255, 106./255));
    s1.material.pattern = pattern_ring(tuple_new_color(1, 1, 1), tuple_new_color(76./255, 86./255, 106./255));
    //s1.material.pattern.default_transformation = matrix_new_scaling4(2, 2, 2);
    //s1.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1, 0, 0), s1.material.pattern.default_transformation);
    s1.material.pattern.default_transformation = matrix_new_scaling4(0.1, 0.1, 0.1);
    s1.material.pattern.default_transformation = matrix_mult_matrix4(matrix_new_rotate_x(PI/2), s1.material.pattern.default_transformation);

    /*
    // Backwall
    struct plane back_wall = plane_new_plane();
    back_wall.default_transformation = matrix_new_rotate_x(-PI/2);
    back_wall.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, 5), back_wall.default_transformation);
    back_wall.material.color = tuple_new_color(76./255, 86./255, 106./255);
    back_wall.material.ambient = 0.1;
    back_wall.material.specular = 0;
    back_wall.material.diffuse = 0.55;

    // Ceiling
    struct plane ceiling = plane_new_plane();
    ceiling.default_transformation = matrix_new_translation4(0, 4, 0);
    ceiling.material.color = tuple_new_color(76./255, 86./255, 106./255);
    ceiling.material.ambient = 0.6;
    ceiling.material.specular = 0;
    ceiling.material.diffuse = 0.9;

    // Leftwall
    struct plane left_wall = plane_new_plane();
    left_wall.default_transformation = matrix_new_rotate_z(-PI/2);
    left_wall.default_transformation = matrix_mult_matrix4(matrix_new_translation4(-5, 0, 0), left_wall.default_transformation);
    left_wall.material.color = tuple_new_color(76./255, 86./255, 106./255);
    left_wall.material.ambient = 0.1;
    left_wall.material.specular = 0;
    left_wall.material.diffuse = 0.55;

    // Rightwall
    struct plane right_wall = plane_new_plane();
    right_wall.default_transformation = matrix_new_rotate_z(PI/2);
    right_wall.default_transformation = matrix_mult_matrix4(matrix_new_translation4(5, 0, 0), right_wall.default_transformation);
    right_wall.material.color = tuple_new_color(76./255, 86./255, 106./255);
    right_wall.material.ambient = 0.1;
    right_wall.material.specular = 0;
    right_wall.material.diffuse = 0.55;


    // Spheres
    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s1.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 1, 3), s1.default_transformation);
    s1.material.color = tuple_new_color(235./255, 203./255, 139./255);
    s1.material.diffuse = 1;
    s1.material.ambient = 0.1;
    s1.material.specular = 0.1;
    s1.material.shininess = 5;

    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s2.default_transformation = matrix_new_scaling4(0.5, 0.5, 0.5);
    s2.default_transformation = matrix_mult_matrix4(matrix_new_translation4(-1.5, 0.5, 1), s2.default_transformation);
    s2.material.color = tuple_new_color(191./255, 97./255, 106./255);
    s2.material.diffuse = 1;
    s2.material.ambient = 0.1;
    s2.material.specular = 0.1;
    s2.material.shininess = 5;

    struct sphere s3 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s3.default_transformation = matrix_new_scaling4(0.75, 0.75, 0.75);
    s3.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1.75, 0.75, 1), s3.default_transformation);
    s3.material.color = tuple_new_color(208./255, 135./255, 112./255);
    s3.material.diffuse = 1;
    s3.material.ambient = 0.1;
    s3.material.specular = 0.1;
    s3.material.shininess = 5;

    struct sphere s4 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s4.default_transformation = matrix_new_scaling4(0.35, 0.35, 0.35);
    s4.default_transformation = matrix_mult_matrix4(matrix_new_translation4(1.25, 0.35, -3), s4.default_transformation);
    s4.material.color = tuple_new_color(163./255, 190./255, 140./255);
    s4.material.diffuse = 1;
    s4.material.ambient = 0.1;
    s4.material.specular = 0.1;
    s4.material.shininess = 10;

    struct sphere s5 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s5.default_transformation = matrix_new_scaling4(0.25, 0.25, 0.25);
    s5.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0.25, -1), s5.default_transformation);
    s5.material.color = tuple_new_color(180./255, 142./255, 173./255);
    s5.material.diffuse = 1;
    s5.material.ambient = 0.1;
    s5.material.specular = 0.5;
    s5.material.shininess = 10;

    // Right sphere
    struct sphere s5 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    transform = matrix_new_scaling4(0.5, 0.5, 0.5);
    transform = matrix_mult_matrix4(matrix_new_translation4(1.5, 0.5, -0.5), transform);
    sphere_set_transform(&s5, transform);
    sphere_set_material(&s5, materials_new_material());
    s5.material.color = tuple_new_color(208./255, 135./255, 112./255);
    s5.material.diffuse = 0.7;
    s5.material.specular = 0.8;

    // Left sphere
    struct sphere s6 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    transform = matrix_new_scaling4(0.33, 0.33, 0.33);
    transform = matrix_mult_matrix4(matrix_new_translation4(-1.5, 0.33, -0.75), transform);
    sphere_set_transform(&s6, transform);
    sphere_set_material(&s6, materials_new_material());
    s6.material.color = tuple_new_color(180./255, 142./255, 173./255);
    s6.material.diffuse = 0.7;
    s6.material.specular = 0.5;
    s6.material.shininess = 300;
    */

    // World
    struct world* world = world_new_world();
    world_add_plane(world, &floor);
    //world_add_plane(world, &back_wall);
    //world_add_plane(world, &ceiling);
    //world_add_plane(world, &left_wall);
    //world_add_plane(world, &right_wall);
    world_add_sphere(world, &s1);
    //world_add_sphere(world, &s2);
    //world_add_sphere(world, &s3);
    //world_add_sphere(world, &s4);
    //world_add_sphere(world, &s5);

    struct point_light light1 = lights_new_point_light(tuple_new_point(0, 0, -3), tuple_new_color(1, 1, 1));
    //struct point_light light2 = lights_new_point_light(tuple_new_point(0, 2.05, 0), tuple_new_color(0.1, 0.1, 0.1));
    //struct point_light light3 = lights_new_point_light(tuple_new_point(0, 3.79, -5), tuple_new_color(1, 1, 1));
    //struct point_light light4 = lights_new_point_light(tuple_new_point(0.01, 3.8, -5), tuple_new_color(1, 1, 1));
    //struct point_light light5 = lights_new_point_light(tuple_new_point(-0.01, 3.8, -5), tuple_new_color(1, 1, 1));
    world_add_point_light(world, &light1);
    //world_add_point_light(world, &light2);
    //world_add_point_light(world, &light3);
    //world_add_point_light(world, &light4);
    //world_add_point_light(world, &light5);


    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/3);
    struct tuple from = tuple_new_point(0, 0, -3);
    struct tuple to = tuple_new_point(0, 0, 0);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
