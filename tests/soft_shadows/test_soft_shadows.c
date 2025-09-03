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

    struct world* w = world_new_default_world();
    struct point_light l = lights_new_point_light(tuple_new_point(-10, -10, -10), tuple_new_color(1, 1, 1));
    world_add_point_light(w, &l);
    struct tuple point = tuple_new_point(-10, -10, 10);
    assert(!world_is_shadowed(w, l.position, point));
    point = tuple_new_point(10, 10, 10);
    assert(world_is_shadowed(w, l.position, point));
    point = tuple_new_point(-20, -20, -20);
    assert(!world_is_shadowed(w, l.position, point));
    point = tuple_new_point(-5, -5, -5);
    assert(!world_is_shadowed(w, l.position, point));

    l.position = tuple_new_point(-10, 10, -10);
    point = tuple_new_point(0, 1.0001, 0);
    assert(ctm_floats_equal(1, world_intensity_at(&l, point, w)));
    point = tuple_new_point(-1.0001, 0, 0);
    assert(ctm_floats_equal(1, world_intensity_at(&l, point, w)));
    point = tuple_new_point(0, 0, -1.0001);
    assert(ctm_floats_equal(1, world_intensity_at(&l, point, w)));
    point = tuple_new_point(0, 0, 1.0001);
    assert(ctm_floats_equal(0, world_intensity_at(&l, point, w)));
    point = tuple_new_point(1.0001, 0, 0);
    assert(ctm_floats_equal(0, world_intensity_at(&l, point, w)));
    point = tuple_new_point(0, -1.0001, 0);
    assert(ctm_floats_equal(0, world_intensity_at(&l, point, w)));
    point = tuple_new_point(0, 0, 0);
    assert(ctm_floats_equal(0, world_intensity_at(&l, point, w)));
    l.position = tuple_new_point(0, 0, -10);
    struct sphere* s = world_get_object_at(w, 0);
    s->material.ambient = 0.1;
    s->material.diffuse = 0.9;
    s->material.specular = 0;
    s->material.color = tuple_new_color(1, 1, 1);
    point = tuple_new_point(0, 0, -1);
    struct tuple eyev = tuple_new_vector(0, 0, -1);
    struct tuple normalv = tuple_new_vector(0, 0, -1);
    struct tuple result = lights_lighting_sphere(s->material, s, l, point, eyev, normalv, 1);
    assert(tuple_equals(result, tuple_new_color(1, 1, 1)));
    result = lights_lighting_sphere(s->material, s, l, point, eyev, normalv, 0.5);
    assert(tuple_equals(result, tuple_new_color(0.55, 0.55, 0.55)));
    result = lights_lighting_sphere(s->material, s, l, point, eyev, normalv, 0);
    assert(tuple_equals(result, tuple_new_color(0.1, 0.1, 0.1)));
    world_free_world(w);

    struct tuple corner = tuple_new_vector(0, 0, 0);
    struct tuple v1 = tuple_new_vector(2, 0, 0);
    struct tuple v2 = tuple_new_vector(0, 0, 1);
    struct area_light_rect rect_light = lights_new_area_light_rect(corner, v1, 4, v2, 2, tuple_new_color(1, 1, 1));
    assert(tuple_equals(corner, rect_light.corner));
    assert(tuple_equals(tuple_new_vector(0.5, 0, 0), rect_light.uvec));
    assert(rect_light.usteps = 4);
    assert(tuple_equals(tuple_new_vector(0, 0, 0.5), rect_light.vvec));
    assert(rect_light.vsteps = 2);
    assert(rect_light.samples = 8);
    assert(tuple_equals(tuple_new_color(1, 1, 1), rect_light.intensity));
    assert(tuple_equals(tuple_new_color(1, 0, 0.5), rect_light.position));

    point = lights_point_on_area_light_rect(&rect_light, 0, 0);
    assert(tuple_equals(point, tuple_new_point(0.25, 0, 0.25)));
    point = lights_point_on_area_light_rect(&rect_light, 1, 0);
    assert(tuple_equals(point, tuple_new_point(0.75, 0, 0.25)));
    point = lights_point_on_area_light_rect(&rect_light, 0, 1);
    assert(tuple_equals(point, tuple_new_point(0.25, 0, 0.75)));
    point = lights_point_on_area_light_rect(&rect_light, 2, 0);
    assert(tuple_equals(point, tuple_new_point(1.25, 0, 0.25)));
    point = lights_point_on_area_light_rect(&rect_light, 3, 1);
    assert(tuple_equals(point, tuple_new_point(1.75, 0, 0.75)));

    w = world_new_default_world();
    corner = tuple_new_point(-0.5, -0.5, -5);
    v1 = tuple_new_vector(1, 0, 0);
    v2 = tuple_new_vector(0, 1, 0);
    rect_light = lights_new_area_light_rect(corner, v1, 2, v2, 2, tuple_new_color(1, 1, 1));
    point = tuple_new_point(0, 0, 2);
    world_add_area_light_rect(w, &rect_light);
    double intensity = world_intensity_at(&rect_light, point, w);
    assert(ctm_floats_equal(0, intensity));
    point = tuple_new_point(1, -1, 2);
    intensity = world_intensity_at(&rect_light, point, w);
    assert(ctm_floats_equal(0.25, intensity));
    point = tuple_new_point(1.5, 0, 2);
    intensity = world_intensity_at(&rect_light, point, w);
    assert(ctm_floats_equal(0.5, intensity));
    point = tuple_new_point(1.25, 1.25, 3);
    intensity = world_intensity_at(&rect_light, point, w);
    assert(ctm_floats_equal(0.75, intensity));
    point = tuple_new_point(0, 0, -2);
    intensity = world_intensity_at(&rect_light, point, w);
    assert(ctm_floats_equal(1, intensity));

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
