#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/tuple.h"
#include "../../includes/ct_math.h"

int main(int argc, char *argv[]) {

    struct tuple v1 = {4,-4,3,1};
    assert(v1.x == 4);
    assert(v1.y == -4);
    assert(v1.z == 3);
    assert(v1.w == 1);

    struct tuple v2 = {1,-1,2,0};
    assert(v2.x == 1);
    assert(v2.y == -1);
    assert(v2.z == 2);
    assert(v2.w == 0);

    struct tuple v3 = tuple_new_point(0, -2, 2);
    assert(v3.x == 0);
    assert(v3.y == -2);
    assert(v3.z == 2);
    assert(v3.w == 1);

    struct tuple v4 = tuple_new_vector(5, -5, 10);
    assert(v4.x == 5);
    assert(v4.y == -5);
    assert(v4.z == 10);
    assert(v4.w == 0);

    double a = 3.555559;
    double b = 3.555557;
    assert(ctm_floats_equal(a, b));
    assert(ctm_floats_equal(b, a));
    b = 3.55555;
    assert(ctm_floats_equal(a, b));
    assert(ctm_floats_equal(b, a));
    b = 3.44449;
    assert(!ctm_floats_equal(a, b));
    assert(!ctm_floats_equal(b, a));
    b = 3.55551;
    assert(!ctm_floats_equal(a, b));
    assert(!ctm_floats_equal(b, a));

    struct tuple v5 = tuple_new_vector(2.33333, -4.11119, 0);
    struct tuple v6 = tuple_new_point(2.333338, -4.111191, 0);
    assert(tuple_equals(v5, v6));
    v6.z = 0.0001;
    assert(!tuple_equals(v5, v6));

    struct tuple v7 = tuple_new_vector(1, 0, -1);
    struct tuple v8 = tuple_new_vector(-1, 0, 1);
    struct tuple v9 = tuple_add(v7, v8);
    struct tuple vres = tuple_new_vector(0, 0, 0);
    assert(tuple_equals(v9, vres));
    v1 = tuple_new_point(3, -2, 5);
    v2 = tuple_new_vector(-2, 3, 1);
    v3 = tuple_add(v1, v2);
    assert(v3.x == 1 && v3.y == 1 && v3.z == 6 && v3.w == 1);

    v4 = tuple_new_point(3, 2, 1);
    v5 = tuple_new_vector(5, 6, 7);
    v6 = tuple_sub(v4, v5);
    assert(v6.x == -2 && v6.y == -4 && v6.z == -6 && v6.w == 1);
    v7 = tuple_new_vector(3, 2, 1);
    v8 = tuple_new_vector(5, 6, 7);
    v9 = tuple_sub(v7, v8);
    assert(v9.x == -2 && v9.y == -4 && v9.z == -6 && v9.w == 0);

    v1 = (struct tuple) {1,-2,3,-4};
    v2 = tuple_negate(v1);
    assert(v2.x == -1 && v2.y == 2 && v2.z == -3 && v2.w == 4);

    v3 = (struct tuple) {1,-2,3,-4};
    v4 = tuple_scalar_mult(v3, 3.5);
    v5 = (struct tuple) {3.5,-7,10.5,-14};
    assert(tuple_equals(v4, v5));

    v6 = (struct tuple) {1,-2,3,-4};
    v7 = tuple_scalar_div(v3, 2);
    v8 = (struct tuple) {0.5,-1,1.5,-2};
    assert(tuple_equals(v7, v8));

    v9 = (struct tuple) {1,0,0,0};
    double res = tuple_mag(v9);
    assert(ctm_floats_equal(res, 1));
    v9 = (struct tuple) {0,1,0,0};
    res = tuple_mag(v9);
    assert(ctm_floats_equal(res, 1));
    v9 = (struct tuple) {0,0,1,0};
    res = tuple_mag(v9);
    assert(ctm_floats_equal(res, 1));
    v9 = (struct tuple) {1,2,3,0};
    res = tuple_mag(v9);
    assert(ctm_floats_equal(res, sqrt(14)));
    v9 = (struct tuple) {-1,-2,-3,0};
    res = tuple_mag(v9);
    assert(ctm_floats_equal(res, sqrt(14)));

    v1 = (struct tuple) {4,0,0,0};
    v2 = tuple_normalize(v1);
    v3 = (struct tuple) {1,0,0,0};
    assert(tuple_equals(v2, v3));
    v1 = (struct tuple) {1,0,0,0};
    v2 = tuple_normalize(v1);
    v3 = (struct tuple) {1,0,0,0};
    assert(tuple_equals(v2, v3));
    v1 = (struct tuple) {1,2,3,0};
    v2 = tuple_normalize(v1);
    v3 = (struct tuple) {1./sqrt(14),2./sqrt(14),3./sqrt(14),0};
    assert(tuple_equals(v2, v3));

    v4 = tuple_new_vector(1, 2, 3);
    v5 = tuple_new_vector(2, 3, 4);
    res = 20;
    assert(tuple_dot(v4, v5) == res);

    v6 = tuple_new_vector(1, 2, 3);
    v7 = tuple_new_vector(2, 3, 4);
    v8 = tuple_cross(v6, v7);
    v9 = tuple_cross(v7, v6);
    v1 = (struct tuple) {-1,2,-1,0};
    v2 = (struct tuple) {1,-2,1,0};
    assert(tuple_equals(v8, v1));
    assert(tuple_equals(v9, v2));

    v1 = tuple_new_point(2, 3, 4);
    //tuple_print(v6);
    //tuple_print(v1);

    return EXIT_SUCCESS;
}
