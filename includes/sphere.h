#ifndef INCLUDES_SPHERE_H
#define INCLUDES_SPHERE_H

#include "vec3.h"
#include "ray.h"
#include <math.h>

struct sphere {
    struct vec3 center;
    float radius;
    // hit record
    struct vec3 p;
    struct vec3 normal;
    float t;
    // Front face
    int front_face;
};

int hit(struct sphere* sphere, struct ray* ray, float r_tmin, float r_tmax);

void set_face_normal(struct sphere* sph, struct ray* ray, struct vec3 outward_normal);

#endif
