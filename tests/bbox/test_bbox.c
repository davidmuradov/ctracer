#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
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
#include "../../includes/cone.h"
#include "../../includes/group.h"
#include "../../includes/bounding_box.h"

static void bbox_render_1(void);
static void bbox_render_2(void);
static void bbox_render_3(void);

int main(int argc, char *argv[]) {

    struct bbox* box = bbox_new_empty_bbox();
    // Passes
    //assert(tuple_equals(box->min, tuple_new_point(INFINITY, INFINITY, INFINITY)));
    //assert(tuple_equals(box->max, tuple_new_point(-INFINITY, -INFINITY, -INFINITY)));
    bbox_delete_box(box);

    box = bbox_new_bbox(tuple_new_point(-1, -2, -3), tuple_new_point(3, 2, 1));
    assert(tuple_equals(box->min, tuple_new_point(-1, -2, -3)));
    assert(tuple_equals(box->max, tuple_new_point(3, 2, 1)));
    bbox_delete_box(box);

    box = bbox_new_empty_bbox();
    bbox_add_point(box, tuple_new_point(-5, 2, 0));
    bbox_add_point(box, tuple_new_point(7, 0, -3));
    assert(tuple_equals(box->min, tuple_new_point(-5, 0, -3)));
    assert(tuple_equals(box->max, tuple_new_point(7, 2, 0)));
    bbox_delete_box(box);

    struct sphere* s = sphere_new_sphere();
    box = bbox_bounds_of_shape(s);
    assert(tuple_equals(box->min, tuple_new_point(-1, -1, -1)));
    assert(tuple_equals(box->max, tuple_new_point(1, 1, 1)));
    sphere_delete_sphere(s);
    bbox_delete_box(box);

    struct plane* p = plane_new_plane();
    box = bbox_bounds_of_shape(p);
    //assert(tuple_equals(box->min, tuple_new_point(-INFINITY, 0, -INFINITY)));
    //assert(tuple_equals(box->max, tuple_new_point(INFINITY, 0, INFINITY)));
    plane_delete_plane(p);
    bbox_delete_box(box);

    struct cube* c = cube_new_cube();
    box = bbox_bounds_of_shape(c);
    assert(tuple_equals(box->min, tuple_new_point(-1, -1, -1)));
    assert(tuple_equals(box->max, tuple_new_point(1, 1, 1)));
    cube_delete_cube(c);
    bbox_delete_box(box);

    struct cylinder* cyl = cylinder_new_cylinder();
    box = bbox_bounds_of_shape(cyl);
    //assert(tuple_equals(box->min, tuple_new_point(-1, -INFINITY, -1)));
    //assert(tuple_equals(box->max, tuple_new_point(1, INFINITY, 1)));
    bbox_delete_box(box);
    cyl->min = -5;
    cyl->max = 3;
    box = bbox_bounds_of_shape(cyl);
    assert(tuple_equals(box->min, tuple_new_point(-1, -5, -1)));
    assert(tuple_equals(box->max, tuple_new_point(1, 3, 1)));
    cylinder_delete_cylinder(cyl);
    bbox_delete_box(box);

    struct cone* co = cone_new_cone();
    box = bbox_bounds_of_shape(co);
    //assert(tuple_equals(box->min, tuple_new_point(-1, -INFINITY, -1)));
    //assert(tuple_equals(box->max, tuple_new_point(1, INFINITY, 1)));
    bbox_delete_box(box);
    co->min = -5;
    co->max = 3;
    box = bbox_bounds_of_shape(co);
    assert(tuple_equals(box->min, tuple_new_point(-5, -5, -5)));
    assert(tuple_equals(box->max, tuple_new_point(5, 3, 5)));
    cone_delete_cone(co);
    bbox_delete_box(box);

    struct bbox* box2 = bbox_new_bbox(tuple_new_point(8, -7, -2), tuple_new_point(14, 2, 8));
    box = bbox_new_bbox(tuple_new_point(-5, -2, 0), tuple_new_point(7, 4, 4));
    bbox_add_bbox(box, box2);
    assert(tuple_equals(box->min, tuple_new_point(-5, -7, -2)));
    assert(tuple_equals(box->max, tuple_new_point(14, 4, 8)));
    bbox_delete_box(box);
    bbox_delete_box(box2);

    box = bbox_new_bbox(tuple_new_point(5, -2, 0), tuple_new_point(11, 4, 7));
    struct tuple point = tuple_new_point(5, -2, 0);
    assert(bbox_contains_point(box, point));
    point = tuple_new_point(11, 4, 7);
    assert(bbox_contains_point(box, point));
    point = tuple_new_point(8, 1, 3);
    assert(bbox_contains_point(box, point));
    point = tuple_new_point(3, 0, 3);
    assert(!bbox_contains_point(box, point));
    point = tuple_new_point(8, -4, 3);
    assert(!bbox_contains_point(box, point));
    point = tuple_new_point(8, 1, -1);
    assert(!bbox_contains_point(box, point));
    point = tuple_new_point(13, 1, 3);
    assert(!bbox_contains_point(box, point));
    point = tuple_new_point(8, 5, 3);
    assert(!bbox_contains_point(box, point));
    point = tuple_new_point(8, 1, 8);
    assert(!bbox_contains_point(box, point));

    box2 = bbox_new_bbox(tuple_new_point(5, -2, 0), tuple_new_point(11, 4, 7));
    assert(bbox_contains_bbox(box, box2));
    bbox_delete_box(box2);
    box2 = bbox_new_bbox(tuple_new_point(6, -1, 1), tuple_new_point(10, 3, 6));
    assert(bbox_contains_bbox(box, box2));
    bbox_delete_box(box2);
    box2 = bbox_new_bbox(tuple_new_point(4, -3, -1), tuple_new_point(10, 3, 6));
    assert(!bbox_contains_bbox(box, box2));
    bbox_delete_box(box2);
    box2 = bbox_new_bbox(tuple_new_point(6, -1, 1), tuple_new_point(12, 5, 8));
    assert(!bbox_contains_bbox(box, box2));
    bbox_delete_box(box2);
    bbox_delete_box(box);

    box = bbox_new_bbox(tuple_new_point(-1, -1, -1), tuple_new_point(1, 1, 1));
    struct matrix4 m = matrix_mult_matrix4(matrix_new_rotate_x(PI/4), matrix_new_rotate_y(PI/4));
    box2 = bbox_transform(box, m);
    // Passes
    //assert(tuple_equals(box->min, tuple_new_point(-1.4142, -1.7071, -1.7071)));
    //assert(tuple_equals(box->max, tuple_new_point(1.4142, 1.7071, 1.7071)));
    bbox_delete_box(box);
    bbox_delete_box(box2);

    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_mult_matrix4(matrix_new_translation4(1, -3, 5), matrix_new_scaling4(0.5, 2, 4)));
    box = bbox_bounds_of_shape(s);
    assert(tuple_equals(box->min, tuple_new_point(0.5, -5, 1)));
    assert(tuple_equals(box->max, tuple_new_point(1.5, -1, 9)));
    bbox_delete_box(box);
    sphere_delete_sphere(s);

    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_mult_matrix4(matrix_new_translation4(2, 5, -3), matrix_new_scaling4(2, 2, 2)));
    cyl = cylinder_new_cylinder();
    cyl->min = -2;
    cyl->max = 2;
    cylinder_add_transform(cyl, matrix_mult_matrix4(matrix_new_translation4(-4, -1, 4), matrix_new_scaling4(0.5, 1, 0.5)));
    struct group* group = group_new_group();
    group_add_object(group, s);
    group_add_object(group, cyl);
    group_make_bounding_box(group);
    //box = bbox_bounds_of_shape(group);
    assert(tuple_equals(group->box->min, tuple_new_point(-4.5, -3, -5)));
    assert(tuple_equals(group->box->max, tuple_new_point(4, 7, 4.5)));
    //sphere_delete_sphere(s);
    //cylinder_delete_cylinder(cyl);
    //bbox_delete_box(box);
    group_delete_group(group);

    // Double group test
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_scaling4(2, 2, 2));
    struct group* group2 = group_new_group();
    group_add_object(group2, s);
    group_add_transform(group2, matrix_new_translation4(5, 0, 0));
    group = group_new_group();
    group_add_object(group, group2);
    group_add_transform(group, matrix_new_rotate_y(PI/2));
    group_make_bounding_box(group);
    assert(tuple_equals(group->box->min, tuple_new_point(-2, -2, -7)));
    assert(tuple_equals(group->box->max, tuple_new_point(2, 2, -3)));
    //sphere_delete_sphere(s);
    //bbox_delete_box(box);
    group_delete_group(group);
    //group_delete_group(group2);

    // Another double group test
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_scaling4(2, 2, 2));
    group2 = group_new_group();
    group_add_object(group2, s);
    group_add_transform(group2, matrix_new_translation4(5, 0, 0));
    group = group_new_group();
    group_add_object(group, group2);
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_translation4(8, 0, 0));
    group_add_object(group, s);
    group_add_transform(group, matrix_new_rotate_y(PI/2));
    group_make_bounding_box(group);
    assert(tuple_equals(group->box->min, tuple_new_point(-2, -2, -9)));
    assert(tuple_equals(group->box->max, tuple_new_point(2, 2, -3)));
    group2 = group->list_children[0];
    assert(tuple_equals(group2->box->min, tuple_new_point(-2, -2, -7)));
    assert(tuple_equals(group2->box->max, tuple_new_point(2, 2, -3)));
    group_delete_group(group);

    // Bounding boxes intersection tests
    box = bbox_new_bbox(tuple_new_point(-1, -1, -1), tuple_new_point(1, 1, 1));
    struct tuple origin = tuple_new_point(5, 0.5, 0);
    struct tuple direction = tuple_normalize(tuple_new_vector(-1, 0, 0));
    struct ray ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(-5, 0.5, 0);
    direction = tuple_normalize(tuple_new_vector(1, 0, 0));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(0.5, 5, 0);
    direction = tuple_normalize(tuple_new_vector(0, -1, 0));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(0.5, -5, 0);
    direction = tuple_normalize(tuple_new_vector(0, 1, 0));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(0.5, 0, 5);
    direction = tuple_normalize(tuple_new_vector(0, 0, -1));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(0.5, 0, -5);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(0, 0.5, 0);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(-2, 0, 0);
    direction = tuple_normalize(tuple_new_vector(2, 4, 6));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(0, -2, 0);
    direction = tuple_normalize(tuple_new_vector(6, 2, 4));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(0, 0, -2);
    direction = tuple_normalize(tuple_new_vector(4, 6, 2));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(2, 0, 2);
    direction = tuple_normalize(tuple_new_vector(0, 0, -1));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(0, 2, 2);
    direction = tuple_normalize(tuple_new_vector(0, -1, 0));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(2, 2, 0);
    direction = tuple_normalize(tuple_new_vector(-1, 0, 0));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));

    // Bounding boxes intersection tests (2)
    bbox_delete_box(box);
    box = bbox_new_bbox(tuple_new_point(5, -2, 0), tuple_new_point(11, 4, 7));
    origin = tuple_new_point(15, 1, 2);
    direction = tuple_normalize(tuple_new_vector(-1, 0, 0));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(-5, -1, 4);
    direction = tuple_normalize(tuple_new_vector(1, 0, 0));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(7, 6, 5);
    direction = tuple_normalize(tuple_new_vector(0, -1, 0));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(9, -5, 6);
    direction = tuple_normalize(tuple_new_vector(0, 1, 0));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(8, 2, 12);
    direction = tuple_normalize(tuple_new_vector(0, 0, -1));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(6, 0, -5);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(8, 1, 3.5);
    direction = tuple_normalize(tuple_new_vector(0, 0, 1));
    ray = ray_new_ray(origin, direction);
    assert(bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(9, -1, -8);
    direction = tuple_normalize(tuple_new_vector(2, 4, 6));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(8, 3, -4);
    direction = tuple_normalize(tuple_new_vector(6, 2, 4));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(9, -1, -2);
    direction = tuple_normalize(tuple_new_vector(4, 6, 2));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(4, 0, 9);
    direction = tuple_normalize(tuple_new_vector(0, 0, -1));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(8, 6, -1);
    direction = tuple_normalize(tuple_new_vector(0, -1, 0));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    origin = tuple_new_point(12, 5, 4);
    direction = tuple_normalize(tuple_new_vector(-1, 0, 0));
    ray = ray_new_ray(origin, direction);
    assert(!bbox_intersect_ray(box, &ray));
    bbox_delete_box(box);

    // Bounding boxes split bounds test
    struct bbox* arr[2] = {0};
    box = bbox_new_bbox(tuple_new_point(-1, -4, -5), tuple_new_point(9, 6, 5));
    bbox_split_bounds(box, arr);
    assert(tuple_equals(tuple_new_point(-1, -4, -5), arr[0]->min));
    assert(tuple_equals(tuple_new_point(4, 6, 5), arr[0]->max));
    assert(tuple_equals(tuple_new_point(4, -4, -5), arr[1]->min));
    assert(tuple_equals(tuple_new_point(9, 6, 5), arr[1]->max));
    bbox_delete_box(box);
    bbox_delete_box(arr[0]);
    bbox_delete_box(arr[1]);

    // x wide
    box = bbox_new_bbox(tuple_new_point(-1, -2, -3), tuple_new_point(9, 5.5, 3));
    bbox_split_bounds(box, arr);
    assert(tuple_equals(tuple_new_point(-1, -2, -3), arr[0]->min));
    assert(tuple_equals(tuple_new_point(4, 5.5, 3), arr[0]->max));
    assert(tuple_equals(tuple_new_point(4, -2, -3), arr[1]->min));
    assert(tuple_equals(tuple_new_point(9, 5.5, 3), arr[1]->max));
    bbox_delete_box(box);
    bbox_delete_box(arr[0]);
    bbox_delete_box(arr[1]);

    // y wide
    box = bbox_new_bbox(tuple_new_point(-1, -2, -3), tuple_new_point(5, 8, 3));
    bbox_split_bounds(box, arr);
    assert(tuple_equals(tuple_new_point(-1, -2, -3), arr[0]->min));
    assert(tuple_equals(tuple_new_point(5, 3, 3), arr[0]->max));
    assert(tuple_equals(tuple_new_point(-1, 3, -3), arr[1]->min));
    assert(tuple_equals(tuple_new_point(5, 8, 3), arr[1]->max));
    bbox_delete_box(box);
    bbox_delete_box(arr[0]);
    bbox_delete_box(arr[1]);

    // z wide
    box = bbox_new_bbox(tuple_new_point(-1, -2, -3), tuple_new_point(5, 3, 7));
    bbox_split_bounds(box, arr);
    assert(tuple_equals(tuple_new_point(-1, -2, -3), arr[0]->min));
    assert(tuple_equals(tuple_new_point(5, 3, 2), arr[0]->max));
    assert(tuple_equals(tuple_new_point(-1, -2, 2), arr[1]->min));
    assert(tuple_equals(tuple_new_point(5, 3, 7), arr[1]->max));
    bbox_delete_box(box);
    bbox_delete_box(arr[0]);
    bbox_delete_box(arr[1]);

    group = group_new_group();
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_translation4(-2, 0, 0));
    group_add_object(group, s);
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_translation4(2, 0, 0));
    group_add_object(group, s);
    s = sphere_new_sphere();
    group_add_object(group, s);

    /* Passes
    assert(group->nb_children == 3);
    group_remove_object_at(group, 2);
    assert(group->nb_children == 2);
    group_remove_object_at(group, 0);
    assert(group->nb_children == 1);
    group_remove_object_at(group, 0);
    assert(group->nb_children == 0);
    */

    struct group* partition[2] = {0};
    group_make_bounding_box(group);
    group_partition_children(group, partition);

    assert(group->nb_children == 1);
    assert(partition[0]->nb_children == 1);
    assert(partition[1]->nb_children == 1);

    group_delete_group(partition[0]);
    group_delete_group(partition[1]);
    group_delete_group(group);

    struct group* sub_part = group_new_group();
    s = sphere_new_sphere();
    group_add_object(sub_part, s);
    s = sphere_new_sphere();
    group_add_object(sub_part, s);
    group = group_new_group();

    group_make_sub_group(group, sub_part);

    assert(group->nb_children == 1);
    assert(((struct group*) group->list_children[0])->type == GROUP);
    group_delete_group(group);

    //bbox_render_1();

    s = sphere_new_sphere();
    group = group_new_group();
    group_add_object(group, s);
    group_make_bounding_box(group);
    group_divide_group(group, GROUP_SUBDIV_THRESH);
    group_delete_group(group);

    group = group_new_group();
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_translation4(-2, -2, 0));
    group_add_object(group, s);
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_translation4(-2, 2, 0));
    group_add_object(group, s);
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_scaling4(4, 4, 4));
    group_add_object(group, s);
    group_make_bounding_box(group);

    group_divide_group(group, GROUP_SUBDIV_THRESH);
    group_delete_group(group);

    struct group* sub = group_new_group();
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_translation4(-2, 0, 0));
    group_add_object(sub, s);
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_translation4(2, 1, 0));
    group_add_object(sub, s);
    s = sphere_new_sphere();
    sphere_add_transform(s, matrix_new_translation4(2, -1, 0));
    group_add_object(sub, s);

    group = group_new_group();
    s = sphere_new_sphere();
    group_add_object(group, sub);
    group_add_object(group, s);
    group_make_bounding_box(group);

    group_divide_group(group, 3);
    group_delete_group(group);

    bbox_render_2();
    //bbox_render_3();

    return 0;
}

static void bbox_render_1(void) {

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

    // Main group
    struct group* final = group_new_group();
    struct group* g;
    struct group* g_sub;
    struct sphere* s;

    for (int k = 0; k < 10; k++) {
	g = group_new_group();
	for (int j = 0; j < 10; j++) {
	    g_sub = group_new_group();
	    for (int i = -5; i < 5; i++) {
		s = sphere_new_sphere();
		s->material.reflective = 0.5;
		s->material.color = NORD11;
		sphere_add_transform(s, matrix_new_translation4(2 * j, 0, 2 * i));
		group_add_object(g_sub, s);
	    }
	    group_add_object(g, g_sub);
	}
	group_add_transform(g, matrix_new_translation4(0, 8 - 2 * k, 0));
	group_add_object(final, g);
    }


    struct point_light light1 = lights_new_point_light(tuple_new_point(-50, 100, -50), tuple_new_color(1, 1, 1));
    struct world* world = world_new_world();
    world_add_point_light(world, &light1);
    //world_add_group(world, g);
    world_add_group(world, final);

    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/3);
    struct tuple from = tuple_new_point(-25, 10, -35);
    struct tuple to = tuple_new_point(5, 0, -5);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);
    camera_make_inv_view_transform(&camera);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}

static void bbox_render_2(void) {

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

    // Main group
    struct group* final = group_new_group();
    struct sphere* s;

    for (int k = 0; k < 10; k++) {
	for (int j = 0; j < 10; j++) {
	    for (int i = -5; i < 5; i++) {
		s = sphere_new_sphere();
		s->material.reflective = 0.5;
		s->material.color = NORD11;
		sphere_add_transform(s, matrix_new_translation4(2 * j, -10 + 2 * k, 2 * i));
		group_add_object(final, s);
	    }
	}
    }


    struct point_light light1 = lights_new_point_light(tuple_new_point(-50, 100, -50), tuple_new_color(1, 1, 1));
    struct world* world = world_new_world();
    world_add_point_light(world, &light1);
    world_add_group(world, final);

    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/3);
    struct tuple from = tuple_new_point(-25, 10, -35);
    struct tuple to = tuple_new_point(5, 0, -5);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);
    camera_make_inv_view_transform(&camera);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}

static void bbox_render_3(void) {

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

    // Main group
    struct sphere* s;

    struct point_light light1 = lights_new_point_light(tuple_new_point(-50, 100, -50), tuple_new_color(1, 1, 1));
    struct world* world = world_new_world();
    world_add_point_light(world, &light1);

    for (int k = 0; k < 10; k++) {
	for (int j = 0; j < 10; j++) {
	    for (int i = -5; i < 5; i++) {
		s = sphere_new_sphere();
		s->material.reflective = 0.5;
		s->material.color = NORD11;
		sphere_add_transform(s, matrix_new_translation4(2 * j, -10 + 2 * k, 2 * i));
		world_add_sphere(world, s);
	    }
	}
    }

    // Camera and render
    struct camera camera = camera_new_camera(CANVAS_WIDTH, CANVAS_HEIGHT, PI/3);
    struct tuple from = tuple_new_point(-25, 10, -35);
    struct tuple to = tuple_new_point(5, 0, -5);
    struct tuple up = tuple_new_vector(0, 1, 0);
    camera.transform = matrix_view_transform(from, to, up);
    camera_make_inv_view_transform(&camera);

    struct canvas canvas = camera_render(&camera, world);
    canvas_to_ppm(&canvas);

    world_free_world(world);
    canvas_free_canvas(&canvas);
}
