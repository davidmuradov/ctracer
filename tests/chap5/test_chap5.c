#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/tuple.h"
#include "../../includes/ct_math.h"
#include "../../includes/canvas.h"
#include "../../includes/matrix.h"
#include "../../includes/ray.h"
#include "intersection.h"
#include "sphere.h"

static void chap5_render(void);

int main(int argc, char *argv[]) {

    struct tuple origin = tuple_new_point(1, 2, 3);
    struct tuple direction = tuple_new_vector(4, 5, 6);
    struct ray r = ray_new_ray(origin, direction);
    assert(tuple_equals(r.o, origin));
    assert(tuple_equals(r.dir, direction));

    origin = tuple_new_point(2, 3, 4);
    direction = tuple_new_vector(1, 0, 0);
    r = ray_new_ray(origin, direction);
    struct tuple pos_exp_1 = tuple_new_point(2, 3, 4);
    struct tuple pos_exp_2 = tuple_new_point(3, 3, 4);
    struct tuple pos_exp_3 = tuple_new_point(1, 3, 4);
    struct tuple pos_exp_4 = tuple_new_point(4.5, 3, 4);
    assert(tuple_equals(ray_position_at(&r, 0), pos_exp_1));
    assert(tuple_equals(ray_position_at(&r, 1), pos_exp_2));
    assert(tuple_equals(ray_position_at(&r, -1), pos_exp_3));
    assert(tuple_equals(ray_position_at(&r, 2.5), pos_exp_4));

    struct tuple s_origin = tuple_new_point(0, 0, 0);
    double s_radius = 1;
    struct sphere s = sphere_new_sphere(s_origin, s_radius);

    struct tuple r_origin = tuple_new_point(0, 0, -5);
    struct tuple r_dir = tuple_new_vector(0, 0, 1);
    r = ray_new_ray(r_origin, r_dir);
    struct intersection_list inter_list = sphere_intersect_ray(&s, &r);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(4, inter_list.list[0].t));
    assert(ctm_floats_equal(6, inter_list.list[1].t));

    intersection_clear_intersection_list(&inter_list);

    r = ray_new_ray((struct tuple) {0,2,-5,1}, r_dir);
    inter_list = sphere_intersect_ray(&s, &r);
    assert(inter_list.nb_intersections == 0);

    intersection_clear_intersection_list(&inter_list);

    r = ray_new_ray((struct tuple) {0,0,0,1}, r_dir);
    inter_list = sphere_intersect_ray(&s, &r);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(-1, inter_list.list[0].t));
    assert(ctm_floats_equal(1, inter_list.list[1].t));

    intersection_clear_intersection_list(&inter_list);

    r = ray_new_ray((struct tuple) {0,0,5,1}, r_dir);
    inter_list = sphere_intersect_ray(&s, &r);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(-6, inter_list.list[0].t));
    assert(ctm_floats_equal(-4, inter_list.list[1].t));

    intersection_clear_intersection_list(&inter_list);

    struct intersection inter;
    inter = intersection_new_intersection(3.5, &s);
    assert(ctm_floats_equal(3.5, inter.t));
    assert(SPHERE == intersection_get_object_type(&inter));
    assert(&s == (struct sphere*) inter.object);
    struct sphere* s_rec = (struct sphere*) inter.object;
    assert(s_rec->id == s.id);
    assert(s_rec->type == s.type);
    assert(ctm_floats_equal(s_rec->radius, s.radius));
    assert(tuple_equals(s_rec->o, s.o));
    assert(s_rec->xs_count == s.xs_count);
    assert(ctm_floats_equal(s_rec->xs[0], s.xs[0]));
    assert(ctm_floats_equal(s_rec->xs[1], s.xs[1]));
    inter = intersection_new_intersection(1, &s);
    struct intersection inter2;
    inter2 = intersection_new_intersection(2, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    intersection_add_intersection_to_list(&inter_list, inter2);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 1));
    assert(ctm_floats_equal(inter_list.list[1].t, 2));

    intersection_clear_intersection_list(&inter_list);

    r = ray_new_ray((struct tuple) {0,0,-5,1}, r_dir);
    inter_list = sphere_intersect_ray(&s, &r);
    assert(inter_list.nb_intersections == 2);
    assert(SPHERE == intersection_get_object_type(&inter_list.list[0]));
    assert(SPHERE == intersection_get_object_type(&inter_list.list[1]));
    assert(&s == (struct sphere*) inter_list.list[0].object);
    assert(&s == (struct sphere*) inter_list.list[1].object);

    intersection_clear_intersection_list(&inter_list);
    struct intersection inter_res;
    inter = intersection_new_intersection(1, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(2, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter_res = intersection_hit(&inter_list);
    assert(ctm_floats_equal(inter_res.t, 1));

    intersection_clear_intersection_list(&inter_list);
    inter = intersection_new_intersection(-1, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(1, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter_res = intersection_hit(&inter_list);
    assert(ctm_floats_equal(inter_res.t, 1));

    intersection_clear_intersection_list(&inter_list);
    inter = intersection_new_intersection(-2, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(-1, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter_res = intersection_hit(&inter_list);
    assert(ctm_floats_equal(inter_res.t, UNDEF_TIME) && inter_res.object == NULL);

    intersection_clear_intersection_list(&inter_list);
    inter = intersection_new_intersection(5, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(7, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(-3, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter = intersection_new_intersection(2, &s);
    intersection_add_intersection_to_list(&inter_list, inter);
    inter_res = intersection_hit(&inter_list);
    assert(ctm_floats_equal(inter_res.t, 2));

    intersection_clear_intersection_list(&inter_list);

    r = ray_new_ray(tuple_new_point(1, 2, 3), tuple_new_vector(0, 1, 0));
    struct matrix4 m_translate = matrix_new_translation4(3, 4, 5);
    struct ray r2 = ray_transform_ray(r, m_translate);
    assert(tuple_equals(r2.o, tuple_new_point(4, 6, 8)));
    assert(tuple_equals(r2.dir, tuple_new_point(0, 1, 0)));

    struct matrix4 m_scale = matrix_new_scaling4(2, 3, 4);
    r2 = ray_transform_ray(r, m_scale);
    assert(tuple_equals(r2.o, tuple_new_point(2, 6, 12)));
    assert(tuple_equals(r2.dir, tuple_new_point(0, 3, 0)));

    struct sphere s2;
    struct matrix4 m_identity = matrix_make_identity4();
    s2 = sphere_new_sphere(tuple_new_point(0, 0, 0), 1);
    assert(matrix_compare_matrix4(m_identity, s2.default_transformation));
    
    struct matrix4 mt2 = matrix_new_translation4(2, 3, 4);
    sphere_set_transform(&s2, mt2);
    assert(matrix_compare_matrix4(mt2, s2.default_transformation));


    r = ray_new_ray(tuple_new_point(0, 0, -5), tuple_new_vector(0, 0, 1));
    mt2 = matrix_new_scaling4(2, 2, 2);
    sphere_set_transform(&s2, mt2);
    inter_list = sphere_intersect_ray(&s2, &r);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 3));
    assert(ctm_floats_equal(inter_list.list[1].t, 7));

    intersection_clear_intersection_list(&inter_list);

    sphere_set_transform(&s2, matrix_new_translation4(5, 0, 0));
    inter_list = sphere_intersect_ray(&s2, &r);
    assert(inter_list.nb_intersections == 0);

    intersection_clear_intersection_list(&inter_list);

    chap5_render();

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
