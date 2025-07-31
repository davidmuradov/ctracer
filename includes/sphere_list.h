#ifndef INCLUDES_SPHERE_LIST_H
#define INCLUDES_SPHERE_LIST_H

#define INIT_SPHERE_LIST	32

#include "sphere.h"

struct sphere_list {
    struct sphere* list;
    int nb_spheres;
    int max_nb_spheres;
};

int sphere_list_init(struct sphere_list* sl);

void add_sphere(struct sphere_list* l, struct sphere s);

int sphere_list_hit(struct sphere_list* l, struct ray* r, float r_tmin, float r_tmax, struct sphere* s);

#endif
