#include "../includes/sphere_list.h"
#include "sphere.h"
#include <stdlib.h>

int sphere_list_init(struct sphere_list* sl) {

    sl->list = calloc(INIT_SPHERE_LIST, sizeof(struct sphere));
    if (!sl->list)
	return 0;

    sl->max_nb_spheres = INIT_SPHERE_LIST;
    sl->nb_spheres = 0;

    return 1;
}

void add_sphere(struct sphere_list* l, struct sphere s) {

    if (l->nb_spheres < l->max_nb_spheres) {
	l->list[l->nb_spheres].center = s.center;
	l->list[l->nb_spheres].radius = s.radius;
	l->nb_spheres++;
    }
    else {
	struct sphere* new_l = realloc(l->list, l->max_nb_spheres * 2);
	free(l->list);
	l->list = new_l;
	l->max_nb_spheres = l->max_nb_spheres * 2;
	l->list[l->nb_spheres].center = s.center;
	l->list[l->nb_spheres].radius = s.radius;
	l->nb_spheres++;
    }
}

int sphere_list_hit(struct sphere_list* l, struct ray* r, float r_tmin, float r_tmax, struct sphere* s) {

    struct sphere temp_sphere= {0};
    int hit_any = 0;
    float closest = r_tmax;

    // Iterate over every sphere of the list
    for (int i = 0; i < l->nb_spheres; i++) {
	if (hit(&temp_sphere, r, r_tmin, closest)) {
	    hit_any = 1;
	    //closest = 
	}
    }

    return hit_any;
}
