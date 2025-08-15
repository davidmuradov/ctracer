#include <assert.h>
#include <math.h>
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

static void chap9_render(void);

int main(int argc, char *argv[]) {

    struct world* w = world_new_world();
    struct sphere s = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct plane p = plane_new_plane();
    struct cube c = cube_new_cube();
    struct cylinder cyl = cylinder_new_cylinder();

    world_add_sphere(w, &s);
    world_add_plane(w, &p);
    world_add_cube(w, &c);
    world_add_cylinder(w, &cyl);

    assert(world_get_object_type(world_get_object_at(w, 0)) == SPHERE);
    assert(world_get_object_type(world_get_object_at(w, 1)) == PLANE);
    assert(world_get_object_type(world_get_object_at(w, 2)) == CUBE);
    assert(world_get_object_type(world_get_object_at(w, 3)) == CYLINDER);

    struct sphere* s_ptr = (struct sphere*) world_get_object_at(w, 0);
    struct plane* p_ptr = (struct plane*) world_get_object_at(w, 1);
    struct cube* c_ptr = (struct cube*) world_get_object_at(w, 2);
    struct cylinder* cyl_ptr = (struct cylinder*) world_get_object_at(w, 3);
    assert(matrix_compare_matrix4(matrix_make_identity4(), s_ptr->default_transformation));
    assert(matrix_compare_matrix4(matrix_make_identity4(), p_ptr->default_transformation));
    assert(matrix_compare_matrix4(matrix_make_identity4(), c_ptr->default_transformation));
    assert(matrix_compare_matrix4(matrix_make_identity4(), cyl_ptr->default_transformation));

    world_free_world(w);

    w = world_new_world();
    s.default_transformation = matrix_new_translation4(2, 3, 4);
    p.default_transformation = matrix_new_translation4(2, 3, 4);
    c.default_transformation = matrix_new_translation4(2, 3, 4);
    cyl.default_transformation = matrix_new_translation4(2, 3, 4);

    world_add_sphere(w, &s);
    world_add_plane(w, &p);
    world_add_cube(w, &c);
    world_add_cylinder(w, &cyl);

    s_ptr = (struct sphere*) world_get_object_at(w, 0);
    p_ptr = (struct plane*) world_get_object_at(w, 1);
    c_ptr = (struct cube*) world_get_object_at(w, 2);
    cyl_ptr = (struct cylinder*) world_get_object_at(w, 3);

    assert(matrix_compare_matrix4(matrix_new_translation4(2, 3, 4), s_ptr->default_transformation));
    assert(matrix_compare_matrix4(matrix_new_translation4(2, 3, 4), p_ptr->default_transformation));
    assert(matrix_compare_matrix4(matrix_new_translation4(2, 3, 4), c_ptr->default_transformation));
    assert(matrix_compare_matrix4(matrix_new_translation4(2, 3, 4), cyl_ptr->default_transformation));

    assert(tuple_equals(tuple_new_color(1, 1, 1), s_ptr->material.color) &&
	    ctm_floats_equal(0.1, s_ptr->material.ambient) &&
	    ctm_floats_equal(0.9, s_ptr->material.diffuse) &&
	    ctm_floats_equal(0.9, s_ptr->material.specular) &&
	    ctm_floats_equal(200, s_ptr->material.shininess));
    assert(tuple_equals(tuple_new_color(1, 1, 1), p_ptr->material.color) &&
	    ctm_floats_equal(0.1, p_ptr->material.ambient) &&
	    ctm_floats_equal(0.9, p_ptr->material.diffuse) &&
	    ctm_floats_equal(0.9, p_ptr->material.specular) &&
	    ctm_floats_equal(200, p_ptr->material.shininess));
    assert(tuple_equals(tuple_new_color(1, 1, 1), c_ptr->material.color) &&
	    ctm_floats_equal(0.1, c_ptr->material.ambient) &&
	    ctm_floats_equal(0.9, c_ptr->material.diffuse) &&
	    ctm_floats_equal(0.9, c_ptr->material.specular) &&
	    ctm_floats_equal(200, c_ptr->material.shininess));
    assert(tuple_equals(tuple_new_color(1, 1, 1), cyl_ptr->material.color) &&
	    ctm_floats_equal(0.1, cyl_ptr->material.ambient) &&
	    ctm_floats_equal(0.9, cyl_ptr->material.diffuse) &&
	    ctm_floats_equal(0.9, cyl_ptr->material.specular) &&
	    ctm_floats_equal(200, cyl_ptr->material.shininess));

    world_free_world(w);

    w = world_new_world();
    p.material.ambient = 1;

    world_add_sphere(w, &s);
    world_add_plane(w, &p);
    world_add_cube(w, &c);
    world_add_cylinder(w, &cyl);

    s_ptr = (struct sphere*) world_get_object_at(w, 0);
    p_ptr = (struct plane*) world_get_object_at(w, 1);
    c_ptr = (struct cube*) world_get_object_at(w, 2);
    cyl_ptr = (struct cylinder*) world_get_object_at(w, 3);

    assert(ctm_floats_equal(1, p_ptr->material.ambient));

    world_free_world(w);

    struct plane plane = plane_new_plane();
    assert(tuple_equals(tuple_new_vector(0, 1, 0), plane_normal_at(&plane, tuple_new_point(0, 0, 0))));
    assert(tuple_equals(tuple_new_vector(0, 1, 0), plane_normal_at(&plane, tuple_new_point(10, 0, -10))));
    assert(tuple_equals(tuple_new_vector(0, 1, 0), plane_normal_at(&plane, tuple_new_point(-5, 0, 150))));
    assert(tuple_equals(tuple_new_vector(0, 0, 0), plane_normal_at(&plane, tuple_new_point(-5, 1, 150))));
    plane.default_transformation = matrix_new_rotate_x(PI/2);
    assert(tuple_equals(tuple_new_vector(0, 0, 1), plane_normal_at(&plane, tuple_new_point(0, 0, 0))));
    assert(tuple_equals(tuple_new_vector(0, 0, 1), plane_normal_at(&plane, tuple_new_point(10, -10, 0))));
    assert(tuple_equals(tuple_new_vector(0, 0, 1), plane_normal_at(&plane, tuple_new_point(-5, 150, 0))));
    assert(tuple_equals(tuple_new_vector(0, 0, 0), plane_normal_at(&plane, tuple_new_point(-5, 150, 0.01))));
    plane.default_transformation = matrix_new_rotate_x(-PI/2);
    assert(tuple_equals(tuple_new_vector(0, 0, -1), plane_normal_at(&plane, tuple_new_point(0, 0, 0))));
    assert(tuple_equals(tuple_new_vector(0, 0, -1), plane_normal_at(&plane, tuple_new_point(10, -10, 0))));
    assert(tuple_equals(tuple_new_vector(0, 0, -1), plane_normal_at(&plane, tuple_new_point(-5, 150, 0))));
    assert(tuple_equals(tuple_new_vector(0, 0, 0), plane_normal_at(&plane, tuple_new_point(-5, 150, 0.01))));
    plane.default_transformation = matrix_mult_matrix4(matrix_new_translation4(0, 0, 1), plane.default_transformation);
    assert(tuple_equals(tuple_new_vector(0, 0, -1), plane_normal_at(&plane, tuple_new_point(0, 0, 1))));
    assert(tuple_equals(tuple_new_vector(0, 0, -1), plane_normal_at(&plane, tuple_new_point(10, -10, 1))));
    assert(tuple_equals(tuple_new_vector(0, 0, -1), plane_normal_at(&plane, tuple_new_point(-5, 150, 1))));
    assert(tuple_equals(tuple_new_vector(0, 0, 0), plane_normal_at(&plane, tuple_new_point(-5, 150, 0.01))));
    plane.default_transformation = matrix_make_identity4();

    //chap9_render();
    return 0;
}

static void chap9_render(void) {

    // Floor
    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct matrix4 transform = matrix_new_translation4(-2.5, -2.5, 10);
    transform = matrix_new_scaling4(10, 0.01, 10);
    sphere_set_transform(&s1, transform);
    sphere_set_material(&s1, materials_new_material());
    s1.material.color = tuple_new_color(76./255, 86./255, 106./255);
    s1.material.specular = 0;
    //s1.material.diffuse = 0.5;

    // Left wall
    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s2.material = s1.material;
    transform = matrix_new_scaling4(10, 0.01, 10);
    transform = matrix_mult_matrix4(matrix_new_rotate_x(PI/2), transform);
    transform = matrix_mult_matrix4(matrix_new_rotate_y(-PI/4), transform);
    transform = matrix_mult_matrix4(matrix_new_translation4(0, 0, 5), transform);
    sphere_set_transform(&s2, transform);

    // Right wall
    struct sphere s3 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    s3.material = s1.material;
    transform = matrix_new_scaling4(10, 0.01, 10);
    transform = matrix_mult_matrix4(matrix_new_rotate_x(PI/2), transform);
    transform = matrix_mult_matrix4(matrix_new_rotate_y(PI/4), transform);
    transform = matrix_mult_matrix4(matrix_new_translation4(0, 0, 5), transform);
    sphere_set_transform(&s3, transform);

    // Middle sphere
    struct sphere s4 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    transform = matrix_new_translation4(-0.5, 1, 0.5);
    sphere_set_transform(&s4, transform);
    sphere_set_material(&s4, materials_new_material());
    s4.material.color = tuple_new_color(163./255, 190./255, 140./255);
    s4.material.diffuse = 0.7;
    s4.material.specular = 0.3;

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

    // World
    struct world* world = world_new_world();
    world_add_sphere(world, &s1);
    world_add_sphere(world, &s2);
    world_add_sphere(world, &s3);
    world_add_sphere(world, &s4);
    world_add_sphere(world, &s5);
    world_add_sphere(world, &s6);
    struct point_light light1 = lights_new_point_light(tuple_new_point(-10, 10, -10), tuple_new_color(1, 1, 1));
    struct point_light light2 = lights_new_point_light(tuple_new_point(0.5, 4, -3), tuple_new_color(1, 1, 1));
    world_add_point_light(world, &light1);
    world_add_point_light(world, &light2);


    // Camera and render
    struct camera camera = camera_new_camera(1920, 1080, PI/3);
    struct tuple from = tuple_new_point(0, 1.5, -5);
    struct tuple to = tuple_new_point(0, 1, 0);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
