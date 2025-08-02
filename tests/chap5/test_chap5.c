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

int main(int argc, char *argv[]) {

    struct tuple origin = tuple_new_point(1, 2, 3);
    struct tuple direction = tuple_new_vector(4, 5, 6);
    struct ray r;
    ray_init(&r, origin, direction);
    assert(tuple_equals(r.o, origin));
    assert(tuple_equals(r.dir, direction));

    origin = tuple_new_point(2, 3, 4);
    direction = tuple_new_vector(1, 0, 0);
    ray_init(&r, origin, direction);
    struct tuple pos_exp_1 = tuple_new_point(2, 3, 4);
    struct tuple pos_exp_2 = tuple_new_point(3, 3, 4);
    struct tuple pos_exp_3 = tuple_new_point(1, 3, 4);
    struct tuple pos_exp_4 = tuple_new_point(4.5, 3, 4);
    assert(tuple_equals(ray_position_at(&r, 0), pos_exp_1));
    assert(tuple_equals(ray_position_at(&r, 1), pos_exp_2));
    assert(tuple_equals(ray_position_at(&r, -1), pos_exp_3));
    assert(tuple_equals(ray_position_at(&r, 2.5), pos_exp_4));

    struct sphere s;
    struct tuple s_origin = tuple_new_point(0, 0, 0);
    double s_radius = 1;
    sphere_init(&s, s_origin, s_radius);

    struct intersection_list inter_list2;
    intersection_new_intersection_list(&inter_list2);
    struct tuple r_origin = tuple_new_point(0, 0, -5);
    struct tuple r_dir = tuple_new_vector(0, 0, 1);
    ray_init(&r, r_origin, r_dir);
    sphere_intersect_ray(&s, &r, &inter_list2);
    assert(inter_list2.nb_intersections == 2);
    assert(ctm_floats_equal(4, inter_list2.list[0].t));
    assert(ctm_floats_equal(6, inter_list2.list[1].t));

    intersection_clear_intersection_list(&inter_list2);

    ray_init(&r, (struct tuple) {0,2,-5,1}, r_dir);
    sphere_intersect_ray(&s, &r, &inter_list2);
    assert(inter_list2.nb_intersections == 0);

    intersection_clear_intersection_list(&inter_list2);

    ray_init(&r, (struct tuple) {0,0,0,1}, r_dir);
    sphere_intersect_ray(&s, &r, &inter_list2);
    assert(inter_list2.nb_intersections == 2);
    assert(ctm_floats_equal(-1, inter_list2.list[0].t));
    assert(ctm_floats_equal(1, inter_list2.list[1].t));

    intersection_clear_intersection_list(&inter_list2);

    ray_init(&r, (struct tuple) {0,0,5,1}, r_dir);
    sphere_intersect_ray(&s, &r, &inter_list2);
    assert(inter_list2.nb_intersections == 2);
    assert(ctm_floats_equal(-6, inter_list2.list[0].t));
    assert(ctm_floats_equal(-4, inter_list2.list[1].t));

    intersection_clear_intersection_list(&inter_list2);

    struct intersection inter;
    intersection_new_intersection(&inter, 3.5, &s);
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
    intersection_new_intersection(&inter, 1, &s);
    struct intersection inter2;
    intersection_new_intersection(&inter2, 2, &s);
    struct intersection_list inter_list;
    intersection_new_intersection_list(&inter_list);
    intersection_add_intersection_to_list(&inter_list, inter);
    intersection_add_intersection_to_list(&inter_list, inter2);
    assert(inter_list.nb_intersections == 2);
    assert(ctm_floats_equal(inter_list.list[0].t, 1));
    assert(ctm_floats_equal(inter_list.list[1].t, 2));

    ray_init(&r, (struct tuple) {0,0,-5,1}, r_dir);
    sphere_intersect_ray(&s, &r, &inter_list2);
    assert(inter_list2.nb_intersections == 2);
    assert(SPHERE == intersection_get_object_type(&inter_list2.list[0]));
    assert(SPHERE == intersection_get_object_type(&inter_list2.list[1]));
    assert(&s == (struct sphere*) inter_list2.list[0].object);
    assert(&s == (struct sphere*) inter_list2.list[1].object);

    intersection_clear_intersection_list(&inter_list2);
    struct intersection inter_res;
    intersection_new_intersection(&inter, 1, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    intersection_new_intersection(&inter, 2, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    inter_res = intersection_hit(&inter_list2);
    assert(ctm_floats_equal(inter_res.t, 1));

    intersection_clear_intersection_list(&inter_list2);
    intersection_new_intersection(&inter, -1, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    intersection_new_intersection(&inter, 1, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    inter_res = intersection_hit(&inter_list2);
    assert(ctm_floats_equal(inter_res.t, 1));

    intersection_clear_intersection_list(&inter_list2);
    intersection_new_intersection(&inter, -2, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    intersection_new_intersection(&inter, -1, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    inter_res = intersection_hit(&inter_list2);
    assert(ctm_floats_equal(inter_res.t, UNDEF_TIME) && inter_res.object == NULL);

    intersection_clear_intersection_list(&inter_list2);
    intersection_new_intersection(&inter, 5, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    intersection_new_intersection(&inter, 7, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    intersection_new_intersection(&inter, -3, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    intersection_new_intersection(&inter, 2, &s);
    intersection_add_intersection_to_list(&inter_list2, inter);
    inter_res = intersection_hit(&inter_list2);
    assert(ctm_floats_equal(inter_res.t, 2));

    ray_init(&r, tuple_new_point(1, 2, 3), tuple_new_vector(0, 1, 0));
    matrix4 m_translate;
    matrix_new_translation4(3, 4, 5, m_translate);
    struct ray r2 = ray_transform_ray(r, m_translate);
    assert(tuple_equals(r2.o, tuple_new_point(4, 6, 8)));
    assert(tuple_equals(r2.dir, tuple_new_point(0, 1, 0)));

    matrix4 m_scale;
    matrix_new_scaling4(2, 3, 4, m_scale);
    r2 = ray_transform_ray(r, m_scale);
    assert(tuple_equals(r2.o, tuple_new_point(2, 6, 12)));
    assert(tuple_equals(r2.dir, tuple_new_point(0, 3, 0)));

    struct sphere s2;
    matrix4 m_identity;
    matrix_make_identity4(m_identity);
    sphere_init(&s2, tuple_new_point(0, 0, 0), 1);
    assert(matrix_compare_matrix4(m_identity, s2.default_transformation));
    
    matrix4 mt2;
    matrix_new_translation4(2, 3, 4, mt2);
    sphere_set_transform(&s2, mt2);
    assert(matrix_compare_matrix4(mt2, s2.default_transformation));

    return EXIT_SUCCESS;
}
