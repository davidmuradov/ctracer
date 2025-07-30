#include "../includes/vec3.h"
#include <math.h>

struct vec3
vec3_add(struct vec3 a, struct vec3 b) {

    struct vec3 c = {0};

    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;

    return c;
}

struct vec3
vec3_sub(struct vec3 a, struct vec3 b) {

    struct vec3 c = {0};

    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;

    return c;
}

struct vec3
vec3_mult(struct vec3 a, float t) {

    struct vec3 c = {0};

    c.x = a.x * t;
    c.y = a.y * t;
    c.z = a.z * t;

    return c;
}

struct vec3
vec3_div(struct vec3 a, float t) {

    struct vec3 c = {0};

    if (!t) {

	c.x = 255;
	c.y = 255;
	c.z = 255;
	return c;
    }

    c.x = a.x / t;
    c.y = a.y / t;
    c.z = a.z / t;

    return c;
}

float
vec3_len(struct vec3 a) {

    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float
vec3_len_sqrd(struct vec3 a) {

    return a.x * a.x + a.y * a.y + a.z * a.z;
}

struct vec3
vec3_mult_vec3(struct vec3 a, struct vec3 b) {

    struct vec3 c = {0};

    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;

    return c;
}

struct vec3
vec3_div_vec3(struct vec3 a, struct vec3 b) {

    struct vec3 c = {0};

    if (!b.x || !b.y || !b.z) {

	c.x = 255;
	c.y = 255;
	c.z = 255;
	return c;
    }

    c.x = a.x / b.x;
    c.y = a.y / b.y;
    c.z = a.z / b.z;

    return c;
}

float
vec3_dot(struct vec3 a, struct vec3 b) {

    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct vec3
vec3_cross(struct vec3 a, struct vec3 b) {

    struct vec3 c = {0};

    c.x = a.y * b.z - b.y * a.z;
    c.y = a.x * b.z - b.x * a.z;
    c.z = a.x * b.y - b.x * a.y;

    return c;
}

struct vec3
vec3_unitv(struct vec3 a) {

    return vec3_div(a, vec3_len(a));
}
