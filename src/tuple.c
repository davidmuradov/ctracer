#include "../includes/tuple.h"
#include "ct_math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct tuple tuple_new_point(double x, double y, double z) {
    return (struct tuple) {x,y,z,1};
}

struct tuple tuple_new_vector(double x, double y, double z) {
    return (struct tuple) {x,y,z,0};
}

int tuple_equals(struct tuple a, struct tuple b) {
    if (ctm_floats_equal(a.x, b.x) &&
	ctm_floats_equal(a.y, b.y) &&
	ctm_floats_equal(a.z, b.z))
	return 1;
    return 0;
}

struct tuple tuple_add(struct tuple a, struct tuple b) {
    //if (a.w && b.w) {
    //    fprintf(stderr, "Error: Added two points together (ILLEGAL)\n");
    //    exit(1);
    //}

    return (struct tuple) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

struct tuple tuple_sub(struct tuple a, struct tuple b) {
    //if (!a.w && b.w) {
    //    fprintf(stderr, "Error: Subtracted point from vector (ILLEGAL)\n");
    //    exit(1);
    //}

    return (struct tuple) {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

struct tuple tuple_negate(struct tuple a) {
    return (struct tuple) {-a.x, -a.y, -a.z, -a.w};
}

struct tuple tuple_scalar_mult(struct tuple a, double b) {
    return (struct tuple) {b * a.x, b * a.y, b * a.z, a.w};
}

struct tuple tuple_scalar_div(struct tuple a, double b) {
    return (struct tuple) {a.x / b, a.y / b, a.z / b, a.w};
}

double tuple_mag(struct tuple a) {
    if (a.w) {
	fprintf(stderr, "Error: Taking magnitude of point (ILLEGAL)\n");
	exit(1);
    }

    return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

struct tuple tuple_normalize(struct tuple a) {
    if (a.w) {
	fprintf(stderr, "Error: Normalizing a point (ILLEGAL)\n");
	exit(1);
    }

    double mag = tuple_mag(a);
    return tuple_scalar_div(a, mag);
}

double tuple_dot(struct tuple a, struct tuple b) {
    if (a.w || b.w) {
	fprintf(stderr, "Error: Dot product on points (ILLEGAL)\n");
	exit(1);
    }

    return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

struct tuple tuple_cross(struct tuple a, struct tuple b) {
    if (a.w || b.w) {
	fprintf(stderr, "Error: Cross product on points (ILLEGAL)\n");
	exit(1);
    }

    struct tuple c;

    c.x = a.y * b.z - b.y * a.z;
    c.y = b.x * a.z - a.x * b.z;
    c.z = a.x * b.y - b.x * a.y;
    c.w = 0;

    return c;
}

void tuple_print(struct tuple a) {
    if (!a.w)
	printf("vector : [%.5f %.5f %.5f]\n", a.x, a.y, a.z);
    else if (a.w == 1)
	printf("point  : [%.5f %.5f %.5f]\n", a.x, a.y, a.z);
    else
	printf("color  : [%.5f %.5f %.5f]\n", a.x, a.y, a.z);
}

struct tuple tuple_new_color(double r, double g, double b) {
    return (struct tuple) {r, g, b, 2};
}

struct tuple tuple_color_mult(struct tuple a, struct tuple b) {
    return (struct tuple) {a.x * b.x, a.y * b.y, a.z * b.z, 2};
}
