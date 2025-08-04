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
#include "camera.h"

static void chap7_render(void);

int main(int argc, char *argv[]) {

    struct world* w = world_new_world();
    assert(world_get_nb_objects(w) == 0);
    assert(world_get_nb_lights(w) == 0);
    assert(world_get_max_nb_lights(w) == 1);
    assert(world_get_max_nb_objects(w) == 1);
    world_free_world(w);

    w = world_new_world();
    struct point_light light = lights_new_point_light(tuple_new_point(-10, 10, -10),
	    tuple_new_color(1, 1, 1));
    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct material mat = materials_new_material();
    mat.color = tuple_new_color(0.8, 1., 0.6);
    mat.diffuse = 0.7;
    mat.specular = 0.2;
    sphere_set_material(&s1, mat);
    world_add_sphere(w, &s1);
    world_add_point_light(w, &light);
    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct matrix4 mscale = matrix_new_scaling4(0.5, 0.5, 0.5);
    sphere_set_transform(&s2, mscale);
    world_add_sphere(w, &s2);

    struct sphere* s1_ret = (struct sphere*) world_get_object_at(w, 0);
    struct sphere* s2_ret = (struct sphere*) world_get_object_at(w, 1);
    struct point_light* light_ret = (struct point_light*) world_get_light_at(w, 0);
    assert(s1_ret == &s1);
    assert(s2_ret == &s2);
    assert(light_ret == &light);

    struct ray r = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    struct intersection_list intersect_world = world_intersect_world(w, &r);
    assert(intersect_world.nb_intersections == 4);
    assert(ctm_floats_equal(intersect_world.list[0].t, 4));
    assert(ctm_floats_equal(intersect_world.list[1].t, 4.5));
    assert(ctm_floats_equal(intersect_world.list[2].t, 5.5));
    assert(ctm_floats_equal(intersect_world.list[3].t, 6));
    intersection_clear_intersection_list(&intersect_world);

    r = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    struct sphere s3 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct intersection inter = intersection_new_intersection(4, &s3);
    struct precompute comps = intersection_prepare_computations(&inter, &r);
    assert(ctm_floats_equal(comps.t, 4));
    assert(comps.object == inter.object);
    assert(tuple_equals(comps.point, tuple_new_point(0, 0, -1)));
    assert(tuple_equals(comps.eyev, tuple_new_vector(0, 0, -1)));
    assert(tuple_equals(comps.normalv, tuple_new_vector(0, 0, -1)));
    assert(!comps.inside);

    r = ray_new_ray(tuple_new_point(0, 0, 0), tuple_new_vector(0, 0, 1));
    inter = intersection_new_intersection(1, &s3);
    comps = intersection_prepare_computations(&inter, &r);
    assert(tuple_equals(comps.point, tuple_new_point(0, 0, 1)));
    assert(tuple_equals(comps.eyev, tuple_new_vector(0, 0, -1)));
    assert(tuple_equals(comps.normalv, tuple_new_vector(0, 0, -1)));
    assert(comps.inside);
    
    r = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    void* first_shape = world_get_object_at(w, 0);
    inter = intersection_new_intersection(4, first_shape);
    comps = intersection_prepare_computations(&inter, &r);
    struct tuple c = world_shade_hit(w, &comps);
    assert(tuple_equals(c, tuple_new_color(0.38066, 0.47583, 0.2855)));
    world_free_world(w);

    w = world_new_world();
    struct point_light new_light = lights_new_point_light(tuple_new_point(0, 0.25, 0),
	    tuple_new_color(1, 1, 1));
    world_add_sphere(w, &s1);
    world_add_sphere(w, &s2);
    world_add_point_light(w, &new_light);
    r = ray_new_ray(tuple_new_point(0, 0, 0), tuple_new_vector(0, 0, 1));
    void* second_shape = world_get_object_at(w, 1);
    inter = intersection_new_intersection(0.5, second_shape);
    comps = intersection_prepare_computations(&inter, &r);
    c = world_shade_hit(w, &comps);
    assert(tuple_equals(c, tuple_new_color(0.90498, 0.90498, 0.90498)));
    world_free_world(w);

    w = world_new_world();
    world_add_sphere(w, &s1);
    world_add_sphere(w, &s2);
    world_add_point_light(w, &light);
    r = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 1, 0));
    c = world_color_at(w, &r);
    assert(tuple_equals(c, tuple_new_color(0, 0, 0)));

    r = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    c = world_color_at(w, &r);
    assert(tuple_equals(c, tuple_new_color(0.38066, 0.47583, 0.2855)));
    world_free_world(w);

    w = world_new_world();
    s1.material.ambient = 1;
    s2.material.ambient = 1;
    world_add_sphere(w, &s1);
    world_add_sphere(w, &s2);
    world_add_point_light(w, &light);
    r = ray_new_ray(tuple_new_point(0, 0, 0.75), tuple_new_vector(0, 0, -1));
    c = world_color_at(w, &r);
    assert(tuple_equals(c, s2.material.color));
    world_free_world(w);

    struct tuple from = tuple_new_point(0, 0, 0);
    struct tuple to = tuple_new_point(0, 0, -1);
    struct tuple up = tuple_new_vector(0, 1, 0);
    struct matrix4 t = matrix_view_transform(from, to, up);
    assert(matrix_compare_matrix4(t, matrix_make_identity4()));

    to = tuple_new_point(0, 0, 1);
    t = matrix_view_transform(from, to, up);
    assert(matrix_compare_matrix4(t, matrix_new_scaling4(-1, 1, -1)));

    from = tuple_new_point(0, 0, 8);
    to = tuple_new_point(0, 0, 0);
    t = matrix_view_transform(from, to, up);
    assert(matrix_compare_matrix4(t, matrix_new_translation4(0, 0, -8)));

    from = tuple_new_point(1, 3, 2);
    to = tuple_new_point(4, -2, 8);
    up = tuple_new_vector(1, 1, 0);
    t = matrix_view_transform(from, to, up);
    struct row4 r1 = matrix_new_row4(-0.50709, 0.50709, 0.67612, -2.36643);
    struct row4 r2 = matrix_new_row4(0.76772, 0.60609, 0.12122, -2.82843);
    struct row4 r3 = matrix_new_row4(-0.35857, 0.59761, -0.71714, 0.00000);
    struct row4 r4 = matrix_new_row4(0.00000, 0.00000, 0.00000, 1.00000);
    struct matrix4 t_exp = matrix_new_matrix4(r1, r2, r3, r4);
    assert(matrix_compare_matrix4(t, t_exp));
    
    struct camera cam = camera_new_camera(160, 120, PI/2);
    assert(cam.hsize == 160);
    assert(cam.vsize == 120);
    assert(ctm_floats_equal(cam.fov, PI/2));
    assert(matrix_compare_matrix4(cam.transform, matrix_make_identity4()));

    cam = camera_new_camera(200, 125, PI/2);
    assert(ctm_floats_equal(cam.pixel_size, 0.01));
    cam = camera_new_camera(125, 200, PI/2);
    assert(ctm_floats_equal(cam.pixel_size, 0.01));

    cam = camera_new_camera(201, 101, PI/2);
    r = camera_ray_for_pixel(&cam, 50, 100); // 100, 50 for author is 50, 100 for me
    assert(tuple_equals(r.o, tuple_new_point(0, 0, 0)));
    assert(tuple_equals(r.dir, tuple_new_vector(0, 0, -1)));

    r = camera_ray_for_pixel(&cam, 0, 0);
    assert(tuple_equals(r.o, tuple_new_point(0, 0, 0)));
    assert(tuple_equals(r.dir, tuple_new_vector(0.66519, 0.33259, -0.66851)));

    cam.transform = matrix_mult_matrix4(matrix_new_rotate_y(PI/4), matrix_new_translation4(0, -2, 5));
    r = camera_ray_for_pixel(&cam, 50, 100);
    assert(tuple_equals(r.o, tuple_new_point(0, 2, -5)));
    assert(tuple_equals(r.dir, tuple_new_vector(sqrt(2)/2, 0, -sqrt(2)/2))); // 100, 50 for author is 50, 100 for me

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
    sphere_set_transform(&s2, mscale);
    world_add_sphere(w, &s2);

    cam = camera_new_camera(11, 11, PI/2);
    from = tuple_new_point(0, 0, -5);
    to = tuple_new_point(0, 0, 0);
    up = tuple_new_vector(0, 1, 0);
    cam.transform = matrix_view_transform(from, to, up);
    struct canvas canvas = camera_render(&cam, w);
    assert(tuple_equals(canvas.grid[INDEX(5, 5, canvas.width)],
		tuple_new_color(0.38066, 0.47583, 0.2855)));

    canvas_free_canvas(&canvas);
    world_free_world(w);

    chap7_render();

    return EXIT_SUCCESS;
}

static void chap7_render(void) {

    // Floor
    struct sphere s1 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct matrix4 transform = matrix_new_translation4(-2.5, -2.5, 10);
    transform = matrix_new_scaling4(10, 0.01, 10);
    sphere_set_transform(&s1, transform);
    sphere_set_material(&s1, materials_new_material());
    s1.material.color = tuple_new_color(76./255, 86./255, 106./255);
    s1.material.specular = 0;

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

    // World
    struct world* world = world_new_world();
    world_add_sphere(world, &s1);
    world_add_sphere(world, &s2);
    world_add_sphere(world, &s3);
    world_add_sphere(world, &s4);
    world_add_sphere(world, &s5);
    world_add_sphere(world, &s6);
    struct point_light light1 = lights_new_point_light(tuple_new_point(-10, 10, -10), tuple_new_color(1, 1, 1));
    struct point_light light2 = lights_new_point_light(tuple_new_point(3, 10, 0), tuple_new_color(1, 1, 1));
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
