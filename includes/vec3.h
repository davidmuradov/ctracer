#ifndef INCLUDES_VEC3_H
#define INCLUDES_VEC3_H

struct vec3 {
    float x, y, z;
};

struct vec3
vec3_add(struct vec3 a, struct vec3 b);

struct vec3
vec3_sub(struct vec3 a, struct vec3 b);

struct vec3
vec3_mult(struct vec3 a, float t);

struct vec3
vec3_div(struct vec3 a, float t);

float
vec3_len(struct vec3 a);

float
vec3_len_sqrd(struct vec3 a);

struct vec3
vec3_mult_vec3(struct vec3 a, struct vec3 b);

struct vec3
vec3_div_vec3(struct vec3 a, struct vec3 b);

float
vec3_dot(struct vec3 a, struct vec3 b);

struct vec3
vec3_cross(struct vec3 a, struct vec3 b);

struct vec3
vec3_unitv(struct vec3 a);

#endif
