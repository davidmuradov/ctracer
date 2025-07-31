#include "../includes/sphere.h"
#include "ray.h"
#include "vec3.h"

int hit(struct sphere* sphere, struct ray* ray, float r_tmin, float r_tmax) {

    struct vec3 oc = vec3_sub(sphere->center, ray->o);
    float a = vec3_len_sqrd(ray->dir);
    float h = vec3_dot(ray->dir, oc);
    float c = vec3_len_sqrd(oc) - sphere->radius * sphere->radius;

    float delta = h * h - a * c;

    if (delta < 0)
	return 0;

    float sqrtd = sqrt(delta);
    float root = (h - sqrtd) / a;
    if (root <= r_tmin || r_tmax <= root) {
	root = (h + sqrtd) / a;
	if (root <= r_tmin || r_tmax <= root)
	    return 0;
    }

    sphere->t = root;
    sphere->p = ray_at(*ray, sphere->t);
    struct vec3 out_norm = vec3_mult(vec3_sub(sphere->p, sphere->center), sphere->radius);
    set_face_normal(sphere, ray, out_norm);
    //struct vec3 inter1 = vec3_sub(sphere->p, sphere->center);
    //sphere->normal = vec3_div(inter1, sphere->radius);

    return 1;
}

void set_face_normal(struct sphere* sph, struct ray* ray, struct vec3 outward_normal) {

    sph->front_face = (vec3_dot(ray->dir, outward_normal) < 0) ? (1):(0);
    sph->normal = (sph->front_face) ? (outward_normal):(vec3_mult(outward_normal, -1));
}
