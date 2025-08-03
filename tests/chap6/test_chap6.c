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
#include "lights.h"
#include "materials.h"

static void chap5_render(void);
static void chap6_render(void);

int main(int argc, char *argv[]) {

    struct sphere s = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct tuple normal = sphere_normal_at(&s, tuple_new_point(1, 0, 0));
    assert(tuple_equals(normal, tuple_new_vector(1, 0, 0)));

    normal = sphere_normal_at(&s, tuple_new_point(0, 1, 0));
    assert(tuple_equals(normal, tuple_new_vector(0, 1, 0)));

    normal = sphere_normal_at(&s, tuple_new_point(0, 0, 1));
    assert(tuple_equals(normal, tuple_new_vector(0, 0, 1)));

    normal = sphere_normal_at(&s, tuple_new_point(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3));
    assert(tuple_equals(normal, tuple_new_vector(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3)));
    assert(tuple_equals(normal, tuple_normalize(normal)));

    struct matrix4 m_transform = matrix_new_translation4(0, 1, 0);
    sphere_set_transform(&s, m_transform);
    normal = sphere_normal_at(&s, tuple_new_point(0, 1.70711, -0.70711));
    assert(tuple_equals(normal, tuple_new_vector(0, 0.70711, -0.70711)));

    m_transform = matrix_mult_matrix4(matrix_new_scaling4(1, 0.5, 1), matrix_new_rotate_z(PI/5));
    sphere_set_transform(&s, m_transform);
    normal = sphere_normal_at(&s, tuple_new_point(0, sqrt(2)/2, -sqrt(2)/2));
    assert(tuple_equals(normal, tuple_new_vector(0, 0.97014, -0.24254)));

    struct tuple v = tuple_new_vector(1, -1, 0);
    struct tuple n = tuple_new_vector(0, 1, 0);
    struct tuple r = sphere_reflect(v, n);
    assert(tuple_equals(r, tuple_new_vector(1, 1, 0)));

    v = tuple_new_vector(0, -1, 0);
    n = tuple_new_vector(sqrt(2)/2, sqrt(2)/2, 0);
    r = sphere_reflect(v, n);
    assert(tuple_equals(r, tuple_new_vector(1, 0, 0)));

    struct point_light light = lights_new_point_light(tuple_new_point(0, 0, 0), tuple_new_color(1, 1, 1));
    assert(tuple_equals(light.position, tuple_new_point(0, 0, 0)));
    assert(tuple_equals(light.intensity, tuple_new_point(1, 1, 1)));

    struct material material = materials_new_material();
    assert(tuple_equals(material.color, tuple_new_color(1, 1, 1)));
    assert(ctm_floats_equal(material.ambient, 0.1));
    assert(ctm_floats_equal(material.diffuse, 0.9));
    assert(ctm_floats_equal(material.specular, 0.9));
    assert(ctm_floats_equal(material.shininess, 200));

    struct sphere s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    assert(tuple_equals(s2.material.color, tuple_new_color(1, 1, 1)));
    assert(ctm_floats_equal(s2.material.ambient, 0.1));
    assert(ctm_floats_equal(s2.material.diffuse, 0.9));
    assert(ctm_floats_equal(s2.material.specular, 0.9));
    assert(ctm_floats_equal(s2.material.shininess, 200));
    material.ambient = 1;
    sphere_set_material(&s2, material);
    assert(ctm_floats_equal(s2.material.ambient, 1));

    material = materials_new_material();
    struct tuple position = tuple_new_point(0, 0, 0);
    struct tuple eyev = tuple_new_vector(0, 0, -1);
    struct tuple normalv = tuple_new_vector(0, 0, -1);
    light = lights_new_point_light(tuple_new_point(0, 0, -10), tuple_new_color(1, 1, 1));
    struct tuple res = lights_lighting(material, position, light, eyev, normalv);
    assert(tuple_equals(res, tuple_new_color(1.9, 1.9, 1.9)));

    eyev = tuple_new_vector(0, sqrt(2)/2, -sqrt(2)/2);
    res = lights_lighting(material, position, light, eyev, normalv);
    assert(tuple_equals(res, tuple_new_color(1, 1, 1)));

    eyev = tuple_new_vector(0, 0, -1);
    light = lights_new_point_light(tuple_new_point(0, 10, -10), tuple_new_color(1, 1, 1));
    res = lights_lighting(material, position, light, eyev, normalv);
    assert(tuple_equals(res, tuple_new_color(0.7364, 0.7364, 0.7364)));

    eyev = tuple_new_vector(0, -sqrt(2)/2, -sqrt(2)/2);
    res = lights_lighting(material, position, light, eyev, normalv);
    assert(tuple_equals(res, tuple_new_color(1.6364, 1.6364, 1.6364)));

    eyev = tuple_new_vector(0, 0, -1);
    light = lights_new_point_light(tuple_new_point(0, 0, 10), tuple_new_color(1, 1, 1));
    res = lights_lighting(material, position, light, eyev, normalv);
    assert(tuple_equals(res, tuple_new_color(0.1, 0.1, 0.1)));

    chap6_render();

    return EXIT_SUCCESS;
}

static void chap5_render(void) {

    struct ray r = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    struct sphere s = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct canvas canvas = canvas_new();
    struct tuple bg = tuple_new_color(76./255, 86./255, 106./255);
    struct tuple fg = tuple_new_color(136./255, 192./255, 208./255);

    //struct tuple viewport_origin = tuple_new_point(0, 0, 10);
    double viewport_height = 9;
    double viewport_width = 16;
    struct tuple viewport_top_left = tuple_new_point(-viewport_width/2, viewport_height/2, 10);
    struct tuple viewport_u = tuple_new_vector(viewport_width, 0, 0);
    struct tuple viewport_v = tuple_new_vector(0, -viewport_height, 0);
    struct tuple viewport_u_del = tuple_scalar_div(viewport_u, CANVAS_WIDTH);
    struct tuple viewport_v_del = tuple_scalar_div(viewport_v, CANVAS_HEIGHT);
    struct tuple diag = tuple_scalar_mult(tuple_add(viewport_u_del, viewport_v_del), 0.5);
    struct tuple pixel00_loc = tuple_add(viewport_top_left, diag);

    struct tuple current_pixel;
    struct intersection_list inter_list;

    // Main render loop
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
	for (int j = 0; j < CANVAS_WIDTH; j++) {
	    current_pixel = tuple_add(pixel00_loc, tuple_add(tuple_scalar_mult(viewport_u_del, j),
			tuple_scalar_mult(viewport_v_del, i)));
	    r.dir = tuple_sub(current_pixel, r.o);
	    inter_list = sphere_intersect_ray(&s, &r);

	    if (inter_list.nb_intersections) {
		canvas_write_pixel(&canvas, i, j, fg);
	    }
	    else {
		canvas_write_pixel(&canvas, i, j, bg);
	    }

	    intersection_clear_intersection_list(&inter_list);
	}
    }

    // Write to ppm image
    canvas_to_ppm(&canvas);
    canvas_free_canvas(&canvas);
}

static void chap6_render(void) {
    struct ray r = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    struct sphere s = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    struct canvas canvas = canvas_new();
    struct tuple bg = tuple_new_color(0, 0, 0);
    struct tuple fg = tuple_new_color(136./255, 192./255, 208./255);
    s.material.color = fg;

    struct point_light light = lights_new_point_light(tuple_new_point(-10, 10, -10), tuple_new_color(1, 1, 1));

    //struct tuple viewport_origin = tuple_new_point(0, 0, 10);
    double viewport_height = 9;
    double viewport_width = 16;
    struct tuple viewport_top_left = tuple_new_point(-viewport_width/2, viewport_height/2, 10);
    struct tuple viewport_u = tuple_new_vector(viewport_width, 0, 0);
    struct tuple viewport_v = tuple_new_vector(0, -viewport_height, 0);
    struct tuple viewport_u_del = tuple_scalar_div(viewport_u, CANVAS_WIDTH);
    struct tuple viewport_v_del = tuple_scalar_div(viewport_v, CANVAS_HEIGHT);
    struct tuple diag = tuple_scalar_mult(tuple_add(viewport_u_del, viewport_v_del), 0.5);
    struct tuple pixel00_loc = tuple_add(viewport_top_left, diag);

    struct tuple current_pixel;
    struct intersection_list inter_list;

    struct tuple eye;
    struct tuple normal;
    struct tuple hit_color;
    struct tuple point;

    // Main render loop
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
	for (int j = 0; j < CANVAS_WIDTH; j++) {
	    current_pixel = tuple_add(pixel00_loc, tuple_add(tuple_scalar_mult(viewport_u_del, j),
			tuple_scalar_mult(viewport_v_del, i)));
	    r.dir = tuple_normalize(tuple_sub(current_pixel, r.o));
	    inter_list = sphere_intersect_ray(&s, &r);

	    if (inter_list.nb_intersections) {
		point = ray_position_at(&r, inter_list.list[0].t);
		struct sphere* s_ptr = (struct sphere*) inter_list.list[0].object;
		normal = sphere_normal_at(s_ptr, point);
		eye = tuple_scalar_mult(r.dir, -1);
		hit_color = lights_lighting(s_ptr->material, point, light, eye, normal);
		canvas_write_pixel(&canvas, i, j, hit_color);
	    }
	    else {
		canvas_write_pixel(&canvas, i, j, bg);
	    }

	    intersection_clear_intersection_list(&inter_list);
	}
    }

    // Write to ppm image
    canvas_to_ppm(&canvas);
    canvas_free_canvas(&canvas);
}
