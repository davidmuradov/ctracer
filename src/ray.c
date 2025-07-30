#include "../includes/ray.h"
#include "../includes/vec3.h"

void
ray_init(struct ray* r, struct vec3 o, struct vec3 dir) {

    r->o = o;
    r->dir = dir;
}

struct vec3
ray_at(struct ray r, float t) {

    return vec3_add(r.o, vec3_mult(r.dir, t));
}
